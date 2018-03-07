// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
//
// This file is part of the hk_kde4classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_kdeimage.h"
#include "hk_kdesimpleform.h"
#include <hk_form.h>

#include <klocale.h>
#include <kapplication.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kurl.h>

#include <qlabel.h>
#include <qscrollarea.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <qlayout.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qmenu.h>
#include <qcursor.h>
#include <qscrollbar.h>

const QPalette::ColorRole hk_kdeimage::viewportBackground = QPalette::Mid;

internal_kdelabel::internal_kdelabel(QWidget* p,hk_kdeimage* i):QLabel(p)
{
    p_image=i;
    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(i);
}

void internal_kdelabel::contextMenuEvent ( QContextMenuEvent* )
{
    QMenu contextMenu(this);
    
    contextMenu.addAction( i18n("&Fit to size"),  this, SLOT(slot_fit2size()),Qt::Key_F);
    contextMenu.addAction( i18n("O&riginal size"),  this, SLOT(slot_originalsize()),Qt::Key_1 );
    contextMenu.addAction( i18n("Zoom &in"),  this, SLOT(slot_zoomin()),Qt::Key_Plus );
    contextMenu.addAction( i18n("Zoom &out"),  this, SLOT(slot_zoomout()),Qt::Key_Minus );
    if (p_image->column()&& !p_image->is_readonly())
    {
      contextMenu.addSeparator();
      contextMenu.addAction( i18n("&Load image"),  this, SLOT(slot_set_image()));
      if (p_image->column()->columntype()==hk_column::binarycolumn)
        contextMenu.addAction( i18n("&Save image"),  this, SLOT(slot_save_image()));
    }

    contextMenu.exec( QCursor::pos() );
}



void internal_kdelabel::slot_set_image(void)
{
  QString dir=QString::fromUtf8(l2u(p_image->path()).c_str());
  if (p_image->column()&&p_image->column()->columntype()==hk_column::binarycolumn) {
    dir="kfiledialog:///image";
  }
  KUrl url = KFileDialog::getImageOpenUrl(dir , this );
  if (!url.fileName().isEmpty()) {
    hk_string v=u2l(url.directory()==dir?
                               url.fileName().toUtf8().data():
                               url.path().toUtf8().data()
                      );
    if (p_image->column()&&p_image->column()->columntype()==hk_column::binarycolumn) {
      struct_raw_data d;
      if (load_file(u2l(url.path().toUtf8().data()),&d)) {
        p_image->column()->set_asbinary(&d);
        delete[] d.data;
      }
      else return;
    }
    else
      p_image->set_value(v);
    p_image->show_image();
    emit signal_image_set();
  }
}

void internal_kdelabel::slot_save_image(void)
{
  if (!p_image->column()) return;
  QString s=KFileDialog::getSaveFileName(KUrl("kfiledialog:///image"),QString::null,this);
  if (!s.isEmpty()) p_image->column()->save_to_file(u2l(s.toUtf8().data()));

}

void  internal_kdelabel::slot_fit2size(void)
{  p_image->set_zoom(0);
  p_image->show_image();
}

void  internal_kdelabel::slot_originalsize(void)
{
  p_image->set_zoom(100);
  p_image->show_image();
}

void  internal_kdelabel::slot_zoomin(void)
{
  if (p_image->zoom()==0)
   p_image->set_zoom(100);
  else
   p_image->set_zoom(p_image->zoom()+25);

  p_image->show_image();
}

void  internal_kdelabel::slot_zoomout(void)
{
  p_image->set_zoom(p_image->zoom()-25);
  p_image->show_image();
}




