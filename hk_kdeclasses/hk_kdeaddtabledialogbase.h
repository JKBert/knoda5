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
** Form interface generated from reading ui file 'hk_kdeaddtabledialogbase.ui'
**
** Created: Sa Jul 30 18:12:32 2005
**      by: The User Interface Compiler ($Id: hk_kdeaddtabledialogbase.h,v 1.2 2005/07/31 12:39:05 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEADDTABLEDIALOGBASE_H
#define HK_KDEADDTABLEDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListWidget;
class QComboBox;
class QPushButton;

class hk_kdeaddtabledialogbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdeaddtabledialogbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdeaddtabledialogbase();

    QListWidget* tablelist;
    QComboBox* datasourcetypefield;
    QPushButton* addbutton;
    QPushButton* buttonClose;

public slots:
    virtual void add_clicked();
    virtual void set_datasources();
    virtual void check_buttons();

protected:
    QGridLayout* hk_kdeaddtabledialogbaseLayout;
    QSpacerItem* spacer1;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDEADDTABLEDIALOGBASE_H
