// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for hk_kde5classes library
//
// This file is part of the hk_kde5classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_kdeform.h"
#include "hk_kdereport.h"
#include "hk_kdereportpartwidget.h"
#include "hk_kdesimplereport.h"
#include "hk_kdesimpleform.h"
#include "hk_kdeform.h"
#include "hk_kdeformpartwidget.h"
#include "hk_kdebutton.h"

#include <hk_database.h>
#include <hk_dsquery.h>
#include <hk_dstable.h>
#include <hk_report.h>
#include <hk_presentation.h>
#include <hk_interpreter.h>

#include <ktabwidget.h>
#include <kdebug.h>

#include <qwidget.h>
#include <qapplication.h>
#include <qtooltip.h>
#include <QMouseEvent>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

hk_kdebutton::hk_kdebutton(QWidget* wid,hk_form* form)
:QPushButton(wid),
hk_button(form)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdebutton::hk_kdebutton");
#endif

    QColor qcolor=palette().color(backgroundRole());
    hk_colour colour(qcolor.red(),qcolor.green(),qcolor.red());
    set_backgroundcolour(colour,false,true);
    qcolor=palette().color(foregroundRole());
    colour.set_colour(qcolor.red(),qcolor.green(),qcolor.red());
    set_foregroundcolour(colour,false,true);
    setDefault(true);

    QObject::connect(this,SIGNAL(clicked()),this,SLOT(button_clicked()));
    QObject::connect(this,SIGNAL(toggled(bool)),this,SLOT(toggle_state_changed(bool)));
}


hk_kdebutton::~hk_kdebutton()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdebutton::~hk_kdebutton");
#endif

}


bool hk_kdebutton::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
    blockSignals(true);
    setGeometry(px,py,pwidth,pheight);
    blockSignals(false);
    return true;
}


bool hk_kdebutton::widget_specific_open_form(void)
{
#ifdef HK_DEBUG
  hkdebug("hk_kdebutton::widget_specific_open_form");
#endif
// cerr <<"hk_kdebutton::widget_specific_open_form"<<endl;
  hk_kdeform* w=find_existing_form();
  hk_kdeformpartwidget* f=find_existing_formpart();
  hk_kdesimpleform* s=NULL;


  if (p_presentation && p_presentation->name()!=object()) {
    if (w==NULL && f==NULL) /* not open yet*/ {
      if (p_presentation!=NULL)
        s=((hk_kdesimpleform*)p_presentation->database()->new_formvisible());
      if (s) {
        if (p_presentation!=NULL) s->set_database(p_presentation->database());
        else s->set_database(database());
        s->load_form(object());
        //s->set_designmode();
        s->enable_datasources(false);
        hk_string c=parsed_condition(s);
        if (c.size()>0) {
          if (s &&s->datasource()) {
	    s->datasource()->set_internalfilter(c);
            s->datasource()->set_use_internalfilter(true);  
	  }
        }
        s->set_viewmode();  
        
        hk_form* fo=dynamic_cast<hk_form*>(p_presentation);
        if (fo) {
//            if (s)cerr <<s->hk_form::name()<<" fo->set_caller_form: "<<fo->name()<<endl;
          if (s && s->formpartwidget()) s->formpartwidget()->set_caller_form(fo->name());
        }
//  else cerr <<"hk_kdebutton: fo ==NULL "<<endl;

	//s->enable_datasources(true);
      }
    }
    else {
      if (w)w->hide();
    }

    if (w||f) {
      if (w) s=w->simpleform();
      else
      s=f->simpleform();

      s->load_form(object());
      s->set_designmode();
      hk_string c=parsed_condition(s);
      if (c.size()>0) {
        if (s &&s->datasource()) {
	    s->datasource()->set_internalfilter(c);
            s->datasource()->set_use_internalfilter(true);
	}

      }
      s->set_viewmode();
    }
  }   // end p_presentation->name()!=object()

 
  if (w) {
    if (show_maximized()) w->showMaximized();
    else w->show();
  }
  else {
    if (f) {
      //to get tabwidget, we need parent of parent
      KTabWidget* view = dynamic_cast<KTabWidget*>(f->parent()!=NULL?f->parent()->parent():NULL);
      if (view){
	view->setCurrentWidget(f);
        return true;
      }
      else {
        if (show_maximized())f->showMaximized();
        else f->show();
      }
    }

    if (s) s->reset_has_changed();

   }
   return true;
}


