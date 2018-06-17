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
** Form interface generated from reading ui file 'subreportdialog.ui'
**
** Created: Don Aug 19 19:38:43 2004
**      by: The User Interface Compiler ($Id: hk_kdesubreportdialogbase.h,v 1.5 2004/08/20 16:41:42 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDESUBREPORTDIALOGBASE_H
#define HK_KDESUBREPORTDIALOGBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QFrame;
class QLabel;
class QComboBox;
class QToolButton;
class QCheckBox;
class QTreeWidget;

class hk_kdesubreportdialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdesubreportdialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdesubreportdialogbase();

    QPushButton* setbutton;
    QPushButton* deletebutton;
    QPushButton* buttonCancel;
    QFrame* Frame15;
    QLabel* subreportlabel;
    QComboBox* subreportfield;
    QFrame* Frame3;
    QToolButton* deletefieldbutton;
    QLabel* mastertext;
    QLabel* subreporttext;
    QComboBox* thisfield;
    QToolButton* addfieldbutton;
    QCheckBox* printbeforefield;
    QComboBox* masterfield;
    QTreeWidget* dependingfieldlist;

public slots:
    virtual void add_dependingfields();
    virtual void check_buttons();
    virtual void delete_dependingfields();
    virtual void delete_subreport();
    virtual void set_subreportlist();
    virtual void set_subreport();
    virtual void subreport_selected();

protected:
    QGridLayout* hk_kdesubreportdialogbaseLayout;
    QVBoxLayout* Layout8;
    QSpacerItem* Spacer1;
    QHBoxLayout* Frame15Layout;
    QHBoxLayout* Frame3Layout;
    QGridLayout* Layout3;
    QSpacerItem* Spacer2_2;
    QSpacerItem* Spacer3;
    QSpacerItem* Spacer2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;

};

#endif // HK_KDESUBREPORTDIALOGBASE_H
