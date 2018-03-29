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
//$Revision: 1.12 $
#include "hk_kdesubreportdialog.h"
#include <hk_string.h>
#include <hk_reportsection.h>
#include <hk_report.h>
#include <hk_database.h>
#include <hk_datasource.h>
#include <vector>
#include <qcombobox.h>
#include <qtreewidget.h>
#include <qheaderview.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>
//TBP icons
/*
 *  Constructs a hk_kdesubreportdialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdesubreportdialog::hk_kdesubreportdialog( hk_reportsection* s,QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdesubreportdialogbase( parent, name, modal, fl )
{
    KIconLoader* loader=KIconLoader::global();
    loader->addAppDir("hk_kde4classes");
    dependingfieldlist -> setHeaderLabels(QStringList() << i18n("subreportfield") << i18n("masterreportfield"));
    dependingfieldlist -> header() -> resizeSections(QHeaderView::ResizeToContents);
    dependingfieldlist -> header() -> setStretchLastSection(false);
    mastertext->setText(i18n("masterreportfield:"));
    subreporttext->setText(i18n("subreportfield:"));
    subreportlabel->setText(i18n("Subreport:"));
    setbutton->setText(i18n("&Set"));
    deletebutton->setText(i18n("&Delete"));
    buttonCancel->setText(i18n("&Cancel"));
    printbeforefield->setText(i18n("Before sectiondata?"));
    p_section=s;
    p_report=s==NULL?NULL:s->report();
    set_masterfield();
    set_subreportlist();
    subreport_selected();
    set_reportsection(s);
    setbutton->setEnabled(true);
    QString buffer=i18n("Subreport dialog");
    if  (p_report!=NULL)
    {
        buffer+=" - ";
        buffer+=QString::fromUtf8(l2u(p_report->name()).c_str());
    }
    setWindowTitle(buffer);

    deletefieldbutton->setIcon(  loader->loadIcon("trash-empty",KIconLoader::NoGroup,KIconLoader::SizeSmall));
    addfieldbutton->setIcon(loader->loadIcon("go-next",KIconLoader::NoGroup,KIconLoader::SizeSmall ) );

}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdesubreportdialog::~hk_kdesubreportdialog()
{
// no need to delete child widgets, Qt does it all for us
}


/*
 * public slot
 */
void hk_kdesubreportdialog::add_dependingfields()
{
    dependingfieldlist->addTopLevelItem( new QTreeWidgetItem(
        QStringList() << thisfield->currentText() << masterfield->currentText()) );

    check_buttons();
}


/*
 * public slot
 */
void hk_kdesubreportdialog::check_buttons()
{
    if (
        subreportfield->currentText().length()>0
        &&  thisfield->currentText().length()>0
        &&  masterfield->currentText().length()>0
        )
        addfieldbutton->setEnabled(true);
    else addfieldbutton->setEnabled(false);
    if ( dependingfieldlist->currentItem()==NULL) deletefieldbutton->setEnabled(false);
    else deletefieldbutton->setEnabled(true);
    if (p_section!=NULL)
    {
        deletebutton->setEnabled(p_section->subreportname().size()>0);
    } else deletebutton->setEnabled(false);
}


/*
 * public slot
 */
void hk_kdesubreportdialog::subreport_selected()
{
    thisfield->clear();
    if (p_section==NULL) return;
    if (subreportfield->currentText().isEmpty())return;
    hk_report* r= new hk_report();
    r->set_database(p_report->database());
    r->load_report(u2l(subreportfield->currentText().toUtf8().data()));
    hk_datasource* ds= r->datasource();
    if (ds==NULL) return;
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
        thisfield->addItem(QString::fromUtf8(l2u((*it)->name()).c_str()));
        it++;
    }
    delete r;

}


/*
 * public slot
 */
void hk_kdesubreportdialog::delete_dependingfields()
{
    delete dependingfieldlist->currentItem();
    check_buttons();
}


/*
 * public slot
 */
void hk_kdesubreportdialog::dependingon_selected()
{
    qWarning( "hk_kdesubreportdialog::dependingon_selected() not yet implemented!" );
}


/*
 * public slot
 */
void hk_kdesubreportdialog::set_subreport()
{
    if (p_section!=NULL)
    {
        p_section->set_subreport(u2l(subreportfield->currentText().toUtf8().data()),printbeforefield->isChecked());
        p_section->clear_depending_fields();
	QTreeWidgetItem* i=dependingfieldlist->invisibleRootItem() -> child(0);
        while (i!=NULL)
        {
            QString st1 = i->text(1);
            QString st2 = i->text(0);
            p_section->add_depending_fields(u2l(st1.toUtf8().data()),u2l(st2.toUtf8().data()));
            i= dependingfieldlist->itemBelow(i);
        }

    }

    accept();

}


/*
 * public slot
 */
void hk_kdesubreportdialog::delete_subreport()
{
    if (p_section!=NULL)
    {
        p_section->set_subreport("");
        p_section->clear_depending_fields();
    }

    accept();
}


/*
 * public slot
 */
void hk_kdesubreportdialog::set_subreportlist()
{
    if (p_section==NULL) return;
    if (p_report->database()==NULL) return;
    subreportfield->clear();
    vector<hk_string>* l=p_report->database()->reportlist();
    vector<hk_string>::iterator it=l->begin();

    while(it!=l->end())
    {
        if ((*it)!=p_report->name())subreportfield->addItem(QString::fromUtf8(l2u((*it)).c_str()));
        it++;
    }
}


void hk_kdesubreportdialog::set_masterfield(void)
{
    masterfield->clear();
    if (p_report==NULL) return;
    if (p_report->datasource()==NULL) return;
    list<hk_column*>* v=p_report->datasource()->columns();
    if (v==NULL) return;
    masterfield->addItem("");

    list<hk_column*>::iterator it=v->begin();
    while (it!=v->end())
    {
        masterfield->addItem(QString::fromUtf8(l2u((*it)->name()).c_str()));
        it++;
    }

}


void hk_kdesubreportdialog::set_reportsection(hk_reportsection* s)
{
    p_section=s;
    p_report=s==NULL?NULL:s->report();
    subreportfield->clear();
    thisfield->clear();
    dependingfieldlist->clear();
    set_subreportlist();
    set_masterfield();

    if (s==NULL) return;
    int c=0;
    QString t=QString::fromUtf8(l2u(p_section->subreportname()).c_str());
    while (c<subreportfield->count())
    {
        if (subreportfield->itemText(c)==t)
        {
            subreportfield->setCurrentIndex(c);
        }
        c++;
    }
    printbeforefield->setChecked(p_section->print_subreport_before_data());
    subreport_selected();
    list<hk_string>* thislist = p_section->depending_on_thisreportfields();
    list<hk_string>* sublist = p_section->depending_on_subreportfields();

    list<hk_string>::iterator this_it=thislist->begin();
    list<hk_string>::iterator sub_it=sublist->begin();
    dependingfieldlist->clear();

    while ((this_it!=thislist->end())&&(sub_it!=sublist->end()))
    {
        dependingfieldlist->addTopLevelItem( new QTreeWidgetItem(QStringList() << QString::fromUtf8(l2u((*sub_it)).c_str()) <<
        QString::fromUtf8(l2u((*this_it)).c_str())) );
	sub_it++;
        this_it++;
    }

    check_buttons();
}
