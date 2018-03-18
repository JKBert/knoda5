// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for knoda -Knorr's Datenbank.
// copyright (c) 2010-2014 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for knoda4 -Knorr's Datenbank.
//
// This file is part of the knoda4 -Knorr's Datenbank.
// This file may be distributed and/or modified under the terms of the
// GNU General Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
/****************************************************************************
** Form interface generated from reading ui file 'hk_kdedriverselect.ui'
**
** Created: Mo Aug 29 07:49:24 2005
**      by: The User Interface Compiler ($Id: hk_kdedriverselectbase.h,v 1.4 2005/08/31 17:59:17 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEDRIVERSELECTBASE_H
#define HK_KDEDRIVERSELECTBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTabWidget;
class QWidget;
class QListWidget;

class hk_kdedriverselectbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdedriverselectbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdedriverselectbase();

    QPushButton* connectbutton;
    QPushButton* cancelbutton;
    QTabWidget* tabwidget;
    QWidget* tab;
    QListWidget* driverlist;
    QWidget* tab_2;
    QPushButton* optionbutton;
    QPushButton* buttonHelp;

public slots:
    virtual void list_clicked();
    virtual void tabwidget_changed();
    virtual void help_clicked();
    virtual void option_clicked();

protected:
    QGridLayout* hk_kdedriverselectbaseLayout;
    QSpacerItem* spacer1;
    QVBoxLayout* tabLayout;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEDRIVERSELECTBASE_H
