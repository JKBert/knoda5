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

#ifndef HK_KDEREPORTCONDITIONDIALOG_H
#define HK_KDEREPORTCONDITIONDIALOG_H
#include "hk_kdereportconditiondialogbase.h"
#include <hk_string.h>
class hk_form;
class hk_button;

class hk_kdereportconditiondialog : public hk_kdereportconditiondialogbase
{
    Q_OBJECT

        public:
        hk_kdereportconditiondialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
        ~hk_kdereportconditiondialog();
	enum enum_type{form,report};
        void    set_values(hk_button* button, hk_form* form,const hk_string& r, enum_type t=report);
    public slots:
        void add_clicked();
        void delete_clicked();
        void formdatasource_changed();
        void check_buttons(void);
        virtual void formfield_change();
        virtual void reportfield_changed();
    protected slots:
        virtual void accept(void);

    private:
        void set_reportvalues(void);
        void set_formvalues(void);
        void set_formdatasources(void);
        void set_formfields(void);
        void set_buttonvalues(void);
        hk_form* p_form;
        hk_button* p_button;
        hk_string p_report;
};
#endif                                            // HK_KDEREPORTCONDITIONDIALOG_H
