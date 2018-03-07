// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2013 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
//
// This file is part of the hk_kde4classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_KDEEXIMPORTDATABASE_H
#define HK_KDEEXIMPORTDATABASE_H
#include "hk_kdeeximportdatabasebase.h"
#include <hk_dbvisible.h>
class hk_connection;
class hk_database;
class hk_kdedblistview;
class QProgressDialog;


class hk_kdeeximportdatabase : public hk_kdeeximportdatabasebase, public hk_dbvisible
{
    Q_OBJECT

public:
    enum enum_mode{m_import,m_export};
    hk_kdeeximportdatabase( hk_database* db, hk_connection* con, enum_mode mode, QWidget* parent = 0, 
			    const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~hk_kdeeximportdatabase();

public slots:
    void upload_clicked();
    void slot_selection_changed();
    void slot_database_selected(int);
    void copying_cancelled();
    void leftnewbutton_clicked();
    void rightnewbutton_clicked();
      virtual void help_clicked();


    protected:
 static  bool set_progress(long int,long int,const hk_string&);
   virtual void        list_changes(listtype type);
     void        dblist_changes(void);
virtual void keyPressEvent ( QKeyEvent * e );

   protected slots:

   virtual void accept(void);

    private:
    void set_objects(void);
    void upload_file(void);
    void upload_file(const hk_string&,filetype );
    void copy_whole_database(void);
    void copy_all_tables(void);
    void copy_all_queries(void);
    void copy_all_views(void);
    void copy_all_forms(void);
    void copy_all_reports(void);
    void copy_all_files(filetype);
    enum_mode p_mode;
    hk_kdedblistview* p_left;
    hk_kdedblistview* p_right;
    hk_database* p_database;
    hk_database* p_connectiondatabase;
    hk_connection* p_connection;
static QProgressDialog* p_progressdialog;
static bool p_cancelcopying;
};

#endif // HK_KDEEXIMPORTDATABASE_H
