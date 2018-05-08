// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
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
//$Revision: 1.141 $
#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#error config.h is needed but not included 
#endif
#include "hk_kdesimpleform.h"
#include "hk_kdeform.h"
#include "hk_kdeformpartwidget.h"
#include "hk_kdegrid.h"
#include "hk_kde4simplegrid.h"
#include "hk_kdelineedit.h"
#include "hk_kdeboolean.h"
#include "hk_kdesimpleform.moc"
#include "hk_kderowselector.h"
#include "hk_kdememo.h"
#include "hk_kdebutton.h"
#include "hk_kdecombobox.h"
#include "hk_kdeformfocus.h"
#include "hk_kdeproperty.h"
#include "hk_kdelabel.h"
#include "hk_kdesubform.h"
#include "hk_kdeimage.h"
#include "../hk_kdegridpart/hk_kdegridpart.h"
#include "hk_kdedbdesigner.h"
#include "hk_kdedate.h"
#include "../hk_kdeformpart/hk_kdeformpart.h"

#include "hk_visible.h"
#include "hk_dsvisible.h"
#include "hk_datasource.h"

#include <qdatetime.h>
#include <qeventloop.h>
#include <qpainter.h>
#include <qevent.h>
#include <qsplitter.h>
#include <qlayout.h>
#include <qcolor.h>
#include <qtooltip.h>
#include <qdesktopwidget.h>
#include <kparts/partmanager.h>
#include <klocale.h>
#include <kaction.h>
#include <kapplication.h>
#include <qclipboard.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qdockwidget.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <kactionmenu.h>
#include <kmenu.h>
#include <kactioncollection.h>
#include <kservice.h>

class hk_kdesimpleformprivate
{
public:
    hk_kdesimpleformprivate(hk_kdesimpleform* theform):while_modechange(false),while_loading(false),eventloop(NULL),
    p_with_label(false),p_designer(NULL),p_dbdesigneraction(NULL),p_propertydock(NULL),
    p_moverect(QRect(0,0,0,0)),p_already_selected_widget_clicked(false),simpleform(theform)
    {         
    }

    ~hk_kdesimpleformprivate()
    {
        if (p_designer) delete p_designer;        
    }

    inline QWidget* focusChild()
    {
        return  simpleform->focusWidget()!=NULL?simpleform->focusWidget():
        simpleform->isAncestorOf(QApplication::focusWidget())?QApplication::focusWidget():NULL;
        
    }
    static const QString mimeCopyFormat;
    bool while_modechange;
    bool while_loading;
    QEventLoop* eventloop;
    bool p_with_label;
    hk_kdedbdesignerwindow* p_designer;
    KAction * p_dbdesigneraction;
    QDockWidget* p_propertydock;
    QRect p_moverect;
    bool p_already_selected_widget_clicked;
    QColor p_filtercolourbuffer;
    hk_kdesimpleform* simpleform;
};

const QString hk_kdesimpleformprivate::mimeCopyFormat = QString::fromAscii("application/x-hk_kdesimpleformcopy");

static void unplugAll(KAction* p_a){
  foreach(QWidget* w, p_a->associatedWidgets())
    w->removeAction(p_a);
}

static void unplugAll(KActionMenu* p_a){
  foreach(QWidget* w, p_a->associatedWidgets())
    w->removeAction(p_a);
}

static KAction* createAction(const QString& lbl, const QString& p_name, KActionCollection* p_ac, hk_kdesimpleform* tgt, 
			     void (hk_kdesimpleform::*action_hander)() ){
  Q_UNUSED(action_hander);  
  KAction * p_a = new KAction(lbl,p_ac);
  
  p_ac->addAction(p_name,p_a);
  QObject::connect(p_ac,SIGNAL(triggered()),tgt, SLOT(*action_handler));
  return p_a;
}

hk_kdesimpleform::hk_kdesimpleform(QWidget* wid,const char* n,Qt::WFlags f) :QWidget(wid,f),hk_form()
{
#ifdef HK_DEBUG
    hkclassname("hk_kdesimpleform");
    // wanna_debug(true);
    hkdebug("hk_kdesimpleform::hk_kdesimpleform");
#endif
    setObjectName(QString::fromAscii(n));
    p_private=new hk_kdesimpleformprivate(this);
    p_partsmanager= new KParts::PartManager(this);

    setFixedWidth(p_actualwidth);
    setFixedHeight(p_actualheight);
    p_autoclose=true;
    p_field2create=other;
    p_property=NULL;
    p_focus = new hk_kdeformfocus(this);
    p_createflag=false;
    //p_controlbutton=false;
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    p_kdeform=NULL;
    p_formpartwidget=NULL;
    p_popup=NULL;
    p_sourcevanishflag=false;
    p_deleteaction=NULL;
    p_copyaction=NULL;
    p_pasteaction=NULL;
    p_cutaction=NULL;
    p_formpropertyaction=NULL;
    p_resizeaction=NULL;
    p_minwidthaction=NULL;
    p_maxwidthaction=NULL;
    p_minheightaction=NULL;
    p_maxheightaction=NULL;
    p_minsizeaction=NULL;
    p_maxsizeaction=NULL;
    p_alignaction=NULL;
    p_alignleftaction=NULL;
    p_alignrightaction=NULL;
    p_aligntopaction=NULL;
    p_alignbottomaction=NULL;
    p_setallaction = NULL;

    connect (QApplication::clipboard(), SIGNAL(dataChanged()),this, SLOT(enable_actions()));
   
    QColor qcolor=palette().color(backgroundRole());
    hk_colour colour(qcolor.red(),qcolor.green(),qcolor.red());
    set_backgroundcolour(colour,false,true);
    p_private->p_filtercolourbuffer=qcolor;
    qcolor=palette().color(foregroundRole());
    colour.set_colour(qcolor.red(),qcolor.green(),qcolor.red());
    set_foregroundcolour(colour,false,true);
}


hk_kdesimpleform::~hk_kdesimpleform(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::~hk_kdesimpleform");
#endif
    list<hk_datasource*>::iterator it= datasources()->begin();
    
    while (it != datasources()->end()) {
        if ((*it) != NULL && (*it)->check_store_changed_data()) 
            (*it)->store_changed_data();
        it++;
    } 
    
    set_actions(NULL);
    delete p_focus;
    while(p_partsmanager->activePart())
    {
       p_partsmanager->removePart(p_partsmanager->activePart());
    }
    if(p_property!=NULL &&!p_private->p_propertydock)
        delete p_property;
    delete p_partsmanager;
    delete p_deleteaction;
    delete p_copyaction;
    delete p_pasteaction;
    delete p_cutaction;
    delete p_private->p_dbdesigneraction;
    delete p_private;
    p_private=NULL;
}


