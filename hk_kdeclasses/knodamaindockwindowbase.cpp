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
//$Revision: 1.56 $
#include "knodamaindockwindowbase.h"
#include "knodamaindockwindow.h"
#include "hk_kdedblistview.h"
// TBP #include "hk_kdeeximportdatabase.h"
#include "hk_kdedbdesigner.h"

#include <hk_connection.h>
#include <hk_dsvisible.h>
#include <hk_database.h>
#include <hk_form.h>
#include <hk_referentialintegrity.h>
#include <hk_drivermanager.h>

#include <qapplication.h>
#include <qpixmap.h>
#include <qclipboard.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qstringlist.h>
#include <qdockwidget.h>
#include <QCloseEvent>
#include <kcombobox.h>
#include <kdebug.h>
#include <kfiledialog.h>
#include <kdirselectdialog.h>
#include <kconfig.h>
#include <klocale.h>
#include <kapplication.h>
#include <kaction.h>
#include <kmenu.h>
#include <kparts/part.h>
#include <kglobal.h>
#include <kcombobox.h>
#include <kactionmenu.h>
#include <kactioncollection.h>
#include <QVBoxLayout>
#include <ktoolbar.h>
#include <KTabWidget>
#include <KIcon>
#include <KConfigGroup>


class knodamaindockwindowbaseprivate
{
  public:
  knodamaindockwindowbaseprivate():p_drivermanager(NULL), p_designer(NULL)
  {
      
  }

QString           p_databasename;
QDockWidget*      p_viewcontainer;
hk_drivermanager* p_drivermanager;
hk_kdedbdesignerwindow* p_designer;
};

