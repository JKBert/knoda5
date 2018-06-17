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

#ifndef HK_KDEPREFERENCESDIALOGBASE_H
#define HK_KDEPREFERENCESDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QTabWidget;
class QWidget;
class QLabel;
class QPushButton;
class QLineEdit;
class QComboBox;
class QSpinBox;

class hk_kdepreferencesdialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdepreferencesdialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdepreferencesdialogbase();

    QTabWidget* regionaltab;
    QWidget* generalpage;
    QLabel* driverpathlabel;
    QPushButton* driverpathbutton;
    QLineEdit* driverpathfield;
    QLabel* defaultdriverlabel;
    QComboBox* defaultdriverfield;
    QSpinBox* snap2gridxfield;
    QLabel* snap2gridxlabel;
    QLabel* snap2gridylabel;
    QSpinBox* snap2gridyfield;
    QLabel* sizetypelabel;
    QComboBox* sizetypefield;
    QComboBox* storepasswordfield;
    QLabel* storepasswordlabel;
    QLabel* automaticupdatelabel;
    QComboBox* automaticupdatefield;
    QWidget* Appearance;
    QLabel* maximizedlabel;
    QComboBox* maximizedfield;
    QLabel* bepedanticlabel;
    QComboBox* bepedanticfield;
    QLabel* use_singleclicklabel;
    QComboBox* use_singleclickfield;
    QWidget* defaults;
    QComboBox* fontfield;
    QSpinBox* fontsizefield;
    QLabel* fontlabel;
    QComboBox* textalignmentfield;
    QComboBox* numberalignmentfield;
    QLabel* textalignmentlabel;
    QLabel* numberalignmentlabel;
    QLabel* separatorlabel;
    QComboBox* separatorfield;
    QLabel* precisionlabel;
    QSpinBox* precisionfield;
    QWidget* regionalpage;
    QLabel* datelabel;
    QLineEdit* datefield;
    QLineEdit* timefield;
    QLabel* timelabel;
    QLabel* datetimelabel;
    QComboBox* measurefield;
    QLabel* measurelabel;
    QLineEdit* datetimefield;
    QLineEdit* localefield;
    QLabel* localelabel;
    QWidget* printerpage;
    QLabel* printcommandlabel;
    QLineEdit* printcommandfield;
    QComboBox* embedfontsfield;
    QLabel* embedfontslabel;
    QPushButton* buttonHelp;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

public slots:
    virtual void driverpathbutton_clicked();
    virtual void help_clicked();

protected:
    QVBoxLayout* hk_kdepreferencesdialogbaseLayout;
    QGridLayout* generalpageLayout;
    QSpacerItem* Spacer6;
    QSpacerItem* Spacer7;
    QGridLayout* AppearanceLayout;
    QSpacerItem* spacer10;
    QSpacerItem* spacer11;
    QGridLayout* defaultsLayout;
    QSpacerItem* Spacer16;
    QSpacerItem* Spacer15;
    QHBoxLayout* Layout3;
    QGridLayout* regionalpageLayout;
    QSpacerItem* Spacer1;
    QSpacerItem* Spacer2;
    QGridLayout* printerpageLayout;
    QSpacerItem* Spacer1_2;
    QSpacerItem* Spacer2_2;
    QHBoxLayout* layout13;
    QSpacerItem* Horizontal_Spacing2;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEPREFERENCESDIALOGBASE_H
