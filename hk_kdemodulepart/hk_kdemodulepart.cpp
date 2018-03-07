// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4modulepart library
//
// This file is part of the hk_kde4modulepart library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.1 $

//***********************************************
//***  hk_kdemodule PART definition             ***
//***********************************************
#include "hk_kdemodulepart.h"
#include <kcomponentdata.h>
#include <kaboutdata.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kactioncollection.h>
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

K_PLUGIN_FACTORY(hk_kdemodulepartfactory, registerPlugin<hk_kdemodulepart>();)
K_EXPORT_PLUGIN(hk_kdemodulepartfactory("hk_kde4modulepart","hk_kde4modulepart"))

class hk_kdemodulepartprivate
{
  public:
    hk_kdemodulepartprivate():p_module(NULL), activate(false)
      { }
    hk_kdemodulepartwidget* p_module;
    bool activate;
};

hk_kdemodulepart::hk_kdemodulepart(QWidget* pWidget, QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent)
{
  setObjectName("hk_kdemodulepart");
  p_private=new hk_kdemodulepartprivate;
  setComponentData(hk_kdemodulepartfactory::componentData());
  p_private->p_module = new hk_kdemodulepartwidget(this,pWidget,0);
  p_private->p_module->setAttribute(Qt::WA_DeleteOnClose);
  setWidget(p_private->p_module);
  KIconLoader* loader=KIconLoader::global();
  loader->addAppDir("hk_kde4classes");
  setXMLFile(KStandardDirs::locate("data","hk_kde4classes/hk_kdemodulepart.rc"));
  p_private -> p_module -> setupActions(actionCollection());
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



/*void hk_kdemodulepart::show_dbdesignercolumndialog(void)
{
    p_private->p_table->simpledbdesigner()->show_dbdesignercolumndialog();
}*/


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






KAboutData* hk_kdemodulepart::createAboutData()
{
    KAboutData* a= new KAboutData("hk_kde4modulepart", "hk_kde4modulepart",ki18n("hk_kde4modulepart"),
        "0.2", ki18n("database module editor"),
        KAboutData::License_GPL,
     ki18n("(c) 2002-2006, Horst Knorr\n(c) 2010-2016 Patrik Hanak"),ki18n(NULL), "http://sourceforge.net/projects/knoda4/",
     "knoda4-bugs@lists.sourceforge.net");
    a -> addAuthor(ki18n("Horst Knorr"),ki18n("Author of original version"), "hk_classes@knoda.org","http://www.knoda.org");
    a -> addAuthor(ki18n("Patrik Hanak"),ki18n("Author of KDE4 port"), "knoda4-admins@lists.sourceforge.net");    
    return a;

}





