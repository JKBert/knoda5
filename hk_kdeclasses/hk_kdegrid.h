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

#ifndef HK_KDEGRID
#define HK_KDEGRID
#include <kde4classesmacros.h>
#include <hk_class.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qframe.h>
#include <hk_dsgrid.h>

class hk_kdesimpleform;
class hk_kdegridpart;
class hk_kde4simplegrid;
class hk_kdegridprivate;
class KMenu;

/**
 *
 *@short  shows data in a grid without columnheaders.
 *@version $Revision: 1.32 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *
 *This widget shows data of a datasource in a grid.
 *If the data is not readonly you can edit the data just by typing
 *in the cell. It has column and row headers and a rowselector.
 *
 *Just set the datasource and enable it. For details how you can use it
 *see @ref hk_kdesimplegrid.
 */

class KDECLASSES_EXPORT hk_kdegrid :public QFrame, public hk_dsgrid
{
    friend class hk_kdegridpart;
    friend class hk_kde4simplegrid;
   
    Q_OBJECT
        public:
        hk_kdegrid(QWidget* w=0,const char* n=0,Qt::WFlags f=0,hk_kdesimpleform* form=NULL);
        virtual ~hk_kdegrid();
/**
 * the datasource which data it has to show
 */
        void    set_datasource(hk_datasource* d);
/**
 * returns the use hk_kdesimplegrid. Please don't delete it !
 */
        hk_kde4simplegrid* simplegrid(void);
        hk_kdegridpart*   part(void);
        using hk_dsgrid::savedata;
virtual void  savedata(ostream& s,bool saveall);
virtual void show_widget(void);
virtual void hide_widget(void);
virtual bool close_widget(void);
        void populate_context_menu(QWidget* owner,KMenu* cmenu );

// needed to fix the issue in hk_dscombobox::value_at 
	hk_string combo_value_at(uint column,ulong row); 


    public slots:
        void    find_clicked(void);
        void    find_next(void);
        void    findargument_changed(void);
        void    print_grid(void);
	void    current_row_changed(void);
	void    current_column_changed(void);
	void    reload(void);
	void    set_column_to_null(void);
	void    load_column_from_file(void);
	void    save_column_to_file(void);
	void    show_gridcolumndialog(void);
	void    delete_selected_rows(void);
	void    delete_current_row(void);

    protected slots:
        void columns_moved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
        void slot_mouse_clicked(void);
        void slot_mouse_doubleclicked(void);
        void slot_simplegrid_focusin(void);
        void slot_simplegrid_focusout(void);
        void slot_simplegrid_focuschanged(int prevRow, int prevCol, int currRow, int currCol);
        
    protected:
        virtual bool  widget_specific_coordinates(unsigned int px,unsigned int py,unsigned int pwidth,unsigned int pheight);
        virtual void  widget_specific_presentationdatasource(long p);
        virtual void  widget_specific_enable_disable(void);
        virtual void  widget_specific_font_changed(void);
	virtual void  widget_specific_insert_mode(void);
	virtual bool  widget_specific_row_change(void);

        virtual void  resizeEvent (QResizeEvent *);
        virtual void  moveEvent ( QMoveEvent * );
        virtual void  before_row_change(void);
        virtual void  widget_specific_backgroundcolour_changed(const hk_colour&);
        virtual void  widget_specific_foregroundcolour_changed(const hk_colour&);
	virtual void  widget_specific_columns_created(void);
	void widget_specific_rowheight_changes(void);
	virtual bool  presentationmode_changed(void);
	bool save_changed();

    private:
	hk_kdegridprivate* p;
	void setpart(hk_kdegridpart* part);
        void    reposition(void);
};

/**
 *@internal
 */

#endif
