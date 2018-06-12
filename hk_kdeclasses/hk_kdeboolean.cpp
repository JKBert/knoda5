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
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.56 $
#include <hk_datasource.h>

#include "hk_kdeboolean.h"
#include <hk_kdeboolean.moc>
#include <QKeyEvent>
#include <qtooltip.h>
#include <hk_form.h>
#include <hk_dsgridcolumn.h>
#include <klocale.h>

hk_kdeboolean::hk_kdeboolean(QWidget* wid,hk_form* form)
:QCheckBox(wid),
hk_dsboolean(form)

{
#ifdef HK_DEBUG
    hkdebug("hk_kdeboolean::hk_kdeboolean");
//     wanna_debug(true);
#endif
    QObject::connect(this,SIGNAL(clicked()),this,SLOT(slot_data_changed()));
    p_widget_specific_row_change=true;

    QColor qcolor=palette().color(backgroundRole());
    hk_colour colour(qcolor.red(),qcolor.green(),qcolor.blue());
    set_backgroundcolour(colour,false,true);
    qcolor=palette().color(QPalette::ButtonText);
    colour.set_colour(qcolor.red(),qcolor.green(),qcolor.blue());
    set_foregroundcolour(colour,false,true);
    setAutoFillBackground(true);
}


hk_kdeboolean::~hk_kdeboolean(void)
{

}


void hk_kdeboolean::keyPressEvent( QKeyEvent * e )
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeboolean::keyPressEvent");
#endif

//   p_while_keyaction=true;
    hk_key nk=hk_key(e->key(),e->modifiers(),(u2l(e->text().isNull()?"":e->text().toUtf8().data())));
    hk_key* k=NULL;
    if (p_gridcolumn)
      {
         p_gridcolumn->set_key(nk);
	 p_gridcolumn->action_on_key();
	 k=p_gridcolumn->key();
      }
      else
      {

       set_key(nk);
       action_on_key();
       k=key();
       }
    if (!k->accept_key())
    {
      e->ignore();
      //p_while_keyaction=false;
      return;

    }



   if (e->key()==Qt::Key_Return)
    {
        if (e->modifiers()&Qt::ControlModifier)
            focusNextPrevChild(false);
        else
            focusNextPrevChild(true);

    }
    else
    if (e->key()==Qt::Key_Escape)
       {
         if (column() && column()->has_changed()) column()->reset_changed_data();
	 else
	 if (datasource() && datasource()->has_changed())
	 {
	    datasource()->reset_changed_data();
	    datasource()->store_changed_data();
	 }
	 widget_specific_row_change();

       }
    else
    {
        if (column()!=NULL) if (column()->is_readonly())return;
        if (is_readonly()) return;
        QCheckBox::keyPressEvent(e);
    }
}


bool hk_kdeboolean::widget_specific_row_change(void)
{
#ifdef HK_DEBUG
    hkdebug("kdeboolean::widget_specific_row_change");
#endif
    hk_column* c=column();
    p_widget_specific_row_change=true;
    if (c!=NULL)
    {
        if ( c->has_changed())
	{
	    if (c->changed_data_is_nullvalue())
	        setCheckState(Qt::PartiallyChecked);
	    else
	       setChecked(c->changed_data_asbool());
	}
	else
	{

	    if (c->is_nullvalue())
	        setCheckState(Qt::PartiallyChecked);
	    else
                setCheckState(c->asbool()?Qt::Checked:Qt::Unchecked);

	}
    }

    p_widget_specific_row_change=false;
    return true;

}


void hk_kdeboolean::slot_data_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("kdeboolean::slotDataChanged");
#endif
    if (p_presentation && p_presentation->mode()==hk_presentation::filtermode && !signalsBlocked())
    {
	hk_string v;
            switch (checkState())
	    {
	      case Qt::Checked:v="%TRUE%";break;
	      case Qt::Unchecked:v="%FALSE%";break;
	      case Qt::PartiallyChecked:v="";break;
	    }

    set_filtervalue(v);
    return;
    }
    if (!p_widget_specific_row_change)
    {
#ifdef HK_DEBUG
        hkdebug("kdeboolean::data has changed!");
#endif
        if (column()!=NULL)
        {
#ifdef HK_DEBUG
            hkdebug("kdeboolean::data will be updated");
#endif
            switch (checkState())
	    {
	      case Qt::Checked:column()->set_asbool(true);break;
	      case Qt::Unchecked:column()->set_asbool(false);break;
	      case Qt::PartiallyChecked:column()->set_asnullvalue();break;
	    }

        }
action_on_valuechanged();
    }
    else if (!column()) action_on_valuechanged();
}


void hk_kdeboolean::widget_specific_enable_disable()
{
#ifdef HK_DEBUG
    hkdebug("kdeboolean::widget_specific_enable_disable");
#endif
    if ((datasource()!=NULL)&&(column()!=NULL))
    {
#ifdef HK_DEBUG
        hkdebug("kdeboolean::if ((datasource()!=NULL)&&(column()!=NULL))");
#endif

        if (datasource()->is_enabled())
        {
            setEnabled(is_enabled());
	    if (column())
	    {
	      setTristate(!column()->is_notnull());

	    }
            widget_specific_row_change();
        }
        else setEnabled(is_enabled());
    }
    else
    {
        if (p_presentation ) setEnabled(p_presentation->mode()==hk_presentation::viewmode && is_enabled());
	else
	setEnabled(is_enabled());
	if (datasource()&&!datasource()->is_enabled()) setEnabled(false);
        setChecked(false);
    }

/* if (p_presentation!=NULL) if (p_presentation->state()==hk_form::design)
 {
    setFocusPolicy(NoFocus);
    setEnabled(true);
  }
  else
*/
    setFocusPolicy(Qt::StrongFocus);

}


