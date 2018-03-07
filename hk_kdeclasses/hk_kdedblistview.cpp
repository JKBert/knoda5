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
#include "hk_kdedblistview.h"
#include <klocale.h>
#include <kapplication.h>
#include <kstandarddirs.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <kconfig.h>
#include <qprogressdialog.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qbuttongroup.h>
#include <QGroupBox>
#include <qlabel.h>
#include <qclipboard.h>
#include <hk_database.h>
#include <hk_connection.h>
#include <hk_drivermanager.h>
#include <kglobal.h>
#include <kicon.h>
#include <kconfiggroup.h>
#include <QDropEvent>


bool hk_kdedblistview::p_cancelcopying=false;
QProgressDialog* hk_kdedblistview::p_progressdialog=NULL;

hk_kdeschemadialog::hk_kdeschemadialog( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name = NULL?"hk_kdeschemadialog":name));
    setModal(modal);
    resize( 253, 100 );
    setWindowTitle( i18n( "Please select" ) );
    setSizeGripEnabled( TRUE );
    hk_kdeschemadialogLayout = new QHBoxLayout(this);
    hk_kdeschemadialogLayout->setMargin(11);
    hk_kdeschemadialogLayout->setSpacing(6);
    hk_kdeschemadialogLayout->setObjectName("hk_kdeschemadialogLayout");

    ButtonGroup1 = new QGroupBox( this );
    ButtonGroup1Layout = new QVBoxLayout();
    ButtonGroup1Layout->setSpacing( 6 );
    ButtonGroup1Layout->setMargin( 11 );  
    ButtonGroup1Layout->setAlignment( Qt::AlignTop );

    schemaonlyfield = new QRadioButton(ButtonGroup1);
    schemaonlyfield->setObjectName(QString::fromAscii("schemaonlyfield"));
    schemaonlyfield->setChecked( TRUE );
    ButtonGroup1Layout->addWidget( schemaonlyfield );
    datafield = new QRadioButton( ButtonGroup1);
    datafield->setObjectName(QString::fromAscii("datafield" ));
    ButtonGroup1Layout->addWidget( datafield );
    
    ButtonGroup1->setLayout(ButtonGroup1Layout);
    hk_kdeschemadialogLayout->addWidget( ButtonGroup1 );

    Layout2 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout2->setMargin(0);
    Layout2->setSpacing(6);
    Layout2->setObjectName("Layout2");

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName(QString::fromAscii("buttonOk"));
    buttonOk->setText( i18n( "&OK" ) );
    buttonOk->setShortcut( 0 );
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    Layout2->addWidget( buttonOk );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName(QString::fromAscii("buttonCancel"));
    buttonCancel->setText( i18n( "&Cancel" ) );
    buttonCancel->setShortcut( 0 );
    buttonCancel->setAutoDefault( TRUE );
    Layout2->addWidget( buttonCancel );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout2->addItem( spacer );
    hk_kdeschemadialogLayout->addLayout( Layout2 );

// signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}


hk_kdeschemadialog::~hk_kdeschemadialog()
{
}



bool hk_kdedblistview::p_use_singleclick=false;//if false double click starts an item


class hk_kdedblistviewprivate
{
public:
hk_kdedblistviewprivate()
	{
	p_databaseitem=NULL;
	p_tableitem=NULL;
	p_queryitem=NULL;
	p_viewitem=NULL;
	p_formitem=NULL;
	p_reportitem=NULL;
	p_moduleitem=NULL;
	p_schemadialog=NULL;
	p_dragging=false;
	p_showmode=hk_kdedblistview::sm_all;
	}
QTreeWidgetItem* p_databaseitem;
QTreeWidgetItem* p_tableitem;
QTreeWidgetItem* p_viewitem;
QTreeWidgetItem* p_queryitem;
QTreeWidgetItem* p_formitem;
QTreeWidgetItem* p_reportitem;
QTreeWidgetItem* p_moduleitem;
hk_kdeschemadialog* p_schemadialog;
hk_kdedblistview::enum_showmode p_showmode;
void clearList(QTreeWidgetItem*);
QMimeData* create_draginfo(objecttype type,const QString& object,hk_database* db);
bool p_dragging;
objecttype otype;
QPoint p_dragstartposition;
                                                  //decoded drag&dropvalues
hk_string xname,xtype,xdirectory,xserver,xhost,xtcp,xuser,xdbname;
};


void hk_kdedblistviewprivate::clearList(QTreeWidgetItem* item)
{
  QTreeWidgetItem * child = item->child(0);
  
  while( child )
  {
    item -> removeChild(child);
    delete child;
    child=item->child(0);
  } 
}

