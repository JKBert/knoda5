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
//$Revision: 1.82 $
#include <hk_datasource.h>
#include "hk_kdecombobox.h"
#include <hk_kdecombobox.moc>
#include <hk_form.h>
#include <qlineedit.h>
#include <QKeyEvent>
#include <qtooltip.h>
#include <kglobalsettings.h>
#include <hk_dsgridcolumn.h>

class qlineedit:public QLineEdit
{
  public:
    qlineedit(hk_kdecombobox* c):QLineEdit(c){p_combobox=c;}
    virtual ~qlineedit(){}
    virtual void  mousePressEvent(QMouseEvent* e)
	{
      p_combobox->mousePressEvent(e);
	}
    virtual void  mouseDoubleClickEvent(QMouseEvent*e)
	{
      p_combobox->mouseDoubleClickEvent(e);
	}

    hk_kdecombobox* p_combobox;
};


class hk_kdecomboboxprivate
{
public:
  hk_kdecomboboxprivate()
  {
    p_while_load_listitems=false;
  }
  bool p_while_load_listitems;
};


hk_kdecombobox::hk_kdecombobox(QWidget* wid,hk_form* form):QComboBox(wid),hk_dscombobox(form)
{
#ifdef HK_DEBUG
// wanna_debug(true);
    hkdebug("hk_kdecombobox::hk_kdecombobox");
#endif
    p_private=new hk_kdecomboboxprivate();
    p_widget_specific_row_change=true;
    p_whiledatachange=false;
    setEditable(true);
    setAutoCompletion(true);
    setDuplicatesEnabled(false);
    setEnabled(false);
    setInsertPolicy(NoInsert);
    p_filter_has_changed=false;

    const QWidget *colorTemplateWidget = (dynamic_cast<hk_form*>(wid) != NULL ? wid : this);
    QColor qcolor=colorTemplateWidget->palette().color(QPalette::Base);
    hk_colour colour(qcolor.red(),qcolor.green(),qcolor.red());
    
    set_backgroundcolour(colour,false,true);
    qcolor=colorTemplateWidget->palette().color(QPalette::WindowText);
    colour.set_colour(qcolor.red(),qcolor.green(),qcolor.red());
    set_foregroundcolour(colour,false,true);

    connect(this,SIGNAL(editTextChanged( const QString& )),this,SLOT(slot_text_changed()));
    connect(this,SIGNAL(activated(int)),this,SLOT(slot_data_changed(int)));

    widget_specific_font_changed();
    setLineEdit( new qlineedit(this));
}


hk_kdecombobox::~hk_kdecombobox()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdecombobox::~hk_kdecombobox");
#endif
    delete p_private;
}


void hk_kdecombobox::slot_data_changed(int index)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::slot_data_changed");
#endif
// cerr <<"slot_data_changed("<<index<<")"<<endl;
   if ((p_presentation && p_presentation->mode()!=hk_presentation::filtermode)||!p_presentation)
     if (mode()==combo)
        return;
   set_selecteditemdata(index);
    if (p_presentation &&p_presentation->mode()==hk_presentation::viewmode)
       action_on_select();

}


void hk_kdecombobox::slot_text_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::slot_text_changed");
#endif
   if (mode()!=combo) return;
    if (p_presentation && p_presentation->mode()==hk_presentation::filtermode)
    {    p_filter_has_changed=true;
    
    set_filtervalue(u2l(currentText().toUtf8().data()));
    
    return;
    }

    int f=find_edittextitem();
    if (f>-1||viewcolumn()==NULL)
        set_selecteditemdata(f);

}


bool hk_kdecombobox::widget_specific_row_change(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::widget_specific_row_change");
#endif
    hk_column* c=column();
    p_widget_specific_row_change=true;
    if (c!=NULL)
    {
        hk_dscombobox::widget_specific_row_change();
    }

    if (mode()==selector)
    {
        blockSignals(true);
        setCurrentIndex(datasource()->row_position());
        blockSignals(false);

    }

    p_widget_specific_row_change=false;
    return true;
}


