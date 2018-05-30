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
** Form interface generated from reading ui file 'hk_kdecsvimportdialog.ui'
**
** Created: Do Sep 1 10:04:45 2005
**      by: The User Interface Compiler ($Id: hk_kdecsvimportdialogbase.h,v 1.11 2005/09/01 08:27:12 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDECSVIMPORTDIALOGBASE_H
#define HK_KDECSVIMPORTDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class QTableWidget;
class QFrame;
class QCheckBox;
class QLineEdit;
class QComboBox;
class QToolButton;

class hk_kdecsvimportdialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdecsvimportdialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdecsvimportdialogbase();

    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QPushButton* morebutton;
    QPushButton* buttonHelp;
    QLabel* textLabel1;
    QTableWidget* previewtable;
    QFrame* moreframe;
    QCheckBox* autoincfield;
    QLineEdit* datetimeformatfield;
    QLineEdit* dateformatfield;
    QLabel* datetimeformatlabel;
    QLabel* localelabel;
    QComboBox* charsetfield;
    QComboBox* localefield;
    QLineEdit* timeformatfield;
    QLabel* charsetlabel;
    QLabel* timeformatlabel;
    QLabel* autoinclabel;
    QLabel* dateformatlabel;
    QLabel* TextLabel1_3;
    QLabel* TextLabel3;
    QComboBox* tablename;
    QLabel* lblAppend;
    QLabel* TextLabel1_2;
    QLineEdit* textdelimiterfield;
    QCheckBox* firstrow;
    QCheckBox* appendrows;
    QLabel* TextLabel1;
    QLineEdit* filefield;
    QToolButton* filebutton;
    QLabel* TextLabel2;
    QComboBox* columnseparatorfield;

public slots:
    virtual void ok_clicked();
    virtual void help_clicked();
    virtual void morebutton_clicked();

protected:
    QGridLayout* hk_kdecsvimportdialogbaseLayout;
    QVBoxLayout* layout6;
    QSpacerItem* Spacer1;
    QGridLayout* layout5;
    QSpacerItem* Spacer1_3;
    QSpacerItem* Spacer1_3_2;
    QVBoxLayout* layout8;
    QGridLayout* moreframeLayout;
    QSpacerItem* Spacer1_3_3;
    QHBoxLayout* Layout10;

protected slots:
    virtual void languageChange();
    virtual void buttons_enabled();
    virtual void filebutton_clicked();
};

#endif // HK_KDECSVIMPORTDIALOGBASE_H
