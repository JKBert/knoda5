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

#ifndef KNODAMAINDOCKWINDOWBASE_H
#define KNODAMAINDOCKWINDOWBASE_H
#include <KAction>
#include <kparts/partmanager.h>
#include <kparts/mainwindow.h>
#include <hk_dsvisible.h>
#include <hk_string.h>
#include <qstring.h>
#include <KTabWidget>

const QString autoform="Autoform";

 typedef class struct_commands
{
 public:
 bool p_showlistwindow;
 bool p_load_connection;
 bool p_classic;
 struct_commands()
 {
   p_showlistwindow=true;
   p_load_connection=false;
   p_classic=false;
 }
};


class knodamaindockwindowbaseprivate;
class knodamaindockwindow;
class hk_kdedblistview;
class hk_connection;
class hk_database;
class QTreeWidgetItem;
class QToolButton;
class KComboBox;
class KActionMenu;

class knodamaindockwindowbase : public KParts::MainWindow, public hk_dsvisible
{
Q_OBJECT
public:
knodamaindockwindowbase(struct_commandlinefields* commandline=NULL,QWidget* parent=0L,const char* name=0L, Qt::WFlags f=Qt::Window,
			const QString& dbname="",const QString& form="");

virtual ~knodamaindockwindowbase();
hk_connection* connection(void);
void set_connection(hk_connection*);
void set_drivername(const hk_string&);
public slots:
void slot_database_selected(int);
 virtual void slot_new_database(void){}
 virtual void slot_delete_database(const QString&){}

 virtual void slot_new_table(void){}
 virtual void slot_delete_table(const QString&){}
 virtual void slot_designmode_table(const QString&){}
 virtual void slot_viewmode_table(const QString&){}

 virtual void slot_new_query(void){}
 virtual void slot_delete_query(const QString&){}
 virtual void slot_designmode_query(const QString&){}
 virtual void slot_viewmode_query(const QString&){}

 virtual void slot_new_view(void){}
 virtual void slot_delete_view(const QString&){}
 virtual void slot_designmode_view(const QString&){}
 virtual void slot_viewmode_view(const QString&){}

 virtual void slot_new_form(void){}
 virtual void slot_delete_form(const QString&){}
 virtual void slot_designmode_form(const QString&){}
 virtual void slot_viewmode_form(const QString&){}

 virtual void slot_new_report(void){}
 virtual void slot_delete_report(const QString&){}
 virtual void slot_designmode_report(const QString&){}
 virtual void slot_viewmode_report(const QString&){}

 virtual void slot_new_module(void){}
 virtual void slot_delete_module(const QString&){}
 virtual void slot_designmode_module(const QString&){}
 
 
 virtual void newwindow_clicked(){}
virtual void rename_clicked(){}
virtual void importcsv_clicked(){}
virtual void exportcsv_clicked(){}
virtual void exportxml_clicked(){}
virtual void newpassworddialog_selected(void){}
virtual void copy_clicked(void){}
virtual void paste_clicked(void){}
virtual void clipboarddata_has_changed(void){}
virtual void databasesettings_clicked(void) {}
virtual void objecthandler_clicked(void) {}
virtual void slot_listbox_popup(QTreeWidgetItem*,const QPoint&){}
virtual void settings_clicked(void){}
virtual void show_drivers(){}
virtual void show_exportdbdrivers(){}
virtual void show_importdbdrivers(){}
virtual void slot_closewindow();
void slot_activate_closeaction();
void set_databases(void);
void driver_selected(int);
void exportdbdriver_selected(int);
void importdbdriver_selected(int);
void slot_disconnect(void);
void slot_open_localdatabase();
void slot_load_connection();
void slot_store_connection();
void slot_referentialintegrity();
	void designer_deleted(void);

protected slots:
  void slot_focusCurrent(int i);
  void slot_closeTab(QWidget* w);
  void slot_captionChanged(QWidget* w, const QString  & rName);
  
protected:  
virtual void closeEvent(QCloseEvent*);
inline KTabWidget* tabs() {return p_tabs;};

KAction*    p_databaseaction;
KComboBox*        p_databasecombobox;
KAction*          p_closeaction;
KAction*          p_quitaction;

KActionMenu* p_newobjectaction;
KActionMenu* p_newconnectionaction;
KActionMenu* p_displaymodeaction;

KAction* p_disconnectaction;
KAction* p_newdatabaseaction;

KAction* p_newtableaction;
KAction* p_newqueryaction;
KAction* p_newviewaction;
KAction* p_newformaction;
KAction* p_newreportaction;
KAction* p_newmoduleaction;
KAction* p_newaction;
KAction* p_alteraction;
KAction* p_startaction;
KAction* p_deleteaction;
KAction* p_copyaction;
KAction* p_pasteaction;
KAction* p_renameaction;
KActionMenu* p_exportaction;
KAction* p_exportcsvaction;
KAction* p_exportxmlaction;
KActionMenu* p_exportdatabaseaction;
KActionMenu* p_importaction;
KAction* p_importcsvaction;
KActionMenu* p_importdatabaseaction;
KAction* p_passwordaction;
KAction* p_databasesettingaction;
KAction* p_objecthandleraction;
KAction* p_newwindowaction;
KAction* p_settingsaction;
KAction* p_openlocaldbaction;
KAction* p_referentialintegrityaction;

KAction* p_loadconnection;
KAction* p_storeconnection;

QToolButton* newbutton;
QToolButton* alterbutton;
QToolButton* startbutton;
QToolButton* deletebutton;

hk_kdedblistview* p_listview;
hk_database*      p_database;
hk_connection*    p_connection;
KTabWidget*        p_tabs;
KParts::PartManager* p_partmanager;

private:
void internal_set_database(const hk_string&);
knodamaindockwindow* internal_new_dockwindow(void);
knodamaindockwindowbaseprivate* p_private;
};


#endif //KNODAMAINDOCKWINDOWBASE_H
