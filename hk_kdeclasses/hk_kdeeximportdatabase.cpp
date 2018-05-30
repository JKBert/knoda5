// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
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

#include "hk_kdeeximportdatabase.h"
#include <hk_database.h>
#include <hk_connection.h>

#include <qtextedit.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qcursor.h>
#include <qprogressdialog.h>
#include <qlabel.h>
#include <QKeyEvent>

//tbp #include <kiconloader.h>
//tbp #include <kstandarddirs.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kfiledialog.h>
#include <kdirselectdialog.h>
#include <klocale.h>
#include <kconfig.h>
#include <kglobal.h>
// tbp #include <ktoolinvocation.h>
#include <KConfigGroup>
#include <KHelpClient>

#include "hk_kdedblistview.h"
#include "hk_kdenewdatabase.h"

bool hk_kdeeximportdatabase::p_cancelcopying=false;
QProgressDialog* hk_kdeeximportdatabase::p_progressdialog=NULL;


/*
 *  Constructs a hk_kdeeximportdatabase which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */


hk_kdeeximportdatabase::hk_kdeeximportdatabase( hk_database* db, hk_connection* con, enum_mode mode, QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : hk_kdeeximportdatabasebase( parent, name, modal, fl ), hk_dbvisible()
{
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group(mode==m_import?"Importdatabase":"Exportdatabase");
  const QRect & rrect=QRect(0,0,500,300);
  QRect g;
  
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g);

  p_mode=mode;
  p_connection=con;
  p_database=db;
  //tbp KIconLoader* loader=KIconLoader::global();
  //tbp loader->addAppDir("hk_kde4classes");
  uploadbutton->setIcon(QIcon::fromTheme("go-next"));

  if (!db ||!con)
  {
    hk_class::show_warningmessage(hk_class::hk_translate("Error while creating export/importdb dialog!\nDatabase or connection not set!"));
    return;
  }
  hk_connection* from=NULL;
  hk_connection* to=NULL;
  if (mode==m_export)
   {
     from=db->connection();
     to=con;
   }
  else
   {
     to=db->connection();
     from=con;
   }
  QString v=QString::fromUtf8(l2u(hk_connection::check_capabilities(from,to)).c_str());
   outputfield->setText(v);

   p_right=new hk_kdedblistview(this);
   int p=layout7->indexOf( right_listview );
   delete right_listview;
   layout7->insertWidget(p,p_right);
   right_listview=p_right;

   p_left=new hk_kdedblistview(this);
   p=layout6->indexOf( left_listview );
   delete left_listview;
   layout6->insertWidget(p,p_left);
   left_listview=p_left;
   setWindowTitle(mode==m_import?i18n("Import database dialog"):i18n("Export database dialog"));

   set_objects();
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeeximportdatabase::~hk_kdeeximportdatabase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 * public slot
 */
void hk_kdeeximportdatabase::upload_clicked()
{  
    cerr <<"upload_clicked()"<<endl;
     hk_kdedblistview* lv=p_left;
     hk_connection* c=p_right->database()->connection();
     if (lv->is_tableitem())
       {     cerr <<"is_table:"<<u2l(lv->currentItem()->text(0).toUtf8().data())<<endl;
           if (!c->server_supports(hk_connection::SUPPORTS_NEW_TABLE))
           {
              show_warningmessage(hk_translate("Driver does not support creating new tables"));
              return;
           }

     	hk_datasource* ds=p_left->database()->new_table(u2l(lv->currentItem()->text(0).toUtf8().data()));
	p_right->database()->copy_table(ds,copyfield->isChecked(),overwritefield->isChecked(),false);
	delete ds;
       }
     else
     if (lv->is_viewitem())
       {     cerr <<"is_view:"<<u2l(lv->currentItem()->text(0).toUtf8().data())<<endl;
           if (!c->server_supports(hk_connection::SUPPORTS_VIEWS))
           {
              show_warningmessage(hk_translate("Driver does not support creating new views"));
              return;
           }
     	hk_datasource* ds=p_left->database()->new_view(u2l(lv->currentItem()->text(0).toUtf8().data()));
	p_right->database()->copy_view(ds);
	delete ds;
       }
     else
     if (lv->is_databaseitem())
      {
           copy_whole_database();
      }
     else     if (lv->is_tableheader())
      {
        copy_all_tables();
      }
     else
     if (lv->is_queryheader())
      {
        copy_all_queries();

      }
     else
     if (lv->is_viewheader())
      {
        copy_all_views();

      }
     else
     if (lv->is_formheader())
      {
        copy_all_forms();

      }
     else
     if (lv->is_reportheader())
      {
        copy_all_reports();

      }
     else
       upload_file();
}


void hk_kdeeximportdatabase::upload_file()
{
  hk_kdedblistview* lv=p_left;
  filetype f=ft_report;
  if (lv->is_queryitem()) f=ft_query;
  else
  if (lv->is_formitem()) f=ft_form;
  else
  if (!lv->is_reportitem())
  {
    show_warningmessage("Bug hk_kdeeximportdatabase::upload_file: Unknown format");
    return;

  }
  upload_file(u2l(lv->currentItem()->text(0).toUtf8().data()),f);

}

void hk_kdeeximportdatabase::upload_file(const hk_string& filename,filetype f)
{
  cerr <<"upload_file:"<<filename<<endl;

  hk_string data=p_left->database()->load(filename,f);
  p_right->database()->save(data,u2l(filename),f,!overwritefield->isChecked(),false);

}



/*
 * public slot
 */
void hk_kdeeximportdatabase::slot_selection_changed()
{
    qWarning( "hk_kdeeximportdatabase::slot_selection_changed() not yet implemented!" );
}

void hk_kdeeximportdatabase::set_objects()
{
p_connectiondatabase=p_connection->new_database();
QString edittxt=QString::fromUtf8(l2u(p_connection->drivername()+" "+p_connection->user()+"@"
				  +p_connection->host()).c_str());
QString dbtxt=QString::fromUtf8(l2u(p_database->connection()->drivername()+" "
                               +p_connection->user()+"@"
                               +p_database->connection()->host()).c_str());
  if (p_mode==m_import)
   {
     rightlabel->setText(dbtxt);
     leftlabel->setText(edittxt);
     p_right->set_database(p_database);
     p_left->set_database(p_connectiondatabase);
   }
  else
   {
     leftlabel->setText(dbtxt);
     rightlabel->setText(edittxt);
     p_left->set_database(p_database);
     p_right->set_database(p_connectiondatabase);

   }
   set_database(p_connectiondatabase);
   rightdatabasefield->setEnabled(p_mode!=m_import);
   rightnewbutton->setEnabled(p_mode!=m_import&&p_connection->server_supports(hk_connection::SUPPORTS_NEW_DATABASE));
   leftdatabasefield->setEnabled(p_mode==m_import);
   leftnewbutton->setEnabled(p_mode==m_import &&p_connection->server_supports(hk_connection::SUPPORTS_LOCAL_FILEFORMAT));
   connect((p_mode==m_import?leftdatabasefield:rightdatabasefield),SIGNAL(activated(int)),this,SLOT(slot_database_selected(int)));
   dblist_changes();
   slot_database_selected(0);
}



void hk_kdeeximportdatabase::list_changes(listtype /*type*/)
{
 dblist_changes();
}

void hk_kdeeximportdatabase::dblist_changes(void)
{
  QComboBox* c=(p_mode==m_import?leftdatabasefield:rightdatabasefield);

  QString curtext=c->currentText();
  if (p_connection&& p_connection->server_needs(hk_connection::NEEDS_DATABASENAME)&& curtext.isEmpty()) curtext=QString::fromUtf8(l2u(p_connection->defaultdatabase()).c_str());
  c->clear();
  if (!p_connection) return;
  vector<hk_string>* db=p_connection->dblist();
  vector<hk_string>::iterator it;
  c->blockSignals(true);
  bool dbfound=false;
  if (db!=NULL)
     {
        int i=0;
        for(it=db->begin();it!=db->end();it++)
          {
            c->addItem(QString::fromUtf8(l2u((*it)).c_str()));
             if (curtext==QString::fromUtf8(l2u((*it)).c_str()))
               {
                c->setCurrentIndex(i);
		dbfound=true;
               }
            i++;
          }

     }
     if (!dbfound)c->setCurrentIndex(0);

  c->blockSignals(false);



}


void hk_kdeeximportdatabase::slot_database_selected(int)
{

if (!p_connection ) return;

  QString newdb;
  QComboBox* c=(p_mode==m_import?leftdatabasefield:rightdatabasefield);
  newdb=c->currentText();
   hk_kdedblistview* lv=(p_mode==m_import?p_left:p_right);

  p_connectiondatabase->set_name(u2l(newdb.toUtf8().data()));
  lv->set_database(p_connectiondatabase);

}

void hk_kdeeximportdatabase::copying_cancelled()
{
    p_cancelcopying=true;
}

bool hk_kdeeximportdatabase::set_progress(long int position,long int total,const hk_string&txt)
{
    if (p_progressdialog)
    {

        p_progressdialog->setWindowTitle(QString::fromUtf8(l2u(txt).c_str()));
	p_progressdialog->setMinimum(0);
        p_progressdialog->setMaximum(total);
        p_progressdialog->setValue(position);
        qApp->processEvents();
    }
    return p_cancelcopying;
}


void hk_kdeeximportdatabase::copy_whole_database(void)
{
  hk_connection* c=p_right->database()->connection();
  if (!c->server_supports(hk_connection::SUPPORTS_NEW_DATABASE))
  {
    show_warningmessage(hk_translate("Driver does not support creating new databases"));
    return;
  }
  if (!show_yesnodialog(hk_translate("Copy whole database?"),true)) return;
  hk_database*   b=p_left->database();
    p_cancelcopying=false;
    QCursor crs=cursor();
    p_progressdialog= new  QProgressDialog(this);
    p_progressdialog->resize(300,p_progressdialog->height());
    connect(p_progressdialog,SIGNAL(canceled()),this,SLOT(copying_cancelled()));
    p_progressdialog->show();
    qApp->processEvents();
    if (!c->copy_database(b,copyfield->isChecked(),true,&hk_kdeeximportdatabase::set_progress))
    {
        hk_string reason=hk_translate("Copy whole database failed!")+"\n"+hk_translate("Servermessage: ")+c->last_servermessage();
        show_warningmessage(reason);

    }
    p_progressdialog->hide();

    delete p_progressdialog;
    p_progressdialog=NULL;
    setCursor(crs);
}

void hk_kdeeximportdatabase::copy_all_tables(void)
{
  hk_connection* c=p_right->database()->connection();
    if (!c->server_supports(hk_connection::SUPPORTS_NEW_TABLE))
  {
    show_warningmessage(hk_translate("Driver does not support creating new tables"));
    return;
  }
  if (!show_yesnodialog(hk_translate("Copy all tables?"),true)) return;
  

  hk_database*   t=p_right->database();
  hk_database*   b=p_left->database();
    p_cancelcopying=false;
        QCursor crs=cursor();

    p_progressdialog= new  QProgressDialog();
    p_progressdialog->resize(300,p_progressdialog->height());
    connect(p_progressdialog,SIGNAL(canceled()),this,SLOT(copying_cancelled()));
    p_progressdialog->show();
    qApp->processEvents();

    vector<hk_string>::iterator it=b->tablelist()->begin();
    bool cancel=false;
    while (it!=b->tablelist()->end()&&!cancel)
    {
        hk_datasource* fds=b->new_table((*it));
        cancel=!t->copy_table(fds,copyfield->isChecked(),true,overwritefield->isChecked(),&hk_kdeeximportdatabase::set_progress);
        delete fds;
        it++;
    }


    delete p_progressdialog;
    p_progressdialog=NULL;
     setCursor(crs);
}

void hk_kdeeximportdatabase::copy_all_queries(void)
{
  copy_all_files(ft_query);
}


void hk_kdeeximportdatabase::copy_all_files(filetype f)
{
   hk_string message;
   switch (f)
   {
     case ft_query:message=hk_translate("Copy all queries?");break;
     case ft_form:message=hk_translate("Copy all forms?");break;
     case ft_report:message=hk_translate("Copy all reports?");break;
     default: show_warningmessage("Bug hk_kdeeximportdatabase::copy_all_files: Unknown filetype ");
              return;

   }

  if (!show_yesnodialog(message,true)) return;
    p_cancelcopying=false;
    QCursor crs=cursor();

    p_progressdialog= new  QProgressDialog();
    p_progressdialog->resize(300,p_progressdialog->height());
    connect(p_progressdialog,SIGNAL(cancelled()),this,SLOT(copying_cancelled()));
    p_progressdialog->show();
    qApp->processEvents();
    vector<hk_string>* objectlist=NULL;
   switch (f)
   {
     case ft_query:objectlist=p_left->database()->querylist();break;
     case ft_form:objectlist=p_left->database()->formlist();break;
     case ft_report:objectlist=p_left->database()->reportlist();break;
     default: objectlist=p_left->database()->querylist();

   }


    vector<hk_string>::iterator it=objectlist->begin();
    while (it!=objectlist->end())
    {
        upload_file(*it,f);
	++it;
    }


    delete p_progressdialog;
    p_progressdialog=NULL;
   setCursor(crs);
}

void hk_kdeeximportdatabase::copy_all_views(void)
{
 if (!p_right->database()->connection()->server_supports(hk_connection::SUPPORTS_NEW_VIEW))
   {
     show_warningmessage(hk_translate("Target database does not support views!"));
     return;
   }
 if (!show_yesnodialog(hk_translate("Copy all views?"),true)) return;
  hk_database*   t=p_right->database();
  hk_database*   b=p_left->database();
    p_cancelcopying=false;
        QCursor crs=cursor();

    p_progressdialog= new  QProgressDialog();
    p_progressdialog->resize(300,p_progressdialog->height());
    connect(p_progressdialog,SIGNAL(cancelled()),this,SLOT(copying_cancelled()));
    p_progressdialog->show();
    qApp->processEvents();

    vector<hk_string>::iterator it=b->viewlist()->begin();
    bool cancel=false;
    while (it!=b->viewlist()->end()&&!cancel)
    {
        hk_datasource* fds=b->new_view((*it));
        cancel=!t->copy_view(fds,&hk_kdeeximportdatabase::set_progress);
        delete fds;
        it++;
    }


    delete p_progressdialog;
    p_progressdialog=NULL;
    setCursor(crs);

}

void hk_kdeeximportdatabase::copy_all_forms(void)
{
  copy_all_files(ft_form);

}

void hk_kdeeximportdatabase::copy_all_reports(void)
{
  copy_all_files(ft_report);

}


void hk_kdeeximportdatabase::leftnewbutton_clicked()
{
  if (!p_connection) return;
  QStringList l;
  QString xmime=QString::fromUtf8(l2u(p_connection->mimetype()).c_str());
  l.append(xmime);
  l.append("all/allfiles");
  QString p="kfiledialog:///"+QString::fromUtf8(l2u(p_connection->drivername()).c_str());
  QString filename;
  filename=QString::null;
 /*tbp 
  if (p_connection->server_needs(hk_connection::NEEDS_DIRECTORY_AS_DATABASE))
  {
    KUrl seldir;
    
    seldir=KFileDialog::getExistingDirectoryUrl(p,this,i18n("Select directory"));
    filename=seldir.directory(NULL);
    if (!filename.isNull())
      if (!seldir.fileName().isEmpty())
        filename+=seldir.fileName();
  }
  else
  {
    KFileDialog* d=new KFileDialog(KUrl(p),QString::null,this);
    d->setMimeFilter(l,xmime);
    d->exec();
    filename=d->selectedFile();
    delete d;
  }
*/
  
  if (!filename.isNull())
  {
     leftdatabasefield->insertItem(0,filename);
     leftdatabasefield->setCurrentIndex(0);
     slot_database_selected(0);
  }

}


void hk_kdeeximportdatabase::rightnewbutton_clicked()
{
    if (!p_connection->server_supports(hk_connection::SUPPORTS_NEW_DATABASE))
        return;
        hk_string r=hk_kdenewdatabase::show_dialog(p_connection->drivername(),p_connection->server_supports(hk_connection::SUPPORTS_LOCAL_FILEFORMAT));
	r=trim(r);
        if (r.size()>0)
        {
            if (!p_connection->create_database(r))
	    {
	      show_warningmessage(hk_translate("Database could not be created"));
	    }
	    else
	    dblist_changes();
        }

}
void hk_kdeeximportdatabase::accept(void)
{
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg =  c->group(p_mode==m_import?"Importdatabase":"Exportdatabase");
  cg.writeEntry("Geometry",geometry());

  hk_kdeeximportdatabasebase::accept();
}

void hk_kdeeximportdatabase::keyPressEvent ( QKeyEvent * e )
{
  if (e->key()==Qt::Key_F1) help_clicked();
}



void hk_kdeeximportdatabase::help_clicked()
{
  KHelpClient::invokeHelp("copy");
}

