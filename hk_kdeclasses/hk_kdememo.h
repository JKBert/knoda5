// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
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

#ifndef HK_KDEMEMO_H
#define HK_KDEMEMO_H
#include <hk_dsmemo.h>
#include <ktextedit.h>
class hk_kdefinddialog;
/**
 *
 *@short KDE Widget to edit/display long data like text in a field.
 *@version $Revision: 1.25 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *Just set the datasource and the column (defined in the parent class @ref hk_dsdatavisible).
 */

class hk_kdememo: public KTextEdit,public hk_dsmemo

{
    Q_OBJECT
        public:
        hk_kdememo(QWidget* wid,hk_form* f=NULL);
        ~hk_kdememo();
	virtual void set_value(const hk_string&);
	virtual hk_string value(void);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);
        virtual void update_filter(void);

    signals:
        void lostFocus();

    protected:
     virtual void focusInEvent ( QFocusEvent * );
     virtual void focusOutEvent ( QFocusEvent * );
     virtual void    widget_specific_tooltip_changed(void);
        virtual bool    widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight);
        virtual bool    widget_specific_row_change(void);
        virtual void    widget_specific_insert_mode(void);
        virtual void    widget_specific_row_deleted(void);
        virtual void    widget_specific_row_added(void);
        virtual void    widget_specific_alignment(void);
	virtual void widget_specific_enabled_changed(void);
	virtual bool	presentationmode_changed(void);
        bool    datasource_disable(void);
        bool    datasource_enable(void);
        void    datasource_delete(void);
        virtual void    widget_specific_font_changed(void);
        virtual void resizeEvent (QResizeEvent *);
        virtual void  moveEvent ( QMoveEvent * ) ;
	virtual void  mousePressEvent(QMouseEvent*);
	virtual void  mouseDoubleClickEvent(QMouseEvent*);
        virtual void widget_specific_backgroundcolour_changed(const hk_colour&);
        virtual void widget_specific_foregroundcolour_changed(const hk_colour&);
        void   keyPressEvent ( QKeyEvent * e );
	virtual void    before_presentationmode_changed(void);

    protected slots:
        void     slot_data_changed(void);
        void    before_row_change(void);
        void    before_store_changed_data(void);
        void    NULLselected(void);
        void    slot_focus_lost(void);
	void    extendContextMenu(QMenu * pM);
    public slots:
        void    find_clicked(void);
        void    find_next(void);
        void    findargument_changed(void);

    private:
        void    reposition(void);
        bool p_widget_specific_row_change;
        void    widget_specific_enable_disable(void);
        bool    has_changed;
        hk_kdefinddialog* p_finddialog;
        bool          p_findfirstview;
        bool p_filter_has_changed;

};
#endif