hk_dsgrid*  hk_kdesimpleform::widget_specific_new_grid(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_new_grid(void)");
#endif
    KParts::ReadWritePart* p_part;
    KService::Ptr service = KService::serviceByDesktopName("hk_kde5gridpart");   
  
    if ( !service ||
        !(p_part = service->createInstance<KParts::ReadWritePart>(this,this, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Grid part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
    }
    // do not activate part, otherwise grid gets focus during load 
    p_partsmanager->addPart(p_part,false);
    hk_kdegrid* g=p_part?(hk_kdegrid*)p_part->widget():new hk_kdegrid(this,0,0,this);
    if (g==NULL) return NULL;
    g->show();
#ifdef HK_DEBUG
    g->hkclassname("hk_kdegrid");
#endif
    return g; 
}


hk_dslineedit*      hk_kdesimpleform::widget_specific_new_lineedit(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_new_lineedit(void)");
#endif
    hk_kdelineedit* l=new hk_kdelineedit(this,this);
    l->show();
#ifdef HK_DEBUG
    l->hkclassname("lineedit");
#endif
//	set_focus(l);
    return l;
}


hk_dsboolean*       hk_kdesimpleform::widget_specific_new_bool(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_new_bool(void)");
#endif
   hk_kdeboolean* b=new hk_kdeboolean(this,this);
    b->show();
#ifdef HK_DEBUG
    b->hkclassname("bool");
#endif
    b->setBackgroundRole(backgroundRole());
//	set_focus(b);
    return b;
}


hk_dscombobox*      hk_kdesimpleform::widget_specific_new_combobox(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_new_combobox(void)");
#endif
    hk_kdecombobox* c=new hk_kdecombobox(this,this);
    c->show();
#ifdef HK_DEBUG
    c->hkclassname("combobox");
#endif
//	set_focus(c);
    return c;
}


hk_dsmemo*      hk_kdesimpleform::widget_specific_new_memo(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_new_memo(void)");
#endif
    hk_kdememo* m=new hk_kdememo(this,this);
    m->show();
#ifdef HK_DEBUG
    m->hkclassname("memo");
#endif
//	set_focus(m);
    return m;
}


hk_button*      hk_kdesimpleform::widget_specific_new_button(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_new_button(void)");
#endif
    hk_kdebutton* b=new hk_kdebutton(this,this);
    b->show();
#ifdef HK_DEBUG
    b->hkclassname("button");
#endif
    return b;
}


hk_dsrowselector*   hk_kdesimpleform::widget_specific_new_rowselector(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_new_rowselector(void)");
#endif
    hk_kderowselector* s=new hk_kderowselector(this,this);
    s->show();
#ifdef HK_DEBUG
    s->hkclassname("rowselector");
#endif
    return s;
}


hk_subform*   hk_kdesimpleform::widget_specific_new_subform(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_new_subform(void)");
#endif
    hk_kdesubform* s=new hk_kdesubform(this);
    s->show();
#ifdef HK_DEBUG
    s->hkclassname("subform");
#endif
    return s;
}


hk_label* hk_kdesimpleform::widget_specific_new_label(void)
{
#ifdef HK_DEBUG
  hkdebug("hk_kdesimpleform::widget_specific_new_label");
#endif
  hk_kdelabel* s=new hk_kdelabel(this);
  s->show();
#ifdef HK_DEBUG
    s->hkclassname("label");
#endif
    s->setBackgroundRole(backgroundRole());
    return s;
}

hk_dsdate*       hk_kdesimpleform::widget_specific_new_date(void)
{
   hk_kdedate* l=new hk_kdedate(this,this);
   l->show();
#ifdef HK_DEBUG
    l->hkclassname("date");
#endif
//	set_focus(l);
    return l;
}

hk_dsimage*      hk_kdesimpleform::widget_specific_new_image(void)
{
  hk_kdeimage* i=new hk_kdeimage(this,this);
#ifdef HK_DEBUG
  i->hkclassname("image");
  i->show();
#endif
  return i;
}


hk_tabvisible*   hk_kdesimpleform::widget_specific_new_tabvisible(void)
{
  return NULL;
}



void    hk_kdesimpleform::widget_specific_fieldresize(hk_visible* v)
{
    if (!v ||!p_property) return;
    if (v==p_property->object())
 	{
	//p_property->set_visible();
	  p_property->set_geometry();
	}
}


void    hk_kdesimpleform::widget_specific_presentationresize(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_presentationresize");
#endif
    setFixedWidth(p_actualwidth);
    setFixedHeight(p_actualheight);
}


void hk_kdesimpleform::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::before_source_vanishes");
#endif
    bool doemit=false;

    if (p_kdeform) p_kdeform->save_form_when_changed();
    else 
    if (p_formpartwidget)p_formpartwidget->save_form_when_changed();
    hk_form::before_source_vanishes();

   QWidget* p=parentWidget();

    if (p_autoclose &&!p_sourcevanishflag)
    {
        if (p!=NULL)
        {
            doemit=true;
        }
    }
    p_sourcevanishflag=true;

  if (doemit)
  {
    setParent(NULL);
    setAttribute(Qt::WA_DeleteOnClose);
    if (p_kdeform) {
      hk_kdeform* k=p_kdeform;
      set_kdeform(NULL);
      k->close();
    }
    else
    if (p_formpartwidget)
      {hk_kdeformpartwidget* k=p_formpartwidget;
       set_formpartwidget(NULL);
       k->close();
      }
    else p->close();
    close();
  }
}


void hk_kdesimpleform::database_has_vanished(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::database_has_vanished");
#endif
    bool doemit=false;
    hk_form::database_has_vanished();
   QWidget* p=parentWidget();

    if (p_autoclose&&!p_sourcevanishflag)
    {
        if (parentWidget()!=NULL)
        {
            doemit=true;
        }
    }
    p_sourcevanishflag=true;
if (doemit)
 {
   setParent(NULL);
   setAttribute(Qt::WA_DeleteOnClose);
    if (p_kdeform)
    {
      hk_kdeform* k=p_kdeform;
      set_kdeform(NULL);
      k->close();
    } else
        if (p_formpartwidget)
      {hk_kdeformpartwidget* k=p_formpartwidget;
       set_formpartwidget(NULL);
       k->close();
      }
    else p->close();
   close();
}
}


void    hk_kdesimpleform::mousePressEvent(QMouseEvent* m)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::mousePressEvent");
#endif
    if (mode()==designmode)
    {
        switch (m->button())
        {
	  case Qt::LeftButton:
            {
                emit mouseclick(m->pos());
                if (p_field2create!=other)create_field(m->pos());
                else
                {
                    clearfocus();
                       set_currentobject(this);
                }
                break;
            }

	  case Qt::RightButton:
            {
                mousemenu(m);
                break;
            }
            default:      ;
        }
    }
    else
    { //viewmode
    //execute script
      if (m->button()== Qt::LeftButton) action_on_click();
    }
    QWidget::mousePressEvent(m);
}


void  hk_kdesimpleform::mouseDoubleClickEvent(QMouseEvent* event)
{
   if (mode()==viewmode
       && event->button()== Qt::LeftButton)
           action_on_doubleclick();
   QWidget::mouseDoubleClickEvent(event);
}



void    hk_kdesimpleform::mouseReleaseEvent(QMouseEvent* )
{
}


void hk_kdesimpleform::set_field2create(enum_visibletype v)
{
    p_field2create=v;

}


hk_visible* hk_kdesimpleform::create_field(const QPoint& position)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::create_field");
#endif
    hk_visible* newfield=NULL;
    QPoint point=position;
	if (snap2gridx()>0&&point.x()%snap2gridx()>0)
		point.setX((point.x()/snap2gridx())*snap2gridx());
	if (snap2gridy()>0&&point.y()%snap2gridy()>0)
		point.setY((point.y()/snap2gridy())*snap2gridy());

       hk_label* l=NULL;
      if (create_with_label()&&(p_field2create != textlabel  &&p_field2create !=boolean ))
      {
       enum_visibletype t= p_field2create;
       p_field2create=textlabel;
	l=dynamic_cast<hk_label*>(create_field(point));
       p_field2create=t;
       point.setX(point.x()+130);
      }
    switch (p_field2create)
    {
        case lineedit : newfield = new_lineedit();
        break;
        case textlabel : newfield = new_label();
        break;
        case button : newfield = new_button();
        break;
        case rowselector : newfield = new_rowselector();
        break;
        case memo : newfield = new_memo();
        break;
        case boolean : newfield = new_bool();
        break;
        case grid : newfield = new_grid();
        break;
        case combobox : newfield = new_combobox();
        break;
        case subform : newfield = new_subform();
        break;
        case image : newfield = new_image();
        break;
/*        case tabvisible : newfield = new_tabvisible();
        break;*/
        case date : newfield = new_date();
        break;
        default:;

    }
    if (newfield!=NULL)
    {
        if (sizetype()==hk_presentation::relative)
	{
	newfield->set_position((point.x()*10000)/QWidget::width(),(point.y()*10000)/QWidget::height());
        if (newfield->type()==grid)
            newfield->set_size((point.x()*10000)/QWidget::width(),(point.y()*10000)/QWidget::height(),4000,3000);
	}
	else
	{
        if (newfield->type()==grid||newfield->type()==memo)
            newfield->set_size((unsigned int)point.x(),(unsigned int)point.y(),300,200);
	else
		newfield->set_size((unsigned int)point.x(),(unsigned int)point.y(),100,30);

	}
        set_focus(dynamic_cast <QWidget*>(newfield),false);
    }
    else cerr <<"hk_kdesimpleform newfield=NULL!"<<endl;
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::create_field focus setzen");
#endif
    p_focus->set_positions();
    p_createflag=true;
    emit field_created();
    p_field2create=other;
    if (create_with_label()&& newfield && l)
       {
       newfield->set_buddylabel(l->presentationnumber());
       }
    return newfield;
}


void hk_kdesimpleform::paintEvent(QPaintEvent* e)
{
    if(p_private->while_loading) return;
    if (mode()==hk_presentation::viewmode ||mode()==hk_presentation::filtermode)
    {
        QWidget::paintEvent(e) ;
        return;
    }

    QPainter p(this);
    p.setClipRegion( e->rect() );
    p.setPen(palette().color(foregroundRole()));
    for (int a=0; a<QWidget::width()/10+20; a++ )
    {
        for (int b=0; b<QWidget::height()/10+20;b++ )
            p.drawPoint(a*10,b*10+3);
    }

}


bool hk_kdesimpleform::eventFilter(QObject* object,QEvent* event)
{
  if (   mode()!=designmode
        ||!object->isWidgetType()
        )
        return QWidget::eventFilter(object,event);

    hk_visible* v = dynamic_cast <hk_visible*>(object);
    hk_visible* p = dynamic_cast <hk_visible*> ( ((QWidget*) object)->parentWidget() );
    if ((v==NULL&& p==NULL)|| dynamic_cast <hk_marker*>(object)!=NULL||!is_formobject((QWidget*) object))
    {
        return QWidget::eventFilter(object,event);
    }
    return formeventFilter(object,event);
}


