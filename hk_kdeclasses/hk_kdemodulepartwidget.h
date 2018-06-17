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

#ifndef HK_KDEMODULEPARTWIDGET_H
#define HK_KDEMODULEPARTWIDGET_H

#include <kde4classesmacros.h>
#include <qvariant.h>
#include <qwidget.h>

#include "hk_datasource.h"
#include "hk_database.h"
#include "hk_module.h"

#include <kaction.h>

class KActionCollection;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QButtonGroup;
class QToolButton;
class hk_kdetoolbar;
class hk_kdemodulepart;

class hk_kdemodulepartwidgetprivate;

/**
 *
 *@short KDE high level widget to define queries and see the result.
 *@version $Revision: 1.2 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *hk_kdemodulepartwidget is a high level widget to define queries and see the result.
 *In this mode of development it has a field to enter the SQL module and a grid which shows the
 * resulting data if there is any. hk_kdemodulepartwidget also handles actionqueries correctly.
 */


class KDECLASSES_EXPORT hk_kdemodulepartwidget : public QWidget ,public hk_module
{
    Q_OBJECT
    friend class hk_kdemodulepart;

        public:
virtual        ~hk_kdemodulepartwidget();

/**
 *when the datasource vanishes (i.e. will be destroyed) and autoclose is true, the hk_kdemodulepartwidget will be
 *closed otherwise it still is on your screen.
 */
        void set_autoclose(bool c);
	bool autoclose(void)const;


virtual	void reset_has_changed(void);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);
        bool save_module(const hk_string& n="",bool ask=true);
        bool load_module(const hk_string& n="");
   bool has_changed();
   void set_block_has_changed(bool);
    signals:
        void signal_closed(QWidget*);
	void signal_captionChanged(QWidget*, const QString&);

    public slots:
        void module_changed(void);
        void savebutton_clicked(void);
        void saveasbutton_clicked(void);
        void close_module(void);
        void print(void);

virtual void show ();
virtual void showMaximized ();


    protected:
        hk_kdemodulepartwidget(hk_kdemodulepart*,QWidget* w=0,const char* n=0,Qt::WFlags f=0);
        virtual void before_source_vanishes(void);
        virtual void closeEvent ( QCloseEvent* e);
        void    keyPressEvent ( QKeyEvent * e );
        virtual void set_has_changed(enum_has_changed forcesetting=standard);

    private:
        KAction*      p_saveaction;
        KAction*      p_saveasaction;
        KAction*      p_printaction;

	KAction*      p_copyaction;
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

        void set_caption(void);
	void setupActions(KActionCollection* ac);
	hk_kdemodulepartwidgetprivate* p_private;

};
#endif
