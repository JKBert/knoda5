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

#include "hk_kdegridcolumndialog.h"
#include "hk_kdegrid.h"
#include "hk_kdeinterpreterdialog.h"
#include <hk_datasource.h>
#include <hk_class.h>
#include <hk_database.h>
#include <hk_presentation.h>
#include <qlineedit.h>
#include <qlistwidget.h>
#include <qtreewidget.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kglobal.h>
#include <klocale.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qscrollarea.h>
#include <qheaderview.h>

class  fieldstruct
{
      public:
        fieldstruct(void)
        {
           use_separators=false;
           readonly=false;
           listdatasourcetype=dt_table;
           digits=-1;
           columntype=hk_dsgridcolumn::columnedit;
           alignment=hk_dsdatavisible::alignleft;
         }
         hk_string name;
         hk_string defaultvalue;
         hk_string identifier;
         hk_dsdatavisible::alignmenttype alignment;
         bool use_separators;
         int digits;
         datasourcetype listdatasourcetype;
         bool readonly;
         hk_string listdatasource;
         hk_string listcolumn;
         hk_string viewcolumn;
         hk_string displayname;
         hk_dsgridcolumn::enum_columntype columntype;
		 hk_string p_before_row_change_action;
		 hk_string p_after_row_change_action;
		 hk_string p_after_delete_action,p_before_delete_action;
		 hk_string p_after_update_action,p_before_update_action;
	     hk_string p_after_insert_action,p_before_insert_action;
		 hk_string p_clickaction,p_doubleclickaction, p_onopenaction,p_oncloseaction;
		 hk_string p_on_getfocus_action,p_on_loosefocus_action,p_on_key_action,p_on_select_action;
};
   
class fielduploadimp: public uploadcodeiface
{
  public:
    fielduploadimp(fieldstruct& fld, hk_string fieldstruct::*ph)
      :uploadhandler(fld), phandlercode(ph) {};
    virtual void upload_text(const hk_string& code) const
    {
      uploadhandler.*phandlercode = code;
    }
    virtual const QString& get_action_text(void) const
    {
        return uploadactiontext;
    }
    virtual ~fielduploadimp() {};
    
  protected:
    static QString uploadactiontext;
    fieldstruct& uploadhandler;
    hk_string fieldstruct::*phandlercode;
};

class hk_kdegridcolumndialogprivate
{
  public:
    hk_kdegridcolumndialogprivate(hk_kdegridcolumndialog* parent, hk_kdegrid* grid):
    p_dlg(parent), p_grid(grid), settxt(i18n("set")), notsettxt(i18n("not set"))
    {
        p_currentfieldit=fields.end();
    }

    void set_buttontext(void);
    void listentries_changed(void);
    void load_presentationdatasources(void);
    void set_columnvalues(void);
    void reload_listdatasource(void);
    void set_currentlistdatasource(void);
    
    hk_kdegridcolumndialog* p_dlg;
    list<fieldstruct> fields;
    list<fieldstruct>::iterator p_currentfieldit;
    hk_kdegrid* p_grid;
    QString     settxt, notsettxt;
};

QString fielduploadimp::uploadactiontext(i18n("Upload to the column definition"));

void hk_kdegridcolumndialogprivate::set_buttontext(void)
{
    if (p_currentfieldit==fields.end()) return;
    p_dlg->onpushactionbutton->setText((*p_currentfieldit).p_clickaction.size()>0?settxt:notsettxt);
    p_dlg->ondoubleclickactionbutton->setText((*p_currentfieldit).p_doubleclickaction.size()>0?settxt:notsettxt);
    p_dlg->onopenactionbutton->setText((*p_currentfieldit).p_onopenaction.size()>0?settxt:notsettxt);
    p_dlg->oncloseactionbutton->setText((*p_currentfieldit).p_oncloseaction.size()>0?settxt:notsettxt);
    p_dlg->beforerowchangeactionbutton->setText((*p_currentfieldit).p_before_row_change_action.size()>0?settxt:notsettxt);
    p_dlg->afterrowchangeactionbutton->setText((*p_currentfieldit).p_after_row_change_action.size()>0?settxt:notsettxt);
    p_dlg->beforeupdateactionbutton->setText((*p_currentfieldit).p_before_update_action.size()>0?settxt:notsettxt);
    p_dlg->afterupdateactionbutton->setText((*p_currentfieldit).p_after_update_action.size()>0?settxt:notsettxt);
    p_dlg->beforedeleteactionbutton->setText((*p_currentfieldit).p_before_delete_action.size()>0?settxt:notsettxt);
    p_dlg->afterdeleteactionbutton->setText((*p_currentfieldit).p_after_delete_action.size()>0?settxt:notsettxt);
    p_dlg->beforeinsertactionbutton->setText((*p_currentfieldit).p_before_insert_action.size()>0?settxt:notsettxt);
    p_dlg->afterinsertactionbutton->setText((*p_currentfieldit).p_after_insert_action.size()>0?settxt:notsettxt);
    p_dlg->onselectactionbutton->setText((*p_currentfieldit).p_on_select_action.size()>0?settxt:notsettxt);

    p_dlg->ongetfocusbutton->setText((*p_currentfieldit).p_on_getfocus_action.size()>0?settxt:notsettxt);
    p_dlg->onloosefocusbutton->setText((*p_currentfieldit).p_on_loosefocus_action.size()>0?settxt:notsettxt);
    p_dlg->onkeybutton->setText((*p_currentfieldit).p_on_key_action.size()>0?settxt:notsettxt);
}

void hk_kdegridcolumndialogprivate::listentries_changed(void)
{
    p_dlg->addbutton->setEnabled(p_dlg->leftbox->count()>0);
    p_dlg->deletebutton->setEnabled(p_dlg->rightbox-> invisibleRootItem() -> childCount()>0);
    p_dlg->storebutton->setEnabled(p_dlg->rightbox-> invisibleRootItem() -> childCount()>0);
    bool b=(p_dlg->columntypefield->currentIndex()==2);
    p_dlg->combotab->setEnabled(b);
    p_dlg->onselectactionbutton->setEnabled(b);
    p_dlg->listdatasourcetypefield->setEnabled((p_grid->presentationdatasource()==-1));
    set_buttontext();
}

void hk_kdegridcolumndialogprivate::load_presentationdatasources(void)
{
//load datasources from hk_presentation
    p_dlg->listdatasourcefield->blockSignals(true);
    p_dlg->listdatasourcefield->clear();
    p_dlg->listdatasourcefield->blockSignals(false);

    list<hk_datasource*>* dslist = p_grid->presentation()->datasources();
    if (dslist==NULL)   return;
    list<hk_datasource*>::iterator it=dslist->begin();
    while (it!=dslist->end())
    {
        QString name= QString::fromUtf8(l2u(p_grid->presentation()->unique_datasourcename((*it)->presentationnumber())).c_str());
        p_dlg->listdatasourcefield->addItem(name);
        it++;
    }
}

void hk_kdegridcolumndialogprivate::set_columnvalues()
{
    if (p_grid==NULL) return;

    p_dlg->leftbox->clear();
    p_dlg->rightbox->clear();
    fields.erase(fields.begin(),fields.end());
    p_currentfieldit=fields.end();
    list<hk_string>* p_visiblelist =p_grid->visible_columns();
    list<hk_string>::iterator it=p_visiblelist->begin();
    int i=0;
    QString def,type;
    while (it!=p_visiblelist->end())
    {
        fieldstruct f;
        hk_dsgridcolumn* gc=p_grid->gridcolumn(i);
        if (gc==NULL) def="";
        else def=QString::fromUtf8 (l2u(gc->raw_defaultvalue()).c_str());

        switch (gc->columntype())
        {
            case hk_dsgridcolumn::columnbool : type =i18n("bool");
                break;
            case hk_dsgridcolumn::columncombo : type =i18n("combo");
                break;
            default : type =i18n("edit");
                break;
        }
        f.name=gc->columnname();
        f.defaultvalue=gc->raw_defaultvalue();
        f.identifier=gc->identifier();
        f.alignment=gc->alignment();
        f.use_separators=gc->use_numberseparator();
        f.displayname=gc->displayname();
        f.readonly=gc->is_readonly();
        f.digits=gc->commadigits();
        f.listdatasourcetype=gc->listdatasourcetype();
        f.listdatasource=gc->listdatasourcename();
        f.listcolumn=gc->listcolumnname();
        f.viewcolumn=gc->viewcolumnname();
        f.columntype=gc->columntype();
	    f.p_clickaction=gc->on_click_action();
	    f.p_doubleclickaction=gc->on_doubleclick_action();
	    f.p_onopenaction=gc->on_open_action();
	    f.p_oncloseaction=gc->on_close_action();
	    f.p_before_row_change_action=gc->before_row_change_action();
	    f.p_after_row_change_action=gc->after_row_change_action();
	    f.p_before_delete_action=gc->before_delete_action();
	    f.p_after_delete_action=gc->after_delete_action();
	    f.p_before_update_action=gc->before_update_action();
	    f.p_after_update_action=gc->after_update_action();
	    f.p_before_insert_action=gc->before_insert_action();
	    f.p_after_insert_action=gc->after_insert_action();
	    f.p_on_getfocus_action=gc->on_getfocus_action();
	    f.p_on_loosefocus_action=gc->on_loosefocus_action();
	    f.p_on_key_action=gc->on_key_action();
	    f.p_on_select_action=gc->on_select_action(); 
	    fields.insert(fields.end(),f);
	
        new QTreeWidgetItem(p_dlg->rightbox,QStringList() << QString::fromUtf8 (l2u((*it)).c_str()) << type);
        it++;i++;
    }

    list<hk_string>* p_nonvisiblelist =p_grid->nonvisible_columns();
    it=p_nonvisiblelist->begin();

    while (it!=p_nonvisiblelist->end())
    {
        p_dlg->leftbox->addItem(QString::fromUtf8 (l2u((*it)).c_str()));
        it++;
    }

    listentries_changed();
}

void hk_kdegridcolumndialogprivate::reload_listdatasource(void)
{
    p_dlg->listcolumnfield->blockSignals(true);
    p_dlg->listcolumnfield->clear();
    p_dlg->listcolumnfield->blockSignals(false);
    p_dlg->viewcolumnfield->blockSignals(true);
    p_dlg->viewcolumnfield->clear();
    p_dlg->viewcolumnfield->blockSignals(false);
    if (!p_grid) return;
    if (!p_grid->datasource())return;
    hk_datasource* ds= NULL;
    if (!p_grid->presentation())
    {
//no presentation
        hk_database* db=p_grid->datasource()->database();
        if (db==NULL) return;
        QString f=p_dlg->listdatasourcefield->itemText(p_dlg->listdatasourcefield->currentIndex());
        if (f.isEmpty()) f="";
        if (p_grid->presentation()) p_grid->presentation()->get_datasource(u2l(f.toUtf8().data()));
        else
        {
	    datasourcetype dt=dt_table;
	     switch (p_dlg->listdatasourcetypefield->currentIndex())
	     {
	       case 1: dt =dt_query;break;
	       case 2: dt =dt_view;break;
	       default: dt=dt_table;
	     }
            ds=db->load_datasource(u2l(f.toUtf8().data()),dt);
        }
        if (ds==NULL) return;
    }
    else
    {                                             // a presenation

    }

    list<hk_column*>* v=NULL;
    if (p_grid->presentation()&&p_currentfieldit!=fields.end())
        ds=p_grid->presentation()->get_datasource((*p_currentfieldit).listdatasource);
    
    hk_string tempfilter;
    bool usetempfilter=false;
    if(ds)
    {
        if (ds->type()!=hk_datasource::ds_table)
        {
            tempfilter=ds->temporaryfilter();
            usetempfilter=ds->use_temporaryfilter();
            ds->set_temporaryfilter("0=1");

            ds->set_use_temporaryfilter(true);
            ds->enable();
        }
        v=ds->columns();
    }
    if (v==NULL)
    {
        if (ds&& ds->type()!=hk_datasource::ds_table)
        {
            ds->disable();
            ds->set_temporaryfilter(tempfilter);
            ds->set_use_temporaryfilter(usetempfilter);
        }

        return;
    }
    p_dlg->listcolumnfield->blockSignals(true);
    p_dlg->viewcolumnfield->blockSignals(true);
    p_dlg->listcolumnfield->addItem("");
    p_dlg->viewcolumnfield->addItem("");
    list<hk_column*>::iterator it=v->begin();
    while (it!=v->end())
    {
        p_dlg->listcolumnfield->addItem(QString::fromUtf8 (l2u((*it)->name()).c_str()));
        p_dlg->viewcolumnfield->addItem(QString::fromUtf8 (l2u((*it)->name()).c_str()));
        it++;
    }
    p_dlg->listcolumnfield->blockSignals(false);
    p_dlg->viewcolumnfield->blockSignals(false);

    if (ds && ds->type()!=hk_datasource::ds_table)
    {
        ds->disable();
        ds->set_temporaryfilter(tempfilter);
        ds->set_use_temporaryfilter(usetempfilter);
    }
}

