
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
//$Revision: 1.77 $
#include "hk_kdelineedit.h"
#include "hk_kdelineedit.moc"
#include <QKeyEvent>
#include <qmenu.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qtooltip.h>
#include <klocale.h>
#include "hk_kdefinddialog.h"
#include <hk_dsgridcolumn.h>

hk_kdelineedit::hk_kdelineedit(QWidget* wid,hk_form* form)
:KLineEdit(wid)
,hk_dslineedit(form)
{
    QObject::connect(this,SIGNAL(textChanged(const QString& )),this,SLOT(slot_data_changed()));
    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT(slot_focus_lost()));
    connect(this,SIGNAL(aboutToShowContextMenu(QMenu*)),this, SLOT(extendContextMenu(QMenu*)));
#ifdef HK_DEBUG
    hkclassname("hk_kdelineedit");
    hkdebug("hk_kdelineedit::hk_kdelineedit");
#endif
    p_widget_specific_row_change=true;
    set_alignment(hk_dslineedit::alignment(),false);
    p_finddialog=NULL;
    p_findfirstview=true;
    p_filter_has_changed=false;

    QColor qcolor=palette().color(backgroundRole());
    hk_colour colour(qcolor.red(),qcolor.green(),qcolor.red());
    set_backgroundcolour(colour,false,true);
    qcolor=palette().color(foregroundRole());
    colour.set_colour(qcolor.red(),qcolor.green(),qcolor.red());
    set_foregroundcolour(colour,false,true);

    widget_specific_font_changed();

}


hk_kdelineedit::~hk_kdelineedit()
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::destructor");
#endif
    p_widget_specific_row_change=true;

}


void hk_kdelineedit::keyPressEvent( QKeyEvent * e )
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


    if (e->key()==Qt::Key_Return)
    {
        if (e->modifiers()&Qt::ControlModifier)
            focusNextPrevChild(false);
        else
            focusNextPrevChild(true);

    }
    else
    {
       if (e->key()==Qt::Key_Escape)
       {
         if (column() && column()->has_changed())
             column()->reset_changed_data();
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
            if (e->key()==Qt::Key_F && e->modifiers()&Qt::ControlModifier)
                find_clicked();
            else
            if ( (column() && column()->is_readonly()) || (is_readonly() && !(p_presentation && p_presentation->mode()==hk_presentation::filtermode)) )
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
		    return;
                }
            }
	    else
	      if (e->key()==Qt::Key_0 &&e->modifiers()&Qt::ControlModifier) {
	       NULLselected();
               e->accept();
               return;
              }

        }
        QLineEdit::keyPressEvent(e);
    }
}


void hk_kdelineedit::slot_data_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::slot_data_changed");
#endif
    
    if (p_presentation && p_presentation->mode()==hk_presentation::filtermode)
    {
    
    p_filter_has_changed=true;
    return;
    }
    
    if (!p_widget_specific_row_change)
    {
#ifdef HK_DEBUG
        hkdebug("kdelineedit::data has changed!");
#endif
        if (column()!=NULL)
        {
#ifdef HK_DEBUG
            hkdebug("kdelineedit::data will be updated");
#endif
            if (is_numerictype(column()))
                column()->set_asstring(format_number(u2l(text().toUtf8().data()),true,use_numberseparator(),commadigits(),hk_class::locale()),true,true);
            else column()->set_asstring(u2l(text().toUtf8().data()),true,true);
        }
      action_on_valuechanged();
    }
}


void hk_kdelineedit::slot_focus_lost()
{
   if (!p_filter_has_changed) return;
   p_filter_has_changed=false;
   if (p_presentation && p_presentation->mode()==hk_presentation::filtermode && !signalsBlocked())
    {
    if (set_filtervalue(u2l(text().toUtf8().data())))
    {
        blockSignals(true);
        setText(QString::fromUtf8(l2u(filtervalue()).c_str()));
        blockSignals(false);
    }
    else
    {
     hk_string r="Error in statement:#"+ u2l(text().toUtf8().data())+"#";
     show_warningmessage(r);
    }
    
    return;
    }

}

bool hk_kdelineedit::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::widget_specific_coordinates");
#endif
    setGeometry(px,py,pwidth,pheight);
    return true;
}