knodamaindockwindowbase::knodamaindockwindowbase(struct_commandlinefields* cl,
 	QWidget* parent,const char* /*name*/, Qt::WFlags f,
	const QString& /*dbname*/,const QString& /*form*/)
	:KParts::MainWindow(parent,f),
	hk_dsvisible()
{
  if (f == Qt::Window) // with default flags set Delete on Close
    setAttribute(Qt::WA_DeleteOnClose);
  QWidget* p_mainviewwidget = NULL;
  p_listview=NULL;
  p_database=NULL;
  p_connection=NULL;
  p_partmanager=new KParts::PartManager(this);
  i18n("&Database");

  connect(p_partmanager,SIGNAL(partAdded(KParts::Part*))
  ,this,SLOT(slot_activate_closeaction()));
  connect(p_partmanager,SIGNAL(partRemoved(KParts::Part*))
  ,this,SLOT(slot_activate_closeaction()));
  connect(p_partmanager,SIGNAL(activePartChanged(KParts::Part*))
   ,this,SLOT(createGUI(KParts::Part*)));
  setComponentName("hk_kde5classes", "Main Window");
  setXMLFile("knodamaindockwindowbase.rc");
  resize( 700, 480 );

  p_private=new knodamaindockwindowbaseprivate;
  bool rt_only=false;
  if (cl) rt_only=cl->runtime_only;
  p_private->p_drivermanager=new hk_drivermanager(rt_only);


  if (cl && cl->p_guicommands &&!cl->p_guicommands->p_showlistwindow)
  {
    p_private->p_viewcontainer=NULL;
  }
  else
  {
    p_private->p_viewcontainer=new QDockWidget(this);
    p_private->p_viewcontainer -> setObjectName("elements");
    p_mainviewwidget = new QWidget(p_private->p_viewcontainer);
    p_listview=new hk_kdedblistview(p_mainviewwidget);
    QVBoxLayout * p_layout = new QVBoxLayout();
    p_mainviewwidget->setLayout(p_layout);
   // p_layout-> addWidget(p_listview);
   }

   QToolBar* toolbar=NULL;

   if (!runtime_only() && p_private->p_viewcontainer)
   {
    toolbar=new QToolBar(p_mainviewwidget);
    p_mainviewwidget->layout()->addWidget(toolbar);
   }
   else
   {
    newbutton=NULL;
    alterbutton=NULL;
    deletebutton=NULL;
    startbutton=NULL;
   }
   
    if (p_private->p_viewcontainer)
    {
     p_mainviewwidget->layout()->addWidget(p_listview);
     p_private->p_viewcontainer->setWidget(p_mainviewwidget);
     p_private->p_viewcontainer->setWindowTitle(i18n("Elements"));
    }
   if (p_listview)p_listview->setWindowTitle(i18n("Elements"));

   if (p_private->p_viewcontainer)
   {
    addDockWidget(Qt::LeftDockWidgetArea,p_private->p_viewcontainer);
    p_private->p_viewcontainer->show();
    }
   p_tabs = new KTabWidget(this);
   p_tabs -> setTabsClosable(true);
   setCentralWidget(p_tabs); 
   connect(p_tabs,SIGNAL(closeRequest(QWidget*)),this, SLOT(slot_closeTab(QWidget*)));
   connect(p_tabs,SIGNAL(currentChanged(int)),this, SLOT(slot_focusCurrent(int)));
   
   p_quitaction=new KAction(QIcon::fromTheme("application-exit"),i18n("E&xit"),actionCollection());
   actionCollection()-> addAction("quit",p_quitaction);
   connect(p_quitaction,SIGNAL(triggered()),this,SLOT(close()));
   p_closeaction=new KAction(QIcon::fromTheme("window-close"),i18n("&Close"),actionCollection());
   actionCollection()-> addAction("close",p_closeaction);
   connect(p_closeaction,SIGNAL(triggered()),this,SLOT(slot_closewindow()));
   p_closeaction->setEnabled(false);
   if ((cl &&cl->p_guicommands &&cl->p_guicommands->p_showlistwindow)||!cl)
   {
   p_databasecombobox= new KComboBox(false,this );
   p_databasecombobox->setMinimumWidth(200);

   QLabel* label=new QLabel(0L);
   label->setText(i18n("Database: "));
   KAction * p_ka = new KAction(NULL);
   actionCollection() -> addAction("databaselabel",p_ka);
   p_ka -> setDefaultWidget(label);
   p_databaseaction=new KAction(i18n("Select database"),actionCollection());
   actionCollection()-> addAction("databaseselect",p_databaseaction);
   p_databaseaction -> setDefaultWidget(p_databasecombobox);
   }
   else
   {
   p_databasecombobox=NULL;
   p_databaseaction=NULL;
   }

  p_newobjectaction= new KActionMenu(i18n("&New"),actionCollection());
  actionCollection() -> addAction("newobject",p_newobjectaction);

  if ( !cl || (cl && cl->p_guicommands->p_showlistwindow) )
  {
    p_newconnectionaction= new KActionMenu(QIcon::fromTheme("network-connect"),i18n("&Connect to"),actionCollection());
    actionCollection()-> addAction("newconnection",p_newconnectionaction);
    p_disconnectaction= new KAction(QIcon::fromTheme("network-disconnect"),i18n("Disconnect"),actionCollection());
    actionCollection()-> addAction("disconnect",p_disconnectaction);
    connect(p_disconnectaction,SIGNAL(triggered()),this,SLOT(slot_disconnect()));
    connect(p_newconnectionaction->menu(),SIGNAL(aboutToShow()),this,SLOT(show_drivers()));
  }
  else
  {
    p_newconnectionaction=NULL;
    p_disconnectaction=NULL;
  }

    p_loadconnection= new KAction(QIcon::fromTheme("folder-documents"),i18n("Load connection"),actionCollection());
    actionCollection()-> addAction("loadconnection",p_loadconnection);
    connect(p_loadconnection,SIGNAL(triggered()),this,SLOT(slot_load_connection()));
    p_storeconnection= new KAction(QIcon::fromTheme("document-save"),i18n("Store connection"),actionCollection());
    actionCollection()-> addAction("storeconnection",p_storeconnection);
    connect(p_storeconnection,SIGNAL(triggered()),this,SLOT(slot_store_connection()));
    p_newdatabaseaction= new KAction(QIcon::fromTheme("folder-documents"),i18n("Database"),actionCollection());
    actionCollection()-> addAction("newdatabase",p_newdatabaseaction);
    connect(p_newdatabaseaction,SIGNAL(triggered()),this,SLOT(slot_new_database()));
    p_newtableaction= new KAction(QIcon::fromTheme("view-form-table"),i18n("Table"),actionCollection());
    actionCollection()-> addAction("newtable",p_newtableaction);
    connect(p_newtableaction,SIGNAL(triggered()),this,SLOT(slot_new_table()));
    p_newqueryaction= new KAction(QIcon::fromTheme("document-preview"),i18n("Query"),actionCollection());
    actionCollection()-> addAction("newquery",p_newqueryaction);
    connect(p_newqueryaction,SIGNAL(triggered()),this,SLOT(slot_new_query()));
    p_newviewaction= new KAction(QIcon::fromTheme("document-preview"),i18n("View"),actionCollection());
    actionCollection()-> addAction("newview",p_newviewaction);
    connect(p_newviewaction,SIGNAL(triggered()),this,SLOT(slot_new_view()));
    p_newformaction= new KAction(QIcon::fromTheme("utilities-terminal"),i18n("Form"),actionCollection());
    actionCollection()-> addAction("newform",p_newformaction);
    connect(p_newformaction,SIGNAL(triggered()),this,SLOT(slot_new_form()));    
    p_newreportaction= new KAction(QIcon::fromTheme("printer"),i18n("Report"),actionCollection());
    actionCollection()-> addAction("newreport",p_newreportaction);
    connect(p_newreportaction,SIGNAL(triggered()),this,SLOT(slot_new_report()));    
    p_newmoduleaction= new KAction(QIcon::fromTheme("document-edit"),i18n("Module"),actionCollection());
    actionCollection()-> addAction("newmodule",p_newmoduleaction);
    connect(p_newmoduleaction,SIGNAL(triggered()),this,SLOT(slot_new_module()));
    p_newobjectaction->addAction(p_newdatabaseaction);
    p_newobjectaction->addAction(p_newtableaction);
    p_newobjectaction->addAction(p_newviewaction);
    p_newobjectaction->addAction(p_newqueryaction);
    p_newobjectaction->addAction(p_newformaction);
    p_newobjectaction->addAction(p_newreportaction);
    p_newobjectaction->addAction(p_newmoduleaction); 

    p_newaction= new KAction(QIcon::fromTheme("document-new"),i18n("&New"),actionCollection());
    actionCollection()-> addAction("newclicked",p_newaction);   
    p_deleteaction= new KAction(QIcon::fromTheme("document-close"),i18n("&Delete"),actionCollection());
    actionCollection()-> addAction("deleteclicked",p_deleteaction);       
    p_startaction= new KAction(QIcon::fromTheme("system-run"),i18n("&Start"),actionCollection());
    actionCollection()-> addAction("startclicked",p_startaction);    
    p_alteraction= new KAction(QIcon::fromTheme("document-edit"), i18n("&Modify"),actionCollection());
    actionCollection()-> addAction("alterclicked",p_alteraction);
      
    p_alteraction->setEnabled(false);
    p_startaction->setEnabled(false);
    p_deleteaction->setEnabled(false);
    
    p_renameaction=new KAction(i18n("&Rename"),actionCollection());
    actionCollection()-> addAction("renameclicked",p_renameaction);
    connect(p_renameaction,SIGNAL(triggered()),this,SLOT(rename_clicked()));    

    p_exportaction= new KActionMenu(i18n("E&xport"),actionCollection());
    actionCollection()-> addAction("export",p_exportaction);
    p_exportcsvaction= new KAction(i18n("&CSV"),actionCollection());
    actionCollection()-> addAction("exportcsv",p_exportcsvaction);
    connect(p_exportcsvaction,SIGNAL(triggered()),this,SLOT(exportcsv_clicked()));
    p_exportxmlaction= new KAction(i18n("&XML"),actionCollection());
    actionCollection()-> addAction("exportxml",p_exportxmlaction);
    connect(p_exportxmlaction,SIGNAL(triggered()),this,SLOT(exportxml_clicked()));
    p_exportaction->addAction(p_exportcsvaction);
    p_exportaction->addAction(p_exportxmlaction);
    p_exportdatabaseaction= new KActionMenu(i18n("To &database"),actionCollection());
    actionCollection()-> addAction("exportdatabase",p_exportdatabaseaction);
    p_exportaction->addAction(p_exportdatabaseaction);
    connect(p_exportdatabaseaction->menu(),SIGNAL(aboutToShow()),this,SLOT(show_exportdbdrivers()));
    p_exportaction->setEnabled(false);
   
    p_importaction= new KActionMenu(i18n("I&mport"),actionCollection());
    actionCollection()-> addAction("import",p_importaction);   
    p_importcsvaction= new KAction(i18n("&CSV"),actionCollection());
    actionCollection()-> addAction("importcsv",p_importcsvaction);
    connect(p_importcsvaction,SIGNAL(triggered()),this,SLOT(importcsv_clicked()));
    p_importaction->addAction(p_importcsvaction);
    p_importdatabaseaction= new KActionMenu(i18n("From &database"),actionCollection());
    actionCollection()-> addAction("importdatabase",p_importdatabaseaction);
    p_importaction->addAction(p_importdatabaseaction);
    connect(p_importdatabaseaction->menu(),SIGNAL(aboutToShow()),this,SLOT(show_importdbdrivers()));
    p_importaction->setEnabled(false);
    
    p_passwordaction= new KAction(i18n("&Change password"),actionCollection());
    actionCollection()-> addAction("changepassword",p_passwordaction);
    connect(p_passwordaction,SIGNAL(triggered()),this,SLOT(newpassworddialog_selected()));
    p_copyaction= new KAction(QIcon::fromTheme("edit-copy"),i18n("&Copy"),actionCollection());
    actionCollection()-> addAction("copy",p_copyaction);
    connect(p_copyaction,SIGNAL(triggered()),this,SLOT(copy_clicked()));
    p_copyaction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    p_pasteaction= new KAction(QIcon::fromTheme("edit-paste"),i18n("&Paste"),actionCollection());
    actionCollection()-> addAction("paste",p_pasteaction);
    connect(p_pasteaction,SIGNAL(triggered()),this,SLOT(paste_clicked()));
    p_pasteaction -> setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_V);
    p_databasesettingaction= new KAction(i18n("&Database settings"),actionCollection());
    actionCollection()-> addAction("dbsettings",p_databasesettingaction);
    connect(p_databasesettingaction,SIGNAL(triggered()),this,SLOT(databasesettings_clicked()));
    p_objecthandleraction= new KAction(i18n("&Object handler"),actionCollection());
    actionCollection()-> addAction("objecthandler",p_objecthandleraction);
    connect(p_objecthandleraction,SIGNAL(triggered()),this,SLOT(objecthandler_clicked()));
    p_objecthandleraction->setEnabled(false);
    p_newwindowaction= new KAction(i18n("&New databasewindow"),actionCollection());
    actionCollection()-> addAction("newwindow",p_newwindowaction);
    connect(p_newwindowaction,SIGNAL(triggered()),this,SLOT(newwindow_clicked()));
    p_settingsaction= new KAction(QIcon::fromTheme("configure"),i18n("&Options"),actionCollection());
    actionCollection()-> addAction("settings",p_settingsaction);
    connect(p_settingsaction,SIGNAL(triggered()),this,SLOT(settings_clicked()));
    p_openlocaldbaction=  new KAction(QIcon::fromTheme("document-open-folder"),i18n("Open &local database"),actionCollection());
    actionCollection()-> addAction("openlocaldb",p_openlocaldbaction);
    connect(p_openlocaldbaction,SIGNAL(triggered()),this,SLOT(slot_open_localdatabase()));
    p_openlocaldbaction->setEnabled(false);
    p_referentialintegrityaction=new KAction(KIcon("dbdesigner"),i18n("Referential integrity"),actionCollection());
    actionCollection()-> addAction("referentialintegrity",p_referentialintegrityaction);
    connect(p_referentialintegrityaction,SIGNAL(triggered()),this,SLOT(slot_referentialintegrity()));
    p_referentialintegrityaction->setEnabled(false);

   set_connection(NULL);

   connect (QApplication::clipboard(), SIGNAL(dataChanged()),this, SLOT(clipboarddata_has_changed()));

   if(p_databasecombobox)
   connect(p_databasecombobox,SIGNAL(activated(int)),this,SLOT(slot_database_selected(int)));
   if (p_listview)
   {  
    if(toolbar) {
      toolbar -> addAction(p_newaction);
      toolbar -> addAction(p_alteraction);
      toolbar -> addAction(p_deleteaction);
      toolbar -> addAction(p_startaction);
    }
    
    connect(p_newaction,SIGNAL(triggered()),p_listview,SLOT(new_clicked()));
    connect(p_startaction,SIGNAL(triggered()),p_listview,SLOT(start_clicked()));
    connect(p_deleteaction,SIGNAL(triggered()),p_listview,SLOT(delete_clicked()));
    connect(p_alteraction,SIGNAL(triggered()),p_listview,SLOT(alter_clicked())); 
    
   connect(p_listview,SIGNAL(signal_delete_database(const QString&)),this,SLOT(slot_delete_database(const QString&)));
   connect(p_listview,SIGNAL(signal_new_database(void)),this,SLOT(slot_new_database(void)));
   connect(p_listview,SIGNAL(signal_reload_databases(void)),this,SLOT(set_databases(void)));

   connect(p_listview,SIGNAL(signal_viewmode_table(const QString&)),this,SLOT(slot_viewmode_table(const QString&)));
   connect(p_listview,SIGNAL(signal_designmode_table(const QString&)),this,SLOT(slot_designmode_table(const QString&)));
   connect(p_listview,SIGNAL(signal_delete_table(const QString&)),this,SLOT(slot_delete_table(const QString&)));
   connect(p_listview,SIGNAL(signal_new_table(void)),this,SLOT(slot_new_table(void)));

   connect(p_listview,SIGNAL(signal_viewmode_query(const QString&)),this,SLOT(slot_viewmode_query(const QString&)));
   connect(p_listview,SIGNAL(signal_designmode_query(const QString&)),this,SLOT(slot_designmode_query(const QString&)));
   connect(p_listview,SIGNAL(signal_delete_query(const QString&)),this,SLOT(slot_delete_query(const QString&)));
   connect(p_listview,SIGNAL(signal_new_query(void)),this,SLOT(slot_new_query(void)));

   connect(p_listview,SIGNAL(signal_viewmode_view(const QString&)),this,SLOT(slot_viewmode_view(const QString&)));
   connect(p_listview,SIGNAL(signal_designmode_view(const QString&)),this,SLOT(slot_designmode_view(const QString&)));
   connect(p_listview,SIGNAL(signal_delete_view(const QString&)),this,SLOT(slot_delete_view(const QString&)));
   connect(p_listview,SIGNAL(signal_new_view(void)),this,SLOT(slot_new_view(void)));

   connect(p_listview,SIGNAL(signal_viewmode_form(const QString&)),this,SLOT(slot_viewmode_form(const QString&)));
   connect(p_listview,SIGNAL(signal_designmode_form(const QString&)),this,SLOT(slot_designmode_form(const QString&)));
   connect(p_listview,SIGNAL(signal_delete_form(const QString&)),this,SLOT(slot_delete_form(const QString&)));
   connect(p_listview,SIGNAL(signal_new_form(void)),this,SLOT(slot_new_form(void)));

   connect(p_listview,SIGNAL(signal_viewmode_report(const QString&)),this,SLOT(slot_viewmode_report(const QString&)));
   connect(p_listview,SIGNAL(signal_designmode_report(const QString&)),this,SLOT(slot_designmode_report(const QString&)));
   connect(p_listview,SIGNAL(signal_delete_report(const QString&)),this,SLOT(slot_delete_report(const QString&)));
   connect(p_listview,SIGNAL(signal_new_report(void)),this,SLOT(slot_new_report(void)));

   connect(p_listview,SIGNAL(signal_designmode_module(const QString&)),this,SLOT(slot_designmode_module(const QString&)));
   connect(p_listview,SIGNAL(signal_delete_module(const QString&)),this,SLOT(slot_delete_module(const QString&)));
   connect(p_listview,SIGNAL(signal_new_module(void)),this,SLOT(slot_new_module(void)));
   
   connect(p_listview, SIGNAL(signal_contextmenu_requested(QTreeWidgetItem*, const QPoint& )),
   this,SLOT(slot_listbox_popup(QTreeWidgetItem*,const QPoint&)));
   
   }
   
   createGUI(NULL);
   slot_database_selected(0);
   connect(kapp,SIGNAL(lastWindowClosed()),kapp,SLOT(quit()));
   
   KSharedConfigPtr cfg=KGlobal::config();
   KConfigGroup cg = cfg->group("knodamain");
   applyMainWindowSettings(cg);

  if (cl)
  {
    if (cl->p_guicommands->p_load_connection)
    {
   hk_database* db=p_private->p_drivermanager->open_connectionfile(cl->database);
   if (db)
   {
     hk_string dbname=db->name();
     set_connection(db->connection());
     internal_set_database(dbname);
   }
   /*else
     show_warningmessage(hk_translate("Database could not be loaded"));*/
    }
    else
    {
    if (cl->driver.size()>0) set_drivername(cl->driver);
    if (cl->database.size()>0) internal_set_database(cl->database);
    }
  }

}


