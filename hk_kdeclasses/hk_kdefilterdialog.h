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
//Version $Revision: 1.7 $

#ifndef HK_KDEFILTERDIALOG_H
#define HK_KDEFILTERDIALOG_H
#include "hk_kdefilterdialogbase.h"

class hk_kdefilterdialog : public hk_kdefilterdialogbase
{
    Q_OBJECT
        
public:
    enum enum_type {filter,editor};
    hk_kdefilterdialog(enum_type type=filter, QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdefilterdialog();

protected slots:
    void text_changed();
    
protected:
    void closeEvent(QCloseEvent*);
    void accept(void);
    void reject(void);
    
private:
    void store_size();
	enum_type p_type;
};
#endif                                            // HK_KDEFILTERDIALOG_H