QMimeData* hk_kdedblistviewprivate::create_draginfo(objecttype type,const QString& object,hk_database* db)
{
    hk_string mystring="<NAME>";
    QMimeData* pres = new QMimeData();
    
    mystring+=u2l(object.toUtf8().data());
    mystring+="</NAME>\n<TYPE>";
    switch (type)
    {
      case ot_database:mystring+="DATABASE";break;
      case ot_table:mystring+="TABLE";break;
      case ot_query:mystring+="QUERY";break;
      case ot_view:mystring+="VIEW";break;
      case ot_form:mystring+="FORM";break;
      case ot_report:mystring+="REPORT";break;
      case ot_module:mystring+="MODULE";break;
    }
    mystring+="</TYPE>\n<DIRECTORY>";
    mystring+=db->database_path()+"/</DIRECTORY>\n<SERVER>";
    mystring+=db->connection()->drivername()+"</SERVER>\n<HOST>";
    mystring+=db->connection()->host()+"</HOST>\n<TCP>";
    mystring+=longint2string(db->connection()->tcp_port())+"</TCP>\n<USER>";
    mystring+=db->connection()->user()+"</USER>\n<DBNAME>";
    mystring+=db->name()+"</DBNAME>";
    mystring="<DRAG>"+mystring+"</DRAG>";
    pres -> setData("application/x-hk_filecopy",mystring.c_str());
    return pres;
}


hk_kdedblistview::hk_kdedblistview (QWidget* parent, const char* /*name*/):
QTreeWidget(parent), hk_dbvisible()
{
  p_private=new hk_kdedblistviewprivate;
  p_private->p_schemadialog= new hk_kdeschemadialog();
  p_private->p_schemadialog->schemaonlyfield->setText(i18n("&Schema only"));
  p_private->p_schemadialog->datafield->setText(i18n("Schema and &data"));

  setContextMenuPolicy(Qt::CustomContextMenu);
  setColumnCount(1);
  headerItem() -> setText(0,i18n("Active database:"));
  setRootIsDecorated(true);
  init_listview();
  connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(item_clicked(QTreeWidgetItem*)));
  connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(item_doubleclicked(QTreeWidgetItem*)));
  connect(this, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_customcontextmenu(QPoint)));
  setAcceptDrops(true);
  setDragEnabled(true);
  setDropIndicatorShown(true);
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Preferences");
  p_use_singleclick=cg.readEntry("UseSingleclick",false);

}




hk_kdedblistview::~hk_kdedblistview()
{
 delete p_private->p_schemadialog;
 delete p_private;

}


void hk_kdedblistview::init_listview()
{
  KIconLoader* loader=KIconLoader::global();
  loader->addAppDir("hk_kde4classes");
  clear();
  setSortingEnabled(false);
  p_private->p_databaseitem=new QTreeWidgetItem(this,QStringList() << i18n("<No database>"));
  p_private->p_databaseitem->setIcon(0,loader->loadIcon("document-open-folder",KIconLoader::NoGroup,KIconLoader::SizeSmall));
  
  p_private->p_moduleitem=new QTreeWidgetItem(QStringList() << i18n("Modules"));
  p_private->p_moduleitem->setIcon(0,loader->loadIcon("document-edit",KIconLoader::NoGroup,KIconLoader::SizeSmall));
  p_private->p_databaseitem -> insertChild(0,p_private->p_moduleitem);
  
  p_private->p_reportitem=new QTreeWidgetItem(QStringList() << i18n("Reports"));
  p_private->p_reportitem->setIcon(0,loader->loadIcon("printer",KIconLoader::NoGroup,KIconLoader::SizeSmall));
  p_private->p_databaseitem -> insertChild(0,p_private->p_reportitem);

  p_private->p_formitem=new QTreeWidgetItem(QStringList() << i18n("Forms"));
  p_private->p_formitem->setIcon(0,loader->loadIcon("utilities-terminal",KIconLoader::NoGroup,KIconLoader::SizeSmall));
  p_private->p_databaseitem -> insertChild(0,p_private->p_formitem);  

  if (database() && database()->connection()->server_supports(hk_connection::SUPPORTS_SQL))
   {
    p_private->p_queryitem=new QTreeWidgetItem(QStringList() << i18n("Queries"));
    p_private->p_queryitem->setIcon(0,loader->loadIcon("document-edit-verify",KIconLoader::NoGroup,KIconLoader::SizeSmall));
    p_private->p_databaseitem -> insertChild(0,p_private->p_queryitem);
   }
   else
   p_private->p_queryitem=NULL;
  if (p_private->p_showmode==hk_kdedblistview::sm_all)
  {
  if (database() && database()->connection()->server_supports(hk_connection::SUPPORTS_VIEWS))
   {
     p_private->p_viewitem=new QTreeWidgetItem(QStringList() << i18n("Views"));
     p_private->p_viewitem->setIcon(0,loader->loadIcon("document-preview",KIconLoader::NoGroup,KIconLoader::SizeSmall));
       p_private->p_databaseitem -> insertChild(0,p_private->p_viewitem);
   }
   else
   {
   p_private->p_viewitem=NULL;
   }
  p_private->p_tableitem=new QTreeWidgetItem(QStringList() << i18n("Tables"));
  p_private->p_tableitem->setIcon(0,loader->loadIcon("view-form-table",KIconLoader::NoGroup,KIconLoader::SizeSmall));
  p_private->p_databaseitem -> insertChild(0,p_private->p_tableitem);

  }
  else p_private->p_tableitem=NULL;
  p_private->p_databaseitem->setExpanded(true);
}




