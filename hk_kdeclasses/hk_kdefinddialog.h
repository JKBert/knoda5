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
// $Revision: 1.7 $

#ifndef HK_KDEFINDDIALOG_H
#define HK_KDEFINDDIALOG_H
#include "hk_kdefinddialogbase.h"

class hk_kdefinddialog : public hk_kdefinddialogbase
{
    Q_OBJECT

    public:
        hk_kdefinddialog( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
        ~hk_kdefinddialog();
    public slots:
        virtual void find_slot();
        virtual void new_findargument();
        signals:
        void signal_findbutton_clicked();
        void signal_findargument_changed();
};
#endif                                            // HK_KDEFINDDIALOG_H
