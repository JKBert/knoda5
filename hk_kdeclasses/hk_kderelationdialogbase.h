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
/****************************************************************************
** Form interface generated from reading ui file 'kderelationdialogbase.ui'
**
** Created: Sa Mär 11 15:53:50 2006
**      by: The User Interface Compiler ($Id: hk_kderelationdialogbase.h,v 1.6 2006/03/12 12:08:49 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDERELATIONDIALOGBASE_H
#define HK_KDERELATIONDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QStackedWidget;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QWidget;
class QLabel;
class QComboBox;
class QGroupBox;
class QRadioButton;
class QPushButton;
class QTableWidget;

class hk_kderelationdialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kderelationdialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kderelationdialogbase();

    QStackedWidget* detailbox;
    QWidget* WStackPage;
    QGroupBox* integritybox;
    QLabel* onupdatelabel;
    QLabel* ondeletelabel;
    QComboBox* onupdatefield;
    QComboBox* ondeletefield;
    QWidget* WStackPage_2;
    QGroupBox* joingroup;
    QRadioButton* button_all;
    QRadioButton* button_equivalent;
    QPushButton* ok_button;
    QPushButton* cancel_button;
    QTableWidget* grid;
    static const int MASTER_COL = 0;
    static const int SLAVE_COL = 1;    
public slots:
    virtual void ok_button_clicked();

protected:
    QGridLayout* hk_kderelationdialogbaseLayout;
    QGridLayout* WStackPageLayout;
    QGridLayout* integrityboxLayout;
    QHBoxLayout* WStackPageLayout_2;
    QVBoxLayout* joingroupLayout;
    QVBoxLayout* Layout5;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDERELATIONDIALOGBASE_H
