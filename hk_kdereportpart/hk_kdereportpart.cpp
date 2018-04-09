// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde5reportpart library
//
// This file is part of the hk_kde5reportpart library.
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

K_PLUGIN_FACTORY_DEFINITION(hk_kdereportpartfactory, registerPlugin<hk_kdereportpart>();)
 
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
    static KAboutData* p_aData;
    static KAboutData& getAboutData(); 
};

KAboutData* hk_kdereportpartprivate::p_aData = NULL; 

KAboutData& hk_kdereportpartprivate::getAboutData()
{
    if ( p_aData == NULL) {
        p_aData = new KAboutData("hk_kde5classes", ki18n("hk_kde5reportpart").toString(),
            "0.2", ki18n("database report editor").toString(),
            KAboutLicense::GPL,
            ki18n("(c) 2002-2004, Horst Knorr\n(c) 2010-2018 Patrik Hanak").toString(),QString(), 
            "http://sourceforge.net/projects/knoda5/",
            "knoda4-bugs@lists.sourceforge.net");
        p_aData->addAuthor(ki18n("Horst Knorr").toString(), ki18n("Author of original version").toString(),
            "hk_classes@knoda.org","http://www.knoda.org");
        p_aData->addAuthor(ki18n("Patrik Hanak").toString(),ki18n("Author of KDE5 port").toString(),
            "knoda4-admins@lists.sourceforge.net");    
    }
    return *p_aData; 
}

hk_kdereportpart::hk_kdereportpart(QWidget* pWidget, QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent)
{
    p_private=new hk_kdereportpartprivate;
    setComponentData(hk_kdereportpartprivate::getAboutData());
    p_private->p_report = new hk_kdereportpartwidget(this,pWidget,0);
    p_private->p_report->setAttribute(Qt::WA_DeleteOnClose);
    // to include the part into other widget, it must not be flagged with Qt::Window, 
    // so we explicitly flag it with Qt::Widget
    p_private->p_report -> setWindowFlags(Qt::Widget);    
    setWidget(p_private->p_report);
    setXMLFile("hk_kdereportpart.rc");
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
