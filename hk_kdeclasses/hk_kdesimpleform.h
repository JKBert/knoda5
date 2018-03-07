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

#ifndef HK_KDESIMPLEFORM_H
#define HK_KDESIMPLEFORM_H
#include <kde4classesmacros.h>
#include "hk_form.h"
#include <qwidget.h>
#include <kparts/partmanager.h>

class hk_kdeformfocus;
class hk_kdeproperty;
class hk_kdeform;
class hk_kdeformpartwidget;
class KMenu;
class KAction;
class KActionMenu;
class hk_kdegridpart;
class hk_kdesimpleformprivate;
class  KActionCollection;

/**
 *
 *@short KDE Widget to display forms.
 *@version $Revision: 1.58 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *hk_kdesimpleform is a KDE widget to display and alter forms.
 *Just set the database with @ref hk_presentation::set_database. Load existing  forms
 *with @ref hk_form::load_form. If @ref hk_presentation::mode is set to 'view' the data will be shown.
 *In mode 'design' you can design your widget.
 *For interactive use it is better to use @ref hk_kdeform because it provides toolbars.
 */

class KDECLASSES_EXPORT hk_kdesimpleform :public QWidget,public hk_form
{
    Q_OBJECT
friend class hk_kdeform;
friend class hk_kdeformpartwidget;
        public:
        hk_kdesimpleform(QWidget* wid,const char* n=0,Qt::WFlags f=0);
        virtual ~hk_kdesimpleform(void);
/**
 * if true the widget will be destroyed when the widget will be closed else just hidden
 */
        void set_autoclose(bool c){p_autoclose=c;}
        virtual bool            set_mode(enum_mode s);
	hk_kdeproperty*  propertyeditor(void);
	hk_kdeform*	   kdeform()const ;
	hk_kdeformpartwidget* formpartwidget() const;
        void set_focus(QWidget*, bool control);
	virtual void script_error(hk_visible*, hk_interpreter::enum_action);
	//bool ctrl_key_pressed(void) const;
        KParts::PartManager* partsmanager()const{return p_partsmanager;}
        bool focus_multipleselected(void) const;
        enum enum_adjust {al,ar,at,ab,sminw,smaxw,sminh,smaxh,smaxs,smins};
virtual	void reset_has_changed(void);
virtual	void goto_taborder_next();
virtual	void goto_taborder_previous();
virtual void goto_taborder_first();
virtual void goto_taborder_last();
virtual void set_focus(hk_visible*);
	void	hide();
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);
        virtual void loaddata(xmlNodePtr definition);
        void set_create_with_label(bool);
        bool create_with_label(void) const;

	signals:
        void mouseclick(const QPoint&);
        void field_created(void);
        void signal_closed(QWidget*);
	void signal_focuswidget_changed(void);
	void signal_has_changed(void);
	void signal_name_changed(void);

    public slots:
        void set_field2create(enum_visibletype);
        hk_kdeformfocus* formfocus(void)const {return p_focus;}
virtual        void show_property(void);
        void set_for_all(enum_bulkoperation);
	void copy(void);
	void paste(void);
	void cut(void);
        void adjust_widgets(enum_adjust);
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
        void delete_widgets(void);
        void copy_widgets(void);
        void paste_widgets(void);
        void cut_widgets(void);
        void enable_actions(void);
	void grid_partinfocus(hk_kdegridpart*);
	void grid_partoutfocus(hk_kdegridpart*,QFocusEvent*);
	void dbdesignaction(void);
	void designer_deleted(void);
        void            load_form(const hk_string& name="");
