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
//$Revision: 1.34 $

#include "hk_kdereportsection.h"
#include "hk_kdesimplereport.h"
#include "hk_kdereportdata.h"
#include "hk_kdeformfocus.h"
#include <qwidget.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qframe.h>
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qevent.h>

const int MINSECTIONSIZE=20;

class headerclass:public QLabel
{
    public:
        headerclass(hk_kdesimplereport* r,hk_kdereportsection* w):QLabel(r)
        {
          p_section=w;
          setBackgroundRole(QPalette::Mid);
	  setAutoFillBackground(true);
        }

    protected:

        virtual  void mousePressEvent(QMouseEvent*)
        {
//cout <<"mousepressevent"<<endl;
            p_section->kdereport()->set_focus(p_section,p_section,false);
        }

    private:
        hk_kdereportsection* p_section;

};


hk_kdereportsection::hk_kdereportsection (hk_kdesimplereport*r,  const char* name, Qt::WFlags fl )
: QWidget( r, fl ) ,hk_reportsection(r)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportsection::hk_kdereportsection");
#endif

    p_kdereport=r;
    setObjectName(QString::fromAscii(name));
    p_header = new  headerclass(r,this);
    QSizePolicy fixedpolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
    p_header->setSizePolicy(fixedpolicy);
    QFont f=p_header->font();
    QFontInfo info(f);

    p_header->setFixedHeight(info.pixelSize()+4);
    p_header->setFrameShape(QFrame::Panel);
    p_header->setFrameShadow(QFrame::Raised);
    setSizePolicy( fixedpolicy );
    setAutoFillBackground(true);

    setFixedHeight(MINSECTIONSIZE);
    resize_section();
    connect( this, SIGNAL( field_created() ), p_kdereport, SLOT( field_created() ) );
    p_displaystring = "Test";
    p_is_active_section=false;
    pix_je_cm=(QApplication::desktop()->screenGeometry(r).width()/1024.0)*32.0  ;
}


hk_kdereportsection::~hk_kdereportsection(void)
{
    delete p_header;
}


void hk_kdereportsection::resize_section(void)
{
    pix_je_cm=(QApplication::desktop()->screenGeometry(this).width()/1024.0)*32.0  ;
    if (p_report->sizetype()==hk_presentation::relative)
    {
     p_maxdisplaywidth=(unsigned int)((double)(p_report->designwidth()-p_report->relativ2horizontal(p_report->border_left()+p_report->border_right()))*pix_je_cm/100+0.5);
     p_maxdisplayheight=(unsigned int) ((double)(p_report->designheight()-p_report->relativ2vertical(p_report->border_top()+p_report->border_bottom()))*pix_je_cm /100+0.5);
    }
    else
    {
    if (p_kdereport->page_header()==this)
    p_maxdisplayheight=(unsigned int)(p_report->border_top()*pix_je_cm /100+0.5);
    else
    if (p_kdereport->page_footer()==this)
    p_maxdisplayheight=(unsigned int)(p_report->border_bottom()*pix_je_cm /100+0.5);
     
    else
    p_maxdisplayheight=(unsigned int) ((double)(p_report->designheight()-p_report->border_top()-p_report->border_bottom())*pix_je_cm /100+0.5);

     p_maxdisplaywidth=(unsigned int) (((double)p_report->designwidth()-p_report->border_left()-p_report->border_right())*pix_je_cm/100+0.5);
    
    }
    setFixedWidth(p_maxdisplaywidth);

    vector<hk_reportdata*>::iterator it=datalist()->begin();
    while (it!=datalist()->end())
    {
        (*it)->set_size((*it)->hk_reportdata::x(),(*it)->hk_reportdata::y(),(*it)->hk_reportdata::width(),(*it)->hk_reportdata::height(),false);

        it++;
    }
    adjust_sectionsize();
}


hk_reportdata* hk_kdereportsection::widget_specific_new_data(void)
{
    hk_kdereportdata* d=new hk_kdereportdata(this);
    d->set_height(3380);
    d->show();
    return d;
}


void hk_kdereportsection::paintEvent(QPaintEvent* e)
{
    QPainter p(this);
    p.setClipRegion( e->rect() );
    p.setPen(palette().color(foregroundRole()));
    for (int a=0; a < QWidget::width()/10+20; a++ )
    {
        for (int b=0; b<QWidget::height()/10+20;b++ )
            p.drawPoint(a*10,b*10);
    }
    p.drawRect(0,0,QWidget::width(),QWidget::height());
}