bool hk_kdesimpleform::formeventFilter(QObject* object,QEvent* event)
{
#ifdef HK_DEBUG
//hkdebug("hk_kdesimpleform::formeventFilter");
#endif
  QWidget* widget= (QWidget*) object;
// check for special widget treatment
  if (dynamic_cast <hk_visible*>(object) == NULL)
  {
    widget = widget->parentWidget();
    QWidget* object =widget;
    while (object!=NULL)
    {
      if (dynamic_cast <hk_kdegrid*>(object)!=NULL
	       ||dynamic_cast <hk_kdesubform*>(object)!=NULL
         )
      {
        widget=object;
        object=NULL;
      }
      else    object=object->parentWidget();
    }
  }
  else
  {
    QWidget* object =widget;
    while (object!=NULL)
    {
      if (dynamic_cast <hk_kdegrid*>(object)!=NULL)
      {
        widget=object;
        object=NULL;
      }
      else   object=object->parentWidget();
    }
  }

  if (dynamic_cast <hk_kderowselector*>(widget)!=NULL)
  {
    if (dynamic_cast <hk_kdegrid*>(widget->parentWidget())!=NULL)
      widget = widget->parentWidget();
  }

// ENDE check for special widget treatment

  switch (event->type())
  {
    case QEvent::KeyPress:
    {
      keyPressEvent((QKeyEvent*)event);
      break;
    };
    case QEvent::KeyRelease:
    {
      keyReleaseEvent((QKeyEvent*)event);
      break;
    };
    case QEvent::MouseButtonRelease:
    {
      if (p_private->p_already_selected_widget_clicked)
        set_focus(widget,false);
      p_private->p_already_selected_widget_clicked=false;
      break;
    };
    case QEvent::MouseButtonPress:
    {
      if (object==this)
      {
        if (((QMouseEvent*)event)->button() != Qt::RightButton)
        {
          mousePressEvent((QMouseEvent*) event);
          return true;
        }
      }
      else
        if (((QMouseEvent*)event)->button() == Qt::LeftButton)
        {                                     //select object
                                                  // i.e. a menu
          if ( dynamic_cast <QMenu*>(object))
            return QWidget::eventFilter(object,event); 
		  p_private->p_already_selected_widget_clicked=has_already_focus(widget);
          if (!p_private->p_already_selected_widget_clicked)
            set_focus(widget,((QMouseEvent*)event)->modifiers() & Qt::ControlModifier);
          if (widget!=NULL) p_focus->show();
          p_originalposition=mapFromGlobal(((QMouseEvent*)event)->globalPos());
		  if (snap2gridx()>0 &&(!(((QMouseEvent*)event)->modifiers()&Qt::ControlModifier)))
		    p_originalposition.setX((p_originalposition.x()/snap2gridx())*snap2gridx());
		  if (snap2gridy()>0&&(!(((QMouseEvent*)event)->modifiers()&Qt::ControlModifier)))
		    p_originalposition.setY((p_originalposition.y()/snap2gridy())*snap2gridy());
          return true;
        }
        else
          if (((QMouseEvent*)event)->button() == Qt::RightButton)
          {
            mousemenu((QMouseEvent*)event);
            return true;
          }
      break;
    };
    case QEvent::MouseMove:
    {
      if ( (((QMouseEvent*)event)->buttons() &  Qt::LeftButton ) == Qt::LeftButton
           && widget!=this )
      {
//move object
        p_private->p_already_selected_widget_clicked=false;
        QPoint newpos=mapFromGlobal(((QMouseEvent*)event)->globalPos());
		if (snap2gridx()>0&&!(((QMouseEvent*)event)->modifiers() & Qt::ControlModifier))
		  newpos.setX((newpos.x()/snap2gridx())*snap2gridx());
		if (snap2gridy()>0&&!(((QMouseEvent*)event)->modifiers() & Qt::ControlModifier))
		  newpos.setY((newpos.y()/snap2gridy())*snap2gridy());
        QPoint difference=p_originalposition-newpos;
        move_widgets(difference.x(),difference.y());
        p_originalposition=newpos;
      }
      break;
    };
    case QEvent::MouseButtonDblClick: return true;
      default              :;
  }
  return QWidget::eventFilter(object,event);
}

void hk_kdesimpleform::move_widgets(int xdiff,int ydiff)
{
//move object
  QWidget* wid=p_focus->widget();
  if (!wid) return;
  QPoint buf=p_originalposition;
  hk_visible* v=dynamic_cast<hk_visible*>(wid);
  if (p_multiplefocus.size()>0)
  {
  if (v) v->set_movebuddylabel(false);
      list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
      while (it!=p_multiplefocus.end())
      {
        hk_kdeformfocus* f=(*it);
        v=dynamic_cast<hk_visible*>(f->widget());
        if (v) v->set_movebuddylabel(false);
        it++;
       }
  }

  
  p_private->p_moverect.setX(wid->x());
  p_private->p_moverect.setY(wid->y());
  p_private->p_moverect.setWidth(wid->width());
  p_private->p_moverect.setHeight(wid->height());
  if (p_multiplefocus.size()>0)
  {
      list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
      while (it!=p_multiplefocus.end())
      {
        hk_kdeformfocus* f=(*it);
        QWidget* w=f->widget();
        if (w)
        {
          if (w->x()<p_private->p_moverect.x())
             p_private->p_moverect.setX(w->x());
          if (w->y()<p_private->p_moverect.y())
             p_private->p_moverect.setY(w->y());
          
          if (w->x()+w->width()>p_private->p_moverect.x()+p_private->p_moverect.width())
             p_private->p_moverect.setWidth(w->x()+w->width()-p_private->p_moverect.x());
          if (w->y()+w->height()>p_private->p_moverect.y()+p_private->p_moverect.height())
             p_private->p_moverect.setHeight(w->y()+w->height()-p_private->p_moverect.y());
        }
        it++;
       }
  }
		QPoint difference(xdiff,ydiff);
  if ( p_private->p_moverect.x()-difference.x()<0)
		    {
		      difference.setX(p_private->p_moverect.x());
		      p_originalposition.setX(buf.x());
		    }
  if ( p_private->p_moverect.y()-difference.y()<0)
		    {
		     difference.setY(p_private->p_moverect.y());
		      p_originalposition.setY(buf.y());
		    }
  if ( p_private->p_moverect.x()+p_private->p_moverect.width()-difference.x()>(signed)designwidth())
      difference.setX(-(designwidth()-p_private->p_moverect.x()-p_private->p_moverect.width()));
  if ( p_private->p_moverect.y()+p_private->p_moverect.height()-difference.y()>(signed)designheight())
      difference.setY(-(designheight()-p_private->p_moverect.y()-p_private->p_moverect.height()));
  if (p_focus->widget()) p_focus->widget()->move(p_focus->widget()->x()-difference.x(),p_focus->widget()->y()-difference.y());
  
      list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
      while (it!=p_multiplefocus.end())
      {
        hk_kdeformfocus* f=(*it);
        QWidget* w=f->widget();
        if (w)
        {
        w->move(w->x()-difference.x(),w->y()-difference.y());
        }
        it++;
       }
  p_focus->set_positions();
  if (p_property)p_property->focus_resized();


  v=dynamic_cast<hk_visible*>(wid);
  if (p_multiplefocus.size()>0)
  {
  if (v) v->set_movebuddylabel(true);
      list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
      while (it!=p_multiplefocus.end())
      {
        hk_kdeformfocus* f=(*it);
        v=dynamic_cast<hk_visible*>(f->widget());
        if (v) v->set_movebuddylabel(true);
        it++;
       }
  }
}


void hk_kdesimpleform::widget_specific_modechanges(enum_mode s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_modechanges");
#endif
    list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();

    if(s==designmode)
    {
        setFocusPolicy(Qt::StrongFocus);
        setFocus();
        qApp->installEventFilter(this);
        show_property();
	if (p_property)
	{
        if (p_focus->widget()!=NULL)
            set_currentobject( dynamic_cast <hk_visible*> (p_focus->widget()));
        else set_currentobject(this);
	}
        while (it!=p_multiplefocus.end())
        {
            hk_kdeformfocus* f=(*it);
            f->show();
            it++;
        }
        p_focus->show();

    }
    else                                          // s!=design
    {
        qApp->removeEventFilter(this);
        while (it!=p_multiplefocus.end())
        {
            hk_kdeformfocus* f=(*it);
            f->hide();
            it++;
        }
        p_focus->hide();
        if(p_property!=NULL)
	  {
	       if (p_private->p_propertydock)
	       {
		 p_private->p_propertydock->hide();
	       }
               else
	       p_property->hide();
	  }
        setFocusPolicy(Qt::ClickFocus);
	if (!is_subform()) {
	    QWidget* w=dynamic_cast<QWidget*> (first_tabobject());

	    if (w) {
	        w->setFocus();

	        QLineEdit* e=dynamic_cast<QLineEdit*>(w);
	        if (e) e->selectAll();
	        else {
	            QTextEdit* t=dynamic_cast<QTextEdit*>(w);
	            if (t) t->selectAll();
	        }
	    }
        }
    }
    enable_actions();
}


