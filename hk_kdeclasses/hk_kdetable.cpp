// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for hk_kde5classes library
//
// This file is part of the hk_kde5classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#include "hk_kdetable.h"
#include <hk_datasource.h>
#include <hk_database.h>
#include <hk_connection.h>
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kmenubar.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kservice.h>

/*
 *  Constructs a hk_kdetable which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */

hk_kdetable::hk_kdetable(QWidget* w,const char* /* n */,Qt::WFlags f):KParts::MainWindow(w, f),hk_dstable()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetable::hk_kdetable");
    hkclassname("hk_kdetable");
#endif
    setObjectName( "hk_kdetable" );
    resize( 596, 480 );
    p_partmanager=new KParts::PartManager(this);
    setXMLFile("hk_kdetable.rc");
    QIcon::setThemeName("oxygen");
    KService::Ptr service = KService::serviceByDesktopName("hk_kde5tablepart");
    if (!service || 
      !(p_part=service->createInstance<KParts::ReadWritePart>(this,this, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Table part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now...")); 
     exit(1);
    } 
    p_partmanager->addPart(p_part);
    p_table = (hk_kdetablepartwidget*)p_part->widget();
    setCentralWidget(p_table);
    connect(p_partmanager,SIGNAL(partRemoved(KParts::Part*)),this, SLOT(part_removed()));
    p_closeaction=new KAction(QIcon::fromTheme("window-close"),i18n("&Close"),actionCollection());
    actionCollection()->addAction("closetable",p_closeaction);
    connect(p_closeaction,SIGNAL(triggered()),this,SLOT(close_table()));
    createGUI(p_part);
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdetable::~hk_kdetable()
{
// no need to delete child widgets, Qt does it all for us
  if (p_table) delete p_table;
  p_table=NULL;
  delete p_partmanager;
}

void hk_kdetable::set_datasource(hk_datasource* d)
{
  if (!p_table) return;
    p_table->set_datasource(d);
    set_caption();
}

bool  hk_kdetable::set_mode(enum_mode s)
{
  if (!p_table) return false;
  return p_table->set_mode(s);
}

void hk_kdetable::closeEvent ( QCloseEvent* e)
{
    QWidget::closeEvent(e);
   emit signal_closed(this);
}

void hk_kdetable::close_table(void)
{
    close();
}

void hk_kdetable::set_caption(void)
{
    hk_datasource* d=datasource();
    if (d!=NULL)
    {
        
	QString n=i18n("Table - ");
        n+=QString::fromUtf8(l2u(d->name()).c_str());
        n+=" (";
        hk_string driver=d->database()->name();
        n+=QString::fromUtf8(l2u(driver).c_str());
        n+=")";
        setWindowTitle(QString::fromUtf8(l2u(d->database()->connection()->drivername()).c_str())+" "+n);
    }
}

void hk_kdetable::part_removed(void)
{
   cerr <<"hk_kdetable::part_removed"<<endl;
   
   p_part=NULL;
   p_table=NULL;
   close();
}

void hk_kdetable::set_autoclose(bool a)
{
  if (!p_table) return;
  p_table->set_autoclose(a);
}

bool hk_kdetable::in_designmode(void)
{
  if (!p_table) return false;
  return p_table->in_designmode();
}
