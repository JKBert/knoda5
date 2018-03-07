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
#ifndef HK_KDECOLUMN
#define HK_KDECOLUMN
#include <hk_dsdatavisible.h>
#include <qobject.h>
/**
 *
 *@short connects a datasource to QT Signals
 *@version $Revision: 1.9 $
 *@author Horst Knorr (hk_classes@knoda.org)
 */
class hk_kdecolumn :public QObject, public hk_dsdatavisible
{
    Q_OBJECT
        public:
        hk_kdecolumn(hk_form* f=NULL);

        signals:
        void signal_insert_mode(void);
        void signal_row_change(void);
        void signal_enable_disable(void);
        void signal_row_added(void);
        void signal_row_deleted(void);
        void signal_before_row_change(void);
        void signal_before_store_changed_data(void);
        void signal_datasource_disable(void);
        void signal_datasource_enable(void);
        void signal_datasource_delete(void);
        void signal_columns_new_created(void);
        void signal_list_changes(listtype type);
        void signal_before_columns_deleted(void);
        void signal_table_structure_changes(void);
        void signal_before_source_vanishes(void);
        void signal_set_column(void);
        void signal_columndata_has_changed(void);
    protected:
        virtual void    widget_specific_insert_mode(void);
        virtual bool    widget_specific_row_change(void);
        virtual void    widget_specific_enable_disable(void);
        virtual void    widget_specific_row_added(void);
        virtual void    widget_specific_row_deleted(void);
        virtual void    before_row_change(void);
        virtual void    before_store_changed_data(void);
        virtual bool    datasource_disable(void);
        virtual bool    datasource_enable(void);
        virtual void    datasource_delete(void);
        virtual void    row_added(void);
        virtual void    row_deleted(void);
        virtual bool    columns_new_created(void);
        virtual void    list_changes(listtype type);
        virtual bool    before_columns_deleted(void);
        virtual bool    table_structure_changes(void);
        virtual void    before_source_vanishes(void);
        virtual void    widget_specific_set_column(void);
        virtual void    columndata_has_changed(void);

    private:

};
#endif
