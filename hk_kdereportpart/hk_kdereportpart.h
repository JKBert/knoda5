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

#ifndef HK_KDEREPORTPART
#define HK_KDEREPORTPART

//TBP #include <kparts/part.h>
#include <KParts/ReadWritePart>
//TBP #include <kparts/factory.h>
#include <KParts/Factory>
#include "../hk_kdeclasses/hk_kdereportpartwidget.h"

class hk_drivermanager;
class hk_kdereportpartprivate;
class KAboutData;

class hk_kdereportpart : public KParts::ReadWritePart
{
friend class hk_kdereportpartwidget;
    Q_OBJECT
        public:
        hk_kdereportpart(QWidget* parentWidget, QObject* parent, const QVariantList &args);
        virtual ~hk_kdereportpart();
        //TBP static KAboutData* createAboutData();
        virtual void setReadWrite(bool rw);
    protected:
        virtual bool openFile();
        virtual bool saveFile();
	virtual void setXMLFile(const QString& file,bool merge=false,bool setxmldoc=true);

    protected slots:

    private:
      hk_kdereportpartprivate* p_private;
};

K_PLUGIN_FACTORY_DECLARATION(hk_kdereportpartfactory)

#endif
