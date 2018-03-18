// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
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
** Form interface generated from reading ui file 'databaseconfig.ui'
**
** Created: Sa Aug 26 22:49:32 2006
**      by: The User Interface Compiler ($Id: hk_kdedatabasesettingbase.h,v 1.7 2006/08/27 07:41:32 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEDATABASESETTINGBASE_H
#define HK_KDEDATABASESETTINGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QTreeWidget;
class QListViewItem;
class QStackedWidget;
class QWidget;
class QComboBox;
class QFrame;

class hk_kdedatabasesettingbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdedatabasesettingbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdedatabasesettingbase();

    QLabel* headertext;
    QLabel* textLabel2;
    QPushButton* buttonHelp;
    QPushButton* ok_button;
    QPushButton* cancel_button;
    QTreeWidget* listview;
    QStackedWidget* widgetstack;
    QWidget* systemtablepage;
    QLabel* createlabel;
    QPushButton* createbutton;
    QComboBox* automaticupdatefield;
    QLabel* automaticupdatelabel;
    QFrame* storageframe;
    QLabel* loadlabel;
    QLabel* querylabel;
    QLabel* formlabel;
    QComboBox* formload;
    QComboBox* queryload;
    QComboBox* allload;
    QLabel* alllabel;
    QLabel* reportlabel;
    QComboBox* reportload;
    QComboBox* moduleload;
    QLabel* modulelabel;
    QWidget* encodingpage;
    QComboBox* databaseencodingfield;
    QLabel* databaseencodinglabel;

public slots:
    virtual void ok_clicked();
    virtual void selection_changed();
    virtual void listview_changed();
    virtual void create_clicked();
    virtual void alload_changed();
    virtual void allstore_changed();
    virtual void encoding_changed();

protected:
    QGridLayout* hk_kdedatabasesettingbaseLayout;
    QSpacerItem* spacer2;
    QHBoxLayout* Layout1;
    QSpacerItem* Horizontal_Spacing2;
    QGridLayout* systemtablepageLayout;
    QGridLayout* storageframeLayout;
    QGridLayout* encodingpageLayout;
    QSpacerItem* spacer3;
    QSpacerItem* spacer4;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEDATABASESETTINGBASE_H
