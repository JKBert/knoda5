// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for hk_kde5classes library
//
// This file is part of the hk_kde5classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef HK_KDETABLE_H
#define HK_KDETABLE_H

#include <kparts/partmanager.h>
#include <kparts/mainwindow.h>
#include <qvariant.h>
#include <qwidget.h>
#include <hk_dstable.h>
#include "hk_kdetablepartwidget.h"

class hk_kdegrid;
class hk_kdegridpart;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QButtonGroup;
class QTable;
class hk_kdetoolbar;

/**
 *
 *@short  KDE high level widget to display and alter tables.
 *@version $Revision: 1.21 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *hk_kdetable is a high level widget to display the content of tables and to define and alter the
 *table structure.
 *
 */

class hk_kdetable : public KParts::MainWindow ,public hk_dstable
{
  Q_OBJECT
        public:
        hk_kdetable(QWidget* w=0,const char* n=0,Qt::WFlags f=0);
        virtual    ~hk_kdetable();
        virtual bool            set_mode(enum_mode s);

/**
 * true if the design window is shown
 */
	bool in_designmode(void);
        virtual void set_datasource(hk_datasource*);

        void set_autoclose(bool c);
/**
 *if the designwindow should not be shown set this true. Default is false.
 */

    signals:
        void signal_closed(QWidget*);
    
    protected slots:
	void part_removed(void);
        void close_table(void);

    protected:
        void closeEvent ( QCloseEvent* e);

    private:
        KAction*      p_closeaction;
        void set_caption(void);
	
        KParts::ReadWritePart* p_part;
	KParts::PartManager* p_partmanager;
	hk_kdetablepartwidget*  p_table;


};
#endif                                            // HK_KDETABLE_H
