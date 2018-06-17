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
//$Revision: 1.7 $
#ifndef HK_KDESUBREPORTDIALOG_H
#define HK_KDESUBREPORTDIALOG_H
#include "hk_kdesubreportdialogbase.h"
class hk_reportsection;
class hk_report;

class hk_kdesubreportdialog : public hk_kdesubreportdialogbase
{
    Q_OBJECT

        public:
        hk_kdesubreportdialog( hk_reportsection* s, QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
        ~hk_kdesubreportdialog();
        void set_reportsection(hk_reportsection* s);
    public slots:
        void add_dependingfields();
        void check_buttons();
        void subreport_selected();
        void delete_dependingfields();
        void dependingon_selected();
        void delete_subreport();
        void set_subreport();
        void set_subreportlist();
        void set_masterfield();
    private:
        hk_reportsection* p_section;
        hk_report* p_report;

};
#endif                                            // HK_KDESUBREPORTDIALOG_H
