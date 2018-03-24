// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4gridpart library
//
// This file is part of the hk_kde4gridpart library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.11 $

//***********************************************
//***  hk_kdegrid PART definition             ***
//***********************************************
#include "hk_kdegridpart.h"
#include "hk_kdesimpleform.h"
#include "hk_kde4simplegrid.h"
#include <kcomponentdata.h>
#include <kaboutdata.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kaction.h>
#include <klocale.h>
#include <kurl.h>
#include <kactioncollection.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <hk_drivermanager.h>
#include <hk_connection.h>
#include <hk_database.h>
#include <hk_datasource.h>
#include <hk_class.h>

K_PLUGIN_FACTORY(hk_kdegridpartfactory, registerPlugin<hk_kdegridpart>();)
K_EXPORT_PLUGIN(hk_kdegridpartfactory("hk_kde5gridpart", "hk_kde5gridpart"))

hk_kdegridpart::hk_kdegridpart(QWidget* pWidget,QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent)
{
  setObjectName("hk_kdegridpart");  
  setComponentData(hk_kdegridpartfactory::componentData());
  hk_kdesimpleform* form=dynamic_cast<hk_kdesimpleform*>(pWidget);
    p_grid = new hk_kdegrid(pWidget,"hk_kdegridpart",0,form);   
    p_grid->setpart(this);
    setWidget(p_grid);
    KIconLoader* loader=KIconLoader::global();
    loader->addAppDir("hk_kde5classes");
    p_columndialogaction = new KAction(KIcon("grid22x22",loader),i18n("&Gridcolumns"),actionCollection());
    actionCollection() -> addAction("gridcolumn",p_columndialogaction);
    connect(p_columndialogaction,SIGNAL(triggered()),this,SLOT(show_gridcolumndialog()));
    p_columndialogaction->setEnabled(!hk_class::runtime_only());
    
    p_copyaction = new KAction(KIcon("edit-copy"),i18n("&Copy"),actionCollection());
    p_copyaction -> setShortcut(Qt::CTRL+Qt::Key_C);
    actionCollection() -> addAction("copy",p_copyaction);
    connect(p_copyaction,SIGNAL(triggered()),p_grid->simplegrid(),SLOT(copy()));
    
    p_pasteaction = new KAction(KIcon("edit-paste"),i18n("&Paste"),actionCollection());
    p_pasteaction -> setShortcut(Qt::CTRL+Qt::Key_V);
    actionCollection() -> addAction("paste",p_pasteaction);
    connect(p_pasteaction,SIGNAL(triggered()),p_grid->simplegrid(),SLOT(paste()));
    
    p_findaction = new KAction(KIcon("find",loader),i18n("&Find in columns"),actionCollection());
    actionCollection() -> addAction("findcolumn",p_findaction);
    connect(p_findaction,SIGNAL(triggered()),p_grid,SLOT(find_clicked()));     
    setXMLFile(KStandardDirs::locate("data","hk_kde5classes/hk_kdegridpart.rc"));
    p_drivermanager=NULL;
    connect (QApplication::clipboard(), SIGNAL(dataChanged()),this, SLOT(clipboarddata_has_changed()));
    clipboarddata_has_changed();
}

hk_kdegridpart::~hk_kdegridpart()
{
  if (p_drivermanager) delete p_drivermanager;

}



void hk_kdegridpart::show_gridcolumndialog(void)
{
  p_grid->show_gridcolumndialog();
}


void hk_kdegridpart::setReadWrite(bool rw)
{
    KParts::ReadWritePart::setReadWrite(rw);
}


bool hk_kdegridpart::openFile()
{
 // URL handling:   mysql:/user:password@host:port/databasename/datasourcetype/datasourcename
 // where datasourcetype is either tables or queries
    KUrl url1 = url();
    if (url1.protocol()==QString::null) return false;
    if (!url1.hasPath()) return false;
    QString path=url1.path();
    QString databasename=path.section('/',0,0);
    if (databasename.length()<1) return false;
    QString dstype=path.section('/',1,1);
    if (dstype!="tables"&&dstype!="queries"&&dstype!="views") return false;
    datasourcetype dt=dt_table;
    if (dstype=="queries") dt=dt_query;
    else
    if (dstype=="views") dt=dt_view;
    QString dsname=path.section('/',2,2);
    if (dsname.length()<1) return false;

    
    if (!p_drivermanager) p_drivermanager=new hk_drivermanager;
    hk_connection* con=p_drivermanager->new_connection(u2l(url1.protocol().toUtf8().data()));
    if (!con) return false;
    if (url1.hasHost()) con->set_host(u2l(url1.host().toUtf8().data()));
    if (url1.port()!=0) con->set_tcp_port(url1.port());
    if (url1.hasUser()) con->set_user(u2l(url1.user().toUtf8().data()));
    if (url1.hasPass()) con->set_password(u2l(url1.pass().toUtf8().data()));
    if (!con->connect())
    {
      delete con;
      return false;
    }  
    hk_database* db= con->new_database(u2l(databasename.toUtf8().data()));
    hk_datasource* ds=db->load_datasource(u2l(dsname.toUtf8().data()),dt);
    p_grid->set_datasource(ds);
    if (ds) ds->enable();
    return true;
}


bool hk_kdegridpart::saveFile()
{
    return true;
}


KAboutData* hk_kdegridpart::createAboutData()
{
    KAboutData* a= new KAboutData("hk_kde5gridpart", "hk_kde5gridpart",ki18n("hk_kde5gridpart"),
        "0.2", ki18n("Datasource editor"),
        KAboutData::License_GPL,
        ki18n("(c) 2002-2003, Horst Knorr\n(c) 2010-2018 Patrik Hanak"),ki18n(NULL), "http://sourceforge.net/projects/knoda5/",
     "knoda4-bugs@lists.sourceforge.net");
    a -> addAuthor(ki18n("Horst Knorr"),ki18n("Author of original version"), "hk_classes@knoda.org","http://www.knoda.org");
    a -> addAuthor(ki18n("Patrik Hanak"),ki18n("Author of KDE5 port"), "knoda4-admins@lists.sourceforge.net");

    return a;

}

void  hk_kdegridpart::clipboarddata_has_changed(void)
{
  p_pasteaction->setEnabled(true);
}