void hk_kdesimpleform::widget_specific_after_loadform(void)
{
//     clearfocus();
//     qApp->processEvents();
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_after_loadform");
#endif
  set_mode(mode()); 
}


void hk_kdesimpleform::clearmultiplefocus(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::clearmultiplefocus");
#endif
    list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
    while (it!=p_multiplefocus.end())
    {
        hk_kdeformfocus* f=(*it);
        it++;
        if (f!=p_focus) delete f;
    }
    p_multiplefocus.erase(p_multiplefocus.begin(),p_multiplefocus.end());
}


void hk_kdesimpleform::keyPressEvent ( QKeyEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::keyPressEvent");
#endif
    switch(event->key())
    {
      case Qt::Key_Left:
            if (p_focus &&p_focus->widget()&&mode()==designmode)
            {
              hk_visible* widget=dynamic_cast<hk_visible*>(p_focus->widget());
              if (widget)
              {
               unsigned int diff=(event->modifiers()&Qt::ControlModifier?1:snap2gridx());
	       if (sizetype()==relative) diff =horizontal2relativ(diff);
         	move_widgets(diff,0);
              }

            }
	break;

      case Qt::Key_Right:
	if (p_focus &&p_focus->widget()&&mode()==designmode)
	{
	hk_visible* widget=dynamic_cast<hk_visible*>(p_focus->widget());
	if (widget)
	{
   	unsigned int diff=(event->modifiers()&Qt::ControlModifier?1:snap2gridx());
	       if (sizetype()==relative) diff =horizontal2relativ(diff);
		move_widgets(-diff,0);
	}

	}
	break;

      case Qt::Key_Up:
	if (p_focus &&p_focus->widget()&&mode()==designmode)
	{
	hk_visible* widget=dynamic_cast<hk_visible*>(p_focus->widget());
	if (widget)
	{
	unsigned int diff=(event->modifiers()&Qt::ControlModifier?1:snap2gridy());
	       if (sizetype()==relative) diff =vertical2relativ(diff);
	move_widgets(0,diff);
	}

	}
	break;

      case Qt::Key_Down:
	if (p_focus &&p_focus->widget()&&mode()==designmode)
	{
	hk_visible* widget=dynamic_cast<hk_visible*>(p_focus->widget());
	if (widget)
	{
	unsigned int diff=(event->modifiers()&Qt::ControlModifier?1:snap2gridy());
	       if (sizetype()==relative) diff =vertical2relativ(diff);
	move_widgets(0,-diff);
	}

	}
	break;

	default  :     ;
    }
    QWidget::keyPressEvent(event);

}


void hk_kdesimpleform::keyReleaseEvent ( QKeyEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::keyReleaseEvent");
#endif
//    if (event->key()==Key_Control)  p_controlbutton=false;
    QWidget::keyReleaseEvent(event);

}


void hk_kdesimpleform::clearfocus(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::clearfocus");
#endif
//     cerr <<"clear():vor p_focus->set_widget(NULL)"<<endl;
    if (p_focus!=NULL) p_focus->set_widget(NULL);
    //p_controlbutton=false;
//     cerr <<"clear():nach p_focus->set_widget(NULL)"<<endl;
    clearmultiplefocus();
//     cerr <<"clear():nach clearmultiplefocus"<<endl;
    enable_actions();
//     cerr <<"clear():nach enablea"<<endl;
    if (p_property && mode()==designmode) p_property->set_object(NULL);
	emit signal_focuswidget_changed();
}



void hk_kdesimpleform::loaddata(xmlNodePtr definition)
{
  clearfocus();
  hk_form::loaddata(definition);
}


void hk_kdesimpleform::set_focus(QWidget*f, bool p_controlbutton)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::set_focus");
#endif
    if (f==NULL || f==this)
    {
        clearfocus();
        return;
    }

        QWidget* widget=dynamic_cast<hk_kdesimpleform*>(f);
	if (widget)
	{
        widget = widget->parentWidget();
        QWidget* object =widget;
        while (object!=NULL)
        {
            if ( dynamic_cast <hk_kdegrid*>(object)!=NULL
	          ||dynamic_cast <hk_kdesubform*>(object)!=NULL
	       )
            {
                widget=object;
                object=NULL;
            }
            else    object=object->parentWidget();
        }
	if (widget) f=widget;
        }
    if (p_controlbutton==true)
    {

      if (has_already_focus(f)) return;

    }

    if (p_controlbutton==true&& p_focus->widget()!=NULL &&p_focus->widget()!=this)
    {
        p_multiplefocus.insert(p_multiplefocus.end(),p_focus);
        hk_kdeformfocus* pf=p_focus;
        p_focus = new hk_kdeformfocus(this);
        pf->repaint();
    }
    else clearmultiplefocus();
    p_focus->set_widget(f);
    enable_actions();
    hk_visible* v= dynamic_cast<hk_visible*>(f);
    if (p_property!=NULL)
    {
        QObject::connect(p_focus,SIGNAL(size_changed()),p_property,SLOT(focus_resized()));
        set_currentobject(v);
    }

  emit signal_focuswidget_changed();
}



bool hk_kdesimpleform::has_already_focus(QWidget* f)
{
if (!f) return false;
        if (p_focus->widget()==f)
        {
            return true;
        }
        list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
        while (it!=p_multiplefocus.end())         //check if widget is already set
        {
            hk_kdeformfocus* w=(*it);
            it++;
            if (w->widget()==f) return true;
        }
return false;
}



void hk_kdesimpleform::delete_widgets(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::delete_widgets");
#endif
    if (mode()!=designmode) return;

    QWidget* w=NULL;
    QWidget* del=NULL;

    list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
    while (it!=p_multiplefocus.end())
    {
        hk_kdeformfocus* f=(*it);
        it++;
        w=f->widget();
        del=w;
        f->set_widget(NULL);
        hk_kdegrid* g=dynamic_cast <hk_kdegrid*>(w);
        remove_visible(dynamic_cast <hk_visible*>(del));
        if (g)
        {
            delete g->part();
        }
        else     delete w;
    }
    w=p_focus->widget();
    del=w;
    p_focus->set_widget(NULL);
    hk_kdegrid* g=dynamic_cast <hk_kdegrid*>(w);
    remove_visible(dynamic_cast <hk_visible*>(del));
    set_currentobject(this);
    if (g) delete g->part(); else delete w;
    clearfocus();
    set_has_changed();
}

void hk_kdesimpleform::mousemenu(QMouseEvent* m)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::mousemenu");
#endif
    KAction* p_act;
    KActionCollection* p_ACol;

    if (p_popup!=NULL) return;
    p_popup=new KMenu(this);
    p_ACol = p_kdeform != NULL ? p_kdeform -> actionCollection():p_formpartwidget->actionCollection();
    if (p_multiplefocus.size()>0)
    {
      p_popup -> addAction(p_kdeform != NULL? p_resizeaction:p_formpartwidget->p_resizeaction);
      p_popup -> addAction(p_kdeform != NULL? p_alignaction:p_formpartwidget->p_alignaction);
    }
    if (p_setallaction == NULL && p_kdeform != NULL) {      
      p_setallaction = new KActionMenu(i18n("Set to all"),p_ACol);
      p_ACol -> addAction("bulk",p_setallaction);
      p_act = new KAction(i18n("Font"),p_ACol);
      p_ACol -> addAction("bulkfont",p_act);
      p_setallaction -> addAction(p_act);
      connect(p_act,SIGNAL(triggered()),this,SLOT(set_for_all(bulkfont)));
      p_act = new KAction(i18n("Foregroundcolour"),p_ACol);
      p_ACol -> addAction("bulkforeground",p_act);
      p_setallaction -> addAction(p_act);
      connect(p_act,SIGNAL(triggered()),this,SLOT(set_for_all(bulkforeground)));
      p_act = new KAction(i18n("Backgroundcolour"),p_ACol);
      p_ACol -> addAction("bulkbackground",p_act);
      p_setallaction -> addAction(p_act);
      connect(p_act,SIGNAL(triggered()),this,SLOT(set_for_all(bulkbackground)));           
    }
    p_popup -> addAction(p_kdeform != NULL? p_setallaction:p_formpartwidget->p_bulkaction);
    
#ifdef HK_DEBUG
    hk_visible* v=dynamic_cast<hk_visible*>(p_focus->widget());
    p_popup->addSeparator();
    p_act = new KAction(((v?v->wanna_debug():wanna_debug())?i18n("Don't debug"):i18n("Debug")),p_ACol);
    p_act -> setData(QVariant(99));
    p_ACol -> addAction("dbg",p_act);
    p_popup -> addAction(p_act);
    p_popup->addSeparator();
