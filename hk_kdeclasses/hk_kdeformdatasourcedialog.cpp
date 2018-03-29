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
//$Revision: 1.39 $
#include "hk_kdeformdatasourcedialog.h"
#include <hk_class.h>
#include <hk_form.h>
#include <hk_datasource.h>
#include <hk_database.h>
#include <qcombobox.h>
#include <vector>
#include <list>
#include <qpushbutton.h>
#include <qtoolbutton.h>

#include <qtextedit.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qtreewidget.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>
// TBP icons
class hk_kdeformdatasourcedialogprivate
{
public:
        hk_presentation* p_presentation;
        hk_datasource* p_datasource;
	hk_kdeformdatasourcedialog::enum_displaytype p_displaytype;
	hk_kdeformdatasourcedialog::enum_edittype p_edittype;
	hk_kdeformdatasourcedialog::enum_clickedbutton p_clickedbutton;
	bool p_allow_delete_datasource;

};



hk_kdeformdatasourcedialog::hk_kdeformdatasourcedialog( hk_presentation* p, QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
: hk_kdeformdatasourcebasedialog( parent, name, modal, fl )
{
    p_private=new hk_kdeformdatasourcedialogprivate;
    p_private->p_presentation=p;
    p_private->p_displaytype=d_form;
    p_private->p_allow_delete_datasource=true;
    p_private->p_edittype=e_all;
    p_private->p_clickedbutton=cb_cancel;
    set_datasource(NULL);
    hk_database* db=p_private->p_presentation->database();
    if (db)
    automaticupdatefield->setChecked(db->is_automatic_data_update());
    else
    automaticupdatefield->setChecked(true);
    KIconLoader* loader=KIconLoader::global();
    loader->addAppDir("hk_kde4classes");
    dependingfieldlist -> setColumnCount(2);
    dependingfieldlist -> setHeaderLabels(QStringList() << i18n("thisfield") << i18n("masterfield"));
    sourcetypefield->addItem(i18n("table"));
    sourcetypefield->addItem(i18n("query"));
    sourcetypefield->addItem(i18n("view"));

    dependingmodefield->addItem(i18n("standard"));
    dependingmodefield->addItem(i18n("no handle"));
    dependingmodefield->addItem(i18n("change"));
    dependingmodefield->addItem(i18n("delete"));
    dependingmodefield->addItem(i18n("change+delete"));
    dependingmodelabel->setText(i18n("depending mode"));
    react_on_changesfield->setText(i18n("react on master changes?"));
    readonlyfield->setText(i18n("is readonly?"));
    basedontext->setText(i18n("based on"));
    dependingontext->setText(i18n("depending on"));
    mastertext->setText(i18n("masterfield"));
    thistext->setText(i18n("thisfield"));
    filtertext->setText(i18n("filter"));
    sortingtext->setText(i18n("sorting"));
    Addbutton->setText(i18n("&Add"));
    alterbutton->setText(i18n("A&lter"));
    deletebutton->setText(i18n("&Delete"));
    buttonCancel->setText(i18n("&Cancel"));
    deletefieldbutton->setIcon(  loader->loadIcon("trash-empty",KIconLoader::NoGroup,KIconLoader::SizeSmall   ));
    addfieldbutton->setIcon(loader->loadIcon("go-next",KIconLoader::NoGroup,KIconLoader::SizeSmall   ) );
    
    sourcetypefield->setCurrentIndex(0);
    setWindowTitle(i18n("Datasource dialog"));    
    set_datasourcelist();
    datasource_selected();
    set_formdatasources();
    init();
}


hk_kdeformdatasourcedialog::~hk_kdeformdatasourcedialog()
{
delete p_private;
}



void hk_kdeformdatasourcedialog::init(void)
{
  if (p_private->p_presentation) return;
}





void hk_kdeformdatasourcedialog::set_datasourcelist()
{
    if (p_private->p_presentation==NULL) return;
    hk_database* db=p_private->p_presentation->database();
    if (db==NULL) return;
    vector<hk_string>* v=NULL;
    datasourcefield->clear();
    bool is_table=true;
    if (sourcetypefield->currentText()==i18n("table"))
    {
        v=db->tablelist();
	readonlyfield->setEnabled(true);
   }
    else     
    if (sourcetypefield->currentText()==i18n("query"))

    {
    v=db->querylist();
	readonlyfield->setEnabled(false);
        datasourcefield->addItem("");
	is_table=false;
   }
   else
   
   {
     v=db->viewlist();
     readonlyfield->setEnabled(false);
     is_table=true;
   }

    vector<hk_string>::iterator it=v->begin();
    while (it!=v->end())
    {
        datasourcefield->addItem(QString::fromUtf8 (l2u((*it)).c_str()));
        it++;
    }
    datasourcefield->setCurrentIndex(0);
    datasource_selected();
    Addbutton->setEnabled(is_table?!datasourcefield->currentText().isEmpty():true);

}


void hk_kdeformdatasourcedialog::datasource_selected()
{
    thisfield->clear();
    alterbutton->setEnabled(false);
    deletebutton->setEnabled(false);
    if (p_private->p_presentation==NULL) return;
    hk_database* db=p_private->p_presentation->database();
    if (db==NULL) return;
    QString txt=i18n("query");
    datasourcetype dt=dt_table;
  
    
    if (sourcetypefield->currentText()==txt) dt=dt_query;
    else
    {
      txt=i18n("view");
      if (sourcetypefield->currentText()==txt) dt=dt_view;
    }  
    
    hk_datasource* ds= db->load_datasource(u2l(datasourcefield->currentText().toUtf8().data()),dt);
    if (ds==NULL) return;
    sqlfield->setEnabled(false);
    if (dt==dt_query&& datasourcefield->currentText().isEmpty())  sqlfield->setEnabled(true);
    if (ds->type()!=hk_datasource::ds_table)
    {
        ds->set_temporaryfilter("0=1");
        ds->set_use_temporaryfilter(true);
        ds->enable();

    }
    sqlfield->blockSignals(true);
    sqlfield->setText(QString::fromUtf8(l2u(ds->sql()).c_str()));
    sqlfield->blockSignals(false);
    list<hk_column*>* v=ds->columns();

    if (v==NULL)
    {
        delete ds ;
        return;
    }
    thisfield->addItem("");
    list<hk_column*>::iterator it=v->begin();
    while (it!=v->end())
    {
        thisfield->addItem(QString::fromUtf8 (l2u((*it)->name()).c_str()));
        it++;
    }
    delete ds;
}


void hk_kdeformdatasourcedialog::add_dependingfields()
{
    dependingfieldlist->addTopLevelItem(new QTreeWidgetItem(QStringList() << thisfield->currentText() << masterfield->currentText()));
}


void hk_kdeformdatasourcedialog::delete_dependingfields()
{
    delete dependingfieldlist->currentItem();
    check_buttons();
}


void hk_kdeformdatasourcedialog::dependingon_selected()
{
    masterfield->clear();
    dependingfieldlist->clear();
    if (p_private->p_presentation==NULL)return;
    list<hk_datasource*>* dslist = p_private->p_presentation->datasources();
    if (dslist==NULL) return;

    hk_datasource* ds=NULL;

    list<hk_datasource*>::iterator nit=dslist->begin();
    while (nit!=dslist->end())
    {
        QString name= QString::fromUtf8 (l2u(p_private->p_presentation->unique_datasourcename((*nit)->presentationnumber())).c_str());
        if (name==dependingonfield->currentText())
        {
            ds=p_private->p_presentation->get_datasource((*nit)->presentationnumber()) ;
        }
        nit++;
    }

    if (ds==NULL) return;
    hk_string tempfilter=ds->temporaryfilter();
    bool usetempfilter=ds->use_temporaryfilter();
    if (ds->type()!=hk_datasource::ds_table)
    {
        ds->set_temporaryfilter("0=1");
        ds->set_use_temporaryfilter(true);
        ds->enable();
    }

    list<hk_column*>* v=ds->columns();
    if (v==NULL)
    {
        if (ds->type()!=hk_datasource::ds_table)
        {
            ds->disable();
            ds->set_use_temporaryfilter(false);
            ds->set_temporaryfilter(tempfilter);
            ds->set_use_temporaryfilter(usetempfilter);
        }
        return;
    }

    masterfield->addItem("");

    list<hk_column*>::iterator it=v->begin();
    while (it!=v->end())
    {
        masterfield->addItem(QString::fromUtf8 (l2u((*it)->name()).c_str()));
        it++;
    }

    if (ds->type()!=hk_datasource::ds_table)
    {
        ds->disable();
        ds->set_use_temporaryfilter(false);
        ds->set_temporaryfilter(tempfilter);
        ds->set_use_temporaryfilter(usetempfilter);
    }

}


void hk_kdeformdatasourcedialog::set_formdatasources(void)
{
    if (p_private->p_presentation==NULL) return;
    dependingonfield->clear();
    list<hk_datasource*>* dslist = p_private->p_presentation->datasources();
    if (dslist==NULL) return;
    list<hk_datasource*>::iterator it=dslist->begin();
    dependingonfield->addItem("");
    while (it!=dslist->end())
    {
        QString name= QString::fromUtf8 (l2u(p_private->p_presentation->unique_datasourcename((*it)->presentationnumber())).c_str());
        dependingonfield->addItem(name);
        it++;
    }

}


void hk_kdeformdatasourcedialog::check_buttons(void)
{
    if (
        dependingonfield->currentText().length()>0
        &&  thisfield->currentText().length()>0
        &&  masterfield->currentText().length()>0
	)

        addfieldbutton->setEnabled(true);
    else addfieldbutton->setEnabled(false);

    if ( dependingfieldlist->currentItem()==NULL) deletefieldbutton->setEnabled(false);
    else deletefieldbutton->setEnabled(true);

}


void   hk_kdeformdatasourcedialog::new_datasource(void)
{
    set_datasourceinfos(false);
    p_private->p_clickedbutton=cb_add;
    accept();
}


void hk_kdeformdatasourcedialog::set_datasourceinfos(bool existingdatasource)
{
// sets the values from the dialog into the hk_datasource

    if (p_private->p_presentation==NULL)  return;

    QString txt=i18n("query");
    datasourcetype dt=dt_table;
    if (sourcetypefield->currentText()==txt) dt=dt_query;
    else
    {
     txt=i18n("view");
     if (sourcetypefield->currentText()==txt) dt=dt_view;
    }
    
    hk_datasource* d=p_private->p_datasource;

    if (!existingdatasource)
    {
        long  id =p_private->p_presentation->new_datasource(u2l(datasourcefield->currentText().toUtf8().data()),dt);
        d=p_private->p_presentation->get_datasource(id);
	p_private->p_datasource=d;
    }
    if (d!=NULL)
    {
        d->clear_depending_fields();
        d->clear_filter();
        d->clear_sorting();
        d->set_readonly(readonlyfield->isChecked());
	d->set_depending_on_presentationdatasource(-1);
        d->set_automatic_data_update(automaticupdatefield->isChecked());
	if (d->name().length()==0)
		d->set_sql(u2l(sqlfield->toPlainText().toUtf8().data()));

        if (sortingfield->text().length()>0)
        {
            hk_string s = u2l(sortingfield->text().toUtf8().data());
            d->set_sorting(s);
        }
        if (filterfield->toPlainText().length()>0) d->set_filter(u2l(filterfield->toPlainText().toUtf8().data()));
        if (dependingonfield->currentText().length()>0 && dependingfieldlist->invisibleRootItem()->childCount()>0)
        {

            hk_datasource* depds=p_private->p_presentation->get_datasource(u2l(dependingonfield->currentText().toUtf8().data()));
            if (depds!=NULL)
            {
                hk_datasource::enum_dependingmodes mode=hk_datasource::depending_standard;
                if (dependingmodefield->currentIndex()==1)  mode=hk_datasource::depending_nohandle;
                else
                if (dependingmodefield->currentIndex()==2)  mode=hk_datasource::depending_change;
                    else
                    if (dependingmodefield->currentIndex()==3)  mode=hk_datasource::depending_delete;
                        else
                        if (dependingmodefield->currentIndex()==4)  mode=hk_datasource::depending_changedelete;

                d->set_depending_on_presentationdatasource(depds->presentationnumber(),react_on_changesfield->isChecked(),mode);
                QTreeWidgetItem* i=dependingfieldlist->invisibleRootItem()->child(0);
                while (i!=NULL)
                {
                    QString st1 = i->text(0);
                    QString st2 = i->text(1);
                    d->add_depending_fields(u2l(st1.toUtf8().data()),u2l(st2.toUtf8().data()));
                    i=dependingfieldlist->itemBelow(i);
                }
            }
        }
    }

}


void  hk_kdeformdatasourcedialog::alter_datasource(void)
{
    set_datasourceinfos(true);
    p_private->p_clickedbutton=cb_alter;
    accept();
}


void hk_kdeformdatasourcedialog::set_datasource(hk_datasource* d)
{
// sets the values from the hk_datasource into the dialog
    p_private->p_datasource=d;
    if (p_private->p_datasource==NULL)
    {
        alterbutton->setEnabled(false);
        deletebutton->setEnabled(false);
        return;
    }

    alterbutton->setEnabled(true);
    deletebutton->setEnabled(true);
    filterfield->setPlainText(QString::fromUtf8 (l2u(p_private->p_datasource->filter()).c_str()));
    sortingfield->setText(QString::fromUtf8 (l2u(p_private->p_datasource->sorting()).c_str()));
    readonlyfield->setChecked(p_private->p_datasource->is_readonly());
    automaticupdatefield->setChecked(p_private->p_datasource->is_automatic_data_update());
    switch (p_private->p_datasource->type())
    {
    case hk_data::ds_table: sourcetypefield->setCurrentIndex(0);break;
    case hk_data::ds_query: sourcetypefield->setCurrentIndex(1);break;
    default: sourcetypefield->setCurrentIndex(2);
    }
    
    set_datasourcelist();
    int c=0;
    QString t=QString::fromUtf8 (l2u(p_private->p_datasource->name()).c_str());
    while (c<datasourcefield->count())
    {
        if (datasourcefield->itemText(c)==t)
        {
            datasourcefield->setCurrentIndex(c);

        }
        c++;
    }
    datasource_selected();
    react_on_changesfield->setChecked(p_private->p_datasource->depending_on_react_on_data_changes());

    sqlfield->blockSignals(true);
    sqlfield->setText(QString::fromUtf8(l2u(p_private->p_datasource->sql()).c_str()));
    sqlfield->blockSignals(false);

    hk_datasource* depds=p_private->p_datasource->depending_on();
    if ( depds==NULL) return;

    c=0;
    t=QString::fromUtf8 (l2u(p_private->p_presentation->unique_datasourcename(depds->presentationnumber())).c_str());
    while (c<dependingonfield->count())
    {
        if (dependingonfield->itemText(c)==t)
        {
            dependingonfield->setCurrentIndex(c);
        }
        c++;
    }
    dependingon_selected();
    list<hk_string>* thislist = p_private->p_datasource->depending_on_thisfields();
    list<hk_string>* masterlist = p_private->p_datasource->depending_on_masterfields();

    list<hk_string>::iterator this_it=thislist->begin();
    list<hk_string>::iterator master_it=masterlist->begin();
    dependingfieldlist->clear();
    while ((this_it!=thislist->end())&&(master_it!=masterlist->end()))
    {
        dependingfieldlist->addTopLevelItem( new QTreeWidgetItem(
            QStringList() << QString::fromUtf8 (l2u((*this_it)).c_str()) << QString::fromUtf8 (l2u((*master_it)).c_str())) );
        master_it++;
        this_it++;
    }

    int m=0;
    switch (p_private->p_datasource->dependingmode())
    {
        case hk_datasource::depending_nohandle : m=1;
        break;
        case hk_datasource::depending_change   : m=2;
        break;
        case hk_datasource::depending_delete   : m=3;
        break;
        case hk_datasource::depending_changedelete : m=4;
        break;
        default                    :m=0;

    };

    dependingmodefield->setCurrentIndex(m);

}


void hk_kdeformdatasourcedialog::delete_datasource(void)
{
    if (p_private->p_allow_delete_datasource)
     {
      delete p_private->p_datasource;
      p_private->p_datasource=NULL;
     }
    p_private->p_clickedbutton=cb_delete;
    accept();
}

void hk_kdeformdatasourcedialog::set_displaytype(enum_displaytype d)
{
  switch (d)
  {
    case d_form :
    			dependingmodefield->show();
			react_on_changesfield->show();
			dependingmodelabel->show();
			sourcetypefield->setEnabled(true);
			Frame17->show();
			Frame6->show();
			readonlyfield->show();
    			break;
    case d_report :
    			dependingmodefield->hide();
			react_on_changesfield->hide();
			dependingmodelabel->hide();
			sourcetypefield->setEnabled(true);
			Frame17->show();
			Frame6->show();
			readonlyfield->show();
    			break;
    case d_query:
    			dependingmodefield->hide();
			react_on_changesfield->hide();
			dependingmodelabel->hide();
			Frame17->hide();
			Frame6->hide();
			sourcetypefield->setEnabled(true);
			readonlyfield->hide();
    			break;
  }
  
    sourcetypefield->clear();
    sourcetypefield->addItem(i18n("table"));
    if (d!=d_query)sourcetypefield->addItem(i18n("query"));
    sourcetypefield->addItem(i18n("view"));

  
  p_private->p_displaytype=d;
  adjustSize();
}

void hk_kdeformdatasourcedialog::sql_changed(void)
{

}


void hk_kdeformdatasourcedialog::set_edittype(enum_edittype t)
{
   p_private->p_edittype=t;
   switch (t)
   {
     case  e_all:
     case  e_add:
     			sourcetypefield->setEnabled(true);
			datasourcefield->setEnabled(true);
     			break;
     case  e_modify:
     			sourcetypefield->setEnabled(false);
			datasourcefield->setEnabled(false);
     			break;

   }

}

hk_datasource* hk_kdeformdatasourcedialog::datasource(void)
{
  return p_private->p_datasource;
}



void hk_kdeformdatasourcedialog::set_allow_delete_datasource(bool a)
{
  p_private->p_allow_delete_datasource=a;
}

bool hk_kdeformdatasourcedialog::allow_delete_datasource(void) const
{
  return p_private->p_allow_delete_datasource;
}

hk_kdeformdatasourcedialog::enum_edittype hk_kdeformdatasourcedialog::edittype(void) const
{
   return p_private->p_edittype;
}

hk_kdeformdatasourcedialog::enum_clickedbutton hk_kdeformdatasourcedialog::clicked_button(void) const
{
  return p_private->p_clickedbutton;
}

hk_kdeformdatasourcedialog::enum_displaytype hk_kdeformdatasourcedialog::displaytype(void) const
{
  return p_private->p_displaytype;
}
