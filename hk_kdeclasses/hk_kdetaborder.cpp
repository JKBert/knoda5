// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2014 Patrik Hanak <hanakp@users.sourceforge.net>
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
// $Revision: 1.8 $
#include "hk_kdetaborder.h"
#include "hk_kdesimpleform.h"
#include <hk_class.h>
#include <hk_dsdatavisible.h>

#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qtreewidget.h>

#include <kiconloader.h>
#include <klocale.h>
#include <kconfig.h>
#include <KConfigGroup>
#include <kapplication.h>
#include <kglobal.h>

/* 
 *  Constructs a hk_kdetaborder which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdetaborder::hk_kdetaborder( hk_kdesimpleform*f, QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : hk_kdetaborderdialogbase( parent, name, modal, fl )
{
  p_form=f;
  KIconLoader* loader=KIconLoader::global();
  loader->addAppDir("hk_kde4classes");
  upbutton->setIcon( loader->loadIcon("go-up",KIconLoader::Small) );
  downbutton->setIcon( loader->loadIcon("go-down",KIconLoader::Small) );
  addbutton->setIcon( loader->loadIcon("go-next",KIconLoader::Small) );
  deletebutton->setIcon( loader->loadIcon("go-previous",KIconLoader::Small) );

  taborderlist -> setSortingEnabled(false);
  if (!p_form) hk_class::show_warningmessage("hk_kdetaborder ERROR form is NULL!!!");
  p_has_changed=false;
  init();
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Taborder");
  const QRect& rrect =  QRect(0,0,500,300);
  QRect g;
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g);

  check_buttons();
}


void hk_kdetaborder::init(void)
{
list<int> t=p_form->taborder();
list<int>::iterator it=t.begin();
QStringList listrec;

while (it!=t.end())
{
  hk_visible* v=p_form->get_visible(*it);
  QString n;
     n.setNum(*it);
  QString type;
  
  
  if (v)
  {
    switch (v->type())
    {
     case hk_visible::button: type=i18n("button");break;
     case hk_visible::rowselector: type=i18n("rowselector");break;
     case hk_visible::boolean: type=i18n("boolean");break;
     case hk_visible::lineedit: type=i18n("lineedit");break;
     case hk_visible::memo: type=i18n("memo");break;
     case hk_visible::combobox: type=i18n("combobox");break;
     case hk_visible::grid: type=i18n("grid");break;
     case hk_visible::subform: type=i18n("subform");break;
     case hk_visible::image: type=i18n("image");break;
     case hk_visible::tabvisible: type=i18n("tabvisible");break;
     default: type=i18n("unknown");
    }
    QString col;
    hk_dsdatavisible* dv=dynamic_cast<hk_dsdatavisible*>(v);
    if (dv)
      col=QString::fromUtf8(dv->columnname().c_str());
    listrec.clear();
    listrec << n << QString::fromUtf8(l2u(v->identifier()).c_str()) << type << col;
    new QTreeWidgetItem(taborderlist,listrec); 
  }
  ++it;
}
taborderlist->setAllColumnsShowFocus(true);
taborderlist -> resizeColumnToContents(0);
taborderlist -> resizeColumnToContents(1);
taborderlist -> resizeColumnToContents(2);

taborderlist -> setCurrentItem(taborderlist->invisibleRootItem()->child(0));

// now set the non-used field names

list<hk_visible*>* l=p_form->visibles();
list<int> vlist;
list<hk_visible*>::iterator lit=l->begin();
while (lit!=l->end())
{
if ((*lit)->type()!=hk_visible::textlabel)
vlist.push_back((*lit)->presentationnumber());
++lit;
}


it=t.begin();
while (it!=t.end())
{
  vlist.remove(*it);
  ++it;
}


t=vlist;
it=t.begin();
while (it!=t.end())
{
  hk_visible* v=p_form->get_visible(*it);
  QString n;
     n.setNum(*it);
  QString type;
  
  
  if (v)
  {
    switch (v->type())
    {
     case hk_visible::button: type=i18n("button");break;
     case hk_visible::rowselector: type=i18n("rowselector");break;
     case hk_visible::boolean: type=i18n("boolean");break;
     case hk_visible::lineedit: type=i18n("lineedit");break;
     case hk_visible::memo: type=i18n("memo");break;
     case hk_visible::combobox: type=i18n("combobox");break;
     case hk_visible::grid: type=i18n("grid");break;
     case hk_visible::subform: type=i18n("subform");break;
     case hk_visible::image: type=i18n("image");break;
     case hk_visible::tabvisible: type=i18n("tabvisible");break;
     default: type=i18n("unknown");
    }
    QString col;
    hk_dsdatavisible* dv=dynamic_cast<hk_dsdatavisible*>(v);
    if (dv)
      col=QString::fromUtf8(dv->columnname().c_str());
    listrec.clear();
    listrec << n << QString::fromUtf8(l2u(v->identifier()).c_str()) << type << col;
    new QTreeWidgetItem( baselist, listrec);
   }
  ++it;
}

baselist->setAllColumnsShowFocus(true);
baselist->resizeColumnToContents(0);
baselist->resizeColumnToContents(1);
baselist->resizeColumnToContents(2);
taborderlist->setFocus();

}

/*  
 *  Destroys the object and frees any allocated resources
 */
