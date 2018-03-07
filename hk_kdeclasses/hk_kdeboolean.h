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

#ifndef HK_KDEBOOLEAN
#define HK_KDEBOOLEAN
#include <qcheckbox.h>
#include <hk_dsboolean.h>
class hk_form;
/**
 *
 *@short KDE Widget to boolean data in a checked box.
 *@version $Revision: 1.21 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *Just set the datasource and the column (defined in the parent class @ref hk_dsdatavisible).
 *To define which value represents TRUE and which one FALSE see @ref hk_column::setboolvalues.
 */

class hk_kdeboolean : public QCheckBox, public hk_dsboolean
{
    Q_OBJECT
        public:
        hk_kdeboolean(QWidget* wid,hk_form* form=NULL);
        ~hk_kdeboolean(void);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);
	virtual void set_value(const hk_string&);
	virtual hk_string value(void);

    protected:
     virtual void focusInEvent ( QFocusEvent * );
     virtual void focusOutEvent ( QFocusEvent * );
     virtual void    widget_specific_tooltip_changed(void);
        virtual bool    widget_specific_row_change(void);
        virtual bool    widget_specific_coordinates(unsigned int px,unsigned int py,unsigned int pwidth,unsigned int pheight);
        virtual void    widget_specific_label_changed(void);
        virtual void    widget_specific_font_changed(void);
        virtual void    widget_specific_insert_mode(void);
        virtual void    widget_specific_row_deleted(void);
        virtual void    widget_specific_row_added(void);
	virtual void widget_specific_enabled_changed(void);
        void   keyPressEvent ( QKeyEvent * e );
        virtual void resizeEvent (QResizeEvent *);
        virtual void  moveEvent ( QMoveEvent * ) ;
        virtual void mousePressEvent(QMouseEvent* m);
        virtual void            mouseDoubleClickEvent(QMouseEvent*);
        virtual void widget_specific_backgroundcolour_changed(const hk_colour& oldcolour);
        virtual void widget_specific_foregroundcolour_changed(const hk_colour& oldcolour);
	virtual bool	presentationmode_changed(void);

    protected slots:
        void     slot_data_changed(void);

    private:
        void    reposition(void);
        bool p_widget_specific_row_change;
        void    widget_specific_enable_disable(void);
};
#endif
