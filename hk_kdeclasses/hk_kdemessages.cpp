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
//$Revision: 1.41 $

#include "hk_kdemessages.h"
#include <kmessagebox.h>
#include "hk_kdepassworddialog.h"
#include "hk_kdenewpassworddialog.h"
#include <hk_database.h>
#include <kapplication.h>
#include <kglobal.h>

#include <KLocalizedString>
#include <QFileDialog>

#include "hk_kdedriverdialog.h"
#include "hk_kdeform.h"
#include "hk_kdesimpleform.h"
#include "hk_kdeformpartwidget.h"
#include "hk_kdereport.h"
#include "hk_kdereportpartwidget.h"
#include "hk_kdesimplereport.h"
#include "hk_kdetable.h" 
#include "hk_kdetablepartwidget.h"
#include "hk_kdequery.h"
#include "hk_kdequerypartwidget.h"
#include "knodamaindockwindow.h"

#include <qapplication.h>
#include <qevent.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include <qmenubar.h>
#include <qdialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qinputdialog.h>

#include <hk_drivermanager.h>
#include <hk_datasource.h>
#include <hk_url.h>

hk_kdetextdialog::hk_kdetextdialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )

{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdetextdialog":name));
    setModal(modal);
    setSizeGripEnabled(true);
    hk_kdetextdialogLayout = new QVBoxLayout(this);
    hk_kdetextdialogLayout->setMargin(4);
    hk_kdetextdialogLayout->setSpacing(6);
    hk_kdetextdialogLayout->setObjectName("hk_kdetextdialogLayout");

    textlabel = new QLabel(this);
    textlabel->setObjectName("textlabel");
    hk_kdetextdialogLayout->addWidget( textlabel );

    textfield = new QLineEdit(this);
    textfield->setObjectName("textfield");
    textfield->setMinimumSize( QSize( 400, 0 ) );
    hk_kdetextdialogLayout->addWidget( textfield );

    Layout1 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    Layout1->setMargin(0);
    Layout1->setSpacing(6);
    Layout1->setObjectName("Layout1");
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( spacer );

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName("buttonOk");
    buttonOk->setAutoDefault(true);
    buttonOk->setDefault(true);
    Layout1->addWidget( buttonOk );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName("buttonCancel");
    buttonCancel->setAutoDefault(true);
    Layout1->addWidget( buttonCancel );
    hk_kdetextdialogLayout->addLayout( Layout1 );
    languageChange();
    resize( QSize(408, 85).expandedTo(minimumSizeHint()) );
    buttonOk->setEnabled(false);
    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( textfield, SIGNAL( textChanged(const QString&) ), this, SLOT( text_changed(const QString&) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdetextdialog::~hk_kdetextdialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdetextdialog::languageChange()
{
    setWindowTitle(i18n("MyDialog"));
    textlabel->setText(i18n("Please enter:"));
    buttonOk->setText(i18n("&OK"));
    buttonCancel->setText(i18n("&Cancel"));
}

void hk_kdetextdialog::text_changed(const QString& t)
{
hk_string txt=trim(u2l(t.toUtf8().data()));
buttonOk->setEnabled(txt.size()>0);
}

void hk_kdewarning(const hk_string& w)
{
    KMessageBox::information(0,QString::fromUtf8 (l2u(w).c_str()),i18n("Information"),0,0);
}


bool hk_kdeyesno_dialog(const hk_string& w,bool )
{
    int ret;
    QString a=i18n("Please decide");
    ret=KMessageBox::questionYesNo(0,QString::fromUtf8 (l2u(w).c_str()),a);
    if (ret==3)    return true;
    else    return false;
}


hk_string hk_kdestringvaluedialog(const hk_string& st)
{
    //bool ok;
    hk_kdetextdialog* txtdlg=new hk_kdetextdialog(0,0,true);
    txtdlg->textlabel->setText(QString::fromUtf8(l2u(st).c_str()));
    txtdlg->exec();
    if (txtdlg->result()==QDialog::Accepted)
    {
        return trim(u2l(txtdlg->textfield->text().toUtf8().data()));
    }
    else return "";
}


 bool hk_kdenewpassworddlg(hk_string& s)
{
    hk_string np;

    hk_kdenewpassworddialog* dlg= new hk_kdenewpassworddialog(0,0,true);
    dlg->exec();

    if (dlg->result()==QDialog::Accepted)
    {
        if (dlg->passwordfield->text()==dlg->repeatfield->text())
        {
            s=u2l(dlg->passwordfield->text().toUtf8().data());
            delete dlg;
            return true;
        }
    }
    delete dlg;
    return false;
}

void hk_kdedatasourceenablefunction(void)
{
  kapp->processEvents();
  //cerr <<"hk_kdedatasourceenablefunction called!"<<endl;
}

hk_form* new_form(hk_database* db,hk_class* parent)
{
  knodamaindockwindow* knoda=dynamic_cast<knodamaindockwindow*>(parent);
  hk_kdeformpartwidget* part=NULL;
  if (knoda)
  {
   part=knoda->new_form();
   part->set_database(db);
   part->show();
   return part->simpleform();
  } 

  hk_kdeform* f=new hk_kdeform(dynamic_cast<QWidget*>(parent),0);

  if (!f) return NULL;
  f->setAttribute(Qt::WA_DeleteOnClose);
  f->set_database(db);
  f->set_designmode();
  if (hk_visible::open_maximized_windows()) f->showMaximized();
  else f->show();
  return f->simpleform();
}

hk_report* new_report(hk_database* db,hk_class* parent)
{
  hk_kdereport* f=NULL;
  knodamaindockwindow* knoda=dynamic_cast<knodamaindockwindow*>(parent);
  hk_kdereportpartwidget* part=NULL;
  if (knoda)
  {
   part=knoda->new_report();
   part->set_database(db);
   part->show();
   return part->simplereport();
  }
  else
  f=new hk_kdereport(dynamic_cast<QWidget*>(parent),0);
  if (!f) return NULL;
  f->setAttribute(Qt::WA_DeleteOnClose);
  f->set_database(db);
  if (hk_visible::open_maximized_windows()) f->showMaximized();
  else f->show();
  f->set_designmode();

  return f->simplereport();
}


hk_form* new_dialogform(hk_database* db,hk_class* )
{
  hk_kdesimpleform* f=new hk_kdesimpleform(qApp->activeWindow(),0,Qt::Dialog);

  if (!f) return NULL;
  f->set_database(db);
  f->set_designmode();
  return f;
}

hk_dstable* new_table(hk_class* parent)
{
  hk_kdetablepartwidget* part=NULL;
  knodamaindockwindow* knoda=dynamic_cast<knodamaindockwindow*>(parent);
  if (knoda)
  {
    part=knoda->new_table();
    part->show();
    return part;
  }
  else
  {
    hk_kdetable* f= new hk_kdetable(dynamic_cast<QWidget*>(parent),0);
    if (!f) return NULL;
    f->setAttribute(Qt::WA_DeleteOnClose);
    if (hk_visible::open_maximized_windows()) f->showMaximized();
    else f->show();

    return f;
  }
}

hk_dsquery* new_query(hk_class* parent)
{
  hk_kdequerypartwidget* part=NULL;
  knodamaindockwindow* knoda=dynamic_cast<knodamaindockwindow*>(parent);
  if (knoda)
  {
    part=knoda->new_query();
    part->show();
    return part;
  }
  else
  {
    hk_kdequery* f= new hk_kdequery(dynamic_cast<QWidget*>(parent),0);
    if (!f) return NULL;
    f->setAttribute(Qt::WA_DeleteOnClose);
    if (hk_visible::open_maximized_windows())f->showMaximized();
    else f->show();

    return f;
  }
}


hk_string hk_kdefiledialog(const hk_string&  url,enum_operationmode mode)
{
  hk_string result;
  QUrl u = QUrl::fromLocalFile(url.c_str());
  QFileDialog fd (KApplication::activeWindow(), QString(i18n("knoda5")), u.path().isEmpty()?QString():u.toLocalFile());

  if (mode==file_save)
    fd.setAcceptMode(QFileDialog::AcceptSave);
  if (fd.exec() == QDialog::Accepted)
      result = u2l(fd.selectedFiles().first().toUtf8().data());
  return result;
}

hk_string hk_kdedirectorydialog(const hk_string& url)
{
  hk_string result;
  QUrl u = QUrl::fromLocalFile(url.c_str());
  
  result=u2l(QFileDialog::getExistingDirectory(KApplication::activeWindow(), QString(i18n("knoda5")),
   u.path().isEmpty()?QString():u.toLocalFile(), QFileDialog::ShowDirsOnly).toUtf8().data());
  if (result.empty())
    result=url;
  return result;
}

void set_kdestandarddialogs(void)
{
    hk_class::set_yesnodialog(&hk_kdeyesno_dialog);
    hk_class::set_warningmessage(&hk_kdewarning);
    hk_connection::set_passworddialog(&hk_kdepassworddlg);
    hk_connection::set_newpassworddialog(&hk_kdenewpassworddlg);
    hk_drivermanager::set_driverselectdialog( &hk_kdedriverselectdialog);
    hk_drivermanager::set_stringvaluedialog( &hk_kdestringvaluedialog);
    hk_drivermanager::set_translatefunction(&hk_kdetranslate);
    hk_database::set_new_formvisiblefunction(&new_form);
    hk_database::set_new_dialogformvisiblefunction(&new_dialogform);
    hk_database::set_new_reportvisiblefunction(&new_report);
    hk_database::set_new_tablevisiblefunction(&new_table);
    hk_database::set_new_queryvisiblefunction(&new_query);
    hk_database::set_filedialog(&hk_kdefiledialog);
    hk_database::set_directorydialog(&hk_kdedirectorydialog);
   // hk_datasource::set_enablefunction(&hk_kdedatasourceenablefunction,10000);
}


void hk_kdepassworddlg(hk_connection* connection,struct_connectionfields* c)
{
    hk_kdepassworddialog* d=new hk_kdepassworddialog(connection,0,0,c);
    d->exec();
    delete d;
}


hk_string hk_kdetranslate(const hk_string& t)
{
    QString erg=i18n(t.c_str());
    return (u2l(erg.toUtf8().data()));
}


 hk_string hk_kdedriverselectdialog(void)
{
    hk_kdedriverdialog* c = new hk_kdedriverdialog();
    c->exec();
    hk_string p=c->drivername();
    delete c;
    return p;
}


hk_string mimetype2driver(const hk_string& mimename)
{
  hk_string driver;
  if (mimename=="application/x-hk_classes-sqlite3"||mimename=="application/x-sqlite3"
      ||mimename=="application/x-kexiproject-sqlite3")
    {
      driver="sqlite3";
    }
    else
    if (mimename=="application/x-hk_classes-sqlite2"||mimename=="application/x-sqlite2"
      ||mimename=="application/x-kexiproject-sqlite2")
    {
     driver="sqlite2";
    }

    else
    if (mimename=="application/x-msaccess")
    {
     driver="mdb";
    }

    else
    if (mimename=="application/x-dbase")
    {
     driver="dbase";
    }
    else
    if (mimename=="application/x-paradox")
    {
     driver="paradox";
    }

  return driver;
}
