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
//Version $Revision: 1.33 $

#include "hk_kdetoolbar.h"
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qtooltip.h>
#include "hk_kdefilterdialog.h"
#include <qtextedit.h>
#include <hk_datasource.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>
#include <qpushbutton.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kparts/mainwindow.h>
#include <ktoggleaction.h>

hk_kdetoolbar::hk_kdetoolbar(QWidget* w,const char* s) :QObject(w) ,hk_dsvisible()
{
#ifdef HK_DEBUG
    hkclassname("hk_kdetoolbar");
    hkdebug("hk_kdetoolbar::hk_kdetoolbar");
#endif
    if (s)
        setObjectName(QString::fromAscii(s));
    p_filterdefinitionaction=NULL;
    p_filterexecaction=NULL;
    p_parent=w;
    p_use_filter=false;
    p_filter_should_be_activated=p_is_activated=false;
    p_viewmode=false;
    p_nodesignmode=false;
    if (runtime_only()) set_nodesignmode(true);
    set_designmode();
}


hk_kdetoolbar::~hk_kdetoolbar(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::~hk_kdetoolbar");
#endif

}


void hk_kdetoolbar::designbutton_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::designbutton_clicked");
#endif
    set_designmode();
}


void hk_kdetoolbar::viewbutton_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::viewbutton_clicked");
#endif

    set_viewmode();

}


void hk_kdetoolbar::set_designmode(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::set_designmode");
#endif
    p_viewmode=false;
//   if (p_filterexecaction)p_filterexecaction->setEnabled(false);
    deactivate_filter();
//  if (p_filterdefinitionaction)p_filterdefinitionaction->setEnabled(false);
}


void hk_kdetoolbar::set_viewmode(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::set_viewmode");
#endif
    p_viewmode=true;
    if (!datasource())return;
    if( p_filter_should_be_activated) activate_filter();
    if (p_filterexecaction)
    { 
    //cerr <<"TOOLBAR set_viewmode"<<endl;
        p_filterexecaction->setEnabled(true);
    }
// if (p_filterdefinitionaction)p_filterdefinitionaction->setEnabled((p_filterexecaction?!p_filterexecaction->isChecked():true));

}


void hk_kdetoolbar::set_nodesignmode(bool d)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::set_nodesignmode");
#endif
    p_nodesignmode=d;
}


void hk_kdetoolbar::filterdefinebutton_clicked()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::filterdefinebutton_clicked");
#endif
    hk_kdefilterdialog* f = new hk_kdefilterdialog(hk_kdefilterdialog::filter,p_parent,0,true);
    f->textfield->setPlainText(p_filter);
    if (f->exec()==QDialog::Accepted)
        set_filter(f->textfield->toPlainText());
    p_filterdefinitionaction->setChecked(!p_filterdefinitionaction->isChecked());
    p_is_activated=false;
    if (!p_filterdefinitionaction->isChecked() && p_filterexecaction->isChecked())
       filterexecbutton_clicked();
    delete f;
}


void hk_kdetoolbar::set_filter(const QString& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::set_filter(QString)");
#endif
    if (!datasource())return;
    p_filter=s; //cerr <<"SETFILTER:"<<s<<endl;
    p_use_filter=!p_filter.isEmpty();
    if (p_filterexecaction)p_filterexecaction->setEnabled(true);

    datasource()->set_temporaryfilter(u2l(p_filter.toUtf8().data()));
    if (p_filterexecaction->isChecked())
    {
        p_is_activated=false;
        filterexecbutton_clicked();
    }
    if (!p_filterexecaction->isEnabled())
    {
        p_filterexecaction->setChecked(false);
        p_is_activated=false;
    }
}


void hk_kdetoolbar::set_filter(bool s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::set_filter(bool)");
#endif
    if (!datasource())return;
    p_filter=""; //cerr <<"SETFILTER"<<endl;
    p_use_filter=s;
    if (p_filterexecaction)p_filterexecaction->setEnabled(true);

    if (p_filterexecaction->isChecked())
    {
        p_is_activated=false;
//         filterexecbutton_clicked();
    }
    if (!p_filterexecaction->isEnabled())
    {
        p_filterexecaction->setChecked(false);
        p_is_activated=false;
    }
}


void hk_kdetoolbar::filterexecbutton_clicked()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::filterexecbutton_clicked");
#endif
    if (datasource()==NULL||!p_filterexecaction)return;

    if (p_filterexecaction->isChecked() )
    {
        if (!p_filterdefinitionaction->isChecked())
         activate_filter();
        p_filter_should_be_activated=true;

    }
    else
    {
        deactivate_filter();
        p_filter_should_be_activated=false;

    }
}


void hk_kdetoolbar::deactivate_filter(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::deactivate_filter");
#endif
    if (datasource())p_filterdefinitionaction->setEnabled(true);
    if (p_filterexecaction)p_filterexecaction->setChecked(false);
    if (datasource()==NULL) return;
    if (!p_is_activated)return;
    datasource()->disable();
    datasource()->set_use_temporaryfilter(false);
//   datasource()->set_filter(p_originalfilter,false);
    if (p_viewmode &&! p_filterdefinitionaction->isChecked())   datasource()->enable();
    p_is_activated=false;

}


void hk_kdetoolbar::activate_filter(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::activate_filter");
#endif
//  if (p_filterdefinitionaction)p_filterdefinitionaction->setEnabled(false);
    if (p_filterexecaction)p_filterexecaction->setChecked(true);
    if (datasource()==NULL) return;
    if (p_is_activated) return;
    datasource()->disable();
    if (!p_filter.isEmpty())
       datasource()->set_temporaryfilter(u2l(p_filter.toUtf8().data()));
    datasource()->set_use_temporaryfilter(true);
    if (p_viewmode) datasource()->enable();
    p_filter_should_be_activated=p_is_activated=true;
}


bool hk_kdetoolbar::is_filter_activated(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetoolbar::is_filter_activated");
#endif
    return p_is_activated;
}


void hk_kdetoolbar::set_filteractions(KToggleAction* filterdefinition,KToggleAction* filterexec)
{
    p_filterdefinitionaction=filterdefinition;
    p_filterexecaction=filterexec;
    if (p_filterdefinitionaction)
        p_filterdefinitionaction->setToolTip(i18n( "Filter definition" ) );
    if (p_filterexecaction)
    {
        p_filterexecaction->setToolTip(i18n( "Filter execution" ));
        p_filterexecaction->setEnabled(true);
    }

}
