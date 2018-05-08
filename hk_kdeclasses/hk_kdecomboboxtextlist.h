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
#ifndef HK_KDECOMBOBOXTEXTLIST_H
#define HK_KDECOMBOBOXTEXTLIST_H
#include "hk_kdecomboboxtextlistbase.h"

class hk_dscombobox;

class hk_kdecomboboxtextlist : public hk_kdecomboboxtextlistbase
{
    Q_OBJECT

public:
    hk_kdecomboboxtextlist( QWidget* parent ,hk_dscombobox*, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdecomboboxtextlist();
public slots:
    void slot_data_changed(int , int );
    void add_row(void);
    void ok_clicked(void);
protected:
    bool eventFilter(QObject* object,QEvent* e);
    void delete_rows(void);
private:
    void set_existing_fields();
    hk_dscombobox* p_combobox;
};

#endif // HK_KDECOMBOBOXTEXTLIST_H
