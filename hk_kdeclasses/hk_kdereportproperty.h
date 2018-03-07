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

#ifndef HK_KDEREPORTPROPERTY_H
#define HK_KDEREPORTPROPERTY_H
#include "hk_kdereportpropertybase.h"
#include <hk_class.h>
class hk_kdesimplereport;
class hk_visible;
class QFontDatabase;
class KColorButton;
class QScrollArea;

class hk_kdereportproperty : public hk_kdereportpropertybase, public hk_class
{
    Q_OBJECT

        public:
        hk_kdereportproperty( hk_kdesimplereport* parent = 0, const char* name = 0,  Qt::WFlags fl = 0 );
        ~hk_kdereportproperty();

    public slots:
        void action_changes();
        void actionobject_changes();
        void add_propertydatasource();
        void data_changes();
        void datasource_selected();
        void format_changes();
        void font_changes();
	void size_changes();
	void position_changes();
        void datasourcebutton_clicked();
        void set_object(hk_visible*);
	hk_visible* object(void);
        void focus_resized(void);
        void subreportbutton_clicked();
	void identifier_changed(void);
        void afterrowchange_action_clicked(int rownumber,const hk_string& warningmessage);
        void afterrowchange_action_clicked(void);
        void beforerowchange_action_clicked(int rownumber,const hk_string& warningmessage);
        void beforerowchange_action_clicked(void);
	void onprint_action_clicked(int rownumber,const hk_string& warningmessage);
	void onprint_action_clicked(void);
        void onprintnewpage_action_clicked(void);
        void onprintnewpage_action_clicked(int rownumber,const hk_string& warningmessage);

    virtual void closeactionbutton_clicked(void);
    virtual void closeactionbutton_clicked(int rownumber,const hk_string& warningmessage);
    virtual void openactionbutton_clicked(void);
    virtual void openactionbutton_clicked(int rownumber,const hk_string& warningmessage);

    
    protected:
            bool eventFilter(QObject* object,QEvent* event);


    private:
        void set_datasourcelist(void);
        void set_objectdsvisible(void);
        void set_objectvisible(void);
        void set_objectfontvisible(void);
        void set_objectsizevisible(void);
        void set_objectpositionvisible(void);
        void set_objectreportvisible(void);
        void set_objectreportsectionvisible(void);
        void set_objectreportdatavisible(void);

        void set_visible(void);
        void set_sizevisible(void);
        void set_fontvisible(void);
        void set_dsvisible(void);
        void set_reportvisible(void);
        void set_reportsectionvisible(void);
        void set_reportdatavisible(void);
	void set_bordervisible(void);
        void use_editor(QLineEdit* e);

        hk_kdesimplereport* p_report;
        hk_visible* p_visible;
        QFontDatabase* p_fontdatabase;
        QComboBox* pageformatfield;
        QComboBox* orientationfield;
        QSpinBox* firstpagenumberfield;
        KColorButton*   p_foregroundcolourbutton;
        KColorButton*   p_backgroundcolourbutton;
        QLabel*   p_foregroundcolourlabel;
        QLabel*   p_backgroundcolourlabel;
	QString  settxt,notsettxt;
	QScrollArea* p_datascrollview;
	QScrollArea* p_formatscrollview;
	QScrollArea* p_framescrollview;
	QScrollArea* p_actionscrollview;
};
#endif                                            // HK_KDEREPORTPROPERTY_H
