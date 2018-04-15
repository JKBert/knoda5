// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
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
/****************************************************************************
** Form interface generated from reading ui file 'formdatasourcedialog.ui'
**
** Created: Sa Sep 17 17:17:56 2005
**      by: The User Interface Compiler ($Id: hk_kdeformdatasourcedialogbase.h,v 1.19 2005/09/17 19:11:18 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEFORMDATASOURCEBASEDIALOG_H
#define HK_KDEFORMDATASOURCEBASEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QFrame;
class QComboBox;
class QLabel;
class QTextEdit;
class QLineEdit;
class QToolButton;
class QCheckBox;
class QTreeWidget;

class hk_kdeformdatasourcebasedialog : public QDialog
{
    Q_OBJECT

public:
    hk_kdeformdatasourcebasedialog( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdeformdatasourcebasedialog();

    QPushButton* Addbutton;
    QPushButton* alterbutton;
    QPushButton* deletebutton;
    QPushButton* buttonCancel;
    QFrame* Frame15;
    QComboBox* datasourcefield;
    QComboBox* sourcetypefield;
    QLabel* basedontext;
    QTextEdit* sqlfield;
    QLabel* sqltext;
    QFrame* Frame17;
    QLabel* filtertext;
    QTextEdit* filterfield;
    QFrame* Frame6;
    QLabel* sortingtext;
    QLineEdit* sortingfield;
    QFrame* Frame3;
    QToolButton* deletefieldbutton;
    QLabel* dependingmodelabel;
    QToolButton* addfieldbutton;
    QLabel* dependingontext;
    QLabel* thistext;
    QLabel* mastertext;
    QComboBox* thisfield;
    QComboBox* masterfield;
    QComboBox* dependingonfield;
    QComboBox* dependingmodefield;
    QCheckBox* react_on_changesfield;
    QCheckBox* readonlyfield;
    QTreeWidget* dependingfieldlist;
    QCheckBox* automaticupdatefield;

public slots:
    virtual void add_dependingfields();
    virtual void alter_datasource();
    virtual void check_buttons();
    virtual void datasource_selected();
    virtual void delete_dependingfields();
    virtual void dependingon_selected();
    virtual void new_datasource();
    virtual void delete_datasource();
    virtual void set_datasourcelist();
    virtual void sql_changed();

protected:
    QGridLayout* hk_kdeformdatasourcebasedialogLayout;
    QVBoxLayout* Layout8;
    QSpacerItem* Spacer1;
    QGridLayout* Frame15Layout;
    QHBoxLayout* Frame17Layout;
    QHBoxLayout* Frame6Layout;
    QGridLayout* Frame3Layout;
    QGridLayout* Layout12;
    QSpacerItem* Spacer2;
    QSpacerItem* Spacer3;
    QSpacerItem* Spacer2_2;
    QSpacerItem* Spacer2_3;
    QSpacerItem* Spacer1_2;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEFORMDATASOURCEBASEDIALOG_H
