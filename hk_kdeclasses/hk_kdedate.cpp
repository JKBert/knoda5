// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
//
// This file is part of the hk_kde4classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_kdedate.h"
#include "hk_kdesimpleform.h"

#include <kapplication.h>
#include <kdatetable.h>
#include <kiconloader.h>
#include <kicon.h>
#include <kdatepicker.h>

#include <qdatetime.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qcursor.h>
#include <qtooltip.h>
#include <qdesktopwidget.h>
#include <QKeyEvent>

#include <hk_datetime.h>


class datepopupprivate
{
public:
datepopupprivate():p_picker(NULL), p_layout(NULL),p_firstrelease(true),
p_eventloop(NULL)
{
}

KDatePicker* p_picker;
QHBoxLayout* p_layout;
bool p_firstrelease;
hk_kdedate* p_date;
QEventLoop* p_eventloop;

};
datepopup::datepopup(QWidget* wid,hk_kdedate* d):QFrame(wid,Qt::Popup)
{
p_private=new datepopupprivate;
p_private->p_picker=new KDatePicker(this);
p_private->p_layout=new QHBoxLayout(this);
QSizePolicy policy(QSizePolicy::Preferred,QSizePolicy::Preferred);
p_private->p_picker->setSizePolicy(policy);
p_private->p_layout->addWidget(p_private->p_picker);
p_private->p_date=d;

hk_string v;
if (d->presentation() && d->presentation()->mode()==hk_presentation::filtermode && !signalsBlocked())
 v=d->filtervalue();
else
if (d->column()) v=d->column()->curval_asstring(); 
hk_datetime dt;
dt.set_dateformat(d->dateformat());
dt.set_date_asstring(v);
QDate date(dt.year(),dt.month(),dt.day());
p_private->p_picker->blockSignals(true);
p_private->p_picker->setDate(date);
p_private->p_picker->blockSignals(false);
qApp->installEventFilter(this);
setMouseTracking(true);
adjustSize();
connect(p_private->p_picker,SIGNAL(dateSelected(QDate )),d,SLOT(slot_date_selected(QDate)));
connect(p_private->p_picker,SIGNAL(dateEntered(QDate )),d,SLOT(slot_date_selected(QDate)));
}

datepopup::~datepopup()
{
delete p_private;
}
  
bool datepopup::eventFilter(QObject* object,QEvent* event)
{

    if (event->type()==QEvent::KeyPress)
        {
            keyPressEvent((QKeyEvent*)event);
        }

    return QFrame::eventFilter(object,event);
}

  
int datepopup::exec(const QWidget* trigger)
{
 int x,y;
 
 if(p_private->p_eventloop != NULL)
     return 1;
 x=0;
 y=0;
 p_private->p_firstrelease=true;
 QPoint globalpos=trigger->parentWidget()->mapToGlobal(trigger->pos());
 
 if (globalpos.y()+trigger->height()+height()+1 <QApplication::desktop()->height())
   { //enough space below
     if (globalpos.x()+width() <QApplication::desktop()->width())
     { //left justified
      x=globalpos.x();
      y=globalpos.y()+trigger->height()+1;  
     }
     else
     {
      x=globalpos.x()-width()-1;
      y=globalpos.y()+trigger->height()+1;  
     }
   
   }
   else
   { //not enough space below
     if (globalpos.x()+width() <QApplication::desktop()->width())
     { //left justified
      x=globalpos.x();
      y=globalpos.y()-height()-1;  
     }
     else
     {
      x=globalpos.x()-width()-1;
      y=globalpos.y()-height()+1;  
     }
   }

  move(x,y);
  show();
  repaint();
  QEventLoop eventloop;
  p_private->p_eventloop = &eventloop;
  eventloop.exec();
  p_private->p_eventloop = NULL;
  hide();
 return 0;
}

void datepopup::mouseReleaseEvent(QMouseEvent*)
{
 if (p_private->p_firstrelease)
  p_private->p_firstrelease=false;
 else
  p_private->p_eventloop -> quit();
}

void datepopup::keyPressEvent(QKeyEvent*e)
{
 if (e->key()==Qt::Key_Escape)
  p_private->p_eventloop -> quit();
}



















class hk_kdedateprivate
{
public:
hk_kdedateprivate()
{
p_lineedit=NULL;
p_button=NULL;
p_layout=NULL;
p_filter_has_changed=false;

}
QLineEdit* p_lineedit;
QPushButton* p_button;
QHBoxLayout* p_layout;
        bool p_filter_has_changed;
};

hk_kdedate::hk_kdedate(QWidget* wid,hk_form* form):QFrame(wid),hk_dsdate(form)
{
p_private=new hk_kdedateprivate;
p_widget_specific_row_change=false;

 QSizePolicy lineeditpolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
p_private->p_layout=new QHBoxLayout(this);
p_private->p_lineedit=new QLineEdit(this);
p_private->p_lineedit->setSizePolicy(lineeditpolicy);
p_private->p_button=new QPushButton(this);
p_private->p_button->setFocusPolicy(Qt::ClickFocus);
 QSizePolicy buttonpolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
p_private->p_button->setSizePolicy(buttonpolicy);
p_private->p_button->setMaximumWidth(25);
p_private->p_button->setMinimumWidth(25);
KIconLoader* loader=KIconLoader::global();
loader->addAppDir("hk_kde4classes");
p_private->p_button->setIcon(KIcon("datebutton",KIconLoader::global()));

p_private->p_layout->addWidget(p_private->p_lineedit);
p_private->p_layout->addWidget(p_private->p_button);
setFocusProxy(p_private->p_lineedit);

    widget_specific_font_changed();

    QObject::connect(p_private->p_lineedit,SIGNAL(textChanged(const QString& )),this,SLOT(slot_data_changed()));
    QObject::connect(p_private->p_button,SIGNAL(pressed()),this,SLOT(button_clicked()));

//     QObject::connect(this,SIGNAL(dateSelected(QDate )),this,SLOT(slot_data_changed()));
//     QObject::connect(this,SIGNAL(tableClicked( )),this,SLOT(slot_table_clicked()));
    QObject::connect(p_private->p_lineedit,SIGNAL(lostFocus()),this,SLOT(slot_focus_lost()));

}

hk_kdedate::~hk_kdedate()
{
delete p_private;
}


void hk_kdedate::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("hk_kdedate::resizeEvent");
#endif
     QFrame::resizeEvent(event);
    reposition();

}


void  hk_kdedate::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdedate::resizeEvent");
#endif
QFrame::moveEvent(event);
    reposition();
}



void hk_kdedate::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdedate::reposition");
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

bool hk_kdedate::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdedate::widget_specific_coordinates");
#endif
    setGeometry(px,py,pwidth,pheight);
//     qApp->processEvents();

    return true;
}


bool    hk_kdedate::widget_specific_row_change(void)
{
    hk_column* c=column();
    p_widget_specific_row_change=true;
    if (datasource()!=NULL)
        if (c!=NULL)
    {
        hk_string v=datasource()->mode()==hk_datasource::mode_insertrow &&!c->has_changed()?
            use_defaultvalue()?defaultvalue():"":
        (c->has_changed()?c->changed_data_asstring():c->asstring(true));
        if ((is_numerictype(c) && c->columntype()!=hk_column::auto_inccolumn && v.length() > 0)
            || (c->columntype()==hk_column::auto_inccolumn
            && datasource()->mode()!=hk_datasource::mode_insertrow))
        {
            v=format_number(v,true,use_numberseparator(),commadigits(),hk_class::locale());
        }

        p_private->p_lineedit->setText(QString::fromUtf8 (l2u(v).c_str()));
        if (hk_dsdatavisible::alignment()==hk_dsdatavisible::alignleft)
            p_private->p_lineedit->setCursorPosition(0);
    }

    p_widget_specific_row_change=false;
    return true;
}

void   hk_kdedate::widget_specific_enable_disable(void)
{

#ifdef HK_DEBUG
    hkdebug("hk_kdedate::widget_specific_enable_disable");
#endif
    if ((datasource()!=NULL)&&(column()!=NULL))
    {
#ifdef HK_DEBUG
        hkdebug("hk_kdedate::if ((datasource()!=NULL)&&(column()!=NULL))");
#endif

        if (datasource()->is_enabled())
        {
            if (column() && column()->columntype()==hk_column::textcolumn) p_private->p_lineedit->setMaxLength(column()->size());
            else p_private->p_lineedit->setMaxLength(32767);

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

        p_private->p_lineedit->blockSignals(true);
        p_private->p_lineedit->setText(QString::fromUtf8 (use_defaultvalue()?l2u(defaultvalue()).c_str():""));
        p_private->p_lineedit->blockSignals(false);
    }


}


void hk_kdedate::slot_data_changed(void)
{
    
    if (p_presentation && p_presentation->mode()==hk_presentation::filtermode)
    {
    
    p_private->p_filter_has_changed=true;
    return;
    }
    
    
    if (!p_widget_specific_row_change)
    {
#ifdef HK_DEBUG
    hkdebug("kdedate::slot_data_changed");
#endif
        if (column()!=NULL)
        {
#ifdef HK_DEBUG
            hkdebug("kdedate::data will be updated");
#endif
            if (is_numerictype(column()))
                column()->set_asstring(format_number(u2l(p_private->p_lineedit->text().toUtf8().data()),true,use_numberseparator(),commadigits(),hk_class::locale()),true,true);
            else column()->set_asstring(u2l(p_private->p_lineedit->text().toUtf8().data()),true,true);
        }
      action_on_valuechanged();
    }
}

void hk_kdedate::show_widget(void)
{
  show();
}

void hk_kdedate::hide_widget(void)
{
  hide();
}

bool hk_kdedate::close_widget(void)
{
  void close();
  return true;
}

void  hk_kdedate::mousePressEvent(QMouseEvent* event)
{
   QFrame::mousePressEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_click();
}



void  hk_kdedate::mouseDoubleClickEvent(QMouseEvent* event)
{
   QFrame::mouseDoubleClickEvent(event);
   if (p_presentation && p_presentation->mode()==hk_presentation::viewmode
       && event->button()== Qt::LeftButton)
           action_on_doubleclick();
}


void hk_kdedate::focusInEvent ( QFocusEvent * e)
{
  action_on_getfocus();
  QFrame::focusInEvent(e);
}

void hk_kdedate::focusOutEvent ( QFocusEvent * e)
{


  action_on_loosefocus();
  QFrame::focusOutEvent(e);
emit  lostFocus();
}

void hk_kdedate::set_value(const hk_string&v)
{


 hk_column* c=column();
 if (c)hk_dsdate::set_value(v);
 else p_private->p_lineedit->setText(QString::fromUtf8 (l2u(v).c_str()));


}


hk_string hk_kdedate::value(void)
{
  if (!column()) return u2l(p_private->p_lineedit->text().toUtf8().data());
  else return hk_dsdate::value();
}

void hk_kdedate::slot_table_clicked(void)
{
        if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
        {

        }

}



void hk_kdedate::widget_specific_font_changed(void)
{
    QFont f=QFont(QString::fromUtf8 (l2u(hk_dsdate::font().fontname()).c_str())
                  ,hk_dsdate::font().fontsize(),hk_dsdate::font().bold()?QFont::Bold:QFont::Normal,
		   hk_dsdate::font().italic());
    //setFont(f);
    //KDateTable* p=dateTable();
    //p->setFont(f);
//     setFontSize(hk_dsdate::font().fontsize());
p_private->p_lineedit->setFont(f);

}




void    hk_kdedate::widget_specific_backgroundcolour_changed(const hk_colour&)
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(p_private->p_lineedit->palette());
    
    p.setColor(p_private->p_lineedit->backgroundRole(),newcolour);
    p_private->p_lineedit->setPalette(p);
}


void    hk_kdedate::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(p_private->p_lineedit->palette());
    
    p.setColor(p_private->p_lineedit->foregroundRole(),newcolour);
    p_private->p_lineedit->setPalette(p);
}

void hk_kdedate::widget_specific_enabled_changed(void)
{
  setEnabled(is_enabled());
}


