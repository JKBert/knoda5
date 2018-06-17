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

#include <hk_datasource.h>
#include <qbrush.h>
#include <hk_kdememo.h>
#include <hk_kdememo.moc>
#include <hk_form.h>
#include <klocale.h>
#include "hk_kdefinddialog.h"
#include <hk_dsgridcolumn.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qtooltip.h>
#include <qmenu.h>
#include <QKeyEvent>

hk_kdememo::hk_kdememo(QWidget* wid,hk_form* f):KTextEdit(wid), hk_dsmemo(f)
{
#ifdef HK_DEBUG
   hkclassname("hk_kdememo");
   //wanna_debug(true);
   hkdebug("hk_kdememo::constructor");
#endif
    has_changed=false;
    // allow PlaintText only - as 0.8.3
    setAcceptRichText(false);
    setText(QString());
    connect(this,SIGNAL(textChanged()),this,SLOT(slot_data_changed()));
    connect(this,SIGNAL(lostFocus()),this,SLOT(slot_focus_lost()));
    connect(this,SIGNAL(aboutToShowContextMenu(QMenu*)),this, SLOT(extendContextMenu(QMenu*)));
    p_widget_specific_row_change=true;
    setEnabled(false);
    setTabChangesFocus(true);
    if (f!=NULL) setLineWrapMode(WidgetWidth);
    set_alignment(hk_dsmemo::alignment(),false);
    QColor qcolor=palette().color(QPalette::Base);
    hk_colour colour(qcolor.red(),qcolor.green(),qcolor.blue());
    set_backgroundcolour(colour,false,true);
    qcolor=textColor();
    colour.set_colour(qcolor.red(),qcolor.green(),qcolor.blue());
    set_foregroundcolour(colour,false,true);

    widget_specific_font_changed();
    p_widget_specific_row_change=false;
    p_finddialog=NULL;
    p_filter_has_changed=false;
}


hk_kdememo::~hk_kdememo()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::destructor");
#endif
   if (has_changed)before_store_changed_data();
}


void hk_kdememo::slot_data_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::slot_data_changed");
#endif
    if (p_presentation && p_presentation->mode()==hk_presentation::filtermode)
    {
    
    p_filter_has_changed=true;
    return;
    }
    
    
    if (!p_widget_specific_row_change)
    {
        has_changed=true;
        set_data_has_changed(); // from hk_dsdatavisible
        action_on_valuechanged();
    }
}


bool hk_kdememo::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::coordinates");
#endif
    blockSignals(true);
    setGeometry(px,py,pwidth,pheight);
    blockSignals(false);
    return true;
}


bool hk_kdememo::widget_specific_row_change(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::widget_specific_row_change");
#endif
    hk_column* c=column();
    p_widget_specific_row_change=true;  
    if (c!=NULL)
    {
#ifdef HK_DEBUG
        hkdebug("hk_kdememo::widget_specific_row_change default");
#endif
        hk_string v=datasource()->mode()==hk_datasource::mode_insertrow &&!c->has_changed()?
            use_defaultvalue()?defaultvalue():"":
        (c->has_changed()?c->changed_data_asstring():c->asstring(true));
        setPlainText(QString::fromUtf8(l2u(v).c_str()));
    }

    p_widget_specific_row_change=false;
    has_changed=false;
    return true;
}


void hk_kdememo::widget_specific_enable_disable()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::widget_specific_enable_disable");
#endif
    if ((datasource()!=NULL)&&(column()!=NULL))
    {
        bool r=p_widget_specific_row_change;
        p_widget_specific_row_change=true;
        if (datasource()->is_enabled()>0)
        {
            setEnabled(is_enabled());
            widget_specific_row_change();
        }
        else setEnabled(false);
        p_widget_specific_row_change=r;
    }
    else
    {
        if (p_presentation ) setEnabled(p_presentation->mode()==hk_presentation::viewmode&&is_enabled());
	else
	setEnabled(is_enabled());
	if (datasource()&&!datasource()->is_enabled()) setEnabled(false);
        blockSignals(true);
        setText("");
        blockSignals(false);
    }
    setFocusPolicy(Qt::StrongFocus);

}


bool hk_kdememo::datasource_disable()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::datasource_disable");
#endif
    bool erg=hk_dsdatavisible::datasource_disable();
    widget_specific_enable_disable();
    return erg;
}


bool hk_kdememo::datasource_enable()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::datasource_enable");
#endif
    bool erg=hk_dsdatavisible::datasource_enable();
    widget_specific_enable_disable();
    return erg;
}


void hk_kdememo::datasource_delete(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::datasource_delete");
#endif
    setText("");
    hk_dsdatavisible::datasource_delete();
    has_changed=false;
}


void hk_kdememo::before_row_change()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::datasource_before_row_change");
#endif
   hk_dsmemo::before_row_change();
    before_store_changed_data();

}


