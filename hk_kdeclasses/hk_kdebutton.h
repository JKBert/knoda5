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
#ifndef HK_KDEBUTTON_H
#define HK_KDEBUTTON_H
#include "hk_button.h"
#include <qpushbutton.h>
class hk_form;
class hk_kdeform;
class hk_kdeformpartwidget;
class hk_kdereport;
class hk_kdereportpartwidget;
/**
 *
 *@short base class for buttons in forms.
 *@version 0.1
 *@author Horst Knorr (hk_classes@knoda.org)
 */
class hk_kdebutton :public QPushButton,public hk_button
{
    Q_OBJECT
        public:
        hk_kdebutton(QWidget*wid,hk_form*f=NULL) ;
        virtual ~hk_kdebutton(void);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);
virtual bool load_icon(const hk_string& filename,bool registerchange=true);
virtual void show_icon();
virtual void set_is_togglebutton(bool,bool registerchange=true,bool forcesetting=false);
virtual void set_is_pushed(bool,bool registerchange=true,bool forcesetting=false);
    public slots:
        void button_clicked(void);


    protected slots:
        void toggle_state_changed(bool state);

    protected:
     virtual void focusInEvent ( QFocusEvent * );
     virtual void focusOutEvent ( QFocusEvent * );
     virtual void    widget_specific_tooltip_changed(void);
        virtual bool widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight);
        virtual bool widget_specific_open_form(void);
        virtual bool widget_specific_close_form(void);
        virtual bool widget_specific_open_table(void);
        virtual bool widget_specific_open_query(void);
        virtual bool widget_specific_preview_report(void);
        virtual bool widget_specific_open_view(void);
	virtual void widget_specific_enabled_changed(void);
        virtual void widget_specific_label_changed(void);
        virtual void widget_specific_font_changed(void);
        virtual void resizeEvent (QResizeEvent *);
        virtual void moveEvent ( QMoveEvent * ) ;
        virtual void widget_specific_backgroundcolour_changed(const hk_colour& oldcolour);
        virtual void widget_specific_foregroundcolour_changed(const hk_colour& oldcolour);
        //virtual bool    widget_specific_close_application(void);
        virtual void            mouseDoubleClickEvent(QMouseEvent*);
	virtual bool	presentationmode_changed(void);

    private:
        void    reposition(void);
        hk_kdeform* find_existing_form(void);
        hk_kdeformpartwidget* find_existing_formpart(void);
        hk_kdereport* find_existing_report(void);
        hk_kdereportpartwidget* find_existing_reportpart(void);

};
#endif