objecttype hk_kdedblistview::current_objecttype() const
{
   if (is_databaseitem(currentItem())) return ot_database;
    else
    if (is_tableitem(currentItem())) return ot_table;
        else
        if (is_viewitem(currentItem())) return ot_view;
        else
        if (is_queryitem(currentItem())) return ot_query;
            else
            if (is_formitem(currentItem())) return ot_form;
            else
            if (is_moduleitem(currentItem())) return ot_module;
                else return ot_report;

}



void hk_kdedblistview::set_database(hk_database* db)
{
  hk_dbvisible::set_database(db);
  if (db) init_listview();

  set_databasename();



}




void hk_kdedblistview::set_databasename(void)
{
if (p_database && p_database->name().size()>0)
    p_private->p_databaseitem->setText(0,QString::fromUtf8(l2u(p_database->name()).c_str()));
    else
    p_private->p_databaseitem->setText(0,i18n("<No database>"));
set_tables();
set_views();
set_queries();
set_forms();
set_reports();
set_modules();
}

void hk_kdedblistview::set_tables(void)
{

  if (!p_database||p_private->p_showmode!=hk_kdedblistview::sm_all) return;
  p_private->clearList(p_private->p_tableitem);
  vector<hk_string>* tb=p_database->tablelist();
  vector<hk_string>::reverse_iterator it;
  if (tb!=NULL)
  {
      for(it=tb->rbegin();it!=tb->rend();it++)
       new QTreeWidgetItem(p_private->p_tableitem,QStringList() << QString::fromUtf8(l2u((*it)).c_str()));
  }
  p_private->p_tableitem->sortChildren(0,Qt::AscendingOrder);
}


void hk_kdedblistview::set_views(void)
{

  if (!p_database||p_private->p_showmode!=hk_kdedblistview::sm_all) return;
  if (!p_private->p_viewitem) return;
  p_private->clearList(p_private->p_viewitem);
  vector<hk_string>* tb=p_database->viewlist();
  vector<hk_string>::reverse_iterator it;
  if (tb!=NULL)
  {
      for(it=tb->rbegin();it!=tb->rend();it++)
       new QTreeWidgetItem(p_private->p_viewitem,QStringList() << QString::fromUtf8(l2u((*it)).c_str()));
  }
  p_private->p_viewitem->sortChildren(0,Qt::AscendingOrder);
}


void hk_kdedblistview::set_queries(void)
{
  if (!p_private->p_queryitem) return;
  QString text=i18n("Queries");
  if (p_database&&
      ((p_database->loadmode(ft_query)==hk_database::central && p_private->p_showmode==sm_all)
      ||(p_private->p_showmode==sm_central)
      )
     )
     text+=i18n(" (Central)");
  p_private->p_queryitem->setText(0,text);

  p_private->clearList(p_private->p_queryitem);
  if (!p_database) return;
  vector<hk_string>* tb;
  switch (p_private->p_showmode)
  {
    case hk_kdedblistview::sm_central: tb=p_database->central_filelist(ft_query);break;
    case hk_kdedblistview::sm_local: tb=p_database->local_filelist(ft_query);break;
    default: tb=p_database->querylist();
  }


  vector<hk_string>::reverse_iterator it;
  if (tb!=NULL)
  {
      for(it=tb->rbegin();it!=tb->rend();it++)
       new QTreeWidgetItem(p_private->p_queryitem,QStringList() << QString::fromUtf8(l2u((*it)).c_str()));
  }
  p_private->p_queryitem->sortChildren(0,Qt::AscendingOrder);
}