void hk_kdegridcolumndialogprivate::set_currentlistdatasource(void)
{
    int current=0;
    int counter=0;
    int c= p_dlg->listdatasourcefield->count();
    while (counter<c)
    {
        if (p_currentfieldit!=fields.end())
        {
            if ( p_dlg->listdatasourcefield->itemText(counter)== QString::fromUtf8 (l2u((*p_currentfieldit).listdatasource).c_str()))
                current=counter;
        }
        counter++;
    }
    p_dlg->listdatasourcefield->blockSignals(true);
    p_dlg->listdatasourcefield->setCurrentIndex(current);
    p_dlg->listdatasourcefield->blockSignals(false);
}

/*
 *  Constructs a hk_kdegridcolumndialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
 
hk_kdegridcolumndialog::hk_kdegridcolumndialog( hk_kdegrid* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdegridcolumndialogbase( parent, name, modal, fl ), p_private(new hk_kdegridcolumndialogprivate(this, parent))
{
    QString no=i18n("no");
    QString yes=i18n("yes");
    leftlabel->setText(i18n("Invisible columns:"));
    rightlabel->setText(i18n("Shown columns:"));
    rightbox -> setColumnCount(2);
    rightbox -> setHeaderLabels(QStringList() << i18n( "Column" ) << i18n( "Columntype" ));
    rightbox -> setSortingEnabled(false);
    rightbox -> header() -> setMovable(false);
    setWindowTitle(i18n("Column select"));
    addbutton->setIcon( QIcon::fromTheme("go-next") );
    deletebutton->setIcon( QIcon::fromTheme("go-previous") );

    columntypefield->addItem(i18n("edit"));
    columntypefield->addItem(i18n("bool"));
    columntypefield->addItem(i18n("combo"));
    listdatasourcetypefield->addItem(i18n("table"));
    listdatasourcetypefield->addItem(i18n("query"));
    listdatasourcetypefield->addItem(i18n("view"));
    alignmentfield->addItem(i18n("Left"));
    alignmentfield->addItem(i18n("Center"));
    alignmentfield->addItem(i18n("Right"));
    separatorfield->addItem(yes);
    separatorfield->addItem(no);
    readonlyfield->addItem(yes);
    readonlyfield->addItem(no);
    readonlylabel->setText(i18n("Read only:"));
    separatorlabel->setText(i18n("Separator:"));
    displaylabel->setText(i18n("Displayname:"));
    columntypelabel->setText(i18n("Columntype:"));
    digitlabel->setText(i18n("Digits:"));
    definitionlabel->setText(i18n("Columndefinition:"));
    listdatasourcetypelabel->setText(i18n("Listdatasourcetype:"));
    TabWidget->setTabText(TabWidget->indexOf(formattab),i18n("&Format"));
    TabWidget->setTabText(TabWidget->indexOf(combotab),i18n("&Combobox"));
    actiontab->setEnabled(p_private->p_grid->presentation()!=NULL);
    connect( columntypefield, SIGNAL( activated(int) ), this, SLOT( columntype_changed() ) );
    
    if (p_private->p_grid && p_private->p_grid->presentationdatasource()==-1)
        listdatasourcetype_changed();
    else
        p_private->load_presentationdatasources();
    p_private->set_columnvalues();
    rightbox->setFocus();
    rightbox->setCurrentItem(rightbox-> invisibleRootItem()->child(0));
    rightbox_selectionchanged(rightbox->currentItem());
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdegridcolumndialog::~hk_kdegridcolumndialog()
{
// no need to delete child widgets, Qt does it all for us
}


/*
 * public slot
 */
void hk_kdegridcolumndialog::addbutton_clicked()
{
    if (leftbox->currentRow()<0)return;

    fieldstruct f;
    f.name=u2l(leftbox->currentItem()->text().toUtf8().data());
    (p_private->fields).insert((p_private->fields).end(),f);
    QListWidgetItem* invfield = leftbox->takeItem(leftbox->currentRow());
    QTreeWidgetItem* feld = new QTreeWidgetItem(rightbox, QStringList() << invfield->text() << i18n("edit"));
    delete invfield;
    p_private->listentries_changed();
    rightbox -> setCurrentItem(feld); 
}


/*
 * public slot
 */
