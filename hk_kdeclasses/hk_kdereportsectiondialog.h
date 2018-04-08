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
//$Revision: 1.9 $
#ifndef HK_KDEREPORTSECTIONDIALOG_H
#define HK_KDEREPORTSECTIONDIALOG_H
#include "hk_kdereportsectiondialogbase.h"
class hk_report;
class hk_kdesimplereport;

class hk_kdereportsectiondialog : public hk_kdereportsectiondialogbase
{
    Q_OBJECT

        public:
        hk_kdereportsectiondialog( hk_kdesimplereport*,QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
        ~hk_kdereportsectiondialog();

    public slots:
        void add_clicked();
        void delete_clicked();
        void down_clicked();
        void up_clicked();
        void check_buttons();
    private:
        void set_sectionlist();
        void set_fieldlist();
        hk_kdesimplereport* p_report;

};
#endif                                            // HK_KDEREPORTSECTIONDIALOG_H
