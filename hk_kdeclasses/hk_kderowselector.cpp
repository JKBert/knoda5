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
//$Revision: 1.33 $

#include <hk_kderowselector.h>
#include <hk_kderowselector.moc>
#include <qtooltip.h>
#include <qstyle.h>
#include <qrect.h>
#include <klocale.h>
#include <kapplication.h>
#include <qpainter.h>
#include <qdrawutil.h>
#include <qnamespace.h>
#include <qsizepolicy.h>
#include <qtooltip.h>
#include <hk_form.h>
#include <kiconloader.h>
#include <kicon.h>

//TBP icons
class rowselectorbutton:public QPushButton
{
    public:
        rowselectorbutton( QWidget* wid,const QString& iconName):QPushButton(wid)
        {
          setFocusPolicy(Qt::ClickFocus);
          KIconLoader::global()->addAppDir("hk_kde4classes");
	  setIcon(KIcon(iconName,KIconLoader::global()));
          setIconSize(QSize(20,20));
          setMaximumSize(QSize(22,20));
        }
    protected:
    private:
};


hk_kderowselector::hk_kderowselector(QWidget* wid,hk_form* form) :
QWidget(wid),hk_dsrowselector(form)
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::hk_kderowselector");
#endif
    QSizePolicy policy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    int lm, tm,rm,bm;
    
    button_delete= new rowselectorbutton(this,"delete");
    button_gotofirst= new rowselectorbutton(this,"first");
    button_gotoprevious= new rowselectorbutton(this,"previous");
    button_gotonext= new rowselectorbutton(this,"next");
    button_gotolast= new rowselectorbutton(this,"last");
    button_store= new rowselectorbutton(this,"store");
    button_insert= new rowselectorbutton(this,"add");
    button_delete->setSizePolicy(policy);
    button_gotofirst->setSizePolicy(policy);
    button_gotolast->setSizePolicy(policy);
    button_gotonext->setSizePolicy(policy);
    button_gotoprevious->setSizePolicy(policy);
    button_store->setSizePolicy(policy);
    button_insert->setSizePolicy(policy);    

    button_gotofirst->setToolTip(i18n("Goto first"));
    button_gotoprevious->setToolTip(i18n("Goto previous"));
    button_gotonext->setToolTip(i18n("Goto next"));
    button_gotolast->setToolTip(i18n("Goto last"));
    button_store->setToolTip(i18n("Store changes"));
    button_delete->setToolTip(i18n("Delete row"));
    button_insert->setToolTip(i18n("Insert new row"));
    label= new KLineEdit(this);
    label->setSizePolicy(policy);
    label->setFocusPolicy(Qt::ClickFocus);
    label -> setMinimumWidth(40);
    layout= new QHBoxLayout(this);
    layout -> getContentsMargins(&lm,&tm, &rm, &bm);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->addWidget(button_delete,1);
    layout->addWidget(button_gotofirst,1);
    layout->addWidget(button_gotoprevious,1);
    layout->addWidget(label,3);
    layout->addWidget(button_gotonext,1);
    layout->addWidget(button_gotolast,1);
    layout->addWidget(button_store,1);
    layout->addWidget(button_insert,1);
    layout->activate();
    layout->setEnabled(true);
    setFocusProxy(label);
    
    
    const QWidget *forecolortemplateWidget, *backcolortemplateWidget;
    if (dynamic_cast<hk_form*>(wid) != NULL) {
        backcolortemplateWidget = button_gotofirst;
        forecolortemplateWidget = label;
        setAutoFillBackground(true);
    } else 
        backcolortemplateWidget = forecolortemplateWidget = this;
    QColor qcolor = backcolortemplateWidget-> palette().color(backcolortemplateWidget->backgroundRole());
    hk_colour colour(qcolor.red(),qcolor.green(),qcolor.red());
    set_backgroundcolour(colour,false,true);
    qcolor = forecolortemplateWidget->palette().color(forecolortemplateWidget->foregroundRole());
    colour.set_colour(qcolor.red(),qcolor.green(),qcolor.red());
    set_foregroundcolour(colour,false,true);

    QObject::connect(button_gotofirst,SIGNAL(clicked()),this,SLOT(slotpress_gotofirst()));
    QObject::connect(button_gotoprevious,SIGNAL(clicked()),this,SLOT(slotpress_gotoprevious()));
    QObject::connect(button_gotonext,SIGNAL(clicked()),this,SLOT(slotpress_gotonext()));
    QObject::connect(button_gotolast,SIGNAL(clicked()),this,SLOT(slotpress_gotolast()));
    QObject::connect(button_insert,SIGNAL(clicked()),this,SLOT(slotpress_insert()));
    QObject::connect(button_delete,SIGNAL(clicked()),this,SLOT(slotpress_delete()));
    QObject::connect(button_store,SIGNAL(clicked()),this,SLOT(slotpress_storechanges()));
    QObject::connect(label,SIGNAL(returnPressed()),this,SLOT(slotlineedit()));
    widget_specific_enable_disable();
}


hk_kderowselector::~hk_kderowselector()
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::~hk_kderowselector");
#endif
    delete layout;
    delete button_gotofirst;
    delete button_gotoprevious;
    delete button_gotonext;
    delete button_gotolast;
    delete label;
    delete button_delete;
    delete button_insert;

}


void hk_kderowselector::slotpress_gotofirst(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::slotpress_gotofirst");
#endif
    emit rowselector_clicked();
    emit firstrow_clicked();
    if (datasource()!=NULL)
        datasource()->goto_first();

}


void hk_kderowselector::slotpress_gotoprevious(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::slotpress_gotoprevious");
#endif
    emit rowselector_clicked();
    emit previousrow_clicked();
    if (datasource()!=NULL)
        datasource()->goto_previous();
}


void hk_kderowselector::slotpress_gotonext(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::slotpress_gotonext");
#endif
    emit rowselector_clicked();
    emit nextrow_clicked();

    if (datasource()!=NULL)
        datasource()->goto_next();
}


void hk_kderowselector::slotpress_gotolast(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::slotpress_gotolast");
#endif
    emit rowselector_clicked();
    emit lastrow_clicked();
    if (datasource()!=NULL)
        datasource()->goto_last();

}


void hk_kderowselector::slotpress_storechanges(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::slotpress_storechanges");
#endif
    if (datasource()!=NULL)
    {
        if (datasource()->is_enabled())
            datasource()->store_changed_data();
    }

}


void hk_kderowselector::slotpress_insert(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::slotpress_insert");
#endif
    emit rowselector_clicked();
    emit insertrow_clicked();

    if (datasource()!=NULL)
    {
//datasource()->mode(mode_insertrow);
        datasource()->setmode_insertrow();
    }

}


void hk_kderowselector::slotpress_delete(void)

{
#ifdef HKDEBUG
    hkdebug("hk_kderowselector::slotpress_delete");
#endif
    emit rowselector_clicked();
    emit deleterow_clicked();

    if (datasource()!=NULL)
    {
//datasource()->mode(mode_deleterow);
        datasource()->delete_actualrow();
    }

}


void hk_kderowselector::slotlineedit()
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::slotlineedit");
#endif
    emit rowselector_clicked();

    if (datasource()!=NULL)
       datasource()->goto_row(atoi( label->text().toUtf8().data())-1);

}


