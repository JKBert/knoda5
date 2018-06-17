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

#ifndef HK_KDEMODULE_H
#define HK_KDEMODULE_H
#include <hk_module.h>
#include <qvariant.h>
#include <qwidget.h>
#include <hk_presentation.h>
#include <hk_class.h>
#include <KParts/ReadWritePart>
#include <kparts/partmanager.h>
#include "hk_kdemodulepartwidget.h"
#include <kparts/mainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QScrollView;
class hk_kdesimplemodule;
class QToolButton;
class QButtonGroup;
class hk_kderowselector;
class hk_kdetoolbar;
class hk_kdeproperty;
class hk_kdegridpart;

/**
 *
 *@short  KDE high level widget to display and design modules
 *@author Horst Knorr (hk_classes@knoda.org)
 *@version $Revision: 1.1 $
 *
 *this is a high level widget to display and design modules. It hereby uses @ref hk_kdesimplemodule.
 *To use it set the database with @ref set_database. If you want to show or edit an existing module
 * load it with @ref load_module.
 */

class hk_kdemodule : public KParts::MainWindow,public hk_class
{
    Q_OBJECT
     friend class hk_kdesimplemodule;
public:
    hk_kdemodule( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    virtual    ~hk_kdemodule();
    void            set_database(hk_database* db);
    hk_database*        database(void);
    void        loaddata(xmlNodePtr);
    void        savedata(ofstream& s);
    void        load_module(const hk_string& n);
/**
 *when the datasource vanishes (i.e. will be destroyed) and autoclose is true, the hk_kdemodule will be
 *closed otherwise it still is on your screen.
 */
    void set_autoclose(bool c);
/**
 *if the designwindow should not be shown set this true. Default is false.
 */
signals:
    void signal_closed(QWidget*);
    
protected:
    virtual void closeEvent ( QCloseEvent* e);
    virtual void        mousePressEvent(QMouseEvent*);
	KAction * p_closeaction;
	void save_module_when_changed();
    protected slots:
	void part_removed(void);
    void close_module(void);
    private:
    void set_caption(void);
	KParts::ReadWritePart* p_part;
	KParts::PartManager* p_partmanager;
	hk_kdemodulepartwidget*  p_module;
};
#endif
