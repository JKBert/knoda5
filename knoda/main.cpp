// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for knoda -Knorr's Datenbank.
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for knoda4 -Knorr's Datenbank.
//
// This file is part of the knoda4 -Knorr's Datenbank.
// This file may be distributed and/or modified under the terms of the
// GNU General Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#include <hk_classes.h>
#include <kapplication.h>
#include <k4aboutdata.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <qdialog.h>
#include <qlistwidget.h>
#include "hk_kdemessages.h"
#include "knodamaindockwindow.h"
#include "hk_kdedriverselect.h"
#include <KLocalizedString>
#include <kconfig.h>
#include <kmimetype.h>
#include <kdebug.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

static KCmdLineOptions options;


int main(int argc,char** argv)
{
    umask(0077);
    K4AboutData aboutData("knoda5","knoda5",ki18n("knoda5"),
        VERSION, ki18n("knoda5 is a database management system"),
	K4AboutData::License_GPL,
        ki18n("(c) 2000-2005 Horst Knorr\n(c) 2010-2018 Patrik Hanak"),ki18n(NULL),"http://sourceforge.net/projects/knoda5/",
     "knoda4-bugs@lists.sourceforge.net");
    aboutData.addAuthor(ki18n("Horst Knorr"),ki18n("Author of original version"), "hk_classes@knoda.org","http://www.knoda.org");
    aboutData.addAuthor(ki18n("Patrik Hanak"),ki18n("Author of KDE5 port"), "knoda4-admins@lists.sourceforge.net");
    aboutData.addCredit (ki18n("Anirban Biswas"),ki18n("Postgresql driver"), "utpal@cal2.vsnl.net.in",NULL);
    aboutData.addCredit (ki18n("Tilman Linneweh"),ki18n("FreeBSD port"), "tilman@arved.de",NULL);

    QString database;
    QString form;
    hk_string driver;
    KCmdLineArgs::init(argc,argv,&aboutData);
    options.add("d <driver>", ki18n("Specifies the database driver"),0);
    options.add("b <database>", ki18n("Specifies a database"), 0 );
    options.add("f <form>", ki18n("Specifies a form (only in combination with -b)"), 0 );
    options.add("runtime", ki18n("No design mode"), 0 );
    options.add("strongruntime",ki18n("No design mode and no database control window"),0);
    options.add("classic", ki18n("SDI look"), 0 );
    options.add("print-sql", ki18n("prints the SQL statements to the standard ouput"), 0);
#ifdef HK_DEBUG
    options.add("debug", ki18n("debug"), 0 );
#endif
    options.add("+[database]",ki18n("local database file (SQLite)"),0);
    
    KCmdLineArgs::addCmdLineOptions(options);
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    KApplication anwendung;
    struct_commands commands;
    KLocalizedString::setApplicationDomain("knoda5");
    anwendung.setWindowIcon(QIcon::fromTheme(QStringLiteral("knoda5")));
    bool d =args->isSet("d");
    bool db = args->isSet("b");
    bool f  = args->isSet("f");
    bool classic= args->isSet("classic");
    bool nolistwindow=args->isSet("strongruntime");
    bool runtime=args->isSet("runtime");
    bool printsql=args->isSet("print-sql");
    
    if (db) database=args->getOption("b");
    if (f) form=args->getOption("f");
    if (d)  driver = args->getOption("d").toStdString();
    if (printsql)  hk_data::set_print_sqlstatements(true);
    if (args->count()>0 && !db)
      {
        db=true;
	database=args->arg(0);      }
    KMimeType::Ptr ptr = KMimeType::findByUrl(database);
    QString mimename = ptr.data()->name(); //cerr <<"mimetype="<<mimename<<endl;
     if (mimename=="application/octet-stream" && !database.isEmpty())
      {
        ptr=KMimeType::findByFileContent(database);
	mimename = ptr.data()->name();
      }
//    cerr <<"used mimetype="<<mimename<<endl;

   if (mimename=="application/x-hk_connection")
   {
   commands.p_load_connection=true;
   driver="X";
   }


    if (driver=="")driver=mimetype2driver(u2l(mimename.toUtf8().data()));
    d=driver.size()>0;

    if (!d&&(db||f))
    {
        cerr << i18n("Please specifiy a database driver!").toStdString() <<endl;
        return -1;

    }
    if (f&&!db)
    {
        cerr <<i18n("Specifying a form only works in combination with specifying a database").toStdString()<<endl;
        return -1;

    }


#ifdef HK_DEBUG
    if (args->isSet("debug")) hk_class::set_generaldebug(true);
#endif
    set_kdestandarddialogs();
    struct_commandlinefields cp;
    cp.driver=driver;
    cp.database=u2l(database.toUtf8().data());
    cp.form=u2l(form.toUtf8().data());
    cp.runtime_only=runtime;
    if (nolistwindow)cp.runtime_only=true;
    commands.p_showlistwindow=!nolistwindow;
    commands.p_classic=classic;
    cp.p_guicommands=&commands;
    int result=0;
    if (classic)
      cerr << i18n("classic user interface is not supported anymore.").toStdString() <<endl;
    
    hk_drivermanager dr;// needed as a dummy to initialize the driverlist path
    if (driver.size()==0)
    {
      hk_kdedriverselect* select=new hk_kdedriverselect();
      if (select->exec()==QDialog::Accepted)
      {
        if (select->driver_selected())
	  cp.driver=u2l(select -> currentDriver().toUtf8().data());
	else
	  {
            commands.p_load_connection=true;
            cp.driver="X";
            cp.database=u2l(select->selected_file().toUtf8().data());
	   }
  	} else result=select->driverlist->count();
	delete select;
      
    }
    if (result>0) return 1;
    knodamaindockwindow* w=new knodamaindockwindow(&cp);
    w->show();
    result=anwendung.exec();
    KGlobal::config()->sync();

    return  result;
}
