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
** Form interface generated from reading ui file 'hk_kdeeximportdatabasebase.ui'
**
** Created: Mi Aug 3 15:21:04 2005
**      by: The User Interface Compiler ($Id: hk_kdeeximportdatabasebase.h,v 1.5 2005/08/03 14:28:49 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEEXIMPORTDATABASEBASE_H
#define HK_KDEEXIMPORTDATABASEBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QCheckBox;
class QTextEdit;
class QLabel;
class QComboBox;
class QTreeWidget;

class hk_kdeeximportdatabasebase : public QDialog
{
    Q_OBJECT

public:
    hk_kdeeximportdatabasebase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~hk_kdeeximportdatabasebase();

    QPushButton* exitbutton;
    QCheckBox* copyfield;
    QPushButton* uploadbutton;
    QTextEdit* outputfield;
    QLabel* rightlabel;
    QComboBox* rightdatabasefield;
    QPushButton* rightnewbutton;
    QTreeWidget* right_listview;
    QCheckBox* overwritefield;
    QLabel* leftlabel;
    QComboBox* leftdatabasefield;
    QPushButton* leftnewbutton;
    QTreeWidget* left_listview;
    QPushButton* buttonHelp;

public slots:
    virtual void download_clicked();
    virtual void upload_clicked();
    virtual void slot_selection_changed();
    virtual void leftnewbutton_clicked();
    virtual void rightnewbutton_clicked();
    virtual void help_clicked();

protected:
    QGridLayout* hk_kdeeximportdatabasebaseLayout;
    QSpacerItem* spacer4;
    QVBoxLayout* layout1;
    QSpacerItem* spacer1;
    QSpacerItem* spacer2;
    QVBoxLayout* layout7;
    QHBoxLayout* layout14;
    QVBoxLayout* layout6;
    QHBoxLayout* layout14_2;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEEXIMPORTDATABASEBASE_H
