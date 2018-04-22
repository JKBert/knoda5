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
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.55 $

#include "hk_kdereport.h"
#include "hk_kdesimplereport.h"
#include "hk_kdereportsectiondialog.h"

#include <hk_database.h>
#include <hk_connection.h>
#include <hk_datasource.h>

#include <qbuttongroup.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qevent.h>

#include <kmenubar.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kglobal.h>

#include <kconfig.h>
#include <kconfiggroup.h>
#include <kapplication.h>
#include <kactioncollection.h>
#include <kservice.h>

hk_kdereport::hk_kdereport (QWidget* parent,  const char* /* name */, Qt::WFlags fl )
: KParts::MainWindow( parent, fl ) ,hk_class()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereport::hk_kdereport");
#endif
    p_partmanager=new KParts::PartManager(this);
    setObjectName("hk_kdereport");
    KSharedConfigPtr c=KGlobal::config();
    const QRect& rrect=QRect(0,0,800,600);    
    KConfigGroup cg = c->group("Report-SDI");
    QRect g;
    QIcon::setThemeName("oxygen");
    
    g=cg.readEntry("Geometry",rrect);
    setGeometry(g);
    
    setXMLFile("hk_kdereport.rc");
    
    KService::Ptr service = KService::serviceByDesktopName("hk_kde5reportpart");
    if (!service ||
      !(p_part=service->createInstance<KParts::ReadWritePart>(this, this, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Report part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now...")); 
     exit(1);
    } 
    p_partmanager->addPart(p_part);
    p_report = (hk_kdereportpartwidget*)p_part->widget();
    setCentralWidget(p_report);
    connect(p_partmanager,SIGNAL(partRemoved(KParts::Part*)),this, SLOT(part_removed()));
    connect(p_partmanager,SIGNAL(activePartChanged(KParts::Part*)),this, SLOT(createGUI(KParts::Part*)));
    p_closeaction=new KAction(QIcon::fromTheme("window-close"),i18n("&Close"),actionCollection());
    actionCollection()->addAction("closereport",p_closeaction);
    connect(p_closeaction,SIGNAL(triggered()),this,SLOT(close_report()));
    createGUI(p_part);
    setAutoSaveSettings("Report-SDI");
}


hk_kdereport::~hk_kdereport(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereport::~hk_kdereport");
#endif
 if (p_report) delete p_report;
 p_report=NULL;
 delete p_partmanager;
}

bool    hk_kdereport::set_presentationdatasource(long n, bool r)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereport::set_presentationdatasource(n)");
#endif
    bool res=true;
    if (r) res= p_report->set_presentationdatasource(n,false);
    return res;
}

long    hk_kdereport::presentationdatasource(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereport::presentationdatasource(n)");
#endif
    return p_report->presentationdatasource();
}

void hk_kdereport::set_database(hk_database* db)
{
    if (!p_report) return;
    p_report->set_database(db);
    set_caption();
}

hk_database* hk_kdereport::database(void)
{
    if (!p_report) return NULL;
    return p_report->database();
}

void hk_kdereport::closeEvent ( QCloseEvent* e)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereport::closeEvent");
#endif
    if (p_report&&p_report->while_executing())
    {
      e->ignore();
      return;
    }  
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Report-SDI");
  cg.writeEntry("Geometry",geometry());
  emit signal_closed(this);
  KParts::MainWindow::closeEvent(e);
}

void hk_kdereport::set_nodesignmode(bool d)
{
    p_report->set_nodesignmode(d);
}

void    hk_kdereport::set_mode(hk_presentation::enum_mode s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereport::set_mode(s)");
#endif
    p_report->set_mode(s);
}

void hk_kdereport::set_designmode(void)
{
    set_mode(hk_presentation::designmode);
}

void hk_kdereport::set_viewmode(void)
{
    set_mode(hk_presentation::viewmode);
}

hk_kdesimplereport* hk_kdereport::simplereport(void)
{
    return p_report->simplereport();
}

void    hk_kdereport::save_report(void)
{
    p_report->simplereport()->save_report("",false);
    set_caption();
}

void hk_kdereport::saveas_report(void)
{
    p_report->saveas_report();
}

bool hk_kdereport::load_report(const hk_string& name)
{
    bool res=p_report->load_report(name);
    set_caption();
    return res;
}

void hk_kdereport::close_report(void)
{
    if (p_report->while_executing())
       p_report->stop_execution();
    close();
}

void hk_kdereport::slot_showtoolbar(void)
{

}

void hk_kdereport::part_removed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereport::part_removed");
#endif   
   p_part=NULL;
   p_report=NULL;
   close();
}

void hk_kdereport::set_caption(void)
{
    QString reportname=i18n( "Report - "  );
    hk_database* db=database();
    reportname+=QString::fromUtf8 (l2u(p_report->simplereport()->hk_presentation::name()).c_str());
    reportname+=" (";
    hk_string driver;
	if (db) driver=db->name();
        reportname+=QString::fromUtf8(l2u(driver).c_str());
        reportname+=")";

    setWindowTitle( reportname  );
    emit signal_setcaption(reportname);
}
