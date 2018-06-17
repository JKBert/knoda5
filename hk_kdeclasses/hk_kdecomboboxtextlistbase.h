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

#ifndef HK_KDECOMBOBOXTEXTLISTBASE_H
#define HK_KDECOMBOBOXTEXTLISTBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTableWidget;
class QLabel;

class hk_kdecomboboxtextlistbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdecomboboxtextlistbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdecomboboxtextlistbase();

    QPushButton* ok_button;
    QPushButton* cancel_button;
    QTableWidget* grid;
    QLabel* descriptionlabel;

public slots:
    virtual void slot_data_changed(int,int);
    virtual void ok_clicked();

protected:
    QGridLayout* hk_kdecomboboxtextlistbaseLayout;
    QVBoxLayout* Layout5;
    QSpacerItem* Spacer1;

protected slots:
    virtual void languageChange();

};

#endif // HK_KDECOMBOBOXTEXTLISTBASE_H