bool hk_kdebutton::widget_specific_close_form(void)
{
#ifdef HK_DEBUG
  hkdebug("hk_kdebutton::widget_specific_close_form");
#endif
  if (object().size()==0){
    // object not defined means close parent form
    QWidget* w=parentWidget();
    if (w) /* first check if the parent is hk_kdeform and then close it*/ {
      QWidget* p=w;
      while (p) {
	hk_kdeform* f= dynamic_cast<hk_kdeform*>(p);
	p=p->parentWidget();
	if (f) {
	  f->close();
	  return true;
	}
	
      }

    }
    hk_form* f=dynamic_cast<hk_form*>(p_presentation);

    if (f) {
      while (f->masterform())
        f=f->masterform();

        if (((hk_kdesimpleform*)(f))->formpartwidget()) {
          ((hk_kdesimpleform*)(f))->formpartwidget()->close();
          return true;
        }
        else ((hk_kdesimpleform*)(f))->close();

    }
    if (w) {
      w->close();
      return true;
    }
    else return false;
  }

  hk_kdeform* w=find_existing_form();
  hk_kdeformpartwidget* f=find_existing_formpart();

  if (w) w->close();
  if (f) f->close(); 
  return true;
}


void hk_kdebutton::button_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdebutton::button_clicked");
#endif

   push_action();

}


void    hk_kdebutton::widget_specific_label_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdebutton::widget_specific_label_changed");
#endif
//     cerr <<"widget_specific_label_changed:"<<label()<<endl;
    setText(QString::fromUtf8 (l2u(label()).c_str()));
    show_icon();
}


void hk_kdebutton::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("hk_kdebutton::resizeEvent");
#endif
    QPushButton::resizeEvent(event);
    reposition();

}


void  hk_kdebutton::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("kdebutton::resizeEvent");
#endif
    QPushButton::moveEvent(event);
    reposition();
}


void hk_kdebutton::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("kdebutton::reposition");
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


void hk_kdebutton::widget_specific_font_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdebutton::widget_specific_font_changed");
#endif
    setFont(QFont(QString::fromUtf8 (l2u(hk_button::font().fontname()).c_str())
                  ,hk_button::font().fontsize(),hk_button::font().bold()?QFont::Bold:QFont::Normal,
		   hk_button::font().italic()));
}


hk_kdeform* hk_kdebutton::find_existing_form(void)
{
   hk_kdeform* w = NULL;
    if (p_presentation->database()!=NULL)         // is the form already open?
    {
        QWidget* sf=dynamic_cast<QWidget*>
            (p_presentation->database()->existing_form(object()));
        while (sf!=NULL && w==NULL)
        {
            sf=sf->parentWidget();
	    hk_form* f=dynamic_cast<hk_form*>(sf);
	    if (f && f->is_subform())sf=NULL;
            w= dynamic_cast <hk_kdeform*> (sf);
        }
    }
    return w;
}


hk_kdeformpartwidget* hk_kdebutton::find_existing_formpart(void)
{
    hk_kdeformpartwidget* w = NULL;
    if (p_presentation->database()!=NULL)         // is the form already open?
    {
        QWidget* sf=dynamic_cast<QWidget*>
            (p_presentation->database()->existing_form(object()));
        while (sf!=NULL && w==NULL)
        {
            sf=sf->parentWidget();
	    hk_form* f=dynamic_cast<hk_form*>(sf);
	    if (f && f->is_subform())sf=NULL;
            w= dynamic_cast <hk_kdeformpartwidget*> (sf);
        }
    }
    return w;
}


hk_kdereport* hk_kdebutton::find_existing_report(void)
{
   hk_kdereport* w = NULL;
    if (p_presentation->database()!=NULL)         // is the report already open?
    {
        QWidget* sf=dynamic_cast<QWidget*>
            (p_presentation->database()->existing_report(object()));
        while (sf!=NULL && w==NULL)
        {
            sf=sf->parentWidget();
            hk_report* r=dynamic_cast<hk_report*>(sf);
            if (r && r->is_subreport())sf=NULL;
            w= dynamic_cast <hk_kdereport*> (sf);
        }
    }
    return w;
}