void  internal_kdelabel::mousePressEvent(QMouseEvent* event)
{
   QLabel::mousePressEvent(event);
   if (p_image->presentation() && p_image->presentation()->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           p_image->action_on_click();
}



void  internal_kdelabel::mouseDoubleClickEvent(QMouseEvent* event)
{
   QLabel::mouseDoubleClickEvent(event);
   if (p_image->presentation() && p_image->presentation()->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           p_image->action_on_doubleclick();
}

class hk_kdeimageprivate
{
public:
hk_kdeimageprivate()
   {
    p_label=NULL;
   }

internal_kdelabel* p_label;
QPixmap p_pixmap;
QImage  p_image;
QScrollArea* p_scrollview;
int cm_x;
int cm_y;
};





hk_kdeimage::hk_kdeimage(QWidget* wid,hk_form*f ):QFrame(wid),hk_dsimage(f)
{
#ifdef HK_DEBUG  
  //wanna_debug(true);
  hkdebug("hk_kdeimage::constructor");
#endif
  p_private=new hk_kdeimageprivate;
  QBoxLayout * l = new QHBoxLayout( this );
  l->setMargin(0);
  p_private->p_scrollview=new QScrollArea(this);
  l->addWidget(p_private->p_scrollview);
  p_private->p_label=new internal_kdelabel(p_private->p_scrollview,this);
  p_private->p_label->installEventFilter(dynamic_cast<hk_kdesimpleform*>(f));
  p_private->p_scrollview->setWidget(p_private->p_label);
  p_private->p_label->setText(i18n("No image"));
  // to enable background drawing
  p_private->p_scrollview->setBackgroundRole(hk_kdeimage::viewportBackground);
  p_private->cm_x=p_private->p_scrollview->contentsMargins().left() + p_private->p_scrollview->contentsMargins().right();
  p_private->cm_y=p_private->p_scrollview->contentsMargins().top() + p_private->p_scrollview->contentsMargins().bottom();

  connect (p_private->p_label,SIGNAL(signal_image_set()),this,SLOT(slot_set_image()));
  setFocusPolicy(Qt::StrongFocus);

  QColor qcolor=p_private->p_label->palette().color(p_private->p_label->backgroundRole());
  hk_colour colour(qcolor.red(),qcolor.green(),qcolor.red());
  set_backgroundcolour(colour,false,true);
  qcolor=p_private->p_label->palette().color(p_private->p_label->foregroundRole());
  colour.set_colour(qcolor.red(),qcolor.green(),qcolor.blue());
  set_foregroundcolour(colour,false,true);
}

hk_kdeimage::~hk_kdeimage()
{
delete p_private;
}

void hk_kdeimage::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeimage::resizeEvent");
#endif
    QFrame::resizeEvent(event);
    reposition();

}


void  hk_kdeimage::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeimage::resizeEvent");
#endif
    QFrame::moveEvent(event);
    reposition();
}
void hk_kdeimage::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("kdeimage::reposition");
#endif
    if (p_presentation==NULL ||p_setcoordinates==true) return;
    p_setwidgetcoordinates=true;
    unsigned int ppx=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QWidget::x()):QWidget::x();
    unsigned int ppy=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QWidget::y()):QWidget::y();
    unsigned int pph=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QWidget::height()):QWidget::height();
    unsigned int ppw=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QWidget::width()):QWidget::width();
    set_size(ppx,ppy,ppw,pph);
    p_setwidgetcoordinates=false;    
}

bool hk_kdeimage::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
#ifdef HK_DEBUG
  hkdebug("hk_kdeimage::widget_specific_coordinates");
#endif
  setGeometry(px,py,pwidth,pheight);
//     qApp->processEvents();
  return true;
}


bool    hk_kdeimage::widget_specific_row_change(void)
{
show_image();
return hk_dsdatavisible::widget_specific_row_change();
}



void hk_kdeimage::show_image()
{
if (column() && column()->columntype()==hk_column::binarycolumn)
 {
 const struct_raw_data* d=(column()->has_changed()?column()->changed_data():column()->asbinary());
 if (!d) return;
if ((zoom()==100?
         p_private->p_pixmap.loadFromData((const uchar*)d->data,(uint)d->length)
        :p_private->p_image.loadFromData((const uchar*)d->data,(uint)d->length))
    )
 {
   zoom_image();
   return;
 }

 }// end binary column


if ((zoom()==100?
         p_private->p_pixmap.load(QString::fromUtf8(l2u(value()).c_str()))
        :p_private->p_image.load(QString::fromUtf8(l2u(value()).c_str())))
    )
 {
   zoom_image();
   return;
 }

if (localimage() &&localimage()->data)
  {
  if ((zoom()==100?
         p_private->p_pixmap.loadFromData((const uchar*)localimage()->data,(uint)localimage()->length)
        :p_private->p_image.loadFromData((const uchar*)localimage()->data,(uint)localimage()->length))
    )
 {
   zoom_image();
   return;
 }


  }

  p_private->p_label->setGeometry(0,0,QWidget::width()-p_private->cm_x,QWidget::height()-p_private->cm_y);
  p_private->p_label->setText(i18n("No image"));

}


