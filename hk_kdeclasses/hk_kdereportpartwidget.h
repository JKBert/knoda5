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

#ifndef HK_KDEREPORTPARTWIDGET_H
#define HK_KDEREPORTPARTWIDGET_H

#include <kde4classesmacros.h>
#include "hk_class.h"
#include <qwidget.h>
#include "hk_presentation.h"
#include <kaction.h>
#include <kxmlguiwindow.h>

class QButtonGroup;
class QLabel;
class hk_kdesimplereport;
class hk_database;
class hk_kdetoolbar;
class QToolButton;
class QVBoxLayout;
class QHBoxLayout;
class KToggleAction;
class QStackedWidget;
class hk_kdereportpart;
class hk_kdepreviewwindow;
class KActionMenu;

class hk_kdereportpartwidgetscrollview;
/**
 *
 *@short  KDE high level widget to display and design reports
 *@author Horst Knorr (hk_classes@knoda.org)
 *@version $Revision: 1.11 $
 *
 *this is a high level widget to display and design reports. It hereby uses @ref hk_kdesimplereport.
 *To use it set the database with @ref set_database. If you want to show or edit an existing reports
 * load it with @ref load_report.
 */

class KDECLASSES_EXPORT hk_kdereportpartwidget:public KXmlGuiWindow, public hk_class
{
friend class hk_kdereportpart;
    Q_OBJECT

        public:
        virtual    ~hk_kdereportpartwidget();
        bool        set_presentationdatasource(long n,bool r=true);
        long        presentationdatasource(void);
        void set_database(hk_database*);
        hk_database* database(void);
        void set_nodesignmode(bool);
        void set_mode(hk_presentation::enum_mode s);
        hk_presentation::enum_mode  mode(void);
        hk_kdesimplereport* simplereport(void);
        bool load_report(const hk_string& name="");
	bool while_executing(void) const;
	void stop_execution(void);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);



        void set_caller_form(const hk_string&);
        hk_string caller_form(void) const;

    signals:
        void signal_closed(QWidget*);
	void signal_setcaption(const QString&);
        void signal_raise_form(const hk_string&);
	void signal_captionChanged(QWidget*, const QString&);

    public slots:
        void set_designmode(void);
        void set_viewmode(void);
        void    save_report(void);
        void    saveas_report(void);
        void    close_report(void);
        void    slot_showtoolbar(void);
        void    print_report(void);
        void    select_section(void);


    protected:
        hk_kdereportpartwidget( hk_kdereportpart*,QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
        void closeEvent ( QCloseEvent* e);

    protected slots:
        void fieldbutton_clicked(void);
        void pointerbutton_clicked(void);
        void viewbutton_clicked(void);
        void designbutton_clicked(void);
        void field_created(void);
        void bulkfont_clicked(void);
        void bulkforegroundcolour_clicked(void);
        void bulkbackgroundcolour_clicked(void);
	void slot_has_changed();
        void clipboard_changed();
	void slot_focuswidget_changed();


    private:
	KToggleAction* p_pointeraction;
        KToggleAction* p_fieldaction;
        KToggleAction* p_designaction;
        KToggleAction* p_viewaction;
        KAction*  p_printaction;
        KAction*  p_sectionaction;
        KAction*  p_reportpropertyaction;
        KAction*  p_saveaction;
        KAction*  p_saveasaction;
        KActionMenu*   p_bulkaction;
        KAction*       p_bulkfontaction;
        KAction*       p_bulkforegroundcolouraction;
        KAction*       p_bulkbackgroundcolouraction;

	KAction* p_deleteaction;
        KAction* p_copyaction;
        KAction* p_pasteaction;
        KAction* p_cutaction;




        KActionMenu* p_resizeaction;
        KAction* p_minwidthaction;
        KAction* p_maxwidthaction;
        KAction* p_minheightaction;
        KAction* p_maxheightaction;
        KAction* p_minsizeaction;
        KAction* p_maxsizeaction;
        KActionMenu* p_alignaction;
        KAction* p_alignleftaction;
        KAction* p_alignrightaction;
        KAction* p_aligntopaction;
        KAction* p_alignbottomaction;
	hk_string p_callerform;


        void set_reporttoolbar(void);
        hk_kdesimplereport* p_report;
        bool p_nodesignmode;
	QStackedWidget* p_stack;
	hk_kdepreviewwindow*  p_previewwindow;
        hk_kdereportpartwidgetscrollview* p_scrollview;
	hk_kdereportpart* p_reportpart;

        bool p_whilepreview;
        bool p_closewindow;
        void set_caption(void);

};
#endif