hk_kdetaborder::~hk_kdetaborder()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 * public slot
 */
void hk_kdetaborder::add_clicked()
{
   if (!baselist->selectionModel()->hasSelection()) return;

   taborderlist -> addTopLevelItem(baselist->takeTopLevelItem(
     baselist->indexOfTopLevelItem(baselist -> selectedItems().first())));
   p_has_changed=true;
   check_buttons();
}

/*
 * public slot
 */
void hk_kdetaborder::check_buttons()
{
  QModelIndex tabselected = taborderlist->selectionModel()->hasSelection()?
  taborderlist->selectionModel()->selectedIndexes().first():QModelIndex();
  QModelIndex baseselected = baselist->selectionModel()->hasSelection()?
  baselist->selectionModel()->selectedIndexes().first():QModelIndex();

   deletebutton->setEnabled(tabselected.isValid());
   upbutton->setEnabled( tabselected.isValid() && tabselected.sibling(tabselected.row()-1,0).isValid());
   downbutton->setEnabled( tabselected.isValid() && tabselected.sibling(tabselected.row()+1,0).isValid());
   addbutton->setEnabled(baseselected.isValid());
   okbutton->setEnabled(p_has_changed);
   QWidget* w=NULL; 
   if (tabselected.isValid() )
       w=dynamic_cast<QWidget*>(p_form->get_visible( taborderlist->model()->data(tabselected).toInt() ));
    
   if ( baseselected.isValid() && baselist->hasFocus())
       w=dynamic_cast<QWidget*>(p_form->get_visible(baselist->model()->data(baseselected).toInt()));
    p_form->set_focus(w,false); 
}

/*
 * public slot
 */
void hk_kdetaborder::delete_clicked()
{
   if (!taborderlist->selectionModel()->hasSelection()) return;

   baselist -> addTopLevelItem(taborderlist->takeTopLevelItem(
     taborderlist->indexOfTopLevelItem(taborderlist -> selectedItems().first())));
   p_has_changed=true;
   check_buttons();
}

/*
 * public slot
 */
void hk_kdetaborder::down_clicked()
{
   if (!taborderlist->selectionModel()->hasSelection()) return;

   QTreeWidgetItem* pTabSel = taborderlist -> selectedItems().first();
   if ( taborderlist -> itemBelow(pTabSel) != 0 ) {
     int i = taborderlist->indexOfTopLevelItem(pTabSel);
     taborderlist -> insertTopLevelItem( i + 1,taborderlist-> takeTopLevelItem(i));
     taborderlist->selectionModel()-> clearSelection();
     taborderlist->setCurrentItem(pTabSel);
   }
   p_has_changed=true;
   check_buttons();
}

/*
 * public slot
 */
void hk_kdetaborder::up_clicked()
{
   if (!taborderlist->selectionModel()->hasSelection()) return;

   QTreeWidgetItem* pTabSel = taborderlist -> selectedItems().first();
   if ( taborderlist -> itemAbove(pTabSel) != 0 ) {
     int i = taborderlist->indexOfTopLevelItem(pTabSel);
     taborderlist -> insertTopLevelItem( i - 1,taborderlist-> takeTopLevelItem(i));
     taborderlist->selectionModel()-> clearSelection();
     taborderlist->setCurrentItem(pTabSel);
   }
   p_has_changed=true;
   check_buttons();
}

void hk_kdetaborder::ok_clicked()
{
  list<int> i;  
  QTreeWidgetItem * pCurrent = taborderlist -> invisibleRootItem()->child(0);
  
  while(pCurrent){
    i.push_back(pCurrent->text(0).toInt());
    pCurrent = taborderlist -> itemBelow(pCurrent);
  }

  p_form->set_taborder(i); 
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Taborder");
  cg.writeEntry("Geometry",geometry());

  accept();
}
