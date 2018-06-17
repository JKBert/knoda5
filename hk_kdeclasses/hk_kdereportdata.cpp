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

#include "hk_kdereportdata.h"
#include "hk_kdereportsection.h"
#include "hk_report.h"

#include <qpainter.h>
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qevent.h>

const unsigned int teiler=10000;

hk_kdereportdata::hk_kdereportdata (hk_kdereportsection*s, const char* name, Qt::WFlags fl )
: QWidget( s, fl ) ,hk_reportdata(s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportdata::hk_kdereportdata");
#endif
    setObjectName(QString::fromAscii(name));
    p_kdesection=s;
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    p.setColor(foregroundRole(),newcolour);
    c=backgroundcolour();
    newcolour.setRgb(c.red(),c.green(),c.blue());
    p.setColor(backgroundRole(),newcolour);
    setAutoFillBackground(true);
    pix_je_cm=(QApplication::desktop()->screenGeometry(this).width()/1024.0)*32.0 ;
}


hk_kdereportdata::~hk_kdereportdata(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportdata::~hk_kdereportdata");
#endif
}


void hk_kdereportdata::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    QColor foreColor = palette().color(foregroundRole());
    
    p.setClipRegion( e->rect() );
    p.setPen(palette().color(backgroundRole()));
    p.setPen( Qt::white );
    p.drawRect(0,0,QWidget::width(),QWidget::height());
    QString d=QString::fromUtf8 ((columnname().size()>0?l2u(columnname()).c_str():"-"));
    d+=" / ";
    d+=QString::fromUtf8 ((hk_reportdata::data().size()>0?l2u(hk_reportdata::data()).c_str():"-"));
    p.setPen(foreColor);
    
    QFont f=QWidget::font();
    QFontInfo info(f);
    p.drawText(5,info.pixelSize()+1,d);
    QColor noline=palette().color(QPalette::Mid);
    
    p.setPen(leftline()?foreColor:noline);    
    p.drawLine(0,0,0,QWidget::height());
    
    p.setPen(rightline()?foreColor:noline);
    p.drawLine(QWidget::width()-1,0,QWidget::width()-1,QWidget::height()-1);
    
    p.setPen(bottomline()?foreColor:noline);
    p.drawLine(0,QWidget::height()-1,QWidget::width()-1,QWidget::height()-1);
    
    p.setPen(topline()?foreColor:noline);
    p.drawLine(0,0,QWidget::width(),0);

    p.setPen(foreColor);
    if (diagonalluro()) p.drawLine(0,QWidget::height()-1,QWidget::width(),0);
    if (diagonalloru()) p.drawLine(0,0,QWidget::width()-1,QWidget::height()-1);
}


hk_kdereportsection* hk_kdereportdata::section(void)
{
    return p_kdesection;
}


void hk_kdereportdata::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("kdereportdata::resizeEvent");
#endif

    QWidget::resizeEvent(event);
    reposition();

}


void  hk_kdereportdata::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("kdereportdata::moveEvent");
#endif
    QWidget::moveEvent(event);
    reposition();
}


void hk_kdereportdata::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportdata::reposition");
#endif
    if (p_presentation==NULL ||p_setcoordinates==true) return;
    p_setwidgetcoordinates=true;
    unsigned int ppx=p_presentation->sizetype()==hk_presentation::relative?
    		     (unsigned int)((QWidget::geometry().x()<0?0:(double)QWidget::geometry().x()*teiler)/ p_kdesection->max_displaywidth()+0.5):
		     (unsigned int)((QWidget::geometry().x()<0?0:(double)QWidget::geometry().x()*100/pix_je_cm+0.5));
    unsigned int ppy=p_presentation->sizetype()==hk_presentation::relative?
    		     (unsigned int)((QWidget::geometry().y()<0?0:(double)QWidget::geometry().y())*teiler / p_kdesection->max_displayheight()+0.5):
		     (unsigned int)((QWidget::geometry().y()<0?0:(double)QWidget::geometry().y())*100/pix_je_cm+0.5);
    unsigned int pph=p_presentation->sizetype()==hk_presentation::relative?
    		     (unsigned int)((QWidget::height()<0?0:(double)QWidget::height())*teiler  /p_kdesection->max_displayheight()+0.5):
		     (unsigned int)(QWidget::height()<0?0:(double)QWidget::height()*100/pix_je_cm+0.5);
    unsigned int ppw=p_presentation->sizetype()==hk_presentation::relative?
    		     (unsigned int)(QWidget::width()<0?0:(double)QWidget::width()*teiler  /p_kdesection->max_displaywidth()+0.5):
		     (unsigned int)(QWidget::width()<0?0:(double)QWidget::width()*100/pix_je_cm+0.5);

    set_size(ppx,ppy,ppw,pph);
    p_kdesection->adjust_sectionsize();
    p_setwidgetcoordinates=false;
}


