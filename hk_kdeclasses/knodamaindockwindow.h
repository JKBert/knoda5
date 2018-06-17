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

#ifndef KNODAMAINDOCKWINDOW_H
#define KNODAMAINDOCKWINDOW_H

#include <kde4classesmacros.h>
#include "knodamaindockwindowbase.h"

class hk_kdetablepartwidget;
class hk_kdequerypartwidget;
class hk_kdeformpartwidget;
class hk_kdereportpartwidget;
class hk_kdemodulepartwidget;

typedef void show_dbfunction(int);

class KDECLASSES_EXPORT knodamaindockwindow : public knodamaindockwindowbase
{
Q_OBJECT

public:
knodamaindockwindow(struct_commandlinefields* commandline=NULL,QWidget* parent=0L,const char* name=0L, Qt::WFlags f=Qt::Window,const QString& dbname="",const QString& form="");
virtual ~knodamaindockwindow();
 
 hk_kdetablepartwidget* new_table(void);
 hk_kdequerypartwidget* new_query(bool as_view=false);
 hk_kdeformpartwidget*  new_form(void);
 hk_kdemodulepartwidget*  new_module(void);
 hk_kdereportpartwidget*new_report(void);

 using KMainWindow::setCaption;
 
public slots:

 virtual void setCaption(const QString&);
 virtual void slot_new_database(void);
 virtual void slot_delete_database(const QString&);


 virtual void slot_new_table(void);
 virtual void slot_delete_table(const QString&);
 virtual void slot_designmode_table(const QString&);
 virtual void slot_viewmode_table(const QString&);

 virtual void slot_new_query(void);
 virtual void slot_delete_query(const QString&);
 virtual void slot_designmode_query(const QString&);
 virtual void slot_viewmode_query(const QString&);

 virtual void slot_new_view(void);
 virtual void slot_delete_view(const QString&);
 virtual void slot_designmode_view(const QString&);
 virtual void slot_viewmode_view(const QString&);

 virtual void slot_new_form(void);
 virtual void slot_delete_form(const QString&);
 virtual void slot_designmode_form(const QString&);
 virtual void slot_viewmode_form(const QString&);

 virtual void slot_new_report(void);
 virtual void slot_delete_report(const QString&);
 virtual void slot_designmode_report(const QString&);
 virtual void slot_viewmode_report(const QString&);
 
 
 virtual void slot_new_module(void);
 virtual void slot_delete_module(const QString&);
 virtual void slot_designmode_module(const QString&);

virtual void rename_clicked();
virtual void importcsv_clicked();
virtual void exportcsv_clicked();
virtual void exportxml_clicked();
virtual void newpassworddialog_selected(void);
virtual void copy_clicked(void);
virtual void paste_clicked(void);
virtual void clipboarddata_has_changed(void);
virtual void slot_listbox_popup(QTreeWidgetItem*,const QPoint&);
virtual void databasesettings_clicked(void);
virtual void objecthandler_clicked(void);
virtual void newwindow_clicked(void); 
virtual void settings_clicked(void);
virtual void show_drivers();
virtual void show_exportdbdrivers();
virtual void show_importdbdrivers();
 
 void slot_raise_form(const hk_string&); 
 protected slots:
 
 void slot_selection_changed(void);
 void slot_trigger_driver(QAction* action);
 void slot_trigger_exportdbdriver(QAction* action);
 void slot_trigger_importdbdriver(QAction* action);
 
 private:
 enum enum_function{f_connect,f_exportdb,f_importdb};
 void show_drivers(QMenu* menu);
hk_kdeformpartwidget* find_existing_form(const hk_string& n);
hk_kdereportpartwidget* find_existing_report(const hk_string& n);
hk_kdetablepartwidget* find_existing_table(const hk_string& n);
hk_kdequerypartwidget* find_existing_query(const hk_string& n);
hk_kdequerypartwidget* find_existing_view(const hk_string& n);
hk_kdemodulepartwidget* find_existing_module(const hk_string& n);
};


#endif //KNODAMAINDOCKWINDOW_H
