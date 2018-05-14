// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
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
#include "hk_kderelationdialog.h"
#include "hk_kdedbdesigner.h"
#include <hk_datasource.h>
#include <hk_database.h>
#include <hk_presentation.h>
#include <hk_connection.h>

#include <qtablewidget.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qcombobox.h>
#include <qbuttongroup.h>
#include <qstackedwidget.h>
#include <qevent.h>
#include <qsignalmapper.h>

#include <kapplication.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <klocale.h>
#include <kglobal.h>

hk_kderelationdialog::hk_kderelationdialog( hk_kdedatasourceframe* master, hk_kdedatasourceframe* slave,hk_kdedbdesigner* parent,hk_kdedbrelation* r,  const char* name, bool modal, Qt::WFlags fl )
    : hk_kderelationdialogbase( parent, name, modal, fl ),p_masterdsframe(master),p_slavedsframe(slave),
    p_designer(parent),p_relation(r),p_signalmerge(new QSignalMapper(grid))
{
  const int w=180;
  grid->installEventFilter(this);
  ok_button->installEventFilter(this);
  cancel_button->installEventFilter(this);
  grid->setColumnWidth(MASTER_COL,w);
  grid->setColumnWidth(SLAVE_COL,w);
  QString mastername=QString::fromUtf8(l2u(
			master->designer()->presentation()
				->unique_shortdatasourcename(master->datasource()->presentationnumber())).c_str());
  QString slavename=QString::fromUtf8(l2u(
			master->designer()->presentation()
				->unique_shortdatasourcename(slave->datasource()->presentationnumber())).c_str());
  grid->horizontalHeaderItem(MASTER_COL)->setText(mastername);
  grid->horizontalHeaderItem(SLAVE_COL)->setText(slavename);
  QString msg=i18n("All of '%1' and equivalent rows of '%2'").arg(mastername).arg(slavename);

  button_equivalent->setText(msg);
  set_stringlist(&p_mastercollist,p_masterdsframe->datasource()->columnnames());
  set_stringlist(&p_slavecollist,p_slavedsframe->datasource()->columnnames());
  add_row();
  set_existing_fields();
  connect(p_signalmerge,SIGNAL(mapped(QWidget*)),this,SLOT(slot_data_changed(QWidget*)));
  KSharedConfigPtr c=KGlobal::config();
  const QRect& rrect = QRect(0,0,500,300);
  KConfigGroup cg = c->group("Relationdialog");
  QRect g;
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g);
  int cw=150;
  cw=cg.readEntry("ColwidthLeft",cw);
  grid->setColumnWidth(MASTER_COL,cw);
  cw=cg.readEntry("ColwidthRight",cw);
  grid->setColumnWidth(SLAVE_COL,cw);
  if (master&&master->designer()->presentation()&&master->designer()->presentation()&&master->designer()->presentation()->presentationtype()==hk_presentation::referentialintegrity)
  {  
   detailbox->show();
   detailbox->setCurrentIndex(0);
  }
  else
  {
   if (master->designer()->presentation()->presentationtype()==hk_presentation::qbe)
   {
   detailbox->show();
   detailbox->setCurrentIndex(1);
   if (master->datasource())
   {
     button_equivalent->setEnabled(p_slavedsframe->datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_JOINS));
   }
   
   
   }
   else
   detailbox->hide();
  }

  QString cascade=i18n("Cascade");
  QString restrict=i18n("Restrict");
  onupdatefield->addItem(restrict);
  onupdatefield->addItem(cascade);
  ondeletefield->addItem(restrict);
  ondeletefield->addItem(cascade);
  if (r)
  {
     onupdatefield->setCurrentIndex(r->updatecascade());
     ondeletefield->setCurrentIndex(r->deletecascade());
  }
}



hk_kderelationdialog::~hk_kderelationdialog()
{
    // no need to delete child widgets, Qt does it all for us
}





void hk_kderelationdialog::add_row(void)
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

 



void hk_kderelationdialog::set_stringlist(QStringList* strlist,list<hk_string>* collist)
{
    strlist->clear();
    strlist->append("");
   list<hk_string>::iterator it= collist->begin();
   while (it!=collist->end())
   {
       strlist->append(QString::fromUtf8(l2u((*it)).c_str()));
     ++it;
   }

}





void hk_kderelationdialog::set_existing_fields(void)
{
  if (!p_slavedsframe->datasource()) return;
  if (p_slavedsframe->designer()->presentation()->presentationtype()==hk_presentation::referentialintegrity)
  {
    set_existing_refintegrityfields();
    return;
  }

  if (p_slavedsframe->datasource()->depending_on()!=p_masterdsframe->datasource())
  {

      return;
  }
  list<hk_string>* slavefields=p_slavedsframe->datasource()->depending_on_thisfields();
  list<hk_string>* masterfields=p_slavedsframe->datasource()->depending_on_masterfields();
  list<hk_string>::iterator slaveit=slavefields->begin();
  list<hk_string>::iterator masterit=masterfields->begin();
  while (slaveit!=slavefields->end())
  {
    (static_cast<QComboBox*>(grid->cellWidget(grid->rowCount()-1,MASTER_COL)))->setCurrentIndex(
      textposition(&p_mastercollist,*masterit) );
    (static_cast<QComboBox*>(grid->cellWidget(grid->rowCount()-1,SLAVE_COL)))->setCurrentIndex(
      textposition(&p_slavecollist,*slaveit) );

    ++slaveit;
    if (masterit!=masterfields->end()) ++masterit;
    add_row();
  }
  button_equivalent->setChecked(p_slavedsframe->datasource()->depending_on_is_left_join());
}


void hk_kderelationdialog::set_existing_refintegrityfields(void)
{
  list <referentialclass>* rl= p_slavedsframe->datasource()->referenceslist();
  list <referentialclass>::iterator rit=rl->begin();
  while (rit!=rl->end())
  {
    if ((*rit).p_masterdatasource==p_masterdsframe->datasource()->name())
    {
      list<dependingclass>::iterator it=(*rit).p_fields.begin();
      while (it!=(*rit).p_fields.end())
      {
        (static_cast<QComboBox*>(grid->cellWidget(grid->rowCount()-1,MASTER_COL)))->setCurrentIndex(
          textposition(&p_mastercollist,(*it).masterfield) );
        (static_cast<QComboBox*>(grid->cellWidget(grid->rowCount()-1,SLAVE_COL)))->setCurrentIndex(
          textposition(&p_slavecollist,(*it).dependingfield));

        ++it;
        add_row();

        }
        onupdatefield->setCurrentIndex(((*rit).p_updatecascade?1:0));
        ondeletefield->setCurrentIndex(((*rit).p_deletecascade?1:0));
        return;
      }
    ++rit;
  }
}

int hk_kderelationdialog::textposition(QStringList* thelist,const hk_string& txt )
{
int result= thelist->indexOf(QString::fromUtf8(l2u(txt).c_str()));
return result>0?result:0;
}



int hk_kderelationdialog::masterindex(const QString& string)
{
return textposition(&p_mastercollist,u2l(string.toUtf8().data()));
}




int hk_kderelationdialog::slaveindex(const QString& string)
{
return textposition(&p_slavecollist,u2l(string.toUtf8().data()));
}




void hk_kderelationdialog::ok_button_clicked(void)
{
  bool result=true;
  if(p_slavedsframe->designer()->presentation()->presentationtype()==hk_presentation::referentialintegrity)
    set_referentialintegrity();
  else
    result=set_relation();

  if (result) accept();
  else reject();
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Relationdialog");
  cg.writeEntry("Geometry",geometry());
  cg.writeEntry("ColwidthLeft",grid->columnWidth(MASTER_COL));
  cg.writeEntry("ColwidthRight",grid->columnWidth(SLAVE_COL));

}


bool hk_kderelationdialog::set_relation(void)
{
  if (p_slavedsframe->datasource()&&p_slavedsframe->datasource()->depending_on())
    {
       hk_kdedbrelation* r=p_designer->get_relation(p_slavedsframe->datasource()->depending_on(),p_slavedsframe->datasource());
       if (r && r!=p_relation) 
       {
       if (!show_yesnodialog(replace_all("%2",
       			replace_all("%1",
       			hk_class::hk_translate("The relation between '%1' and '%2' is in the way. Remove it?"),
       			p_designer->presentation()->unique_shortdatasourcename(p_slavedsframe->datasource()->presentationnumber())),
       			p_designer->presentation()->unique_shortdatasourcename(p_slavedsframe->datasource()->depending_on()->presentationnumber())),true)) return false ;

       p_designer->delete_relation(r);
       }

    }


p_slavedsframe->datasource()->clear_depending_fields();
 p_slavedsframe->datasource()->set_depending_on_presentationdatasource(
 		p_masterdsframe->datasource()->presentationnumber(),
		p_slavedsframe->datasource()->depending_on_react_on_data_changes(),
		p_slavedsframe->datasource()->dependingmode()
		);
p_slavedsframe->datasource()->set_depending_on_is_left_join(button_equivalent->isChecked());
for (int row=0;row<grid->rowCount()-1;++row)  
  {
    hk_string mastertxt=u2l(static_cast<QComboBox*>(grid->cellWidget(row,MASTER_COL))->currentText().toUtf8().data());
    hk_string slavetxt=u2l(static_cast<QComboBox*>(grid->cellWidget(row,SLAVE_COL))->currentText().toUtf8().data());
    if (mastertxt.size()>0 && slavetxt.size()>0)
        p_slavedsframe->datasource()->add_depending_fields(slavetxt,mastertxt,true);
  }
return true;
}

void hk_kderelationdialog::set_referentialintegrity(void)
{
      if (p_relation && p_relation->referentialname().size()>0)
      {
        if (!p_slavedsframe->datasource()->drop_reference( p_relation->referentialname()))
        {
         hk_class::show_warningmessage(hk_class::hk_translate("Error: could not delete old reference"));
         return;
        }
      }
      referentialclass ref;
      ref.p_masterdatasource=p_masterdsframe->datasource()->name();
      for (int row=0;row<grid->rowCount()-1;++row)        
      {
        dependingclass dep;
        dep.masterfield=u2l(static_cast<QComboBox*>(grid->cellWidget(row,MASTER_COL))->currentText().toUtf8().data());
        dep.dependingfield=u2l(static_cast<QComboBox*>(grid->cellWidget(row,SLAVE_COL))->currentText().toUtf8().data());
        if (dep.masterfield.size()>0 && dep.dependingfield.size()>0)
        ref.p_fields.push_back(dep);
      }
      ref.p_updatecascade=(onupdatefield->currentIndex()==1);
      ref.p_deletecascade=(ondeletefield->currentIndex()==1);
      p_slavedsframe->datasource()->add_reference(ref);
   if (p_relation)
   {
   p_relation->set_referentialintegrity(ref,p_masterdsframe,p_slavedsframe);

   }
}

void hk_kderelationdialog::slot_data_changed(QWidget* pcur)
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
  ok_button->setEnabled(pmaster&&pslave&&!pmaster->currentText().isEmpty() &&!pslave->currentText().isEmpty());  
  grid->selectionModel()->clearSelection();
}




void hk_kderelationdialog::keyPressEvent(QKeyEvent* /*event*/)
{
}



void hk_kderelationdialog::delete_rows(void)
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




bool hk_kderelationdialog::eventFilter(QObject* object,QEvent* e)
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