void hk_kdedblistview::set_forms(void)
{
  QString text=i18n("Forms");
  if (p_database&&
      ((p_database->loadmode(ft_form)==hk_database::central && p_private->p_showmode==sm_all)
      ||(p_private->p_showmode==sm_central)
      )
     )
     text+=i18n(" (Central)");
  p_private->p_formitem->setText(0,text);

 p_private->clearList(p_private->p_formitem);
  if (!p_database) return;
  vector<hk_string>* tb;
  switch (p_private->p_showmode)
  {
    case hk_kdedblistview::sm_central: tb=p_database->central_filelist(ft_form);break;
    case hk_kdedblistview::sm_local: tb=p_database->local_filelist(ft_form);break;
    default: tb=p_database->formlist();
  }
  vector<hk_string>::reverse_iterator it;
  if (tb!=NULL)
  {
      it=tb->rbegin();
      while(it!=tb->rend())
       {
	new QTreeWidgetItem(p_private->p_formitem,QStringList() << QString::fromUtf8(l2u((*it)).c_str()));
        it++;
       }
  }
  if (p_private->p_formitem)p_private->p_formitem->sortChildren(0,Qt::AscendingOrder);
}



void hk_kdedblistview::set_reports(void)
{
  QString text=i18n("Reports");
  if (p_database&&
      ((p_database->loadmode(ft_report)==hk_database::central && p_private->p_showmode==sm_all)
      ||(p_private->p_showmode==sm_central)
      )
     )
     text+=i18n(" (Central)");
  p_private->p_reportitem->setText(0,text);

 p_private->clearList(p_private->p_reportitem);
  if (!p_database) return;
  vector<hk_string>* tb;
  switch (p_private->p_showmode)
  {
    case hk_kdedblistview::sm_central: tb=p_database->central_filelist(ft_report);break;
    case hk_kdedblistview::sm_local: tb=p_database->local_filelist(ft_report);break;
    default: tb=p_database->reportlist();
  }
  vector<hk_string>::reverse_iterator it;
  if (tb!=NULL)
  {
      for(it=tb->rbegin();it!=tb->rend();it++)
       new QTreeWidgetItem(p_private->p_reportitem,QStringList() << QString::fromUtf8(l2u((*it)).c_str()));
  }
  if (p_private->p_reportitem)p_private->p_reportitem->sortChildren(0,Qt::AscendingOrder);
}



void hk_kdedblistview::set_modules(void)
{
  QString text=i18n("Modules");
  if (p_database&&
      ((p_database->loadmode(ft_module)==hk_database::central && p_private->p_showmode==sm_all)
      ||(p_private->p_showmode==sm_central)
      )
     )
     text+=i18n(" (Central)");
  p_private->p_moduleitem->setText(0,text);

 p_private->clearList(p_private->p_moduleitem);
  if (!p_database) return;
  vector<hk_string>* tb;
  switch (p_private->p_showmode)
  {
    case hk_kdedblistview::sm_central: tb=p_database->central_filelist(ft_module);break;
    case hk_kdedblistview::sm_local: tb=p_database->local_filelist(ft_module);break;
    default: tb=p_database->modulelist();
  }
  vector<hk_string>::reverse_iterator it;
  if (tb!=NULL)
  {
      for(it=tb->rbegin();it!=tb->rend();it++)
       new QTreeWidgetItem(p_private->p_moduleitem, QStringList() << QString::fromUtf8(l2u((*it)).c_str()));
  }
  if (p_private->p_moduleitem)p_private->p_moduleitem->sortChildren(0,Qt::AscendingOrder);
}





void hk_kdedblistview::item_clicked(QTreeWidgetItem* item)
{
  if (p_use_singleclick) item_selected(item);

}

void hk_kdedblistview::item_doubleclicked(QTreeWidgetItem* item)
{
  if (!p_use_singleclick) item_selected(item);
}

void hk_kdedblistview::set_use_singleclick(bool u)
{
  p_use_singleclick=u;
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Preferences");
  cg.writeEntry("UseSingleclick",p_use_singleclick);
}

bool hk_kdedblistview::use_singleclick(void)
{
  return p_use_singleclick;
}