knodamaindockwindowbase::~knodamaindockwindowbase()
{
  delete p_partmanager;
  delete p_quitaction;
  delete p_databaseaction;
  delete p_openlocaldbaction;
  delete p_private->p_drivermanager;
  delete p_private;
}

hk_connection* knodamaindockwindowbase::connection(void)
{
  return p_connection;
}

void knodamaindockwindowbase::slot_focusCurrent(int /*i*/ )
{
  if( p_tabs->currentWidget() )
    p_tabs->currentWidget()->setFocus();
}

void knodamaindockwindowbase::slot_closeTab(QWidget* w)
{
  if (!w) {
    cerr<<"knodamaindockwindowbase::knodatabs::slot_closeTab\nWARNING! w==NULL"<<endl;
    return;
  }
  w->close();
}

void knodamaindockwindowbase::slot_captionChanged(QWidget* w, const QString  & rName)
{
  p_tabs ->  setTabText(p_tabs -> indexOf(w),rName);
}

void knodamaindockwindowbase::closeEvent(QCloseEvent*event)
{
  KSharedConfigPtr c=KGlobal::config();

 if (!runtime_only()){
    KConfigGroup cg = c->group("knodamain");
    saveMainWindowSettings( cg );
  } 
  for(int i = p_tabs -> count() -1;i >= 0; i--){
    if( ! p_tabs -> widget(i) -> close()) {
      event -> ignore();
      return;
    }
  }
  KParts::MainWindow::closeEvent(event); 
  KApplication::kApplication()->processEvents();
  if (p_database){ 
    p_database->disable();
    delete p_database;
  }
  p_database=NULL;
}

