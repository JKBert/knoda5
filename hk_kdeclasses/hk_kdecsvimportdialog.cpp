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
//$Revision: 1.32 $

#include "hk_kdecsvimportdialog.h"
#include "hk_kdecsvimportdialog.moc"
#include <hk_database.h>

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qtablewidget.h>
#include <qheaderview.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qcheckbox.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qprogressdialog.h>
#include <QKeyEvent>
#include <kfiledialog.h>
#include <klocale.h>
#include <qapplication.h>
#include <qframe.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kglobal.h>
#include <ktoolinvocation.h>

/*
 *  Constructs a hk_kdecsvimportdialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */



bool    hk_kdecsvimportdialog::p_cancelimport=false;
QProgressDialog* hk_kdecsvimportdialog::p_progressdialog=NULL;


hk_kdecsvimportdialog::hk_kdecsvimportdialog( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdecsvimportdialogbase( parent, name, modal, fl ),hk_importcsv()
{
    moreframe->hide();
    if ( !name )
        setObjectName( "hk_kdecsvimportdialog" );
    setWindowTitle( i18n( "CSV Import Dialog"  ) );
    TextLabel1->setText( i18n( "Filename:"  ) );
    TextLabel3->setText( i18n( "Text delimiter"  ) );
    TextLabel1_3->setText( i18n( "Field names in first row"  ));
    TextLabel1_2->setText( i18n("Tablename:"  ) );
    TextLabel2->setText( i18n("Column Separator"  ));
    p_tab=i18n("Tab");
    columnseparatorfield->setEditable(true);
    columnseparatorfield->addItem(  ","  );
    columnseparatorfield->addItem(  ";"  );
    columnseparatorfield->addItem(  "|"  );
    columnseparatorfield->addItem( p_tab);
    for (unsigned int i=0;i<sizeof(charsets)/sizeof(hk_string);++i)
        charsetfield->addItem(QString::fromUtf8(l2u(charsets[i]).c_str()));
    
    for (unsigned int i=0;i<sizeof(locales)/sizeof(hk_string);++i)
        localefield->addItem(QString::fromUtf8(l2u(locales[i]).c_str()));
    
    buttonOk->setText( i18n( "&OK"  ) );
    buttonCancel->setText( i18n( "&Cancel"  ) );
    buttonHelp->setText( i18n( "&Help"  ));
    connect(charsetfield,SIGNAL(textChanged(const QString&)),this,SLOT(buttons_enabled()));
    
    KSharedConfigPtr c=KGlobal::config();
    KConfigGroup cg = c->group("CSVImport");
    QRect const & rrect=QRect(0,0,500,300);
    QRect g;
  
    g=cg.readEntry("Geometry",rrect);
  
    filefield->setText(cg.readEntry("Filename",""));
    firstrow->setChecked(cg.readEntry("FirstRowContainsFieldNames",true));
    
    textdelimiterfield->setText(cg.readEntry("TextDelimiter","\""));
  
    timeformatfield->setText(cg.readEntry("TimeFormat",QString::fromUtf8(l2u(defaulttimeformat()).c_str())));
    dateformatfield->setText(QString::fromUtf8(l2u(defaultdateformat()).c_str()));
    datetimeformatfield->setText(cg.readEntry("DateFormat",cg.readEntry("DateTimeFormat",QString::fromUtf8(l2u(defaultdatetimeformat()).c_str()))));
   
    autoincfield->setChecked(cg.readEntry("DetectAutoInc",true));
    appendrows->setChecked(cg.readEntry("AppendRows",false));
    morebutton->setChecked(cg.readEntry("ShowOptions",false));
    
    QString defaultText = cg.readEntry("Charset",QString::fromUtf8(l2u(filecharset()).c_str()));
    int ind = charsetfield->findText(defaultText);
    if (ind != -1)
        charsetfield->setCurrentIndex(ind);
    else 
        // is editable
        charsetfield->setEditText(defaultText);
    
    defaultText = cg.readEntry("Locale",QString::fromUtf8(l2u(filelocale()).c_str()));
    ind = localefield->findText(defaultText);
    if (ind != -1)
        localefield->setCurrentIndex(ind);
    else
        // is editable
        localefield->setEditText(defaultText);
    
    defaultText = cg.readEntry("ColumnSeparator",",");
    ind = columnseparatorfield->findText(defaultText);
    if (ind != -1)
        columnseparatorfield->setCurrentIndex(ind);
    else
        // is editable
        columnseparatorfield->setEditText(defaultText);
  
    setGeometry(g);
    previewtable->adjustSize();
}


hk_kdecsvimportdialog::~hk_kdecsvimportdialog()
{
// no need to delete child widgets, Qt does it all for us
//    delete p_import;
}


void hk_kdecsvimportdialog::ok_clicked(void)
{
//  wanna_debug(true);
    set_filename(u2l(filefield->text().toUtf8().data()));
    datasource()->set_name(u2l(tablename->currentText().toUtf8().data()));
    set_firstrow_contains_fieldnames(firstrow->isChecked());
    QString sep=columnseparatorfield->currentText();
    set_progressdialog(&hk_kdecsvimportdialog::set_progress);
    if (sep==p_tab) sep="\t";
    set_detect_autoinc(autoincfield->isChecked());
    set_filedefinition(u2l(textdelimiterfield->text().toUtf8().data()),u2l(sep.toUtf8().data()),"\n",u2l(datetimeformatfield->text().toUtf8().data()),u2l(dateformatfield->text().toUtf8().data()),u2l(timeformatfield->text().toUtf8().data())
    ,u2l(charsetfield->currentText().toUtf8().data()),u2l(localefield->currentText().toUtf8().data()));
    set_append_rows(appendrows->isChecked());
    before_execute();
    bool r=execute();
    after_execute();
    if (r ) accept();
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("CSVImport");
  cg.writeEntry("Geometry",geometry());
  cg.writeEntry("Filename",filefield->text());
  cg.writeEntry("FirstRowContainsFieldNames",firstrow->isChecked());
  cg.writeEntry("ColumnSeparator",columnseparatorfield->currentText());
  cg.writeEntry("TextDelimiter",textdelimiterfield->text());
  cg.writeEntry("DateTimeFormat",datetimeformatfield->text());
  cg.writeEntry("DateFormat",dateformatfield->text());
  cg.writeEntry("TimeFormat",timeformatfield->text());
  cg.writeEntry("DetectAutoInc",autoincfield->isChecked());
  cg.writeEntry("Charset",charsetfield->currentText());
  cg.writeEntry("Locale",localefield->currentText());
  cg.writeEntry("AppendRows",appendrows->isChecked());
  cg.writeEntry("ShowOptions",(morebutton->isChecked()));

}


void hk_kdecsvimportdialog::filebutton_clicked()
{

    p_file = KFileDialog::getOpenFileName( KUrl("kfiledialog:///csv"), "*.csv\n*", this,i18n("Select a CSV file"));
    if (!p_file.isEmpty())
      filefield->setText(p_file);
}


void hk_kdecsvimportdialog::buttons_enabled()
{
    if (    !filefield->text().isEmpty()
        &&!columnseparatorfield->currentText().isEmpty()
        &&!tablename->currentText().isEmpty()
        )
        buttonOk->setEnabled(true);
    else buttonOk->setEnabled(false);
    autoincfield->setEnabled(!appendrows->isChecked());
    set_tablepreview();

}


void    hk_kdecsvimportdialog::before_execute(void)
{
   p_cancelimport=false;
   p_progressdialog= new  QProgressDialog(this);
   p_progressdialog->setWindowTitle(i18n("Importing data..."));
   p_progressdialog->setMinimum(0);
   p_progressdialog->setMaximum(100);
   connect(p_progressdialog,SIGNAL(canceled()),this,SLOT(printing_cancelled()));
   p_progressdialog->show();
   qApp->processEvents();
}


void    hk_kdecsvimportdialog::after_execute(void)
{
  delete p_progressdialog;
  p_progressdialog=NULL;
}


bool hk_kdecsvimportdialog::set_progress(long int position,long int total,const hk_string&txt)
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


void hk_kdecsvimportdialog::printing_cancelled(void)
{
    cout <<"cancelclicked"<<endl;
    p_cancelimport=true;

}

void hk_kdecsvimportdialog::morebutton_clicked()
{
    
    if (morebutton->isChecked())
     moreframe->show();
     else
     moreframe->hide();
//adjustSize();
}

bool hk_kdecsvimportdialog::widget_specific_after_new_row(void)
{
/*  if (p_progressdialog!=NULL)
  {
     p_progressdialog->setProgress(datasource()->row_position());
  }
  qApp->processEvents();*/

    return !p_cancelimport;
}

void hk_kdecsvimportdialog::keyPressEvent ( QKeyEvent * e )
{
  if (e->key()==Qt::Key_F1) help_clicked();
}



void hk_kdecsvimportdialog::help_clicked()
{
KToolInvocation::invokeHelp("importcsv");
}



void hk_kdecsvimportdialog::set_tablepreview()
{
  QTableWidgetItem *curCell;
  
  previewtable->setRowCount(0);
  if (filefield->text().isEmpty())
  {
    previewtable->setColumnCount(0);
    return;
  }
    QString sep=columnseparatorfield->currentText();
    if (sep==p_tab) sep="\t";
    set_filedefinition(u2l(textdelimiterfield->text().toUtf8().data()),u2l(sep.toUtf8().data()),"\n",u2l(datetimeformatfield->text().toUtf8().data()),u2l(dateformatfield->text().toUtf8().data()),u2l(timeformatfield->text().toUtf8().data())
    ,u2l(charsetfield->currentText().toUtf8().data()),u2l(localefield->currentText().toUtf8().data()));
  hk_string sbuffer;
  ifstream* p_filestream=new ifstream(u2l(filefield->text().toUtf8().data()).c_str(),ios::in);
  vector<hk_string>* l=NULL;
  if (firstrow->isChecked())
   {
    get_line(*p_filestream,sbuffer);
    l=create_valuelist(sbuffer);
    previewtable->setColumnCount(l->size());
    vector<hk_string>::iterator it=l->begin();
    int i=0;
    while (it!=l->end())
    {
      previewtable -> setHorizontalHeaderItem(i, new QTableWidgetItem(QString::fromUtf8(l2u(*it).c_str())));
      ++it;++i;
    }

   }
    const int maxpreview=5;
    bool eof=false;
    int i=0;
    for (i=0;i<maxpreview &&!eof;++i)
     {
        if (!p_filestream->eof())
        {
        get_line(*p_filestream,sbuffer);           //zweite bis maxpreview Datenzeile einlesen
        l=create_valuelist(sbuffer);
        if (l->size()!=(unsigned int)previewtable->columnCount()&& i==0)
           previewtable->setColumnCount(l->size());

        vector<hk_string>::iterator it=l->begin();
        if (!l->empty())
          previewtable->setRowCount(previewtable->rowCount()+1);
        int j=0;
        while (it!=l->end())
        {
          curCell = new QTableWidgetItem(QString::fromUtf8(l2u(*it).c_str()));
          curCell->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
          previewtable->setItem(previewtable->rowCount()-1,j,curCell);
          ++it;++j;
        }


        }
        else eof=true;//eof

     }
  if (!firstrow->isChecked())
  {
    if (appendrows->isChecked())
    {
      for (int i=0;i<previewtable->columnCount();)
      {
        QString n;
        n.setNum(i+1);
        previewtable->horizontalHeaderItem(i)->setText(n);
        ++i;
      }

    }
    else
    {
      for (int i=0;i<previewtable->columnCount();++i)
      {
        QString f;
        f.setNum(i);
        f=i18n("field_")+f;
        previewtable->horizontalHeaderItem(i)->setText(f);
      }
    }
  }
  previewtable->resizeRowsToContents();
  delete p_filestream;
}


void hk_kdecsvimportdialog::set_datasource(hk_datasource* d)
{
 hk_importcsv::set_datasource(d);
   tablename->clear();
 if (d)
 {
   vector<hk_string>* tbl=d->database()->tablelist();
   tablename->addItem("");
   vector<hk_string>::iterator it=tbl->begin();
   while (it!=tbl->end())
   {
     tablename->addItem(QString::fromUtf8(l2u(*it).c_str()));
     ++it;
   }
 }
}
