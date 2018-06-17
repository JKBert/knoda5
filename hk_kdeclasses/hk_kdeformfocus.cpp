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

#include "hk_kdeformfocus.h"
#include <qpainter.h>
#include <qdesktopwidget.h>
#include "hk_kdesimpleform.h"
#include "hk_kdesimplereport.h"
#include "hk_kdereportsection.h"
#include <hk_report.h>
#include <qevent.h>
#include <qapplication.h>
const int markersize=6;

hk_marker::hk_marker(hk_kdesimpleform* w,hk_kdeformfocus* f,enum_markertype type):QWidget(w),hk_class()
{
    setFixedSize( markersize, markersize );
    p_widget=NULL;
    p_type=type;
    p_focus=f;
    setMouseTracking(false);
    p_form=w;
    p_report=NULL;
    p_section=NULL;
//    wanna_debug(true);

}


hk_marker::hk_marker(hk_kdereportsection* w,hk_kdeformfocus* f,enum_markertype type):QWidget(w),hk_class()
{
    setFixedSize( markersize, markersize );
    p_widget=NULL;
    p_type=type;
    p_focus=f;
    setMouseTracking(false);
    p_report=NULL;
    if (w!=NULL)p_report=w->kdereport();
    p_section=w;
    p_form=NULL;
//    wanna_debug(true);

}


hk_marker::~hk_marker()
{

}


void hk_marker::paintEvent( QPaintEvent * )
{
    QPainter p( this );
    p.fillRect(0,0,width(),height(),Qt::white);
    p.setPen(Qt::blue);
    if (p_form!=NULL)
        if (p_form->formfocus()==p_focus)  p.setPen(Qt::red);
    if (p_section!=NULL) {
      if (p_section->kdereport()->formfocus()==p_focus)  p.setPen(Qt::red);
    }

    p.drawRect( 0, 0, width()-1, height()-1 );
}


void hk_marker::set_widget(QWidget* wid)
{
    p_widget=wid;
    if (wid==NULL) hide();
    else
    {
        show();
    }
}


hk_marker::enum_markertype hk_marker::type(void)
{
    return p_type;
}


void hk_marker::set_cursors(void)
{
    switch (p_type)
    {
      case lo    :       setCursor(Qt::SizeFDiagCursor );
        break;
      case lm    :       setCursor(Qt::SizeHorCursor);
        break;
      case lu    :       setCursor(Qt::SizeBDiagCursor);
        break;
      case mo    :       setCursor(Qt::SizeVerCursor);
        break;
      case mu    :       setCursor(Qt::SizeVerCursor);
        break;
      case ro    :       setCursor(Qt::SizeBDiagCursor);
        break;
      case rm    :       setCursor(Qt::SizeHorCursor);
        break;
      case ru    :       setCursor(Qt::SizeFDiagCursor);
        break;

    }

}


void hk_marker::mousePressEvent( QMouseEvent* event )
{
#ifdef HK_DEBUG
    hkdebug("hk_marker::MousePressEvent");
#endif
    if (p_widget==NULL)return;
    if (event->button()==Qt::LeftButton)
    {
        p_originalposition=event->pos();
    }
}


