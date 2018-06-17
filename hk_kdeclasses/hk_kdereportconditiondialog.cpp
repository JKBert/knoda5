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

#include "hk_kdereportconditiondialog.h"
#include <hk_report.h>
#include <hk_form.h>
#include <hk_button.h>
#include <hk_database.h>

#include <qcombobox.h>
#include <qpushbutton.h>
#include <qtreewidget.h>
#include <qlabel.h>
#include <kiconloader.h>
#include <klocale.h>
#include <qheaderview.h>

hk_kdereportconditiondialog::hk_kdereportconditiondialog( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdereportconditiondialogbase( parent, name, modal, fl )
{
    QIcon::setThemeName("oxygen");
    deletebutton->setIcon(QIcon::fromTheme("trash-empty"));
    addbutton->setIcon(QIcon::fromTheme("go-next")); 
    formdatasourcelabel->setText( i18n( "Formdatasource:" ) );
    formfieldlabel->setText( i18n( "Formfield:" ) );

    check_buttons();
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdereportconditiondialog::~hk_kdereportconditiondialog()
{
// no need to delete child widgets, Qt does it all for us
}


/*
 * public slot
 */
void hk_kdereportconditiondialog::add_clicked()
{
    new QTreeWidgetItem(conditionlist,
			QStringList() << reportfield->currentText() << conditionfield->currentText() << 
			formdatasourcefield->currentText() << formfieldfield->currentText());
    check_buttons();
}


/* 
 * public slot
 */
void hk_kdereportconditiondialog::delete_clicked()
{
    delete conditionlist->currentItem();
    check_buttons();
}


void    hk_kdereportconditiondialog::set_values(hk_button* button, hk_form* form,const hk_string& r, enum_type t)
{
    p_form=form;
    p_report=r;
    p_button=button;
    if (!p_form||!p_button) return;
    if (t==hk_kdereportconditiondialog::form) set_formvalues();
    else     set_reportvalues();
    conditionlist-> setHeaderLabels(QStringList() << (t==hk_kdereportconditiondialog::form?i18n("Masterfield")
    							:i18n("Reportfield")) << i18n("condition")
				   << i18n("formdatasource") << i18n("formfield"));
    setWindowTitle( t==hk_kdereportconditiondialog::form?i18n("Formconditiondialog"):i18n( "Reportconditiondialog" ) );
    conditionlabel->setText( i18n( "Condition:" ) );
    reportlabel->setText(t==hk_kdereportconditiondialog::form?i18n("Masterfield"): i18n( "Reportfield:" ) );

    set_formdatasources();
    set_formfields();
    set_buttonvalues();
    conditionlist -> header() -> resizeSections(QHeaderView::ResizeToContents);
}


void hk_kdereportconditiondialog::set_reportvalues(void)
{

    hk_report* r = new hk_report();
    r->set_database(p_form->database());
    r->load_report(p_report);

    reportfield->blockSignals(true);
    reportfield->clear();
    reportfield->blockSignals(false);
    hk_database* db=r->database();
    if (db==NULL) return;
    hk_datasource* ds= r->datasource();
    if (ds==NULL) return;
    list<hk_column*>* v=NULL;
    hk_datasource* tmpds=NULL;

    if (ds->type()==hk_datasource::ds_query)
    {
        tmpds=ds->database()->new_resultquery();
        if (tmpds)
        {
            tmpds->set_sql(ds->sql());
            tmpds->set_filter("1=0");
            tmpds->enable();
            v=tmpds->columns();

        }

    }
    else    v=ds->columns();
    if (v==NULL)
    {
        if (tmpds)
        {
            tmpds->disable();
            delete tmpds;
        }
        return;
    }
    reportfield->addItem("");
    list<hk_column*>::iterator it=v->begin();
    while (it!=v->end())
    {
        reportfield->addItem(QString::fromUtf8(l2u((*it)->name()).c_str()));
        it++;
    }
    if (tmpds)
    {
        tmpds->disable();
        delete tmpds;
    }

    delete r;
    check_buttons();
}

void hk_kdereportconditiondialog::set_formvalues(void)
{

    hk_form* r = new hk_form();
    r->set_database(p_form->database());
    r->load_form(p_report);

    reportfield->blockSignals(true);
    reportfield->clear();
    reportfield->blockSignals(false);
    hk_database* db=r->database();
    if (db==NULL) return;
    hk_datasource* ds= r->datasource();
    if (ds==NULL) return;
    list<hk_column*>* v=NULL;
    hk_datasource* tmpds=NULL;

    if (ds->type()==hk_datasource::ds_query)
    {
        tmpds=ds->database()->new_resultquery();
        if (tmpds)
        {
            tmpds->set_sql(ds->sql());
            tmpds->set_filter("1=0");
            tmpds->enable();
            v=tmpds->columns();

        }

    }
    else    v=ds->columns();
    if (v==NULL)
    {
        if (tmpds)
        {
            tmpds->disable();
            delete tmpds;
        }
        return;
    }
    reportfield->addItem("");
    list<hk_column*>::iterator it=v->begin();
    while (it!=v->end())
    {
        reportfield->addItem(QString::fromUtf8(l2u((*it)->name()).c_str()));
        it++;
    }
    if (tmpds)
    {
        tmpds->disable();
        delete tmpds;
    }

    delete r;
    check_buttons();
}

void hk_kdereportconditiondialog::set_formdatasources(void)
{

    if (p_form==NULL) return;
    formdatasourcefield->blockSignals(true);
    formdatasourcefield->clear();
    formdatasourcefield->addItem("");
    formfieldfield->blockSignals(true);
    formfieldfield->clear();
    formfieldfield->blockSignals(false);

    list<hk_datasource*>* dslist = p_form->datasources();
    if (dslist==NULL)
    {
        formdatasourcefield->blockSignals(false);
        return;
    }
    list<hk_datasource*>::iterator it=dslist->begin();
    while (it!=dslist->end())
    {
        QString name= QString::fromUtf8 (l2u(p_form->unique_datasourcename((*it)->presentationnumber())).c_str());
        formdatasourcefield->addItem(name);
        it++;
    }
    formdatasourcefield->blockSignals(false);

}


void hk_kdereportconditiondialog::set_formfields(void)
{
    setWindowTitle("");
    formfieldfield->blockSignals(true);
    formfieldfield->clear();
    formfieldfield->blockSignals(false);
    if (p_form==NULL) return;
    hk_database* db=p_form->database();
    if (db==NULL) return;
    QString f=formdatasourcefield->currentText();
    if (f.isEmpty()) f="";
    hk_datasource* ds= p_form->get_datasource(u2l(f.toUtf8().data()));
    if (ds==NULL) return;
    list<hk_column*>* v=NULL;
    hk_datasource* tmpds=NULL;

    if (ds->type()==hk_datasource::ds_query)
    {
        tmpds=ds->database()->new_resultquery();
        if (tmpds)
        {
            tmpds->set_sql(ds->sql());
            tmpds->set_filter("1=0");
            tmpds->enable();
            v=tmpds->columns();

        }

    }
    else    v=ds->columns();
    if (v==NULL)
    {
        if (tmpds)
        {
            tmpds->disable();
            delete tmpds;
        }
        return;
    }
    formfieldfield->blockSignals(true);
    formfieldfield->addItem("");
    list<hk_column*>::iterator it=v->begin();
    while (it!=v->end())
    {
        formfieldfield->addItem(QString::fromUtf8 (l2u((*it)->name()).c_str()));

        it++;
    }
    formfieldfield->blockSignals(false);
    if (tmpds)
    {
        tmpds->disable();
        delete tmpds;
    }

    check_buttons();

}


void hk_kdereportconditiondialog::formdatasource_changed(void)
{
    set_formfields();
    formfieldfield->setEditable(formdatasourcefield->currentText().length()==0);
    check_buttons();
}


void hk_kdereportconditiondialog::check_buttons(void)
{

    if (
        reportfield->currentText().length()>0
        &&  formfieldfield->currentText().length()>0
        )
        addbutton->setEnabled(true);
    else addbutton->setEnabled(false);

    deletebutton->setEnabled(conditionlist->currentItem()!=NULL);
//     buttonOk->setEnabled(conditionlist->childCount()>0);
}


void hk_kdereportconditiondialog::formfield_change()
{

    check_buttons();
}


void hk_kdereportconditiondialog::reportfield_changed()
{
    check_buttons();

}


void hk_kdereportconditiondialog::set_buttonvalues(void)
{

    if (!p_button) return;
    list<hk_button::struct_condition>* l=p_button->conditions();
    list<hk_button::struct_condition>::iterator it=l->begin();
    while (it!=l->end())
    {
        new QTreeWidgetItem(conditionlist,
            QStringList() << QString::fromUtf8 (l2u((*it).reportfield).c_str()) <<
            QString::fromUtf8 (l2u((*it).condition).c_str()) <<
            QString::fromUtf8 (l2u((*it).formdatasource).c_str()) <<
            QString::fromUtf8 (l2u((*it).formfield).c_str()));

        ++it;
    }
    check_buttons();

}


void hk_kdereportconditiondialog::accept(void)
{
    p_button->clear_conditions();

    QTreeWidgetItem* i=conditionlist-> invisibleRootItem() -> child(0);
    while (i!=NULL)
    {
        QString st1 = i->text(0);
        QString st2 = i->text(1);
        QString st3 = i->text(2);
        QString st4 = i->text(3);
        p_button->add_condition(u2l(st1.toUtf8().data()),u2l(st2.toUtf8().data()),u2l(st3.toUtf8().data()),u2l(st4.toUtf8().data()));
        i=conditionlist->itemBelow(i);
    }

    hk_kdereportconditiondialogbase::accept();
}
