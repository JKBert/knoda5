// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2014 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
//
// This file is part of the hk_kde4classes library.
// This file may be distributed and/or modified under the terms of the
// GNU General Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef HK_KDEINDEXEDITDIALOG_H
#define HK_KDEINDEXEDITDIALOG_H

#include <qvariant.h>
#include <qwidget.h>
#include <hk_datasource.h>
#include <hk_dsvisible.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QCheckBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QListBoxItem;
class QPushButton;
class QToolButton;

/**
 *
 *@short KDE widget to create and alter indices of a table.
 *@version $Revision: 1.7 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *
 */

class hk_kdeindexeditdialog : public QWidget, public hk_dsvisible
{
    Q_OBJECT

        public:
        hk_kdeindexeditdialog( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
        ~hk_kdeindexeditdialog();

    public slots:
        virtual void addbutton_clicked();
        virtual void deletebutton_clicked();
        virtual void exitbutton_clicked();
        virtual void okbutton_clicked();
        virtual void set_indexvalues(const hk_string& name,bool unique,list<hk_string>& fields,bool newindex=false);
        virtual void check_enablebuttons(void);
        virtual void listentries_changed();
    protected:
        virtual void before_source_vanishes(void);
        QHBoxLayout* hk_kdeindexeditdialogLayout;
        QVBoxLayout* Layout4;
        QVBoxLayout* Layout7;
        QVBoxLayout* Layout2;
        QVBoxLayout* Layout5;
        QVBoxLayout* Layout13;
        QVBoxLayout* Layout11;
        QHBoxLayout* Layout12;
        QVBoxLayout* ButtonGroup7Layout;
        bool p_changed;
        bool in_list(const hk_string& f,list<hk_string>& l);
        QLabel* TextLabel1_2;
        QListWidget* dsfieldlist;
        QToolButton* addbutton;
        QToolButton* deletebutton;
        QLabel* TextLabel2_2;
        QListWidget* indexfieldlist;
        QLabel* TextLabel1;
        QLineEdit* indexnamefield;
        QLabel* TextLabel2;
        QCheckBox* uniquefield;
        QGroupBox* ButtonGroup7;
        QPushButton* storebutton;
        QPushButton* exitbutton;

};
#endif                                            // HK_KDEINDEXEDITDIALOG_H
