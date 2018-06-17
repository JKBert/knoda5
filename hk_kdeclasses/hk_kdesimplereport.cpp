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

#include "hk_kdesimplereport.h"
#include "hk_kdereportsection.h"
#include "hk_kdereportproperty.h"
#include "hk_kdereport.h"
#include "../hk_kdereportpart/hk_kdereportpart.h"
#include "hk_kdereportdata.h"
#include "hk_kdeformfocus.h"
#include <hk_reportsectionpair.h>
#include <qlayout.h>
#include <hk_kdeformfocus.h>
#include <hk_datasource.h>
#include <qapplication.h>
#include <qprogressdialog.h>
#include <klocale.h>
#include <kaction.h>
#include <kactionmenu.h>
#include <kactioncollection.h>
#include <kmenu.h>
#include <qdockwidget.h>
#include <qclipboard.h>
#include <qcolor.h>
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qevent.h>
#include <QMimeData>

const int headerheight=20;
class hk_kdesimplereportprivate
{
   public:
   hk_kdesimplereportprivate():while_modechange(false), p_propertydock(NULL)
       {
       }
	static const QString mimeCopyFormat;
        static const QByteArray startTagDrag;
        static const QByteArray endTagDrag;
        bool while_modechange;
        hk_kdereportproperty* p_property;
        QVBoxLayout* p_reportlayout;
        hk_kdereport* p_kdereport;
	hk_kdereportpartwidget* p_reportpartwidget;
        hk_kdeformfocus* p_focus;
        hk_kdereportsection* p_focussection;
        list<hk_kdeformfocus*> p_multiplefocus;
        QPoint p_originalposition;
        //bool p_controlbutton;
        bool p_autoclose;
        bool p_sourcevanishflag;
        bool p_createfield;
        bool p_cancelprinting;
        QProgressDialog* p_progressdialog;
        QMenu* p_popup;
        KAction* p_deleteaction;
        KAction* p_copyaction;
        KAction* p_pasteaction;
        KAction* p_cutaction;

        KActionMenu* p_resizeaction;
        KAction* p_minwidthaction;
        KAction* p_maxwidthaction;
        KAction* p_minheightaction;
        KAction* p_maxheightaction;
        KAction* p_minsizeaction;
        KAction* p_maxsizeaction;
        KActionMenu*p_alignaction;
        KAction* p_alignleftaction;
        KAction* p_alignrightaction;
        KAction* p_aligntopaction;
        KAction* p_alignbottomaction;
	QDockWidget* p_propertydock;
        double pix_je_cm;
};

const QString hk_kdesimplereportprivate::mimeCopyFormat = QString::fromAscii("application/x-hk_kdesimplereportcopy");
const QByteArray hk_kdesimplereportprivate::startTagDrag = "<DRAG>";
const QByteArray hk_kdesimplereportprivate::endTagDrag = "</DRAG>";

hk_kdesimplereport::hk_kdesimplereport (QWidget* parent,  const char* name, Qt::WFlags fl )
: QWidget( parent, fl ) ,hk_report()
{
#ifdef HK_DEBUG
    //wanna_debug(true);
    hkclassname("hk_kdesimplereport");
    hkdebug("hk_kdesimplereport::hk_kdesimplereport");
#endif
    setObjectName(QString::fromAscii(name));
    p_private=new hk_kdesimplereportprivate;
    p_private->while_modechange=false;
    p_private->p_popup=NULL;
    p_private->p_deleteaction=NULL;
    p_private->p_copyaction=NULL;
    p_private->p_pasteaction=NULL;
    p_private->p_cutaction=NULL;
    p_private->p_resizeaction=NULL;
    p_private->p_minwidthaction=NULL;
    p_private->p_maxwidthaction=NULL;
    p_private->p_minheightaction=NULL;
    p_private->p_maxheightaction=NULL;
    p_private->p_minsizeaction=NULL;
    p_private->p_maxsizeaction=NULL;
    p_private->p_alignaction=NULL;
    p_private->p_alignleftaction=NULL;
    p_private->p_alignrightaction=NULL;
    p_private->p_aligntopaction=NULL;
    p_private->p_alignbottomaction=NULL;
    setGeometry(0,0,800,600);
    p_private->p_sourcevanishflag=false;
    p_private->p_focus = NULL;
    p_private->p_property=NULL;
    p_private->p_focussection=NULL;
    p_private->p_progressdialog=NULL;
    p_private->pix_je_cm=(QApplication::desktop()->screenGeometry(this).width()/1024.0)*32.0 ;

    set_reporttype("Postscript",false);
    //create_propertyeditor();
    //p_private->p_property->show();
    //set_propertyobject(this);
    p_private->p_reportlayout=NULL;
    p_private->p_autoclose=true;
    p_private->p_createfield=false;
    p_private->p_kdereport=NULL;
    p_private->p_reportpartwidget=NULL;
    init_report();
    set_sections();
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    reset_has_changed();
    connect (QApplication::clipboard(), SIGNAL(dataChanged()),this, SLOT(enable_actions()));
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p = palette();
    p.setColor(foregroundRole(),newcolour);
    c=backgroundcolour();
    newcolour.setRgb(c.red(),c.green(),c.blue());    
    p.setColor(backgroundRole(),newcolour);
    setAutoFillBackground(true);
    setPalette(p);  
}


hk_kdesimplereport::~hk_kdesimplereport(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::~hk_kdesimplereport");
#endif
    delete p_private->p_deleteaction;
    delete p_private->p_copyaction;
    delete p_private->p_pasteaction;
    delete p_private->p_cutaction;

    delete p_private;
    p_private=NULL;
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::~hk_kdesimplereport END");
#endif

}

void hk_kdesimplereport::set_reportpartwidget(hk_kdereportpartwidget* w)
{
  p_private->p_reportpartwidget=w;
  if (p_private->p_property)
    {
       delete p_private->p_property;
       p_private->p_property=NULL;
    }

  show_property();
}



hk_kdereportpartwidget* hk_kdesimplereport::reportpartwidget() const
{
return p_private->p_reportpartwidget;

}