void hk_marker::mouseMoveEvent( QMouseEvent* event )
{
#ifdef HK_DEBUG
    hkdebug("hk_marker::MouseMoveEvent");
#endif
    if (p_widget==NULL) return;

    if (( event->buttons() & Qt::LeftButton ) == Qt::LeftButton )
    {
        p_focus->p_markermove=true;
        int minheight=10;
        int minwidth=10;
        unsigned int maxright=10000;
        unsigned int maxdown=10000;
        if (p_form!=NULL)
        {
            maxright=p_form->designwidth();
            maxdown=p_form->designheight();
            minheight=1;                          //(int) (p_form->designheight()*0.01);
            minwidth=1;                           //(int) (p_form->designwidth()*0.01);
        }
        else
        if (p_section!=NULL)
        {  
            maxright=p_section->max_displaywidth();
            maxdown=p_section->max_displayheight();
            if (p_report->sizetype()==hk_report::relative)
	    {
	     if (p_section==p_report->page_header())
             {
                maxdown=(signed)p_report->relativ2vertical(p_report->border_top())*(unsigned int)p_focus->pix_je_cm/100;
             }
             else
             if (p_section==p_report->page_footer())
             {
                maxdown=(signed)p_report->relativ2vertical(p_report->border_bottom())*(unsigned int)p_focus->pix_je_cm/100;
             }
            } 
            minheight=(int) (p_section->max_displayheight()*0.007);
            minwidth=(int) (p_section->max_displaywidth()*0.007); 
        }
        QPoint newpos=mapFromGlobal(event->globalPos());
        QPoint difference=p_originalposition-newpos;
        p_originalposition=newpos;
        switch (p_type)
        {
            case lo    :
                if ( p_widget->x()-difference.x()<0) difference.setX(p_widget->x());
                if ( p_widget->y()-difference.y()<0) difference.setY(p_widget->y());
                if ( p_widget->height()+difference.y()<minheight) difference.setY(minheight-p_widget->height());
                if (p_widget->width()+difference.x()<minwidth) difference.setX(p_widget->width()-minwidth);
                p_widget->setGeometry(p_widget->x()-difference.x(),p_widget->y()-difference.y(),p_widget->width()+difference.x(),p_widget->height()+difference.y());
                break;
            case lm    :
                if ( p_widget->x()-difference.x()<0) difference.setX(p_widget->x());
                if (p_widget->width()+difference.x()<minwidth) difference.setX(p_widget->width()-minwidth);
                p_widget->setGeometry(p_widget->x()-difference.x(),p_widget->y(),p_widget->width()+difference.x(),p_widget->height());
                break;
            case lu    :
                if ( p_widget->x()-difference.x()<0) difference.setX(p_widget->x());
                if ( p_widget->height()-difference.y()<minheight) difference.setY(minheight-p_widget->height());
                if (p_widget->width()+difference.x()<minwidth) difference.setX(p_widget->width()-minwidth);
                if ( p_widget->y()+p_widget->height()-difference.y()>(int) maxdown) difference.setY(-(maxdown-p_widget->y()-p_widget->height()));
                p_widget->setGeometry(p_widget->x()-difference.x(),p_widget->y(),p_widget->width()+difference.x(),p_widget->height()-difference.y());
                break;
            case mo    :
                if ( p_widget->y()-difference.y()<0)
                {
                    difference.setY(p_widget->y());
                }
                if ( p_widget->height()+difference.y()<minheight) difference.setY(minheight-p_widget->height());

                p_widget->setGeometry(p_widget->x(),p_widget->y()-difference.y(),p_widget->width(),p_widget->height()+difference.y());
                break;
            case mu    :
                if ( p_widget->height()-difference.y()<minheight) difference.setY(p_widget->height()-minheight);
                if ( p_widget->y()+p_widget->height()-difference.y()>(int) maxdown) difference.setY(-(maxdown-p_widget->y()-p_widget->height()));
                p_widget->setGeometry(p_widget->x(),p_widget->y(),p_widget->width(),p_widget->height()-difference.y());

                break;
            case ro    :
                if ( p_widget->y()-difference.y()<0) difference.setY(p_widget->y());
                if ( p_widget->height()+difference.y()<minheight) difference.setY(minheight-p_widget->height());
                if ( p_widget->width()-difference.x()<minwidth) difference.setX(p_widget->width()-minwidth);
                if ( p_widget->x()+p_widget->width()-difference.x()>(int) maxright) difference.setX(-(maxright-p_widget->x()-p_widget->width()));
                p_widget->setGeometry(p_widget->x(),p_widget->y()-difference.y(),p_widget->width()-difference.x(),p_widget->height()+difference.y());
                break;
            case rm    :
                if ( p_widget->width()-difference.x()<minwidth) difference.setX(p_widget->width()-minwidth);
                if ( p_widget->x()+p_widget->width()-difference.x()>(int) maxright) difference.setX(-(maxright-p_widget->x()-p_widget->width()));
                p_widget->setGeometry(p_widget->x(),p_widget->y(),p_widget->width()-difference.x(),p_widget->height());
                break;
            case ru    :
                if ( p_widget->height()-difference.y()<minheight) difference.setY(p_widget->height()-minheight);
                if ( p_widget->width()-difference.x()<minwidth) difference.setX(p_widget->width()-minwidth);
                if ( p_widget->x()+p_widget->width()-difference.x()>(int) maxright) difference.setX(-(maxright-p_widget->x()-p_widget->width()));
                if ( p_widget->y()+p_widget->height()-difference.y()>(int) maxdown) difference.setY(-(maxdown-p_widget->y()-p_widget->height()));
                p_widget->setGeometry(p_widget->x(),p_widget->y(),p_widget->width()-difference.x(),p_widget->height()-difference.y());
                break;

        }
        QPoint p=pos();
        p_focus->set_positions();
        p_originalposition +=(p-pos());
        p_focus->marker_moved();
        p_focus->p_markermove=false;
    }
    if (p_section!=NULL)
    {
        int b=p_widget->y()+p_widget->height();

/*      if (p_section==p_report->page_head()||p_section==p_report->page_foot())
      {
       if (p_section==p_report->page_head()&&p_section->QWidget::height()<b&&(unsigned int)b<p_report->border_top()*32/100)p_section->setFixedHeight(b);
       else
       if (p_section==p_report->page_foot()&&p_section->QWidget::height()<b&&(unsigned int)b<p_report->border_bottom()*32/100)p_section->setFixedHeight(b);
      }
      else
         */
      if (p_section->QWidget::height()<b&&(unsigned int)b<p_section->max_displayheight())
        {
            p_section->setFixedHeight(b);cout <<"mousemoveevent setFixedHeight"<<endl;
        }
    }

}