#endif

        QWidget* gridwidget=NULL;
        QWidget* object =p_focus->widget();
        while (object!=NULL)
        {
            if ( dynamic_cast <hk_kdegrid*>(object)!=NULL  )
            {
                gridwidget=object;
                object=NULL;
            }
            else    object=object->parentWidget();
        }

    
   
    if (p_focus->widget()!=NULL)
    {
      p_act = new KAction(i18n("&Delete"),p_ACol);
      p_act->setData(QVariant(13));
      p_ACol->addAction("delete",p_act);
      p_popup -> addAction(p_act);
      p_popup->addSeparator();
      p_act = new KAction(i18n("Into background"),p_ACol);
      p_act->setData(QVariant(16));
      p_ACol->addAction("into_back",p_act);
      p_popup -> addAction(p_act);
      p_act = new KAction(i18n("Into foreground"),p_ACol);
      p_act->setData(QVariant(17));
      p_ACol->addAction("into_for",p_act);
      p_popup -> addAction(p_act);
      if (gridwidget) {
        p_act = new KAction(i18n("Define columns"),p_ACol);
        p_act->setData(QVariant(18));
        p_ACol->addAction("def_cols",p_act);
        p_popup -> addAction(p_act);      
      }
      p_popup->addSeparator();
    }
    p_act = new KAction(i18n("Show &property editor"),p_ACol);
    p_act->setData(QVariant(14));
    p_ACol->addAction("show_property",p_act);
    p_popup -> addAction(p_act);
    p_act = new KAction(i18n("&Save form"),p_ACol);
    p_act->setData(QVariant(15));
    p_ACol->addAction("save_form",p_act);
    p_popup -> addAction(p_act);
    
    QAction* p_resAct = p_popup->exec( m->globalPos() );
    switch (p_resAct == NULL?0:p_resAct->data().toInt())
    {
        case 13:      delete_widgets();
        break;
       case 14:
        {
            show_property();
            break;
        }
        case 15:
        {
            save_form("",false);
            break;
        }
        case 16:
        {
            hk_visible* v=dynamic_cast<hk_visible*>(p_focus->widget());
            if (v)
            {
                v->lower_widget();
                p_focus->widget()->lower();
                p_focus->set_widget(p_focus->widget());
            }
            if (p_multiplefocus.size()>0)
            {
                list<hk_kdeformfocus*>::reverse_iterator it=p_multiplefocus.rbegin();
                while(it!=p_multiplefocus.rend())
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
            if (p_multiplefocus.size()>0)
            {
                list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
                while(it!=p_multiplefocus.end())
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

            hk_visible* v=dynamic_cast<hk_visible*>(p_focus->widget());
            if (v)
            {
                v->raise_widget();
                p_focus->widget()->raise();
                p_focus->set_widget(p_focus->widget());
            }
            break;
        }
        case 18:    {  
        		hk_kdegrid* gr=dynamic_cast<hk_kdegrid*>(gridwidget);
        		if (gr)
        		   gr->show_gridcolumndialog();
        		
        		
        	    }
        		break; 
#ifdef HK_DEBUG
        case 99: 
                    {
                    hk_visible* v=dynamic_cast<hk_visible*>(p_focus->widget());
                    if (v)
                    {
                    v->wanna_debug(!v->wanna_debug()); 
                    }
                    else
                    wanna_debug(!wanna_debug()); 
                    break;
                    }
#endif
        default:
	       ;
    }

    delete p_popup;
    p_popup=NULL;
}

void hk_kdesimpleform::show_property(void)
{
  if (mode()!=hk_presentation::designmode) return;
  bool newproperty=false;
  if (p_property==NULL)
  {
    if (p_kdeform||p_formpartwidget)
	{
	  p_property =new hk_kdeproperty(this, 0, 0);
      if (p_kdeform)
      {
        QPixmap p;
	    p_private->p_propertydock= new QDockWidget(p_kdeform);
	    p_private->p_propertydock->setWidget(p_property);
	  }
	  else
	    if (p_formpartwidget)
        {
	      QPixmap p;
	      p_private->p_propertydock=new QDockWidget(p_formpartwidget);
	      p_private->p_propertydock->setWidget(p_property);
	      p_formpartwidget->addDockWidget(Qt::RightDockWidgetArea,p_private->p_propertydock);
	    }
      newproperty=true;
    }
  }
  if (p_property!=NULL &&!masterform())
  {
     if (p_private->p_propertydock)
     {
       p_private->p_propertydock->show();
	 }
     else
       p_property->show();
     //qApp->processEvents();
     if (newproperty)
     {
	   int screenwidth=qApp->desktop()->availableGeometry(this).width();
	   int screenheight=qApp->desktop()->availableGeometry(this).height();
	   if (!p_private->p_propertydock)
	     p_property->move(screenwidth-p_property->frameGeometry().width(),
	   screenheight-p_property->frameGeometry().height());
	 }
  }
}


void hk_kdesimpleform::adjust_widgets(hk_kdesimpleform::enum_adjust s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::adjust_widgets");
#endif
    int maxw=0;
    int maxh=0;
    int minw=90000;
    int minh=90000;
    int minx=90000;
    int miny=90000;
    int maxx=0;
    int maxy=0;
    list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
    while (it!=p_multiplefocus.end())
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
    QWidget* w=p_focus->widget();
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

    it=p_multiplefocus.begin();
    while (it!=p_multiplefocus.end())
    {
        hk_kdeformfocus* f=(*it);
        it++;
        resize_widget(f->widget(),s,minw,maxw,minh,maxh,minx,maxx,miny,maxy);
    }

    resize_widget(p_focus->widget(),s,minw,maxw,minh,maxh,minx,maxx,miny,maxy);
    repaint_focus();
}


void hk_kdesimpleform::resize_widget(QWidget* w,enum_adjust s,int minw,int maxw,int minh,int maxh, int minx, int maxx, int miny, int maxy)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::resize_widget");
#endif
    switch (s)
    {
        case hk_kdesimpleform::sminw :  w->setGeometry(w->x(),w->y(),minw,w->height());
        break;
        case hk_kdesimpleform::smaxw :  w->setGeometry(w->x(),w->y(),maxw,w->height());
        break;
        case hk_kdesimpleform::sminh :  w->setGeometry(w->x(),w->y(),w->width(),minh);
        break;
        case hk_kdesimpleform::smaxh :  w->setGeometry(w->x(),w->y(),w->width(),maxh);
        break;
        case hk_kdesimpleform::smins :  w->setGeometry(w->x(),w->y(),minw,minh);
        break;
        case hk_kdesimpleform::smaxs :  w->setGeometry(w->x(),w->y(),maxw,maxh);
        break;
        case hk_kdesimpleform::al :  w->setGeometry(minx,w->y(),w->width(),w->height());
        break;
        case hk_kdesimpleform::ar :  w->setGeometry(maxx-w->width(),w->y(),w->width(),w->height());
        break;
        case hk_kdesimpleform::at :  w->setGeometry(w->x(),miny,w->width(),w->height());
        break;
        case hk_kdesimpleform::ab :  w->setGeometry(w->x(),maxy-w->height(),w->width(),w->height());
        break;
        default:;
    }

}


void hk_kdesimpleform::repaint_focus(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::repaint_focus");
#endif
    list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
    while (it!=p_multiplefocus.end())
    {
        hk_kdeformfocus* f=(*it);
        it++;
        f->set_positions();
    }
    p_focus->set_positions();
}


void hk_kdesimpleform::widget_specific_presentationdatasource(long p)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_new_presentationdatasource");
#endif
    if (p_kdeform!=NULL) p_kdeform->set_presentationdatasource(p,false);
    else
    if (p_formpartwidget!=NULL) p_formpartwidget->set_presentationdatasource(p,false);
}


void hk_kdesimpleform::set_kdeform(hk_kdeform* k)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::set_kdeform");
#endif
    p_kdeform=k;
    set_actions(p_kdeform?p_kdeform->actionCollection():NULL);
}

void hk_kdesimpleform::set_formpartwidget(hk_kdeformpartwidget* k)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::set_kdeformpartwidget");
#endif
    p_formpartwidget=k;
 //   set_actions(p_formpartwidget?p_formpartwidget->formpart()->actionCollection():NULL);
}


