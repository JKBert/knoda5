// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// This file is part of the hk_classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_KDEDATE
#define HK_KDEDATE
#include "hk_dsdate.h"
#include <qframe.h>
#include <qdatetime.h>

class hk_kdedateprivate;

class hk_kdedate : public QFrame, public hk_dsdate
{
Q_OBJECT

public:
  hk_kdedate(QWidget* wid,hk_form* form=NULL);
  virtual ~hk_kdedate();
  virtual void show_widget(void);
  virtual void hide_widget(void);
  virtual bool close_widget(void);
  virtual void set_value(const hk_string&);
  virtual hk_string value(void);
  virtual void update_filter(void);
public slots:
  virtual void slot_date_selected(QDate);
signals:
  void lostFocus();

protected:
  virtual void  resizeEvent (QResizeEvent *);
        virtual void  moveEvent ( QMoveEvent * ) ;
        virtual bool  widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight);
        virtual bool  widget_specific_row_change(void);
        virtual void  widget_specific_enable_disable(void);
        
        
        virtual void    widget_specific_insert_mode(void);
        virtual void    widget_specific_row_added(void);
        virtual void    widget_specific_row_deleted(void);
        virtual void    widget_specific_alignment(void);
	virtual void    widget_specific_tooltip_changed(void);
        
        
        virtual void            mousePressEvent(QMouseEvent*);
        virtual void            mouseDoubleClickEvent(QMouseEvent*);
     virtual void focusInEvent ( QFocusEvent * );
     virtual void focusOutEvent ( QFocusEvent * );
        virtual void widget_specific_backgroundcolour_changed(const hk_colour&);
        virtual void widget_specific_foregroundcolour_changed(const hk_colour&);
        virtual void widget_specific_font_changed(void);
	virtual void widget_specific_enabled_changed(void);
	virtual bool	presentationmode_changed(void);

    protected slots:
        void     slot_data_changed(void);
        void slot_table_clicked(void);
        void button_clicked(void);
        void     slot_focus_lost(void);


private:
	        void    reposition(void);
        bool    p_widget_specific_row_change;
 hk_kdedateprivate* p_private;
};



class datepopupprivate;

class datepopup:public QFrame
{
  Q_OBJECT
  public:
  datepopup(QWidget* wid,hk_kdedate*);
  virtual ~datepopup();
  int exec(const QWidget* trigger);
  
  protected:
  virtual void mouseReleaseEvent(QMouseEvent*);
  virtual void keyPressEvent(QKeyEvent*);
  bool eventFilter(QObject* object,QEvent* event);
  
  private:
  datepopupprivate* p_private;
};

#endif //hk_kdedate
