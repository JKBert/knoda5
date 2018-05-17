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
//$Revision: 1.64 $

#include "hk_kdequery.h"
#include "hk_kdequery.moc"
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <klocale.h>
#include <hk_actionquery.h>
#include <hk_connection.h>
#include "hk_kdegrid.h"
#include "hk_kdeqbe.h"
#include <locale.h>

#include <kmenubar.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <kservice.h>

/*
 *  Constructs a hk_kdequery which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */

hk_kdequery::hk_kdequery(QWidget* w,const char* /* n */,Qt::WFlags f):KParts::MainWindow(w,f),hk_dsquery()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdequery::hk_kdequery");
    hkclassname("hk_kdequery");
#endif
    resize( 596, 480 );
    p_partmanager=new KParts::PartManager(this);
    QIcon::setThemeName("oxygen");
    QPixmap p;
    setXMLFile("hk_kdequery.rc");

    KService::Ptr service = KService::serviceByDesktopName("hk_kde5querypart");   
    if (!service || 
      !(p_part=service->createInstance<KParts::ReadWritePart>(this,this, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Query part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
    }
    p_partmanager->addPart(p_part);
    p_query = (hk_kdequerypartwidget*)p_part->widget();

    setCentralWidget(p_query);
    connect(p_partmanager,SIGNAL(partRemoved(KParts::Part*)),this, SLOT(part_removed()));
    connect(p_partmanager,SIGNAL(activePartChanged(KParts::Part*)),this, SLOT(createGUI(KParts::Part*)));
    p_closeaction=new KAction(QIcon::fromTheme("window-close"),i18n("&Close"),actionCollection());
    actionCollection()->addAction("closequery",p_closeaction);
    connect(p_closeaction,SIGNAL(triggered()),this,SLOT(close_query()));
    set_mode(designmode);
    createGUI(p_part);
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdequery::~hk_kdequery()
{
// no need to delete child widgets, Qt does it all for us
#ifdef HK_DEBUG
  hkdebug("hk_kdequery::~hk_kdequery");
#endif
  setCentralWidget(NULL);
  if (p_query) delete p_query;
  p_query=NULL;
  delete p_partmanager;
#ifdef HK_DEBUG
  hkdebug("hk_kdequery::~hk_kdequery END");
#endif

}

bool  hk_kdequery::set_mode(enum_mode s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_mode");
#endif
 if (!p_query) return false;
return p_query->set_mode(s);
}

void hk_kdequery::set_nodesignmode(bool d)
{
 if (!p_query) return;
p_query->set_nodesignmode(d);

}


bool hk_kdequery::save_query(const hk_string& n,bool ask)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::save_query");
#endif

    set_caption();
 if (!p_query) return false;

 return p_query->save_query(n,ask);
}


bool hk_kdequery::load_query(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::load_query");
#endif
 if (!p_query) return false;
 return p_query->load_query();
}


void hk_kdequery::close_query(void)
{
    close();
}

void hk_kdequery::closeEvent ( QCloseEvent* e)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::closeEvent");
#endif
    if (p_query&&p_query->has_changed())
    {
        save_query();
        reset_has_changed();

    }
    KParts::MainWindow::closeEvent(e);
   emit signal_closed(this);
}

void hk_kdequery::set_caption(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_caption");
#endif
    hk_datasource* d=datasource();
    if (d!=NULL)
    {
        QString n=((d->type()==hk_datasource::ds_view)? i18n("View - "):i18n("Query - "));

        n+=QString::fromUtf8(l2u(d->name()).c_str());
        n+=" (";
        hk_string driver=d->database()->name();
        n+=QString::fromUtf8(l2u(driver).c_str());
        n+=")";
	QString name=QString::fromUtf8(l2u(d->database()->connection()->drivername()).c_str())+" "+n;
        setWindowTitle(name);
        emit signal_captionChanged(this, name);
 }

}

bool hk_kdequery::in_designmode(void) const
{
if (!p_query) return false;
return p_query->in_designmode();
}

void hk_kdequery::part_removed(void)
{
   cerr <<"hk_kdequery::part_removed"<<endl;

   p_part=NULL;
   p_query=NULL;
   close();
}

void hk_kdequery::set_datasource(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_datasource");
#endif
 if (!p_query) return;
    hk_dsquery::set_datasource(d);
    p_query->set_datasource(d);
}

void hk_kdequery::set_autoclose(bool c)
{
 if (!p_query) return;
  p_query->set_autoclose(c);
}

void hk_kdequery::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdequery::before_source_vanishes");
#endif

 if (!p_query) return;
    if (p_query->autoclose())
        close();
    else
    {
        hk_datasource* d=datasource();
        if (d!=NULL)d->disable();
        set_datasource(NULL);

    }
    //hk_dsquery::before_source_vanishes();
}