void knodamaindockwindowbase::slot_database_selected(int )
{
if (!p_connection ) return;

  QString newdb;
  if (p_databasecombobox)newdb=p_databasecombobox->currentText();
  if (newdb!=p_private->p_databasename)
  {
    internal_set_database(u2l(newdb.toUtf8().data()));
  }

}

void knodamaindockwindowbase::internal_set_database(const hk_string& newdb)
{
    if (newdb.size()==0) return;
    if (p_connection && !p_connection->database_exists(newdb) )
    {
        show_warningmessage(replace_all("%1",hk_translate("Database '%1' not found"),newdb));
        return;
    }
    if (p_connection && !p_database) 
        p_database=p_connection->new_database();
    if (p_database!=NULL)
    {
        if (p_database->set_name(newdb))
            p_private->p_databasename=QString::fromUtf8(l2u(newdb).c_str());
        else 
            p_private->p_databasename="";
        set_datasource(p_database->new_resultquery());
        p_database->set_usewidgetparent(this);
    }
    if (p_listview)
        p_listview->set_database(p_database);
    p_objecthandleraction->setEnabled(p_database&& p_database->has_centralstoragetable()&&!runtime_only());
    if (p_databasecombobox) {
        //p_databasecombobox->setCurrentText(QString::fromUtf8(l2u(newdb).c_str()));
        int i = p_databasecombobox->findText(p_private->p_databasename);
        if (i != -1)
            p_databasecombobox->setCurrentIndex(i);
        else
            // is not editable
            p_databasecombobox->setItemText(p_databasecombobox->currentIndex(), p_private->p_databasename);           
    }
    if (p_database)
    {  //automatically start "Autoform" when not --strongruntime is set
        if (p_database->form_exists(u2l(autoform.toUtf8().data())))
            slot_viewmode_form(autoform);
    }
}


