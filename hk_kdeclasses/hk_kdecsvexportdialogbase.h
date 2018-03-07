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
** Form interface generated from reading ui file 'hk_kdecsvexportdialog.ui'
**
** Created: Do Sep 1 10:22:16 2005
**      by: The User Interface Compiler ($Id: hk_kdecsvexportdialogbase.h,v 1.2 2005/09/01 08:27:11 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDECSVEXPORTDIALOGBASE_H
#define HK_KDECSVEXPORTDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;
class QToolButton;
class QComboBox;
class QCheckBox;
class QFrame;
class QPushButton;

class hk_kdecsvexportdialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdecsvexportdialogbase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~hk_kdecsvexportdialogbase();

    QLabel* TextLabel1;
    QLineEdit* filefield;
    QToolButton* filebutton;
    QComboBox* tablefield;
    QComboBox* typefield;
    QLabel* typelabel;
    QLabel* TextLabel1_2_2;
    QComboBox* columnseparatorfield;
    QLabel* TextLabel2;
    QCheckBox* firstrow;
    QLabel* TextLabel3;
    QLabel* TextLabel1_3;
    QLineEdit* textdelimiterfield;
    QFrame* moreframe;
    QLabel* dateformatlabel;
    QComboBox* localefield;
    QLineEdit* timeformatfield;
    QLineEdit* dateformatfield;
    QLineEdit* datetimeformatfield;
    QComboBox* charsetfield;
    QLabel* charsetlabel;
    QLabel* localelabel;
    QLabel* timeformatlabel;
    QLabel* datetimeformatlabel;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;
    QPushButton* morebutton;
    QPushButton* buttonHelp;

public slots:
    virtual void ok_clicked();
    virtual void help_clicked();
    virtual void listtype_changed();
    virtual void morebutton_clicked();

protected:
    QGridLayout* hk_kdecsvexportdialogbaseLayout;
    QSpacerItem* Spacer1_3;
    QHBoxLayout* Layout10;
    QGridLayout* moreframeLayout;
    QVBoxLayout* layout7;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

    virtual void buttons_enabled();
    virtual void filebutton_clicked();


};

#endif // HK_KDECSVEXPORTDIALOGBASE_H
