// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for hk_kde5classes library
//
// This file is part of the hk_kde5classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef HK_KDEDBDESIGNER
#define HK_KDEDBDESIGNER
#include <qwidget.h>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qframe.h>
#include <qpixmap.h>
#include <kparts/mainwindow.h>
#include <hk_dbvisible.h>
#include <hk_referentialintegrity.h>
#include <hk_dsdatavisible.h>
#include <kde4classesmacros.h>

class hk_kdefieldlistPrivate;
class hk_presentation;
class hk_kdedbdesignerprivate;
class hk_kdedatasourceframe;
class QVBoxLayout;
class QGridLayout;
class hk_kdedbrelation;
class QScrollArea;
class hk_kdefieldlistPrivate;

class KDECLASSES_EXPORT hk_kdedbdesigner:public QWidget, public hk_dbvisible
{
    Q_OBJECT
public:
  hk_kdedbdesigner(QWidget* parent=0);
  virtual ~hk_kdedbdesigner();
  void set_presentation(hk_presentation*);
  hk_presentation* presentation(void);
  QScrollArea* scrollview(void) const;
  hk_kdedatasourceframe* get_dsframe(hk_datasource*);
  hk_kdedbrelation* get_relation(hk_datasource* ds);
  hk_kdedbrelation* get_relation(hk_datasource* master,hk_datasource* slave);
  hk_kdedbrelation* get_relation(hk_kdedatasourceframe* master,hk_kdedatasourceframe* slave);
  referentialclass* get_referentialintegrity(hk_datasource* master,hk_datasource* slave);
  hk_kdedbrelation* add_relation(hk_kdedatasourceframe* master,hk_kdedatasourceframe* slave);
  hk_kdedbrelation* add_relation(hk_datasource* master,hk_datasource* slave);
  hk_kdedbrelation* add_referentialintegrity(referentialclass ref, hk_kdedatasourceframe* ds);
  hk_kdedatasourceframe* add_dsframe(hk_datasource*);
  void set_all_relations(void);

signals:
  void signal_definition_has_changed(void);
  void signal_field_doubleclicked(int,const hk_string&);

public slots:
  void clear_datasources();
  void set_datasources();
  void relation_clicked(hk_kdedbrelation*);
  void new_focus(hk_kdedatasourceframe*);
  void delete_datasource(hk_kdedatasourceframe*);
  void delete_relation(hk_kdedbrelation*);
  void add_datasource(void);

  void slot_field_doubleclicked(int, const hk_string&);

private:
  void raise_datasources(void);
  hk_datasource* already_added_table(const hk_string&t);
  void remove_relation(hk_kdedbrelation*);

  hk_kdedbdesignerprivate* p_private;
};







class kdedatasourcelabel:public QLabel
{
public:
  friend class hk_kdedatasourceframe;
  kdedatasourcelabel(hk_kdedatasourceframe* w);
protected:
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent*);
  bool p_drag;
  int p_startx;
  int p_starty;
  int p_offsetx;
  int p_offsety;
  hk_kdedatasourceframe* p_datasourceframe;
};

class hk_kdefieldlistPrivate;

class hk_kdefieldlist:public QListWidget
{
Q_OBJECT
public:
  hk_kdefieldlist(hk_kdedatasourceframe* parent);
  ~hk_kdefieldlist();
  void clearFocus(void);
  QSize sizeHint() const;

signals:
  void signal_field_doubleclicked(int,const hk_string&);

protected:
  void mouseMoveEvent(QMouseEvent*);
  void focusInEvent(QFocusEvent * event);

  virtual void mouseDoubleClickEvent(QMouseEvent*);
  void dragEnterEvent(QDragEnterEvent*);
  void dropEvent(QDropEvent* );
  virtual void contextMenuEvent(QContextMenuEvent*);

private:
  hk_kdedatasourceframe* p_datasourceframe;
  QPoint p_dragstartposition;
  hk_kdefieldlistPrivate *dptr;
};

