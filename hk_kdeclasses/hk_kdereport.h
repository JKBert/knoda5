// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2013 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
//
// This file is part of the hk_kde4classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef HK_KDEREPORT_H
#define HK_KDEREPORT_H
#include "hk_class.h"
#include <qwidget.h>
#include "hk_presentation.h"
#include <KParts/ReadWritePart>
#include "hk_kdereportpartwidget.h"
#include <kparts/partmanager.h>
#include <kparts/mainwindow.h>

class QButtonGroup;
class QLabel;
class hk_kdesimplereport;
class hk_database;
class hk_kdetoolbar;
class QToolButton;
class QVBoxLayout;
class QHBoxLayout;
class KRadioAction;
class QWidgetStack;

/**
 *
 *@short  KDE high level widget to display and design reports
 *@author Horst Knorr (hk_classes@knoda.org)
 *@version $Revision: 1.21 $
 *
 *this is a high level widget to display and design reports. It hereby uses @ref hk_kdesimplereport.
 *To use it set the database with @ref set_database. If you want to show or edit an existing reports
 * load it with @ref load_report.
 */

class hk_kdereport:public KParts::MainWindow, public hk_class
{

    Q_OBJECT

public:
    hk_kdereport( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    virtual    ~hk_kdereport();
    bool        set_presentationdatasource(long n,bool r=true);
    long        presentationdatasource(void);
    void set_database(hk_database*);
    hk_database* database(void);
    void set_nodesignmode(bool);
    void set_mode(hk_presentation::enum_mode s);
    hk_kdesimplereport* simplereport(void);
    bool load_report(const hk_string& name="");
signals:
    void signal_closed(QWidget*);
	void signal_setcaption(const QString&);

public slots:
    void set_designmode(void);
    void set_viewmode(void);
    void    save_report(void);
    void    saveas_report(void);
    void    close_report(void);
    void    slot_showtoolbar(void);
protected:
    void closeEvent ( QCloseEvent* e);

protected slots:
   	void part_removed(void);
	
private:
    KAction*  p_closeaction;
	KParts::ReadWritePart* p_part;
	KParts::PartManager* p_partmanager;
	hk_kdereportpartwidget*  p_report;
    void set_caption(void);
};
#endif