void hk_kdecombobox::widget_specific_enable_disable()
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::widget_specific_enable_disable");
#endif
    if ((datasource()!=NULL)&&((column()!=NULL)||mode()==selector))
    {
#ifdef HK_DEBUG
        hkdebug("kdecombobox::if ((datasource()!=NULL)&&(column()!=NULL))");
#endif

        if (datasource()->is_enabled()>0)
        {

            setEnabled(is_enabled());
            setAutoCompletion(true);

        }
        else setEnabled(false);
    }
    else
    {
        if (p_presentation ) setEnabled(p_presentation->mode()==hk_presentation::viewmode && is_enabled());
	else
	setEnabled(is_enabled());
	if (datasource()&&!datasource()->is_enabled()) setEnabled(false);
    }
    setFocusPolicy(Qt::StrongFocus);

}


bool hk_kdecombobox::datasource_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::datasource_enable");
#endif
    bool erg=hk_dscombobox::datasource_enable();
    load_listitems();
    return erg;
}


bool hk_kdecombobox::datasource_disable(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::datasource_disable");
#endif
    before_row_change();
    blockSignals(true);
    clear();                                      //clears the columnbox
    blockSignals(false);
    if (listdatasource()!=NULL&& (mode()==combo||mode()==combo_noedit)) listdatasource()->disable();
    return hk_dsdatavisible::datasource_disable();

}


void hk_kdecombobox::set_currentlistitem(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::set_currentlistitem");
#endif

    if (datasource()==NULL) return;
    if (column()==NULL) return;
    
    
     if (p_presentation && p_presentation->mode()==hk_presentation::filtermode)
     {
       int i = findText(QString::fromUtf8(l2u(filtervalue()).c_str()));
       if (i != -1)
           setCurrentIndex(i);
       else 
           // must be editable 
           setEditText(QString::fromUtf8(l2u(filtervalue()).c_str()));
       return;
     }
    
    
    if (mode()==combo_noedit)
    {
      unsigned int p=0;
      if (use_textlist())
      {
         QString t=QString::fromUtf8(l2u(
	            column()->asstring_at(datasource()->row_position(),true)).c_str());
	 int c=0;
	 while (c<count())
	  {
	    if (itemText(c)==t)
	       {
	       p=(unsigned int)c;
	       c=count();
	       }
	    ++c;
	  }
      }
      else
      {
      if (listcolumn()) p=listcolumn()->find(column()->asstring_at(datasource()->row_position(),true),true);
      if (p>datasource()->max_rows())
        p=0;
      }
      setCurrentIndex(p);

    }
    else
    setEditText(QString::fromUtf8 (l2u(value_at(datasource()->row_position())).c_str()));
}


void hk_kdecombobox::set_selecteditemdata(int index)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::set_selecteditemdata");
#endif
    if (mode()==selector)
    {
        selector_row_changed(index);
        action_on_valuechanged();
        return;
    }

    if (p_presentation && p_presentation->mode()==hk_presentation::filtermode)
    {
       //must be editable
       setEditText("'"+itemText(index)+"'");
       slot_focus_lost();
       return;
    }



    if (column()==NULL)
    {
    action_on_valuechanged();
     return;
    }
    blockSignals(true);
    if (lineEdit()!=NULL) lineEdit()->blockSignals(true);

    if((listcolumn()==NULL)||(listdatasource()==NULL)||(viewcolumn()==NULL))
    {
#ifdef HK_DEBUG
        hkdebug("kdecombobox::set_selecteditemdata listcolumn==NULL");
#endif
        p_whiledatachange=true;
	column()->set_asstring(u2l(currentText().toUtf8().data()),true,true);
	p_whiledatachange=false;
        blockSignals(false);
        if (lineEdit()!=NULL) lineEdit()->blockSignals(false);
        return;
    }
    unsigned int r=viewcolumn()->find(u2l(itemText(index).toUtf8().data()),true);
    if (r<listdatasource()->max_rows())
    {
#ifdef HK_DEBUG
        hkdebug("r= ",(int)r);
        hkdebug(viewcolumn()->asstring_at(r,true),listcolumn()->asstring_at(r,true));
#endif
        p_whiledatachange=true;
	column()->set_asstring(listcolumn()->asstring_at(r,false),true,false);
	p_whiledatachange=false;
    }
    blockSignals(false);
    if (lineEdit()!=NULL) lineEdit()->blockSignals(false);
    action_on_valuechanged();
}


