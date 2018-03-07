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
//$Revision: 1.4 $

#ifndef HK_KDEDATABASESETTING_H
#define HK_KDEDATABASESETTING_H
#include "hk_kdedatabasesettingbase.h"
class hk_database;
class QTreeWidgetItem;

class hk_kdedatabasesetting : public hk_kdedatabasesettingbase
{
    Q_OBJECT

public:
    hk_kdedatabasesetting( hk_database*,QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~hk_kdedatabasesetting();
public slots:
    virtual void create_clicked();
    virtual void selection_changed();
    virtual void listview_changed();
    virtual void ok_clicked();
    virtual void alload_changed();
    virtual void allstore_changed();
    virtual void encoding_changed();

private:
hk_database* p_database;    
QTreeWidgetItem* p_systemtableitem;
QTreeWidgetItem* p_localeitem;

};

#endif // HK_KDEDATABASESETTING_H