hk_kdereportpartwidget* hk_kdebutton::find_existing_reportpart(void)
{
  hk_kdereportpartwidget* w = NULL;
    if (p_presentation->database()!=NULL)         // is the report already open?
    {
        QWidget* sf=dynamic_cast<QWidget*>
            (p_presentation->database()->existing_report(object()));

        while (sf!=NULL && w==NULL)
        {
            sf=sf->parentWidget();
	    hk_report* r=dynamic_cast<hk_report*>(sf);
	    if (r && r->is_subreport())sf=NULL;
            w= dynamic_cast <hk_kdereportpartwidget*> (sf);
        }
    }
    return w;
}


bool hk_kdebutton::widget_specific_open_table(void)
{
    if (p_presentation==NULL) return false;
    if (p_presentation->database()!=NULL)
    {
        hk_dstable* w=p_presentation->database()->new_tablevisible();
        hk_datasource* d=p_presentation->database()->new_table(object());
        w->set_datasource(d);
	QWidget* wid=(QWidget*) w;
        if (show_maximized())wid->showMaximized();
        else wid->show();
        w->set_viewmode();
    }
    return true;
}


bool hk_kdebutton::widget_specific_open_query(void)
{
    if (p_presentation==NULL) return false;
    if (p_presentation->database()!=NULL)
    {
        hk_dsquery* w=p_presentation->database()->new_queryvisible();
        hk_datasource* d=p_presentation->database()->new_resultquery();
        d->set_name(object());
        w->set_datasource(d);
        w->load_query();
        w->set_viewmode();
        if (show_maximized())((QWidget*)w)->showMaximized();
        else ((QWidget*)w)->show();
    }
    return true;
}


bool hk_kdebutton::widget_specific_open_view(void)
{
    if (p_presentation==NULL) return false;
    if (p_presentation->database()!=NULL)
    {
        hk_dsquery* w=p_presentation->database()->new_queryvisible();
        hk_datasource* d=p_presentation->database()->new_view();
        d->set_name(object());
        w->set_datasource(d);
	w->set_use_qbe(false,false);
        w->set_viewmode();
        if (show_maximized())((QWidget*)w)->showMaximized();
        else ((QWidget*)w)->show();
    }
    return true;
}


bool    hk_kdebutton::widget_specific_preview_report(void)
{ 
  hk_kdereport* w=find_existing_report();
  hk_kdereportpartwidget* f=find_existing_reportpart();
  if (w||f) {
    KTabWidget* view =NULL;
    if (w) view=dynamic_cast<KTabWidget*>(w->parent()!=NULL?w->parent()->parent():NULL);
    if (f) view=dynamic_cast<KTabWidget*>(f->parent()!=NULL?f->parent()->parent():NULL);
    if (view) {
      bool in_execute=false;
      if (w) in_execute=w->simplereport()->while_executing();
      else
	if (f) in_execute=f->simplereport()->while_executing();
      if (in_execute) return true;
      if (w) w->set_designmode();
      if (f) f->set_designmode();
      hk_string c=parsed_condition(w?w->simplereport():f->simplereport());
      if (c.size()>0) {
        if (w &&w->simplereport()->datasource()) {
	  w->simplereport()->datasource()->set_internalfilter(c);
          w->simplereport()->datasource()->set_use_internalfilter(true);
	}
	if (f &&f->simplereport()->datasource()) {
	  f->simplereport()->datasource()->set_internalfilter(c);
          f->simplereport()->datasource()->set_use_internalfilter(true);
	}

      }
      if (w) {
	w->set_viewmode();
	view->setCurrentWidget(w);
      }
      if (f) {
	f->set_viewmode();
	view->setCurrentWidget(f);
      }
      return true;
    }
    else return false;
  }

  if (!w&&!f) {                                 // not open yet
    hk_kdesimplereport* r=((hk_kdesimplereport*)p_presentation->database()->new_reportvisible());
    if (!r) {
      show_warningmessage("r==NULL!");
      return false;
    }

    if (p_presentation!=NULL) r->set_database(p_presentation->database());
    else r->set_database(database());
    r->load_report(object());
    r->reset_has_changed();
    hk_string c=parsed_condition(r);
    if (c.size()>0&&r->datasource()) {
      r->datasource()->set_internalfilter(c);
      r->datasource()->set_use_internalfilter(true);
    }
    r->set_viewmode();
    hk_form* fo=dynamic_cast<hk_form*>(p_presentation);
    if (fo) {
//            if (r)cerr <<r->hk_report::name()<<" fo->set_caller_form: "<<fo->name()<<endl;
      if (r && r->reportpartwidget()) r->reportpartwidget()->set_caller_form(fo->name());
    }
  }
  else {
    if (w) w->hide();
  }

  if (w) {
    if (show_maximized()) w->showMaximized();
    else w->show();
  }
  else {
    if (f) {
      KTabWidget* view=dynamic_cast<KTabWidget*>(f->parent()!=NULL?f->parent()->parent():NULL);
      if (view) {
	 view->setCurrentWidget(f);
         return true;
      }
      else {
	if (show_maximized()) f->showMaximized();
        else f->show();
      }
    }
  }
// cerr <<"PREVIEW END"<<endl;
  return true;
}





