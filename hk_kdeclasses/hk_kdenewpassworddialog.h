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
//$Revision: 1.6 $
#ifndef HK_KDENEWPASSWORDDIALOG_H
#define HK_KDENEWPASSWORDDIALOG_H
#include "hk_kdenewpassworddialogbase.h"

class hk_kdenewpassworddialog : public hk_kdenewpassworddialogbase
{
    Q_OBJECT

        public:
        hk_kdenewpassworddialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
        ~hk_kdenewpassworddialog();

    public slots:
        void data_changed();
        void ok_clicked();
};
#endif                                            // HK_KDENEWPASSWORDDIALOG_H
