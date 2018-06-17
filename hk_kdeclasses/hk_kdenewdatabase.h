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
#ifndef HK_KDENEWDATABASE_H
#define HK_KDENEWDATABASE_H
#include "hk_kdenewdatabasebase.h"
#include <hk_string.h>

class hk_kdenewdatabase : public hk_kdenewdatabasedialogbase
{
    Q_OBJECT

public:
    hk_kdenewdatabase( const hk_string& drivername,QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdenewdatabase();
    static hk_string show_dialog(const hk_string& drivername,bool local);

public slots:
    virtual void ok_clicked();
    virtual void directory_clicked();
    virtual void text_changed();
private:
    hk_string p_drivername;
};

#endif // HK_KDENEWDATABASE_H
