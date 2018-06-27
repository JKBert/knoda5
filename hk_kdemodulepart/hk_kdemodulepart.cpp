// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4modulepart library
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for hk_kde5modulepart library
//
// This file is part of the hk_kde5modulepart library.
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
#include "hk_kdemodulepart.h"
#include <KAboutData>
#include <kstandardaction.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <klocale.h>
#include <kurl.h>

#include <qapplication.h>
#include <qclipboard.h>
#include <qcombobox.h>
#include <hk_drivermanager.h>
#include <hk_connection.h>
#include <hk_database.h>
#include <hk_datasource.h>

K_PLUGIN_FACTORY_DEFINITION(hk_kdemodulepartfactory, registerPlugin<hk_kdemodulepart>();)

class hk_kdemodulepartprivate
{
  public:
    hk_kdemodulepartprivate():p_module(NULL), activate(false)
      { }
    static const KAboutData& getAboutData();
    hk_kdemodulepartwidget* p_module;
    bool activate;
  private:
    static KAboutData aboutData;
    static bool aboutDataInitialized;     
};

KAboutData hk_kdemodulepartprivate::aboutData (LIB_MODULE_NAME, ki18n("hk_kde5modulepart").toString(),
            "0.2", ki18n("database module editor").toString(),
            KAboutLicense::GPL,
            ki18n("(c) 2002-2006, Horst Knorr\n(c) 2010-2018 Patrik Hanak").toString(),QString(), 
            "http://sourceforge.net/projects/knoda5/",
            "knoda4-bugs@lists.sourceforge.net");

bool hk_kdemodulepartprivate::aboutDataInitialized = false;

const KAboutData& hk_kdemodulepartprivate::getAboutData()
{
    if (!aboutDataInitialized) {
        aboutData.addAuthor(ki18n("Horst Knorr").toString(), ki18n("Author of original version").toString(),
            "hk_classes@knoda.org","http://www.knoda.org");
        aboutData.addAuthor(ki18n("Patrik Hanak").toString(),ki18n("Author of KDE5 port").toString(),
            "knoda4-admins@lists.sourceforge.net");
        aboutDataInitialized = true;
    }
    return aboutData; 
}

hk_kdemodulepart::hk_kdemodulepart(QWidget* pWidget, QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent), p_private(new hk_kdemodulepartprivate())
{
  setObjectName("hk_kdemodulepart");  
  setComponentData(hk_kdemodulepartprivate::getAboutData());
  p_private->p_module = new hk_kdemodulepartwidget(this,pWidget,0);
  p_private->p_module->setAttribute(Qt::WA_DeleteOnClose);
  setWidget(p_private->p_module);
  setXMLFile("hk_kdemodulepart.rc");
  p_private->p_module->setupActions(actionCollection());
}

hk_kdemodulepart::~hk_kdemodulepart()
{
  if (widget())
  {
    if (p_private->p_module->has_changed())
     p_private->p_module->save_module();
  }
  delete p_private;
}

void hk_kdemodulepart::setReadWrite(bool rw)
{
    KParts::ReadWritePart::setReadWrite(rw);
}

bool hk_kdemodulepart::openFile()
{
 // URL handling:   mysql:/user:password@host:port/databasename/datasourcetype/datasourcename
 // where datasourcetype is either tables or queries
    return true;
}

bool hk_kdemodulepart::saveFile()
{
    return true;
}
