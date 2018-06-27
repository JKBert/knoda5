// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4formpart library
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for hk_kde5formpart library
//
// This file is part of the hk_kde5formpart library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.13 $

//***********************************************
//***  hk_kdeform PART definition             ***
//***********************************************
#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#error config.h is needed but not included 
#endif
#include "hk_kdeformpart.h"
#include <KAboutData>
#include "hk_kdesimpleform.h"
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
#include "hk_kdetoolbar.h"

K_PLUGIN_FACTORY_DEFINITION(hk_kdeformpartfactory, registerPlugin<hk_kdeformpart>();)

class hk_kdeformpartprivate
{
  public:
    hk_kdeformpartprivate()
  	{
		p_form=NULL;
		activate=false;
	}
    static const KAboutData& getAboutData();    
    hk_kdeformpartwidget* p_form;
    bool activate;
  private:
    static KAboutData aboutData;
    static bool aboutDataInitialized;
};

KAboutData hk_kdeformpartprivate::aboutData  (LIB_MODULE_NAME, ki18n("hk_kde5formpart").toString(),
            "0.2", ki18n("database form editor").toString(),
            KAboutLicense::GPL,
            ki18n("(c) 2002-2004, Horst Knorr\n(c) 2010-2018 Patrik Hanak").toString(),QString(), 
            "http://sourceforge.net/projects/knoda5/",
            "knoda4-bugs@lists.sourceforge.net");

bool hk_kdeformpartprivate::aboutDataInitialized = false;

const KAboutData& hk_kdeformpartprivate::getAboutData()
{
    if (!aboutDataInitialized) {
        aboutData.addAuthor(ki18n("Horst Knorr").toString(),ki18n("Author of original version").toString(),
            "hk_classes@knoda.org","http://www.knoda.org");
        aboutData.addAuthor(ki18n("Patrik Hanak").toString(),ki18n("Author of KDE5 port").toString(),
            "knoda4-admins@lists.sourceforge.net");
        aboutDataInitialized = true;   
    }
    return aboutData;  
}

hk_kdeformpart::hk_kdeformpart(QWidget* pWidget,QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent), p_private(new hk_kdeformpartprivate())
{
    setComponentData(hk_kdeformpartprivate::getAboutData());
    setXMLFile("hk_kdeformpartdesign.rc");
    p_private->p_form = new hk_kdeformpartwidget(this,pWidget,0);
    p_private->p_form->setAttribute(Qt::WA_DeleteOnClose);
    // to include the part into other widget, it must not be flagged with Qt::Window, 
    // so we explicitly flag it with Qt::Widget
    p_private->p_form->setWindowFlags(Qt::Widget);
    setWidget(p_private->p_form);

}

hk_kdeformpart::~hk_kdeformpart()
{
  if (widget())
  {
    if (p_private->p_form->simpleform()->has_changed())
      p_private->p_form->simpleform()->save_form();
  }
  delete p_private;
}

void hk_kdeformpart::setReadWrite(bool rw)
{
    KParts::ReadWritePart::setReadWrite(rw);
}

bool hk_kdeformpart::openFile()
{
 // URL handling:   mysql:/user:password@host:port/databasename/datasourcetype/datasourcename
 // where datasourcetype is either tables or queries
    return true;
}

bool hk_kdeformpart::saveFile()
{
    return true;
}

void hk_kdeformpart::setXMLFile(const QString& file,bool merge,bool setxmldoc)
{
  KParts::ReadWritePart::setXMLFile(file,merge,setxmldoc);
  if (manager()&&manager()->activePart()==this&&! p_private->activate)
  {
    p_private->activate=true;
  //manager()->blockSignals(true);
    manager()->setActivePart(NULL);
  //manager()->blockSignals(false);
    manager()->setActivePart(this);
    p_private->activate=false;
  }
}
