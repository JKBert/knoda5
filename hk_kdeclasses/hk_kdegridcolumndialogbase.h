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
** Form interface generated from reading ui file 'gridcolumnselect2.ui'
**
** Created: Fr Okt 27 08:25:04 2006
**      by: The User Interface Compiler ($Id: hk_kdegridcolumndialogbase.h,v 1.15 2006/10/27 06:38:24 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEGRIDCOLUMNDIALOGBASE_H
#define HK_KDEGRIDCOLUMNDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTreeWidget;
class QTreeWidgetItem;
class QListWidget;
class QListBoxItem;
class QToolButton;
class QGroupBox;
class QPushButton;
class QTabWidget;
class QWidget;
class QLineEdit;
class QComboBox;
class QSpinBox;

class hk_kdegridcolumndialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdegridcolumndialogbase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~hk_kdegridcolumndialogbase();

    QLabel* rightlabel;
    QTreeWidget* rightbox;
    QLabel* leftlabel;
    QListWidget* leftbox;
    QToolButton* addbutton;
    QToolButton* deletebutton;
    QLabel* definitionlabel;
    QGroupBox* ButtonGroup7;
    QPushButton* storebutton;
    QPushButton* exitbutton;
    QTabWidget* TabWidget;
    QWidget* formattab;
    QLineEdit* displayfield;
    QLabel* displaylabel;
    QLabel* columntypelabel;
    QLabel* digitlabel;
    QLabel* alignmentlabel;
    QComboBox* alignmentfield;
    QComboBox* columntypefield;
    QComboBox* separatorfield;
    QLabel* separatorlabel;
    QComboBox* readonlyfield;
    QSpinBox* digitfield;
    QLabel* readonlylabel;
    QLabel* Defaultlabel;
    QLineEdit* defaultfield;
    QLineEdit* identifierfield;
    QLabel* identifierlabel;
    QWidget* combotab;
    QLabel* viewcolumnlabel;
    QComboBox* viewcolumnfield;
    QLabel* listdatasourcetypelabel;
    QLabel* listcolumnlabel;
    QComboBox* listcolumnfield;
    QComboBox* listdatasourcetypefield;
    QLabel* listdatasourcelabel;
    QComboBox* listdatasourcefield;
    QWidget* actiontab;
    QLabel* pushactionlabel;
    QPushButton* onpushactionbutton;
    QPushButton* ondoubleclickactionbutton;
    QLabel* doubleclickactionlabel;
    QLabel* onloosefocuslabel;
    QPushButton* ongetfocusbutton;
    QPushButton* onloosefocusbutton;
    QLabel* ongetfocuslabel;
    QPushButton* beforeupdateactionbutton;
    QPushButton* afterrowchangeactionbutton;
    QLabel* beforeinsertlabel;
    QPushButton* afterdeleteactionbutton;
    QLabel* beforeupdateactionlabel;
    QLabel* openactionlabel;
    QPushButton* afterupdateactionbutton;
    QPushButton* beforeinsertactionbutton;
    QLabel* beforedeletelabel;
    QLabel* afterupdatelabel;
    QLabel* afterdeletelabel;
    QLabel* beforerowchangelabel;
    QLabel* closeactionlabel;
    QPushButton* oncloseactionbutton;
    QPushButton* onopenactionbutton;
    QPushButton* beforerowchangeactionbutton;
    QPushButton* beforedeleteactionbutton;
    QLabel* afterrowchangelabel;
    QPushButton* onkeybutton;
    QLabel* onkeylabel;
    QLabel* afterinsertlabel;
    QPushButton* afterinsertactionbutton;
    QLabel* onselectlabel;
    QPushButton* onselectactionbutton;

public slots:
    virtual void addbutton_clicked();
    virtual void alignment_changed();
    virtual void columntype_changed();
    virtual void defaultvalue_changed();
    virtual void deletebutton_clicked();
    virtual void digits_changed();
    virtual void displayname_changed();
    virtual void exitbutton_clicked();
    virtual void listcolumn_changed();
    virtual void listdatasource_changed();
    virtual void listdatasourcetype_changed();
    virtual void okbutton_clicked();
    virtual void readonly_changed();
    virtual void rightbox_selectionchanged( QTreeWidgetItem * );
    virtual void separator_changed();
    virtual void viewcolumn_changed();
    virtual void onclickaction_clicked();
    virtual void ondoubleclickaction_clicked();
    virtual void onopenaction_clicked();
    virtual void oncloseaction_clicked();
    virtual void beforeupdateaction_clicked();
    virtual void afterupdateaction_clicked();
    virtual void beforerowchangeaction_changed();
    virtual void afterrowchangeaction_changed();
    virtual void beforedeleteaction_clicked();
    virtual void afterdeleteaction_clicked();
    virtual void beforeinsertaction_clicked();
    virtual void afterinsertaction_clicked();
    virtual void on_getfocusaction_clicked();
    virtual void on_loosefocusaction_clicked();
    virtual void onkeybutton_clicked();
    virtual void onselectaction_clicked();
    virtual void identifier_changed();

protected:
    QGridLayout* hk_kdegridcolumndialogbaseLayout;
    QSpacerItem* Spacer5;
    QSpacerItem* Spacer5_2;
    QSpacerItem* Spacer1;
    QVBoxLayout* Layout6;
    QVBoxLayout* Layout4;
    QVBoxLayout* Layout7;
    QSpacerItem* Spacer2;
    QSpacerItem* Spacer3;
    QVBoxLayout* Layout2;
    QVBoxLayout* ButtonGroup7Layout;
    QGridLayout* formattabLayout;
    QSpacerItem* Spacer22;
    QSpacerItem* Spacer23;
    QGridLayout* combotabLayout;
    QSpacerItem* Spacer20;
    QSpacerItem* Spacer21;
    QGridLayout* actiontabLayout;
    QSpacerItem* Spacer6;
    QSpacerItem* spacer23;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEGRIDCOLUMNDIALOGBASE_H