bool hk_kdelineedit::widget_specific_row_change(void)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::widget_specific_row_change");
#endif
    hk_column* c=column();
    p_widget_specific_row_change=true;
    if (datasource()!=NULL)
        if (c!=NULL)
    {
        hk_string v=datasource()->mode()==hk_datasource::mode_insertrow &&!c->has_changed()?
            use_defaultvalue()?defaultvalue():"":
        (c->has_changed()?c->changed_data_asstring():c->asstring(true));
        if ( (is_numerictype(c) && c->columntype()!=hk_column::auto_inccolumn&&v.length() > 0)
            || (c->columntype()==hk_column::auto_inccolumn
            && datasource()->mode()!=hk_datasource::mode_insertrow) )
        {
            v=format_number(v,true,use_numberseparator(),commadigits(),hk_class::locale());
        }

        blockSignals(true);
        setText(QString::fromUtf8 (l2u(v).c_str()));
        blockSignals(false);
        if (hk_dsdatavisible::alignment()==hk_dsdatavisible::alignleft)
            setCursorPosition(0);
    }

    p_widget_specific_row_change=false;
    return true;
}


void hk_kdelineedit::widget_specific_enable_disable()
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::widget_specific_enable_disable");
#endif
    if ((datasource()!=NULL)&&(column()!=NULL))
    {
#ifdef HK_DEBUG
        hkdebug("kdelineedit::if ((datasource()!=NULL)&&(column()!=NULL))");
#endif

        if (datasource()->is_enabled())
        {
            if (column() && column()->columntype()==hk_column::textcolumn) setMaxLength(column()->size());
            else setMaxLength(32767);

            setEnabled(is_enabled());
            widget_specific_row_change();
        }
        else setEnabled(false);
    }
    else
    {
        if (p_presentation ) setEnabled(p_presentation->mode()==hk_presentation::viewmode &&is_enabled());
	else
	setEnabled(is_enabled());

        blockSignals(true);
        setText(QString::fromUtf8 (use_defaultvalue()?l2u(defaultvalue()).c_str():""));
        blockSignals(false);
    }

}


bool hk_kdelineedit::datasource_disable()
{
    bool erg=hk_dsdatavisible::datasource_disable();
    widget_specific_enable_disable();
    return erg;
}


bool hk_kdelineedit::datasource_enable()
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::datasource_enable");
#endif
    bool erg=hk_dsdatavisible::datasource_enable();
    widget_specific_enable_disable();
    return erg;
}


void hk_kdelineedit::datasource_delete(void)
{
    setText("");
    hk_dsdatavisible::datasource_delete();

}


void    hk_kdelineedit::widget_specific_insert_mode(void)
{
    blockSignals(true);
    setText(QString::fromUtf8 (use_defaultvalue()?l2u(defaultvalue()).c_str():""));
    blockSignals(false);
}


void hk_kdelineedit::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::resizeEvent");
#endif
    KLineEdit::resizeEvent(event);
    reposition();

}


void  hk_kdelineedit::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::resizeEvent");
#endif
    KLineEdit::moveEvent(event);
    reposition();
}

void  hk_kdelineedit::mousePressEvent(QMouseEvent* event)
{
   KLineEdit::mousePressEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_click();
}



void  hk_kdelineedit::mouseDoubleClickEvent(QMouseEvent* event)
{
   KLineEdit::mouseDoubleClickEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_doubleclick();
}



void hk_kdelineedit::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::reposition");
#endif
    if (p_presentation==NULL ||p_setcoordinates==true) return;
    p_setwidgetcoordinates=true;
    unsigned int ppx=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(KLineEdit::x()):KLineEdit::x();
    unsigned int ppy=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(KLineEdit::y()):KLineEdit::y();
    unsigned int pph=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(KLineEdit::height()):KLineEdit::height();
    unsigned int ppw=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(KLineEdit::width()):KLineEdit::width();
    set_size(ppx,ppy,ppw,pph);
    p_setwidgetcoordinates=false;
}


void hk_kdelineedit::widget_specific_font_changed(void)
{
    setFont(QFont(QString::fromUtf8 (l2u(hk_dslineedit::font().fontname()).c_str())
                  ,hk_dslineedit::font().fontsize(),hk_dslineedit::font().bold()?QFont::Bold:QFont::Normal,
		   hk_dslineedit::font().italic()));
}