bool hk_kdedate::presentationmode_changed()
{
if (!p_presentation)
  {
    return hk_dsdate::presentationmode_changed();
  }

bool result=hk_dsdate::presentationmode_changed();
if (p_presentation->mode()==hk_presentation::designmode)
   {
    setEnabled(false);
        p_private->p_lineedit->blockSignals(true);
        p_private->p_lineedit->setText("");
        p_private->p_lineedit->blockSignals(false);
   }
  else
    if (p_presentation->mode()==hk_presentation::filtermode)
   {
    if ( columnname().size()>0 && datasource())
    {
       setEnabled(true);
        p_private->p_lineedit->blockSignals(true);
        p_private->p_lineedit->setText(QString::fromUtf8(l2u(filtervalue()).c_str()));
        p_private->p_lineedit->blockSignals(false);
    }
    else
      setEnabled(false);
    }
 else
 {
   setEnabled(is_enabled());
   if (!datasource()||!column())
    {
        p_private->p_lineedit->blockSignals(true);
        p_private->p_lineedit->setText(QString::fromUtf8 (use_defaultvalue()?l2u(defaultvalue()).c_str():""));
        p_private->p_lineedit->blockSignals(false);

    }
 }
return result;
}


void    hk_kdedate::widget_specific_insert_mode(void)
{
//  widget_specific_row_change();
    p_private->p_lineedit->blockSignals(true);
    p_private->p_lineedit->setText(QString::fromUtf8 (use_defaultvalue()?l2u(defaultvalue()).c_str():""));
    p_private->p_lineedit->blockSignals(false);
}

void hk_kdedate::widget_specific_row_added(void)
{
    widget_specific_row_change();
}


void hk_kdedate::widget_specific_row_deleted(void)
{
    widget_specific_row_change();
}



void hk_kdedate::widget_specific_alignment(void)
{
    Qt::AlignmentFlag f;
    switch (hk_dsdate::alignment())
    {
        case alignright  : f= Qt::AlignRight;
        break;
        case aligncenter  : f= Qt::AlignHCenter;
        break;
        default       : f= Qt::AlignLeft;

    }
    p_private->p_lineedit->setAlignment(f);

}

void  hk_kdedate::widget_specific_tooltip_changed(void)
{
    setToolTip(QString::fromUtf8 (l2u(tooltip()).c_str()));
}



void hk_kdedate::button_clicked(void)
{
datepopup* d=new datepopup(NULL,this);

d->setFrameShape(QFrame::Box);
d->setFrameStyle(QFrame::WinPanel|QFrame::Sunken);
d->exec(p_private->p_button);

delete d;
}



void hk_kdedate::slot_date_selected(QDate d)
{
	hk_datetime dt;
	dt.set_dateformat(dateformat());
	dt.set_date(d.day(),d.month(),d.year());
   if (p_presentation && p_presentation->mode()==hk_presentation::filtermode && !signalsBlocked())
    {
	p_private->p_lineedit->setText("#"+QString::fromUtf8 (l2u(dt.date_asstring()).c_str())+"#");
    }
    else
	p_private->p_lineedit->setText(QString::fromUtf8 (l2u(dt.date_asstring()).c_str()));
slot_focus_lost();
}


void hk_kdedate::slot_focus_lost()
{
// cerr <<"slot_focus_lost:"<<p_private->p_filter_has_changed<<endl;
   if (!p_private->p_filter_has_changed) return;
   p_private->p_filter_has_changed=false;
   if (p_presentation && p_presentation->mode()==hk_presentation::filtermode && !signalsBlocked())
    {
    if (set_filtervalue(u2l(p_private->p_lineedit->text().toUtf8().data())))
    {
        p_private->p_lineedit->blockSignals(true);
        p_private->p_lineedit->setText(QString::fromUtf8(l2u(filtervalue()).c_str()));
        p_private->p_lineedit->blockSignals(false);
    }
    else
    {
     hk_string r="Error in statement:#"+ u2l(p_private->p_lineedit->text().toUtf8().data())+"#";
     show_warningmessage(r);
    }
    
    return;
    }

}


void hk_kdedate::update_filter(void)
{
slot_focus_lost();

}
