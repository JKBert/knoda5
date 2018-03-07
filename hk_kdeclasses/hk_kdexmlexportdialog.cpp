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

#include "hk_kdexmlexportdialog.h"
#include <hk_database.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <QKeyEvent>
#include <qpushbutton.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <hk_database.h>
#include <hk_connection.h>
#include <qprogressdialog.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kglobal.h>
#include <ktoolinvocation.h>



QProgressDialog* hk_kdexmlexportdialog::p_progressdialog=NULL;
bool    hk_kdexmlexportdialog::p_cancelimport=false;





hk_kdexmlexportdialog::hk_kdexmlexportdialog(const QString& table, QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdexmlexportdialogbase( parent, name, modal, fl ), hk_reportxml()
{
    p_tablename=table;
    p_datasourcetype=dt_table;
    maindocumenttagfield->setText(QString::fromUtf8(l2u(maindocumenttag()).c_str()));
    rowelementfield->setText(QString::fromUtf8(l2u(rowtag()).c_str()));
    structurefield->setChecked(includetableschema());
    structurefield->setText(i18n("include tableschema?"));
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("XMLExport");
  QRect const &  rrect=QRect(0,0,500,300); 
  QRect g;
  
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g);
}


hk_kdexmlexportdialog::~hk_kdexmlexportdialog()
{
// no need to delete child widgets, Qt does it all for us
}


void hk_kdexmlexportdialog::ok_clicked()
{
    set_filename(u2l(filefield->text().toUtf8().data()));
    long ds=new_datasource(u2l(tablenamefield->currentText().toUtf8().data()),p_datasourcetype);
    set_presentationdatasource(ds);
    set_maindocumenttag(u2l(maindocumenttagfield->text().toUtf8().data()));
    set_rowtag(u2l(rowelementfield->text().toUtf8().data()));
    set_includetableschema(structurefield->isChecked());
    set_fieldname_as_attribute(attributefield->isChecked()?fieldnameattribute:fieldname);
    p_cancelimport=false;
   p_progressdialog= new  QProgressDialog(this);
   p_progressdialog->setWindowTitle(i18n("Exporting data..."));
   p_progressdialog->setMinimum(0);
   p_progressdialog->setMaximum(100);
   connect(p_progressdialog,SIGNAL(canceled()),this,SLOT(printing_cancelled()));
   p_progressdialog->show();
   qApp->processEvents();
    if ( execute()) accept();
    delete p_progressdialog;
  p_progressdialog=NULL;

  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg= c->group("XMLExport");
  cg.writeEntry("Geometry",geometry());
}

void hk_kdexmlexportdialog::printing_cancelled(void)
{
    cout <<"cancelclicked"<<endl;
    p_cancelimport=true;

}

void hk_kdexmlexportdialog::buttons_enabled()
{
    if (    !filefield->text().isEmpty()
        &&!maindocumenttagfield->text().isEmpty()
        &&!rowelementfield->text().isEmpty()
        &&!tablenamefield->currentText().isEmpty()
        )
        buttonOk->setEnabled(true);
    else buttonOk->setEnabled(false);
}


void hk_kdexmlexportdialog::filebutton_clicked()
{
    p_file = KFileDialog::getSaveFileName( KUrl("kfiledialog:///xml"), "*.xml|\n*", this,i18n("Select a XML file"));
    if(!p_file.isEmpty())
      filefield->setText(p_file);
}


void hk_kdexmlexportdialog::set_usetablelist()
{
    p_datasourcetype=dt_table;
    set_datasourcelist();
}

void hk_kdexmlexportdialog::set_useviewlist()
{
    p_datasourcetype=dt_view;
    set_datasourcelist();
}

void hk_kdexmlexportdialog::set_usequerylist()
{
    p_datasourcetype=dt_query;
    set_datasourcelist();
}




void hk_kdexmlexportdialog::set_datasourcelist(void)
{
    tablenamefield->clear();
    hk_database* p_db=database();
    if (p_db==NULL)return ;
    vector<hk_string>* tbl=p_db->tablelist();
    switch (p_datasourcetype)
    {
    case dt_table:
    {
        //tbl already set as default;
        tablenamelabel->setText( i18n("Tablename:"  ) );
	break;
    }
    case dt_query:
    {
        tbl=p_db->querylist();
        tablenamelabel->setText( i18n("Queryname:"  ) );
	break;
    }
    case dt_view:
    {
        tbl=p_db->viewlist();
        tablenamelabel->setText( i18n("Viewname:"  ) );
	break;
    }
    }
    vector<hk_string>::iterator it;
    if (tbl!=NULL)
    {
        for(it=tbl->begin();it!=tbl->end();it++)
        {
            tablenamefield->addItem(QString::fromUtf8(l2u((*it)).c_str()));
            if (!p_tablename.isEmpty()&& p_tablename==(*it).c_str()) 
		    tablenamefield->setCurrentIndex(tablenamefield->count()-1);
        }
    }

}


void hk_kdexmlexportdialog::listtype_changed()
{
switch (typefield->currentIndex())
{
 case 1: p_datasourcetype=dt_query;break;
 case 2: p_datasourcetype=dt_view;break;
 default: p_datasourcetype=dt_table;
}
set_datasourcelist();
}


void hk_kdexmlexportdialog::set_database(hk_database* d)
{
    typefield->clear();
    typefield->addItem(i18n("Table"));
    typefield->addItem(i18n("Query"));
    hk_presentation::set_database(d);
    if (d && d->connection()->server_supports(hk_connection::SUPPORTS_VIEWS))
    {
        typefield->addItem(i18n("View"));

    }
    set_datasourcelist();
}


void hk_kdexmlexportdialog::help_clicked()
{
KToolInvocation::invokeHelp("exportxml");
}

bool hk_kdexmlexportdialog::set_progress(long int position,long int total,const hk_string&txt)
{
    if (p_progressdialog)
    {

        p_progressdialog->setWindowTitle(QString::fromUtf8(l2u(txt).c_str()));
        p_progressdialog->setMaximum(total);
        p_progressdialog->setValue(position);
        p_progressdialog->raise();
        qApp->processEvents();
    }
    return p_cancelimport;
}

void hk_kdexmlexportdialog::keyPressEvent ( QKeyEvent * e )
{
  if (e->key()==Qt::Key_F1) help_clicked();
}

 void hk_kdexmlexportdialog::excel_clicked()
 {
  maindocumenttagfield->setEnabled(!excelxmlfield->isChecked());
  rowelementfield->setEnabled(!excelxmlfield->isChecked());
  structurefield->setEnabled(!excelxmlfield->isChecked());
  attributefield->setEnabled(!excelxmlfield->isChecked());
  set_excelxml(excelxmlfield->isChecked());
 }
 
 
