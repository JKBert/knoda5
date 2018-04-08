// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for knoda -Knorr's Datenbank
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for knoda4
//
// This file is part of knoda4 -Knorr's Datenbank 4.
// This file may be distributed and/or modified under the terms of the
// GNU General Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.51 $
#include "knodamaindockwindow.h"
#include "hk_kdedblistview.h"
/*TBP #include "hk_kdecsvimportdialog.h"
#include "hk_kdecsvexportdialog.h"
#include "hk_kdexmlexportdialog.h" */
#include "hk_kdedatabasesetting.h"
// TBP #include "hk_kdeobjecthandler.h"
#include "hk_kdepreferencesdialog.h"
/* TBP #include "hk_kdetablepartwidget.h"
#include "hk_kdequerypartwidget.h"
#include "hk_kdeformpartwidget.h" */
#include "hk_kdereportpartwidget.h" 
#include "hk_kdemodulepartwidget.h" 
#include "hk_kdenewdatabase.h"
#include "hk_kdedblistview.h"

#include <hk_connection.h>
#include <hk_drivermanager.h>
#include <hk_database.h>
#include <hk_report.h>

#include <qlayout.h>
#include <qclipboard.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qmenu.h>
#include <kcombobox.h>
#include <klocale.h>
//TBP #include <kservice.h>
#include <kiconloader.h>
#include <kapplication.h>
#include <kactionmenu.h>
#include <kmenu.h>
#include <KXMLGUIFactory>
#include <KTabWidget>
#include <KPushButton>
#include <KToolBar>
#include <KService>
#include <KParts/ReadWritePart>

//TBP icons
knodamaindockwindow::knodamaindockwindow(
        struct_commandlinefields* commandline,
 	QWidget* parent,const char* name, Qt::WFlags f,
	const QString& dbname,const QString& form):knodamaindockwindowbase(commandline,parent,name,f)
{
  if (p_listview)
    connect(p_listview,SIGNAL(itemSelectionChanged()),this,SLOT(slot_selection_changed()));
  clipboarddata_has_changed();
  if (!dbname.isEmpty()&& p_databasecombobox) {
    int c=p_databasecombobox->count();
    int count=0;
    bool found=false;
    
    while (count<c && !found) {
      QString s=p_databasecombobox->itemText(count);
      if (!s.isEmpty()) {
        if (s==dbname) {
          p_databasecombobox->setCurrentIndex(count);
          slot_database_selected(count);
          found=true;
        }
      }
      ++count;
    }
    if (!found) {
      p_databasecombobox->addItem(dbname);
      p_databasecombobox->setCurrentIndex(count);
    }
    if (!form.isEmpty())
      slot_viewmode_form(form);

  } else {
    if (p_database) {
      //automatically start "Autoform" when --strongruntime is set
      if (p_database->form_exists(u2l(autoform.toUtf8().data())))
        slot_viewmode_form(autoform);
    }

  }
  slot_selection_changed();
  if (p_newconnectionaction != NULL) {
    connect(p_newconnectionaction->menu(),SIGNAL(triggered(QAction*)),this,SLOT(slot_trigger_driver(QAction*)));
    QToolButton* btn = dynamic_cast<QToolButton*>(toolBar()->widgetForAction(p_newconnectionaction));
    
    if (btn)
        btn->setPopupMode(QToolButton::InstantPopup);
  }
  connect(p_exportdatabaseaction->menu(),SIGNAL(triggered(QAction*)),this,SLOT(slot_trigger_exportdbdriver(QAction*)));
  connect(p_importdatabaseaction->menu(),SIGNAL(triggered(QAction*)),this,SLOT(slot_trigger_importdbdriver(QAction*)));
  if (commandline && commandline->form.size()>0)
    slot_viewmode_form(QString::fromUtf8(l2u(commandline->form).c_str()));
}

