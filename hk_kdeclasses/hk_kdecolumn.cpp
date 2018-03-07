// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_kdeclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
// ****************************************************************************
//$Revision: 1.11 $
#include "hk_kdecolumn.h"
#include "hk_form.h"

hk_kdecolumn::hk_kdecolumn(hk_form* f): hk_dsdatavisible(f)
{

}


void        hk_kdecolumn::widget_specific_insert_mode(void)
{
    emit signal_insert_mode();
}


bool        hk_kdecolumn::widget_specific_row_change(void)
{
    emit signal_row_change();
    return hk_dsvisible::widget_specific_row_change() ;

}


void        hk_kdecolumn::widget_specific_enable_disable(void)
{
    emit signal_enable_disable();
}


void        hk_kdecolumn::widget_specific_row_added(void)
{
    emit signal_row_added();
}


void        hk_kdecolumn::widget_specific_row_deleted(void)
{
    emit signal_row_deleted();
}


void        hk_kdecolumn::before_row_change(void)
{
hk_dsdatavisible::before_row_change();
    emit signal_before_row_change();
}


void        hk_kdecolumn::before_store_changed_data(void)
{
hk_dsdatavisible::before_store_changed_data();
    emit signal_before_store_changed_data();
}


bool        hk_kdecolumn::datasource_disable(void)
{
    emit signal_datasource_disable();
    return hk_dsvisible::datasource_disable();
}


bool        hk_kdecolumn::datasource_enable(void)
{
    emit signal_datasource_enable();
    return hk_dsvisible::datasource_enable();
}


void        hk_kdecolumn::datasource_delete(void)
{
    emit signal_datasource_delete();
}


void        hk_kdecolumn::row_added(void)
{
    emit signal_row_added();
}


void        hk_kdecolumn::row_deleted(void)
{
    emit signal_row_deleted();
}


bool        hk_kdecolumn::columns_new_created(void)
{
    emit signal_columns_new_created();
    return hk_dsvisible::columns_new_created();
}


void        hk_kdecolumn::list_changes(listtype type)
{
    emit signal_list_changes(type);
}


bool        hk_kdecolumn::before_columns_deleted(void)
{
    emit signal_before_columns_deleted();
    return hk_dsvisible::before_columns_deleted();
}


bool        hk_kdecolumn::table_structure_changes(void)
{
    emit signal_table_structure_changes();
    return hk_dsvisible::table_structure_changes();
}


void        hk_kdecolumn::before_source_vanishes(void)
{
    emit signal_before_source_vanishes();
}


void hk_kdecolumn::widget_specific_set_column(void)
{
    emit signal_set_column();
}


void hk_kdecolumn::columndata_has_changed(void)
{
    emit signal_columndata_has_changed();

}
