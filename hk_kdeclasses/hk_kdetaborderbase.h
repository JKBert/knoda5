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
/****************************************************************************
** Form interface generated from reading ui file 'taborder.ui'
**
** Created: Sa Jul 1 14:50:12 2006
**      by: The User Interface Compiler ($Id: hk_kdetaborderbase.h,v 1.4 2006/07/01 13:32:21 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDETABORDERDIALOGBASE_H
#define HK_KDETABORDERDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class QTreeWidget;
class QToolButton;
class QPushButton;

class hk_kdetaborderdialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdetaborderdialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdetaborderdialogbase();

    QFrame* Frame14;
    QLabel* baselistlabel;
    QTreeWidget* baselist;
    QToolButton* addbutton;
    QToolButton* deletebutton;
    QToolButton* upbutton;
    QToolButton* downbutton;
    QLabel* boxlabel_2_2;
    QTreeWidget* taborderlist;
    QPushButton* okbutton;
    QPushButton* cancelbutton;

public slots:
    virtual void add_clicked();
    virtual void check_buttons();
    virtual void delete_clicked();
    virtual void down_clicked();
    virtual void up_clicked();
    virtual void ok_clicked();

protected:
    QHBoxLayout* hk_kdetaborderdialogbaseLayout;
    QHBoxLayout* Frame14Layout;
    QVBoxLayout* layout7;
    QVBoxLayout* Layout4;
    QSpacerItem* Spacer2;
    QSpacerItem* Spacer1_2;
    QVBoxLayout* taborderlistlabel;
    QVBoxLayout* layout6;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDETABORDERDIALOGBASE_H
