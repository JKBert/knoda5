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

#include "hk_kdecomboboxtextlist.h"
#include <qtablewidget.h>
#include <qpushbutton.h>
#include <QKeyEvent>
#include <hk_dscombobox.h>

hk_kdecomboboxtextlist::hk_kdecomboboxtextlist( QWidget* parent, hk_dscombobox* c, const char* name, bool modal, Qt::WFlags fl )
    : hk_kdecomboboxtextlistbase( parent, name, modal, fl )
{
  p_combobox=c;
  grid->installEventFilter(this);
  ok_button->installEventFilter(this);
  cancel_button->installEventFilter(this);
  set_existing_fields();
  grid->resizeColumnToContents(0);
}

/*  
 *  Destroys the object and frees any allocated resources
 */
hk_kdecomboboxtextlist::~hk_kdecomboboxtextlist()
{
    // no need to delete child widgets, Qt does it all for us
}


void hk_kdecomboboxtextlist::slot_data_changed(int row, int /*col*/)
{
  if ( row == grid->rowCount()-1 && grid->item(row,0) &&
	  !grid->item(row,0)->text().isEmpty())
	  add_row();
}


void hk_kdecomboboxtextlist::add_row(void)
{
  grid->setRowCount(grid->rowCount()+1);
}

bool hk_kdecomboboxtextlist::eventFilter(QObject* object,QEvent* e)
{
 QKeyEvent* event=dynamic_cast<QKeyEvent*>(e);
 if (event  && event->key()==Qt::Key_Delete && event->type()==QEvent::KeyRelease)
   {
       delete_rows(); 
       return true;
   }
  else return hk_kdecomboboxtextlistbase::eventFilter(object,e);
}

void hk_kdecomboboxtextlist::delete_rows(void)
{
  QList<QTableWidgetItem*> selList = grid->selectedItems();
  
  for(int i=0;i < selList.size();i++) 
	grid->removeRow(selList[i]->row());

}

void hk_kdecomboboxtextlist::set_existing_fields(void)
{
  if (!p_combobox)
    return;
  
  list<hk_string>* l=p_combobox->textlist();
  list<hk_string>::iterator it=l->begin();
  while (it!=l->end()) {
	grid->setItem(grid->rowCount()-1,0,new QTableWidgetItem(QString::fromUtf8(l2u((*it)).c_str())));
	++it;
  }
}


void hk_kdecomboboxtextlist::ok_clicked(void)
{
  if (p_combobox) {
    list<hk_string> l;
    for (int row=0;row<grid->rowCount()-1;++row) {
    
     hk_string txt=u2l(grid->item(row,0)->text().toUtf8().data());
     l.push_back(txt);
    }

    p_combobox->set_textlist(l);
  }
  accept();
}