bool hk_kdereportdata::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportdata::widget_specific_coordinates");
#endif
// px,py,pwidth,pheight in zzm
//cout <<"py: "<<(py*p_kdesection->max_displayheight()+0.5)/report()->designheight()<<" ph: "<<(pheight*p_kdesection->max_displayheight()+0.5)/report()->designheight()<<" sum= "<<py+pheight<<" top: "<<report()->relativ2vertical(report()->border_top())*32/100<<endl;
//cout <<"pw: "<<(px*p_kdesection->max_displaywidth())/report()->designwidth()+1<<" ph: "<<(pwidth*p_kdesection->max_displaywidth())/report()->designwidth()+1<<" sum= "<<py+pheight<<endl;
if (report()->sizetype()==hk_presentation::relative)
{
    if ( (p_kdesection == report()->page_header() &&
    (double)report()->relativ2vertical(report()->border_top()) * pix_je_cm / 100 < ((double)py*p_kdesection->max_displayheight()) / report()->designheight() + (pheight*p_kdesection->max_displayheight()) / report()->designheight())
        || (p_kdesection == report()->page_footer() && (double)report()->relativ2vertical(report()->border_bottom())*pix_je_cm / 100 < (py*p_kdesection->max_displayheight()) / report()->designheight() + (pheight*p_kdesection->max_displayheight()) / report()->designheight())
        )
    {
// cout <<"relativ Keine größenanpassung!"<<endl;
        return false;
    }
}
else
{
    if ( (p_kdesection == report()->page_header() && (double)report()->border_top() * pix_je_cm / 100 < ((double)py*p_kdesection->max_displayheight()) / report()->designheight() + (pheight*p_kdesection->max_displayheight()) / report()->designheight())
        || (p_kdesection==report()->page_footer() && (double)report()->border_bottom() * pix_je_cm / 100 < (py*p_kdesection->max_displayheight()) / report()->designheight() + (pheight*p_kdesection->max_displayheight()) / report()->designheight())
        )
    {
//  cout <<"absolut: Keine größenanpassung!"<<endl;
        return false;
    }
}
unsigned int dw=report()->designwidth();
unsigned int dh=report()->designheight();
    if (report()->sizetype()==hk_presentation::absolute)
    {
      if (report()->page_header()==p_kdesection || report()->page_footer()==p_kdesection)
      {
     dh=(unsigned int)(p_kdesection->max_displayheight()*100/pix_je_cm);
//      cerr <<"dh kalkuliert:"<<dh<<endl;
      }
      else
           dh=dh-report()->border_top()-report()->border_bottom();

     dw=dw-report()->border_left()-report()->border_right();
    }
    if (report()->mode()==hk_report::designmode)
    { 
    setGeometry(
    (int)    (((double)px*p_kdesection->max_displaywidth())/dw+0.5),
    (int)    (((double)py*p_kdesection->max_displayheight())/dh+0.5),
    (int)    (((double)pwidth*p_kdesection->max_displaywidth())/dw+0.5),
    (int)    (((double)pheight*p_kdesection->max_displayheight())/dh+0.5));
    qApp->processEvents();
    p_kdesection->adjust_sectionsize();
    }
    return true;
}


void    hk_kdereportdata::widget_specific_backgroundcolour_changed(const hk_colour&)
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(backgroundRole(),newcolour);
    setPalette(p);
}


void    hk_kdereportdata::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(foregroundRole(),newcolour);
    setPalette(p);    
}
