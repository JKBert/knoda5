// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2014 Patrik Hanak <hanakp@users.sourceforge.net>
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
** Form interface generated from reading ui file 'finddialog.ui'
**
** Created: Sa Jul 30 18:50:16 2005
**      by: The User Interface Compiler ($Id: hk_kdefinddialogbase.h,v 1.4 2005/07/31 12:39:05 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEFINDDIALOGBASE_H
#define HK_KDEFINDDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QGroupBox;
class QCheckBox;
class QComboBox;

class hk_kdefinddialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdefinddialogbase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~hk_kdefinddialogbase();

    QLabel* searchlabel;
    QPushButton* ok_button;
    QPushButton* cancel_button;
    QGroupBox* optionsgroup;
    QCheckBox* case_sensitivebox;
    QCheckBox* all_columnsbox;
    QCheckBox* part_of_columnbox;
    QCheckBox* find_backwardsbox;
    QComboBox* searchfield;

public slots:
    virtual void find_slot();
    virtual void new_findargument();

protected:
    QGridLayout* hk_kdefinddialogbaseLayout;
    QVBoxLayout* Layout5;
    QSpacerItem* Spacer1;
    QGridLayout* optionsgroupLayout;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEFINDDIALOGBASE_H