void hk_kdegridcolumndialog::deletebutton_clicked()
{
   if (rightbox->currentItem()==NULL)return;
   QTreeWidgetItem* f=rightbox->currentItem();
    
    if (f==NULL||((p_private->p_currentfieldit)==(p_private->fields).end()))  return;

    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
    {
        (p_private->fields).erase(p_private->p_currentfieldit);
        p_private->p_currentfieldit=(p_private->fields).end();
    };

    leftbox->insertItem(leftbox->currentRow()+1, rightbox->currentItem()->text(0));
    QTreeWidgetItem* nextitem =rightbox -> itemBelow(rightbox -> currentItem());   
    
    if (!nextitem)
        nextitem = rightbox -> itemAbove(rightbox -> currentItem()); 
    rightbox -> invisibleRootItem() -> removeChild(f);
    delete f;
    p_private->listentries_changed();
    rightbox->setCurrentItem(nextitem);
}


/*
 * public slot
 */
void hk_kdegridcolumndialog::exitbutton_clicked()
{
    close();
}


/*
 * public slot
 */
void hk_kdegridcolumndialog::okbutton_clicked()
{
    list<hk_string> p_l;
    list<fieldstruct>::iterator it=(p_private->fields).begin();
    while (it!=(p_private->fields).end())
    {
        p_l.insert(p_l.end(),(*it).name);
        it++;
    }

    p_private->p_grid->set_gridcolumns(p_l);

    it=(p_private->fields).begin();
    unsigned int k=0;
    while (it!=(p_private->fields).end())
    {
        hk_dsgridcolumn* gc=p_private->p_grid->gridcolumn(k);
        if (gc!=NULL)
        {
            gc->set_defaultvalue((*it).defaultvalue);
            gc->set_columntype((*it).columntype);
            gc->set_alignment((*it).alignment);
            gc->set_numberformat((*it).use_separators,(*it).digits);
            gc->set_displayname((*it).displayname);
            gc->set_identifier((*it).identifier);
            gc->set_readonly((*it).readonly);

	    gc->set_on_click_action((*it).p_clickaction,true,true);
	    gc->set_on_doubleclick_action((*it).p_doubleclickaction,true,true);
	    gc->set_on_open_action((*it).p_onopenaction,true,true);
	    gc->set_on_close_action((*it).p_oncloseaction,true,true);
	    gc->set_before_row_change_action((*it).p_before_row_change_action,true,true);
	    gc->set_after_row_change_action((*it).p_after_row_change_action,true,true);
	    gc->set_before_delete_action((*it).p_before_delete_action,true,true);
	    gc->set_after_delete_action((*it).p_after_delete_action,true,true);
	    gc->set_before_update_action((*it).p_before_update_action,true,true);
	    gc->set_after_update_action((*it).p_after_update_action,true,true);
	    gc->set_before_insert_action((*it).p_before_insert_action,true,true);
	    gc->set_after_insert_action((*it).p_after_insert_action,true,true);
	    gc->set_on_getfocus_action((*it).p_on_getfocus_action,true,true);
	    gc->set_on_loosefocus_action((*it).p_on_loosefocus_action,true,true);
	    gc->set_on_key_action((*it).p_on_key_action,true,true);
	    gc->set_on_select_action((*it).p_on_select_action,true,true);

            if ((*it).columntype==hk_dsgridcolumn::columncombo)
            {
                if (p_private->p_grid->presentation())
                {                                 
                    hk_datasource* ds=p_private->p_grid->presentation()->get_datasource((*it).listdatasource);
                    if (ds) gc->set_listpresentationdatasource(ds->presentationnumber());
                }

                gc->set_listdatasource((*it).listdatasource,(*it).listdatasourcetype);
                gc->set_listcolumnname((*it).listcolumn);
                gc->set_viewcolumnname((*it).viewcolumn);
            }
        }
        it++;k++;

    }

    accept();
}

void hk_kdegridcolumndialog::defaultvalue_changed()
{

    if (p_private->p_currentfieldit!=(p_private->fields).end())
        (*(p_private->p_currentfieldit)).defaultvalue=u2l(defaultfield->text().toUtf8().data());

}