bool hk_kdecombobox::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
    setGeometry(px,py,pwidth,pheight);
    return true;
}


void hk_kdecombobox::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::resizeEvent");
#endif
    QComboBox::resizeEvent(event);
    reposition();
}


void hk_kdecombobox::keyPressEvent(QKeyEvent * e)
{
   //p_while_keyaction=true;
    
     if (p_presentation && p_presentation->mode()==hk_presentation::filtermode)
     {
        QComboBox::keyPressEvent(e);
        return;
     }

    
    
    hk_key nk=hk_key(e->key(),e->modifiers(),(e->text().isNull()?"":u2l(e->text().toUtf8().data())));
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


        {
            if ((column()&&column()->is_readonly())||is_readonly())
            {
                switch (e->key())
                {
		  case Qt::Key_Left:
		  case Qt::Key_Right:
		  case Qt::Key_Home:
		  case Qt::Key_End:
		  case Qt::Key_Return: break;
		  case Qt::Key_C: if (e->modifiers()&Qt::ControlModifier)
                    {                             //Copy pressed
                        break;
                    }

                    default:
		    //p_while_keyaction=false;
		    return;
                }
            }
	}

    QComboBox::keyPressEvent(e);
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

       // if (column()!=NULL)
/*  if (!e->isAccepted())
     if (datasource()&&datasource()->is_enabled())
        e->accept();*/
}


void  hk_kdecombobox::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::resizeEvent");
#endif
    QComboBox::moveEvent(event);
    reposition();
}


void hk_kdecombobox::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::reposition");
#endif
    if (p_presentation==NULL ||p_setcoordinates==true) return;
    p_setwidgetcoordinates=true;
    unsigned int ppx=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QComboBox::x()):QComboBox::x();
    unsigned int ppy=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QComboBox::y()):QComboBox::y();
    unsigned int pph=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QComboBox::height()):QComboBox::height();
    unsigned int ppw=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QComboBox::width()):QComboBox::width();
    set_size(ppx,ppy,ppw,pph);
    p_setwidgetcoordinates=false;
}


void    hk_kdecombobox::widget_specific_insert_mode(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::widget_specific_insert_mode");
#endif
    blockSignals(true);
    unsigned int r=0;

    hk_string d;

        if (datasource()==NULL|| column()==NULL)
	  {

	  }
    else
    if((listcolumn()==NULL)||(listdatasource()==NULL)||(viewcolumn()==NULL))
    {
        if (column()!=NULL)
        {
            d= (is_numerictype(column())?
                format_number(defaultvalue(),true,use_numberseparator(),commadigits(),hk_class::locale())
                :defaultvalue());
        }

    }
    else
    {
     r=listcolumn()->find(defaultvalue(),true);
    if (r<=listdatasource()->max_rows())
    {
        if (!viewcolumn()->is_nullvalue_at(r)&&is_numerictype(viewcolumn()))
            d= format_number(viewcolumn()->asstring_at(r,true),true,use_numberseparator(),commadigits(),hk_class::locale());
        else
            d= viewcolumn()->asstring_at(r,true);
    }
       if (mode()==combo_noedit)
       {
         setCurrentIndex(r);
	}

   }


    setEditText(QString::fromUtf8 (l2u(d).c_str()));




    blockSignals(false);
}