void hk_kdememo::before_store_changed_data()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::datasource_before_store_changed_data");
#endif
    if ((!p_widget_specific_row_change)&&(has_changed))
    {
        if (column()!=NULL)
        {
            column()->set_asstring(u2l(toPlainText().toUtf8().data()),true,true);
            has_changed=false;
        }
    }
}


void hk_kdememo::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("kdememo::resizeEvent");
#endif
    QTextEdit::resizeEvent(event);
    reposition();

}


void  hk_kdememo::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("kdememo::resizeEvent");
#endif
    QTextEdit::moveEvent(event);
    reposition();
}


void hk_kdememo::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("kdememo::reposition");
#endif
    if (p_presentation==NULL ||p_setcoordinates==true) return;
    p_setwidgetcoordinates=true;
    unsigned int ppx=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QTextEdit::x()):QTextEdit::x();
    unsigned int ppy=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QTextEdit::y()):QTextEdit::y();
    unsigned int pph=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QTextEdit::height()):QTextEdit::height();
    unsigned int ppw=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QTextEdit::width()):QTextEdit::width();
    set_size(ppx,ppy,ppw,pph);
    p_setwidgetcoordinates=false;
}

void  hk_kdememo::mousePressEvent(QMouseEvent* event)
{
   KTextEdit::mousePressEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_click();
}



void  hk_kdememo::mouseDoubleClickEvent(QMouseEvent* event)
{
   KTextEdit::mouseDoubleClickEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_doubleclick();
}

void hk_kdememo::widget_specific_font_changed(void)
{
    setFont(QFont(QString::fromUtf8 (l2u(hk_dsmemo::font().fontname()).c_str())
                  ,hk_dsmemo::font().fontsize(),hk_dsmemo::font().bold()?QFont::Bold:QFont::Normal,
		   hk_dsmemo::font().italic()));
}


void    hk_kdememo::widget_specific_insert_mode(void)
{
    blockSignals(true);
    setPlainText(QString::fromUtf8(use_defaultvalue()?l2u(defaultvalue()).c_str():""));
    blockSignals(false);
}


void hk_kdememo::keyPressEvent( QKeyEvent * e )
{
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
      return;

    }




    if (column()==NULL)
    {
         QTextEdit::keyPressEvent(e);
         return;
    }

    if (e->key()==Qt::Key_F && (e->modifiers()&Qt::ControlModifier)) find_clicked();
            else
        if (column()->is_readonly()||is_readonly())
        {
        switch (e->key())
          {
	  case Qt::Key_Left:
	  case Qt::Key_Right:
	  case Qt::Key_Home:
	  case Qt::Key_End:
	    case Qt::Key_Return: break;
            default: return;
          }
        }

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
    else  QTextEdit::keyPressEvent(e);
}


void hk_kdememo::widget_specific_row_added(void)
{
    widget_specific_row_change();
}


void hk_kdememo::widget_specific_row_deleted(void)
{
    widget_specific_row_change();
}


void hk_kdememo::widget_specific_alignment(void)
{
    Qt::AlignmentFlag f;
    switch (hk_dsmemo::alignment())
    {
        case alignright  : f= Qt::AlignRight;
        break;
        case aligncenter  : f= Qt::AlignHCenter;
        break;
        default       : f= Qt::AlignLeft;

    }
//     bool r=p_widget_specific_row_change;
    blockSignals(true);
    setAlignment(f);
    blockSignals(false);
// cout <<" hk_kdememo::widget_specific_alignment "<<f<<endl;

}


void    hk_kdememo::widget_specific_backgroundcolour_changed(const hk_colour&)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdememo::widget_specific_backgroundcolour_changed");
#endif  
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());

    p.setColor(QPalette::Base,newcolour);
    setPalette(p);
}


void    hk_kdememo::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    
    setTextColor(newcolour);
}

void hk_kdememo::extendContextMenu(QMenu * pM)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::extendContextMenu");
#endif  
    hk_column* c=column();
    if (c  && !c->is_readonly()&&!c->is_notnull()) {
      pM->addSeparator(); 
      pM->addAction(i18n( "set 'NULL' value" ),this, SLOT(NULLselected()), Qt::CTRL+Qt::Key_0);
    }
}

void hk_kdememo::find_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("kdememo::find_clicked");
#endif
    if (!column()) return;
    if (p_finddialog==NULL)
    {
        p_finddialog=new hk_kdefinddialog(this,0,false);
        connect(p_finddialog, SIGNAL(signal_findbutton_clicked()),this,SLOT(find_next()));
        connect(p_finddialog, SIGNAL(signal_findargument_changed()),this,SLOT(findargument_changed()));
    }

    p_findfirstview=true;
    findargument_changed();
    p_finddialog->show();
}