void hk_kdegridcolumndialog::columntype_changed()
{
    hk_dsgridcolumn::enum_columntype t=hk_dsgridcolumn::columnedit;
    switch (columntypefield->currentIndex())
    {
        case 0 : t=hk_dsgridcolumn::columnedit;break;
        case 1 : t=hk_dsgridcolumn::columnbool;break;
        case 2 : t=hk_dsgridcolumn::columncombo;break;
        default : t=hk_dsgridcolumn::columnedit;
    }
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
        (*(p_private->p_currentfieldit)).columntype=t;
    else cout <<"Warnung: p_currentfieldit==.end"<<endl;

    combotab->setEnabled(t==2);
    onselectactionbutton->setEnabled(t==2);

    rightbox->currentItem()->setText(1,columntypefield->currentText());
    p_private->listentries_changed();
}


void hk_kdegridcolumndialog::rightbox_selectionchanged(QTreeWidgetItem* i)
{
    if (i==NULL) return;

    list<fieldstruct>::iterator it=(p_private->fields).begin();
    bool found=false;
    hk_string nn=u2l(i->text(0).toUtf8().data());
    while (it!=(p_private->fields).end()&&!found)
    {
        if ((*it).name==nn)
        {
            p_private->p_currentfieldit=it;
            found=true;
        }
        it++;
    }
    if ((p_private->p_currentfieldit)==(p_private->fields).end()) return;
    defaultfield->blockSignals(true);
    defaultfield->setText( QString::fromUtf8 (l2u((*(p_private->p_currentfieldit)).defaultvalue).c_str()));
    defaultfield->blockSignals(false);

    displayfield->blockSignals(true);
    displayfield->setText( QString::fromUtf8 (l2u((*(p_private->p_currentfieldit)).displayname).c_str()));
    displayfield->blockSignals(false);

    identifierfield->blockSignals(true);
    identifierfield->setText( QString::fromUtf8 (l2u((*(p_private->p_currentfieldit)).identifier).c_str()));
    identifierfield->blockSignals(false);

    readonlyfield->blockSignals(true);
    readonlyfield->setCurrentIndex( ((*(p_private->p_currentfieldit)).readonly?0:1));
    readonlyfield->blockSignals(false);

    columntypefield->blockSignals(true);
    int p=0;
    switch ((*(p_private->p_currentfieldit)).columntype)
    {
        case hk_dsgridcolumn::columnbool :p=1;break;
        case hk_dsgridcolumn::columncombo :p=2;break;
        default : p=0;
    }
    combotab->setEnabled(p==2);

    columntypefield->setCurrentIndex(p);
    columntypefield->blockSignals(false);
    alignmentfield->blockSignals(true);
    p=0;
    switch ((*(p_private->p_currentfieldit)).alignment)
    {
        case hk_dsdatavisible::aligncenter :p=1;break;
        case hk_dsdatavisible::alignright :p=2;break;
        default : p=0;
    }

    alignmentfield->setCurrentIndex(p);
    alignmentfield->blockSignals(false);

    separatorfield->blockSignals(true);
    separatorfield->setCurrentIndex((*(p_private->p_currentfieldit)).use_separators?0:1);
    separatorfield->blockSignals(false);

    digitfield->blockSignals(true);
    digitfield->setValue((*(p_private->p_currentfieldit)).digits);
    digitfield->blockSignals(false);

    if (p_private->p_grid->presentationdatasource()==-1)
    {
        listdatasourcetypefield->blockSignals(true);
	int i=0;
	 switch ((*(p_private->p_currentfieldit)).listdatasourcetype)
	 {
	   case dt_query: i=1;break;
	   case dt_view: i=2;break;
	   default: i=0;
	 }
        listdatasourcetypefield->setCurrentIndex(i);
        listdatasourcetypefield->blockSignals(false);
        listdatasourcetype_changed();
    }
    p_private->reload_listdatasource();
    p_private->set_currentlistdatasource();

    QString defaultText = QString::fromUtf8 (l2u((*(p_private->p_currentfieldit)).listcolumn).c_str());
    listcolumnfield->blockSignals(true);
    int ind = listcolumnfield->findText(defaultText);
    if (ind != -1)
        listcolumnfield->setCurrentIndex(ind);
    else
        // is editable
        listcolumnfield->setEditText(defaultText);
    //listcolumnfield->setCurrentText();
    listcolumnfield->blockSignals(false);

    defaultText = QString::fromUtf8 (l2u((*(p_private->p_currentfieldit)).viewcolumn).c_str());
    viewcolumnfield->blockSignals(true);
    ind = viewcolumnfield->findText(defaultText);
    if (ind != -1)
        viewcolumnfield->setCurrentIndex(ind);
    else
        // is editable
        viewcolumnfield->setEditText(defaultText);
    //viewcolumnfield->setCurrentText();
    viewcolumnfield->blockSignals(false);

    p_private->listentries_changed();
    p_private->set_buttontext();
}