void hk_kdesimpleform::set_actions(KActionCollection* collection)
{
   if (collection)
    {
    KIconLoader loader (LIB_MODULE_NAME);
    p_deleteaction=createAction(i18n("&Delete"),"deleteclicked",collection,this,&hk_kdesimpleform::delete_widgets);
    p_deleteaction -> setShortcut(Qt::Key_Delete);
    p_copyaction=createAction(i18n("&Copy"),"copy",collection,this,&hk_kdesimpleform::copy);
    p_copyaction-> setShortcut(Qt::CTRL+Qt::Key_C);
    p_pasteaction=createAction(i18n("&Paste"),"paste",collection,this,&hk_kdesimpleform::paste);
    p_pasteaction->setShortcut(Qt::CTRL+Qt::Key_V);
    p_cutaction=createAction(i18n("Cu&t"),"cutclicked",collection,this,&hk_kdesimpleform::cut);
    p_cutaction->setShortcut(Qt::CTRL+Qt::Key_X);
    p_formpropertyaction=createAction(i18n("&Propertyeditor"),"viewproperty",collection,this,&hk_kdesimpleform::show_property);
    p_formpropertyaction->setIcon(QIcon(loader.iconPath("propertyeditor",KIconLoader::User)));
    
    p_resizeaction=new KActionMenu(i18n("Adjust &size"),collection);
    collection->addAction("size",p_resizeaction);
    p_minwidthaction=createAction(i18n("Minimum width"),"minwidth",collection,this,&hk_kdesimpleform::adjust_minw);
    p_maxwidthaction=createAction(i18n("Maximum width"),"maxwidth",collection,this,&hk_kdesimpleform::adjust_maxw);
    p_minheightaction=createAction(i18n("Minimum height"),"minheight",collection,this,&hk_kdesimpleform::adjust_minh);
    p_maxheightaction=createAction(i18n("Maximum height"),"maxheight",collection,this,&hk_kdesimpleform::adjust_maxh);
    p_minsizeaction=createAction(i18n("Minimum size"),"minsize",collection,this,&hk_kdesimpleform::adjust_mins);
    p_maxsizeaction=createAction(i18n("Maximum size"),"maxsize",collection,this,&hk_kdesimpleform::adjust_maxs);
    p_resizeaction->addAction(p_minwidthaction);
    p_resizeaction->addAction(p_maxwidthaction);
    p_resizeaction->addAction(p_minheightaction);
    p_resizeaction->addAction(p_maxheightaction);
    p_resizeaction->addAction(p_minsizeaction);
    p_resizeaction->addAction(p_maxsizeaction);
    p_alignaction=new KActionMenu(i18n("&Align"),collection);
    collection->addAction("align",p_alignaction);
    p_alignleftaction= createAction(i18n("&Left"),"alignleft",collection,this,&hk_kdesimpleform::align_left);
    p_alignrightaction= createAction(i18n("&Right"),"alignright",collection,this,&hk_kdesimpleform::align_right);
    p_aligntopaction= createAction(i18n("&Top"),"aligntop",collection,this,&hk_kdesimpleform::align_top);
    p_alignbottomaction = createAction(i18n("&Bottom"),"alignbottom",collection,this, &hk_kdesimpleform::align_bottom );
    p_alignaction->addAction(p_alignleftaction);
    p_alignaction->addAction(p_alignrightaction);
    p_alignaction->addAction(p_aligntopaction);
    p_alignaction->addAction(p_alignbottomaction);
    p_private->p_dbdesigneraction=createAction(i18n("Database designer"),"dbdesigner",collection,this,&hk_kdesimpleform::dbdesignaction);
    p_private->p_dbdesigneraction->setIcon(QIcon(loader.iconPath("dbdesigner",KIconLoader::User)));
   }
   else
   {
    if (p_deleteaction) unplugAll(p_deleteaction);
    if (p_copyaction) unplugAll(p_copyaction);
    if (p_pasteaction) unplugAll(p_pasteaction);
    if (p_cutaction) unplugAll(p_cutaction);
    if (p_formpropertyaction) unplugAll(p_formpropertyaction);

    if (p_resizeaction) unplugAll(p_resizeaction);
    if (p_minwidthaction) unplugAll(p_minwidthaction);
    if (p_maxwidthaction) unplugAll(p_maxwidthaction);
    if (p_minheightaction) unplugAll(p_minheightaction);
    if (p_maxheightaction) unplugAll(p_maxheightaction);
    if (p_minsizeaction) unplugAll(p_minsizeaction);
    if (p_maxsizeaction) unplugAll(p_maxsizeaction);
    if (p_alignaction) unplugAll(p_alignaction);
    if (p_alignleftaction) unplugAll(p_alignleftaction);
    if (p_alignrightaction) unplugAll(p_alignrightaction);
    if (p_aligntopaction) unplugAll(p_aligntopaction);
    if (p_alignbottomaction) unplugAll(p_alignbottomaction);
    if (p_private->p_dbdesigneraction) unplugAll(p_private->p_dbdesigneraction);

    delete p_deleteaction;
    delete p_copyaction;
    delete p_pasteaction;
    delete p_cutaction;
    delete p_formpropertyaction;

    delete p_resizeaction;
    delete p_minwidthaction;
    delete p_maxwidthaction;
    delete p_minheightaction;
    delete p_maxheightaction;
    delete p_minsizeaction;
    delete p_maxsizeaction;
    delete p_alignaction;
    delete p_alignleftaction;
    delete p_alignrightaction;
    delete p_aligntopaction;
    delete p_alignbottomaction;
    delete p_private->p_dbdesigneraction;

    p_deleteaction=NULL;
    p_copyaction=NULL;
    p_pasteaction=NULL;
    p_cutaction=NULL;
    p_formpropertyaction=NULL;

    p_resizeaction=NULL;
    p_minwidthaction=NULL;
    p_maxwidthaction=NULL;
    p_minheightaction=NULL;
    p_maxheightaction=NULL;
    p_minsizeaction=NULL;
    p_maxsizeaction=NULL;
    p_alignaction=NULL;
    p_alignleftaction=NULL;
    p_alignrightaction=NULL;
    p_aligntopaction=NULL;
    p_alignbottomaction=NULL;
    p_private->p_dbdesigneraction=NULL;

   }
    //if (p_property) delete p_property;
    //p_property=NULL;
}




bool hk_kdesimpleform::is_formobject(QWidget* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::is_formobject");
#endif
    QWidget* object =v;

    while (object!=NULL)
    {
        if (object==this) return true;
        if (object==p_property) return false;
        object=object->parentWidget();

    }

    return false;
}


void hk_kdesimpleform::create_copydata(ostream& stream,QWidget*w)
{
    hk_string vistag="FORMOBJECT";
    STRINGSTREAM tststream;

    hk_string tststring;
    if (w)
    {
        hk_visible* v = dynamic_cast<hk_visible*> (w);
        if (v)
        {
            start_mastertag(stream,vistag);
            v->savedata(stream);
            v->savedata(tststream);
            end_mastertag(stream,vistag);
        };

    };

}


void hk_kdesimpleform::copy_widgets(void)
{
  //To be fixed: if grid is selected, shortcut copy  Ctrl+C does not work, Copy item in menu must be used
    if (mode()!=designmode) return;
    if (!p_focus || !p_focus->widget()) return;
    QMimeData *mimedata = new QMimeData();
    QByteArray copydata("<DRAG>");
    STRINGSTREAM stream;

    list<hk_kdeformfocus*>::iterator it=p_multiplefocus.begin();
    while (it!=p_multiplefocus.end())
    {
        create_copydata(stream,(*it)->widget());
        it++;
    }
    if (p_focus) create_copydata(stream,p_focus->widget());
    char c;
    while (stream.get(c))
        copydata.append(c);
 
    copydata.append("</DRAG>");
    mimedata->setData(p_private->mimeCopyFormat,copydata);
    QApplication::clipboard()->setMimeData(mimedata);
}


void hk_kdesimpleform::paste_widgets(void)
{
    const QMimeData *clipmime = QApplication::clipboard()->mimeData();

    if (clipmime == NULL || !clipmime->hasFormat(p_private->mimeCopyFormat))
        return;

    clearfocus();
    hk_string def=clipmime->data(p_private->mimeCopyFormat).constData();
    xmlDocPtr doc=xmlParseMemory(def.c_str(),def.size());
    xmlNodePtr definition=xmlDocGetRootElement(doc);
    if (!definition) return;
    int i=1;
    xmlNodePtr value;
    hk_string buffer2, xxx;
   
    while ((value=get_tagvalue(definition,"FORMOBJECT",xxx,i))) {
        get_tagvalue(value,"VISIBLETYPE",buffer2);
        hk_visible* vis=new_object(buffer2);
        if (vis!=NULL) {
            vis->set_load_presentationnumber(false);
            vis->loaddata(value);
            register_object(vis);
            QWidget* w=dynamic_cast<QWidget*>(vis);
            if (w) set_focus(w,true);
            
        }
        i++;        
    } 
}


void hk_kdesimpleform::cut_widgets(void)
{
    copy_widgets();
    delete_widgets();

}

void hk_kdesimpleform::enable_actions(void)
{
    if (mode()==viewmode)
    {
        if (p_copyaction)p_copyaction->setEnabled(true);
        if (p_cutaction) p_cutaction->setEnabled(false);
        if (p_pasteaction) p_pasteaction->setEnabled(true);
        if (p_deleteaction) p_deleteaction->setEnabled(false);
	if (p_private->p_dbdesigneraction) p_private->p_dbdesigneraction->setEnabled(false);
    }
    else
    {                                             //designmode
        if (p_copyaction)p_copyaction->setEnabled(p_focus&&p_focus->widget());
        if (p_cutaction)p_cutaction->setEnabled(p_focus&&p_focus->widget());
        if (p_deleteaction)p_deleteaction->setEnabled(p_focus&&p_focus->widget());
        if (p_pasteaction) 
            p_pasteaction->setEnabled(QApplication::clipboard()->mimeData()&&
            QApplication::clipboard()->mimeData()->hasFormat(p_private->mimeCopyFormat.toAscii()));
	if (p_private->p_dbdesigneraction) p_private->p_dbdesigneraction->setEnabled(true);
    }
    if (p_resizeaction) p_resizeaction->setEnabled(mode()==designmode&&p_multiplefocus.size()>0);
    if (p_alignaction) p_alignaction->setEnabled(mode()==designmode&&p_multiplefocus.size()>0);
}


