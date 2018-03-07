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
#ifndef HK_KDELABEL
#define HK_KDELABEL
#include <hk_label.h>
#include <qlabel.h>
class hk_kdesimpleform;
/**
 *
 *@short KDE Widget to display text in a form
 *@version $Revision: 1.20 $
 *@author Horst Knorr (hk_classes@knoda.org)
 */
class hk_kdelabel :public QLabel, public hk_label
{
    Q_OBJECT
        public:
        hk_kdelabel(hk_kdesimpleform* form);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);
    protected:
     virtual void    widget_specific_tooltip_changed(void);
        virtual void    widget_specific_label_changed(void);
        virtual bool    widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight);
        virtual void    widget_specific_font_changed(void);
        virtual void    widget_specific_alignment(void);
        virtual void resizeEvent (QResizeEvent *);
        virtual void  moveEvent ( QMoveEvent * ) ;
        virtual void            mousePressEvent(QMouseEvent*);
        virtual void            mouseDoubleClickEvent(QMouseEvent*);
        virtual void widget_specific_backgroundcolour_changed(const hk_colour&);
        virtual void widget_specific_foregroundcolour_changed(const hk_colour&);
	virtual void drawFrame(QPainter*);
	virtual void paintEvent(QPaintEvent* event);
	virtual void widget_specific_topline_changed(void);
	virtual void widget_specific_bottomline_changed(void);
	virtual void widget_specific_leftline_changed(void);
	virtual void widget_specific_rightline_changed(void);
	virtual void widget_specific_diagonalluro_changed(void);
	virtual void widget_specific_diagonalloru_changed(void);
	virtual bool	presentationmode_changed(void);

    private:
        void    reposition(void);
	void    set_borders(void);

};
#endif