void hk_kdedblistview::item_selected(QTreeWidgetItem* item)
{
  if (!item) return;
  if (item->parent()==p_private->p_tableitem)
      emit signal_viewmode_table(item->text(0));
  else
  if (p_private->p_queryitem && item->parent()==p_private->p_queryitem)
      emit signal_viewmode_query(item->text(0));
  else
  if (p_private->p_viewitem && item->parent()==p_private->p_viewitem)
      emit signal_viewmode_view(item->text(0));
else
  if (item->parent()==p_private->p_formitem)
      emit signal_viewmode_form(item->text(0));
else
  if (item->parent()==p_private->p_reportitem)
      emit signal_viewmode_report(item->text(0));
else
  if (item->parent()==p_private->p_moduleitem)
      emit signal_designmode_module(item->text(0));
else
  if (item==p_private->p_tableitem&& !runtime_only() &&p_database&& p_database->name().size()>0)
      emit signal_new_table();
  else
  if (item==p_private->p_queryitem&& !runtime_only()&&p_database&& p_database->name().size()>0)
      emit signal_new_query();
else
  if (item==p_private->p_viewitem&& !runtime_only()&&p_database&& p_database->name().size()>0)
      emit signal_new_view();
else
  if (item==p_private->p_formitem&& !runtime_only()&&p_database&& p_database->name().size()>0)
      emit signal_new_form();
else
  if (item==p_private->p_reportitem&& !runtime_only()&&p_database&& p_database->name().size()>0)
      emit signal_new_report();
else
  if (item==p_private->p_moduleitem&& !runtime_only()&&p_database&& p_database->name().size()>0)
      emit signal_new_module();
else
  if (item==p_private->p_databaseitem&& !runtime_only()&&p_database)
      emit signal_new_database();

}





bool hk_kdedblistview::is_tableitem(QTreeWidgetItem*item,bool includingheader) const
{
  if (!item) return false;
  if (includingheader && item==p_private->p_tableitem) return true;
  if (item->parent()==p_private->p_tableitem) return true;
  return false;
}

bool hk_kdedblistview::is_queryitem(QTreeWidgetItem*item,bool includingheader) const
{
  if (!item||!p_private->p_queryitem) return false;
  if (includingheader && item==p_private->p_queryitem) return true;
  if (item->parent()==p_private->p_queryitem) return true;
  return false;
}

bool hk_kdedblistview::is_viewitem(QTreeWidgetItem*item,bool includingheader) const
{
  if (!item || !p_private->p_viewitem) return false;
  if (includingheader && item==p_private->p_viewitem) return true;
  if (item->parent()==p_private->p_viewitem) return true;
  return false;
}

bool hk_kdedblistview::is_formitem(QTreeWidgetItem*item,bool includingheader) const
{
  if (!item) return false;
  if (includingheader && item==p_private->p_formitem) return true;
  if (item->parent()==p_private->p_formitem) return true;
  return false;
}

bool hk_kdedblistview::is_reportitem(QTreeWidgetItem*item,bool includingheader) const
{
  if (!item) return false;
  if (includingheader && item==p_private->p_reportitem) return true;
  if (item->parent()==p_private->p_reportitem) return true;
  return false;
}

bool hk_kdedblistview::is_moduleitem(QTreeWidgetItem*item,bool includingheader) const
{
  if (!item) return false;
  if (includingheader && item==p_private->p_moduleitem) return true;
  if (item->parent()==p_private->p_moduleitem) return true;
  return false;
}

bool hk_kdedblistview::is_databaseitem(QTreeWidgetItem* item) const
{
  if (!item) return false;
  return (item==p_private->p_databaseitem) ;
}

bool hk_kdedblistview::is_headeritem(QTreeWidgetItem* item) const
{
  if (!item) return false;
  return (item==p_private->p_databaseitem ||
          item==p_private->p_tableitem ||
	  item==p_private->p_queryitem ||
	  item==p_private->p_viewitem ||
	  item==p_private->p_formitem ||
	  item==p_private->p_moduleitem ||
	  item==p_private->p_reportitem
	 );
}

bool hk_kdedblistview::is_tableheader(QTreeWidgetItem* item) const
{
   return item==p_private->p_tableitem;
}

bool hk_kdedblistview::is_queryheader(QTreeWidgetItem* item) const
{
   return item==p_private->p_queryitem;
}

bool hk_kdedblistview::is_viewheader(QTreeWidgetItem* item) const
{
   return item==p_private->p_viewitem;
}

bool hk_kdedblistview::is_formheader(QTreeWidgetItem* item) const
{
   return item==p_private->p_formitem;
}

bool hk_kdedblistview::is_reportheader(QTreeWidgetItem* item) const
{
   return item==p_private->p_reportitem;
}

bool hk_kdedblistview::is_moduleheader(QTreeWidgetItem* item) const
{
   return item==p_private->p_moduleitem;
}






bool hk_kdedblistview::is_tableheader(void) const
{
   return is_tableheader(currentItem());
}

bool hk_kdedblistview::is_queryheader(void) const
{
   return is_queryheader(currentItem());
}

bool hk_kdedblistview::is_viewheader(void) const
{
   return is_viewheader(currentItem());
}

bool hk_kdedblistview::is_formheader(void) const
{
   return is_formheader(currentItem());
}

bool hk_kdedblistview::is_reportheader(void) const
{
   return is_reportheader(currentItem());
}








bool hk_kdedblistview::is_tableitem(bool includeheader) const
{
return is_tableitem(currentItem(),includeheader);
}

