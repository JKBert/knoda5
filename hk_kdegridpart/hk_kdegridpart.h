// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4gridpart library
//
// This file is part of the hk_kde4gridpart library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef HK_KDEGRIDPART
#define HK_KDEGRIDPART
#include <KParts/ReadWritePart>
#include <KParts/Factory>
#include <KPluginFactory>
#include "hk_kdegrid.h"

class hk_drivermanager;
class KAction;

class hk_kdegridpart : public KParts::ReadWritePart
{
    Q_OBJECT
    public:
        hk_kdegridpart(QWidget* parentWidget, QObject* parent, const QVariantList &args);
        virtual ~hk_kdegridpart();
        virtual void setReadWrite(bool rw);
        hk_kdegrid* grid(void){return p_grid;}
    protected:
        virtual bool openFile();
        virtual bool saveFile();

    protected slots:
        virtual void show_gridcolumndialog(void);
	virtual void clipboarddata_has_changed(void);

    private:
        hk_kdegrid* p_grid;
        KAction* p_columndialogaction;
        KAction* p_findaction;
        KAction* p_copyaction;
        KAction* p_pasteaction;
	hk_drivermanager* p_drivermanager;
};

K_PLUGIN_FACTORY_DECLARATION(hk_kdegridpartfactory)

#endif