void hk_kdegridcolumndialog::alignment_changed()
{
    hk_dsdatavisible::alignmenttype a;
    switch (alignmentfield->currentIndex())
    {
        case 1 :a=hk_dsdatavisible::aligncenter;
        break;
        case 2 : a=hk_dsdatavisible::alignright;
        break;
        default : a=hk_dsdatavisible::alignleft;
    }
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
        (*(p_private->p_currentfieldit)).alignment=a;
}


void hk_kdegridcolumndialog::displayname_changed()
{
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
        (*(p_private->p_currentfieldit)).displayname=u2l(displayfield->text().toUtf8().data());

}

void hk_kdegridcolumndialog::identifier_changed()
{
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
        (*(p_private->p_currentfieldit)).identifier=u2l(identifierfield->text().toUtf8().data());

}

void hk_kdegridcolumndialog::digits_changed()
{
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
        (*(p_private->p_currentfieldit)).digits=digitfield->value();

}

void hk_kdegridcolumndialog::separator_changed()
{
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
        (*(p_private->p_currentfieldit)).use_separators=(separatorfield->currentIndex()==0);

}

void hk_kdegridcolumndialog::readonly_changed()
{
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
        (*(p_private->p_currentfieldit)).readonly=(readonlyfield->currentIndex()==0);

}

void hk_kdegridcolumndialog::listdatasource_changed()
{
    QString f=listdatasourcefield->currentText();
    if (f.isEmpty()) f="";
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
    {
        (*(p_private->p_currentfieldit)).listdatasource=u2l(f.toUtf8().data());
    }
    p_private->reload_listdatasource();

}

void hk_kdegridcolumndialog::listdatasourcetype_changed()
{
    if (!p_private->p_grid->datasource())return;
    hk_database* db=p_private->p_grid->datasource()->database();
    if (db==NULL) return;
    vector<hk_string>* v=NULL;
    switch (listdatasourcetypefield->currentIndex())
    {
      case 1 : v=db->querylist();break;
      case 2 : v=db->viewlist();break;
      default: v=db->tablelist();
    }
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
    {
        datasourcetype dt;
	switch (listdatasourcetypefield->currentIndex())
	{
	  case 1: dt=dt_query;break;
	  case 2: dt=dt_view;break;
	  default: dt=dt_table;
	}
	(*(p_private->p_currentfieldit)).listdatasourcetype=dt;
    }
    listdatasourcefield->blockSignals(true);
    listdatasourcefield->clear();
    vector<hk_string>::iterator it=v->begin();
    int p=0;
    while (it!=v->end())
    {
        listdatasourcefield->addItem(QString::fromUtf8 (l2u((*it)).c_str()));
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
        if ((*(p_private->p_currentfieldit)).listdatasource==(*it))listdatasourcefield->setCurrentIndex(p);
        it++;++p;
    }
    listdatasourcefield->blockSignals(false);
    p_private->reload_listdatasource();
    listdatasource_changed();
}

void hk_kdegridcolumndialog::viewcolumn_changed()
{
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
    {
        (*(p_private->p_currentfieldit)).viewcolumn=u2l(viewcolumnfield->currentText().toUtf8().data());
    }
}


void hk_kdegridcolumndialog::listcolumn_changed()
{
    if ((p_private->p_currentfieldit)!=(p_private->fields).end())
    {
        (*(p_private->p_currentfieldit)).listcolumn=u2l(listcolumnfield->currentText().toUtf8().data());
//cout <<"listcolumn_changed auf  "<<(*p_currentfieldit).listcolumn<<endl;
    }
}