void hk_kdeboolean::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("kdeboolean::resizeEvent");
#endif
    QCheckBox::resizeEvent(event);
    reposition();

}


void  hk_kdeboolean::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("kdeboolean::moveEvent");
#endif
    QCheckBox::moveEvent(event);
    reposition();
}


void hk_kdeboolean::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("kdeboolean::reposition");
#endif
    if (p_presentation==NULL ||p_setcoordinates==true) return;
    p_setwidgetcoordinates=true;
    unsigned int ppx=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QCheckBox::x()):QCheckBox::x();
    unsigned int ppy=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QCheckBox::y()):QCheckBox::y();
    unsigned int pph=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QCheckBox::height()):QCheckBox::height();
    unsigned int ppw=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QCheckBox::width()):QCheckBox::width();
    set_size(ppx,ppy,ppw,pph);
    p_setwidgetcoordinates=false;
}


bool hk_kdeboolean::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeboolean::widget_specific_coordinates");
#endif
    setGeometry(px,py,pwidth,pheight);
//     qApp->processEvents();
    return true;
}


void    hk_kdeboolean::widget_specific_label_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeboolean::widget_specific_label_changed");
#endif
    if (label().size()>0)setText(QString::fromUtf8 (l2u(label()).c_str()));
    else setText(i18n("label"));
}


void hk_kdeboolean::widget_specific_font_changed(void)
{
    setFont(QFont(QString::fromUtf8 (l2u(hk_dsboolean::font().fontname()).c_str()),hk_dsboolean::font().fontsize()));
}


void    hk_kdeboolean::widget_specific_insert_mode(void)
{
    blockSignals(true);
    setChecked(use_defaultvalue()?(raw_defaultvalue()=="%TRUE%"):false);
    blockSignals(false);
}


void hk_kdeboolean::mousePressEvent(QMouseEvent* m)
{
    if (column()!=NULL) if (column()->is_readonly()||is_readonly()) return;
    QCheckBox::mousePressEvent(m);
       if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && m->button()== Qt::LeftButton)
           action_on_click();

}



void  hk_kdeboolean::mouseDoubleClickEvent(QMouseEvent* event)
{
   QCheckBox::mouseDoubleClickEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_doubleclick();
}




void hk_kdeboolean::widget_specific_row_added(void)
{
    widget_specific_row_change();
}


void hk_kdeboolean::widget_specific_row_deleted(void)
{
    widget_specific_row_change();
}


void    hk_kdeboolean::widget_specific_backgroundcolour_changed(const hk_colour& )
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    // rectangle background
    p.setColor(QPalette::Button,newcolour);
    // widget background
    p.setColor(backgroundRole(),newcolour);
    setPalette(p);
}


void    hk_kdeboolean::widget_specific_foregroundcolour_changed(const hk_colour& )
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    // tick color
    p.setColor(QPalette::ButtonText,newcolour);
    // widget label
    p.setColor(foregroundRole(),newcolour);
    setPalette(p);
}


void  hk_kdeboolean::widget_specific_tooltip_changed(void)
{
    setToolTip(QString::fromUtf8 (l2u(tooltip()).c_str()));
}




void hk_kdeboolean::focusInEvent ( QFocusEvent * e)
{
  action_on_getfocus();
  QCheckBox::focusInEvent(e);
}

void hk_kdeboolean::focusOutEvent ( QFocusEvent * e)
{
  action_on_loosefocus();
  QCheckBox::focusOutEvent(e);

}

void hk_kdeboolean::show_widget(void)
{
  show();
}

void hk_kdeboolean::hide_widget(void)
{
  hide();
}

bool hk_kdeboolean::close_widget(void)
{
  return close();
}

void hk_kdeboolean::set_value(const hk_string&v)
{
 hk_column* c=column();
 hk_dsboolean::set_value(v);
 if (!c)
   {
    if (v.size()==0)
        setCheckState(Qt::PartiallyChecked);
    else
        setChecked(string2upper(v)!="FALSE");
   }

}

hk_string hk_kdeboolean::value(void)
{
  if (!column()) return isChecked()?"TRUE":"FALSE";
  else return hk_dsboolean::value();
}

bool	hk_kdeboolean::presentationmode_changed(void)
{
if (!p_presentation)
  {
    return hk_dsboolean::presentationmode_changed();
  }
bool result=hk_dsboolean::presentationmode_changed();
if (p_presentation->mode()==hk_presentation::designmode)
   {
    setEnabled(false);
        blockSignals(true);
        setChecked(false);
        blockSignals(false);
   }
else 
   if (p_presentation->mode()==hk_presentation::filtermode)
   {
    if ( columnname().size()>0)
    {
       setEnabled(true);
        blockSignals(true);
 	setTristate(true);
        setCheckState(Qt::PartiallyChecked);
	if (filtervalue().size()>0)
	{
	hk_string v=string2upper(filtervalue());
	if (v=="=%TRUE% ") setChecked(true);
	else
	if (v=="=%FALSE% ") setChecked(false);
// 	cerr <<"v=#"<<v<<"#"<<endl;
	}
	else
	cerr <<" mode_changed no filtervalue()"<<endl;
        blockSignals(false);
   }
    else
      setEnabled(false);
   }
 else
 {
   setEnabled(is_enabled());
   if (column())setTristate(!column()->is_notnull());
   if (!datasource()||!column())
    {
        blockSignals(true);
        setChecked(use_defaultvalue()?(raw_defaultvalue()=="%TRUE%"):false);
        blockSignals(false);

    }
 }
return result;

}


void hk_kdeboolean::widget_specific_enabled_changed(void)
{
  setEnabled(is_enabled());
}

