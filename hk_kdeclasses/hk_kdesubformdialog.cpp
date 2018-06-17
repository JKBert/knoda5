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

#include "hk_kdesubformdialog.h"
#include "hk_kdedbdesigner.h"
#include <hk_datasource.h>
#include <hk_presentation.h>
#include <hk_subform.h>
#include <hk_form.h>
#include <qtablewidget.h>
#include <qcombobox.h>
#include <qsignalmapper.h>
#include <qpushbutton.h>
#include <QKeyEvent>

hk_kdesubformdialog::hk_kdesubformdialog( hk_subform* subform,QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
        : hk_kderelationdialogbase( parent, name, modal, fl ),p_masterds(subform->datasource()),p_subform(subform),
        p_signalmerge(new QSignalMapper(grid))
{
    const int w=180;
    grid->installEventFilter(this);
    ok_button->installEventFilter(this);
    cancel_button->installEventFilter(this);
    grid->setColumnWidth(MASTER_COL,w);
    grid->setColumnWidth(SLAVE_COL,w);
    p_form=new hk_form();

    if (p_masterds) p_form->set_database(p_masterds->database());
    p_form->load_form(p_subform->name());
    p_slaveds=p_form->datasource();

    if (p_masterds)
    {
        grid->horizontalHeaderItem(MASTER_COL)->setText(QString::fromUtf8(l2u(p_masterds->name()).c_str()));
        set_stringlist(&p_mastercollist,p_masterds->columns());
    }
    if (p_slaveds)
    {
        grid->horizontalHeaderItem(SLAVE_COL)->setText(QString::fromUtf8(l2u(p_slaveds->name()).c_str()));
        set_stringlist(&p_slavecollist,p_slaveds->columns());
    }
    add_row();
    set_existing_fields();
    connect(p_signalmerge,SIGNAL(mapped(QWidget*)),this,SLOT(slot_data_changed(QWidget*)));
}

hk_kdesubformdialog::~hk_kdesubformdialog()
{
    delete p_form;
}

void hk_kdesubformdialog::add_row(void)
{
  QComboBox* masteritem = new QComboBox(grid);
  QComboBox* slaveitem = new QComboBox(grid);
  
  masteritem -> addItems(p_mastercollist);
  slaveitem -> addItems(p_slavecollist);
  grid -> setRowCount(grid -> rowCount()+1);
  grid -> setCellWidget(grid -> rowCount()-1,MASTER_COL, masteritem);
  grid -> setCellWidget(grid -> rowCount()-1,SLAVE_COL, slaveitem);
  masteritem -> setProperty("row",grid -> rowCount()-1);
  slaveitem -> setProperty("row",grid -> rowCount()-1);
  masteritem -> setProperty("col",MASTER_COL);
  slaveitem -> setProperty("col",SLAVE_COL);  
  masteritem->installEventFilter(this);
  slaveitem->installEventFilter(this);
  connect(masteritem,SIGNAL(currentIndexChanged(int)),p_signalmerge,SLOT(map()));
  connect(slaveitem,SIGNAL(currentIndexChanged(int)),p_signalmerge,SLOT(map()));
  // each row is identified by masteritem cell widget
  p_signalmerge->setMapping(masteritem,masteritem);
  p_signalmerge->setMapping(slaveitem,masteritem);    
}

void hk_kdesubformdialog::set_stringlist(QStringList* strlist,list<hk_column*>* collist)
{
    strlist->clear();
    strlist->append("");
    list<hk_column*>::iterator it= collist->begin();
    while (it!=collist->end())
    {
        strlist->append(QString::fromUtf8(l2u((*it)->name()).c_str()));
        ++it;
    }
}

void hk_kdesubformdialog::set_existing_fields(void)
{ 
  if (!p_subform)return;
  list<dependingclass>* l=p_subform->depending_on_fields();
  list<dependingclass>::iterator it=l->begin();
  while (it!=l->end())
  {
    int masterpos=textposition(&p_mastercollist,(*it).masterfield);
    if (masterpos>=0)
      (static_cast<QComboBox*>(grid->cellWidget(grid->rowCount()-1,MASTER_COL)))->setCurrentIndex(masterpos);
    int dependingpos=textposition(&p_slavecollist,(*it).dependingfield);
    if (dependingpos>=0)
      (static_cast<QComboBox*>(grid->cellWidget(grid->rowCount()-1,SLAVE_COL)))->setCurrentIndex(dependingpos);    
    ++it;
    add_row();
  }
}

int hk_kdesubformdialog::textposition(QStringList* thelist,const hk_string& txt )
{
    return thelist->indexOf(QString::fromUtf8(l2u(txt).c_str()));
}

int hk_kdesubformdialog::masterindex(const QString& string)
{
    return textposition(&p_mastercollist,u2l(string.toUtf8().data()));
}

int hk_kdesubformdialog::slaveindex(const QString& string)
{
    return textposition(&p_slavecollist,u2l(string.toUtf8().data()));
}

void hk_kdesubformdialog::ok_button_clicked(void)
{
  p_subform->clear_depending_fields();
  for (int row=0;row<grid->rowCount()-1;++row)
  {
    hk_string mastertxt=u2l(static_cast<QComboBox*>(grid->cellWidget(row,MASTER_COL))->currentText().toUtf8().data());
    hk_string slavetxt=u2l(static_cast<QComboBox*>(grid->cellWidget(row,SLAVE_COL))->currentText().toUtf8().data());    
    if (mastertxt.size()>0 && slavetxt.size()>0)
        p_subform->add_depending_fields(slavetxt,mastertxt,true);
  }

  accept();
}

void hk_kdesubformdialog::slot_data_changed(QWidget* pcur)
{  
  int rw = pcur -> property("row").toInt();
  QComboBox* pmaster = static_cast<QComboBox*>(pcur);
  QComboBox* pslave = static_cast<QComboBox*>(grid->cellWidget(rw,1));
  
  if (rw==grid->rowCount()-1 && pslave
        && !pmaster->currentText().isEmpty()
        && !pslave->currentText().isEmpty() )
  {
    add_row();
  }
  grid->selectionModel()->clearSelection();
}

void hk_kdesubformdialog::keyPressEvent(QKeyEvent* /*event*/)
{
}

void hk_kdesubformdialog::delete_rows(void)
{ 
  int beginrow=grid->selectedRanges().isEmpty()?-1:grid->selectedRanges().first().topRow();

  if (beginrow < 0) beginrow = grid -> currentRow();
  if (beginrow==grid->rowCount()-1) return;  

  if (beginrow > -1) {
    grid->removeCellWidget(beginrow,MASTER_COL);
    grid->removeCellWidget(beginrow,SLAVE_COL);
    grid->removeRow(beginrow);
  }
  slot_data_changed(grid->cellWidget(beginrow,MASTER_COL));
  // once a row is deleleted, rows with higher row id must be adjusted
  for(int i=beginrow;i<grid->rowCount();i++) {
    static_cast<QComboBox*>(grid->cellWidget(beginrow,MASTER_COL))->setProperty("row",i);
    static_cast<QComboBox*>(grid->cellWidget(beginrow,SLAVE_COL))->setProperty("row",i);
  }
}

bool hk_kdesubformdialog::eventFilter(QObject* object,QEvent* e)
{
    QKeyEvent* event=dynamic_cast<QKeyEvent*>(e);
    if (event  && event->key()==Qt::Key_Delete && event->type()==QEvent::KeyRelease)
    {
      if (object -> property("row").isValid())
        grid -> setCurrentCell(object -> property("row").toInt(),object -> property("col").toInt());

      delete_rows();
      return true;
    }
    else return hk_kderelationdialogbase::eventFilter(object,e);
}
