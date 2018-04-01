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

#ifndef HK_KDEMODULEPART
#define HK_KDEMODULEPART

#include <KParts/ReadWritePart>
#include <KParts/Factory>
#include <KPluginFactory>
#include "../hk_kdeclasses/hk_kdemodulepartwidget.h"

class hk_drivermanager;
class hk_kdemodulepartprivate;
class KAboutData;

class hk_kdemodulepart : public KParts::ReadWritePart
{
friend class hk_kdemodulepartwidget;
    Q_OBJECT
        public:
        hk_kdemodulepart(QWidget* parentWidget, QObject* parent,  const QVariantList &args);
        virtual ~hk_kdemodulepart();
        virtual void setReadWrite(bool rw);
    protected:
        virtual bool openFile();
        virtual bool saveFile();

    private:
      hk_kdemodulepartprivate* p_private;
};

K_PLUGIN_FACTORY_DECLARATION(hk_kdemodulepartfactory)

#endif