void    hk_kdereportsection::mousePressEvent(QMouseEvent* m)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportsection::mousePressEvent");
#endif
    if (p_report->mode()==hk_presentation::designmode)
    {
        switch (m->button())
        {
	  case Qt::LeftButton:
            {
//emit mouseclick(m->pos());
                if (p_kdereport->field2create())
                {
                    create_field(m->pos());
                }
                else
                {
                    p_kdereport->set_focus2property();
                }
                break;
            }

	  case Qt::RightButton:
            {
//mousemenu(m);
                break;
            }
            default:      ;
        }
    }
    QWidget::mousePressEvent(m);
}


void hk_kdereportsection::create_field(const QPoint& position)
{
    hk_reportdata* newfield =new_data();
    QPoint point=position;
    if ((p_kdereport->snap2gridx()>0)&&(point.x()%p_kdereport->snap2gridx()>0))
         point.setX((point.x()/p_kdereport->snap2gridx())*p_kdereport->snap2gridx());
    if ((p_kdereport->snap2gridy()>0)&&(point.y()%p_kdereport->snap2gridy()>0))
         point.setY((point.y()/p_kdereport->snap2gridy())*p_kdereport->snap2gridy());


    if (newfield!=NULL)
    {
        if (p_kdereport->sizetype()==hk_presentation::relative)
	{
	newfield->set_size((unsigned int)(point.x()*10000.0/max_displaywidth()+0.5),(unsigned int)(point.y()*10000.0/max_displayheight()),
            (unsigned int)(400.0*10000/p_report->designwidth()+0.5),
            (unsigned int)(100.0*10000/p_report->designheight()+0.5));
        }
	else
	{
  point.setX( (int)(hk_presentation::relative?
((point.x()<0?0:(double)point.x()*10000)/max_displaywidth()+0.5):
((point.x()<0?0:(double)point.x()*100/pix_je_cm+0.5)))
);

  point.setY( (int)(hk_presentation::relative?
((point.y()<0?0:(double)point.y()*10000)/max_displayheight()+0.5):
((point.y()<0?0:(double)point.y()*100/pix_je_cm+0.5)))
);


	newfield->set_size((unsigned int)point.x(),(unsigned int)point.y(),300,75);
	}
	p_kdereport->set_focus(dynamic_cast <QWidget*>(newfield),this,false);
    }
    p_kdereport->focus()->set_positions();
    emit field_created();
    p_kdereport->set_field2create(false);

}


hk_kdeformfocus* hk_kdereportsection::new_focus(void)
{
    hk_kdeformfocus* f=new hk_kdeformfocus(this);
    return f;

}


unsigned int hk_kdereportsection::max_displaywidth(void)
{
    return p_maxdisplaywidth;
}


unsigned int hk_kdereportsection::max_displayheight(void)
{
    return p_maxdisplayheight;

}


void hk_kdereportsection::set_displaystring(const QString&d)
{
    p_displaystring=d;
    p_header->setText(p_displaystring+": "+QString::fromUtf8(l2u(columnname()).c_str()));
    repaint();
}


QString hk_kdereportsection::displaystring(void)
{
    return p_displaystring;
}


void hk_kdereportsection::widget_specific_after_loaddata(void)
{
    adjust_sectionsize();
}


void hk_kdereportsection::adjust_sectionsize(void)
{
//cerr <<"adjus_sectionsize: ";
//if (report()->mode()==hk_report::viewmode) return;
    vector<hk_reportdata*>::iterator it=p_data.begin();
    int m=0;
    while (it!=p_data.end())
    {
        hk_kdereportdata* p=dynamic_cast<hk_kdereportdata*>((*it));
        if (p!=NULL)
        {
            int b=p->QWidget::y()+p->QWidget::height();
            if (b>m)m=b;
        }
        it++;
    }
    //cerr <<m;
      
    
     if (report()->sizetype()==hk_presentation::relative)
          m+=(int)((double)relativ2vertical(offset())*max_displayheight()/report()->designheight()+0.5);
     else
          m+=(int)((double)offset()*max_displayheight()/report()->designheight()+0.5);
    if (m<MINSECTIONSIZE)
      m=MINSECTIONSIZE;
    
    setFixedHeight(m);
}


QWidget* hk_kdereportsection::header()
{
    return p_header;
}


void hk_kdereportsection::set_activesection(bool active)
{
    p_is_active_section=active;
    p_header->setBackgroundRole(p_is_active_section?QPalette::Highlight:QPalette::Mid);
    p_header->setFrameShadow(p_is_active_section?QFrame::Sunken:QFrame::Raised);
}


void hk_kdereportsection::widget_specific_offset_changed(void)
{
    adjust_sectionsize();
}
