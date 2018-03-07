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
 //$Revision: 1.12 $

#ifndef HK_KDEDBLISTVIEW_H
#define HK_KDEDBLISTVIEW_H
#include <qtreewidget.h>
#include <qdialog.h>

#include <hk_dbvisible.h>

class hk_kdedblistviewprivate;
class QProgressDialog;
class hk_kdeschemadialog;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QLabel;
class QPushButton;
class QRadioButton;
class KAction;
class KActionMenu;


class hk_kdedblistview: public QTreeWidget, public hk_dbvisible
{
Q_OBJECT
public:
hk_kdedblistview (QWidget* parent=0L, const char* name=0L);
virtual ~hk_kdedblistview();
void set_database(hk_database*);
bool is_tableitem(QTreeWidgetItem*,bool includingheader=false) const;
bool is_queryitem(QTreeWidgetItem*,bool includingheader=false) const;
bool is_viewitem(QTreeWidgetItem*,bool includingheader=false) const;
bool is_formitem(QTreeWidgetItem*,bool includingheader=false) const;
bool is_reportitem(QTreeWidgetItem*,bool includingheader=false) const;
bool is_moduleitem(QTreeWidgetItem*,bool includingheader=false) const;
bool is_databaseitem(QTreeWidgetItem*) const;
bool is_headeritem(QTreeWidgetItem*) const;
bool is_tableitem(bool includingheader=false) const;
bool is_queryitem(bool includingheader=false) const;
bool is_viewitem(bool includingheader=false) const;
bool is_formitem(bool includingheader=false) const;
bool is_reportitem(bool includingheader=false) const;
bool is_moduleitem(bool includingheader=false) const;
bool is_databaseitem(void) const;
bool is_headeritem(void) const;
bool is_tableheader(void) const;
bool is_queryheader(void) const;
bool is_viewheader(void) const;
bool is_formheader(void) const;
bool is_reportheader(void) const;
bool is_tableheader(QTreeWidgetItem*) const;
bool is_queryheader(QTreeWidgetItem*) const;
bool is_viewheader(QTreeWidgetItem*) const;
bool is_formheader(QTreeWidgetItem*) const;
bool is_reportheader(QTreeWidgetItem*) const;
bool is_moduleheader(QTreeWidgetItem*) const;
bool cancel(void){return p_cancelcopying;}
void reload_lists(void);
enum enum_showmode {sm_all,sm_central,sm_local};

void set_showmode(enum_showmode);
enum_showmode showmode(void) const;

static void set_use_singleclick(bool);
static bool use_singleclick(void) ;
virtual bool canAccept(const QMimeData* mimedata);


signals:

 void signal_new_database(void);
 void signal_delete_database(const QString&);
 void signal_reload_databases(void);

 void signal_new_table(void);
 void signal_delete_table(const QString&);
 void signal_designmode_table(const QString&);
 void signal_viewmode_table(const QString&);

 void signal_new_view(void);
 void signal_delete_view(const QString&);
 void signal_designmode_view(const QString&);
 void signal_viewmode_view(const QString&);

 void signal_new_query(void);
 void signal_delete_query(const QString&);
 void signal_designmode_query(const QString&);
 void signal_viewmode_query(const QString&);

 void signal_new_form(void);
 void signal_delete_form(const QString&);
 void signal_designmode_form(const QString&);
 void signal_viewmode_form(const QString&);

 void signal_new_report(void);
 void signal_delete_report(const QString&);
 void signal_designmode_report(const QString&);
 void signal_viewmode_report(const QString&);


 void signal_new_module(void);
 void signal_delete_module(const QString&);
 void signal_designmode_module(const QString&);
 
 void signal_contextmenu_requested(QTreeWidgetItem*, const QPoint& );

public slots:
virtual void new_clicked();
virtual void start_clicked();
virtual void alter_clicked();
virtual void delete_clicked();
        void copy(void);
        void paste(void);
        void copying_cancelled();


protected slots:

void item_clicked(QTreeWidgetItem*);
void item_doubleclicked(QTreeWidgetItem*);
void slot_customcontextmenu(const QPoint& pnt);


protected:
static  bool set_progress(long int,long int,const hk_string&);

        void item_selected(QTreeWidgetItem* );
virtual void list_changes(listtype type);

 bool decode_action(const QMimeData* event);
 bool copy_presentationfile(void);
 bool copy_table(void);
 bool copy_view(void);
 bool copy_database(void);
 hk_connection* find_connection(void);
 virtual QMimeData* mimeData(const QList<QTreeWidgetItem*> items) const;
 virtual bool dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data, Qt::DropAction action);
 virtual QStringList mimeTypes() const;
 virtual Qt::DropActions supportedDropActions() const;

private:
objecttype current_objecttype() const;
void init_listview(void);
void set_databasename(void);
void set_tables(void);
void set_views(void);
void set_queries(void);
void set_forms(void);
void set_reports(void);
void set_modules(void);
bool execute_action(void);
hk_kdedblistviewprivate* p_private;
static QProgressDialog* p_progressdialog;
static bool p_cancelcopying;
static bool p_use_singleclick;
};

class hk_kdeschemadialog : public QDialog
{
    Q_OBJECT

        public:
        hk_kdeschemadialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
        ~hk_kdeschemadialog();

        QGroupBox* ButtonGroup1;
        QRadioButton* schemaonlyfield;
        QRadioButton* datafield;
        QPushButton* buttonOk;
        QPushButton* buttonCancel;

    protected:
        QHBoxLayout* hk_kdeschemadialogLayout;
        QVBoxLayout* ButtonGroup1Layout;
        QVBoxLayout* Layout2;
};


#endif //HK_KDEDBLISTVIEW_H
