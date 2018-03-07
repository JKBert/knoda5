// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4reportpart library
//
// This file is part of the hk_kde4reportpart library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.12 $

//***********************************************
//***  hk_kdereport PART definition             ***
//***********************************************
#include "hk_kdereportpart.h"
#include "hk_kdesimplereport.h"
#include <kaboutdata.h>
#include <kcomponentdata.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kaction.h>
#include <klocale.h>
#include <kurl.h>
#include <kparts/partmanager.h>

#include <qapplication.h>
#include <qclipboard.h>
#include <qcombobox.h>
#include <hk_drivermanager.h>
#include <hk_connection.h>
#include <hk_database.h>
#include <hk_datasource.h>

K_PLUGIN_FACTORY(hk_kdereportpartfactory,registerPlugin<hk_kdereportpart>();)
K_EXPORT_PLUGIN(hk_kdereportpartfactory("hk_kde4reportpart","hk_kde4reportpart"))

class hk_kdereportpartprivate
{
  public:
  hk_kdereportpartprivate()
  	{
		p_report=NULL;
		activate=false;
	}
   hk_kdereportpartwidget* p_report;
   bool activate;
};

hk_kdereportpart::hk_kdereportpart(QWidget* pWidget, QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent)
{
    p_private=new hk_kdereportpartprivate;
    setComponentData(hk_kdereportpartfactory::componentData());
    p_private->p_report = new hk_kdereportpartwidget(this,pWidget,0);
    p_private->p_report->setAttribute(Qt::WA_DeleteOnClose);
    // to include the part into other widget, it must not be flagged with Qt::Window, 
    // so we explicitly flag it with Qt::Widget
    p_private->p_report -> setWindowFlags(Qt::Widget);    
    setWidget(p_private->p_report);
    setXMLFile(KStandardDirs::locate("data","hk_kde4classes/hk_kdereportpartdesign.rc"));
}

hk_kdereportpart::~hk_kdereportpart()
{
  if (widget())
  {
    if (p_private->p_report->simplereport()->has_changed())
      p_private->p_report->simplereport()->save_report();
  }
  delete p_private;
}



/*void hk_kdereportpart::show_dbdesignercolumndialog(void)
{
    p_private->p_table->simpledbdesigner()->show_dbdesignercolumndialog();
}*/


void hk_kdereportpart::setReadWrite(bool rw)
{
    KParts::ReadWritePart::setReadWrite(rw);
}


bool hk_kdereportpart::openFile()
{
 // URL handling:   mysql:/user:password@host:port/databasename/datasourcetype/datasourcename
 // where datasourcetype is either tables or queries


    return true;
}


bool hk_kdereportpart::saveFile()
{
    return true;
}

void hk_kdereportpart::setXMLFile(const QString& file,bool merge,bool setxmldoc)
{
KParts::ReadWritePart::setXMLFile(file,merge,setxmldoc);
if (manager()&&manager()->activePart()==this &&! p_private->activate)
{
  p_private->activate=true;
  manager()->blockSignals(true);
  manager()->setActivePart(NULL);
  manager()->blockSignals(false);
  manager()->setActivePart(this);

  p_private->activate=false;
} 
}


KAboutData* hk_kdereportpart::createAboutData()
{
    KAboutData* a= new KAboutData("hk_kde4reportpart", "hk_kde4reportpart", ki18n("hk_kde4reportpart"),
        "0.2", ki18n("database report editor"),
        KAboutData::License_GPL,
        ki18n("(c) 2002-2004, Horst Knorr\n(c) 2010-2016 Patrik Hanak"), ki18n(NULL), "http://sourceforge.net/projects/knoda4/",
     "knoda4-bugs@lists.sourceforge.net");
    a -> addAuthor(ki18n("Horst Knorr"),ki18n("Author of original version"), "hk_classes@knoda.org","http://www.knoda.org");
    a -> addAuthor(ki18n("Patrik Hanak"),ki18n("Author of KDE4 port"), "knoda4-admins@lists.sourceforge.net");    

    return a;

}






