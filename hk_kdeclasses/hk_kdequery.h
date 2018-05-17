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

#ifndef HK_KDEQUERY_H
#define HK_KDEQUERY_H

#include <qvariant.h>
#include <kparts/partmanager.h>
#include "hk_kdequerypartwidget.h"
#include <kparts/mainwindow.h>
#include "hk_dsquery.h"
#include "hk_datasource.h"
#include "hk_database.h"
#include <KParts/ReadWritePart>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QButtonGroup;
class QTable;
class QToolButton;
class QMultiLineEdit;

/**
 *
 *@short KDE high level widget to define queries and see the result.
 *@version $Revision: 1.31 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *hk_kdequery is a high level widget to define queries and see the result.
 *In this mode of development it has a field to enter the SQL query and a grid which shows the
 * resulting data if there is any. hk_kdequery also handles actionqueries correctly.
 */


class hk_kdequery : public KParts::MainWindow ,public hk_dsquery
{
    Q_OBJECT

        public:
        hk_kdequery(QWidget* w=0,const char* n=0,Qt::WFlags f=0);
virtual        ~hk_kdequery();
        virtual bool            set_mode(enum_mode s);
/**
 *returns true if the sqlwindow or the qbe window is shown. Else returns false (when the result data is shown)
 */
        bool in_designmode(void) const ;


/**
 *if the designwindow should not be shown set this true. Default is false.
 */
        void set_nodesignmode(bool n);
        virtual void set_datasource(hk_datasource*);

        bool save_query(const hk_string& n="",bool ask=true);
        bool load_query(void);
        void set_autoclose(bool c);
    signals:
        void signal_closed(QWidget*);
	void signal_captionChanged(QWidget*, const QString&); 

    public slots:
        void close_query(void);
    protected slots:
	void part_removed(void);


    protected:
        virtual void closeEvent ( QCloseEvent* e);
        virtual void before_source_vanishes(void);


    private:
        void set_caption(void);
	KParts::ReadWritePart* p_part;
	KParts::PartManager* p_partmanager;
	hk_kdequerypartwidget*  p_query;
	KAction * p_closeaction;


};
#endif
