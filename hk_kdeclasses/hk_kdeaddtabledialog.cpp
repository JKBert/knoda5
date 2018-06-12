// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
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
//$Revision: 1.4 $
#include "hk_kdeaddtabledialog.h"
#include "hk_kdedbdesigner.h"

#include <qlistwidget.h>
#include <qcombobox.h>
#include <qpushbutton.h>

#include <klocale.h>
#include <kconfig.h>
#include <kapplication.h>
#include <kconfiggroup.h>

#include <hk_presentation.h>
#include <hk_datasource.h>
#include <hk_database.h>
#include <hk_definitions.h>
#include <kglobal.h>

/*
 *  Constructs a hk_kdeaddtabledialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdeaddtabledialog::hk_kdeaddtabledialog( hk_kdedbdesigner* designer,bool allowqueries, QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : hk_kdeaddtabledialogbase( parent, name, modal, fl )
{
  p_designer=designer;
  p_allowqueries=allowqueries;
  p_added=false;
  datasourcetypefield->addItem(i18n("table"));
  if (p_designer->presentation()->presentationtype()!=hk_presentation::referentialintegrity) {
    datasourcetypefield->addItem(i18n("view"));
   if (allowqueries)
     datasourcetypefield->addItem(i18n("query"));
  }
  datasourcetypefield->setCurrentIndex(0);
  addbutton->setEnabled(false);
  set_datasources();
  check_buttons();
  
  KSharedConfigPtr c = KGlobal::config();
  const QRect& rrect = QRect(0,0,500,300);
  KConfigGroup cg = c->group("AddDatasource");
  QRect g;
  
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g);
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeaddtabledialog::~hk_kdeaddtabledialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 * public slot
 */
void hk_kdeaddtabledialog::add_clicked()
{

    if (!addbutton->isEnabled()) return;
    datasourcetype dt=dt_table;
    if(datasourcetypefield->currentIndex()==2)
        dt=dt_query;
    else if(datasourcetypefield->currentIndex()==1)
             dt=dt_view;
   hk_datasource* ds=p_designer->presentation()->get_datasource(p_designer->presentation()->new_datasource(
   u2l(tablelist->currentItem()->text().toUtf8().data()) ,dt ));
   if (!ds)
     {
       show_warningmessage("Bug: datasource could not be created");
       return;
     }
   ds->set_designsize(2000,2000,false);
  		hk_kdedatasourceframe* k=p_designer->add_dsframe(ds);
		k->set_focus(k);
   p_designer->presentation()->has_changed(true);
   if (p_designer->presentation()->presentationtype()==hk_presentation::referentialintegrity)
   {
     QListWidgetItem *p_cur = tablelist->takeItem(tablelist->currentRow());
     delete p_cur;
     p_designer->set_all_relations();
   }
    p_added=true;
}


void hk_kdeaddtabledialog::set_datasources()
{
    hk_database* db=p_designer->presentation()->database();
    if (db==NULL) return;
    tablelist->clear();
    vector<hk_string>* v=NULL;
    if(datasourcetypefield->currentIndex()==2)
      v=db->querylist();
    else if(datasourcetypefield->currentIndex()==1)
      v=db->viewlist();
    else v=db->tablelist();

    vector<hk_string>::iterator it=v->begin();

    while (it!=v->end())
    {
        if (p_designer->presentation()->presentationtype()==hk_presentation::referentialintegrity)
        {
          if(!already_added_table((*it)))
 	     tablelist->addItem(QString::fromUtf8 (l2u((*it)).c_str()));
        }
        else
          tablelist->addItem(QString::fromUtf8 (l2u((*it)).c_str()));
        it++;
    }
    tablelist->setCurrentItem(0);
check_buttons();
}


void hk_kdeaddtabledialog::check_buttons()
{
  addbutton->setEnabled(tablelist->count()>0);
}


void hk_kdeaddtabledialog::accept()
{
  KSharedConfigPtr c = KGlobal::config();
  KConfigGroup cg = c->group("AddDatasource");
  
  cg.writeEntry("Geometry",geometry());
  hk_kdeaddtabledialogbase::accept();
}


bool hk_kdeaddtabledialog::already_added_table(const hk_string&t)
{
  list<hk_datasource*>*l=p_designer->presentation()->datasources();
  list<hk_datasource*>::iterator it=l->begin();
  while (it!=l->end())
  {
    hk_datasource* d=(*it);
    if (d->type()==hk_datasource::ds_table && d->name()==t) return true;
    ++it;
  }

  return false;
}

