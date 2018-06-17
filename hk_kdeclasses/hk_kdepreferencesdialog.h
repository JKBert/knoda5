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

#ifndef HK_KDEPREFERENCESDIALOG_H
#define HK_KDEPREFERENCESDIALOG_H

#include <kde4classesmacros.h>
#include "hk_kdepreferencesdialogbase.h"

class KDECLASSES_EXPORT hk_kdepreferencesdialog : public hk_kdepreferencesdialogbase
{
    Q_OBJECT

        public:
        hk_kdepreferencesdialog( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
        ~hk_kdepreferencesdialog();
    public slots:
        void driverpathbutton_clicked();
      virtual void help_clicked();
   protected:

virtual void keyPressEvent ( QKeyEvent * e );
    void accept(void);

};
#endif                                            // HK_KDEPREFERENCESDIALOG_H
