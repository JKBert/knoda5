// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_kdeclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU General Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef HK_KDEROWSELECTOR
#define HK_KDEROWSELECTOR
#include <qwidget.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <klineedit.h>
#include <hk_dsrowselector.h>
#include <hk_datasource.h>

class hk_form;
/**
 *
 *@short KDE Widget to select a specific row
 *@version $Revision: 1.11 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *Just set the datasource (defined in the parent class @ref hk_dsvisible) to use it.
 *You can press the buttons to change to the first, previous,next, last row, or enter
 *the wished row number (first row is number 1).
 */

class hk_kderowselector : public QWidget , public  hk_dsrowselector
{
    Q_OBJECT

        public:
        hk_kderowselector(QWidget* wid=0,hk_form* form=NULL);
        virtual ~hk_kderowselector();

    protected slots:
        void    slotpress_gotofirst(void);
        void    slotpress_gotoprevious(void);
        void    slotpress_gotonext(void);
        void    slotpress_gotolast(void);
        void    slotpress_storechanges(void);
        void    slotpress_delete(void);
        void    slotpress_insert(void);
        void    slotlineedit(void);

        signals:
        void    previousrow_clicked();
        void    nextrow_clicked();
        void    insertrow_clicked();
        void    lastrow_clicked();
        void    firstrow_clicked();
        void    deleterow_clicked();
/**
 * signal will be emitted whenever the rowselector is used
 */
        void    rowselector_clicked();

    protected:
     virtual void focusInEvent ( QFocusEvent * );
     virtual void focusOutEvent ( QFocusEvent * );
     virtual void    widget_specific_tooltip_changed(void);
        void    widget_specific_enable_disable(void);
        bool    widget_specific_row_change(void);
        void    datasource_delete(void);
        bool    datasource_enable(void);
        virtual void        widget_specific_insert_mode(void);
        virtual void resizeEvent (QResizeEvent *);
        virtual void  moveEvent ( QMoveEvent * ) ;
        bool    widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight);
        virtual void widget_specific_backgroundcolour_changed(const hk_colour&);
        virtual void widget_specific_foregroundcolour_changed(const hk_colour&);
	virtual void widget_specific_enabled_changed(void);
	virtual bool	presentationmode_changed(void);

    private:
        QPushButton* button_delete;
        QPushButton* button_insert;
        QPushButton* button_gotofirst;
        QPushButton* button_gotoprevious;
        QPushButton* button_gotonext;
        QPushButton* button_gotolast;
        QPushButton* button_store;
        KLineEdit*   label;
        QHBoxLayout* layout;
        void    reposition(void);

};
#endif
