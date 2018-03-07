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

#ifndef HK_KDECOMBOBOX
#define HK_KDECOMBOBOX
#include <hk_dscombobox.h>
#include <qcombobox.h>
/**
 *
 *@short KDE Widget to edit/display data in a combobox.
 *@version $Revision: 1.34 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *Just set the datasource and the column (defined in the parent class @ref hk_dsdatavisible).
 *To fill the listbox with values set the datasource which contains the wished values with
 *@ref set_listdatasource. The columnname of the row which should be displayed in the listbox
 *must be set with @ref set_viewcolumn. The row which contains the data to be written has to
 *be specified with @ref set_listcolumn.
 */

 class hk_kdecomboboxprivate;

class hk_kdecombobox :public QComboBox,public hk_dscombobox
{
    Q_OBJECT
friend class qlineedit;
	public:
        hk_kdecombobox(QWidget* wid,hk_form* form=NULL);
        virtual ~hk_kdecombobox(void);
	virtual void set_value(const hk_string&);
	virtual hk_string value(void);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);
        virtual void update_filter(void);

    protected:
     virtual void focusInEvent ( QFocusEvent * );
     virtual void focusOutEvent ( QFocusEvent * );
     virtual void    widget_specific_tooltip_changed(void);
        virtual bool    datasource_enable(void);
        virtual bool    datasource_disable(void);
        virtual void    widget_specific_enable_disable();
        virtual bool    widget_specific_row_change(void);
        virtual bool    widget_specific_coordinates(unsigned int px,unsigned int py,unsigned int pwidth,unsigned int pheight);
        virtual void    widget_specific_insert_mode(void);
        virtual void    widget_specific_row_deleted(void);
        virtual void    widget_specific_row_added(void);
	virtual void widget_specific_enabled_changed(void);
        virtual void    widget_specific_mode_change(void);
        virtual void    after_store_changed_data(void);
        virtual void    widget_specific_font_changed(void);
	virtual bool	presentationmode_changed(void);

        virtual void resizeEvent (QResizeEvent *);
        virtual void keyPressEvent(QKeyEvent *);
        virtual void            mousePressEvent(QMouseEvent*);
        virtual void  moveEvent ( QMoveEvent * ) ;
        virtual void  load_listitems(void);
        virtual void widget_specific_backgroundcolour_changed(const hk_colour&);
        virtual void widget_specific_foregroundcolour_changed(const hk_colour&);
        virtual void            mouseDoubleClickEvent(QMouseEvent*);
	virtual void    before_presentationmode_changed(void);

    protected slots:
        void slot_data_changed(int);
        void slot_text_changed(void);
        void slot_focus_lost(void);

    private:
        void load_listitems_from_column();
	void load_listitems_from_textlist();
	void    selector_row_changed(int index);
        void        reposition(void);
        bool        p_widget_specific_row_change;
	bool 	    p_whiledatachange;
        void        set_currentlistitem(void);
        void        set_selecteditemdata(int);
        int         find_edittextitem(void);
	hk_kdecomboboxprivate* p_private;
        bool p_filter_has_changed;

};
#endif
