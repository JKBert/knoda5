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

#ifndef HK_KDEFORMDATASOURCEDIALOG_H
#define HK_KDEFORMDATASOURCEDIALOG_H
#include "hk_kdeformdatasourcedialogbase.h"

class hk_presentation;
class hk_datasource;
class hk_kdeformdatasourcedialogprivate;

class hk_kdeformdatasourcedialog : public hk_kdeformdatasourcebasedialog
{
    Q_OBJECT

public:
    hk_kdeformdatasourcedialog( hk_presentation* form, QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdeformdatasourcedialog();
    void set_datasource(hk_datasource*);
	hk_datasource* datasource(void);
	enum enum_displaytype{d_form,d_report,d_query};
	void set_displaytype(enum_displaytype);
	enum_displaytype displaytype(void) const;
	enum enum_edittype{e_all,e_modify,e_add};
	void set_edittype(enum_edittype);
	enum_edittype edittype(void) const ;
	enum enum_clickedbutton {cb_add,cb_alter,cb_delete,cb_cancel};
	enum_clickedbutton clicked_button(void) const;
	void set_allow_delete_datasource(bool);
	bool allow_delete_datasource(void) const;

protected slots:
    virtual void add_dependingfields();
    virtual void delete_dependingfields();
    virtual void datasource_selected();
    virtual void set_datasourcelist();
    virtual void dependingon_selected();
    virtual void check_buttons();
    virtual void delete_datasource();
    virtual void new_datasource(void);
	virtual void sql_changed(void);

private:
    void alter_datasource(void);
    void set_datasourceinfos(bool existingdatasource);
    void set_formdatasources();
	void init(void);
	hk_kdeformdatasourcedialogprivate* p_private;
};
#endif                                            // HK_KDEFORMDATASOURCEDIALOG_H
