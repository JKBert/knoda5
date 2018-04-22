// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
//
// This file is part of the hk_kde4classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided ASp IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.1 $

#include "hk_kdemodule.h"
#include "hk_kdemodule.moc"

#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <kstatusbar.h>
#include <qstatusbar.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kglobal.h>
#include <qpushbutton.h>

#include <hk_class.h>
#include <hk_database.h>

#include <kmenubar.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <kapplication.h>
#include <kactioncollection.h>
#include <kservice.h>

/*
 *  Constructs a hk_kdemodule which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
hk_kdemodule::hk_kdemodule( QWidget* parent,  const char* name, Qt::WFlags fl )
: KParts::MainWindow( parent, fl ) ,hk_class()
{
#ifdef HK_DEBUG
    hkclassname("Hauptmodul");
    hkdebug("hk_kdemodule::hk_kdemodule");
#endif
    if (name)
        setObjectName(QString::fromAscii(name));
    p_partmanager=new KParts::PartManager(this);

    QString n="Form-SDI";
    KSharedConfigPtr c=KGlobal::config();
    KConfigGroup cg = c->group(n);
    QRect rect(0,0,800,600);
    const QRect& rrect = QRect(0,0,800,600);
    QRect g;
    g=cg.readEntry("Geometry",rrect);
    setGeometry(g);

    QIcon::setThemeName("oxygen");
    setXMLFile("hk_kdemodule.rc");

    KService::Ptr service = KService::serviceByDesktopName("hk_kde5modulepart");
    
    if (!service || 
      !(p_part=service->createInstance<KParts::ReadWritePart>(this,this, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Form part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
    }
    p_partmanager->addPart(p_part);
    p_module = (hk_kdemodulepartwidget*)p_part->widget();
    setCentralWidget(p_module);
    connect(p_partmanager,SIGNAL(partRemoved(KParts::Part*)),this, SLOT(part_removed()));
    connect(p_partmanager,SIGNAL(activePartChanged(KParts::Part*)),this, SLOT(createGUI(KParts::Part*)));
    p_closeaction=new KAction(QIcon::fromTheme("window-close"),i18n("&Close"),actionCollection());
    actionCollection()->addAction("closemodule",p_closeaction);
    connect(p_closeaction,SIGNAL(triggered()),this,SLOT(close_module()));
    createGUI(p_part);
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdemodule::~hk_kdemodule()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::~hk_kdemodule");
#endif
 if (p_module) delete p_module;
 p_module=NULL;
 delete p_partmanager;
}

void hk_kdemodule::part_removed(void)
{
   cerr <<"hk_kdemodule::part_removed"<<endl;

   p_part=NULL;
   p_module=NULL;
   close();


}

void    hk_kdemodule::set_database(hk_database* db)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::database(db)");
#endif
    if (!p_module) return;

    p_module->set_database(db);
}


hk_database*        hk_kdemodule::database(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::database");
#endif
    if (!p_module) return NULL;
    return p_module->database();
}



void hk_kdemodule::closeEvent ( QCloseEvent* e)
{

#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::closeEvent");
//set_generaldebug(true);
#endif
  KSharedConfigPtr c=KGlobal::config();
  QString n="Form-SDI";
  KConfigGroup cg = c->group(n);
  cg.writeEntry("Geometry",geometry());

  emit signal_closed(this);

  QWidget::closeEvent(e);
}







void hk_kdemodule::savedata(ofstream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::savedata(s)");
#endif
    if (!p_module) return;
    p_module->savedata(s);

}


void hk_kdemodule::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::loaddata");
#endif
    if (!p_module) return;
    p_module->loaddata(definition);
}


void hk_kdemodule::load_module(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::load_module");
#endif
    if (!p_module) return;

    if (database()==NULL)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Module error: No database defined!"));
        return ;

    }
//  p_module->modulename(n);
    p_module->load_module(n);
    set_caption();
}


void hk_kdemodule::set_autoclose(bool c)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::set_autoclose");
#endif
    if (!p_module) return;
    p_module->set_autoclose(c);
}


void hk_kdemodule::save_module_when_changed(void)
{

   if (p_module &&p_module->has_changed())
    {
        p_module->save_module();
    }

}


void    hk_kdemodule::mousePressEvent(QMouseEvent*m)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::mousePressEvent");
#endif
    QWidget::mousePressEvent(m);

}










void hk_kdemodule::set_caption(void)
{

    if (!p_module) return;

    QString modulename=i18n( "Module - "  );
     modulename+=QString::fromUtf8 (l2u(p_module->hk_module::name()).c_str());
    setWindowTitle( modulename  );

}


void hk_kdemodule::close_module(void)
{

    close();
}




