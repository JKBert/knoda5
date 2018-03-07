// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2013 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
//
// This file is part of the hk_kde4classes library
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef HK_KDEPROPERTY_H
#define HK_KDEPROPERTY_H
#include "hk_kdepropertybase.h"
#include <hk_class.h>

class QScrollArea;class QFontDatabase;
class QScrollView;
class hk_kdesimpleform;
class KColorButton;

class hk_kdeproperty : public hk_kdepropertyeditorbase,public hk_class
{
    Q_OBJECT
    friend class hk_kdesimpleform;

        public:
        hk_kdeproperty( hk_kdesimpleform* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
        ~hk_kdeproperty();

    public slots:
        void action_changes();
        void actionobject_changes();
        void data_changes();
        void format_changes();
        void geometry_changes();
        void set_object(hk_visible*);
	hk_visible* object(void);
        void focus_resized(void);
        void set_datasourcelist(void);
	void identifier_changed(void);
    	virtual void tooltipfield_changed();
        virtual void add_formdatasource();
        virtual void add_listformdatasource();
        void datasource_selected();
        void listdatasource_selected();
        void set_actionobjectlist(void);
        void combomode_changed(void);
        void conditionbutton_clicked();
        void pushactionbutton_clicked(int rownumber,const hk_string& warningmessage);
        void pushactionbutton_clicked(void);
        void doubleclickactionbutton_clicked(void);
        void doubleclickactionbutton_clicked(int rownumber,const hk_string& warningmessage);
        void openactionbutton_clicked(int rownumber,const hk_string& warningmessage);
        void openactionbutton_clicked(void);
        void closeactionbutton_clicked(int rownumber,const hk_string& warningmessage);
        void closeactionbutton_clicked(void);
        void beforedeletebutton_clicked(int rownumber,const hk_string& warningmessage);
        void beforedeletebutton_clicked(void);
        void afterdeletebutton_clicked(int rownumber,const hk_string& warningmessage);
        void afterdeletebutton_clicked(void);
        void beforeinsertbutton_clicked(int rownumber,const hk_string& warningmessage);
        void beforeinsertbutton_clicked(void);
        void afterinsertbutton_clicked(int rownumber,const hk_string& warningmessage);
        void afterinsertbutton_clicked(void);
        void beforeupdatebutton_clicked(int rownumber,const hk_string& warningmessage);
        void beforeupdatebutton_clicked(void);
        void afterupdatebutton_clicked(int rownumber,const hk_string& warningmessage);
        void afterupdatebutton_clicked(void);
        void afterrowchangebutton_clicked(int rownumber,const hk_string& warningmessage);
        void afterrowchangebutton_clicked(void);
        void beforerowchangebutton_clicked(int rownumber,const hk_string& warningmessage);
        void beforerowchangebutton_clicked(void);
        void ongetfocusbutton_clicked(int rownumber,const hk_string& warningmessage);
        void ongetfocusbutton_clicked(void);
        void onloosefocusbutton_clicked(int rownumber,const hk_string& warningmessage);
        void onloosefocusbutton_clicked(void);
        void onkeybutton_clicked(int rownumber,const hk_string& warningmessage);
        void onkeybutton_clicked(void);
        void onselectbutton_clicked(int rownumber,const hk_string& warningmessage);
        void onvaluechangedbutton_clicked(int rownumber,const hk_string& warningmessage);
        void onvaluechangedbutton_clicked(void);
    virtual void onselectbutton_clicked();
    virtual void use_textlist_changed();
    virtual void use_textlistbutton_clicked();

    virtual void border_changes();
    virtual void subformbutton_clicked();
    virtual void localimage_selected();
    virtual void has_changed();
    virtual void buttonformat_changed();

    protected:
            bool eventFilter(QObject* object,QEvent* event);


    private:
        hk_visible* p_visible;
// change the propertyfields when hk_visible changes
        void set_visible(void);
	void set_geometry(void);
        void set_dsvisible(void);
        void set_dsdatavisible(void);
        void set_gridvisible(void);
        void set_button(void);
	void set_subform(void);
	void set_labelvisible(void);
	void set_imagevisible(void);

//change values in hk_visible when the property fields changes
        void set_objectvisible();
        void set_objectdsvisible(void);
        void set_objectdsdatavisible(void);
	void set_objectsubform(void);
	void set_objectimagevisible(void);
        void set_objectaction(void);
        void set_viewcolumnvalues(void);
        void use_editor(QLineEdit* e);
        QFontDatabase* p_fontdatabase;
        hk_kdesimpleform* p_form;
        QString  settxt,notsettxt;
	QScrollArea* p_datascrollview;
	QScrollArea* p_formatscrollview;
	QScrollArea* p_framescrollview;
	QScrollArea* p_actionscrollview;


};
#endif                                            // HK_KDEPROPERTY_H