void hk_kdesimplereport::set_kdereport(hk_kdereport* k)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::set_kdereport");
#endif
    p_private->p_kdereport=k;
    if (p_private->p_kdereport)
    {
    if (p_private->p_property)
      {
       delete p_private->p_property;
       p_private->p_property=NULL;
      }

      show_property();
      p_private->p_deleteaction=new KAction(i18n("&Delete"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("deleteclicked",p_private->p_deleteaction);
      p_private->p_deleteaction->setShortcut(Qt::Key_Delete);
      connect(p_private->p_deleteaction,SIGNAL(triggered()),this,SLOT(delete_widgets()));
      p_private->p_copyaction= new KAction(i18n("&Copy"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("copy",p_private->p_copyaction);
      p_private->p_copyaction->setShortcut(Qt::CTRL+Qt::Key_C);
      connect(p_private->p_copyaction,SIGNAL(triggered()),this,SLOT(copy_widgets()));      
      p_private->p_pasteaction=new KAction(i18n("&Paste"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("pasteclicked",p_private->p_pasteaction);
      p_private->p_pasteaction->setShortcut(Qt::CTRL+Qt::Key_V);
      connect(p_private->p_pasteaction,SIGNAL(triggered()),this,SLOT(paste_widgets()));      
      p_private->p_cutaction=new KAction(i18n("Cu&t"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("cutclicked",p_private->p_cutaction);
      p_private->p_cutaction->setShortcut(Qt::CTRL+Qt::Key_X);
      connect(p_private->p_cutaction,SIGNAL(triggered()),this,SLOT(cut_widgets()));      
      p_private->p_resizeaction=new KActionMenu(i18n("Adjust &size"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("size",p_private->p_resizeaction);
      p_private->p_minwidthaction=new KAction(i18n("Minimum width"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("minwidth",p_private->p_minwidthaction);
      connect(p_private->p_minwidthaction,SIGNAL(triggered()),this,SLOT(adjust_minw()));
      p_private->p_maxwidthaction=new KAction(i18n("Maximum width"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("maxwidth",p_private->p_maxwidthaction);
      connect(p_private->p_maxwidthaction,SIGNAL(triggered()),this,SLOT(adjust_maxw()));
      p_private->p_minheightaction=new KAction(i18n("Minimum height"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("minheight",p_private->p_minheightaction);
      connect(p_private->p_minheightaction,SIGNAL(triggered()),this,SLOT(adjust_minh()));      
      p_private->p_maxheightaction=new KAction(i18n("Maximum height"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("maxheight",p_private->p_maxheightaction);
      connect(p_private->p_maxheightaction,SIGNAL(triggered()),this,SLOT(adjust_maxh()));      
      p_private->p_minsizeaction=new KAction(i18n("Minimum size"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("minsize",p_private->p_minsizeaction);
      connect(p_private->p_minsizeaction,SIGNAL(triggered()),this,SLOT(adjust_mins()));
      p_private->p_maxsizeaction=new KAction(i18n("Maximum size"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()-> addAction("maxsize",p_private->p_maxsizeaction);
      connect(p_private->p_maxsizeaction,SIGNAL(triggered()),this,SLOT(adjust_maxs()));
      p_private->p_resizeaction->addAction(p_private->p_minwidthaction);
      p_private->p_resizeaction->addAction(p_private->p_maxwidthaction);
      p_private->p_resizeaction->addAction(p_private->p_minheightaction);
      p_private->p_resizeaction->addAction(p_private->p_maxheightaction);
      p_private->p_resizeaction->addAction(p_private->p_minsizeaction);
      p_private->p_resizeaction->addAction(p_private->p_maxsizeaction);

      p_private->p_alignaction=new KActionMenu(i18n("&Align"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("align",p_private->p_alignaction);
      p_private->p_alignleftaction=new KAction(i18n("&Left"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("alignleft",p_private->p_alignleftaction);
      connect(p_private->p_alignleftaction,SIGNAL(triggered()),this,SLOT(align_left()));
      p_private->p_alignrightaction=new KAction(i18n("&Right"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("alignright",p_private->p_alignrightaction);
      connect(p_private->p_alignrightaction,SIGNAL(triggered()),this,SLOT(align_right()));
      p_private->p_aligntopaction=new KAction(i18n("&Top"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("aligntop",p_private->p_aligntopaction);
      connect(p_private->p_aligntopaction,SIGNAL(triggered()),this,SLOT(align_top()));
      p_private->p_alignbottomaction=new KAction(i18n("&Bottom"),p_private->p_kdereport->actionCollection());
      p_private->p_kdereport->actionCollection()->addAction("alignbottom",p_private->p_alignbottomaction);
      connect(p_private->p_alignbottomaction,SIGNAL(triggered()),this,SLOT(align_bottom()));
      p_private->p_alignaction->addAction(p_private->p_alignleftaction);
      p_private->p_alignaction->addAction(p_private->p_alignrightaction);
      p_private->p_alignaction->addAction(p_private->p_aligntopaction);
      p_private->p_alignaction->addAction(p_private->p_alignbottomaction);
      enable_actions();
    }
    else
    {
    delete p_private->p_deleteaction;
    delete p_private->p_copyaction;
    delete p_private->p_pasteaction;
    delete p_private->p_cutaction;
    delete p_private->p_resizeaction;
    delete p_private->p_minwidthaction;
    delete p_private->p_maxwidthaction;
    delete p_private->p_minheightaction;
    delete p_private->p_maxheightaction;
    delete p_private->p_minsizeaction;
    delete p_private->p_maxsizeaction;
    delete p_private->p_alignaction;
    delete p_private->p_alignleftaction;
    delete p_private->p_alignrightaction;
    delete p_private->p_aligntopaction;
    delete p_private->p_alignbottomaction;

    p_private->p_deleteaction=NULL;
    p_private->p_copyaction=NULL;
    p_private->p_pasteaction=NULL;
    p_private->p_cutaction=NULL;
    p_private->p_resizeaction=NULL;
    p_private->p_minwidthaction=NULL;
    p_private->p_maxwidthaction=NULL;
    p_private->p_minheightaction=NULL;
    p_private->p_maxheightaction=NULL;
    p_private->p_minsizeaction=NULL;
    p_private->p_maxsizeaction=NULL;
    p_private->p_alignaction=NULL;
    p_private->p_alignleftaction=NULL;
    p_private->p_alignrightaction=NULL;
    p_private->p_aligntopaction=NULL;
    p_private->p_alignbottomaction=NULL;

    }
}


hk_reportsection* hk_kdesimplereport::widget_specific_new_section(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::widget_specific_new_section");
#endif
    hk_reportsection* s=new hk_kdereportsection(this);
    return s;
}

bool hk_kdesimplereport::eventFilter(QObject* object,QEvent* event)
{
#ifdef HK_DEBUG
//hkdebug("hk_kdesimplereport::eventFilter");
#endif
    if (   mode()!=designmode
        ||!object->isWidgetType()
        )
        return QWidget::eventFilter(object,event);
    hk_visible* v = dynamic_cast <hk_visible*>(object);
    hk_visible* p = dynamic_cast <hk_visible*> ( ((QWidget*) object)->parentWidget() );
    if ((v==NULL&& p==NULL)|| dynamic_cast <hk_marker*>(object)!=NULL||!is_reportobject((QWidget*) object))
    {
        return QWidget::eventFilter(object,event);
    }
    return reporteventFilter(object,event);
}


bool hk_kdesimplereport::reporteventFilter(QObject* object,QEvent* event)
{
#ifdef HK_DEBUG
//hkdebug("hk_kdesimpleform::formeventFilter");
#endif



    QWidget* widget= (QWidget*) object;

    switch (event->type())
    {
        case QEvent::KeyPress        :
        {
            keyPressEvent((QKeyEvent*)event);
            break;
        };
        case QEvent::KeyRelease      :
        {
            keyReleaseEvent((QKeyEvent*)event);
            break;
        };
        case QEvent::MouseButtonPress    :
        {

            if (object==this)
            {
                if (((QMouseEvent*)event)->button() !=Qt::RightButton)
                {
                    mousePressEvent((QMouseEvent*) event);

                    return true;
                }
            }
            else
            if (((QMouseEvent*)event)->button() ==Qt::LeftButton)
            {                                     //select object
                                                  // i.e. a menu
                if ( dynamic_cast <QMenu*>(object))
                    return QWidget::eventFilter(object,event);
                if ( dynamic_cast <hk_kdereportsection*>(object))
                    return QWidget::eventFilter(object,event);

                hk_kdereportsection* s=NULL;
                hk_kdereportdata* d=dynamic_cast<hk_kdereportdata*>(object);
                if (d!=NULL) s=d->section();
                set_focus(widget,s,((QMouseEvent*)event)->modifiers() &Qt::ControlModifier);

                if (widget!=NULL&&p_private->p_focus!=NULL)p_private->p_focus->show();
// the following allows the label widget of a hk_kdereportsection to
// activate the section on a mousepressevent
                if (!s && widget) return QWidget::eventFilter(object,event);
                p_private->p_originalposition=mapFromGlobal(((QMouseEvent*)event)->globalPos());

		if (snap2gridx()>0&&!(((QMouseEvent*)event)->modifiers() & Qt::ControlModifier))
		p_private->p_originalposition.setX((p_private->p_originalposition.x()/snap2gridx())*snap2gridx());

		if (snap2gridy()>0&&!(((QMouseEvent*)event)->modifiers() & Qt::ControlModifier))
		p_private->p_originalposition.setY((p_private->p_originalposition.y()/snap2gridy())*snap2gridy());
                return true;
            }
            else
            if (((QMouseEvent*)event)->button() ==Qt::RightButton)
            {
                mousemenu((QMouseEvent*)event);
                return true;
            }
            break;
        };
        case QEvent::MouseMove       :
        {
            if (p_private->p_focus!=NULL)
                if (
                (((QMouseEvent*)event)->buttons() & Qt::LeftButton ) == Qt::LeftButton&&
                widget==p_private->p_focus->widget()
                )
            {
//move object
                QPoint newpos=mapFromGlobal(((QMouseEvent*)event)->globalPos());

		if (snap2gridx()>0&&!(((QMouseEvent*)event)->modifiers() & Qt::ControlModifier))
		newpos.setX((newpos.x()/snap2gridx())*snap2gridx());

		if (snap2gridy()>0&&!(((QMouseEvent*)event)->modifiers() & Qt::ControlModifier))
		newpos.setY((newpos.y()/snap2gridy())*snap2gridy());

                QPoint difference=p_private->p_originalposition-newpos;
		QPoint buf=p_private->p_originalposition;
                p_private->p_originalposition=newpos;
                if ( widget->x()-difference.x()<0)
		    {
		     difference.setX(widget->x());
		      p_private->p_originalposition.setX(buf.x());
		    }
                if ( widget->y()-difference.y()<0)
		    {
		     difference.setY(widget->y());
		      p_private->p_originalposition.setY(buf.y());
		    }
                if (p_private->p_focussection!=NULL)
                {
//cout <<"wx: "<<widget->x()<<" ww: "<<widget->width()<<" md: "<<p_private->p_focussection->max_displaywidth()<<endl;
                    if ( widget->x()+widget->width()-difference.x()>(signed)p_private->p_focussection->max_displaywidth())
                        difference.setX(-(p_private->p_focussection->max_displaywidth()-widget->x()-widget->width()));

                    if (p_private->p_focussection==page_header()||p_private->p_focussection==page_footer())
                    {
                        if (p_private->p_focussection==page_header())
                        {
                            if (sizetype()==hk_presentation::relative)
			    {
			    if ( widget->y()+widget->height()-difference.y()>(signed)relativ2vertical(border_top())*p_private->pix_je_cm/100+0.5)
                                                  //limits size of header section
                                difference.setY((int)-(relativ2vertical(border_top())*p_private->pix_je_cm/100-widget->y()-widget->height()+0.5));
                             }
			   else
			   {
			    if ( widget->y()+widget->height()-difference.y()>(signed)border_top()*p_private->pix_je_cm/100+0.5)
                                                  //limits size of header section
                                difference.setY((int)-(border_top()*p_private->pix_je_cm/100-widget->y()-widget->height()+0.5));

			   }
			}
                        else                      //page_FOOTER
                        {
                            if (sizetype()==hk_presentation::relative)
			    {
                            if ( widget->y()+widget->height()-difference.y()>(signed)relativ2vertical(border_bottom())*p_private->pix_je_cm/100+0.5)
                                                  //limits size of footer section
                                difference.setY((int)-(relativ2vertical(border_bottom())*p_private->pix_je_cm/100-widget->y()-widget->height()+0.5));
                            }
			    else
			    {
                            if ( widget->y()+widget->height()-difference.y()>(signed)border_bottom()*p_private->pix_je_cm/100+0.5)
                                                  //limits size of footer section
                                difference.setY((int)-(border_bottom()*p_private->pix_je_cm/100-widget->y()-widget->height()+0.5));
                            }
			}
                    }
                    else
                    {
                       if ( widget->y()+widget->height()-difference.y()>(signed)p_private->p_focussection->max_displayheight())
                            difference.setY(-(p_private->p_focussection->max_displayheight()-widget->y()-widget->height()));
                    }
                } else cout <<"p_private->p_focussection==NULL!!!"<<endl;
                widget->move(widget->x()-difference.x(),widget->y()-difference.y());
                if (widget->parentWidget()!=NULL)
                {
                    int b=widget->y()+widget->height();
                    if (widget->parentWidget()->height()<b)
                    {
                        if (p_private->p_focussection!=NULL)
                            if (p_private->p_focussection->max_displayheight()<(unsigned int)b)
                                widget->parentWidget()->setFixedHeight(b);cout <<"hk_kdesimplereport setFixedheight"<<endl;
                    }
                }

                p_private->p_focus->set_positions();
                p_private->p_property->focus_resized();
            }
            break;
        };

        case QEvent::MouseButtonDblClick : return true;
        default              :;

    }

    return QWidget::eventFilter(object,event);
}


void hk_kdesimplereport::set_focus(QWidget*f,hk_kdereportsection* s, bool ctrl)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::set_focus");
#endif
    if (f==NULL || f==this ||s==NULL)
    {
        clearfocus();
	emit signal_focuswidget_changed();
        return;

    }

    if (s==f)
    {
//set sectionproperty
        set_actualsection(s);
        clearfocus();
        set_propertyobject(s);
        return;

    }
    set_actualsection(s);
    if (p_private->p_focus==NULL) p_private->p_focus=s->new_focus();
    if (ctrl==true)
    {

        if (p_private->p_focus->widget()==f ) return;
        list<hk_kdeformfocus*>::iterator it=p_private->p_multiplefocus.begin();
        while (it!=p_private->p_multiplefocus.end())         //check if widget is already set
        {
            hk_kdeformfocus* w=(*it);
            it++;
            if (w->widget()==f) return;
        }

    }

    if (ctrl==true&& p_private->p_focus->widget()!=NULL &&p_private->p_focus->widget()!=this)
    {
        p_private->p_multiplefocus.insert(p_private->p_multiplefocus.end(),p_private->p_focus);
        hk_kdeformfocus* pf=p_private->p_focus;
        p_private->p_focus=NULL;
        pf->repaint();
    }
    else clearmultiplefocus();
    if (p_private->p_focus!=NULL) delete p_private->p_focus;
    p_private->p_focus = s->new_focus();
    p_private->p_focus->set_widget(f);
    set_propertyobject(f);
    enable_actions();
emit signal_focuswidget_changed();

}


void hk_kdesimplereport::set_actualsection(hk_kdereportsection* s)
{
    p_private->p_focussection=s;

}


void hk_kdesimplereport::set_propertyobject(QWidget* f)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::set_propertyobject");
#endif
    hk_visible* v= dynamic_cast<hk_visible*>(f);
    if (p_private->p_property!=NULL)
    {
        if (p_private->p_focus!=NULL)QObject::connect(p_private->p_focus,SIGNAL(size_changed()),p_private->p_property,SLOT(focus_resized()));
        if (v==NULL) v=this;
        p_private->p_property->set_object(v);
    }

}


void hk_kdesimplereport::clearfocus(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::clearfocus");
#endif
    if (p_private->p_focus!=NULL)
    {
        delete p_private->p_focus;
        p_private->p_focus=NULL;

    }
    //p_private->p_controlbutton=false;
    clearmultiplefocus();
    enable_actions();
}


void hk_kdesimplereport::clearmultiplefocus(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::clearmultiplefocus");
#endif
    list<hk_kdeformfocus*>::iterator it=p_private->p_multiplefocus.begin();
    while (it!=p_private->p_multiplefocus.end())
    {
        hk_kdeformfocus* f=(*it);
        it++;
        if (f!=p_private->p_focus) delete f;
    }
    p_private->p_multiplefocus.erase(p_private->p_multiplefocus.begin(),p_private->p_multiplefocus.end());
    enable_actions();
}


void hk_kdesimplereport::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::before_source_vanishes");
#endif
    if (has_changed()) save_report();
    hk_report::before_source_vanishes();
    bool doemit=false;
    if (!p_private)return;
    QWidget* p=parentWidget();
    if (p_private->p_autoclose && !p_private->p_sourcevanishflag)
    {
        if (parentWidget()!=NULL)
        {
            doemit=true;
        }
    }
    p_private->p_sourcevanishflag=true;
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::before_source_vanishes ENDE");
#endif
 if (doemit)
 {
    setParent(NULL);
    setAttribute(Qt::WA_DeleteOnClose);
    if (p_private->p_kdereport)
      {
       hk_kdereport* k=p_private->p_kdereport;
       set_kdereport(NULL);
       k->close();
      }
    else
        if (p_private->p_reportpartwidget)
      {
       hk_kdereportpartwidget* k=p_private->p_reportpartwidget;
       set_reportpartwidget(NULL);
       k->close();
      }
    else p->close();
   close();

}

}


void hk_kdesimplereport::database_has_vanished(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::database_has_vanished");
#endif
    hk_report::database_has_vanished();
    bool doemit=false;
    QWidget* p=parentWidget();
    if (p_private->p_autoclose && !p_private->p_sourcevanishflag)
    {
        if (parentWidget()!=NULL)
        {
            doemit=true;
        }
    }
    p_private->p_sourcevanishflag=true;
 if (doemit)
 {
    setParent(NULL);
    setAttribute(Qt::WA_DeleteOnClose);
    if (p_private->p_kdereport)
      {hk_kdereport* k=p_private->p_kdereport;
       set_kdereport(NULL);
       k->close();
      }
    else
     if (p_private->p_reportpartwidget)
      {hk_kdereportpartwidget* k=p_private->p_reportpartwidget;
       set_reportpartwidget(NULL);
       k->close();
      }
    else p->close();
   close();
 }
}

void hk_kdesimplereport::set_field2create(bool c)
{
    p_private->p_createfield=c;

}


bool hk_kdesimplereport::field2create(void)
{
    return p_private->p_createfield;
}


bool hk_kdesimplereport::widget_specific_before_modechanges(enum_mode )
{
    /*if (m==viewmode)
    {

        return   execute();

    }
    else*/ return true;

}

bool hk_kdesimplereport::set_mode(enum_mode m)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::set_mode");
#endif
    if (p_private->while_modechange) return true;
    p_private->while_modechange=true;
    bool r=hk_report::set_mode(m);
    if (p_private->p_kdereport)
    {
        if (m==viewmode&&!r)
        {
	        hk_report::set_mode(designmode);
 	        p_private->p_kdereport->set_mode(designmode);
	    }
	    else {
 	        p_private->p_kdereport->set_mode(m);
        }
    }
    else
    if (p_private->p_reportpartwidget)
    {
        if (m==viewmode&&!r)
   		{
		    hk_report::set_mode(designmode);
 		    p_private->p_reportpartwidget->set_mode(designmode);
		}
		else
 		    p_private->p_reportpartwidget->set_mode(m);
    }
    p_private->while_modechange=false;
    return r;
}

void hk_kdesimplereport::widget_specific_modechanges(enum_mode s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::widget_specific_modechanges");
#endif
    list<hk_kdeformfocus*>::iterator it=p_private->p_multiplefocus.begin();
    if(s==designmode)
    {
        setFocusPolicy(Qt::StrongFocus);
        setFocus();
        qApp->installEventFilter(this);
        show_property();
	if (p_private->p_focus!=NULL)
        {
            if (p_private->p_focus->widget()!=NULL)
                p_private->p_property->set_object( dynamic_cast <hk_visible*> (p_private->p_focus->widget()));
            else p_private->p_property->set_object(this);
        }
        while (it!=p_private->p_multiplefocus.end())
        {

            hk_kdeformfocus* f=(*it);
            f->show();
            it++;
        }
        if (p_private->p_focus!=NULL)p_private->p_focus->show();

    }
    else                                          // s!=design
    {
        qApp->removeEventFilter(this);
        while (it!=p_private->p_multiplefocus.end())
        {
            hk_kdeformfocus* f=(*it);
            f->hide();
            it++;
        }
        if (p_private->p_focus!=NULL) p_private->p_focus->hide();
        if(p_private->p_property!=NULL)
	  {
	       if (p_private->p_propertydock)
	       {
	         p_private->p_propertydock->hide();
		 //p_private->p_propertydock->hide();
	       }
               else
	       p_private->p_property->hide();
	  }
        setFocusPolicy(Qt::ClickFocus);
    }
    enable_actions();
}

/*bool hk_kdesimplereport::ctrl_key_pressed(void) const
{
return p_private->p_controlbutton;
}*/

void hk_kdesimplereport::keyPressEvent ( QKeyEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::keyPressEvent");
#endif
     switch(event->key())
    {
//        case Key_Control:  p_private->p_controlbutton=true;
//        break;
       case Qt::Key_Delete:   delete_widgets();
        break;

       case Qt::Key_Left:
            if (p_private->p_focus &&p_private->p_focus->widget())
            {
              hk_visible* widget=dynamic_cast<hk_visible*>(p_private->p_focus->widget());
              if (widget)
              {
               unsigned int diff=(event->modifiers()&Qt::ControlModifier?1:snap2gridx());
               diff=(unsigned int)(sizetype()==hk_presentation::relative?
	       				(p_private->p_focussection?((double)diff*10000)/ p_private->p_focussection->max_displaywidth()+0.5:diff):
		     			((double)diff*100)/p_private->pix_je_cm+0.5);
        if (widget->x()>=diff) widget->set_x(widget->x()-diff);
              }

            }
break;


       case Qt::Key_Right:
if (p_private->p_focus &&p_private->p_focus->widget())
{
hk_visible* widget=dynamic_cast<hk_visible*>(p_private->p_focus->widget());
if (widget)
{
unsigned int diff=(event->modifiers()&Qt::ControlModifier?1:snap2gridx());
               diff=(unsigned int)(sizetype()==hk_presentation::relative?
	       				(p_private->p_focussection?((double)diff*10000)/ p_private->p_focussection->max_displaywidth()+0.5:diff):
		     			((double)diff*100)/p_private->pix_je_cm+0.5);
 if (widget->x()+widget->width()+diff<=10000) widget->set_x(widget->x()+diff);
}

}
break;


       case Qt::Key_Up:
if (p_private->p_focus &&p_private->p_focus->widget())
{
hk_visible* widget=dynamic_cast<hk_visible*>(p_private->p_focus->widget());
if (widget)
{
unsigned int diff=(event->modifiers()&Qt::ControlModifier?1:snap2gridy());
               diff=(unsigned int)(sizetype()==hk_presentation::relative?
	       				(p_private->p_focussection?((double)diff*10000)/ p_private->p_focussection->max_displayheight()+0.5:diff):
		     			((double)diff*100)/p_private->pix_je_cm+0.5); 
if (widget->y()>=diff) widget->set_y(widget->y()-diff);
}

}
break;


       case Qt::Key_Down:
if (p_private->p_focus &&p_private->p_focus->widget())
{
hk_visible* widget=dynamic_cast<hk_visible*>(p_private->p_focus->widget());
if (widget &&p_private->p_focussection)
{
unsigned int diff=(event->modifiers()&Qt::ControlModifier?1:snap2gridy());
               diff=(unsigned int)(sizetype()==hk_presentation::relative?
	       				(p_private->p_focussection?((double)diff*10000)/ p_private->p_focussection->max_displayheight()+0.5:diff):
		     			((double)diff*100)/p_private->pix_je_cm+0.5); 
if (p_private->p_focussection==page_header()||p_private->p_focussection==page_footer())
{
if (p_private->p_focussection==page_header())
{
if ( widget->y()+widget->height()+diff>=border_top())
diff=border_top()-widget->y()-widget->height();//limits size of header section
}
else//page_FOOTER
{
if ( widget->y()+widget->height()+diff>border_bottom())
diff=border_bottom()-widget->y()-widget->height();//limits size of footer section
}
}
if (widget->y()+widget->height()+diff<=10000) widget->set_y(widget->y()+diff);
}
}
break;

        default    :     ;
    }
    QWidget::keyPressEvent(event);

}


void hk_kdesimplereport::keyReleaseEvent ( QKeyEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::keyReleaseEvent");
#endif
   // if (event->key()==Key_Control)  p_private->p_controlbutton=false;
    QWidget::keyReleaseEvent(event);

}


void hk_kdesimplereport::delete_widgets(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::delete_widgets");
#endif
    QWidget* w=NULL;
//QWidget* del=NULL;
    list<hk_kdeformfocus*>::iterator it=p_private->p_multiplefocus.begin();
    while (it!=p_private->p_multiplefocus.end())
    {
        hk_kdeformfocus* f=(*it);
        it++;
        w=f->widget();
//del=w;
        f->set_widget(NULL);
//     remove_visible(dynamic_cast <hk_visible*>(del));
        delete w;
    }
    if (p_private->p_focus!=NULL)
    {
        w=p_private->p_focus->widget();
//del=w;
        p_private->p_focus->set_widget(NULL);
    }
//  remove_visible(dynamic_cast <hk_visible*>(del));
    if (p_private->p_property!=NULL) p_private->p_property->set_object(this);
    delete w;
    clearfocus();
    set_has_changed();
}


hk_kdeformfocus* hk_kdesimplereport::formfocus(void)
{
    return p_private->p_focus;
}


void hk_kdesimplereport::field_created(void)
{
    emit signalfield_created();
}


void hk_kdesimplereport::set_focus2property(void)
{

    clearfocus();
    set_propertyobject(this);

}


void hk_kdesimplereport::set_sections(void)
{

    delete p_private->p_reportlayout;
    p_private->p_reportlayout=new QVBoxLayout(this);
    p_private->p_reportlayout->setSizeConstraint(QLayout::SetFixedSize);
    p_private->p_reportlayout->setSpacing(0);
    p_private->p_reportlayout->setMargin(0);

    hk_kdereportsection* section=NULL;
    section=dynamic_cast<hk_kdereportsection*>(page_header());
    section->header()->setMinimumHeight(headerheight);
    p_private->p_reportlayout->addWidget(section->header());
    p_private->p_reportlayout->addWidget(section);
    section->show();
    section->header()->show();

    section=dynamic_cast<hk_kdereportsection*>(report_header());
    section->header()->setMinimumHeight(headerheight);
    p_private->p_reportlayout->addWidget(section->header());
    p_private->p_reportlayout->addWidget(section);
    section->show();
    section->header()->show();
    vector<hk_reportsectionpair*>* s=sectionpairs();
    vector<hk_reportsectionpair*>::iterator it=s->begin();
    while (it!=s->end())
    {
        hk_kdereportsection* s=dynamic_cast<hk_kdereportsection*>((*it)->headersection());
        if (s!=NULL)
        {
            s->header()->setMinimumHeight(headerheight);
            s->set_displaystring(i18n("Sectionheader"));
            p_private->p_reportlayout->addWidget(s->header());
            p_private->p_reportlayout->addWidget(s);
            s->show();
            s->header()->show();
        }
        it++;
    }

    hk_kdereportsection* sec=dynamic_cast<hk_kdereportsection*>(datasection());

    if (sec!=NULL)
    {
        sec->header()->setMinimumHeight(headerheight);
	p_private->p_reportlayout->addWidget(sec->header());
        p_private->p_reportlayout->addWidget(sec);
        sec->set_displaystring(i18n("Datasection"));
        sec->show();
	sec->header()->show();

    }

    int i=s->size();
    if (i>0)
    {
        while (i>0)
        {
            hk_kdereportsection* sec=dynamic_cast<hk_kdereportsection*>((*s)[i-1]->footersection());
            sec->header()->setMinimumHeight(headerheight);
            p_private->p_reportlayout->addWidget(sec->header());
            p_private->p_reportlayout->addWidget(sec);
            sec->set_displaystring(i18n("Sectionfooter"));
            sec->header()->show();
            sec->show();
            i--;
        }
    }

    section=dynamic_cast<hk_kdereportsection*>(report_footer());
    section->header()->setMinimumHeight(headerheight);
    p_private->p_reportlayout->addWidget(section->header());
    section->show();
    section->header()->show();
    p_private->p_reportlayout->addWidget(dynamic_cast<hk_kdereportsection*>(report_footer()));
    section=dynamic_cast<hk_kdereportsection*>(page_footer());
    p_private->p_reportlayout->addWidget(section->header());
    section->header()->show();
    p_private->p_reportlayout->addWidget(section);
    hk_kdereportsection* k=static_cast <hk_kdereportsection*>(page_header());
    k->set_displaystring(i18n("Pageheader"));
    k->show();
    k=static_cast <hk_kdereportsection*>(page_footer());
    k->set_displaystring(i18n("Pagefooter"));
    k->show();
    k=static_cast <hk_kdereportsection*>(report_header());
    k->set_displaystring(i18n("Reportheader"));
    k->show();
    k=static_cast <hk_kdereportsection*>(report_footer());
    k->set_displaystring(i18n("Reportfooter"));
    k->show();
}


bool hk_kdesimplereport::load_report(const hk_string& name)
{
    bool res=hk_report::load_report(name);
    set_sections();
    return res;
}


void hk_kdesimplereport::widget_specific_after_loadreport()
{
    clearfocus();
    if (p_private->p_property)
    {
    p_private->p_property->set_object(this);
    p_private->p_property->datasource_selected();
    }
// qApp->processEvents();

}


void    hk_kdesimplereport::widget_specific_before_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::widget_specific_before_execute");
#endif  
    p_private->p_cancelprinting=false;
    p_private->p_progressdialog= new  QProgressDialog();
    p_private->p_progressdialog -> setWindowModality(Qt::WindowModal);
    p_private->p_progressdialog->setWindowTitle(i18n("Generating report..."));
    p_private->p_progressdialog->setMaximum(datasource()->max_rows());
    p_private->p_progressdialog->setValue(0);
    connect(p_private->p_progressdialog,SIGNAL(canceled()),this,SLOT(printing_cancelled()));
    p_private->p_progressdialog->show();
    qApp->processEvents(); 
}


void    hk_kdesimplereport::widget_specific_after_execute(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::widget_specific_after_execute");
#endif  
    p_private->p_progressdialog->setValue(p_private->p_progressdialog->maximum());
    delete p_private->p_progressdialog;
    p_private->p_progressdialog=NULL;
}


bool hk_kdesimplereport::widget_specific_after_new_page(void)
{
  
    if (p_private->p_progressdialog!=NULL)
    {
        if (datasource()) 
	  p_private->p_progressdialog->setValue(datasource()->row_position()); 
	
    }
    if (p_private->p_reportpartwidget)qApp->processEvents();

    return !p_private->p_cancelprinting;
}


void hk_kdesimplereport::printing_cancelled(void)
{
    p_private->p_cancelprinting=true;

}


void hk_kdesimplereport::mousemenu(QMouseEvent* m)
{
    if (p_private->p_popup!=NULL) return;

    p_private->p_popup=new QMenu(this);
    QAction* p_Action;
    
    if (p_private->p_multiplefocus.size()>0)
    {
        QMenu* p_adjustsize = p_private->p_popup->addMenu(i18n( "Adjust &size" ));
	QMenu* p_align = p_private->p_popup->addMenu(i18n( "&Align" ));
	
        p_Action=p_adjustsize->addAction( i18n( "Minimum width" ) );
	p_Action-> setData(QVariant(3));
        p_Action=p_adjustsize->addAction( i18n( "Maximum width" ));
	p_Action-> setData(QVariant(4));
        p_Action=p_adjustsize->addAction( i18n( "Minimum height" ));
	p_Action-> setData(QVariant(5));
        p_Action=p_adjustsize->addAction( i18n( "Maximum height" ));
	p_Action-> setData(QVariant(6));
        p_Action=p_adjustsize->addAction( i18n( "Minimum size" ));
	p_Action-> setData(QVariant(7));
        p_Action=p_adjustsize->addAction( i18n( "Maximum size" ));
	p_Action-> setData(QVariant(8));
	p_Action=p_align->addAction( i18n( "&Left" ) );
	p_Action-> setData(QVariant(9));
        p_Action=p_align->addAction( i18n( "&Right" ));
	p_Action-> setData(QVariant(10));
        p_Action=p_align->addAction( i18n( "&Top" ) );
	p_Action-> setData(QVariant(11));
        p_Action=p_align->addAction( i18n( "&Bottom" ));
	p_Action-> setData(QVariant(12));
    }
    QMenu* p_settoall = p_private->p_popup->addMenu(i18n("Set to all"));
    p_Action=p_settoall->addAction(i18n("Font"));
    p_Action-> setData(QVariant(19));
    p_Action=p_settoall->addAction(i18n("Foregroundcolour"));
    p_Action-> setData(QVariant(20));
    p_Action=p_settoall->addAction(i18n("Backgroundcolour"));
    p_Action-> setData(QVariant(21));    
    if (p_private->p_focus!=NULL)
        if (p_private->p_focus->widget()!=NULL)
    {
      p_Action = p_private->p_popup->addAction( i18n( "&Delete" ) );   
      p_Action-> setData(QVariant(13)); 
      p_private->p_popup->addSeparator();
      p_Action = p_private->p_popup->addAction( i18n( "Into background" ) );   
      p_Action-> setData(QVariant(16));
      p_Action = p_private->p_popup->addAction( i18n( "Into foreground" ) );   
      p_Action-> setData(QVariant(17));
      p_private->p_popup->addSeparator();
    }
    p_Action = p_private->p_popup->addAction( i18n( "Show &property editor" ) );   
    p_Action-> setData(QVariant(14));
    p_Action = p_private->p_popup->addAction( i18n( "&Save report" ) );
    p_Action-> setData(QVariant(15));
    p_Action = p_private->p_popup->exec( m->globalPos() );
   switch (p_Action !=NULL?p_Action->data().toInt():0)
    {
        case 3:       adjust_widgets(sminw);
        break;
        case 4:       adjust_widgets(smaxw);
        break;
        case 5:       adjust_widgets(sminh);
        break;
        case 6:       adjust_widgets(smaxh);
        break;
        case 7:       adjust_widgets(smins);
        break;
        case 8:       adjust_widgets(smaxs);;
        break;
        case 9:       adjust_widgets(al);
        break;
        case 10:      adjust_widgets(ar);
        break;
        case 11:      adjust_widgets(at);
        break;
        case 12:      adjust_widgets(ab);
        break;
        case 13:      delete_widgets();
        break;
        case 14:
        {
            show_property();
            break;
        }
        case 15:
        {
            save_report("",false);
            break;
        }
        case 16:
        {
            hk_visible* v=dynamic_cast<hk_visible*>(p_private->p_focus->widget());
            if (v)
            {
                v->lower_widget();
                p_private->p_focus->widget()->lower();
                p_private->p_focus->set_widget(p_private->p_focus->widget());
            }
            if (p_private->p_multiplefocus.size()>0)
            {
                list<hk_kdeformfocus*>::reverse_iterator it=p_private->p_multiplefocus.rbegin();
                while(it!=p_private->p_multiplefocus.rend())
                {

                    v=dynamic_cast<hk_visible*>((*it)->widget());
                    if (v)
                    {
                        v->lower_widget();
                        (*it)->widget()->lower();
                        ++it;
                    }
                }
            }

            break;
        }
        case 17:
        {
            if (p_private->p_multiplefocus.size()>0)
            {
                list<hk_kdeformfocus*>::iterator it=p_private->p_multiplefocus.begin();
                while(it!=p_private->p_multiplefocus.end())
                {

                    hk_visible* v=dynamic_cast<hk_visible*>((*it)->widget());
                    if (v)
                    {
                        v->raise_widget();
                        (*it)->widget()->raise();
                        ++it;
                    }
                }
            }

            hk_visible* v=dynamic_cast<hk_visible*>(p_private->p_focus->widget());
            if (v)
            {
                v->raise_widget();
                p_private->p_focus->widget()->raise();
                p_private->p_focus->set_widget(p_private->p_focus->widget());
            }
            break;
        }
        case 19 :   set_for_all(bulkfont);break;
        case 20:    set_for_all(bulkforeground);break;
        case 21:    set_for_all(bulkbackground);break;
        default:;
    }

    delete p_private->p_popup; 
    p_private->p_popup=NULL;
}


void hk_kdesimplereport::adjust_widgets(hk_kdesimplereport::enum_adjust s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::adjust_widgets");
#endif
    int maxw=0;
    int maxh=0;
    int minw=90000;
    int minh=90000;
    int minx=90000;
    int miny=90000;
    int maxx=0;
    int maxy=0;
    list<hk_kdeformfocus*>::iterator it=p_private->p_multiplefocus.begin();
    while (it!=p_private->p_multiplefocus.end())
    {
        hk_kdeformfocus* f=(*it);
        it++;
        QWidget* w=f->widget();
        if  (maxw <w->width()) maxw=w->width();
        if  (maxh <w->height()) maxh=w->height();
        if  (minw >w->width()) minw=w->width();
        if  (minh >w->height()) minh=w->height();
        if  (minx >w->x()) minx=w->x();
        if  (maxx <w->x()+w->width()) maxx=w->x()+w->width();
        if  (miny >w->y()) miny=w->y();
        if  (maxy <w->y()+w->height()) maxy=w->y()+w->height();

    }
    if (p_private->p_focus==NULL) return;
    QWidget* w=p_private->p_focus->widget();
    if (w==NULL)
    {
        return;
    }
    if  (maxw <w->width()) maxw=w->width();
    if  (maxh <w->height()) maxh=w->height();
    if  (minw >w->width()) minw=w->width();
    if  (minh >w->height()) minh=w->height();
    if  (minx >w->x()) minx=w->x();
    if  (maxx <w->x()+w->width()) maxx=w->x()+w->width();
    if  (miny >w->y()) miny=w->y();
    if  (maxy <w->y()+w->height()) maxy=w->y()+w->height();

    it=p_private->p_multiplefocus.begin();
    while (it!=p_private->p_multiplefocus.end())
    {
        hk_kdeformfocus* f=(*it);
        it++;
        resize_widget(f->widget(),s,minw,maxw,minh,maxh,minx,maxx,miny,maxy);
    }

    resize_widget(p_private->p_focus->widget(),s,minw,maxw,minh,maxh,minx,maxx,miny,maxy);
    repaint_focus();
}


void hk_kdesimplereport::resize_widget(QWidget* w,enum_adjust s,int minw,int maxw,int minh,int maxh, int minx, int maxx, int miny, int maxy)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport:resize_widget");
#endif
    switch (s)
    {
        case hk_kdesimplereport::sminw :  w->setGeometry(w->x(),w->y(),minw,w->height());
        break;
        case hk_kdesimplereport::smaxw :  w->setGeometry(w->x(),w->y(),maxw,w->height());
        break;
        case hk_kdesimplereport::sminh :  w->setGeometry(w->x(),w->y(),w->width(),minh);
        break;
        case hk_kdesimplereport::smaxh :  w->setGeometry(w->x(),w->y(),w->width(),maxh);
        break;
        case hk_kdesimplereport::smins :  w->setGeometry(w->x(),w->y(),minw,minh);
        break;
        case hk_kdesimplereport::smaxs :  w->setGeometry(w->x(),w->y(),maxw,maxh);
        break;
        case hk_kdesimplereport::al :  w->setGeometry(minx,w->y(),w->width(),w->height());
        break;
        case hk_kdesimplereport::ar :  w->setGeometry(maxx-w->width(),w->y(),w->width(),w->height());
        break;
        case hk_kdesimplereport::at :  w->setGeometry(w->x(),miny,w->width(),w->height());
        break;
        case hk_kdesimplereport::ab :  w->setGeometry(w->x(),maxy-w->height(),w->width(),w->height());
        break;
        default:;
    }

}


void hk_kdesimplereport::repaint_focus(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::repaint_focus");
#endif
    list<hk_kdeformfocus*>::iterator it=p_private->p_multiplefocus.begin();
    while (it!=p_private->p_multiplefocus.end())
    {
        hk_kdeformfocus* f=(*it);
        it++;
        f->set_positions();
    }
    p_private->p_focus->set_positions();
}


bool hk_kdesimplereport::is_reportobject(QWidget* v)
{
#ifdef HK_DEBUG
    //hkdebug("hk_kdesimplereport::is_reportobject");
#endif
    QWidget* object =v;

    while (object!=NULL)
    {
        if (object==this) return true;
        if (p_private && object==p_private->p_property) return false;
        object=object->parentWidget();

    }

    return false;
}


void hk_kdesimplereport::show_property(void)
{
    if (mode()!=hk_presentation::designmode)return;
    bool newproperty=false;
    if (p_private->p_property==NULL)
    {
         p_private->p_property=new hk_kdereportproperty(this, 0, 0);
	 p_private->p_property->set_object(this);
         if (p_private->p_kdereport)
        {
	 p_private->p_propertydock= new QDockWidget(p_private->p_kdereport);
	 p_private->p_propertydock->setWidget(p_private->p_property);	 
	}
	else
	if (p_private->p_reportpartwidget)
        {
	 p_private->p_propertydock=new QDockWidget(p_private->p_reportpartwidget);
	 p_private->p_propertydock->setWidget(p_private->p_property);
	 p_private->p_reportpartwidget->addDockWidget(Qt::RightDockWidgetArea,p_private->p_propertydock);	  
	}

       newproperty=true;

    }

    if (p_private->p_property!=NULL)
    {
     if (p_private->p_propertydock)
        {
	  //p_private->p_propertydock->makeDockVisible();
	   p_private->p_propertydock->show();
          //p_private->p_propertydock->show();
	}

     else
     p_private->p_property->show();
     //qApp->processEvents();


       if (newproperty)
       {
	int screenwidth=qApp->desktop()->availableGeometry(this).width();
	int screenheight=qApp->desktop()->availableGeometry(this).height();
	     if (!p_private->p_propertydock)
		p_private->p_property->move(screenwidth-p_private->p_property->frameGeometry().width(),
			 screenheight-p_private->p_property->frameGeometry().height());
	}
    }
}


void hk_kdesimplereport::create_copydata(ostream& stream,QWidget*w)
{
    hk_string vistag="REPORTOBJECT";
    if (w)
    {
        hk_reportdata* v = dynamic_cast<hk_reportdata*> (w);
        if (v)
        {
            start_mastertag(stream,vistag);
            v->savedata(stream,(reporttype()=="Userdefined"));
            end_mastertag(stream,vistag);
        };

    };

}


void hk_kdesimplereport::copy_widgets(void)
{
    if (mode()!=designmode) return;
    if (!p_private->p_focus || !p_private->p_focus->widget()) return;
    QMimeData *mimedata = new QMimeData();
    QByteArray copydata;    
    STRINGSTREAM stream;

    list<hk_kdeformfocus*>::iterator it=p_private->p_multiplefocus.begin();
    while (it!=p_private->p_multiplefocus.end())
    {
        create_copydata(stream,(*it)->widget());
        it++;
    }
    if (p_private->p_focus) create_copydata(stream,p_private->p_focus->widget());
    char c;
    while (stream.get(c))
        copydata.append(c);
    
    mimedata->setData(p_private->mimeCopyFormat,p_private->startTagDrag + copydata +
        p_private->endTagDrag);
    QApplication::clipboard()->setMimeData(mimedata);
}

bool hk_kdesimplereport::canProcessClipboard() const
{
    return QApplication::clipboard()->mimeData() != NULL &&
    QApplication::clipboard()->mimeData()->hasFormat(p_private->mimeCopyFormat);
}

void hk_kdesimplereport::paste_widgets(void)
{
    const QMimeData *clipmime = QApplication::clipboard()->mimeData();
    
    if (clipmime == NULL || !clipmime->hasFormat(p_private->mimeCopyFormat))
        return;
    clearfocus();
    hk_string def=u2l(clipmime->data(p_private->mimeCopyFormat).constData());
    xmlDocPtr doc=xmlParseMemory(def.c_str(),def.size());
    xmlNodePtr definition=xmlDocGetRootElement(doc);
    int i=1;
    xmlNodePtr value;
    hk_string xxx,buffer2;
    hk_kdereportsection* rs=dynamic_cast<hk_kdereportsection*>((p_private->p_focussection?p_private->p_focussection:datasection()));
    if (!rs)
    {
        show_warningmessage(u2l(i18n("Error while paste action: No reportsection!!!").toUtf8().data()));
        return;
    }
    while ((value=get_tagvalue(definition,"REPORTOBJECT",xxx,i)))
    {

        hk_reportdata* vis=rs->new_data();
        if (vis!=NULL)
        {
            vis->set_load_presentationnumber(false);
            vis->loaddata(value,(reporttype()=="Userdefined"));
            register_object(vis);
            QWidget* w=dynamic_cast<QWidget*>(vis);
            if (w)
            {
                set_focus(w,rs,true);
                w->repaint();
            }
        }
        i++;
    }

}


void hk_kdesimplereport::cut_widgets(void)
{
    copy_widgets();
    delete_widgets();

}


void hk_kdesimplereport::enable_actions(void)
{
    if (mode()==viewmode)
    {
        if (p_private->p_copyaction)p_private->p_copyaction->setEnabled(false);
        if (p_private->p_cutaction)p_private->p_cutaction->setEnabled(false);
        if (p_private->p_pasteaction)p_private->p_pasteaction->setEnabled(false);
        if (p_private->p_deleteaction)p_private->p_deleteaction->setEnabled(false);

    }
    else
    {                                             //designmode
        if (p_private->p_copyaction)p_private->p_copyaction->setEnabled(p_private->p_focus&&p_private->p_focus->widget());
        if (p_private->p_cutaction)p_private->p_cutaction->setEnabled(p_private->p_focus&&p_private->p_focus->widget());
        if (p_private->p_deleteaction)p_private->p_deleteaction->setEnabled(p_private->p_focus&&p_private->p_focus->widget());
        if (p_private->p_pasteaction) p_private->p_pasteaction->setEnabled(QApplication::clipboard()->mimeData() &&
            QApplication::clipboard()->mimeData()->hasFormat(p_private->mimeCopyFormat));
    }

   if (p_private->p_resizeaction) p_private->p_resizeaction->setEnabled(mode()==designmode&&p_private->p_multiplefocus.size()>0);
   if (p_private->p_alignaction) p_private->p_alignaction->setEnabled(mode()==designmode&&p_private->p_multiplefocus.size()>0);

}


void hk_kdesimplereport::align_left(void)
{
    adjust_widgets(al);
}


void hk_kdesimplereport::align_right(void)
{
    adjust_widgets(ar);
}


void hk_kdesimplereport::align_top(void)
{
    adjust_widgets(at);
}


void hk_kdesimplereport::align_bottom(void)
{
    adjust_widgets(ab);
}


void hk_kdesimplereport::adjust_minw(void)
{
    adjust_widgets(sminw);
}


void hk_kdesimplereport::adjust_maxw(void)
{
    adjust_widgets(smaxw);
}


void hk_kdesimplereport::adjust_minh(void)
{
    adjust_widgets(sminh);
}


void hk_kdesimplereport::adjust_maxh(void)
{
    adjust_widgets(smaxh);
}


void hk_kdesimplereport::adjust_mins(void)
{
    adjust_widgets(smins);
}


void hk_kdesimplereport::adjust_maxs(void)
{
    adjust_widgets(smaxs);
}


void hk_kdesimplereport::widget_specific_presentationresize(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimplereport::widget_specific_presentationresize");
#endif
    set_sectionsize(datasection());
    set_sectionsize(report_header());
    set_sectionsize(report_footer());
    set_sectionsize(page_header());
    set_sectionsize(page_footer());
    vector<hk_reportsectionpair*>* sp=sectionpairs();

    if (sp)
    {
        vector<hk_reportsectionpair*>::iterator it=sp->begin();
        while (it!=sp->end())
        {
            set_sectionsize((*it)->headersection());
            set_sectionsize((*it)->footersection());
            ++it;
        }
    }
}


void hk_kdesimplereport::set_sectionsize(hk_reportsection* rs)
{
    hk_kdereportsection* s= dynamic_cast<hk_kdereportsection*>(rs);
    if (s) s->resize_section();
}


void    hk_kdesimplereport::widget_specific_backgroundcolour_changed(const hk_colour&)
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p = palette();
    
    p.setColor(backgroundRole(),newcolour);
    setPalette(p);    

}


void    hk_kdesimplereport::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p = palette();
    
    p.setColor(foregroundRole(),newcolour);
    setPalette(p);
}

void hk_kdesimplereport::set_for_all(enum_bulkoperation bulk)
{
    if (p_private->p_focus)
    {
        hk_visible* v= dynamic_cast<hk_visible*>(p_private->p_focus->widget());
        if (v)
        {
            switch (bulk)
            {
                case bulkforeground : set_foregroundcolour(v->foregroundcolour());
                break;
                case bulkbackground : set_backgroundcolour(v->backgroundcolour());
                break;
                case bulkfont: set_font(v->font());

            }
        }
    }
    bulk_operation(bulk);

}

void hk_kdesimplereport::set_autoclose(bool c)
{
p_private->p_autoclose=c;
}


hk_kdeformfocus* hk_kdesimplereport::focus(void)
{
return p_private->p_focus;
}

hk_kdereport* hk_kdesimplereport::kdereport(void) 
{
return p_private->p_kdereport;
}




void hk_kdesimplereport::script_error(hk_visible* v, hk_interpreter::enum_action a)
{
    if (!v)
    {
        cerr<<"hk_kdesimplereport::script_error called without visible object!"<<endl;
        return;
    }

    hk_string error=replace_all("%LINENUMBER%",hk_translate("Line %LINENUMBER%: "),longint2string(interpreter()->error_rownumber()))
    				+interpreter()->errormessage();

    hk_dsvisible* dv=dynamic_cast<hk_dsvisible*>(v);
    if (dv)
    {
	    hk_datasource* ds=dv->datasource();
	    if (ds) ds->set_ignore_changed_data();
    }

    if (!runtime_only())
    {
        if (kdereport()) kdereport()->set_mode(hk_presentation::designmode);
        if (p_private->p_reportpartwidget)
            p_private->p_reportpartwidget->set_mode(hk_presentation::designmode);
        if (p_private->p_property)
	    {
            hk_kdereportsection* s=NULL;
            hk_kdereportdata* d=dynamic_cast<hk_kdereportdata*>(v);
            if (d!=NULL) s=d->section();
	        set_focus(d,s,false);
	        switch (a)
	        {
		    case hk_interpreter::a_on_open :
				p_private->p_property->openactionbutton_clicked(presentation()->interpreter()->error_rownumber()-1,
                    error);
				break;
            case hk_interpreter::a_on_close :
				p_private->p_property->closeactionbutton_clicked(presentation()->interpreter()->error_rownumber()-1,
                    error);
				break;
            case hk_interpreter::a_after_row_change :
				p_private->p_property->afterrowchange_action_clicked(presentation()->interpreter()->error_rownumber()-1,
                   error);
				break;
            case hk_interpreter::a_before_row_change :
				p_private->p_property->beforerowchange_action_clicked(presentation()->interpreter()->error_rownumber()-1,
                    error);
				break;
            case hk_interpreter::a_on_print_data :
				p_private->p_property->onprint_action_clicked(presentation()->interpreter()->error_rownumber()-1,
                    error);
				break;
            case hk_interpreter::a_on_print_new_page :
				p_private->p_property->onprintnewpage_action_clicked(presentation()->interpreter()->error_rownumber()-1,
                    error);
				break;
			default:;
            }
        } 
    }
    else     show_warningmessage(error);
}

void    hk_kdesimplereport::widget_specific_fieldresize(hk_visible* v)
{
  if (!v||!p_private->p_property) return;
  if (v==p_private->p_property->object())
    p_private->p_property->set_object(v);
}



bool hk_kdesimplereport::focus_multipleselected(void) const
{
return p_private->p_multiplefocus.size()>0;
}


void hk_kdesimplereport::set_has_changed(enum_has_changed forcesetting)
{
hk_report::set_has_changed(forcesetting);
emit signal_has_changed();
}



void hk_kdesimplereport::reset_has_changed(void)
{
  hk_report::reset_has_changed();
emit signal_has_changed();
}

