// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde5querypart library
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for hk_kde5querypart library
//
// This file is part of the hk_kde5querypart library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#error config.h is needed but not included 
#endif
#include "hk_kdequerypart.h"
#include "hk_kdeqbe.h"
#include <hk_qbe.h>
#include <kaboutdata.h>
#include <kcomponentdata.h>
#include <kstandardaction.h>
#include <kiconloader.h>
#include <kaction.h>
#include <ktoggleaction.h>
#include <klocale.h>
#include <kparts/partmanager.h>
#include <kactioncollection.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qcombobox.h>
#include <hk_drivermanager.h>
#include <hk_connection.h>
#include <hk_database.h>
#include <hk_datasource.h>

K_PLUGIN_FACTORY_DEFINITION(hk_kdequerypartfactory, registerPlugin<hk_kdequerypart>();)

class hk_kdequerypartprivate
{
  public:
    hk_kdequerypartprivate():p_query(NULL),activate(false)
  	{
	}
    static const KAboutData& getAboutData(); 
    hk_kdequerypartwidget* p_query;
    bool activate;
  private:
    static KAboutData aboutData;
    static bool aboutDataInitialized;
};

KAboutData hk_kdequerypartprivate::aboutData (LIB_MODULE_NAME, ki18n("hk_kde5querypart").toString(),
            "0.2", ki18n("database query editor").toString(),
            KAboutLicense::GPL,
            ki18n("(c) 2002-2004, Horst Knorr\n(c) 2010-2018 Patrik Hanak").toString(),QString(), 
            "http://sourceforge.net/projects/knoda5/",
            "knoda4-bugs@lists.sourceforge.net");

bool hk_kdequerypartprivate::aboutDataInitialized = false;

const KAboutData& hk_kdequerypartprivate::getAboutData()
{
    if (!aboutDataInitialized) {
        aboutData.addAuthor(ki18n("Horst Knorr").toString(),ki18n("Author of original version").toString(),
            "hk_classes@knoda.org","http://www.knoda.org");
        aboutData.addAuthor(ki18n("Patrik Hanak").toString(),ki18n("Author of KDE5 port").toString(),
            "knoda4-admins@lists.sourceforge.net");        
    }
    return aboutData;  
}

hk_kdequerypart::hk_kdequerypart(QWidget* pWidget,QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent), p_private(new hk_kdequerypartprivate)
{
    setComponentData(hk_kdequerypartprivate::getAboutData());
    p_private->p_query = new hk_kdequerypartwidget(this,pWidget,0);
    p_private->p_query->setAttribute(Qt::WA_DeleteOnClose);
    setWidget(p_private->p_query);
    setXMLFile("hk_kdequerypartqbe.rc");
    p_private->p_query->setupActions(actionCollection());
    connect(p_private->p_query->p_addaction,SIGNAL(triggered()),this,SLOT(add_action()));
    connect(p_private->p_query->p_qbetypeselect,SIGNAL(activated(int)),this,SLOT(qbetypeselect_action(int)));
    connect(p_private->p_query->kdeqbe(),SIGNAL(signal_qbetype_has_changed()),this,SLOT(qbetypechange_action()));
    connect(p_private->p_query->kdeqbe(),SIGNAL(signal_distinct_has_changed()),this,SLOT(distinct_action()));
}

hk_kdequerypart::~hk_kdequerypart()
{
  if (widget())
  {
     if (p_private->p_query->has_changed())
     p_private->p_query->save_query();
  }
  delete p_private;
}

void hk_kdequerypart::setReadWrite(bool rw)
{
    KParts::ReadWritePart::setReadWrite(rw);
}

bool hk_kdequerypart::openFile()
{
 // URL handling:   mysql:/user:password@host:port/databasename/datasourcetype/datasourcename
 // where datasourcetype is either tables or queries
    return true;
}

bool hk_kdequerypart::saveFile()
{
    return true;
}

void hk_kdequerypart::add_action(void)
{
  p_private->p_query->kdeqbe()->add_datasource();
}

void hk_kdequerypart::qbetypeselect_action(int t)
{
 hk_qbe::enum_querytype ntype=hk_qbe::qt_select;
 switch (t)
 {
    case 1 : ntype=hk_qbe::qt_groupselect;break;
    case 2 : ntype=hk_qbe::qt_update;break;
    case 3 : ntype=hk_qbe::qt_delete;break;
    default: ntype=hk_qbe::qt_select;
 }
p_private->p_query->kdeqbe()->set_querytype(ntype);

}

void hk_kdequerypart::qbetypechange_action(void)
{
p_private->p_query->p_qbetypeselect->blockSignals(true);
int nr=0;
switch(p_private->p_query->qbe()->querytype())
{
  case hk_qbe::qt_groupselect : nr=1;break;
  case hk_qbe::qt_update : nr=2;break;
  case hk_qbe::qt_delete : nr=3;break;
  default	      :	nr=0;
}
p_private->p_query->p_qbetypeselect->setCurrentIndex(nr);
p_private->p_query->p_qbetypeselect->blockSignals(false);
}

void hk_kdequerypart::distinct_action(void)
{
  p_private->p_query->p_distinctaction->setChecked(p_private->p_query->kdeqbe()->distinct());
}

void hk_kdequerypart::setXMLFile(const QString& file,bool merge,bool setxmldoc)
{
//if (manager()&&manager()->activePart()==this)
  KParts::ReadWritePart::setXMLFile(file,merge,setxmldoc);
  if (manager()&&manager()->activePart()==this&&! p_private->activate)
  {
    p_private->activate=true;
    manager()->setActivePart(NULL);
    manager()->setActivePart(this);
    p_private->activate=false;
  }
}



