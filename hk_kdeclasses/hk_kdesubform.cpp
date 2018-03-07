// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2013 Patrik Hanak <hanakp@users.sourceforge.net>
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

#include "hk_kdesubform.h"
#include "hk_kdesimpleform.h"

#include <qapplication.h>

hk_kdesubform::hk_kdesubform(hk_kdesimpleform* f):QScrollArea(f),hk_subform(f)
{
   hk_kdesimpleform* subf= new hk_kdesimpleform(viewport());
   setWidget(subf);
   set_subform(subf);
   subf->set_viewmode();
   setFrameStyle(QFrame::Panel|QFrame::Sunken);
}

hk_kdesubform::~hk_kdesubform()
{

}

bool hk_kdesubform::widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesubform::widget_specific_coordinates");
#endif
    setGeometry(px,py,pwidth,pheight);
    qApp->processEvents();

    return true;
}

void hk_kdesubform::resizeEvent ( QResizeEvent * event )
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesubform::resizeEvent");
#endif
    QScrollArea::resizeEvent(event);
    reposition();

}


void  hk_kdesubform::moveEvent ( QMoveEvent * event)
{
#ifdef HK_DEBUG
    hkdebug("kdelineedit::resizeEvent");
#endif
    QScrollArea::moveEvent(event);
    reposition();
}

void hk_kdesubform::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdesubform::reposition");
#endif
    if (p_presentation==NULL ||p_setcoordinates==true) return;
    p_setwidgetcoordinates=true;
    unsigned int ppx=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QScrollArea::x()):QScrollArea::x();
    unsigned int ppy=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QScrollArea::y()):QScrollArea::y();
    unsigned int pph=p_presentation->sizetype()==hk_presentation::relative?p_presentation->vertical2relativ(QScrollArea::height()):QScrollArea::height();
    unsigned int ppw=p_presentation->sizetype()==hk_presentation::relative?p_presentation->horizontal2relativ(QScrollArea::width()):QScrollArea::width();
    set_size(ppx,ppy,ppw,pph);
    p_setwidgetcoordinates=false;
}

bool hk_kdesubform::presentationmode_changed(void)
{
 bool result=hk_subform::presentationmode_changed();
 if (p_presentation && p_presentation->mode()==hk_presentation::filtermode)
 {
   setEnabled(false);
   return result;
 }
 else setEnabled(is_enabled());
 if (!result) return false;
 if (p_presentation && p_presentation->mode()==hk_presentation::viewmode)
    ((hk_kdesimpleform*)(subform()))-> repaint();
return true;
}

void hk_kdesubform::show_widget(void)
{
  show();
}

void hk_kdesubform::hide_widget(void)
{
  hide();
}

bool hk_kdesubform::close_widget(void)
{
  return close();
}


void hk_kdesubform::widget_specific_enabled_changed(void)
{
  setEnabled(is_enabled());
}