void    hk_kdememo::find_next(void)
{
#ifdef HK_DEBUG
    hkdebug("kdememo::find_next");
#endif
    bool search=true;
    if (p_finddialog==NULL)return;
    while (search)
    {
        if (p_finddialog->all_columnsbox->isChecked())
        {
            cout <<"noch nicht eingebaut"<<endl;
            search=false;
        }

        else
        {
            hk_column* c= column();
            if (c==NULL) return;
            unsigned int from=0;
            unsigned int to=0;
            if (p_finddialog->find_backwardsbox->isChecked())
            {

                from=0;
                to=((datasource()->row_position()>0)&&!p_findfirstview)?
                    datasource()->row_position()-1:
                (p_findfirstview?datasource()->row_position():datasource()->max_rows()-1);
            }
            else
            {
                from=((datasource()->row_position()<datasource()->max_rows()-2)&&!p_findfirstview)?
                    datasource()->row_position()+1:
                (p_findfirstview?datasource()->row_position():0);
                to=datasource()->max_rows()-1;
            }

            unsigned int r=c->find( from,
                to,
                u2l(p_finddialog->searchfield->currentText().toUtf8().data()),
                !p_finddialog->part_of_columnbox->isChecked(),
                p_finddialog->case_sensitivebox->isChecked(),
                p_finddialog->find_backwardsbox->isChecked()
                );
            if (r<datasource()->max_rows())
            {
                datasource()->goto_row(r);
                search=false;
            }
            else
            {
                if (!p_findfirstview)
                {
                    show_warningmessage(hk_translate("Not found."));
                    search=false;
                    return;
                }
                else
                {
                    datasource()->goto_first();
                }

            }
        }

        p_findfirstview=false;
    }

}


void hk_kdememo::findargument_changed(void)
{
    p_findfirstview=true;

}

void    hk_kdememo::NULLselected(void)
{

    hk_column* c=column();
    if (c)
    {
        c->set_asnullvalue();
        widget_specific_row_change();
    }

}




void hk_kdememo::set_value(const hk_string&v)
{
 hk_column* c=column();
 hk_dsmemo::set_value(v);
 if (!c||p_presentation->mode()==hk_presentation::filtermode) 
   setPlainText(QString::fromUtf8 (l2u(v).c_str()));
}

hk_string hk_kdememo::value(void)
{
  if (!column()) return u2l(toPlainText().toUtf8().data());
  else return hk_dsmemo::value();
}


bool	hk_kdememo::presentationmode_changed(void)
{
bool result=hk_dsmemo::presentationmode_changed();
if (!result)return false;
if (!p_presentation) return true;
if (p_presentation->mode()==hk_presentation::designmode)
   {
    setEnabled(false);
        blockSignals(true);
        setText("");
        blockSignals(false);
   }
 else 
   if (p_presentation->mode()==hk_presentation::filtermode)
   {
    if ( columnname().size()>0 && datasource())
    {
       setEnabled(true);
        blockSignals(true);
        setPlainText(QString::fromUtf8(l2u(filtervalue()).c_str()));
        blockSignals(false);
    }
    else
      setEnabled(false);
   }
 else
 {
   setEnabled(is_enabled());
 }
return true;
}

void  hk_kdememo::widget_specific_tooltip_changed(void)
{
    setToolTip(QString::fromUtf8 ((tooltip()).c_str()));
}




void hk_kdememo::focusInEvent ( QFocusEvent * e)
{
  action_on_getfocus();
  QTextEdit::focusInEvent(e);
}

void hk_kdememo::focusOutEvent ( QFocusEvent * e)
{
  action_on_loosefocus();
  QTextEdit::focusOutEvent(e);
  emit  lostFocus();


}


void hk_kdememo::show_widget(void)
{
  show();
}

void hk_kdememo::hide_widget(void)
{
  hide();
}

bool hk_kdememo::close_widget(void)
{
  return close();
}

void hk_kdememo::widget_specific_enabled_changed(void)
{
  setEnabled(is_enabled());
}



void hk_kdememo::slot_focus_lost()
{
   if (!p_filter_has_changed) return;
   p_filter_has_changed=false;
   if (p_presentation && p_presentation->mode()==hk_presentation::filtermode && !signalsBlocked())
    { 
    if (set_filtervalue(u2l(toPlainText().toUtf8().data())))
    {
        blockSignals(true);
        setPlainText(QString::fromUtf8(l2u(filtervalue()).c_str()));
        blockSignals(false);
    }
    else
    {
     hk_string r="Error in statement:#"+ u2l(toPlainText().toUtf8().data())+"#";
     show_warningmessage(r);
    }
    
    return;
    }

}

void    hk_kdememo::before_presentationmode_changed(void)
{
// slot_focus_lost();
}

void hk_kdememo::update_filter(void)
{
slot_focus_lost();

}