void hk_kdecombobox::load_listitems(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::load_listitems");
#endif
   if(p_whiledatachange||p_private->p_while_load_listitems) return;
    p_private->p_while_load_listitems=true;
  if (use_textlist())
   load_listitems_from_textlist();
  else
   load_listitems_from_column();
    p_private->p_while_load_listitems=false;
}

void  hk_kdecombobox::load_listitems_from_textlist(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::load_listitems_from_textlist");
#endif
    blockSignals(true);
    clear();
    list<hk_string>* tl=textlist();
    list<hk_string>::iterator it=tl->begin();
    while (it!=tl->end())
    {
     addItem(QString::fromUtf8 (l2u((*it)).c_str()));
     ++it;
    }
    set_currentlistitem();
    blockSignals(false);

}

void  hk_kdecombobox::load_listitems_from_column(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::load_listitems_from_column");
#endif

    blockSignals(true);
    if  (listdatasource()==NULL)
    {
#ifdef HK_DEBUG
        hkdebug("kdecombobox::load_listitems listdatasource==NULL");
#endif
	clear();
	set_currentlistitem();
//cout <<"listdatasource==NULL"<<endl;
        blockSignals(false);
        return;
    }
    if (listdatasource()->depending_on() || count()==0 || 
         (!listdatasource()->depending_on()
          && (!listdatasource()->is_enabled() ))
       )
    {
     clear();
     listdatasource()->disable();
     listdatasource()->enable();
     unsigned int r=listdatasource()->max_rows();
     if (viewcolumn()!=NULL)
     {
        for (unsigned int k=0;k<r;k++)
        {
            addItem(QString::fromUtf8 (l2u(viewcolumn()->asstring_at(k,true)).c_str()));
        }
     }
    }
    set_currentlistitem();
    blockSignals(false);
}


void hk_kdecombobox::widget_specific_row_added(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::widget_specific_row_added");
#endif
   widget_specific_row_change();
}


void hk_kdecombobox::widget_specific_row_deleted(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::widget_specific_row_deleted");
#endif
    widget_specific_row_change();
}


int     hk_kdecombobox::find_edittextitem(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::find_edittextitem");
#endif
    QString value;
    if (lineEdit()!=NULL)value= lineEdit()->text();
    for (int i=0;i<count();i++)
    {
        if (value==itemText(i))
        {
#ifdef HK_DEBUG
            hkdebug("kdecombobox::find_edittextitem value found",i);
#endif
            return i;
        }
    }
#ifdef HK_DEBUG
    hkdebug("kdecombobox::find_edittextitem value not found");
#endif

    return -1;
}


void    hk_kdecombobox::widget_specific_mode_change(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::widget_specific_mode_change");
#endif
//     disconnect(this,SIGNAL(activated(int)),this,SLOT(slot_data_changed(int)));
//     disconnect(this,SIGNAL(textChanged( const QString& )),this,SLOT(slot_text_changed()));
    if (mode()==combo)
    {
        setEditable(true);
//     connect(this,SIGNAL(textChanged( const QString& )),this,SLOT(slot_text_changed()));

    }
    else
    {

        setEditable(false);
//     connect(this,SIGNAL(activated(int)),this,SLOT(slot_data_changed(int)));

    }

}


void hk_kdecombobox::selector_row_changed(int index)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::selector_row_changed");
#endif
    if (listdatasource()) listdatasource()->goto_row(index);

}


void    hk_kdecombobox::after_store_changed_data(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::after_store_changed_data");
#endif
    hk_dscombobox::after_store_changed_data();
    if (mode()==selector) setCurrentIndex(datasource()->row_position());

}


void hk_kdecombobox::widget_specific_font_changed(void)
{
    setFont(QFont(QString::fromUtf8 (l2u(hk_dscombobox::font().fontname()).c_str())
                  ,hk_dscombobox::font().fontsize(),hk_dscombobox::font().bold()?QFont::Bold:QFont::Normal,
		   hk_dscombobox::font().italic()));
}