knodamaindockwindow::~knodamaindockwindow()
{
}

  void knodamaindockwindow::slot_new_database(void)
  {
   /*TBP if (!p_connection->server_supports(hk_connection::SUPPORTS_NEW_DATABASE))
        return;
        hk_string r=hk_kdenewdatabase::show_dialog(p_connection->drivername(),p_connection->server_supports(hk_connection::SUPPORTS_LOCAL_FILEFORMAT));
	r=trim(r);
        if (r.size()>0)
        {
            if (!p_connection->create_database(r))
	    {
	      show_warningmessage(hk_translate("Database could not be created"));
	    }
	    set_databases();
        } */
  }

  void knodamaindockwindow::slot_delete_database(const QString& t)
  {
   /*TBP if (!p_connection->server_supports(hk_connection::SUPPORTS_DELETE_DATABASE))
        return;
    if (p_database && p_database->name()==u2l(t.toUtf8().data()))
        p_database=NULL;
    if (p_connection->delete_database(u2l(t.toUtf8().data())))
    {
    set_databases();
    slot_selection_changed();
    } */
  }


  void knodamaindockwindow::slot_new_table(void)
  {
   /*TBP if (!p_connection->server_supports(hk_connection::SUPPORTS_NEW_TABLE))
   {
     show_warningmessage(hk_translate("Driver does not support creating new tables"));
     return;
   }
   hk_kdetablepartwidget* o=new_table();
   hk_datasource* d=p_database->new_table();
   if (o)
   {
   o->set_datasource(d);
   o->set_designmode();
   } */
  }

  void knodamaindockwindow::slot_delete_table(const QString& t)
  {
   p_database->delete_table(u2l(t.toUtf8().data()));

  }

  void knodamaindockwindow::slot_designmode_table(const QString& t)
  {
   /*TBP hk_kdetablepartwidget* w=find_existing_table(u2l(t.toUtf8().data()));
    if (w)
    {
      tabs()->setCurrentWidget(w);
      if (w->mode()!=hk_presentation::designmode)w->set_designmode();
      return;
    }

   hk_kdetablepartwidget* o=new_table();
   hk_datasource* d=p_database->new_table();
   if (d)d->set_name(u2l(t.toUtf8().data()));
   if (o)
   {
   o->set_datasource(d);
   o->set_designmode();
   }*/
  }


  void knodamaindockwindow::slot_viewmode_table(const QString& t)
  {
   /*TBP hk_kdetablepartwidget* w=find_existing_table(u2l(t.toUtf8().data()));
    
    if (w)
    {
      tabs()->setCurrentWidget(w);
      if (w->mode()!=hk_presentation::viewmode)w->set_viewmode();
      return;
    }

   hk_kdetablepartwidget* o=new_table();
   hk_datasource* d=p_database->new_table();
   if (d)d->set_name(u2l(t.toUtf8().data()));
   if (o)
   {
   o->set_datasource(d);
   o->set_viewmode();
   } */
  }

  void knodamaindockwindow::slot_new_query(void)
  {
   /*TBP hk_kdequerypartwidget* o=new_query(false);
    hk_datasource* d=p_database->new_resultquery();
    if (o)
    {
      o->set_datasource(d);
      o->set_designmode();
    }*/
  }

  void knodamaindockwindow::slot_delete_query(const QString& t)
  {
     p_database->delete_file(u2l(t.toUtf8().data()),ft_query);
  }

  void knodamaindockwindow::slot_designmode_query(const QString&t)
  {
   /*TBP hk_kdequerypartwidget* w=find_existing_query(u2l(t.toUtf8().data()));
    
    if (w)
    {
      tabs()->setCurrentWidget(w);
      if (w->mode()!=hk_presentation::designmode)w->set_designmode();
      return;
    }
   
   hk_kdequerypartwidget* o=new_query(false);
   hk_datasource* d=p_database->new_resultquery();
   if (d)d->set_name(u2l(t.toUtf8().data()));
   if (o)
   {
   o->set_datasource(d);
   o->load_query();
   o->set_designmode();
   }*/
}

  void knodamaindockwindow::slot_viewmode_query(const QString&t)
  {
   /*TBP hk_kdequerypartwidget* w=find_existing_query(u2l(t.toUtf8().data()));

    if (w)
    {
      tabs()->setCurrentWidget(w);
      if (w->mode()!=hk_presentation::viewmode)w->set_viewmode();
      return;
    }

   hk_kdequerypartwidget* o=new_query(false);
   hk_datasource* d=p_database->new_resultquery();
   if (d)d->set_name(u2l(t.toUtf8().data()));
   if (o)
   {
   o->set_datasource(d);
   o->load_query();
   o->set_viewmode();
   } */
}

    void knodamaindockwindow::slot_new_view(void)
  {
/* TBP   if (!p_connection->server_supports(hk_connection::SUPPORTS_VIEWS))
   {
     show_warningmessage(hk_translate("Driver does not support creating new views"));
     return;
   }
   hk_kdequerypartwidget* o=new_query(true);
   hk_datasource* d=p_database->new_view();
   if (o)
   {
   o->set_datasource(d);
   o->set_designmode();
   } */
  }


  void knodamaindockwindow::slot_delete_view(const QString& t)
  {
     p_database->delete_view(u2l(t.toUtf8().data()));
  }


  void knodamaindockwindow::slot_designmode_view(const QString&t)
  {
 /* TBP   hk_kdequerypartwidget* w=find_existing_view(u2l(t.toUtf8().data()));
    
    if (w)
    {
      tabs() -> setCurrentWidget(w);
      if (w->mode()!=hk_presentation::designmode)w->set_designmode();
      return;
    }
   
   hk_kdequerypartwidget* o=new_query(true);
   hk_datasource* d=p_database->new_view();
   if (d)d->set_name(u2l(t.toUtf8().data()));
   if (o)
   {
   o->set_datasource(d);
   o->set_use_qbe(false,false);
   o->set_designmode();
   } */
}

  void knodamaindockwindow::slot_viewmode_view(const QString&t)
  {
   /*TBP hk_kdequerypartwidget* w=find_existing_view(u2l(t.toUtf8().data()));
    
    if (w)
    {
      tabs()->setCurrentWidget(w);
      if (w->mode()!=hk_presentation::viewmode)w->set_viewmode();
      return;
    }
   
   hk_kdequerypartwidget* o=new_query(true);
   hk_datasource* d=p_database->new_view();
   if (d)d->set_name(u2l(t.toUtf8().data()));
   if (o)
   {
   o->set_datasource(d);
   o->set_use_qbe(false,false);
   o->set_viewmode();
   } */
}

  void knodamaindockwindow::slot_new_form(void)
  {
    /* TBP if (!p_database) return;
    hk_kdeformpartwidget* w=new_form();
    if (w)
    {
    w->set_database(p_database);
    w->set_designmode();
    } */
 }

  void knodamaindockwindow::slot_delete_form(const QString& t)
  {
      p_database->delete_file(u2l(t.toUtf8().data()),ft_form);
  }

  void knodamaindockwindow::slot_designmode_form(const QString& t)
 {
   /*TBP if (!p_database) return;
    hk_kdeformpartwidget* w=find_existing_form(u2l(t.toUtf8().data()));
    if (w) {
      tabs()->setCurrentWidget(w);    
    }
    else {
      w=new_form();
      if (!w) return;
      w->set_database(p_database);
      w->load_form(u2l(t.toUtf8().data()));
    }
    if (w->mode()!=hk_form::designmode) w->set_designmode(); */
}
    
  void knodamaindockwindow::slot_viewmode_form(const QString& t)
 {
  /* TBP  if (!p_database) return;
    hk_kdeformpartwidget* w=find_existing_form(u2l(t.toUtf8().data())); 
    if (w) {
      tabs()->setCurrentWidget(w);    
    }
    else {
      w=new_form();
      if (!w) return;
      w->set_database(p_database);
      w->load_form(u2l(t.toUtf8().data()));
    }
    if (w->mode()!=hk_form::viewmode) { 
      w->set_viewmode();
    } */
}

  void knodamaindockwindow::slot_new_report(void)
  {
    if (!p_database) return;
    hk_kdereportpartwidget* w=new_report();
    if (w) {
      w->set_database(p_database);
      w->set_designmode();
    } 
  }

  void knodamaindockwindow::slot_delete_report(const QString& t)
  {
      p_database->delete_file(u2l(t.toUtf8().data()),ft_report);

  }


  void knodamaindockwindow::slot_designmode_report(const QString&t)
  {
    if (!p_database) return;
    hk_kdereportpartwidget* w=find_existing_report(u2l(t.toUtf8().data()));
    if (w)
    {
      tabs()->setCurrentWidget(w);
      if (w->mode()!=hk_report::designmode)w->set_designmode();
      return;
    }
          
    w=new_report();
    if (!w)return;
    w->set_database(p_database);
    w->load_report(u2l(t.toUtf8().data()));
    w->set_designmode();
  }

  void knodamaindockwindow::slot_viewmode_report(const QString& t)
  {
    if (!p_database) return;
    hk_kdereportpartwidget* w=find_existing_report(u2l(t.toUtf8().data()));
    if (w)
    {
      tabs()->setCurrentWidget(w);
      if (w->mode()!=hk_report::viewmode)w->set_viewmode();
      return;
    }
          
    w=new_report();
    if (!w) return;
    w->set_database(p_database);
    w->load_report(u2l(t.toUtf8().data()));
    w->set_viewmode();
  }

