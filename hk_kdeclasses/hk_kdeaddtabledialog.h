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
#ifndef HK_KDEADDTABLEDIALOG_H
#define HK_KDEADDTABLEDIALOG_H
#include "hk_kdeaddtabledialogbase.h"
#include <hk_string.h>
class hk_kdedbdesigner;
class hk_kdeaddtabledialog : public hk_kdeaddtabledialogbase
{
    Q_OBJECT

public:
    hk_kdeaddtabledialog(hk_kdedbdesigner*, bool allowqueries,QWidget* parent = 0, const char* name = 0, 
      bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdeaddtabledialog();
    bool datasource_added() const{return p_added;}
public slots:
    virtual void add_clicked();
    virtual void set_datasources();
    virtual void check_buttons();

protected slots:
    virtual void accept();

private:
    bool already_added_table(const hk_string&);
    hk_kdedbdesigner* p_designer;
    bool p_added;
    bool p_allowqueries;
};

#endif // HK_KDEADDTABLEDIALOG_H
