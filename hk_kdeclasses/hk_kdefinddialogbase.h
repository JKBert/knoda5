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
    hk_kdefinddialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
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
