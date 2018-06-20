// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4dbdesignerpart library
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for hk_kde5dbdesignerpart library
//
// This file is part of the hk_kde5dbdesignerpart library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.4 $

//***********************************************
//***  hk_kdedbdesigner PART definition             ***
//***********************************************
#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#error config.h is needed but not included 
#endif
#include "hk_kdedbdesignerpart.h"
#include <kaboutdata.h>
#include <kcomponentdata.h>
#include <kstandardaction.h>
#include <kiconloader.h>
#include <kaction.h>
#include <klocale.h>
#include <kurl.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <hk_drivermanager.h>
#include <hk_connection.h>
#include <hk_database.h>
#include <hk_datasource.h>
#include <kactioncollection.h>

K_PLUGIN_FACTORY_DEFINITION(hk_kdedbdesignerpartfactory, registerPlugin<hk_kdedbdesignerpart>();)

class hk_kdedbdesignerpartprivate
{
  public:
    hk_kdedbdesignerpartprivate():p_dbdesigner(NULL)
  	{
	}
    static KAboutData* p_aData;
    static KAboutData& getAboutData(); 
    hk_kdedbdesigner* p_dbdesigner;
    KAction* p_addaction;
};

KAboutData* hk_kdedbdesignerpartprivate::p_aData = NULL; 

KAboutData& hk_kdedbdesignerpartprivate::getAboutData()
{
    if ( p_aData == NULL) {
        p_aData = new KAboutData(LIB_MODULE_NAME, ki18n("hk_kde5dbdesignerpart").toString(),
            "0.2", ki18n("Database editor").toString(),
            KAboutLicense::GPL,
            ki18n("(c) 2002-2004, Horst Knorr\n(c) 2010-2018 Patrik Hanak").toString(),QString(), 
            "http://sourceforge.net/projects/knoda5/",
            "knoda4-bugs@lists.sourceforge.net");
        p_aData->addAuthor(ki18n("Horst Knorr").toString(),ki18n("Author of original version").toString(),
            "hk_classes@knoda.org","http://www.knoda.org");
        p_aData->addAuthor(ki18n("Patrik Hanak").toString(),ki18n("Author of KDE5 port").toString(),
            "knoda4-admins@lists.sourceforge.net");        
    }
    return *p_aData;  
}

hk_kdedbdesignerpart::hk_kdedbdesignerpart(QWidget* pWidget, QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent), p_private(new hk_kdedbdesignerpartprivate)
{
  KIconLoader loader (LIB_MODULE_NAME);
  
  setComponentData(hk_kdedbdesignerpartprivate::getAboutData());
  p_private->p_dbdesigner = new hk_kdedbdesigner(pWidget);
  setWidget(p_private->p_dbdesigner);
  setXMLFile("hk_kdedbdesignerpart.rc");

  p_private->p_addaction = new KAction(i18n("&Add datasource"),actionCollection());
  p_private->p_addaction->setIcon(QIcon(loader.iconPath("gridadd22x22",KIconLoader::User)));
  actionCollection()->addAction("add",p_private->p_addaction);
  connect(p_private->p_addaction,SIGNAL(triggered()),this,SLOT(add_action()));
}

hk_kdedbdesignerpart::~hk_kdedbdesignerpart()
{
  delete p_private->p_addaction;
  delete p_private;
}

void hk_kdedbdesignerpart::setReadWrite(bool rw)
{
  KParts::ReadWritePart::setReadWrite(rw);
}

bool hk_kdedbdesignerpart::openFile()
{
 // URL handling:   mysql:/user:password@host:port/databasename/datasourcetype/datasourcename
 // where datasourcetype is either tables or queries
  return true;
}

bool hk_kdedbdesignerpart::saveFile()
{
  return true;
}

hk_kdedbdesigner* hk_kdedbdesignerpart::dbdesigner(void)
{
  return p_private->p_dbdesigner;
}

void hk_kdedbdesignerpart::add_action(void)
{
  if (p_private->p_dbdesigner) p_private->p_dbdesigner->add_datasource();
}
