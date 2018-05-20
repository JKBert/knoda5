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
 //$Revision: 1.3 $

#ifndef HK_KDEOBJECTHANDLERBASE_H
#define HK_KDEOBJECTHANDLERBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QLabel;
class hk_kdedblistview;
class QListViewItem;

class hk_kdeobjecthandlerbase : public QDialog
{
    Q_OBJECT

public:
    hk_kdeobjecthandlerbase( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdeobjecthandlerbase();
    QPushButton* uploadbutton;
    QPushButton* downloadbutton;
    QPushButton* exitbutton;
    hk_kdedblistview* locallistview;
    hk_kdedblistview* centrallistview;

public slots:
    virtual void download_clicked();
    virtual void upload_clicked();
    virtual void slot_selection_changed();

protected:
    QGridLayout* hk_kdeobjecthandlerbaseLayout;
    QVBoxLayout* layout1;
    QSpacerItem* spacer1;
    QSpacerItem* spacer2;
    QHBoxLayout* layout2;
    QSpacerItem* spacer3;
    QVBoxLayout* layout3;
    QVBoxLayout* layout4;

protected slots:
    virtual void languageChange();
};

#endif // HK_KDEOBJECTHANDLERBASE_H