bool    hk_kdesimpleform::set_mode(enum_mode s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::set_mode");
#endif
    if (p_private->while_modechange)
        return true;
    interpreter()->set_block_execution(s==filtermode);
    p_private->while_modechange=true;
    bool r=hk_form::set_mode(s);
    if (!r) {  
        p_private->while_modechange=false;
        return false;
    }
    QPalette pl(palette());
    pl.setColor(backgroundRole(),p_private->p_filtercolourbuffer);
    setPalette(pl);
    if (s==hk_presentation::viewmode) {
        setEnabled(is_enabled());
        if (p_formpropertyaction) p_formpropertyaction->setEnabled(false);
        if (p_private->p_designer) {
            delete p_private->p_designer;
            p_private->p_designer=NULL;
        }
        repaint();
    } else {
        setEnabled(true);
        if (s==filtermode) {
            QPalette p = palette();
            p.setColor(backgroundRole(), Qt::yellow);
            setPalette(p);
            if (p_formpropertyaction) p_formpropertyaction->setEnabled(false);
            if (p_private->p_designer) {
                delete p_private->p_designer;
                p_private->p_designer=NULL;
            }
        }else {//designmode
            if (p_property)p_property->setEnabled(true);
            if(p_formpropertyaction) p_formpropertyaction->setEnabled(true); 
        }
    }
    if (p_kdeform)
        p_kdeform->set_mode(s);
    else
        if (p_formpartwidget) p_formpartwidget->set_mode(s);
    p_private->while_modechange=false;
    return r;
}


void hk_kdesimpleform::align_left(void)
{
    adjust_widgets(al);
}


void hk_kdesimpleform::align_right(void)
{
    adjust_widgets(ar);
}


void hk_kdesimpleform::align_top(void)
{
    adjust_widgets(at);
}


void hk_kdesimpleform::align_bottom(void)
{
    adjust_widgets(ab);
}


void hk_kdesimpleform::adjust_minw(void)
{
    adjust_widgets(sminw);
}


void hk_kdesimpleform::adjust_maxw(void)
{
    adjust_widgets(smaxw);
}


void hk_kdesimpleform::adjust_minh(void)
{
    adjust_widgets(sminh);
}


void hk_kdesimpleform::adjust_maxh(void)
{
    adjust_widgets(smaxh);
}


void hk_kdesimpleform::adjust_mins(void)
{
    adjust_widgets(smins);
}


void hk_kdesimpleform::adjust_maxs(void)
{
    adjust_widgets(smaxs);
}


void    hk_kdesimpleform::widget_specific_backgroundcolour_changed(const hk_colour& oldcolour)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_backgroundcolour_changed");
#endif  
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p=palette();
    p.setColor(QPalette::Button,newcolour);
    p.setColor(backgroundRole(),newcolour);
    setPalette(p);
    
    list<hk_visible*>::iterator it=visibles()->begin();
    p_private->p_filtercolourbuffer=newcolour;
    while (it!=visibles()->end())
    {
        switch ((*it)->type())
        {
            case textlabel :;
            case boolean : if ((*it)->backgroundcolour()==oldcolour)
            (*it)->set_backgroundcolour(backgroundcolour(),false);
            break;
            default:;

        }

        ++it;
    }
}


void    hk_kdesimpleform::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    p.setColor(foregroundRole(), newcolour);
    setPalette(p);
}


