// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2013 Patrik Hanak <hanakp@users.sourceforge.net>
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
** Form interface generated from reading ui file 'newpassworddialog.ui'
**
** Created: Sa Jul 30 19:27:47 2005
**      by: The User Interface Compiler ($Id: hk_kdenewpassworddialogbase.h,v 1.3 2005/07/31 12:39:05 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDENEWPASSWORDDIALOGBASE_H
#define HK_KDENEWPASSWORDDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;

class hk_kdenewpassworddialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdenewpassworddialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdenewpassworddialogbase();

    QLabel* repeatlabel;
    QLineEdit* passwordfield;
    QLineEdit* repeatfield;
    QLabel* passwordlabel;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QLabel* headlinelabel;

public slots:
    virtual void data_changed();
    virtual void ok_clicked();

protected:
    QGridLayout* hk_kdenewpassworddialogbaseLayout;
    QVBoxLayout* Layout3;
    QSpacerItem* Spacer1_2;
    QGridLayout* Layout2;
    QVBoxLayout* Layout5;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDENEWPASSWORDDIALOGBASE_H