void knodamaindockwindowbase::set_databases(void)
{
  if (!p_databasecombobox) return;
  QString curtext=p_databasecombobox->currentText();
  if (p_connection&& p_connection->server_needs(hk_connection::NEEDS_DATABASENAME)&& curtext.isEmpty()) curtext=QString::fromUtf8(l2u(p_connection->defaultdatabase()).c_str());
  p_databasecombobox->clear();
  if (!p_connection) return;
  vector<hk_string>* db=p_connection->dblist();
  vector<hk_string>::iterator it;
  p_databasecombobox->blockSignals(true);
  bool dbfound=false;
  if (db!=NULL)
     {
        int i=0;
        for(it=db->begin();it!=db->end();it++)
          {
            p_databasecombobox->addItem(QString::fromUtf8(l2u((*it)).c_str()));
             if (curtext==QString::fromUtf8(l2u((*it)).c_str()))
               {
                p_databasecombobox->setCurrentIndex(i);
		dbfound=true;
               }
            i++;
          }

     }
     if (!dbfound)slot_database_selected(0);

  p_databasecombobox->blockSignals(false);

}

void knodamaindockwindowbase::set_connection(hk_connection* c)
{
  p_connection=c;
  p_database=NULL;
  p_private->p_databasename="";
  if (p_listview)p_listview->set_database(NULL);
  set_datasource(NULL);
  if (!p_connection)set_databases();
  if (p_connection)
    { p_importaction->setEnabled(true);
      p_exportaction->setEnabled(true);
      hk_string s=c->drivername()+" "+c->user()+"@"+c->host()+":"+ulongint2string(c->tcp_port());
      setCaption(QString::fromUtf8(l2u(s).c_str()));
      p_newdatabaseaction->setEnabled(p_connection->server_supports(hk_connection::SUPPORTS_NEW_DATABASE));
      set_databases();
      p_openlocaldbaction->setEnabled(p_connection->server_supports(hk_connection::SUPPORTS_LOCAL_FILEFORMAT));
      p_referentialintegrityaction->setEnabled(p_connection->server_supports(hk_connection::SUPPORTS_REFERENTIALINTEGRITY));
      p_importcsvaction->setEnabled(p_connection->server_supports(hk_connection::SUPPORTS_NEW_TABLE));
    }
if (p_listview)p_listview->setEnabled(p_connection);
if (p_databasecombobox)p_databasecombobox->setEnabled(p_connection);
if (p_private->p_viewcontainer)p_private->p_viewcontainer->setEnabled(p_connection);
p_renameaction->setEnabled(p_connection);
p_copyaction->setEnabled(p_connection);
p_passwordaction->setEnabled(p_connection&&p_connection->server_needs(hk_connection::NEEDS_PASSWORD));

if (p_disconnectaction) p_disconnectaction->setEnabled(p_connection);
p_storeconnection->setEnabled(p_connection&&!runtime_only());
p_newobjectaction->setEnabled(p_connection&&!runtime_only());
p_newaction->setEnabled(p_connection&&!runtime_only());
p_databasesettingaction->setEnabled(p_connection&&!runtime_only());
 }



void knodamaindockwindowbase::exportdbdriver_selected(int index)
{
   vector<hk_string>* drv=hk_drivermanager::driverlist();
    vector<hk_string>::iterator it;
    hk_string drvname;
    int di=0;
    for(it=drv->begin();it!=drv->end();it++)
    {
        if (di==index)
	{
	     drvname=(*it);
	     break;

	}
        di++;
    }
     if (drvname.size()==0) return;


 hk_connection* con=p_private->p_drivermanager->new_connection(drvname);
 if (con!=NULL)
  {
  	if (!con->show_passworddialog())
       	{
      	delete con;
        return;
      	}
       if (con->server_needs(hk_connection::NEEDS_DATABASENAME))
	   {
	     //internal_set_database(con->defaultdatabase());
	   }
       if (con->connect())
        {
	/* TBP   hk_kdeeximportdatabase* dialog=new   hk_kdeeximportdatabase(p_database,con,hk_kdeeximportdatabase::m_export);
	   dialog->exec();
           delete dialog;  */
	}

   delete con;

   }

}


void knodamaindockwindowbase::importdbdriver_selected(int index)
{
  vector<hk_string>* drv=hk_drivermanager::driverlist();
    vector<hk_string>::iterator it;
    hk_string drvname;
    int di=0;
    for(it=drv->begin();it!=drv->end();it++)
    {
        if (di==index)
	{
	     drvname=(*it);
	     break;

	}
        di++;
    }
     if (drvname.size()==0) return;

 hk_connection* con=p_private->p_drivermanager->new_connection(drvname);
 if (con!=NULL)
  {
  	if (!con->show_passworddialog())
       	{
      	delete con;
        return;
      	}
       if (con->server_needs(hk_connection::NEEDS_DATABASENAME))
	   {
	     //internal_set_database(con->defaultdatabase());
	   }
       if (con->connect())
        {
	   /* TBP hk_kdeeximportdatabase* dialog=new   hk_kdeeximportdatabase(p_database,con,hk_kdeeximportdatabase::m_import);
	   dialog->exec();
           delete dialog; */
	}

   delete con;

   }
}

