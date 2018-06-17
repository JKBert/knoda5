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

#ifndef HK_KDESIMPLEREPORT_H
#define HK_KDESIMPLEREPORT_H
#include <hk_report.h>
#include <qwidget.h>
#include <list>

class hk_kdereportproperty;
class QVBoxLayout;
class KAction;
class KActionMenu;
class hk_kdeformfocus;
class hk_kdereportsection;
class hk_kdereport;
class hk_kdereportpartwidget;
class QProgressDialog;
class QPopupMenu;
class hk_kdesimplereportprivate;
/**
 *
 *@short KDE Widget to design and execute reports.
 *@version $Revision: 1.33 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *hk_kdesimplereport is a KDE widget to design and execute reports.
 *Just set the database with @ref hk_presentation::set_database. Load existing  reports
 *with @ref hk_report::load_report. hk_kdesimplereport does not include a preview window.
 *In mode 'design' you can design your widget.
 *For interactive use it is better to use @ref hk_kdereport because it provides toolbars and a preview window.
 */

class hk_kdesimplereport:public QWidget, public hk_report
{
friend class hk_kdereport;
friend class hk_kdereportpartwidget;
    Q_OBJECT

        public:
        hk_kdesimplereport( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
        virtual    ~hk_kdesimplereport();

        void set_autoclose(bool c);
        void set_field2create(bool);
        bool field2create(void);
        void set_focus(QWidget*,hk_kdereportsection*,bool multiple);
        void clearfocus(void);
        hk_kdeformfocus* focus(void);
        hk_kdeformfocus* formfocus(void);
        virtual bool load_report(const hk_string& name="");
	virtual bool set_mode(enum_mode);
	hk_kdereport* kdereport(void);
	virtual void script_error(hk_visible*, hk_interpreter::enum_action);
	//bool ctrl_key_pressed(void) const;
        bool focus_multipleselected(void) const;
        bool canProcessClipboard() const;
virtual	void reset_has_changed(void);
	hk_kdereportpartwidget* reportpartwidget() const;

        signals:
        void signalfield_created(void);
	void signal_has_changed(void);
	void signal_focuswidget_changed(void);

    public slots:
        void field_created(void);
        void set_focus2property(void);
        void set_sections(void);
        void show_property(void);
        void set_for_all(enum_bulkoperation);

    protected:
         virtual void set_has_changed(enum_has_changed forcesetting=standard);

        virtual hk_reportsection* widget_specific_new_section(void);
        virtual void keyPressEvent ( QKeyEvent * event );
        virtual void keyReleaseEvent ( QKeyEvent * event);
        virtual void widget_specific_after_loadreport(void);
        virtual bool widget_specific_before_modechanges(enum_mode);
        bool eventFilter(QObject* object,QEvent* event);

        bool reporteventFilter(QObject* object,QEvent* event);

        virtual void before_source_vanishes(void);
        virtual void database_has_vanished(void);
        virtual void widget_specific_modechanges(enum_mode s);
        virtual void    widget_specific_before_execute(void);
        virtual void    widget_specific_after_execute(void);
        virtual bool widget_specific_after_new_page(void);
        virtual void widget_specific_presentationresize(void);
        virtual void widget_specific_backgroundcolour_changed(const hk_colour&);
        virtual void widget_specific_foregroundcolour_changed(const hk_colour&);
        virtual void            widget_specific_fieldresize(hk_visible* v);

        void mousemenu(QMouseEvent* m);
        enum enum_adjust {al,ar,at,ab,sminw,smaxw,sminh,smaxh,smaxs,smins};
        void adjust_widgets(enum_adjust);
        void resize_widget(QWidget*,enum_adjust,int minw,int maxw,int minh,int maxh,int minx,int maxx,int miny,int maxy);
        void repaint_focus(void);

    protected slots:
        void printing_cancelled(void);
        void delete_widgets(void);
        void copy_widgets(void);
        void paste_widgets(void);
        void cut_widgets(void);
        void enable_actions(void);
    
    private slots:
        void align_left(void);
        void align_right(void);
        void align_top(void);
        void align_bottom(void);
        void adjust_minw(void);
        void adjust_maxw(void);
        void adjust_minh(void);
        void adjust_maxh(void);
        void adjust_mins(void);
        void adjust_maxs(void);

	
	
    protected:
            void set_kdereport(hk_kdereport*);
	    void set_reportpartwidget(hk_kdereportpartwidget*);

	
    private:
        bool is_reportobject(QWidget*);
        void clearmultiplefocus(void);
        //void create_propertyeditor(void);
        void set_actualsection(hk_kdereportsection*);
        void set_propertyobject(QWidget*);
        void create_copydata(ostream&,QWidget*);
        void set_sectionsize(hk_reportsection*);
	hk_kdesimplereportprivate* p_private;
};
#endif