void hk_kderowselector::widget_specific_enable_disable(void)
{
    unsigned long p,q;
#ifdef HK_DEBUG
    hkdebug("kderowselector::widget_specific_enable_disable");
#endif
    if (datasource()==NULL)
    {
        button_gotofirst->setEnabled(false);
        button_gotoprevious->setEnabled(false);
        button_gotonext->setEnabled(false);
        button_gotolast->setEnabled(false);
        button_store->setEnabled(false);
        button_delete->setEnabled(false);
        button_insert->setEnabled(false);
        label->setText("");
        label->setEnabled(false);

        return;
    }

// change enable/disable

    p=datasource()->row_position();
    q=datasource()->max_rows();
//insertmode
    if (datasource()->mode()==hk_datasource::mode_insertrow)
    {
        button_gotofirst->setEnabled(false);
        button_gotoprevious->setEnabled(false);
        button_gotonext->setEnabled(false);
        button_gotolast->setEnabled(false);
        button_delete->setEnabled(false);
        return;
    }
// normalmode;
    unsigned long  buf;
    char* s= new char[50];

    if (datasource()!=NULL) buf=datasource()->row_position(); else buf=0;
    sprintf(s,"%lu",buf+1);
    label->setText(s);
    delete[] s;

    if ((buf==0)||((!datasource()->is_enabled())))
    {
        button_gotofirst->setEnabled(false);
        button_gotoprevious->setEnabled(false);
    }
    else
    {
        button_gotofirst->setEnabled(true);
        button_gotoprevious->setEnabled(true);
    }

    if ((p==q-1)||(!datasource()->is_enabled()))
    {
        button_gotonext->setEnabled(false);
        button_gotolast->setEnabled(false);
    }
    else
    {
        button_gotonext->setEnabled(true);
        button_gotolast->setEnabled(true);
    }
    if (!datasource()->is_enabled())label->setText(" ");
    label->setEnabled(datasource()->is_enabled());
    if ((datasource()->is_enabled())&&(!datasource()->is_readonly()&&!is_readonly()))
    {
        button_delete->setEnabled(true);
        button_insert->setEnabled(true);
        button_store->setEnabled(true);
    }
    else
    {
        button_delete->setEnabled(false);
        button_insert->setEnabled(false);
        button_store->setEnabled(false);
    }
}


bool hk_kderowselector::widget_specific_row_change(void)
{
    unsigned long p;
#ifdef HK_DEBUG
    hkdebug("kderowselector::widget_specific_row_change");
#endif
    if (datasource()!=NULL) p=datasource()->row_position(); else p=0;

    label->setText(ulongint2string(p+1).c_str());
    widget_specific_enable_disable();
    return true;
}


void    hk_kderowselector::widget_specific_insert_mode(void)
{
    widget_specific_enable_disable();
}


void hk_kderowselector::datasource_delete(void)
{

#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::datasource_delete");
#endif
    hk_dsvisible::datasource_delete();
    widget_specific_enable_disable();
}


bool hk_kderowselector::datasource_enable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kderowselector::datasource_enable");
#endif
    widget_specific_row_change();
    return true;
}


bool hk_kderowselector::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
    blockSignals(true);
    setGeometry(px,py,pwidth,pheight);
    blockSignals(false);
    return true;
}


void hk_kderowselector::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("kderowselector::resizeEvent");
#endif
    QWidget::resizeEvent(event);
    reposition();

}


void  hk_kderowselector::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("kderowselector::resizeEvent");
#endif
    QWidget::moveEvent(event);
    reposition();
}


void hk_kderowselector::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("kderowselector::reposition");
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


void    hk_kderowselector::widget_specific_backgroundcolour_changed(const hk_colour&)
{
    hk_colour c=backgroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(palette());
    
    p.setColor(backgroundRole(), newcolour);
    setPalette(p);
}


void    hk_kderowselector::widget_specific_foregroundcolour_changed(const hk_colour&)
{
    hk_colour c=foregroundcolour();
    QColor newcolour(c.red(),c.green(),c.blue());
    QPalette p(label->palette());
    
    p.setColor(label->foregroundRole(), newcolour);
    label->setPalette(p);    
}


void  hk_kderowselector::widget_specific_tooltip_changed(void)
{
    setToolTip(QString::fromUtf8 (l2u(tooltip()).c_str()));
}




void hk_kderowselector::focusInEvent ( QFocusEvent * e)
{
  action_on_getfocus();
  QWidget::focusInEvent(e);
}

void hk_kderowselector::focusOutEvent ( QFocusEvent * e)
{
  action_on_loosefocus();
  QWidget::focusOutEvent(e);
}

void hk_kderowselector::widget_specific_enabled_changed(void)
{
  setEnabled(is_enabled());
}

bool	hk_kderowselector::presentationmode_changed(void)
{
  bool result=hk_dsrowselector::presentationmode_changed();
  setEnabled(is_enabled());
  return result;
}


