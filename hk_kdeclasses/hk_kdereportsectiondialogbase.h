// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2014 Patrik Hanak <hanakp@users.sourceforge.net>
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
/****************************************************************************
** Form interface generated from reading ui file 'reportsection.ui'
**
** Created: Sa Jul 30 22:09:41 2005
**      by: The User Interface Compiler ($Id: hk_kdereportsectiondialogbase.h,v 1.4 2005/07/31 12:39:05 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEREPORTSECTIONDIALOGBASE_H
#define HK_KDEREPORTSECTIONDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QComboBox;
class QToolButton;
class QListWidget;
class QListBoxItem;
class QPushButton;

class hk_kdereportsectiondialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdereportsectiondialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdereportsectiondialogbase();

    QFrame* Frame14;
    QLabel* fieldlabel;
    QComboBox* sectionfield;
    QToolButton* addbutton;
    QToolButton* deletebutton;
    QToolButton* upbutton;
    QToolButton* downbutton;
    QListWidget* sectionbox;
    QLabel* boxlabel;
    QPushButton* exitbutton;

public slots:
    virtual void add_clicked();
    virtual void check_buttons();
    virtual void delete_clicked();
    virtual void down_clicked();
    virtual void up_clicked();

protected:
    QHBoxLayout* hk_kdereportsectiondialogbaseLayout;
    QHBoxLayout* Frame14Layout;
    QVBoxLayout* Layout5;
    QSpacerItem* Spacer3;
    QVBoxLayout* Layout4;
    QSpacerItem* Spacer2;
    QSpacerItem* Spacer1_2;
    QGridLayout* Layout10;
    QVBoxLayout* Layout11;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEREPORTSECTIONDIALOGBASE_H