void knodamaindockwindowbase::driver_selected(int index)
{
   vector<hk_string>* drv=hk_drivermanager::driverlist();
    vector<hk_string>::iterator it;
    hk_string drvname;
    int di=0;
    for(it=drv->begin();it!=drv->end();it++)
    {
        if (di==index)
	{
	     drvname=(*it);
	     break;

	}
        di++;
    }
     if (drvname.size()==0) return;

    if (p_connection)
    {
      knodamaindockwindow* w=internal_new_dockwindow();
      w->set_drivername(drvname);
      w->show();
      return;

    }


    hk_connection* c=p_connection;
    if (c)
    {
    set_connection(NULL);

      c->disconnect();
      delete c;
      c=NULL;
    }
    p_private->p_databasename="";
    set_drivername(drvname);


}


knodamaindockwindow* knodamaindockwindowbase::internal_new_dockwindow(void)
{
 struct_commands commands;
 struct_commandlinefields cp;
 cp.p_guicommands=&commands;
 commands.p_showlistwindow=(p_private->p_viewcontainer!=NULL);
 cp.runtime_only=runtime_only()||!commands.p_showlistwindow;
  knodamaindockwindow* w=new knodamaindockwindow(&cp);
 return w;
}

void knodamaindockwindowbase::set_drivername(const hk_string& dr)
{
 hk_connection* con=p_private->p_drivermanager->new_connection(dr);
 if (con!=NULL)
  {
  	if (!con->show_passworddialog())
       	{
      	delete con;
        return;
      	}
       if (con->server_needs(hk_connection::NEEDS_DATABASENAME))
	   {
	     internal_set_database(con->defaultdatabase());
	   }
       if (con->connect())
        {
 	  set_connection(con);
       if (con->server_needs(hk_connection::NEEDS_DATABASENAME))
	   {
	     internal_set_database(con->defaultdatabase());
	   }


	}
	else
	{
	 delete con;

	}
   }
}



void knodamaindockwindowbase::slot_activate_closeaction()
{
  const QList<KParts::Part*> p=p_partmanager->parts();
  p_closeaction->setEnabled(!p.isEmpty());
}


