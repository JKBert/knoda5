// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4querypart library
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

#ifndef HK_KDEQUERYPART
#define HK_KDEQUERYPART
#include <KParts/ReadWritePart>
#include <KParts/Factory>
#include <KPluginFactory>
#include "../hk_kdeclasses/hk_kdequerypartwidget.h"

class hk_drivermanager;
class hk_kdequerypartprivate;

class hk_kdequerypart : public KParts::ReadWritePart
{
    friend class hk_kdequerypartwidget;
    Q_OBJECT
    public:
        hk_kdequerypart(QWidget* parentWidget,QObject* parent, const QVariantList &args);
        virtual ~hk_kdequerypart();
        virtual void setReadWrite(bool rw);
    protected:
        virtual bool openFile();
        virtual bool saveFile();
	virtual void setXMLFile(const QString& file,bool merge=false,bool setxmldoc=true);

    protected slots:
   	virtual void add_action(void);
   	virtual void qbetypeselect_action(int);
   	virtual void distinct_action(void);
	virtual void qbetypechange_action(void);
    private:
      hk_kdequerypartprivate* p_private;

};

K_PLUGIN_FACTORY_DECLARATION(hk_kdequerypartfactory)

#endif
