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

#ifndef HK_KDENEWDATABASEDIALOGBASE_H
#define HK_KDENEWDATABASEDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QPushButton;

class hk_kdenewdatabasedialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdenewdatabasedialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdenewdatabasedialogbase();

    QLabel* databaselabel;
    QLineEdit* databasefield;
    QPushButton* directorybutton;
    QPushButton* okbutton;
    QPushButton* cancelbutton;

public slots:
    virtual void ok_clicked();
    virtual void directory_clicked();
    virtual void text_changed();

protected:
    QVBoxLayout* hk_kdenewdatabasedialogbaseLayout;
    QHBoxLayout* layout2;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDENEWDATABASEDIALOGBASE_H
