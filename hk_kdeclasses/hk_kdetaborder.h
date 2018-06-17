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
// $Revision: 1.3 $
#ifndef HK_KDETABORDER_H
#define HK_KDETABORDER_H
#include "hk_kdetaborderbase.h"

class hk_kdesimpleform;

class hk_kdetaborder : public hk_kdetaborderdialogbase
{
    Q_OBJECT

public:
    hk_kdetaborder( hk_kdesimpleform* ,QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdetaborder();

public slots:
    void add_clicked();
    void check_buttons();
    void delete_clicked();
    void down_clicked();
    void up_clicked();
    void ok_clicked();

private:
void init();
hk_kdesimpleform* p_form; 
bool p_has_changed;   
};

#endif // HK_KDETABORDER_H