bool hk_kdedblistview::is_queryitem(bool includeheader) const
{
return is_queryitem(currentItem(),includeheader);

}

bool hk_kdedblistview::is_viewitem(bool includeheader) const
{
return is_viewitem(currentItem(),includeheader);

}

bool hk_kdedblistview::is_formitem(bool includeheader) const
{
return is_formitem(currentItem(),includeheader);

}

bool hk_kdedblistview::is_reportitem(bool includeheader) const
{
return is_reportitem(currentItem(),includeheader);

}

bool hk_kdedblistview::is_moduleitem(bool includeheader) const
{
return is_moduleitem(currentItem(),includeheader);

}

bool hk_kdedblistview::is_databaseitem(void) const
{
return is_databaseitem(currentItem());

}

bool hk_kdedblistview::is_headeritem(void) const
{
return is_headeritem(currentItem());

}






void hk_kdedblistview::new_clicked()
{
    if (is_tableitem(currentItem(),true)) emit signal_new_table();
    else
    if (is_queryitem(currentItem(),true)) emit signal_new_query();
    else
    if (is_formitem(currentItem(),true)) emit signal_new_form();
    else
    if (is_reportitem(currentItem(),true)) emit signal_new_report();
    else
    if (is_moduleitem(currentItem(),true)) emit signal_new_module();
    else
    if (is_databaseitem(currentItem())) emit signal_new_database();
    else
    if (is_viewitem(currentItem(),true)) emit signal_new_view();


}



void hk_kdedblistview::alter_clicked()
{
    if (is_tableitem(currentItem())) emit signal_designmode_table(currentItem()->text(0));
    else
    if (is_queryitem(currentItem())) emit signal_designmode_query(currentItem()->text(0));
    else
    if (is_viewitem(currentItem())) emit signal_designmode_view(currentItem()->text(0));
    else
    if (is_formitem(currentItem())) emit signal_designmode_form(currentItem()->text(0));
    else
    if (is_reportitem(currentItem())) emit signal_designmode_report(currentItem()->text(0));
    else
    if (is_moduleitem(currentItem())) emit signal_designmode_module(currentItem()->text(0));
}

void hk_kdedblistview::delete_clicked()
{
    if (is_tableitem(currentItem(),true)) emit signal_delete_table(currentItem()->text(0));
    else
    if (is_queryitem(currentItem(),true)) emit signal_delete_query(currentItem()->text(0));
    else
    if (is_viewitem(currentItem(),true)) emit signal_delete_view(currentItem()->text(0));
    else
    if (is_formitem(currentItem(),true)) emit signal_delete_form(currentItem()->text(0));
    else
    if (is_reportitem(currentItem(),true)) emit signal_delete_report(currentItem()->text(0));
    else
    if (is_moduleitem(currentItem())) emit signal_delete_module(currentItem()->text(0));
    else
    if (is_databaseitem(currentItem())) emit signal_delete_database(currentItem()->text(0));


}


void hk_kdedblistview::start_clicked()
{
    if (is_tableitem(currentItem())) emit signal_viewmode_table(currentItem()->text(0));
    else
    if (is_queryitem(currentItem())) emit signal_viewmode_query(currentItem()->text(0));
    else
    if (is_viewitem(currentItem())) emit signal_viewmode_view(currentItem()->text(0));
    else
    if (is_formitem(currentItem())) emit signal_viewmode_form(currentItem()->text(0));
    else
    if (is_reportitem(currentItem())) emit signal_viewmode_report(currentItem()->text(0));
    else
    if (is_moduleitem(currentItem())) emit signal_designmode_module(currentItem()->text(0));

}

void    hk_kdedblistview::list_changes(listtype type)
{
   switch (type)
    {
        case lt_view : set_views();
        break;
        case lt_query : set_queries();
        break;
        case lt_form : set_forms();
        break;
        case lt_report : set_reports();
        break;
        case lt_module : set_modules();
        break;
        default : set_tables();

    }

}

bool hk_kdedblistview::canAccept(const QMimeData* mimedata)
{
  return mimedata -> hasFormat("application/x-hk_filecopy");
}

QMimeData* hk_kdedblistview::mimeData(const QList<QTreeWidgetItem*> items) const
{
  if(items.size() > 0) {
    return p_private -> create_draginfo(current_objecttype(),currentItem()->text(0),database());
  }else
    return new QMimeData();
}

bool hk_kdedblistview::dropMimeData(QTreeWidgetItem* /*parent*/, int /*index*/, const QMimeData* data, Qt::DropAction /*action*/)
{
   if(decode_action(data)) 
     execute_action();
   return true;
}

