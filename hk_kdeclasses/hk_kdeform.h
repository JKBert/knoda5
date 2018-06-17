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

#ifndef HK_KDEform_H
#define HK_KDEform_H
#include <hk_form.h>
#include <qvariant.h>
#include <qwidget.h>
#include <hk_presentation.h>
#include <hk_class.h>
#include <kparts/partmanager.h>
#include <KParts/ReadWritePart>
#include "hk_kdeformpartwidget.h"
#include <kparts/mainwindow.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class hk_kdesimpleform;
class QToolButton;
class QButtonGroup;
class hk_kderowselector;
class hk_kdetoolbar;
class hk_kdeproperty;
class hk_kdegridpart;

/**
 *
 *@short  KDE high level widget to display and design forms
 *@author Horst Knorr (hk_classes@knoda.org)
 *@version $Revision: 1.35 $
 *
 *this is a high level widget to display and design forms. It hereby uses @ref hk_kdesimpleform.
 *To use it set the database with @ref set_database. If you want to show or edit an existing form
 * load it with @ref load_form.
 */

class hk_kdeform : public KParts::MainWindow,public hk_class
{
    Q_OBJECT
     friend class hk_kdesimpleform;
        public:
        hk_kdeform( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
        virtual    ~hk_kdeform();

        hk_presentation::enum_mode  mode(void);
/**
 * See @ref hk_kdesimpleform for details
 */
        void            set_mode(hk_presentation::enum_mode s);
        void            set_viewmode(void);
        void            set_designmode(void);
/**
 * See @ref hk_kdesimpleform for details
 */
        hk_dsgrid*      new_grid(void);
/**
 * See @ref hk_kdesimpleform for details
 */
        hk_dslineedit*      new_lineedit(void);
/**
 * See @ref hk_kdesimpleform for details
 */
        hk_dsboolean*       new_bool(void);
/**
 * See @ref hk_kdesimpleform for details
 */
        hk_button*      new_button(void);
/**
 * See @ref hk_kdesimpleform for details
 */
        hk_dsrowselector*   new_rowselector(void);
/**
 * See @ref hk_kdesimpleform for details
 */
        hk_dsmemo*      new_memo(void);

	hk_subform*     new_subform(void);	
/**
 * See @ref hk_kdesimpleform for details
 */
        void            set_formsize(unsigned int width, unsigned int height);
/**
 * See @ref hk_kdesimpleform for details
 */
        void            set_database(hk_database* db);
        hk_database*        database(void);
        list<hk_datasource*>*   datasources(void);
        long            new_datasource(const hk_string& name,datasourcetype dt=dt_table);
        hk_datasource*      get_datasource(long nr);
/**
 * returns a pointer to the internally used hk_kdesimpleform. Don't delete the object!
 */
        hk_kdesimpleform*   simpleform(void);
/**
 * if r=true the simpleform will also be set. Usually you have to leave it to "true"
 */
        bool        set_presentationdatasource(long n,bool r=true);
        long        presentationdatasource(void);
        void        loaddata(xmlNodePtr);
        void        savedata(ofstream& s);
        void        load_form(const hk_string& n);
/**
 *when the datasource vanishes (i.e. will be destroyed) and autoclose is true, the hk_kdeform will be
 *closed otherwise it still is on your screen.
 */

        void set_autoclose(bool c);
/**
 *if the designwindow should not be shown set this true. Default is false.
 */
        void set_nodesignmode(bool n);
	void save_form_when_changed(void);
	
	hk_kdeproperty* propertyeditor(void);
    signals:
        void signal_closed(QWidget*);
        void signal_captionChanged(QWidget*, const QString&);    
	

    protected:
        virtual void closeEvent ( QCloseEvent* e);
        virtual void        mousePressEvent(QMouseEvent*);
	KAction * p_closeaction;
    protected slots:
	void part_removed(void);
	void grid_partinfocus(hk_kdegridpart*);
	void grid_partoutfocus(hk_kdegridpart*);
        void close_form(void);
    private:
        void set_caption(void);
	KParts::ReadWritePart* p_part;
	KParts::PartManager* p_partmanager;
	hk_kdeformpartwidget*  p_form;
};
#endif