//*********************************************
//***             hk_kdeformfocus           ***
//*********************************************

hk_kdeformfocus::hk_kdeformfocus(hk_kdesimpleform* wid) :QObject(),hk_class()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformfocus::hk_kdeformfocus");
//wanna_debug(true);
#endif

    p_markermove=false;
    p_widget=NULL;
    p_section=NULL;
    hk_marker* marker= new hk_marker(wid,this,hk_marker::lo);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::lm);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::lu);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::mo);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::mu);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::ro);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::rm);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::ru);

    p_markers.insert(p_markers.end(),marker);
   pix_je_cm=(QApplication::desktop()->screenGeometry(marker).width()/1024.0)*32.0;
}


hk_kdeformfocus::hk_kdeformfocus(hk_kdereportsection* wid) :QObject(),hk_class()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformfocus::hk_kdeformfocus");
//wanna_debug(true);
#endif
    p_markermove=false;
    p_widget=NULL;
    p_section=wid;
    hk_marker* marker= new hk_marker(wid,this,hk_marker::lo);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::lm);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::lu);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::mo);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::mu);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::ro);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::rm);

    p_markers.insert(p_markers.end(),marker);
    marker= new hk_marker(wid,this,hk_marker::ru);

    p_markers.insert(p_markers.end(),marker);
   pix_je_cm=(QApplication::desktop()->screenGeometry(marker).width()/1024.0)*32.0;
}


hk_kdeformfocus::~hk_kdeformfocus(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformfocus::~hk_kdeformfocus");
#endif
    list<hk_marker*>::iterator it=p_markers.begin();
    while (it!=p_markers.end())
    {
        hk_marker* m=(*it);
        it++;
        delete m;
    }

}


