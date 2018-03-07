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
//$Revision: 1.29 $

#include "hk_kdecsvexportdialog.h"
#include "hk_kdecsvexportdialog.moc"

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <QKeyEvent>
#include <kfiledialog.h>
#include <klocale.h>
#include <vector>
#include <hk_database.h>
#include <hk_connection.h>
#include <qprogressdialog.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kglobal.h>
#include <ktoolinvocation.h>

bool    hk_kdecsvexportdialog::p_cancelimport=false;
QProgressDialog* hk_kdecsvexportdialog::p_progressdialog=NULL;
/*
 *  Constructs a hk_kdecsvexportdialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdecsvexportdialog::hk_kdecsvexportdialog( const QString& table, QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdecsvexportdialogbase( parent, name, modal, fl )    ,hk_reportcsv()
{
    if ( !name )
        setObjectName( "hk_kdecsvexportdialog" );
    resize( 328, 171 );
    p_tablename=table;
    setWindowTitle( i18n( "CSV export Dialog"  ) );
    setSizeGripEnabled( TRUE );
    setMaximumSize( QSize( 32767, 32767 ) );
 // signals and slots connections
    p_datasourcetype=dt_table;
    KSharedConfigPtr c=KGlobal::config();
    KConfigGroup cg = c->group("CSVExport");
    QRect const &  rrect=QRect(0,0,500,300);
    QRect g;
    g=cg.readEntry("Geometry",rrect);
    filefield->setText(cg.readEntry("Filename",""));
    firstrow->setChecked(cg.readEntry("FirstRowContainsFieldNames",true));

    textdelimiterfield->setText(cg.readEntry("TextDelimiter","\""));
  
  
    timeformatfield->setText(cg.readEntry("TimeFormat",QString::fromUtf8(l2u(defaulttimeformat()).c_str())));
    dateformatfield->setText(QString::fromUtf8(l2u(defaultdateformat()).c_str()));
    datetimeformatfield->setText(cg.readEntry("DateFormat",cg.readEntry("DateTimeFormat",QString::fromUtf8(l2u(defaultdatetimeformat()).c_str()))));
  
    morebutton->setChecked(cg.readEntry("ShowOptions",false));
    setGeometry(g);
    p_tab=i18n("Tab");
    columnseparatorfield->setEditable(true);
    columnseparatorfield->addItem(  ","  );
    columnseparatorfield->addItem(  ";"  );
    columnseparatorfield->addItem(  "|"  );
    columnseparatorfield->addItem( p_tab);

    timeformatfield->setText(QString::fromUtf8(l2u(defaulttimeformat()).c_str()));
    dateformatfield->setText(QString::fromUtf8(l2u(defaultdateformat()).c_str()));
    datetimeformatfield->setText(QString::fromUtf8(l2u(defaultdatetimeformat()).c_str()));
    for (unsigned int i=0;i<sizeof(charsets)/sizeof(hk_string);++i)
        charsetfield->addItem(QString::fromUtf8(l2u(charsets[i]).c_str()));

    for (unsigned int i=0;i<sizeof(locales)/sizeof(hk_string);++i)
        localefield->addItem(QString::fromUtf8(l2u(locales[i]).c_str()));

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
    
    morebutton_clicked();
}
 

hk_kdecsvexportdialog::~hk_kdecsvexportdialog()
{
// no need to delete child widgets, Qt does it all for us
//    delete p_export;
}


void hk_kdecsvexportdialog::ok_clicked(void)
{
//  wanna_debug(true);
    set_filename(u2l(filefield->text().toUtf8().data()));
    long ds=new_datasource(u2l(tablefield->currentText().toUtf8().data()),p_datasourcetype);
    set_presentationdatasource(ds);
    set_withheader(firstrow->isChecked());
    QString sep=columnseparatorfield->currentText();
    if (sep==p_tab) sep="\t";
    set_filedefinition(
    u2l(textdelimiterfield->text().toUtf8().data()),
    u2l(sep.toUtf8().data()),
    "\n",
    u2l(datetimeformatfield->text().toUtf8().data()),
    u2l(dateformatfield->text().toUtf8().data()),
    u2l(timeformatfield->text().toUtf8().data()),
    u2l(charsetfield->currentText().toUtf8().data()),
    u2l(localefield->currentText().toUtf8().data()));
    set_progressdialog(&hk_kdecsvexportdialog::set_progress);
    p_cancelimport=false;
    set_filecharset(u2l(charsetfield->currentText().toUtf8().data()));
    set_filelocale(u2l(localefield->currentText().toUtf8().data()));
    p_progressdialog= new  QProgressDialog(this);
    p_progressdialog->setWindowTitle(i18n("Exporting data..."));
    p_progressdialog->setMinimum(0);
    p_progressdialog->setMaximum(100);
    connect(p_progressdialog,SIGNAL(cancelled()),this,SLOT(printing_cancelled()));
    p_progressdialog->show();
    qApp->processEvents();
  if ( execute()) accept();
  delete p_progressdialog;
  p_progressdialog=NULL;

  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg =  c->group("CSVExport");
  cg.writeEntry("Geometry",geometry());
  cg.writeEntry("Filename",filefield->text());
  cg.writeEntry("FirstRowContainsFieldNames",firstrow->isChecked());
  cg.writeEntry("ColumnSeparator",columnseparatorfield->currentText());
  cg.writeEntry("TextDelimiter",textdelimiterfield->text());
  cg.writeEntry("DateTimeFormat",datetimeformatfield->text());
  cg.writeEntry("DateFormat",dateformatfield->text());
  cg.writeEntry("TimeFormat",timeformatfield->text());
  cg.writeEntry("Charset",charsetfield->currentText());
  cg.writeEntry("Locale",localefield->currentText());
  cg.writeEntry("ShowOptions",(morebutton->isChecked()));
}

void hk_kdecsvexportdialog::printing_cancelled(void)
{
    cout <<"cancelclicked"<<endl;
    p_cancelimport=true;

}

void hk_kdecsvexportdialog::filebutton_clicked()
{
    p_file = KFileDialog::getSaveFileName( KUrl("kfiledialog:///csv"), "*.csv\n*", this,i18n("Select a CSV file"));
    if (!p_file.isEmpty())
      filefield->setText(p_file);
}


void hk_kdecsvexportdialog::buttons_enabled()
{
    if (    !filefield->text().isEmpty()
        &&!columnseparatorfield->currentText().isEmpty()
        &&!tablefield->currentText().isEmpty()
        )
        buttonOk->setEnabled(true);
    else buttonOk->setEnabled(false);
}


void hk_kdecsvexportdialog::set_usetablelist()
{
    p_datasourcetype=dt_table;
    set_datasourcelist();
}

void hk_kdecsvexportdialog::set_useviewlist()
{
    p_datasourcetype=dt_view;
    set_datasourcelist();
}

void hk_kdecsvexportdialog::set_usequerylist()
{
    p_datasourcetype=dt_query;
    set_datasourcelist();
}


void hk_kdecsvexportdialog::set_database(hk_database* d)
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

void hk_kdecsvexportdialog::listtype_changed()
{
switch (typefield->currentIndex())
{
 case 1: p_datasourcetype=dt_query;break;
 case 2: p_datasourcetype=dt_view;break;
 default: p_datasourcetype=dt_table;
}
set_datasourcelist();
}


void hk_kdecsvexportdialog::set_datasourcelist(void)
{
    tablefield->clear();
    hk_database* p_db=database();
    if (p_db==NULL)return ;
    vector<hk_string>* tbl=p_db->tablelist();
    switch (p_datasourcetype)
    {
    case dt_table:
    {
        //tbl already set as default
        //TextLabel1_2->setText( i18n("Tablename:"  ) );
	break;
    }
    case dt_query:
    {
        tbl=p_db->querylist();
        //TextLabel1_2->setText( i18n("Queryname:"  ) );
	break;
    }
    case dt_view:
    {
        tbl=p_db->viewlist();
        //TextLabel1_2->setText( i18n("Viewname:"  ) );
	break;
    }
    }
    vector<hk_string>::iterator it;
    if (tbl!=NULL)
    {
        for(it=tbl->begin();it!=tbl->end();it++)
        {
            QString l=QString::fromUtf8 (l2u((*it)).c_str());
            tablefield->addItem(l);
            if (!p_tablename.isEmpty()&& p_tablename==l) tablefield->setCurrentIndex(tablefield->count()-1);
        }
    }

}


void hk_kdecsvexportdialog::keyPressEvent ( QKeyEvent * e )
{
  if (e->key()==Qt::Key_F1) help_clicked();
}



void hk_kdecsvexportdialog::help_clicked()
{
KToolInvocation::invokeHelp("exportcsv");
}


bool hk_kdecsvexportdialog::set_progress(long int position,long int total,const hk_string&txt)
{
    if (p_progressdialog)
    {
        p_progressdialog->setWindowTitle(QString::fromUtf8(l2u(txt).c_str()));
	p_progressdialog->setMinimum(0);
        p_progressdialog->setMaximum(total);
        p_progressdialog->setValue(position);
        p_progressdialog->raise();
        qApp->processEvents();
    }
    return p_cancelimport;
}

void hk_kdecsvexportdialog::morebutton_clicked()
{
    
    if (morebutton->isChecked())
     moreframe->show();
     else
     moreframe->hide();
//adjustSize();
}

