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
//$Revision: 1.33 $
#include "hk_kdelabel.h"
#include "hk_kdesimpleform.h"
#include <qapplication.h>
#include <qpainter.h>
#include <qtooltip.h>
#include <QMouseEvent>
#include <klocale.h>

hk_kdelabel::hk_kdelabel(hk_kdesimpleform* form):QLabel(form), hk_label(form)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdelabel::hk_kdelabel");
#endif
    set_label("");
    if(form)
    {
        QColor qcolor=form->palette().color(form->backgroundRole());
        hk_colour colour(qcolor.red(),qcolor.green(),qcolor.red());
        set_backgroundcolour(colour,false,true);
        qcolor=form->palette().color(form->foregroundRole());
        colour.set_colour(qcolor.red(),qcolor.green(),qcolor.red());
        set_foregroundcolour(colour,false,true);
    }
    widget_specific_font_changed();
    setFrameStyle(QFrame::NoFrame);
    setAutoFillBackground(true);
}


void    hk_kdelabel::widget_specific_label_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdelabel::widget_specific_label_changed");
#endif
    if (label().size()>0)
     setText(QString::fromUtf8 (l2u(label()).c_str()));
    else
    {
      if ((topline()+bottomline()+leftline()+rightline()+diagonalloru()+diagonalluro())==0)
      		setText(i18n("label"));
      else 	setText("");
    }
}


bool hk_kdelabel::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
#ifdef HK_DEBUG
    hkdebug("kdelabel::widget_specific_coordinates");
#endif
    setGeometry(px,py,pwidth,pheight);
//     qApp->processEvents();

    return true;
}

void  hk_kdelabel::mousePressEvent(QMouseEvent* event)
{
   QLabel::mousePressEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_click();
}


void  hk_kdelabel::mouseDoubleClickEvent(QMouseEvent* event)
{
   QLabel::mouseDoubleClickEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_doubleclick();
}



void hk_kdelabel::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("kdelabel::resizeEvent");
#endif
    QLabel::resizeEvent(event);
    reposition();

}


void  hk_kdelabel::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("kdelabel::resizeEvent");
#endif
    QLabel::moveEvent(event);
    reposition();
}


void hk_kdelabel::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("kdelabel::reposition");
#endif
    if (p_presentation==NULL ||p_setcoordinates==true) return;
    p_setwidgetcoordinates=true;
    unsigned int ppx=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QLabel::x()):QLabel::x();
    unsigned int ppy=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QLabel::y()):QLabel::y();
    unsigned int pph=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QLabel::height()):QLabel::height();
    unsigned int ppw=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QLabel::width()):QLabel::width();
    set_size(ppx,ppy,ppw,pph);
    p_setwidgetcoordinates=false;
}


void hk_kdelabel::widget_specific_font_changed(void)
{
    setFont(QFont(QString::fromUtf8 (l2u(hk_label::font().fontname()).c_str())
                  ,hk_label::font().fontsize(),hk_label::font().bold()?QFont::Bold:QFont::Normal,
		   hk_label::font().italic()));
}


void hk_kdelabel::widget_specific_alignment(void)
{
    Qt::Alignment f;
    
    switch (hk_label::alignment())
    {
        case alignright  : f= Qt::AlignRight;
        break;
        case aligncenter  : f= Qt::AlignHCenter;
        break;
        default       : f= Qt::AlignLeft;

    }
    setAlignment(f);
    setWordWrap(true);
}


void    hk_kdelabel::widget_specific_backgroundcolour_changed(const hk_colour&)
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(backgroundRole(),newcolour);
    setPalette(p);
}


void    hk_kdelabel::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(foregroundRole(),newcolour);
    setPalette(p);
}

void hk_kdelabel::paintEvent(QPaintEvent* event)
{
  QLabel::paintEvent(event);
  QPainter p(this); 
  drawFrame(&p);
}

void hk_kdelabel::drawFrame(QPainter* p)
{
    p->setPen(palette().color(foregroundRole()));
    QPen pen=p->pen();
    if (leftline()>0)
    	{
	    pen.setWidth(leftline());
	    p->setPen(pen);
		p->drawLine(0,0,0,QWidget::height());
	}
    if (rightline()>0)
    	{
	    pen.setWidth(rightline());
	    p->setPen(pen);
		p->drawLine(QWidget::width()-1,0,QWidget::width()-1,QWidget::height());
	}
    if (bottomline()>0)
    	{
	    pen.setWidth(bottomline());
	    p->setPen(pen);
		p->drawLine(0,QWidget::height()-1,QWidget::width()-1,QWidget::height()-1);
	}
    if (topline()>0)
    	{
	    pen.setWidth(topline());
	    p->setPen(pen);
		p->drawLine(0,0,QWidget::width(),0);
	}
    if (diagonalluro()>0)
    	{
	    pen.setWidth(diagonalluro());
	    p->setPen(pen);
		p->drawLine(0,QWidget::height()-1,QWidget::width(),0);
	}

    if (diagonalloru()>0)
    	{
	    pen.setWidth(diagonalloru());
	    p->setPen(pen);
		p->drawLine(0,0,QWidget::width()-1,QWidget::height()-1);
	}

}

void hk_kdelabel::widget_specific_topline_changed(void)
{
  set_borders();
}

void hk_kdelabel::widget_specific_bottomline_changed(void)
{
  set_borders();
}

void hk_kdelabel::widget_specific_leftline_changed(void)
{
  set_borders();
}

void hk_kdelabel::widget_specific_rightline_changed(void)
{
  set_borders();
}

void hk_kdelabel::widget_specific_diagonalluro_changed(void)
{
  set_borders();
}

void hk_kdelabel::widget_specific_diagonalloru_changed(void)
{
  set_borders();
}

void hk_kdelabel::set_borders(void)
{
  reposition();
  widget_specific_label_changed();

  repaint();
}

void  hk_kdelabel::widget_specific_tooltip_changed(void)
{
    setToolTip(QString::fromUtf8 (l2u(tooltip()).c_str()));
}


void hk_kdelabel::show_widget(void)
{
  show();
}

void hk_kdelabel::hide_widget(void)
{
  hide();
}

bool hk_kdelabel::close_widget(void)
{
  return close();
}

bool hk_kdelabel::presentationmode_changed()
{
if (!p_presentation)
  {
    setEnabled(is_enabled());
    return hk_label::presentationmode_changed();
  }
bool result=hk_label::presentationmode_changed();
   setEnabled(is_enabled());
  return  result;
}


