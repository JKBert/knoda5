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

#include "hk_kdereportsectiondialog.h"
#include <hk_class.h>
#include <hk_report.h>
#include <hk_kdesimplereport.h>
#include <hk_reportsection.h>
#include <hk_reportsectionpair.h>
#include <hk_datasource.h>
#include <hk_column.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlistwidget.h>
#include <qapplication.h>
#include <qtoolbutton.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>

hk_kdereportsectiondialog::hk_kdereportsectiondialog( hk_kdesimplereport* r,QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdereportsectiondialogbase( parent, name, modal, fl )
{
    fieldlabel->setText(i18n("Section to add:"));
    boxlabel->setText(i18n("Existing sections"));
    p_report=r;
    QIcon::setThemeName("oxygen");
    
    deletebutton->setIcon(QIcon::fromTheme("trash-empty"));
    addbutton->setIcon(QIcon::fromTheme("go-next"));
    upbutton->setIcon(QIcon::fromTheme("go-up"));
    downbutton->setIcon(QIcon::fromTheme("go-down"));

    set_sectionlist();
    set_fieldlist();
}


hk_kdereportsectiondialog::~hk_kdereportsectiondialog()
{
}


void hk_kdereportsectiondialog::add_clicked()
{
    int p=sectionbox->currentRow();
    if (p<0)p=0;
    hk_reportsectionpair* s=p_report->new_sectionpair_at(p);
    s->set_columnname(u2l(sectionfield->currentText().toUtf8().data()));
    sectionbox->insertItem(p,sectionfield->currentText());
    p_report->set_sections();
    qApp->processEvents();
}


void hk_kdereportsectiondialog::delete_clicked()
{
    int p=sectionbox->currentRow();
    QListWidgetItem* p_cur;
    
    if (p<0)
        return;
    hk_reportsectionpair* s=(*p_report->sectionpairs())[p];
    delete s;
    p_report->set_sections();
    qApp->processEvents();
    p_cur = sectionbox->takeItem(p);
    delete p_cur;
}


void hk_kdereportsectiondialog::down_clicked()
{
    int c = sectionbox->currentRow();
    QListWidgetItem* p_cur = sectionbox->takeItem(c);

    p_report->move_sectionpair(c, 1);
    p_report->has_changed(true);
    p_report->set_sections();
    sectionbox->insertItem(c + 1, p_cur);
    sectionbox->setCurrentRow(c + 1);
    qApp->processEvents(); 
}


void hk_kdereportsectiondialog::up_clicked()
{
    int c = sectionbox->currentRow();
    QListWidgetItem* p_cur = sectionbox->takeItem(c);
      
    p_report->move_sectionpair(c, -1);
    p_report->has_changed(true);
    p_report->set_sections();
    sectionbox->insertItem(c - 1,p_cur);
    sectionbox->setCurrentRow(c - 1);   
    qApp->processEvents(); 
}


void hk_kdereportsectiondialog::set_sectionlist(void)
{
    sectionbox->clear();
    if (p_report==NULL) return;
    vector<hk_reportsectionpair*>* p= p_report->sectionpairs();
    vector<hk_reportsectionpair*>::iterator it=p->begin();
    while(it!=p->end())
    {
	    sectionbox->addItem(QString::fromUtf8(l2u((*it)->columnname()).c_str()));
        it++;
    }
}


void hk_kdereportsectiondialog::set_fieldlist(void)
{
    sectionfield->clear();
    sectionfield->addItem("");
    if (p_report==NULL) return;
    hk_datasource* d=p_report->datasource();
    if (d==NULL) return;
    hk_string tempfilter;
    if (d->type()!=hk_datasource::ds_table)
    {
        tempfilter=d->temporaryfilter();
        d->set_temporaryfilter("0=1");
        d->set_use_temporaryfilter(true);
        d->enable();
    }
    list<hk_column*>* cols=d->columns();
    if (cols==NULL)
    {
        if (d->type()!=hk_datasource::ds_table)
        {
            d->disable();
            d->set_use_temporaryfilter(false);
            d->set_temporaryfilter(tempfilter);
        }
        return;
    }
    list<hk_column*>::iterator it=cols->begin();
    while (it!=cols->end())
    {
        sectionfield->addItem(QString::fromUtf8(l2u((*it)->name()).c_str()));
        it++;
    }

    if (d->type()!=hk_datasource::ds_table)
    {
        d->disable();
        d->set_use_temporaryfilter(false);
        d->set_temporaryfilter(tempfilter);
    }
    check_buttons();
}


void hk_kdereportsectiondialog::check_buttons(void)
{
  deletebutton->setEnabled(sectionbox->currentRow()!=-1);
  upbutton->setEnabled(sectionbox->currentRow()>0);
  downbutton->setEnabled(sectionbox->currentRow()>-1 &&sectionbox->currentRow()<(signed)sectionbox->count()-1);
  addbutton->setEnabled(!sectionfield->currentText().isEmpty());
}