class hk_kdedatasourceframe :public QFrame, public hk_dsdatavisible
{
Q_OBJECT
public:
  friend class hk_kdedbdesigner;
  hk_kdedatasourceframe(hk_kdedbdesigner* designer,QWidget* parent,hk_datasource* ds);
  void set_fields(void);
  void set_focus(hk_kdedatasourceframe*);
  void clearFocus(void);
  void mousePressEvent(QMouseEvent*);
  hk_kdedbdesigner* designer() const;
  kdedatasourcelabel* header() const {return p_header;}

public slots:
  void edit(void);
  void set_header(void);
signals:
  void signal_moved(void);

protected:
  virtual void moveEvent(QMoveEvent*);
  virtual void resizeEvent(QResizeEvent*);
  bool eventFilter(QObject* object,QEvent* event);

private:
  void set_new_position(void);
  hk_kdedbdesigner* p_designer;
  QListWidget* 	p_listbox;
  QGridLayout*	p_layout;
  kdedatasourcelabel* p_header;
  bool p_positionupdate;
  QPixmap keyicon;

};  //end hk_kdedatasourceframe

class hk_kdedbrelationPrivate;

class hk_kdedbrelation:public QWidget,public hk_class
{
Q_OBJECT
public:
  hk_kdedbrelation(QWidget* parent, bool is_integrity);
  ~hk_kdedbrelation();
  void set_datasources(hk_kdedatasourceframe* master,hk_kdedatasourceframe* slave);
  void set_referentialintegrity(referentialclass c,hk_kdedatasourceframe* master,hk_kdedatasourceframe* slave);
  hk_kdedatasourceframe* masterdatasource(void) const;
  hk_kdedatasourceframe* slavedatasource(void) const;
  void setFocus();
  void set_focus(void);
  hk_string referentialname()const;
  bool updatecascade()const;
  bool deletecascade()const;
signals:
  void signal_relation_clicked(hk_kdedbrelation*);

public slots:
  void slot_relation_clicked(hk_kdedbrelation*);
  void edit(void);

protected:
  void paint_relation(QPainter*);
  void paintEvent(QPaintEvent*);
  void keyPressEvent(QKeyEvent*);
  QString tooltipfields(referentialclass* c=NULL);
  virtual void mouseMoveEvent(QMouseEvent*);
  virtual void mousePressEvent(QMouseEvent*);
  virtual void mouseDoubleClickEvent(QMouseEvent*);
  virtual void contextMenuEvent(QContextMenuEvent*);

protected slots:
  void datasource_moved(void);

private:
  hk_kdedatasourceframe* p_masterdatasource;
  hk_kdedatasourceframe* p_slavedatasource;
  hk_kdedbrelationPrivate* dptr;
  int p_fromx,p_fromy,p_tox,p_toy;
  bool p_is_focused;
  bool p_is_referentialintegrity;
  hk_string p_refname;
  bool p_deletecascade;
  bool p_updatecascade;
};


class hk_kdemovewidget: public QWidget
{
Q_OBJECT
public:
  enum enum_orientation {horizontal,vertical,diagonal};
  hk_kdemovewidget( enum_orientation orientation, QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
  ~hk_kdemovewidget();
  enum_orientation orientation(void) const {return p_orientation;}
protected:
  void mousePressEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent*);

private:
  bool p_drag;
  int p_startx;
  int p_starty;
  int p_offsetx;
  int p_offsety;
  enum_orientation p_orientation;

};

class hk_kdedbdesignerwindowprivate;

class hk_kdedbdesignerwindow: public KParts::MainWindow
{
Q_OBJECT
public:
  hk_kdedbdesignerwindow( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
  ~hk_kdedbdesignerwindow();
  hk_kdedbdesigner* designer(void) const;

signals:
  void signal_closed(void);

protected:
  void closeEvent(QCloseEvent*);

private:
  hk_kdedbdesignerwindowprivate* p_private;
};

#endif //HK_KDEDBDESIGNER