void    hk_kdecombobox::widget_specific_backgroundcolour_changed(const hk_colour&)
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(QPalette::Base,newcolour);
    setPalette(p);

}


void    hk_kdecombobox::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette pl(palette());

    pl.setColor(QPalette::Text,newcolour);
    setPalette(pl);
}


void  hk_kdecombobox::mousePressEvent(QMouseEvent* event)
{
   QComboBox::mousePressEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_click();
}



void  hk_kdecombobox::mouseDoubleClickEvent(QMouseEvent* event)
{
   QComboBox::mouseDoubleClickEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_doubleclick();
}



void hk_kdecombobox::set_value(const hk_string&v)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::set_value");
#endif
    setEditText(QString::fromUtf8 (l2u(v).c_str()));
    hk_dscombobox::set_value(v);
}


hk_string hk_kdecombobox::value(void)
{
  return u2l(currentText().toUtf8().data());
}


bool	hk_kdecombobox::presentationmode_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("kdecombobox::presentationmode_changed");
#endif
bool r=hk_dscombobox::presentationmode_changed();
if (!r) return false;
if (!p_presentation) return true;
if (p_presentation->mode()==hk_presentation::designmode)
   {
    setEnabled(false);
        blockSignals(true);
        setEditText("");
        blockSignals(false);
   }
else 
   if (p_presentation->mode()==hk_presentation::filtermode)
   {
    if ( columnname().size()>0 && mode()!=selector)
    {
       setEnabled(true);
       setEditable(true);
       blockSignals(true);
       clear();	
       load_listitems();
       int i = findText(QString::fromUtf8(l2u(filtervalue()).c_str()));
       if (i != -1)
           setCurrentIndex(i);
       else
           setEditText(QString::fromUtf8(l2u(filtervalue()).c_str()));
       blockSignals(false);
    }
    else
      setEnabled(false);
   }
 else
 {
 widget_specific_mode_change();
   setEnabled(is_enabled());
   if (use_textlist()&&!column())
      load_listitems();
 }
return true;
}


void  hk_kdecombobox::widget_specific_tooltip_changed(void)
{
    setToolTip(QString::fromUtf8 (l2u(tooltip()).c_str()));
}

void hk_kdecombobox::focusInEvent ( QFocusEvent * e)
{
  action_on_getfocus();
  QComboBox::focusInEvent(e);
}

void hk_kdecombobox::focusOutEvent ( QFocusEvent * e)
{
  slot_focus_lost();
  action_on_loosefocus();
  QComboBox::focusOutEvent(e);

}

void    hk_kdecombobox::slot_focus_lost()
{
if (!p_filter_has_changed) return;
   p_filter_has_changed=false;
   if (p_presentation && p_presentation->mode()==hk_presentation::filtermode && !signalsBlocked())
    {  
    if (set_filtervalue(u2l(currentText().toUtf8().data())))
    {
        blockSignals(true);
        int i = findText(QString::fromUtf8(l2u(filtervalue()).c_str()));
        if (i != -1)
            setCurrentIndex(i);
        else
            setEditText(QString::fromUtf8(l2u(filtervalue()).c_str()));
        blockSignals(false);
    }
    else
    {
     hk_string r="Error in statement:#"+ u2l(currentText().toUtf8().data())+"#";
     show_warningmessage(r);
    }
    
    }

}


void hk_kdecombobox::show_widget(void)
{
  show();
}

void hk_kdecombobox::hide_widget(void)
{
  hide();
}

bool hk_kdecombobox::close_widget(void)
{
  return close();
}


void hk_kdecombobox::widget_specific_enabled_changed(void)
{
  setEnabled(is_enabled());
}

void    hk_kdecombobox::before_presentationmode_changed(void)
{
//slot_focus_lost();
}



void hk_kdecombobox::update_filter(void)
{
slot_focus_lost();

}