void hk_kdesimpleform::set_for_all(enum_bulkoperation bulk)
{
    if (p_focus)
    {
        hk_visible* v= dynamic_cast<hk_visible*>(p_focus->widget());
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

hk_kdeproperty* hk_kdesimpleform::propertyeditor(void)
{
   return p_property;
}


hk_kdeform* hk_kdesimpleform::kdeform() const
{
   return p_kdeform;
}

hk_kdeformpartwidget* hk_kdesimpleform::formpartwidget(void) const
{

return  p_formpartwidget;
}

void hk_kdesimpleform::script_error(hk_visible* v, hk_interpreter::enum_action a)
{
  if (!v)
  {
    cerr<<"hk_kdesimpleform::script_error called without visible object!"<<endl;
    return;
  }

  p_while_errorhandling=true;

  hk_string error=replace_all("%LINENUMBER%",hk_translate("Line %LINENUMBER%: "),longint2string(interpreter()->error_rownumber()))
    				+interpreter()->errormessage();

  hk_dsgridcolumn* gc=dynamic_cast<hk_dsgridcolumn*>(v);
  if (gc)
  {
    error=replace_all("%1",hk_translate("Gridcolumn '%1':\n"),gc->columnname())+error;
  }

  hk_dsvisible* dv=dynamic_cast<hk_dsvisible*>(v);
  if (dv)
  {
    hk_datasource* ds=dv->datasource();
	if (ds) ds->set_ignore_changed_data();
  }

  if (!runtime_only())
  {
    if (!gc||(gc &&!a==hk_interpreter::a_on_key))
    {
      if (a==hk_interpreter::a_on_open) p_private->while_modechange=false;
      if (kdeform()) kdeform()->set_mode(hk_presentation::designmode);
      else  
	    set_mode(hk_presentation::designmode);
    }
    else
    {
      show_warningmessage(error);
      return;
    }
    if (p_property)
	{
	  if (mode()==hk_form::designmode)
	    set_focus(dynamic_cast<QWidget*>(v),false);
	  set_currentobject(v);
	  switch (a)
	  {
	    case hk_interpreter::a_click :
		  p_property->pushactionbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
		  break;
		case hk_interpreter::a_doubleclick :
		  p_property->doubleclickactionbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
		  break;
		case hk_interpreter::a_on_open :
		  p_property->openactionbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
		  break;
		case hk_interpreter::a_on_close :
		  p_property->closeactionbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
		  break;
		case hk_interpreter::a_on_getfocus :
		  p_property->ongetfocusbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
		  break;
		case hk_interpreter::a_on_loosefocus :
		  p_property->onloosefocusbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
		  break;
		case hk_interpreter::a_on_key :
		  p_property->onkeybutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
		  break;
		case hk_interpreter::a_after_row_change :
		  p_property->afterrowchangebutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
		  break;
		case hk_interpreter::a_before_row_change :
		  p_property->beforerowchangebutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
		  break;
		case hk_interpreter::a_after_update :
				p_property->afterupdatebutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
				break;
		case hk_interpreter::a_before_update :
				p_property->beforeupdatebutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
				break;
		case hk_interpreter::a_before_delete :
				p_property->beforedeletebutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
				break;
		case hk_interpreter::a_after_delete :
				p_property->afterdeletebutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
				break;
		case hk_interpreter::a_before_insert :
				p_property->beforeinsertbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
				break;
		case hk_interpreter::a_after_insert :
				p_property->afterinsertbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
				break;
		case hk_interpreter::a_on_select :
				p_property->onselectbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
				break;
		case hk_interpreter::a_on_valuechanged :
				p_property->onvaluechangedbutton_clicked(presentation()->interpreter()->error_rownumber()-1,error);
				break;
			default:;

	    }
      } else 
        show_warningmessage(hk_string("No Property editor: ") + error);

   }
   else
     show_warningmessage(error);
}

/*bool hk_kdesimpleform::ctrl_key_pressed(void) const
{
return p_controlbutton;
}*/

void hk_kdesimpleform::grid_partinfocus(hk_kdegridpart* p)
{
  if (p_kdeform) p_kdeform->grid_partinfocus(p);
  else
  if (p_formpartwidget) p_formpartwidget->grid_partinfocus(p);
}

void hk_kdesimpleform::grid_partoutfocus(hk_kdegridpart* g,QFocusEvent* e)
{
  if (g)
  {
    hk_kdegrid* grid=g->grid();
    if (e->reason()==Qt::TabFocusReason || e->reason()==Qt::BacktabFocusReason )
    {
      QWidget* w=dynamic_cast<QWidget*>(e->reason()==Qt::BacktabFocusReason
  		?taborder_previous(grid) :taborder_next(grid));

      if (w) w->setFocus();
    }
  }

  if (p_kdeform) p_kdeform->grid_partoutfocus(0L);
  else
    if (p_formpartwidget)p_formpartwidget->grid_partoutfocus(0L);
   
/*p_copyaction->setEnabled(false);
p_pasteaction->setEnabled(false);*/
}

void hk_kdesimpleform::copy(void)
{
if (mode()==designmode)
  {
    copy_widgets();
    return;
  }

  hk_kdelineedit* l=dynamic_cast<hk_kdelineedit*>(focusWidget());
  if (l)  l->copy();
  else
  {
   hk_kde4simplegrid* g=dynamic_cast<hk_kde4simplegrid*>(p_private->focusChild());
   if (g) 
     g->copy();
   else
      {
       hk_kdecombobox* c=dynamic_cast<hk_kdecombobox*>(focusWidget());
       if (c &&c->lineEdit()) c->lineEdit()->copy();
      }
  }

}

void hk_kdesimpleform::paste(void)
{
if (mode()==designmode)
  {
    paste_widgets();
    return;
  } 
  hk_kdelineedit* l=dynamic_cast<hk_kdelineedit*>(focusWidget());
  if (l)  l->paste();
  else
  {
   hk_kde4simplegrid* g=dynamic_cast<hk_kde4simplegrid*>(p_private->focusChild());
   if (g) g->paste();
     else
      {
       hk_kdecombobox* c=dynamic_cast<hk_kdecombobox*>(focusWidget());
       if (c &&c->lineEdit()) c->lineEdit()->paste();
      }
  }
}

void hk_kdesimpleform::cut(void)
{
if (mode()==designmode)
  {
    cut_widgets();
    return;
  }
  // never comes here as cut action is disabled in enable_actions() for view mode
  hk_kdelineedit* l=dynamic_cast<hk_kdelineedit*>(focusWidget());
  if (l)  l->cut();
  else
  {
   hk_kde4simplegrid* g=dynamic_cast<hk_kde4simplegrid*>(p_private->focusChild());

   if (g) g->cut();
     else
      {
       hk_kdecombobox* c=dynamic_cast<hk_kdecombobox*>(focusWidget());
       if (c &&c->lineEdit()) c->lineEdit()->cut();
      }
  }

}


void hk_kdesimpleform::dbdesignaction(void)
{
 /*TBP    if (p_private->p_designer)
     	{
	 p_private->p_designer->raise();
	 return;
	}

    p_private->p_designer=new hk_kdedbdesignerwindow();
    connect (p_private->p_designer,SIGNAL(signal_closed()),this,SLOT(designer_deleted()));
    p_private->p_designer->designer()->set_database(database());
    p_private->p_designer->designer()->set_presentation(this);
    set_block_has_changed(true);
    p_private->p_designer->show();
    set_block_has_changed(false); */
}

void hk_kdesimpleform::designer_deleted(void)
{
  set_currentobject(p_property->object());
  p_private->p_designer=NULL;
}

bool hk_kdesimpleform::focus_multipleselected(void) const
{
return p_multiplefocus.size()>0;
}


void hk_kdesimpleform::set_has_changed(enum_has_changed forcesetting)
{
hk_form::set_has_changed(forcesetting);
emit signal_has_changed();
}



void hk_kdesimpleform::reset_has_changed(void)
{
  hk_form::reset_has_changed();
emit signal_has_changed();
}

void hk_kdesimpleform::load_form(const hk_string& name)
{
  p_private -> while_loading = true;
  hk_form::load_form(name);
  if (p_formpartwidget) p_formpartwidget->set_caption();   
  p_private->while_loading = false;
}

bool hk_kdesimpleform::focusNextPrevChild(bool forward)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::focusNextPrevChild");
#endif  
  hk_visible* v=NULL;
  QWidget* wid=p_private->focusChild();
  
  while (!v && wid) {
    v=dynamic_cast<hk_visible*>(wid);
    hk_kde4simplegrid* grid=dynamic_cast<hk_kde4simplegrid*>(wid);

    if (grid) {
      v=grid->kdegrid();
    }
    else
      wid=wid->parentWidget();
  }
  if (!v) {
     cerr <<"hk_kdesimpleform::focusNextPrevChild focusData kein hk_visible objekt!"<<endl;
     return false;
  }
  if (forward)
    v=taborder_next(v);
  else
    v=taborder_previous(v);

  if (!v) {
    cerr <<"hk_kdesimpleform::focusNextPrevChild taborder_next/previsous gab NULL zurck!"<<endl;
    return false;
  }
  QWidget* w=dynamic_cast<QWidget*>(v);
  if (w) {
   w->setFocus();   
  } 
  return true;
}


void hk_kdesimpleform::goto_taborder_next()
{
focusNextPrevChild(true);
}

void hk_kdesimpleform::goto_taborder_previous()
{
focusNextPrevChild(false);
}

void hk_kdesimpleform::goto_taborder_first()
{
  QWidget* w=dynamic_cast<QWidget*>(first_tabobject());

  if (w)
   {
   w->setFocus();
   }

}

void hk_kdesimpleform::goto_taborder_last()
{
  QWidget* w=dynamic_cast<QWidget*>(last_tabobject());
  if (w)
   {
   w->setFocus();
   }
}

void hk_kdesimpleform::set_focus(hk_visible* v)
{
  if (!v) return;
  QWidget* w=dynamic_cast<QWidget*>(v);
  if (w) w->setFocus();
}



void  hk_kdesimpleform::widget_specific_tooltip_changed(void)
{
    setToolTip(QString::fromUtf8 (l2u(tooltip()).c_str()));
}




void hk_kdesimpleform::focusInEvent ( QFocusEvent * e)
{
  action_on_getfocus();
  QWidget::focusInEvent(e);
}

void hk_kdesimpleform::focusOutEvent ( QFocusEvent * e)
{
  action_on_loosefocus();
  QWidget::focusOutEvent(e);

}





bool hk_kdesimpleform::show_asdialog(hk_form* parentform)
{
   if ( p_private->eventloop != NULL ) {
	qWarning( "hk_kdesimpleform::exec: Recursive call detected." );
	return false;
    }
    hk_kdesimpleform* pf=dynamic_cast<hk_kdesimpleform*>(parentform);
    if (pf) {
      setWindowFlags(Qt::Dialog);
      setWindowModality(Qt::ApplicationModal);
      setParent(pf);
      move(pos());
    }
    setAttribute(Qt::WA_DeleteOnClose,false);

    Qt::WindowModality wModality = windowModality();
    setWindowModality( Qt::ApplicationModal );

    show();
    QEventLoop eventloop;
    p_private->eventloop = &eventloop;
    eventloop.exec();

    if ( (wModality & Qt::ApplicationModal) == 0 )
	setWindowModality( wModality );
    
  return true;
}


void hk_kdesimpleform::done( void )
{
    list<hk_datasource*>* l=datasources();
    list<hk_datasource*>::iterator it=l->begin();
    bool store_ok=true;
    while(it!=l->end()&& store_ok)
    {
    if ((*it)->has_changed())
      store_ok=(*it)->store_changed_data();
    ++it;
    }

    if (store_ok) hide();
    close();
}

void hk_kdesimpleform::closeEvent ( QCloseEvent* e)
{
 if (p_private->eventloop != NULL)
   done();
 if (isHidden())e->accept();
 else e->ignore();
}


void hk_kdesimpleform::hide()
{
    if ( isHidden() )
	return;

    QWidget::hide();
    if (p_private->eventloop != NULL) {
        QEventLoop* tmpLoop = p_private->eventloop; 
	p_private->eventloop = NULL;
	tmpLoop->quit();
    }
}


bool hk_kdesimpleform::widget_specific_coordinates(uint px,uint py,uint ,uint )
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesimpleform::widget_specific_coordinates");
#endif
    if (p_kdeform ||p_formpartwidget) return true;
    move(px,py);
    return true;
}


void hk_kdesimpleform::show_widget(void)
{
  show();
}

void hk_kdesimpleform::hide_widget(void)
{
  hide();
}

bool hk_kdesimpleform::close_widget(void)
{

  if (p_formpartwidget) return p_formpartwidget->close();
  else if (p_kdeform) return p_kdeform->close();
  else
  return close();
}

void hk_kdesimpleform::set_create_with_label(bool w)
{
p_private->p_with_label=w;
}

bool hk_kdesimpleform::create_with_label(void) const
{
  return p_private->p_with_label;
}

void hk_kdesimpleform::set_currentobject(hk_visible* o)
{
if (!p_property) return;
 p_property->set_object(o);

}


void hk_kdesimpleform::widget_specific_enabled_changed(void)
{
}

int hk_kdesimpleform::screen_width()
{
  return QApplication::desktop()->screenGeometry(this).width();
}

int hk_kdesimpleform::screen_height()
{
  return QApplication::desktop()->screenGeometry(this).height();
}


bool hk_kdesimpleform::intervaltasks(void)
{
cerr <<"hk_kdesimpleform::intervaltasks"<<endl;
 cerr << "pendingevents:" << qApp->hasPendingEvents() << " time:"
 << QTime::currentTime().toString().toStdString() << endl;
 cerr <<"NACH processEvents()"<<endl;
return true;
}


void hk_kdesimpleform::widget_specific_set_name(void)
{
  emit signal_name_changed();
}