void knodamaindockwindow::slot_new_module(void)
{
    if (!p_database) return;
    hk_kdemodulepartwidget* w=new_module();
    if (!w)return;
    w->set_database(p_database);
    w->setFocus(); 
//     w->set_designmode(); 
}

void knodamaindockwindow::slot_delete_module(const QString& t)
{
  p_database->delete_file(u2l(t.toUtf8().data()),ft_module);
}


void knodamaindockwindow::slot_designmode_module(const QString&t)
{
  if (!p_database) return;
  hk_kdemodulepartwidget* w=find_existing_module(u2l(t.toUtf8().data()));
  if (w) {
    tabs()->setCurrentWidget(w);
  }
  else {          
    w=new_module();
    if (!w)return;
    w->set_database(p_database);
    w->load_module(u2l(t.toUtf8().data()));   
  }
  w->setFocus();
}

hk_kdetablepartwidget* knodamaindockwindow::new_table(void)
{
  /* TBP KService::Ptr service = KService::serviceByDesktopName("hk_kde4tablepart");
  KParts::ReadWritePart* p_part;

  if (!service ||
      !(p_part = service->createInstance<KParts::ReadWritePart>(tabs(), this, QVariantList()))) {
    show_warningmessage(hk_translate("Fatal error! Table part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
  }
  hk_kdetablepartwidget* table = (hk_kdetablepartwidget*)p_part->widget();
  if (!table) {
    show_warningmessage(hk_translate("Warning! Tablepartwiget could not be created!"));
    return NULL;
  }
  p_partmanager->addPart(p_part);
  connect(table, SIGNAL(signal_captionChanged(QWidget*,QString)), this, SLOT(slot_captionChanged(QWidget*,QString)));
  int i = tabs() -> addTab(table,i18n("Table"));
  tabs() -> setTabIcon(i,KIcon("view-form-table"));
  tabs() -> setCurrentIndex(i);
  table->setFocus();
  return table; */ return NULL;
}

  hk_kdequerypartwidget* knodamaindockwindow::new_query(bool as_view)
  {
   /* TBP KService::Ptr service = KService::serviceByDesktopName("hk_kde4querypart");
    KParts::ReadWritePart* p_part;
    if (!service ||
      !(p_part = service->createInstance<KParts::ReadWritePart>(tabs(), this, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Query part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
    }
    hk_kdequerypartwidget* query = (hk_kdequerypartwidget*)p_part->widget();
    if (!query)
    {
      show_warningmessage(hk_translate("Warning! Querypartwiget could not be created!"));
      return NULL;

    }
    KIconLoader* loader=KIconLoader::global();
    loader->addAppDir("hk_kde4classes");

    p_partmanager->addPart(p_part);
    int i = tabs() -> addTab(query,i18n("Query"));
    tabs() -> setTabIcon(i,KIcon(as_view?"document-preview":"document-edit-verify"));
    connect(query, SIGNAL(signal_captionChanged(QWidget*,QString)), this, SLOT(slot_captionChanged(QWidget*,QString)));
    tabs() -> setCurrentIndex(i);       
    return query; */ return NULL;
  }


  hk_kdeformpartwidget* knodamaindockwindow::new_form(void)
  {
    /* TBP KService::Ptr service = KService::serviceByDesktopName("hk_kde4formpart");
    KParts::ReadWritePart* p_part;
    
    if (!service || 
      !(p_part = service->createInstance<KParts::ReadWritePart>(tabs(), this, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Form part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
    }
     hk_kdeformpartwidget* form = (hk_kdeformpartwidget*)p_part->widget();
     if (!form)
      {
        show_warningmessage(hk_translate("Warning! Formpartwiget could not be created!"));
	return NULL;

      }
     p_partmanager->addPart(p_part);
     int i = tabs() -> addTab(form,i18n("Form"));
     tabs() -> setTabIcon(i,KIcon("utilities-terminal"));
     connect(form, SIGNAL(signal_captionChanged(QWidget*,QString)), this, SLOT(slot_captionChanged(QWidget*,QString)));
     connect(form,SIGNAL(signal_raise_form(const hk_string&))
          ,this,SLOT(slot_raise_form(const hk_string&)));
   tabs() -> setCurrentIndex(i);
   form->setFocus();   
   return form; */ return NULL;
  }

