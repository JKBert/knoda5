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
//$Revision: 1.8 $
#include "hk_kdedatasource.h"

hk_kdedatasource::hk_kdedatasource(hk_presentation* p): hk_dsvisible(p)
{

}


void        hk_kdedatasource::widget_specific_insert_mode(void)
{
    emit signal_insert_mode();
}


bool        hk_kdedatasource::widget_specific_row_change(void)
{
    emit signal_row_change();
    return hk_dsvisible::widget_specific_row_change() ;

}


void        hk_kdedatasource::widget_specific_enable_disable(void)
{
    emit signal_enable_disable();
}


void        hk_kdedatasource::widget_specific_row_added(void)
{
    emit signal_row_added();
}


void        hk_kdedatasource::widget_specific_row_deleted(void)
{
    emit signal_row_deleted();
}


void        hk_kdedatasource::before_row_change(void)
{
    emit signal_before_row_change();
}


void        hk_kdedatasource::before_store_changed_data(void)
{
    emit signal_before_store_changed_data();
}


bool        hk_kdedatasource::datasource_disable(void)
{
    emit signal_datasource_disable();
    return hk_dsvisible::datasource_disable();
}


bool        hk_kdedatasource::datasource_enable(void)
{
    emit signal_datasource_enable();
    return hk_dsvisible::datasource_enable();
}


void        hk_kdedatasource::datasource_delete(void)
{
    emit signal_datasource_delete();
}


void        hk_kdedatasource::row_added(void)
{
    emit signal_row_added();
}


void        hk_kdedatasource::row_deleted(void)
{
    emit signal_row_deleted();
}


bool        hk_kdedatasource::columns_new_created(void)
{
    emit signal_columns_new_created();
    return hk_dsvisible::columns_new_created();
}


void        hk_kdedatasource::list_changes(listtype type)
{
    emit signal_list_changes(type);
}


bool        hk_kdedatasource::before_columns_deleted(void)
{
    emit signal_before_columns_deleted();
    return hk_dsvisible::before_columns_deleted();
}


bool        hk_kdedatasource::table_structure_changes(void)
{
    emit signal_table_structure_changes();
    return hk_dsvisible::table_structure_changes();
}


void        hk_kdedatasource::before_source_vanishes(void)
{
    emit signal_before_source_vanishes();
}
