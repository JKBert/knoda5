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

#ifndef HK_KDETABLEDESIGN_H
#define HK_KDETABLEDESIGN_H
#include <hk_dsgrid.h>
#include <hk_dsvisible.h>
#include <qvariant.h>
#include <qwidget.h>
#include <hk_column.h>
#include <qstring.h>
#include <qstringlist.h>
#include <list>

class QTreeWidgetItem;
class QTreeWidget;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QButtonGroup;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QTreeWidget;
class QPushButton;
class QSpinBox;
class QToolButton;
class hk_kdeindexwindow;

/**
 *
 *@short  KDE widget to alter tables.
 *@version $Revision: 1.22 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *hk_kdetabledesign is a widget to interactively define and alter the
 *table structure.
 *
 */

class hk_kdetabledesign : public QWidget ,public hk_dsvisible
{
    Q_OBJECT

        public:
        hk_kdetabledesign( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
        virtual    ~hk_kdetabledesign();

        virtual void set_datasource(hk_datasource* d);
        hk_datasource* datasource(void);
	bool has_changed() const;
	signals:
	void signal_has_changed();
	void signal_tablename_changed();
    public slots:
/**
 *call this function at the end to alter the table
 */
        virtual bool alter_table(bool ask=true);
        virtual void load_fieldinfo();
        void set_designmode();
        virtual void alterbutton_clicked();
    protected slots:
        virtual void delete_clicked();
        virtual void index_clicked();
        virtual void name_changed();
        virtual void new_clicked();
        virtual void primary_changed();
        virtual void notnull_changed();
        virtual void selection_changed();
        virtual void indexselection_changed();
        virtual void size_changed();
        virtual void type_changed();
        virtual void new_index();
        virtual void alter_index();
        virtual void delete_index();
        virtual void set_indexlist(void);

    protected:

        virtual void showEvent(QShowEvent* e);
        virtual void    list_changes(listtype type);
	void set_fields_enabled(void);
        QGridLayout* hk_kdetabledesignLayout;
        QHBoxLayout* Layoutindexwindow;
        QVBoxLayout* buttonlayout;
        QHBoxLayout* Layout8;
        QHBoxLayout* Layout7;
        QVBoxLayout* Layout22;
        QHBoxLayout* Layout25;
        QVBoxLayout* Layout24;
        QHBoxLayout* Layout23;
        QVBoxLayout* editlayout;
        QVBoxLayout* fieldbuttonlayout;
        QHBoxLayout* fieldtoplayout;

        QTreeWidget* fieldlist;
        QPushButton* newbutton;
        QPushButton* deletebutton;
        QPushButton* alterbutton;
        QPushButton* indexdeletebutton;
        QPushButton* indexnewbutton;
        QPushButton* indexalterbutton;
        QLabel* fieldnamelabel;
        QLabel* columntypelabel;
        QLabel* sizelabel;
        QLabel* notnulllabel;
        QLabel* primaryindexlabel;
        QLineEdit* namefield;
        QComboBox* typefield;
        QSpinBox* sizefield;
        QCheckBox* primaryfield;
        QCheckBox* notnullfield;
        QTreeWidget* indexwindow;

        void create_columntypelist(void);
    private :
        void structure_changes(bool c);
        void clear_fieldlist(void);
        hk_column::enum_columntype ftype(const QString&);
        enum enum_state {unchanged,delfield,newfield,altered};
        int fieldname_count(const hk_string&);
/**
 * returns true if name is a SQL key word
 */
        //bool is_keyword(const QString&);
        typedef  class  fieldstruct
        {
            public:
                hk_string oldname;
                bool   oldprimary;
                bool   oldnotnull;
                hk_column::enum_columntype oldtype;
                long    oldsize;
                hk_string newname;
                bool   newprimary;
                bool   newnotnull;
                hk_column::enum_columntype newtype;
                long    newsize;
                enum_state state;
        };
        typedef class dictclass
        {
            public:
                QString name;
                hk_column::enum_columntype type;
        };
        void show_fieldinfos(fieldstruct*);
        list<dictclass>   columntype_dictionary;
        list<fieldstruct> fields;
        list<fieldstruct>::iterator actualfieldit;
        QTreeWidgetItem* lastmarkedItem;
        bool p_fieldlist_created;
        bool p_has_changed;
        bool fieldnamecheck;
//        QStringList p_keywordlist;
};
#endif                                            // HK_KDETABLEdesign_H