QStringList hk_kdedblistview::mimeTypes() const
{
  QString res("application/x-hk_filecopy");
  return QStringList(res);
}

Qt::DropActions hk_kdedblistview::supportedDropActions() const
{
  return Qt::CopyAction;
}

void hk_kdedblistview::slot_customcontextmenu(const QPoint& pnt)
{
  QTreeWidgetItem* itm = itemAt(pnt);
  QPoint gPos = viewport() -> mapToGlobal(pnt);
  
  emit signal_contextmenu_requested(itm,gPos);
}


bool hk_kdedblistview::decode_action(const QMimeData* event)
{
    if (!event) return false;
    if (!event->hasFormat("application/x-hk_filecopy"))return false;
    hk_string eventtxt=event->data("application/x-hk_filecopy").data();
   xmlDocPtr doc=xmlParseMemory(eventtxt.c_str(),eventtxt.size());
   xmlNodePtr node=xmlDocGetRootElement(doc);

    if (!(
        hk_class::get_tagvalue(node,"NAME",p_private->xname)
        &&hk_class::get_tagvalue(node,"TYPE",p_private->xtype)
        &&hk_class::get_tagvalue(node,"DIRECTORY",p_private->xdirectory)
        &&hk_class::get_tagvalue(node,"SERVER",p_private->xserver)
        &&hk_class::get_tagvalue(node,"HOST",p_private->xhost)
        &&hk_class::get_tagvalue(node,"TCP",p_private->xtcp)
        &&hk_class::get_tagvalue(node,"USER",p_private->xuser)
        &&hk_class::get_tagvalue(node,"DBNAME",p_private->xdbname)
        )
        )
    {
        show_warningmessage("Error in drag&drop protocol");
        return false;
    }
    if (p_private->xtype=="REPORT")
    {
        p_private->otype=ot_report;
    }
    else
    if (p_private->xtype=="MODULE")
    {
        p_private->otype=ot_module;
    }
    else
    if (p_private->xtype=="VIEW")
    {
        p_private->otype=ot_view;
    }
    else
    if (p_private->xtype=="QUERY")
    {
        p_private->otype=ot_query;
    }
    else
    if (p_private->xtype=="FORM")
    {
        p_private->otype=ot_form;
    }
    else
    if (p_private->xtype=="TABLE")
    {
        p_private->otype=ot_table;
    }
    else
    {
        p_private->otype=ot_database;
    }

    return true;

}



bool hk_kdedblistview::execute_action(void)
{
    switch (p_private->otype)
    {
        case ot_table:
        {
            copy_table();
            break;
        }
        case ot_view:
        {
            copy_view();
            break;
        }
        case ot_database:
        {
            copy_database();
            break;
        }
        default: copy_presentationfile();
    }
    return true;

}

bool hk_kdedblistview::copy_presentationfile(void)
{
    if (!database()) return false;
    hk_connection* con=find_connection();
    hk_connection* newcon=database()->connection()->drivermanager()->new_connection(p_private->xserver);
    newcon->set_host(p_private->xhost);
    newcon->set_tcp_port(localestring2uint(p_private->xtcp));
    newcon->set_user(p_private->xuser);
    if (con) newcon->set_password(con->password());
    newcon->connect();
    if (!newcon->is_connected())
    {
        delete newcon;
        return false;
    }
    hk_database* db=newcon->new_database(p_private->xdbname);
    hk_string input=db->load(p_private->xname,(filetype)p_private->otype);

      database()->save(input,p_private->xname,(filetype)p_private->otype,true,true);

   return true;
}




bool hk_kdedblistview::copy_table(void)
{
    if (!database()) return false;
    hk_connection* con=find_connection();
    hk_connection* newcon=database()->connection()->drivermanager()->new_connection(p_private->xserver);
    newcon->set_host(p_private->xhost);
    newcon->set_tcp_port(localestring2uint(p_private->xtcp));
    newcon->set_user(p_private->xuser);
    if (con) newcon->set_password(con->password());
    newcon->connect();
    if (!newcon->is_connected())
    {
        delete newcon;
        return false;
    }
    hk_database* db=newcon->new_database(p_private->xdbname);
    hk_datasource* ds=NULL;
    if (db)ds= db->new_table(p_private->xname);
    bool data=true;
    int i=p_private->p_schemadialog->exec();

    if (p_private->p_schemadialog->schemaonlyfield->isChecked())data=false;
    p_private->p_schemadialog->hide();
    if (i==QDialog::Rejected)return false;
    p_cancelcopying=false;
    p_progressdialog= new  QProgressDialog(parentWidget());
    p_progressdialog->resize(300,p_progressdialog->height());
    connect(p_progressdialog,SIGNAL(canceled()),this,SLOT(copying_cancelled()));
    p_progressdialog->show();
    qApp->processEvents();
    bool result= database()->copy_table(ds,data,true,true,&hk_kdedblistview::set_progress);
    delete ds;
    delete p_progressdialog;
    p_progressdialog=NULL;
    newcon->disconnect();
    delete newcon;
    return result;
}


