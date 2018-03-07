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
#ifndef HK_KDEIMAGE
#define HK_KDEIMAGE
#include <hk_dsimage.h>
#include <qframe.h>
#include <qlabel.h>
class hk_kdeimageprivate;

class hk_kdeimage : public QFrame,public hk_dsimage
{
  Q_OBJECT
  friend class internal_kdelabel;
public:
  hk_kdeimage(QWidget* wid,hk_form* form=NULL);
  virtual ~hk_kdeimage();
  virtual void show_widget(void);
  virtual void hide_widget(void);
  virtual bool close_widget(void);
  virtual void set_value(const hk_string&);
  virtual bool load_localimage(const hk_string& filename,bool registerchange=true);
  virtual void show_image(void);
protected:
  virtual void  resizeEvent (QResizeEvent *);
  virtual void  moveEvent ( QMoveEvent * ) ;
  virtual bool  widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight);
  virtual bool  widget_specific_row_change(void);
  virtual void  widget_specific_enable_disable(void);
  virtual void  widget_specific_insert_mode(void);
  virtual void widget_specific_backgroundcolour_changed(const hk_colour&);
  virtual void widget_specific_foregroundcolour_changed(const hk_colour&);
  virtual void widget_specific_font_changed(void);
  virtual void widget_specific_enabled_changed(void);
  virtual void focusInEvent ( QFocusEvent * );
  virtual void focusOutEvent ( QFocusEvent * );
  void    keyPressEvent ( QKeyEvent * e );
  virtual bool	presentationmode_changed(void);
protected slots:
  void slot_set_image(void);
private:
  const static QPalette::ColorRole viewportBackground;
  void zoom_image(void);
  void    reposition(void);
  hk_kdeimageprivate* p_private;
};


class internal_kdelabel:public QLabel
{
 Q_OBJECT

 signals:
 void signal_image_set();
 public:
  internal_kdelabel(QWidget* parent, hk_kdeimage*);
 public slots:
  void slot_set_image(void);
  void  slot_save_image(void);
  void  slot_fit2size(void);
  void  slot_originalsize(void);
  void  slot_zoomin(void);
  void  slot_zoomout(void);

 protected:
 virtual void   mousePressEvent(QMouseEvent*);
 virtual void   mouseDoubleClickEvent(QMouseEvent*);

 private:
 void contextMenuEvent ( QContextMenuEvent*);
 hk_kdeimage* p_image;
};


#endif // hk_kdeimage