/*bool    hk_kdebutton::widget_specific_close_application(void)
{
qApp->quit();
return true;
}*/


void    hk_kdebutton::widget_specific_backgroundcolour_changed(const hk_colour&)
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(backgroundRole(),newcolour);
    setPalette(p);
}


void    hk_kdebutton::widget_specific_foregroundcolour_changed(const hk_colour& )
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(QPalette::ButtonText,newcolour);
    setPalette(p);
}




void  hk_kdebutton::mouseDoubleClickEvent(QMouseEvent* event)
{
   QPushButton::mouseDoubleClickEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_doubleclick();
}



void  hk_kdebutton::widget_specific_tooltip_changed(void)
{
    setToolTip(QString::fromUtf8 (l2u(tooltip()).c_str()));
}




void hk_kdebutton::focusInEvent ( QFocusEvent * e)
{
  action_on_getfocus();
  QPushButton::focusInEvent(e);
}

void hk_kdebutton::focusOutEvent ( QFocusEvent * e)
{
  action_on_loosefocus();
  QPushButton::focusOutEvent(e);

}


void hk_kdebutton::show_widget(void)
{
  show();
}

void hk_kdebutton::hide_widget(void)
{
  hide();
}

bool hk_kdebutton::close_widget(void)
{
  return close();
}

bool hk_kdebutton::load_icon(const hk_string& filename,bool registerchange)
{  //cerr<<"load_icon"<<endl;
  bool result=hk_button::load_icon(filename,registerchange);
  if (result)
  {
   show_icon();
  }
  return result;

}

void hk_kdebutton::show_icon()
{ //cerr <<"show_icon"<<endl;
if (!hk_button::icon()->data) return;
   QPixmap p_pixmap;
   if (p_pixmap.loadFromData((const uchar*)hk_button::icon()->data,(uint)hk_button::icon()->length))
       setIcon(p_pixmap);
   else
     {
       hk_string d;
       data2hex(hk_button::icon(),d);
//        cerr <<d<<endl;
     }

}


void hk_kdebutton::widget_specific_enabled_changed(void)
{
  setEnabled(is_enabled());
}

bool hk_kdebutton::presentationmode_changed()
{
if (!p_presentation)
  {
    setEnabled(is_enabled());
    return hk_button::presentationmode_changed();
  }
bool result=hk_button::presentationmode_changed();
   if (p_presentation->mode()==hk_presentation::filtermode)
   setEnabled(false);
   else
   setEnabled(is_enabled());
   if (is_togglebutton())set_is_pushed(is_pushed(),false,false);
  return  result;
}


void hk_kdebutton::set_is_togglebutton(bool t,bool registerchange,bool forcesetting)
{
 if (!t)set_is_pushed(false,registerchange,forcesetting);
 setCheckable(t);
 hk_button::set_is_togglebutton(t,registerchange,forcesetting);
}

void hk_kdebutton::set_is_pushed(bool p,bool registerchange,bool forcesetting)
{
 hk_button::set_is_pushed(p,registerchange,forcesetting);
 blockSignals(true);
 setChecked(p);
 blockSignals(false);

}


void hk_kdebutton::toggle_state_changed(bool state)
{
 set_is_pushed(state);
}





