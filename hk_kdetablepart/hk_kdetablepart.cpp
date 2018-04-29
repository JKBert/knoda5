// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4tablepart library
//
// This file is part of the hk_kde5tablepart library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.6 $

//***********************************************
//***  hk_kdetable PART definition             ***
//***********************************************

#include "hk_kdetablepart.h"
#include "../hk_kdeclasses/hk_kdetablepartwidget.h"
#include <KAboutData>
#include <kcomponentdata.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kaction.h>
#include <klocale.h>

#include <qapplication.h>
#include <qclipboard.h>
#include <qcombobox.h>
#include <hk_drivermanager.h>
#include <hk_connection.h>
#include <hk_database.h>
#include <hk_datasource.h>
#include "hk_kdegrid.h"
#include "hk_kdetabledesign.h"
#include "hk_kdetoolbar.h"

K_PLUGIN_FACTORY_DEFINITION(hk_kdetablepartfactory, registerPlugin<hk_kdetablepart>();)

class hk_kdetablepartprivate
{
  public:
    hk_kdetablepartprivate(hk_kdetablepartwidget* ptable = NULL):p_table(ptable)
  	{
	}
    static KAboutData* p_aData;
    static KAboutData& getAboutData();
    hk_kdetablepartwidget* p_table;
};

KAboutData* hk_kdetablepartprivate::p_aData = NULL; 

KAboutData& hk_kdetablepartprivate::getAboutData()
{
    if ( p_aData == NULL) {
        p_aData = new KAboutData("hk_kde5classes", ki18n("hk_kde5tablepart").toString(),
            "0.2", ki18n("database table editor").toString(),
            KAboutLicense::GPL,
            ki18n("(c) 2002-2004, Horst Knorr\n(c) 2010-2018 Patrik Hanak").toString(),QString(), 
            "http://sourceforge.net/projects/knoda5/",
            "knoda4-bugs@lists.sourceforge.net");
        p_aData->addAuthor(ki18n("Horst Knorr").toString(),ki18n("Author of original version").toString(),
            "hk_classes@knoda.org","http://www.knoda.org");
        p_aData->addAuthor(ki18n("Patrik Hanak").toString(),ki18n("Author of KDE5 port").toString(),
            "knoda4-admins@lists.sourceforge.net");        
    }
    return *p_aData; 
}

hk_kdetablepart::hk_kdetablepart(QWidget* pWidget, QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent),p_private(new hk_kdetablepartprivate())
{
    setObjectName("hk_kdetablepart");
    setComponentData(hk_kdetablepartprivate::getAboutData());
    p_private->p_table = new hk_kdetablepartwidget(pWidget,0);
    p_private->p_table->setAttribute(Qt::WA_DeleteOnClose);
    setWidget(p_private->p_table);
    //TBP KIconLoader* loader=KIconLoader::global();
    //TBP loader->addAppDir("hk_kde5classes");
    setXMLFile("hk_kdetablepart.rc");
    p_private->p_table-> setupActions(actionCollection());
}

hk_kdetablepart::~hk_kdetablepart()
{
  if (widget())
  {
    p_private->p_table->p_design->alter_table(true);
  }
  
  delete p_private;
}



/*void hk_kdetablepart::show_dbdesignercolumndialog(void)
{
    p_private->p_table->simpledbdesigner()->show_dbdesignercolumndialog();
}*/


void hk_kdetablepart::setReadWrite(bool rw)
{
    KParts::ReadWritePart::setReadWrite(rw);
}


bool hk_kdetablepart::openFile()
{
 // URL handling:   mysql:/user:password@host:port/databasename/datasourcetype/datasourcename
 // where datasourcetype is either tables or queries


    return true;
}


bool hk_kdetablepart::saveFile()
{
    return true;
}

/*TBP
KAboutData* hk_kdetablepart::createAboutData()
{
    KAboutData* a= new KAboutData("hk_kde5tablepart", "hk_kde5tablepart",ki18n("hk_kde5tablepart"),
        "0.2", ki18n("database table editor"),
        KAboutData::License_GPL,
        ki18n("(c) 2002-2004, Horst Knorr\n(c) 2010-2018 Patrik Hanak"),ki18n(NULL), "http://sourceforge.net/projects/knoda5/",
     "knoda4-bugs@lists.sourceforge.net");
    a->addAuthor(ki18n("Horst Knorr"),ki18n("Author of original version"), "hk_classes@knoda.org","http://www.knoda.org");
    a->addAuthor(ki18n("Patrik Hanak"),ki18n("Author of KDE5 port"), "knoda4-admins@lists.sourceforge.net");    


    return a;

} */