void hk_kdelineedit::widget_specific_row_added(void)
{
    widget_specific_row_change();
}


void hk_kdelineedit::widget_specific_row_deleted(void)
{
    widget_specific_row_change();
}


void hk_kdelineedit::widget_specific_alignment(void)
{
    Qt::AlignmentFlag f;
    switch (hk_dslineedit::alignment())
    {
        case alignright  : f= Qt::AlignRight;
        break;
        case aligncenter  : f= Qt::AlignHCenter;
        break;
        default       : f= Qt::AlignLeft;

    }
    setAlignment(f);

}

void hk_kdelineedit::extendContextMenu(QMenu* pM)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::extendContextMenu");
#endif  
    hk_column* c=column();
    if (c  && !c->is_readonly()&&!c->is_notnull())
    {
        pM->addAction(i18n("Find"),this, SLOT(find_clicked()) );
        pM->addSeparator();
        pM->addAction( i18n( "set 'NULL' value" ),this, SLOT(NULLselected()), Qt::CTRL+Qt::Key_0  );
    }
}

void    hk_kdelineedit::NULLselected(void)
{

    hk_column* c=column();
    if (c)
    {
        c->set_asnullvalue();
        widget_specific_row_change();
    }

}


void hk_kdelineedit::find_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::find_clicked");
#endif
 /*TBP   if (!column()) return;
    if (p_finddialog==NULL)
    {
        p_finddialog=new hk_kdefinddialog(this,0,false);
        connect(p_finddialog, SIGNAL(signal_findbutton_clicked()),this,SLOT(find_next()));
        connect(p_finddialog, SIGNAL(signal_findargument_changed()),this,SLOT(findargument_changed()));
    }

    p_findfirstview=true;
    findargument_changed();
    p_finddialog->show(); */
}


void    hk_kdelineedit::find_next(void)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::find_next");
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


void hk_kdelineedit::findargument_changed(void)
{
    p_findfirstview=true;

}


void    hk_kdelineedit::widget_specific_backgroundcolour_changed(const hk_colour&)
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(backgroundRole(),newcolour);
    setPalette(p);
}


void    hk_kdelineedit::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(foregroundRole(),newcolour);
    setPalette(p);
}


void hk_kdelineedit::set_value(const hk_string&v)
{
 hk_column* c=column();
 if (c)hk_dslineedit::set_value(v);
 else setText(QString::fromUtf8 (l2u(v).c_str()));

}

hk_string hk_kdelineedit::value(void)
{
  if (!column()) return u2l(text().toUtf8().data());
  else return hk_dslineedit::value();
}

void    hk_kdelineedit::before_presentationmode_changed(void)
{
}

void hk_kdelineedit::update_filter(void)
{
slot_focus_lost();
}



bool	hk_kdelineedit::presentationmode_changed(void)
{
if (!p_presentation)
  {
    return hk_dslineedit::presentationmode_changed();
  }

 bool result=hk_dslineedit::presentationmode_changed();
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
        setText(QString::fromUtf8(l2u(filtervalue()).c_str()));
        blockSignals(false);
    }
    else
      setEnabled(false);
   }
  else
 {
   setEnabled(is_enabled());
   if (!datasource()||!column())
    {
        blockSignals(true);
        setText(QString::fromUtf8 (use_defaultvalue()?l2u(defaultvalue()).c_str():""));
        blockSignals(false);

    }
 }
return result;

}


void hk_kdelineedit::widget_specific_enabled_changed(void)
{
  setEnabled(is_enabled());
}



void  hk_kdelineedit::widget_specific_tooltip_changed(void)
{
    setToolTip(QString::fromUtf8 (l2u(tooltip()).c_str()));
}




void hk_kdelineedit::focusInEvent ( QFocusEvent * e)
{
  action_on_getfocus();
  KLineEdit::focusInEvent(e);
}

void hk_kdelineedit::focusOutEvent ( QFocusEvent * e)
{
  action_on_loosefocus();
  KLineEdit::focusOutEvent(e);

}

void hk_kdelineedit::closeEvent ( QCloseEvent * e )
{
  KLineEdit::closeEvent(e);
}

void hk_kdelineedit::show_widget(void)
{
  show();
}

void hk_kdelineedit::hide_widget(void)
{
  hide();
}

bool hk_kdelineedit::close_widget(void)
{
  return close();
}



