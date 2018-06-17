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

#ifndef HK_KDEOBJECTHANDLER_H
#define HK_KDEOBJECTHANDLER_H
#include "hk_kdeobjecthandlerbase.h"

class hk_database;

class hk_kdeobjecthandler : public hk_kdeobjecthandlerbase
{
    Q_OBJECT

public:
    hk_kdeobjecthandler( hk_database*,QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdeobjecthandler();

protected:
    void accept(void);
    
protected slots:    
    virtual void download_clicked();
    virtual void upload_clicked();
    virtual void slot_selection_changed();
    
private:
    hk_database* p_database;    
};

#endif // HK_KDEOBJECTHANDLER_H
