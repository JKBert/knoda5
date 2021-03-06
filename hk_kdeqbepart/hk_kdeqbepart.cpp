// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde5qbepart library
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for hk_kde5qbepart library
//
// This file is part of the hk_kde5qbepart library.
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
#include "hk_kdeqbepart.h"
#include <kaboutdata.h>
#include <kcomponentdata.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kaction.h>
#include <ktoggleaction.h>
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

K_PLUGIN_FACTORY_DEFINITION(hk_kdeqbepartfactory, registerPlugin<hk_kdeqbepart>();)

class hk_kdeqbepartprivate
{
  public:
    hk_kdeqbepartprivate():p_qbe(NULL)
  	{
	}
    static const KAboutData& getAboutData(); 
    hk_kdeqbe* p_qbe;
    KAction* p_addaction;
    KToggleAction* p_distinctaction;
    KAction* p_qbetypeselectaction; 
    QComboBox* p_qbetypeselect;
  private:
    static KAboutData aboutData;
    static bool aboutDataInitialized;
};

KAboutData hk_kdeqbepartprivate::aboutData (LIB_MODULE_NAME, ki18n("hk_kde5qbepart").toString(),
            "0.2", ki18n("Query by Example editor").toString(),
            KAboutLicense::GPL,
            ki18n("(c) 2002-2004, Horst Knorr\n(c) 2010-2018 Patrik Hanak").toString(),QString(), 
            "http://sourceforge.net/projects/knoda5/",
            "knoda4-bugs@lists.sourceforge.net");

bool hk_kdeqbepartprivate::aboutDataInitialized = false;

const KAboutData& hk_kdeqbepartprivate::getAboutData()
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

hk_kdeqbepart::hk_kdeqbepart(QWidget* pWidget,QObject* parent, const QVariantList &)
:KParts::ReadWritePart(parent), p_private(new hk_kdeqbepartprivate())
{
    setComponentData(hk_kdeqbepartprivate::getAboutData());
    p_private->p_qbe = new hk_kdeqbe(pWidget);
    setWidget(p_private->p_qbe);
    KIconLoader loader (LIB_MODULE_NAME);
    setXMLFile("hk_kdeqbepart.rc");

    p_private->p_addaction = new KAction(QIcon(loader.iconPath("gridadd22x22",KIconLoader::User)),
      i18n("&Add datasource"),actionCollection());
    actionCollection() -> addAction("add",p_private->p_addaction);
    connect(p_private->p_addaction,SIGNAL(triggered()),this,SLOT(add_action()));     
    p_private->p_distinctaction = new KToggleAction(i18n("&Distinct rows"),actionCollection());
    actionCollection() -> addAction("distinct",p_private->p_distinctaction);
    connect(p_private->p_distinctaction,SIGNAL(triggered()),p_private->p_qbe,SLOT(distinct_changed()));
    p_private->p_qbetypeselect= new QComboBox(NULL);
    p_private->p_qbetypeselect->setObjectName(QString::fromAscii("p_qbetypeselect"));
    p_private->p_qbetypeselect->addItem(i18n("Select query"));
    p_private->p_qbetypeselect->addItem(i18n("Groupselect query"));
    p_private->p_qbetypeselect->addItem(i18n("Update query"));
    p_private->p_qbetypeselect->addItem(i18n("Delete query"));
    p_private->p_qbetypeselectaction=new KAction(i18n("&Add datasource"),actionCollection());
    p_private->p_qbetypeselectaction->setDefaultWidget(p_private->p_qbetypeselect);
    actionCollection()->addAction("qbetypeselect",p_private->p_qbetypeselectaction);
    connect(p_private->p_qbetypeselect,SIGNAL(activated(int)),this,SLOT(qbetypeselect_action(int)));
    connect(p_private->p_qbe,SIGNAL(signal_qbetype_has_changed()),this,SLOT(qbetypechange_action()));
    connect(p_private->p_qbe,SIGNAL(signal_distinct_has_changed()),this,SLOT(distinct_action()));
}

hk_kdeqbepart::~hk_kdeqbepart()
{
  delete p_private;
}

void hk_kdeqbepart::setReadWrite(bool rw)
{
    KParts::ReadWritePart::setReadWrite(rw);
}

bool hk_kdeqbepart::openFile()
{
 // URL handling:   mysql:/user:password@host:port/databasename/datasourcetype/datasourcename
 // where datasourcetype is either tables or queries
    return true;
}

bool hk_kdeqbepart::saveFile()
{
    return true;
}

hk_kdeqbe* hk_kdeqbepart::dbdesigner(void)
{
  return p_private->p_qbe;
}

void hk_kdeqbepart::add_action(void)
{
  p_private->p_qbe->add_datasource();
}


void hk_kdeqbepart::qbetypeselect_action(int t)
{
  hk_qbe::enum_querytype ntype=hk_qbe::qt_select;
  switch (t)
  {
    case 1 : ntype=hk_qbe::qt_groupselect;break;
    case 2 : ntype=hk_qbe::qt_update;break;
    case 3 : ntype=hk_qbe::qt_delete;break;
    default: ntype=hk_qbe::qt_select;
  }
  p_private->p_qbe->set_querytype(ntype);
}

void hk_kdeqbepart::qbetypechange_action(void)
{
  p_private->p_qbetypeselect->blockSignals(true);
  int nr=0;
  switch(p_private->p_qbe->querytype())
  {
    case hk_qbe::qt_groupselect : nr=1;break;
    case hk_qbe::qt_update : nr=2;break;
    case hk_qbe::qt_delete : nr=3;break;
    default	      :	nr=0;
  }
  p_private->p_qbetypeselect->setCurrentIndex(nr);
  p_private->p_qbetypeselect->blockSignals(false);
}

void hk_kdeqbepart::distinct_action(void)
{
  p_private->p_distinctaction->setChecked(p_private->p_qbe->distinct());
}