void knodamaindockwindowbase::slot_closewindow()
{
  KParts::Part* p=p_partmanager->activePart();
  if (!p) return;
  p->widget()->close();
}


void knodamaindockwindowbase::slot_disconnect(void)
{

 if (p_connection)p_connection->disconnect();
 delete p_connection;
 p_openlocaldbaction->setEnabled(false);
 p_referentialintegrityaction->setEnabled(false);
 p_objecthandleraction->setEnabled(false);
 p_importaction->setEnabled(false);
 p_exportaction->setEnabled(false);
 set_connection(NULL);
}


void knodamaindockwindowbase::slot_open_localdatabase()
{
    /* TBP
  if (!p_connection) return;
  QStringList l;
  QString xmime=QString::fromUtf8(l2u(p_connection->mimetype()).c_str());
  l.append(xmime);cerr <<"add xmime:"<<xmime.toStdString()<<endl;
  l.append("all/allfiles");
  QString p="kfiledialog:///"+QString::fromUtf8(l2u(p_connection->drivername()).c_str());
  QString filename;
  filename=QString::null;

  if (p_connection->server_needs(hk_connection::NEEDS_DIRECTORY_AS_DATABASE))
  {
    KUrl seldir;
    
    seldir=KFileDialog::getExistingDirectoryUrl(p,this,i18n("Select directory"));
    filename=seldir.directory(NULL);
    if (!filename.isNull())
      if (!seldir.fileName().isEmpty())
      filename+=seldir.fileName();  
  }
  else
  {
    KFileDialog* d=new KFileDialog(KUrl(p),QString::null,this);
    d->setMimeFilter(l,xmime);
    d->exec();
    filename=d->selectedFile();
    delete d;
  }
  if (!filename.isNull())
  {
  cerr <<"FILENAME="<<filename.toStdString()<<endl;
      if (p_databasecombobox)
        p_databasecombobox->addItem(filename);
     internal_set_database(u2l(filename.toUtf8().data()));
  } */
}


void knodamaindockwindowbase::slot_referentialintegrity()
{
   if (!p_private->p_designer)
       p_private->p_designer=new hk_kdedbdesignerwindow();
//    connect (p_private->p_designer,SIGNAL(signal_closed()),this,SLOT(designer_deleted()));
    p_private->p_designer->designer()->set_database(p_database);
    hk_referentialintegrity* rf=new hk_referentialintegrity();
    rf->set_database(p_database);
    rf->load_referentialintegrity();
    p_private->p_designer->designer()->set_presentation(rf);
    //set_block_has_changed(true);
    p_private->p_designer->show();
    //set_block_has_changed(false);
    connect (p_private->p_designer,SIGNAL(signal_closed()),this,SLOT(designer_deleted()));
}

void knodamaindockwindowbase::designer_deleted(void)
{
cerr<<"designer_deleted"<<endl;

p_private->p_designer=NULL;
}



void knodamaindockwindowbase::slot_load_connection()
{
  /* TBP if (!p_private->p_drivermanager) return;
  QStringList l;
  QString xmime="application/x-hk_connection";
  l.append(xmime);
  KFileDialog* d=new KFileDialog(KUrl("kfiledialog:///hkc"),QString::null,this);
  d->setMimeFilter(l,xmime);
  d->exec();
  QString filename=d->selectedFile();
  if (!filename.isNull())
  {
//execute new connection
   hk_database* db=p_private->p_drivermanager->open_connectionfile(u2l(filename.toUtf8().data()));
   if (db)
   {
     hk_string dbname=db->name();
    if (p_connection)
    {
       knodamaindockwindow* w=internal_new_dockwindow();

	w->set_connection(db->connection());
	w->internal_set_database(dbname);
	w->show();

    }
    else
    {
     set_connection(db->connection());
     internal_set_database(dbname);
    }
   }
  }
  delete d; */
}


void knodamaindockwindowbase::slot_store_connection()
{
 /* TBP if (!p_database) return;
  QStringList l;
  QString xmime="application/x-hk_connection";
  l.append(xmime);
  cerr <<"add xmime:"<<xmime.toStdString()<<endl;
  KFileDialog* d=new KFileDialog(KUrl("kfiledialog:///hkc"),QString::null,this);
  d->setMimeFilter(l,xmime);
  d->setOperationMode(KFileDialog::Saving);
  d->exec();

  QString filename=d->selectedFile();
  if (!filename.isNull())
  {
    KSharedConfigPtr c=KGlobal::config();
    KConfigGroup cg = c->group("Preferences");
    bool g=cg.readEntry("StorePassword",false);
    p_database->store_connectionfile(u2l(filename.toUtf8().data()),g);
  }
  delete d; */
}



