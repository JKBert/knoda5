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
** Form interface generated from reading ui file 'xmlexportdialog.ui'
**
** Created: Di Jul 4 17:49:50 2006
**      by: The User Interface Compiler ($Id: hk_kdexmlexportdialogbase.h,v 1.8 2006/07/04 16:06:23 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEXMLEXPORTDIALOGBASE_H
#define HK_KDEXMLEXPORTDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLineEdit;
class QComboBox;
class QLabel;
class QToolButton;
class QCheckBox;
class QPushButton;

class hk_kdexmlexportdialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdexmlexportdialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdexmlexportdialogbase();

    QLineEdit* maindocumenttagfield;
    QLineEdit* rowelementfield;
    QComboBox* typefield;
    QLabel* typelabel;
    QComboBox* tablenamefield;
    QLabel* maindocumenttaglabel;
    QLabel* tablenamelabel;
    QLabel* rowelementlabel;
    QLabel* filelable;
    QLineEdit* filefield;
    QToolButton* filebutton;
    QCheckBox* structurefield;
    QCheckBox* attributefield;
    QCheckBox* excelxmlfield;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QPushButton* buttonHelp;

public slots:
    virtual void ok_clicked();
    virtual void listtype_changed();
    virtual void help_clicked();
    virtual void buttons_enabled();

protected:
    QGridLayout* hk_kdexmlexportdialogbaseLayout;
    QSpacerItem* Spacer2;
    QGridLayout* layout7;
    QHBoxLayout* Layout10;
    QVBoxLayout* layout5;
    QSpacerItem* Spacer1_2;
    QVBoxLayout* Layout5;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();
    virtual void excel_clicked();
    virtual void filebutton_clicked();
};

#endif // HK_KDEXMLEXPORTDIALOGBASE_H
