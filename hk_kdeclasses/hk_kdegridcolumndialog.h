// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2014 Patrik Hanak <hanakp@users.sourceforge.net>
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

#ifndef HK_KDEGRIDCOLUMNDIALOG_H
#define HK_KDEGRIDCOLUMNDIALOG_H
#include "hk_kdegridcolumndialogbase.h"
#include <hk_string.h>
#include <list>
#include <hk_definitions.h>
#include <hk_dsgridcolumn.h>

class QTreeWidgetItem;
class hk_kdegrid;
/**
 *
 *@internal
 *@version $Revision: 1.19 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *
 */

class hk_kdegridcolumndialog : public hk_kdegridcolumndialogbase
{
    Q_OBJECT

  public:
    hk_kdegridcolumndialog( hk_kdegrid* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdegridcolumndialog();

  public slots:
    void addbutton_clicked();
    void deletebutton_clicked();
    void exitbutton_clicked();
    void okbutton_clicked();
    void defaultvalue_changed();
    void columntype_changed();
    void rightbox_selectionchanged(QTreeWidgetItem*);
    void alignment_changed();
    void displayname_changed();
    void identifier_changed();
    void listcolumn_changed();
    void listdatasource_changed();
    void listdatasourcetype_changed();
    void digits_changed();
    void readonly_changed();
    void separator_changed();
    void viewcolumn_changed();
    virtual void onclickaction_clicked();
    virtual void ondoubleclickaction_clicked();
    virtual void onopenaction_clicked();
    virtual void oncloseaction_clicked();
    virtual void beforeupdateaction_clicked();
    virtual void afterupdateaction_clicked();
    virtual void beforerowchangeaction_changed();
    virtual void afterrowchangeaction_changed();
    virtual void beforedeleteaction_clicked();
    virtual void afterdeleteaction_clicked();
    virtual void beforeinsertaction_clicked();
    virtual void afterinsertaction_clicked();
    virtual void on_getfocusaction_clicked();
    virtual void on_loosefocusaction_clicked();
    virtual void onkeybutton_clicked();
    virtual void onselectaction_clicked();

  private:
    typedef  class  fieldstruct
    {
      public:
        fieldstruct(void)
        {
           use_separators=false;
           readonly=false;
           listdatasourcetype=dt_table;
           digits=-1;
           columntype=hk_dsgridcolumn::columnedit;
           alignment=hk_dsdatavisible::alignleft;
         }
         hk_string name;
         hk_string defaultvalue;
         hk_string identifier;
         hk_dsdatavisible::alignmenttype alignment;
         bool use_separators;
         int digits;
         datasourcetype listdatasourcetype;
         bool readonly;
         hk_string listdatasource;
         hk_string listcolumn;
         hk_string viewcolumn;
         hk_string displayname;
         hk_dsgridcolumn::enum_columntype columntype;
		 hk_string p_before_row_change_action;
		 hk_string p_after_row_change_action;
		 hk_string p_after_delete_action,p_before_delete_action;
		 hk_string p_after_update_action,p_before_update_action;
	     hk_string p_after_insert_action,p_before_insert_action;
		 hk_string p_clickaction,p_doubleclickaction, p_onopenaction,p_oncloseaction;
		 hk_string p_on_getfocus_action,p_on_loosefocus_action,p_on_key_action,p_on_select_action;
      };
	void set_buttontext(void);
    list<fieldstruct> fields;
    void listentries_changed(void);
    void load_presentationdatasources(void);
    void set_columnvalues(void);
    void reload_listdatasource(void);
    void set_currentlistdatasource(void);
    hk_kdegrid* p_grid;
    list<fieldstruct>::iterator p_currentfieldit;
	QString     settxt, notsettxt;
};
#endif                                            // HK_KDEGRIDCOLUMNDIALOG_H