bool hk_kdedblistview::copy_view(void)
{
    if (!database()) return false;
    hk_connection* con=find_connection();
    hk_connection* newcon=database()->connection()->drivermanager()->new_connection(p_private->xserver);
    newcon->set_host(p_private->xhost);
    newcon->set_tcp_port(localestring2uint(p_private->xtcp));
    newcon->set_user(p_private->xuser);
    if (con) newcon->set_password(con->password());
    newcon->connect();
    if (!newcon->is_connected())
    {
        delete newcon;
        return false;
    }
    hk_database* db=newcon->new_database(p_private->xdbname);
    hk_datasource* ds=NULL;
    if (db)ds= db->new_view(p_private->xname);
    p_cancelcopying=false;
    p_progressdialog= new  QProgressDialog(parentWidget());
    p_progressdialog->resize(300,p_progressdialog->height());
    connect(p_progressdialog,SIGNAL(cancelled()),this,SLOT(copying_cancelled()));
    p_progressdialog->show();
    qApp->processEvents();
    bool result= database()->copy_view(ds,&hk_kdedblistview::set_progress);
    delete ds;
    delete p_progressdialog;
    p_progressdialog=NULL;
    newcon->disconnect();
    delete newcon;
    return result;
}


void hk_kdedblistview::copying_cancelled()
{
    p_cancelcopying=true;
}


bool hk_kdedblistview::set_progress(long int position,long int total,const hk_string&txt)
{
    if (p_progressdialog)
    {
        p_progressdialog->setWindowTitle(QString::fromUtf8(l2u(txt).c_str()));
        p_progressdialog->setMaximum(total);
        p_progressdialog->setValue(position);
        qApp->processEvents();
    }
    return p_cancelcopying;
}


bool hk_kdedblistview::copy_database(void)
{
    if (!database()) return false;
    hk_connection* con=find_connection();
    hk_connection* newcon=database()->connection()->drivermanager()->new_connection(p_private->xserver);
    newcon->set_host(p_private->xhost);
    newcon->set_tcp_port(localestring2uint(p_private->xtcp));
    newcon->set_user(p_private->xuser);
    if (con) newcon->set_password(con->password());
    newcon->connect();
    if (!newcon->is_connected())
    {
        delete newcon;
        return false;
    }
    hk_database* db=newcon->new_database(p_private->xdbname);
    bool data=true;
    int i=p_private->p_schemadialog->exec();
    if (p_private->p_schemadialog->schemaonlyfield->isChecked())data=false;
    p_private->p_schemadialog->hide();
    if (i==QDialog::Rejected)return false;
    p_cancelcopying=false;
    p_progressdialog= new  QProgressDialog(parentWidget());
    p_progressdialog->resize(300,p_progressdialog->height());
    connect(p_progressdialog,SIGNAL(cancelled()),this,SLOT(copying_cancelled()));
    p_progressdialog->show();
    qApp->processEvents();
    bool result= database()->connection()->copy_database(db,data,true,&hk_kdedblistview::set_progress);
    delete p_progressdialog;
    p_progressdialog=NULL;
    newcon->disconnect();
    delete newcon;
    emit signal_reload_databases();
    return result;
}


hk_connection* hk_kdedblistview::find_connection(void)
{
    return database()->connection()->drivermanager()->find_existing_connection(p_private->xserver,p_private->xhost,localestring2uint(p_private->xtcp),p_private->xuser);
}

void hk_kdedblistview::copy(void)
{
  QApplication::clipboard()->setMimeData(p_private->create_draginfo(current_objecttype(),currentItem()->text(0),database()));
}

void hk_kdedblistview::paste(void)
{
  if(decode_action(QApplication::clipboard()->mimeData(QClipboard::Clipboard))
    ) execute_action();

}
void hk_kdedblistview::reload_lists()
{
   list_changes(lt_table);
   list_changes(lt_query);
   list_changes(lt_form);
   list_changes(lt_report);
   list_changes(lt_module);

}


void hk_kdedblistview::set_showmode(enum_showmode sm)
{
  p_private->p_showmode=sm;
  setAcceptDrops(sm==sm_all);
  setDragEnabled(sm==sm_all);
  init_listview();
  set_databasename();
}

hk_kdedblistview::enum_showmode hk_kdedblistview::showmode(void) const
{
   return p_private->p_showmode;
}