void hk_kdegridcolumndialog::onclickaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uclick(*(p_private->p_currentfieldit), &fieldstruct::p_clickaction);
    hk_kdeinterpreterdialog d(uclick);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_clickaction,false);
    d.set_caption(NULL,"on_click_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::ondoubleclickaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp udoubleclick(*(p_private->p_currentfieldit), &fieldstruct::p_doubleclickaction);
    hk_kdeinterpreterdialog d(udoubleclick);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_doubleclickaction,false);
    d.set_caption(NULL,"ondoubleclick_action");
    (void) d.exec(0, warning); // upload handled by the dialog
    
    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::onopenaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uonopen(*(p_private->p_currentfieldit), &fieldstruct::p_onopenaction);
    hk_kdeinterpreterdialog d(uonopen);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_onopenaction,false);
    d.set_caption(NULL,"on_open_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::oncloseaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uonclose(*(p_private->p_currentfieldit), &fieldstruct::p_oncloseaction);
    hk_kdeinterpreterdialog d(uonclose);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_oncloseaction,false);
    d.set_caption(NULL,"on_close_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::beforeupdateaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp ubeforeupdate(*(p_private->p_currentfieldit), &fieldstruct::p_before_update_action);
    hk_kdeinterpreterdialog d(ubeforeupdate);

    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_before_update_action,false);
    d.set_caption(NULL,"before_update_action"); // upload handled by the dialog
    (void) d.exec(0,warning);

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::afterupdateaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uafterupdate(*(p_private->p_currentfieldit), &fieldstruct::p_after_update_action);
    hk_kdeinterpreterdialog d(uafterupdate);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_after_update_action,false);
    d.set_caption(NULL,"after_update_action"); // upload handled by the dialog
    (void) d.exec(0, warning);

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::beforerowchangeaction_changed()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp ubeforerow(*(p_private->p_currentfieldit), &fieldstruct::p_before_row_change_action);
    hk_kdeinterpreterdialog d(ubeforerow);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_before_row_change_action,false);
    d.set_caption(NULL,"before_row_change_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::afterrowchangeaction_changed()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uafterrow(*(p_private->p_currentfieldit), &fieldstruct::p_after_row_change_action);
    hk_kdeinterpreterdialog d(uafterrow);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_after_row_change_action,false);
    d.set_caption(NULL,"after_row_change_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::beforedeleteaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp ubeforedelete(*(p_private->p_currentfieldit), &fieldstruct::p_before_delete_action);
    hk_kdeinterpreterdialog d(ubeforedelete);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_before_delete_action,false);
    d.set_caption(NULL,"before_delete_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::afterdeleteaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uafterdelete(*(p_private->p_currentfieldit), &fieldstruct::p_after_delete_action);
    hk_kdeinterpreterdialog d(uafterdelete);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_after_delete_action,false);
    d.set_caption(NULL,"after_delete_action");
    (void) d.exec(0, warning); // upload handled by the dialog
    
    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::beforeinsertaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp ubeforeinsert(*(p_private->p_currentfieldit), &fieldstruct::p_before_insert_action);
    hk_kdeinterpreterdialog d(ubeforeinsert);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_before_insert_action,false);
    d.set_caption(NULL,"before_insert_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::afterinsertaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uafterinsert(*(p_private->p_currentfieldit), &fieldstruct::p_after_insert_action);
    hk_kdeinterpreterdialog d(uafterinsert);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_after_insert_action,false);
    d.set_caption(NULL,"after_insert_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::on_getfocusaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp ugetfocus(*(p_private->p_currentfieldit), &fieldstruct::p_on_getfocus_action);
    hk_kdeinterpreterdialog d(ugetfocus);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_on_getfocus_action,false);
    d.set_caption(NULL,"on_getfocus_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::on_loosefocusaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uloosefocus(*(p_private->p_currentfieldit), &fieldstruct::p_on_loosefocus_action);
    hk_kdeinterpreterdialog d(uloosefocus);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_on_loosefocus_action,false);
    d.set_caption(NULL,"on_loosefocus_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}

void hk_kdegridcolumndialog::onkeybutton_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uonkey(*(p_private->p_currentfieldit), &fieldstruct::p_on_key_action);
    hk_kdeinterpreterdialog d(uonkey);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_on_key_action,false);
    d.set_caption(NULL,"on_key_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}


void hk_kdegridcolumndialog::onselectaction_clicked()
{
    if ((p_private->p_currentfieldit)==(p_private->fields).end())
        return;

    hk_string warning;
    fielduploadimp uonselect(*(p_private->p_currentfieldit), &fieldstruct::p_on_select_action);
    hk_kdeinterpreterdialog d(uonselect);
    
    d.setWindowModality(Qt::ApplicationModal);
    d.set_code((*(p_private->p_currentfieldit)).p_on_select_action,false);
    d.set_caption(NULL,"on_select_action");
    (void) d.exec(0, warning); // upload handled by the dialog

    p_private->set_buttontext();
}
