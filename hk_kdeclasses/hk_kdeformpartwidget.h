// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
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

#ifndef HK_KDEFORMPARTWIDGET_H
#define HK_KDEFORMPARTWIDGET_H
#include <kde4classesmacros.h>
#include <hk_form.h>
#include <qvariant.h>
#include <qwidget.h>
#include <hk_presentation.h>
#include <hk_class.h>
#include <kaction.h>
#include <kxmlguiwindow.h>


class QScrollArea;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class hk_kdesimpleform;
class QToolButton;
class QButtonGroup;
class KToggleAction;
class KActionMenu;
class hk_kderowselector;
class hk_kdetoolbar;
class hk_kdeproperty;
class hk_kdegridpart;
class hk_kdeformpart;

/**
 *
 *@short  KDE high level widget to display and design forms
 *@author Horst Knorr (hk_classes@knoda.org)
 *@version $Revision: 1.19 $
 *
 *this is a high level widget to display and design forms. It hereby uses @ref hk_kdesimpleform.
 *To use it set the database with @ref set_database. If you want to show or edit an existing form
 * load it with @ref load_form.
 */

class KDECLASSES_EXPORT hk_kdeformpartwidget : public KXmlGuiWindow,public hk_class
{
    Q_OBJECT
     friend class hk_kdesimpleform;
     friend class hk_kdeformpart;
        public:
        virtual    ~hk_kdeformpartwidget();

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
        void        loaddata(xmlNodePtr definition);
        void        savedata(ofstream& s);
        void        load_form(const hk_string& n);
	hk_kdeformpart* formpart(void) const{return p_formpart;}
/**
 *when the datasource vanishes (i.e. will be destroyed) and autoclose is true, the hk_kdetable will be
 *closed otherwise it still is on your screen.
 */

        void set_autoclose(bool c);
/**
 *if the designwindow should not be shown set this true. Default is false.
 */
        void set_nodesignmode(bool n);
	hk_kdeproperty* propertyeditor(void);
	void save_form_when_changed(void);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);
        void set_caller_form(const hk_string&);
        hk_string caller_form(void) const;

    signals:
        void signal_closed(QWidget*);
        void signal_raise_form(const hk_string&);
        void signal_captionChanged(QWidget*, const QString&);

    public slots:
            void set_caption(void);


    protected:
        hk_kdeformpartwidget( hk_kdeformpart* ,QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
        virtual void closeEvent ( QCloseEvent* e);
        virtual void        mousePressEvent(QMouseEvent*);
    protected slots:
        void formbutton_clicked(void);
        void close_form(void);
        void designbutton_clicked(void);
        void fieldbutton_clicked(void);
        void field_created(void);
        void save_form(void);
        void saveas_form(void);
        void bulkfont_clicked(void);
        void bulkforegroundcolour_clicked(void);
        void bulkbackgroundcolour_clicked(void);
	void grid_partinfocus(hk_kdegridpart*);
	void grid_partoutfocus(hk_kdegridpart*);
	void slot_focuswidget_changed();
	void slot_has_changed();
	void clipboard_changed();
	void taborder_clicked();
	void withlabelbutton_clicked();
	void show_gridcolumndialog();
	void filterdefinebutton_clicked();
	void clear_filter();
	void reload_form();
    private:
        KToggleAction* p_designaction;
        KToggleAction* p_viewaction;
        KAction* p_saveaction;
        KAction* p_saveasaction;

        KToggleAction*  p_pointeraction;
        KToggleAction*  p_lineeditaction;
        KToggleAction*  p_labelaction;
        KToggleAction*  p_multilineeditaction;
        KToggleAction*  p_booleanaction;
        KToggleAction*  p_comboboxaction;
        KToggleAction*  p_gridaction;
        KToggleAction*  p_selectoraction;
        KToggleAction*  p_buttonaction;
        KToggleAction*  p_subformaction;
        KToggleAction*  p_imageaction;
        KToggleAction*  p_tabwidgetaction;
        KToggleAction*  p_dateaction;
        KToggleAction*       p_filterdefinitionaction;
        KToggleAction* p_filterexecaction;
        KToggleAction* p_withlabelaction;
        KAction*       p_clearfilteraction;
        KActionMenu*   p_bulkaction;
        KAction*       p_bulkfontaction;
        KAction*       p_bulkforegroundcolouraction;
        KAction*       p_bulkbackgroundcolouraction;
        KAction* p_deleteaction;
        KAction* p_copyaction;
        KAction* p_pasteaction;
        KAction* p_cutaction;
        KAction* p_reloadaction;
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
        KAction* p_dbdesigneraction;
	KAction* p_taborderaction;
	KAction* p_definegridcolumnaction;
	hk_kdeformpart* p_formpart;
	hk_string p_callerform;

        hk_kdetoolbar* toolbar;
        hk_kdesimpleform* p_form;
        hk_kderowselector* p_rowselector;
        QScrollArea* p_scrollview;
        bool p_nodesignmode;
        bool p_dontclose;
};
#endif
