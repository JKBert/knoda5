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

#ifndef HK_KDEFORMPART
#define HK_KDEFORMPART
#include <KParts/ReadWritePart>
#include <KParts/Factory>
#include <KPluginFactory>
#include <kaction.h>
#include "../hk_kdeclasses/hk_kdeformpartwidget.h"

class hk_drivermanager;
class hk_kdeformpartprivate;
class hk_kdetoolbar;

class hk_kdeformpart : public KParts::ReadWritePart
{
friend class hk_kdeformpartwidget;
    Q_OBJECT
    public:
        hk_kdeformpart(QWidget* parentWidget,QObject* parent, const QVariantList &args);
        virtual ~hk_kdeformpart();
        static KAboutData* createAboutData();
        virtual void setReadWrite(bool rw);
    
    protected:
        virtual bool openFile();
        virtual bool saveFile();
	virtual void setXMLFile(const QString& file,bool merge=false,bool setxmldoc=true);

    private:
      hk_kdeformpartprivate* p_private;
};

K_PLUGIN_FACTORY_DECLARATION(hk_kdeformpartfactory)

#endif