hk_kdereportpartwidget* knodamaindockwindow::new_report(void)
{
    KService::Ptr service = KService::serviceByDesktopName("hk_kde5reportpart");
    KParts::ReadWritePart* p_part;
        
    if (!service ||
      !(p_part = service->createInstance<KParts::ReadWritePart>(tabs(), this, QVariantList())))
    {
        show_warningmessage(hk_translate("Fatal error! Report part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
        exit(1);
    }
    hk_kdereportpartwidget* report = (hk_kdereportpartwidget*)p_part->widget();
    if (!report)
    {
        show_warningmessage(hk_translate("Warning! Reportpartwiget could not be created!"));
        return NULL;
    }
    p_partmanager->addPart(p_part);
    int i = tabs() -> addTab(report,i18n("Report"));
    QIcon::setThemeName("oxygen");
    tabs()->setTabIcon(i,QIcon::fromTheme("document-print"));
    connect(report, SIGNAL(signal_captionChanged(QWidget*,QString)), this, SLOT(slot_captionChanged(QWidget*,QString)));
    connect(report,SIGNAL(signal_raise_form(const hk_string&)),this,SLOT(slot_raise_form(const hk_string&)));
    tabs()->setCurrentIndex(i);
    report->setFocus();
    return report;
}

hk_kdemodulepartwidget* knodamaindockwindow::new_module(void)
{   
  KService::Ptr service = KService::serviceByDesktopName("hk_kde5modulepart");
  KParts::ReadWritePart* p_part;
  
  if (!service ||
    !(p_part = service->createInstance<KParts::ReadWritePart>(tabs(), this, QVariantList())))
  {
    show_warningmessage(hk_translate("Fatal error! Module part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
  }
  hk_kdemodulepartwidget* module = (hk_kdemodulepartwidget*)p_part->widget();
  if (!module)
  {
    show_warningmessage(hk_translate("Warning! Modulepartwiget could not be created!"));
    return NULL;
  }
   
  p_partmanager->addPart(p_part);
  int i = tabs() -> addTab(module,i18n("Module"));
  QIcon::setThemeName("oxygen");
  tabs() -> setTabIcon(i,QIcon::fromTheme("document-edit"));
  connect(module, SIGNAL(signal_captionChanged(QWidget*,QString)), this, SLOT(slot_captionChanged(QWidget*,QString)));
  tabs() -> setCurrentIndex(i);
  module->setFocus();
  return module;
}   

void knodamaindockwindow::rename_clicked()
{
  if (!p_listview) return;
  if (!p_listview->currentItem()||p_listview->is_headeritem())
    return;

  hk_string newname=trim(show_stringvaluedialog(hk_translate("Enter new name:")));
  if (newname.size()==0) return;
    filetype f;
    if (p_listview->is_tableitem())
       f=ft_table;
    else
    if (p_listview->is_queryitem())
       f=ft_query;
    else
    if (p_listview->is_formitem())
       f=ft_form;
    else
       f=ft_report;

    if (!p_database->rename_file(u2l(p_listview->currentItem()->text(0).toUtf8().data())
    ,newname,f))
    show_warningmessage(hk_translate("Could not rename object!"));
}

void knodamaindockwindow::importcsv_clicked()
{
  /* TBP  if (p_database==NULL) return;
    hk_kdecsvimportdialog* i= new hk_kdecsvimportdialog(0,0,true);
    hk_datasource* ds=p_database->new_table();
//   ds->wanna_debug(true);
    i->set_datasource(ds);
    i->exec();
    delete i; */
}

void knodamaindockwindow::exportcsv_clicked()
{
  /*TBP if (p_database==NULL||!p_listview) return;
  hk_kdecsvexportdialog* e =new hk_kdecsvexportdialog(p_listview->currentItem()->text(0),0,0,true);
  e->set_database(p_database);
  if (p_listview->is_viewitem()) e->set_useviewlist();
  else
    if (p_listview->is_queryitem()) e->set_usequerylist();
    else
      e->set_usetablelist();
  e->exec();
  delete e; */
}

void knodamaindockwindow::exportxml_clicked()
{
  /* TBP if (p_database==NULL||!p_listview) return;
  hk_kdexmlexportdialog* e =new hk_kdexmlexportdialog(p_listview->currentItem()->text(0),0,0,true);
    e->set_database(p_database);
    if (p_listview->is_viewitem())e->set_useviewlist();
    else
    if (p_listview->is_queryitem())e->set_usequerylist();
    else
    e->set_usetablelist();
    e->exec();
    delete e; */
}

void knodamaindockwindow::newpassworddialog_selected(void)
{
    if (p_connection!=NULL) p_connection->show_newpassworddialog();
}

void knodamaindockwindow::copy_clicked(void)
{
if (p_listview)p_listview->copy();
}

void knodamaindockwindow::paste_clicked(void)
{
if (p_listview)p_listview->paste();
}

void knodamaindockwindow::clipboarddata_has_changed(void)
{
  p_copyaction->setEnabled(p_connection&&p_listview&&(!p_listview->is_headeritem()
                        ||p_listview->is_databaseitem()));
  p_pasteaction->setEnabled(p_listview && 
    p_listview -> canAccept(QApplication::clipboard()->mimeData(QClipboard::Clipboard)));
}

void knodamaindockwindow::slot_selection_changed(void)
{
    if (!p_connection) return;
    p_newaction->setEnabled(!runtime_only()
        && p_database && p_database->name().size()>0 && p_listview 
        && ((p_listview->is_databaseitem() && p_connection->server_supports(hk_connection::SUPPORTS_NEW_DATABASE))
        || (p_listview->is_tableitem(true) && p_connection->server_supports(hk_connection::SUPPORTS_NEW_TABLE))
        || (p_listview->is_viewitem(true) && p_connection->server_supports(hk_connection::SUPPORTS_NEW_VIEW))
        || p_listview->is_queryitem(true) || p_listview->is_formitem(true) || p_listview->is_reportitem(true) || p_listview->is_moduleitem(true)
    ) );
 
 bool e=!runtime_only() && p_database && p_database->name().size()>0;
 p_newdatabaseaction->setEnabled(!runtime_only() && p_connection->server_supports(hk_connection::SUPPORTS_NEW_DATABASE));
 p_newtableaction->setEnabled(e && p_connection->server_supports(hk_connection::SUPPORTS_NEW_TABLE));
 p_newqueryaction->setEnabled(e && p_connection->server_supports(hk_connection::SUPPORTS_SQL));
 p_newviewaction->setEnabled(e && p_connection->server_supports(hk_connection::SUPPORTS_NEW_VIEW));
 p_newformaction->setEnabled(e);
 p_newreportaction->setEnabled(e);

 p_alteraction->setEnabled(p_listview && !p_listview->is_headeritem() && !runtime_only());

 p_startaction->setEnabled(p_listview && !p_listview->is_headeritem());

 p_deleteaction->setEnabled(p_listview && (!p_listview->is_headeritem()
 			    || p_listview->is_databaseitem()
 		           ) && !runtime_only() && p_database && p_database->name().size()>0
			   );

 p_renameaction->setEnabled(p_listview 
     && ((p_listview->is_databaseitem() && p_connection->server_supports(hk_connection::SUPPORTS_RENAME_DATABASE))
     || (p_listview->is_tableitem() && p_connection->server_supports(hk_connection::SUPPORTS_RENAME_TABLE))
     || ((p_listview->is_queryitem()||p_listview->is_formitem() || p_listview->is_reportitem()) && !runtime_only())
     ) );
 p_exportaction->setEnabled(p_listview );
 p_exportcsvaction->setEnabled(p_listview && !runtime_only());
 p_exportxmlaction->setEnabled(p_listview && !runtime_only());
 p_importaction->setEnabled(!runtime_only());
 p_importcsvaction->setEnabled(!runtime_only());
 p_databasesettingaction->setEnabled(p_database&&!runtime_only());
 p_passwordaction->setEnabled(p_listview &&(p_listview->database()!=NULL)
    && p_connection->server_needs(hk_connection::NEEDS_PASSWORD)); 
 clipboarddata_has_changed();
}

void knodamaindockwindow::slot_listbox_popup(QTreeWidgetItem* item,const QPoint&point)
{
  if (!item) return;
  QMenu * popup=static_cast<QMenu *> ( factory()->container("popup", this));
  if (popup)
        popup->exec( point );


}

void knodamaindockwindow::databasesettings_clicked(void)
{
 /*TBP hk_kdedatabasesetting* setting=new hk_kdedatabasesetting(p_database);
  if (setting->exec()==QDialog::Accepted)
    {
      if (p_listview)p_listview->reload_lists();
    }
  p_objecthandleraction->setEnabled(p_database&& p_database->has_centralstoragetable()&&!runtime_only());
  delete setting; */
}

void knodamaindockwindow::objecthandler_clicked(void)
{
 /* TBP hk_kdeobjecthandler* h=new hk_kdeobjecthandler(p_database);
  h->exec();

  delete h; */
}


void knodamaindockwindow::newwindow_clicked(void)
{
  knodamaindockwindow* w=new knodamaindockwindow();
  
  w->show();
}

void knodamaindockwindow::show_drivers(KMenu* menu)
{
  /*TBP  if (!menu) return;
    menu->clear();
    vector<hk_string>* drv=hk_drivermanager::driverlist();
    vector<hk_string>::iterator it;
    int di=0;
    QAction* menuItem;
    
    for(it=drv->begin();it!=drv->end();it++)
    {
        menuItem = menu->addAction(QString::fromUtf8(l2u((*it)).c_str()));
        menuItem->setData(QVariant(di));
        di++;
    } */
}

void knodamaindockwindow::show_drivers()
{
   /*TBP KMenu* menu= p_newconnectionaction->menu();
    
    show_drivers(menu); */
}

void knodamaindockwindow::show_exportdbdrivers()
{
  /*TBP  KMenu* menu= p_exportdatabaseaction->menu();
    
    show_drivers(menu); */
}

void knodamaindockwindow::show_importdbdrivers()
{
  /* TBP  KMenu* menu= p_importdatabaseaction->menu();
    
    show_drivers(menu); */
}

void knodamaindockwindow::slot_trigger_driver(QAction* action)
{
    driver_selected(action->data().toInt());
}

void knodamaindockwindow::slot_trigger_exportdbdriver(QAction* action)
{
    exportdbdriver_selected(action->data().toInt());
}

void knodamaindockwindow::slot_trigger_importdbdriver(QAction* action)
{
    importdbdriver_selected(action->data().toInt());
}

void knodamaindockwindow::settings_clicked()
{
    hk_kdepreferencesdialog* p_dialog= new hk_kdepreferencesdialog(0,0,true);
    
    p_dialog->exec();
    delete p_dialog; 

}

hk_kdeformpartwidget* knodamaindockwindow::find_existing_form(const hk_string& n)
{
  /* TBP if (!p_database) return NULL;
  hk_kdeformpartwidget* w = NULL;
  QWidget* sf=dynamic_cast<QWidget*> (p_database->existing_form(n));
  while (sf!=NULL && w==NULL) {
    sf=sf->parentWidget();
    hk_form* f=dynamic_cast<hk_form*>(sf);
    if (f && f->is_subform()) sf=NULL;
    w= dynamic_cast <hk_kdeformpartwidget*> (sf);
  }
  return w; */ return NULL;
}

void knodamaindockwindow::setCaption(const QString& c)
{
  if (p_connection)
    {
        hk_string s=p_connection->drivername()+" "+p_connection->user()+"@"+p_connection->host()+":"+ulongint2string(p_connection->tcp_port());
        knodamaindockwindowbase::setCaption(QString::fromUtf8(l2u(s).c_str()));
    }
    else  knodamaindockwindowbase::setCaption(c);
}

hk_kdereportpartwidget* knodamaindockwindow::find_existing_report(const hk_string& n)
{
  if (!p_database) return NULL;
  hk_kdereportpartwidget* w = NULL;
  QWidget* sf=dynamic_cast<QWidget*>(p_database->existing_report(n));

  while (sf!=NULL && w==NULL) {
    sf=sf->parentWidget();
    hk_report* r=dynamic_cast<hk_report*>(sf);
    if (r && r->is_subreport())sf=NULL;
    w= dynamic_cast <hk_kdereportpartwidget*> (sf);
  }
  return w; 
}

hk_kdetablepartwidget* knodamaindockwindow::find_existing_table(const hk_string& n)
{
 /*TBP hk_kdetablepartwidget* w = NULL;
  
  for(int i = 0;i < tabs()->count();i++){
    w=dynamic_cast<hk_kdetablepartwidget*>(tabs() -> widget(i));
    if (w &&  w->datasource() && w->datasource()->name()==n) return w;   
  } */
  return NULL;
}


hk_kdequerypartwidget* knodamaindockwindow::find_existing_query(const hk_string& n)
{
 /*TBP  hk_kdequerypartwidget* w = NULL;
  
  for(int i = 0;i < tabs()->count();i++){ 
    w=dynamic_cast<hk_kdequerypartwidget*>(tabs() -> widget(i));
    if (w &&  w->datasource() && w->datasource()->type()==hk_data::ds_query&& w->datasource()->name()==n) return w;
  } */
  return NULL;
}

hk_kdemodulepartwidget* knodamaindockwindow::find_existing_module(const hk_string& n)
{
  hk_kdemodulepartwidget* w = NULL;
  
  for(int i = 0;i < tabs()->count();i++){
    w=dynamic_cast<hk_kdemodulepartwidget*>(tabs() -> widget(i));
    if (w &&  w->hk_module::name()==n) return w;
  }
  return NULL;
}

hk_kdequerypartwidget* knodamaindockwindow::find_existing_view(const hk_string& n)
{
 /*TBP hk_kdequerypartwidget* w = NULL;

  for(int i = 0;i < tabs()->count();i++){
    w=dynamic_cast<hk_kdequerypartwidget*>(tabs() -> widget(i));
    if (w &&  w->datasource() && w->datasource()->type()==hk_data::ds_view&& w->datasource()->name()==n) return w;
  }*/
  return NULL;
}


void knodamaindockwindow::slot_raise_form(const hk_string& fo)
{
 /*TBP hk_kdeformpartwidget* f=find_existing_form(fo);
  if (!f) return;
  tabs()->setCurrentWidget(f);  */
}