void hk_kdeimage::zoom_image(void)
{
#ifdef HK_DEBUG
  hkdebug("kdeimage::zoom_image");
#endif
  if (zoom()==0) {  // fit to frame
    p_private->p_pixmap.convertFromImage(

  (
   p_private->p_image.width()<QWidget::width() - p_private->cm_x
   &&
   p_private->p_image.height()<QWidget::height() - p_private->cm_y
  )? p_private->p_image :
  p_private->p_image.scaled(QWidget::width() - p_private->cm_x,
                QWidget::height() - p_private->cm_y, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  }
  else {
    if (zoom()!=100) {
      int w=(p_private->p_image.width()*zoom())/100;
      int h=(p_private->p_image.height()*zoom())/100;
      p_private->p_pixmap.convertFromImage(
      p_private->p_image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
  }
  p_private->p_label->setGeometry(0,0,p_private->p_pixmap.width(),p_private->p_pixmap.height());
  p_private->p_label->setPixmap(p_private->p_pixmap);
}

void   hk_kdeimage::widget_specific_enable_disable(void)
{

    if ((datasource()!=NULL)&&(column()!=NULL))
    {
#ifdef HK_DEBUG
        hkdebug("hk_kdeimage::if ((datasource()!=NULL)&&(column()!=NULL))");
#endif

        if (datasource()->is_enabled())
        {

            widget_specific_row_change();
        }
       // else setEnabled(false);
    }
    else
    {
	setEnabled(is_enabled());
        //blockSignals(true);
        p_private->p_label->clear();
        p_private->p_label->setText(i18n("No image "));
        show_image();
        //blockSignals(false);
    }



}

void    hk_kdeimage::widget_specific_insert_mode(void)
{
    p_private->p_label->setText(i18n("No image "));
}

void hk_kdeimage::slot_set_image(void)
{
    widget_specific_row_change();
}


void hk_kdeimage::show_widget(void)
{
  show();
}

void hk_kdeimage::hide_widget(void)
{
  hide();
}

bool hk_kdeimage::close_widget(void)
{
  return close();
}

void hk_kdeimage::focusInEvent ( QFocusEvent * e)
{
  //setFrameStyle(QFrame::Panel | QFrame::Sunken);
  action_on_getfocus();
  QFrame::focusInEvent(e);
}

void hk_kdeimage::focusOutEvent ( QFocusEvent * e)
{
//  setFrameStyle(QFrame::Panel | QFrame::Plain);
  action_on_loosefocus();
  QFrame::focusOutEvent(e);
}






void hk_kdeimage::keyPressEvent( QKeyEvent * e )
{
    hk_key nk=hk_key(e->key(),e->modifiers(),(e->text().isNull()?"":u2l(e->text().toUtf8().data())));
       set_key(nk);
       action_on_key();
       hk_key* k=key();
    if (!k->accept_key())
    {
      e->ignore();
      return;

    }

     if (e->key()==Qt::Key_F )
        p_private->p_label->slot_fit2size();
     else
     if (e->key()==Qt::Key_Plus )
        p_private->p_label->slot_zoomin();
     else
     if (e->key()==Qt::Key_Minus )
        p_private->p_label->slot_zoomout();
     else
     if (e->key()==Qt::Key_1 && zoom()!=100)
        p_private->p_label->slot_originalsize();
     else
     if (e->key()==Qt::Key_2 )
       {
        int val=e->modifiers()&Qt::ControlModifier?50:200;
        if (zoom()!=val)
        {
         set_zoom(val);
         show_image();
        }
       }
     else
     if (e->key()==Qt::Key_3 )
       {
        int val=e->modifiers()&Qt::ControlModifier?33:300;
        if (zoom()!=val)
        {
         set_zoom(val);
         show_image();
        }
       }


   QFrame::keyPressEvent(e);
}

void    hk_kdeimage::widget_specific_backgroundcolour_changed(const hk_colour&)
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette plabel(p_private->p_label->palette());
    QPalette pview(p_private->p_scrollview->viewport()->palette());
    
    plabel.setColor(p_private->p_label->backgroundRole(),newcolour);
    pview.setColor(hk_kdeimage::viewportBackground,newcolour);
    p_private->p_label->setPalette(plabel);
    p_private->p_scrollview->viewport()->setPalette(pview);
}

void    hk_kdeimage::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(p_private->p_label->palette());
    
    p.setColor(p_private->p_label->foregroundRole(),newcolour);
    p_private->p_label->setPalette(p);
}


void hk_kdeimage::set_value(const hk_string& v)
{
  hk_dsimage::set_value(v);
  if (!column()) show_image();
}

void hk_kdeimage::widget_specific_font_changed(void)
{
    p_private->p_label->setFont(QFont(QString::fromUtf8 (l2u(hk_dsimage::font().fontname()).c_str())
                  ,hk_dsimage::font().fontsize(),hk_dsimage::font().bold()?QFont::Bold:QFont::Normal,
		   hk_dsimage::font().italic()));
}

bool hk_kdeimage::load_localimage(const hk_string& filename,bool registerchange)
{
  bool result=hk_dsimage::load_localimage(filename,registerchange);
  if (result)
    show_image();
  return result;

}

void hk_kdeimage::widget_specific_enabled_changed(void)
{
  setEnabled(is_enabled());
}

bool	hk_kdeimage::presentationmode_changed(void)
{
bool result=hk_dsimage::presentationmode_changed();
  if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)setEnabled(is_enabled());
  return result;
}