void hk_kdeformfocus::set_positions(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformfocus::set_positions");
#endif
    if (p_widget==NULL) return;

    list<hk_marker*>::iterator it=p_markers.begin();
    while (it!=p_markers.end())
    {
        hk_marker* m=(*it);
        it++;
        hk_marker::enum_markertype t=m->type();
        switch (t)
        {
            case hk_marker::lo:;
            m->move(p_widget->x()-markersize+markersize/2,p_widget->y()-markersize /2);
            break;
            case hk_marker::lm:;
            m->move(p_widget->x()-markersize+markersize/2,p_widget->y()+p_widget->height()/2-markersize /2);
            break;
            case hk_marker::lu:;
            m->move(p_widget->x()-markersize+markersize/2,p_widget->y()+p_widget->height()-markersize /2 );
            break;
            case hk_marker::mo:;
            m->move(p_widget->x()+p_widget->width()/2-markersize/2,p_widget->y()-markersize /2);
            break;
            case hk_marker::mu:;
            m->move(p_widget->x()+p_widget->width()/2-markersize/2,p_widget->y()+p_widget->height()-markersize /2);
            break;
            case hk_marker::ro:;
            m->move(p_widget->x()+p_widget->width()-markersize/2,p_widget->y()-markersize /2);
            break;
            case hk_marker::rm:;
            m->move(p_widget->x()+p_widget->width()-markersize/2,p_widget->y()+p_widget->height()/2-markersize /2);
            break;
            case hk_marker::ru:;
            m->move(p_widget->x()+p_widget->width()-markersize/2,p_widget->y()+p_widget->height()-markersize /2 );

        };

    }
    if (p_section!=NULL)
    {
        int b=p_widget->y()+p_widget->height();
	if (p_section->QWidget::height()<b&&(unsigned int)b<p_section->max_displayheight())
        {
            p_section->setFixedHeight(b);
//cout <<"gr�e anpassen "<<b<<" max "<<p_section->max_displayheight()<<endl;
        }
    }

}


void hk_kdeformfocus::set_widget(QWidget* wid)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformfocus::set_widget");
#endif
    if (p_widget!=NULL) p_widget->removeEventFilter(this);
    p_widget=wid;
    list<hk_marker*>::iterator it=p_markers.begin();
    while (it!=p_markers.end())
    {
        hk_marker* m=(*it);
        it++;
        m->set_widget(wid);
    }
    if(p_widget!=NULL) p_widget->installEventFilter(this);

    set_positions();
    show() ;
}


void hk_kdeformfocus::show(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformfocus::show");
#endif
    if (p_widget==NULL)
    {
        hide();
        return;

    }
    list<hk_marker*>::iterator it=p_markers.begin();
    while (it!=p_markers.end())
    {
        hk_marker* m=(*it);
        it++;
        m->set_cursors();
        m->show();
        m->raise();
    }

}


void hk_kdeformfocus::hide(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformfocus::hide");
#endif
    list<hk_marker*>::iterator it=p_markers.begin();
    while (it!=p_markers.end())
    {
        hk_marker* m=(*it);
        it++;
        m->hide();
    }

}


QWidget* hk_kdeformfocus::widget(void)
{
    return p_widget;
}


void hk_kdeformfocus::repaint(void)
{
    list<hk_marker*>::iterator it=p_markers.begin();
    while (it!=p_markers.end())
    {
        hk_marker* m=(*it);
        it++;
        m->repaint();
    }

}


bool hk_kdeformfocus::eventFilter(QObject* object,QEvent* event)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformfocus::eventFilter");
#endif

    if (object==p_widget
        &&(event->type()==QEvent::Resize||event->type()==QEvent::Move )
        &&!p_markermove)   set_positions();

    return QObject::eventFilter(object,event);
}


void hk_kdeformfocus::marker_moved(void)
{
    emit size_changed();
}
