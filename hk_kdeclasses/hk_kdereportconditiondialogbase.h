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
** Form interface generated from reading ui file 'reportconditiondialog.ui'
**
** Created: Sa Jul 30 21:44:40 2005
**      by: The User Interface Compiler ($Id: hk_kdereportconditiondialogbase.h,v 1.4 2005/07/31 12:39:05 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEREPORTCONDITIONDIALOGBASE_H
#define HK_KDEREPORTCONDITIONDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QGroupBox;
class QTreeWidget;
class QLabel;
class QComboBox;

class hk_kdereportconditiondialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdereportconditiondialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdereportconditiondialogbase();

    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QGroupBox* GroupBox2;
    QTreeWidget* conditionlist;
    QLabel* conditionlabel;
    QPushButton* addbutton;
    QComboBox* formdatasourcefield;
    QComboBox* conditionfield;
    QLabel* reportlabel;
    QComboBox* reportfield;
    QLabel* formdatasourcelabel;
    QLabel* formfieldlabel;
    QPushButton* deletebutton;
    QComboBox* formfieldfield;

public slots:
    virtual void add_clicked();
    virtual void delete_clicked();
    virtual void formdatasource_changed();
    virtual void formfield_change();
    virtual void check_buttons();
    virtual void reportfield_changed();

protected:
    QGridLayout* hk_kdereportconditiondialogbaseLayout;
    QVBoxLayout* Layout5;
    QSpacerItem* Spacer1;
    QGridLayout* GroupBox2Layout;
    QGridLayout* Layout17;
    QSpacerItem* Spacer2;
    QSpacerItem* Spacer5_2;
    QSpacerItem* Spacer23;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEREPORTCONDITIONDIALOGBASE_H