virtual bool show_asdialog(hk_form* parentform=NULL);
virtual int screen_width();
virtual int screen_height();
virtual bool intervaltasks(void);

    protected:
        virtual void closeEvent ( QCloseEvent* e);
     virtual void focusInEvent ( QFocusEvent * );
     virtual void focusOutEvent ( QFocusEvent * );
     virtual void    widget_specific_tooltip_changed(void);
        virtual bool focusNextPrevChild(bool forward);
        virtual void set_has_changed(enum_has_changed forcesetting=standard);
        void set_kdeform(hk_kdeform*);
	void set_formpartwidget(hk_kdeformpartwidget*);
	void set_actions(KActionCollection*);
	bool has_already_focus(QWidget*);
        virtual bool    widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight);
        virtual void widget_specific_set_name(void);

        virtual hk_dsgrid*      widget_specific_new_grid(void);
        virtual hk_dsrowselector*   widget_specific_new_rowselector(void);
        virtual hk_dslineedit*      widget_specific_new_lineedit(void);
        virtual hk_dsboolean*       widget_specific_new_bool(void);
        virtual hk_dsmemo*      widget_specific_new_memo(void);
        virtual hk_button*      widget_specific_new_button(void);
        virtual hk_dscombobox*      widget_specific_new_combobox(void);
        virtual hk_label*       widget_specific_new_label(void);
	virtual hk_subform*     widget_specific_new_subform(void);
        virtual void            widget_specific_fieldresize(hk_visible* v);
        virtual void            widget_specific_presentationresize(void);
        virtual void            before_source_vanishes(void);
        virtual void            widget_specific_modechanges(enum_mode);
        virtual void            widget_specific_presentationdatasource(long);
        virtual void            widget_specific_after_loadform(void);
        virtual void        widget_specific_backgroundcolour_changed(const hk_colour&);
        virtual void        widget_specific_foregroundcolour_changed(const hk_colour&);
        virtual hk_dsdate*       widget_specific_new_date(void);
        virtual hk_dsimage*      widget_specific_new_image(void);
        virtual hk_tabvisible*   widget_specific_new_tabvisible(void);
	virtual void widget_specific_enabled_changed(void);
        virtual void            database_has_vanished(void);

	virtual void            paintEvent(QPaintEvent*);
        virtual void            mousePressEvent(QMouseEvent*);
        virtual void            mouseReleaseEvent(QMouseEvent*);
        virtual void            mouseDoubleClickEvent(QMouseEvent*);

        virtual void            keyPressEvent ( QKeyEvent * );
        virtual void            keyReleaseEvent ( QKeyEvent * );
        bool eventFilter(QObject* object,QEvent* event);
        bool formeventFilter(QObject* object,QEvent* event);
        void move_widgets(int xdiff,int ydiff);
    protected slots:
    virtual void done(void);
    
    private:


    private slots:

    private:
        bool is_formobject(QWidget*);
        hk_visible* create_field(const QPoint&);
        void clearmultiplefocus(void);
        void clearfocus(void);
        void mousemenu(QMouseEvent*);
        void resize_widget(QWidget*,enum_adjust,int minw,int maxw,int minh,int maxh,int minx,int maxx,int miny,int maxy);
        void repaint_focus(void);
        void create_copydata(ostream&,QWidget*);
        bool p_autoclose;
        bool p_sourcevanishflag;
        enum_visibletype p_field2create;
        hk_kdeformfocus* p_focus;
        list<hk_kdeformfocus*> p_multiplefocus;
        bool p_createflag;
        QPoint p_originalposition;
        //bool p_controlbutton;
        hk_kdeproperty* p_property;
        hk_kdeform* p_kdeform;
	hk_kdeformpartwidget* p_formpartwidget;
        KMenu* p_popup;
        KParts::PartManager* p_partsmanager;
        KAction* p_deleteaction;
        KAction* p_copyaction;
        KAction* p_pasteaction;
        KAction* p_cutaction;
        KAction*       p_formpropertyaction;
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
	KActionMenu* p_setallaction;
	hk_kdesimpleformprivate* p_private;
        void set_currentobject(hk_visible* );
//    KAction* p_resizeaction;
//    KAction* p_moveaction;
};
#endif
