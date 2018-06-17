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

#ifndef HK_KDEQUERYPARTWIDGET_H
#define HK_KDEQUERYPARTWIDGET_H

#include <kde4classesmacros.h>
#include <qvariant.h>
#include <qstackedwidget.h>
#include "hk_dsquery.h"
#include "hk_datasource.h"
#include "hk_database.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QButtonGroup;
class QTable;
class QToolButton;
class QProgressDialog;
class QMultiLineEdit;
class QComboBox;
class hk_kdetoolbar;
class hk_kdegrid;
class hk_kdegridpart;
class hk_kdeqbe;
class hk_kdequerypart;
class KAction;
class KToggleAction;
class KActionCollection;

class hk_kdequerypartwidgetprivate;

/**
 *
 *@short KDE high level widget to define queries and see the result.
 *@version $Revision: 1.13 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *hk_kdequerypartwidget is a high level widget to define queries and see the result.
 *In this mode of development it has a field to enter the SQL query and a grid which shows the
 * resulting data if there is any. hk_kdequerypartwidget also handles actionqueries correctly.
 */


class KDECLASSES_EXPORT hk_kdequerypartwidget : public QStackedWidget ,public hk_dsquery
{
    friend class hk_kdequerypart;
    Q_OBJECT

        public:
virtual        ~hk_kdequerypartwidget();
        virtual bool            set_mode(enum_mode s);
/**
 *returns true if the sqlwindow or the qbe window is shown. Else returns false (when the result data is shown)
 */
        bool in_designmode(void) const ;
        virtual void set_datasource(hk_datasource*);

/**
 *when the datasource vanishes (i.e. will be destroyed) and autoclose is true, the hk_kdequerypartwidget will be
 *closed otherwise it still is on your screen.
 */
        void set_autoclose(bool c);
	bool autoclose(void)const;

/**
 *if the designwindow should not be shown set this true. Default is false.
 */
        void set_nodesignmode(bool n);

        bool save_query(const hk_string& n="",bool ask=true);
        bool load_query(void);
	hk_dsgrid *grid(void);
	virtual hk_qbe*  qbe(void);
	hk_kdeqbe*   kdeqbe(void) const;
virtual	void reset_has_changed(void);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);

    signals:
        void signal_closed(QWidget*);
	void signal_qbetypechanged(void);
	void signal_captionChanged(QWidget*, const QString&);

    public slots:
        void designbutton_clicked(void);
        void querybutton_clicked(void);
        void query_changed(void);
        void savebutton_clicked(void);
        void saveasbutton_clicked(void);
        void close_query(void);
        void reload_query(void);
        void print(void);
	void action_useqbe(void);
	void copy_clicked(void);
	void copybackendsql_clicked(void);
	void paste_clicked(void);
	void replace_clicked(void);
	void find_clicked(void);
	void findnext_clicked(void);
	void findprevious_clicked(void);
	void cut_clicked(void);
	void undo_clicked(void);
	void redo_clicked(void);
	void selectall_clicked(void);
	void configeditor_clicked(void);
	void storeresult_clicked(void);

virtual void show ();
virtual void showMaximized ();


    protected:
        hk_kdequerypartwidget(hk_kdequerypart*,QWidget* w=0,const char* n=0,Qt::WFlags f=0);
        virtual void before_source_vanishes(void);
        virtual void closeEvent ( QCloseEvent* e);
	void    setupActions(KActionCollection* pac);
        virtual void set_has_changed(enum_has_changed forcesetting=standard);
 static  bool set_progress(long int,long int,const hk_string&);
    protected slots:
	void qbe_has_changed(void);
        void exec_cancelled(void);

    private:
        KToggleAction* p_designaction;
        KToggleAction* p_viewaction;
        KAction*      p_saveaction;
        KAction*      p_saveasaction;
        KAction*      p_printaction;
        KAction*      p_reloadaction;

	KAction*      p_copyaction;
	KAction*      p_copybackendsqlaction;
	KAction*      p_cutaction;
	KAction*      p_undoaction;
	KAction*      p_redoaction;
        KAction*      p_pasteaction;
        KAction*      p_replaceaction;
        KAction*      p_findaction;
        KAction*      p_findnextaction;
        KAction*      p_findpreviousaction;
	KAction*      p_selectallaction;
	KAction*      p_storeresultaction;
	KAction*      p_configeditoraction;

	KToggleAction*      p_qbeaction;
        KAction* p_addaction;
        KToggleAction* p_distinctaction;
        KAction* p_qbetypeselectaction;
        QComboBox* p_qbetypeselect;
        void set_caption(void);
	void internal_set_designmode(void);
	void internal_set_viewmode(void);
        static bool p_cancel;
	hk_kdequerypartwidgetprivate* p_private;
static        QProgressDialog* p_progressdialog;
        void before_execute(void);
        void after_execute(void);

};
#endif
