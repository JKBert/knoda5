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

#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#error config.h is needed but not included 
#endif
#include "hk_kdetablepartwidget.h"
#include <hk_datasource.h>
#include <hk_database.h>
#include <hk_connection.h>
#include "hk_kdetabledesign.h"
#include "hk_kdegrid.h"
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <kapplication.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qprogressdialog.h>
#include "hk_kdetoolbar.h"
#include "../hk_kdetablepart/hk_kdetablepart.h"
#include <kmenubar.h>
#include <kaction.h>
#include <ktoggleaction.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kactioncollection.h>
#include <KService>
#include <KIconLoader>
#include <KIconEngine>

bool    hk_kdetablepartwidget::p_cancel=false;
QProgressDialog* hk_kdetablepartwidget::p_progressdialog=NULL;
/*
 *  Constructs a hk_kdetablepartwidget which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */

hk_kdetablepartwidget::hk_kdetablepartwidget(QWidget* w,const char* n,Qt::WFlags f):QStackedWidget(w),hk_dstable()
{
#ifdef HK_DEBUG
    hkclassname("hk_kdetablepartwidget");
    //wanna_debug(true);
    hkdebug("hk_kdetablepartwidget::hk_kdetablepartwidget");
#endif
    setObjectName(n);
    setWindowFlags(windowFlags() | f);
    resize( 596, 480 );
    setFocusPolicy(Qt::StrongFocus);
    p_designaction=NULL;
    p_viewaction=NULL;
    p_printaction=NULL;
    p_filterdefinitionaction=NULL;
    p_filterexecaction=NULL;
    p_findaction=NULL;
    p_copyaction=NULL;
    p_pasteaction=NULL;
    p_columndialogaction=NULL;
    p_reloadaction=NULL;
    p_saveaction=NULL;
    p_autoclose=true;
    KService::Ptr service = KService::serviceByDesktopName("hk_kde5gridpart");
    
    if ( !service || 
      !(p_gridpart=service->createInstance<KParts::ReadWritePart>(this,this, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Grid part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
    }
    p_grid = (hk_kdegrid*)p_gridpart->widget();
    p_grid->set_enablingbehaviour(true,true);
    p_design = new hk_kdetabledesign(this,0);
    p_design->setAttribute(Qt::WA_DeleteOnClose);
    addWidget(p_design);
    addWidget(p_grid);
    setCurrentWidget(p_design);
    p_toolbar = new hk_kdetoolbar( this, "p_toolbar" );
    p_toolbar->set_nodesignmode(runtime_only());
    internal_designbutton_clicked();
    connect (p_design,SIGNAL(signal_has_changed()),this,SLOT(slot_has_changed()));
    connect (p_design,SIGNAL(signal_tablename_changed()),this,SLOT(set_caption()));
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdetablepartwidget::~hk_kdetablepartwidget()
{
  #ifdef HK_DEBUG
    hkdebug("hk_kdetablepartwidget::destructor");
#endif
// no need to delete child widgets, Qt does it all for us
    p_design->alter_table();
    hk_datasource* d=p_grid->datasource();
    if (d!=NULL)
    {
        if (d->name().size()>0)p_grid->save_table(d->name(),false);
        if (!d->presentation())d->disable();
    }
    delete p_design; 
}

void hk_kdetablepartwidget::setupActions(KActionCollection* ac)
{
    KIconLoader loader (LIB_MODULE_NAME);
	QIcon::setThemeName("oxygen");
	
    p_printaction=new KAction(QIcon::fromTheme("document-print"),i18n("&Print"), ac);
    ac-> addAction("print",p_printaction);
    connect(p_printaction,SIGNAL(triggered()),kdegrid(),SLOT(print_grid()));     
    p_printaction->setEnabled(false);
    
    if (runtime_only()) {
      p_designaction=NULL;
      p_viewaction=NULL;
      p_saveaction=NULL;     
    }
    else {
      p_designaction=new KToggleAction(QIcon::fromTheme("document-edit"),i18n("&Design mode"),ac);
      ac-> addAction("designmode",p_designaction);
      connect(p_designaction,SIGNAL(triggered()),this,SLOT(designbutton_clicked()));         
      p_designaction->setEnabled(!runtime_only());
      
      p_viewaction=new KToggleAction(QIcon::fromTheme("system-run"),i18n("&View mode"),ac);
      ac-> addAction("viewmode",p_viewaction);
      connect(p_viewaction,SIGNAL(triggered()),this,SLOT(tablebutton_clicked())); 
      
      QActionGroup* pGroup = new QActionGroup(this);
      p_designaction->setActionGroup(pGroup);
      p_viewaction->setActionGroup(pGroup); 
      
      p_saveaction=new KAction(QIcon::fromTheme("document-save"),i18n("Save"),ac);
      ac-> addAction("save",p_saveaction);
      connect(p_saveaction,SIGNAL(triggered()),this,SLOT(save_table()));       
      p_saveaction->setEnabled(false);
    } 
   
  p_reloadaction=new KAction(QIcon::fromTheme("view-refresh"),i18n("Reload"),ac);
  ac-> addAction("reload",p_reloadaction);
  connect(p_reloadaction,SIGNAL(triggered()),this,SLOT(reload_table()));    
    
  p_filterdefinitionaction=new KToggleAction(QIcon(loader.iconPath("filter",KIconLoader::User)),i18n("Filterdefinition"),ac);
  ac-> addAction("filterdefinition",p_filterdefinitionaction);
  connect(p_filterdefinitionaction,SIGNAL(triggered()),toolbar(),SLOT(filterdefinebutton_clicked()));    

  p_filterexecaction=new KToggleAction(QIcon(loader.iconPath("filterexec",KIconLoader::User)),i18n("Filterexecution"),ac);
  ac-> addAction("filterexec",p_filterexecaction);
  connect(p_filterexecaction,SIGNAL(triggered()),toolbar(),SLOT(filterexecbutton_clicked())); 
  
  toolbar()->set_filteractions(p_filterdefinitionaction,p_filterexecaction);
      
  p_columndialogaction = new KAction(QIcon(loader.iconPath("grid22x22",KIconLoader::User)),i18n("&Gridcolumns"),ac);
  ac-> addAction("gridcolumn",p_columndialogaction);
  connect(p_columndialogaction,SIGNAL(triggered()),(const QObject *) kdegrid(),SLOT(show_gridcolumndialog())); 
  p_columndialogaction->setEnabled(!hk_class::runtime_only());
    
  p_copyaction = new KAction(QIcon::fromTheme("edit-copy"),i18n("&Copy"),ac);
  p_copyaction -> setShortcut(Qt::CTRL+Qt::Key_C);
  ac-> addAction("copy",p_copyaction);
  connect(p_copyaction,SIGNAL(triggered()),(const QObject *) kdegrid()->simplegrid(),SLOT(copy()));
  
  p_pasteaction = new KAction(QIcon::fromTheme("edit-paste"),i18n("&Paste"),ac);
  p_pasteaction -> setShortcut(Qt::CTRL+Qt::Key_V);
  ac-> addAction("paste",p_pasteaction);
  connect(p_pasteaction,SIGNAL(triggered()),(const QObject *) kdegrid()->simplegrid(),SLOT(paste()));

  p_findaction = new KAction(QIcon(loader.iconPath("find",KIconLoader::User)),i18n("&Find in columns"),ac);
  ac-> addAction("findcolumn",p_findaction);
  connect(p_findaction,SIGNAL(triggered()),kdegrid(),SLOT(find_clicked()));
}

void hk_kdetablepartwidget::set_datasource(hk_datasource* d)
{
    hk_dsmodevisible::set_datasource(d);
    p_grid->set_datasource(d);
    p_design->set_datasource(d);
    p_toolbar->set_datasource(d); 
    if (d!=NULL)
    {
      p_grid->load_table();
      p_toolbar->set_filter(QString::fromUtf8(l2u(d->temporaryfilter()).c_str()));
    }
    p_grid->set_font(hk_font());
    set_caption();
}


hk_datasource* hk_kdetablepartwidget::datasource(void)
{

    return hk_dsmodevisible::datasource();
}


void hk_kdetablepartwidget::internal_designbutton_clicked(void)
{
    if (runtime_only())
    {
        set_mode(viewmode);
        return;
    }
    hk_datasource* d=p_grid->datasource();
    if (d!=NULL)
    {
        if (d->is_enabled())d->store_changed_data();
        d->disable();
    } 

    p_design->set_designmode();
    setCurrentWidget(p_design);
    p_design->setFocus();
    p_toolbar->set_designmode();
    if (p_designaction)
      p_designaction->setChecked(true);
    if (p_printaction)
     p_printaction->setEnabled(false);
    if (p_reloadaction)
     p_reloadaction->setEnabled(false);
//   p_filterdefinitionaction->setEnabled(false);
    set_caption();
    if (p_columndialogaction)
    p_columndialogaction->setEnabled(false);
    setFocusProxy(p_design);
}


void hk_kdetablepartwidget::internal_tablebutton_clicked(void)
{
   if (!p_design->alter_table())
        {
//p_toolbar->set_designmode();
            set_mode(designmode);
            return;
        }

    hk_datasource* d=p_grid->datasource();
    if (d!=NULL)
      {
      d->set_progressdialog(&hk_kdetablepartwidget::set_progress);
      before_execute();
       if (!d->enable())
        {
         after_execute();
         hk_string reason=hk_translate("Table could not be executed")+"\n"+hk_translate("Servermessage: ")+d->database()->connection()->last_servermessage();
//          show_warningmessage(reason);
        }
        else
         after_execute(); 
      }
    setCurrentWidget(p_grid); 

    if (p_printaction)p_printaction->setEnabled(true);
    
    p_toolbar->set_viewmode();
    if (p_viewaction)p_viewaction->setChecked(true);
    if (p_filterdefinitionaction)p_filterdefinitionaction->setEnabled(true);
    if (p_reloadaction)
     p_reloadaction->setEnabled(true);
    set_caption();
    if (p_columndialogaction)
    p_columndialogaction->setEnabled(true);
    setFocusProxy(p_grid);
}




void hk_kdetablepartwidget::tablebutton_clicked()
{
  set_mode(viewmode);
}


void hk_kdetablepartwidget::designbutton_clicked()
{
  set_mode(designmode);
}


bool  hk_kdetablepartwidget::set_mode(enum_mode s)
{
    hk_dsmodevisible::set_mode(s);
    switch (s) {
      case hk_dsmodevisible::designmode :
      case hk_dsmodevisible::filtermode :
	internal_designbutton_clicked();
	break;
      case hk_dsmodevisible::viewmode :
	internal_tablebutton_clicked();
	break;
    }
  slot_has_changed();
  return true;
}

void hk_kdetablepartwidget::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetablepartwidget::before_source_vanishes");
#endif
    if (datasource()!=NULL && datasource()->name().size()>0)
        p_grid->save_table(datasource()->name(),false);
    
    if (p_autoclose)
        close();
    else {
        hk_datasource* d=datasource();
        if (d!=NULL)d->disable();
        if (p_grid!=NULL) p_grid->set_datasource(NULL);
    }
}


void hk_kdetablepartwidget::closeEvent ( QCloseEvent* e)
{
    alter_table();
    QWidget::closeEvent(e);
   emit signal_closed(this);
}


void hk_kdetablepartwidget::alter_table(void)
{
   if ( mode()==hk_presentation::designmode)
     p_design->alter_table();
      slot_has_changed();
}


void hk_kdetablepartwidget::close_table(void)
{
    close();
}


void hk_kdetablepartwidget::set_caption(void)
{
    hk_datasource* d=datasource();
    
    if (d!=NULL)
    {

	QString n=i18n("Table - ");
        if (d->name().size()>0)
	  n+=QString::fromUtf8(l2u(d->name()).c_str());
	else n+=i18n("Unnamed");
	emit signal_captionChanged(this, n);
    }

}


hk_dsgrid* hk_kdetablepartwidget::grid() const
{
return p_grid;
}

hk_kdegrid* hk_kdetablepartwidget::kdegrid() const
{
return p_grid;
}

hk_kdetoolbar* hk_kdetablepartwidget::toolbar() const
{
return p_toolbar;
}

KParts::ReadWritePart* hk_kdetablepartwidget::gridpart(void) const
{
 return p_gridpart;
}

bool hk_kdetablepartwidget::in_designmode(void)
{
  return mode()==designmode;
}


void hk_kdetablepartwidget::save_table(void)
{
  p_design->alterbutton_clicked();
  slot_has_changed();

}


void hk_kdetablepartwidget::slot_has_changed(void)
{
 if (p_saveaction)
 {
   p_saveaction->setEnabled(mode()==designmode && p_design->has_changed());
 }
}


void hk_kdetablepartwidget::show_widget(void)
{
  show();
}

void hk_kdetablepartwidget::hide_widget(void)
{
  hide();
}

bool hk_kdetablepartwidget::close_widget(void)
{
  return close();
}

void    hk_kdetablepartwidget::before_execute(void)
{
   p_cancel=false;
   p_progressdialog= new  QProgressDialog(this);
   p_progressdialog -> setWindowModality(Qt::WindowModal);
   p_progressdialog->setWindowTitle(i18n("Executing query ..."));
   p_progressdialog->setMaximum(100);
   p_progressdialog->setMinimum(0);
   connect(p_progressdialog,SIGNAL(canceled()),this,SLOT(exec_cancelled()));
   p_progressdialog->show(); 
   KApplication::kApplication() -> processEvents();
 
  
}

void    hk_kdetablepartwidget::after_execute(void)
{
  delete p_progressdialog;
  p_progressdialog=NULL;
}


bool hk_kdetablepartwidget::set_progress(long int position,long int total,const hk_string&txt)
{
    if (p_progressdialog)
    {

        p_progressdialog->setWindowTitle(QString::fromUtf8(l2u(txt).c_str()));
	p_progressdialog->setMinimum(0);
        p_progressdialog->setMaximum(total);
        p_progressdialog->setValue(position);
        p_progressdialog->raise(); 
	KApplication::kApplication() -> processEvents();
    }
    return p_cancel;
}

void hk_kdetablepartwidget::exec_cancelled(void)
{
    p_cancel=true;

}

void hk_kdetablepartwidget::reload_table()
{
    hk_datasource* d=p_grid->datasource();
    if (d!=NULL)
    {
     unsigned long rp=d->row_position();
     d->disable();
     d->enable();
     if (rp>0)
       d->goto_row(rp);
    } 
}
