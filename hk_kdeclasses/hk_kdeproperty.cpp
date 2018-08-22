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

#include "hk_kdeproperty.h"
#include "hk_visible.h"
#include "hk_dsvisible.h"
#include "hk_dsdatavisible.h"
#include "hk_kdeformdatasourcedialog.h"
#include "hk_kdesimpleform.h"
#include "hk_kdespinbox.h"
#include "hk_kdelabel.h"
#include "hk_kdereportconditiondialog.h"
#include "hk_kdeinterpreterdialog.h"
#include "hk_kdesubformdialog.h"
#include <hk_database.h>
#include <hk_colour.h>
#include <hk_button.h>
#include "hk_kdesubform.h"
#include "hk_kdefilterdialog.h"
#include "hk_kdecomboboxtextlist.h"
#include "hk_kdeimage.h"

#include <qcombobox.h>
#include <qspinbox.h>
#include <qfontdatabase.h>
#include <qlineedit.h>
#include <qsizepolicy.h>
#include <qtextedit.h>
#include <QKeyEvent>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qtooltip.h>
#include <qlayout.h>
#include <qscrollarea.h>
#include <qcolor.h>
#include <hk_datasource.h>
#include <hk_dscombobox.h>
#include <klocale.h>
#include <kcolorbutton.h>
#include <kfiledialog.h>
#include <kdebug.h>

class visibleuploadimp: public uploadcodeiface
{
public:
    visibleuploadimp(hk_visible& pv, void (hk_visible::*p_sf) (const hk_string&, bool, bool))
      :p_visible(pv), p_visiblesavefn(p_sf) {};
    virtual void upload_text(const hk_string& code) const
    {
        (p_visible.*p_visiblesavefn)(code,true, true);
    }
    virtual const QString& get_action_text(void) const
    {
        return uploadactiontext;
    }
    virtual ~visibleuploadimp() {};
    
protected:
    static QString uploadactiontext;
    hk_visible& p_visible;
    void (hk_visible::*p_visiblesavefn) (const hk_string&, bool, bool);
};

class dsvisibleuploadimp: public visibleuploadimp
{
public:
    dsvisibleuploadimp(hk_dsvisible& pv, void (hk_dsvisible::*p_sf) (const hk_string&, bool, bool))
      :visibleuploadimp(dynamic_cast<hk_visible&>(pv),NULL), p_dsvisiblesavefn(p_sf) {};
    virtual void upload_text(const hk_string& code) const
    {
        ((dynamic_cast<hk_dsvisible&>(p_visible)).*p_dsvisiblesavefn)(code,true, true);
    }
    virtual ~dsvisibleuploadimp() {};
    
protected:
    void (hk_dsvisible::*p_dsvisiblesavefn) (const hk_string&, bool, bool);
};

QString visibleuploadimp::uploadactiontext(i18n("Upload to the form"));

/*
 *  Constructs a hk_kdeproperty which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdeproperty::hk_kdeproperty( hk_kdesimpleform* parent,  const char* name, Qt::WFlags fl )
: hk_kdepropertyeditorbase( parent, name,  fl ), hk_class()
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkdebug("hk_kdeproperty::hk_kdeproperty");
#endif
    combomodefield->setEnabled(true);

    tab->removeTab(tab->indexOf(tab_2));

    p_datascrollview=new QScrollArea(tab);
    p_datascrollview->setWidgetResizable(true);
    tab->addTab(p_datascrollview, i18n("&Data"));
    p_datascrollview->setWidget(tab_2);
    tab_2 -> setParent(p_datascrollview);

    tab->removeTab(tab->indexOf(tab_3));

    p_formatscrollview=new QScrollArea(tab);
    p_formatscrollview->setWidgetResizable(true);
    tab->addTab(p_formatscrollview, i18n("&Format"));
    p_formatscrollview->setWidget(tab_3);
    tab_3->setParent(p_formatscrollview);

    tab->removeTab(tab->indexOf(frametab));

    p_framescrollview=new QScrollArea(tab);
    tab->addTab(p_framescrollview, i18n("F&rame"));
    p_framescrollview->setWidget(frametab);
    frametab->setParent(p_framescrollview);    

    tab->removeTab(tab->indexOf(tab_4));

    p_actionscrollview=new QScrollArea(tab);
    p_actionscrollview->setWidgetResizable(true);
    tab->addTab(p_actionscrollview, i18n("&Action"));
    p_actionscrollview->setWidget(tab_4);
    tab_4->setParent(p_actionscrollview);
    
    QString no=i18n("no");
    QString yes=i18n("yes");
    p_form=parent;
    p_visible=NULL;
    widthfield->setRange(0,10000);
    heightfield->setRange(0,10000);
    xfield->setRange(0,10000);
    yfield->setRange(0,10000);
    p_fontdatabase = new QFontDatabase();
    fontfield->insertItems(fontfield->count(),p_fontdatabase->families());
    actionfield->addItem("");
    actionfield->addItem(i18n("Open form"));
    actionfield->addItem(i18n("Close form"));
    actionfield->addItem(i18n("Open table"));
    actionfield->addItem(i18n("Open query"));
    actionfield->addItem(i18n("Preview report"));
    actionfield->addItem(i18n("Print report"));
    actionfield->addItem(i18n("Goto first row"));
    actionfield->addItem(i18n("Goto last row"));
    actionfield->addItem(i18n("Goto next row"));
    actionfield->addItem(i18n("Goto previous row"));
    actionfield->addItem(i18n("Insert row"));
    actionfield->addItem(i18n("Delete row"));
    actionfield->addItem(i18n("Store row"));
    actionfield->addItem(i18n("Execute actionquery"));
    actionfield->addItem(i18n("Close application"));
    actionfield->addItem(i18n("Open view"));

    actionfield->setCurrentIndex(0); 
    labellabel->setText(i18n("Label:"));
    xlabel->setText(i18n("X:"));
    ylabel->setText(i18n("Y:"));
    widthlabel->setText(i18n("Width:"));
    heightlabel->setText(i18n("Height:"));
    fontlabel->setText(i18n("Font:"));
    fontsizelabel->setText(i18n("Fontsize:"));
    datasourcelabel->setText(i18n("Datasource:"));
    columnlabel->setText(i18n("Column:"));
    actionlabel->setText(i18n("Action:"));
    objectlabel->setText(i18n("Object:"));
    defaultlabel->setText(i18n("Default value:"));
    digitfield->setRange(-1,10);
    listdatasourcefield->setEnabled(true);
    listdatasourcelabel->setEnabled(true);
    listdatasourcebutton->setEnabled(true);

    listdatasourcelabel->setText(i18n("Listdatasource:"));
    listcolumnlabel->setText(i18n("Listcolumn:"));
    viewcolumnlabel->setText(i18n("Viewcolumn:"));

    showmaximizedlabel->setText(i18n("show maximized:"));
    showmaximizedfield->addItem(no);
    showmaximizedfield->addItem(yes);
    separatorfield->addItem(yes);
    separatorfield->addItem(no);
    usetextlistfield->addItem(yes);
    usetextlistfield->addItem(no);
    usetextlistfield->setEnabled(true);
    usetextlistbutton->setEnabled(true);
    readonlyfield->addItem(yes);
    readonlyfield->addItem(no);
    enabledfield->addItem(yes);
    enabledfield->addItem(no);
    istogglebuttonfield->addItem(yes);
    istogglebuttonfield->addItem(no);
    togglepushedfield->addItem(yes);
    togglepushedfield->addItem(no);

    alignmentlabel->setText(i18n("Alignment:"));
    alignmentfield->addItem(i18n("Left"));
    alignmentfield->addItem(i18n("Center"));
    alignmentfield->addItem(i18n("Right"));
    alignmentfield->addItem(i18n("Default"));
    connect( alignmentfield, SIGNAL( activated(const QString&) ), this, SLOT( format_changes() ) );
    connect( alignmentfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
    digitlabel->setText(i18n("Precision:"));
    separatorlabel->setText(i18n("Separator:"));
    boldfield->addItem(yes);
    boldfield->addItem(no);
    italicfield->addItem(yes);
    italicfield->addItem(no);
    boldlabel->setText(i18n("Bold:"));
    italiclabel->setText(i18n("Italic:"));
    p_foregroundcolourlabel->setText( i18n( "Foregroundcolour" ) );
    p_backgroundcolourlabel->setText( i18n( "Backgroundcolour" ) );
    toplinefield->set_displaytype(hk_kdespinbox::pixels);
    bottomlinefield->set_displaytype(hk_kdespinbox::pixels);
    leftlinefield->set_displaytype(hk_kdespinbox::pixels);
    rightlinefield->set_displaytype(hk_kdespinbox::pixels);
    diagonallurofield->set_displaytype(hk_kdespinbox::pixels);
    diagonallorufield->set_displaytype(hk_kdespinbox::pixels);
    toplinefield->setSingleStep(1);
    bottomlinefield->setSingleStep(1);
    leftlinefield->setSingleStep(1);
    rightlinefield->setSingleStep(1);
    diagonallurofield->setSingleStep(1);
    diagonallorufield->setSingleStep(1);

    connect( p_foregroundcolourbutton, SIGNAL( changed(const QColor&) ), this, SLOT( format_changes() ) );
    connect( p_backgroundcolourbutton, SIGNAL( changed(const QColor&) ), this, SLOT( format_changes() ) );
    settxt=i18n("set");
    notsettxt=i18n("not set");

    labelfield->installEventFilter(this);
    defaultfield->installEventFilter(this);
    identifierfield->installEventFilter(this);
    tooltipfield->installEventFilter(this);
    QString info=i18n("Press <F2> for a larger editor window");
    labelfield->setToolTip(info);
    defaultfield->setToolTip(info);
    identifierfield->setToolTip(info);
    tooltipfield->setToolTip(info);

    QSize s=tab_3->geometry().size();
    if (frametab->width()>s.width())s.setWidth(frametab->width());
    if (tab_2->width()>s.width())s.setWidth(tab_2->width());
    if (tab_4->width()>s.width())s.setWidth(tab_4->width());

    if (frametab->height()>s.height())s.setHeight(frametab->height());
    if (tab_2->height()>s.height())s.setHeight(tab_2->height());
    if (tab_4->height()>s.height())s.setHeight(tab_4->height());

    QSizePolicy sp=QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    frametab->setSizePolicy(sp);
    tab_2->setSizePolicy(sp);
    tab_3->setSizePolicy(sp);
    tab_4->setSizePolicy(sp);
    frametab->setMinimumSize(s);
    tab_2->setMinimumSize(s);
    tab_3->setMinimumSize(s);
    tab_4->setMinimumSize(s);
}



/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeproperty::~hk_kdeproperty()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::~hk_kdeproperty");
#endif
    delete  p_fontdatabase;
}


/*
 * public slot
 */
void hk_kdeproperty::action_changes()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::action_changes()");
#endif
//cerr <<"action_changes"<<endl;
    if (p_form==NULL) return;
    if (p_form->database()==NULL) return;
    set_actionobjectlist();
    set_objectaction();
}


void hk_kdeproperty::set_actionobjectlist(void)
{
    objectfield->clear();
    int i = actionfield->currentIndex()-1; //cerr <<"set_actionobjectlist i="<<i<<endl;
    vector<hk_string>* liste=NULL;
    objectfield->addItem("");
    if (i==2)  liste=p_form->database()->tablelist();
    else
    if (i==3||i==13)  liste=p_form->database()->querylist();
        else
        if (i==4||i==5)  liste=p_form->database()->reportlist();
            else
            if (i==0||i==1)  liste=p_form->database()->formlist();
                if (liste==NULL) return;
    vector<hk_string>::iterator it =  liste->begin();
    while (it!=liste->end())
    {
        objectfield->addItem(QString::fromUtf8 (l2u((*it)).c_str()));
        it++;
    }
    objectfield->setCurrentIndex(0);

}


void hk_kdeproperty::actionobject_changes()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::action_changes()");
#endif
    if (p_visible==NULL) return;
    if (p_visible->type()==hk_visible::button) set_objectaction();
}


/*
 * public slot
 */
void hk_kdeproperty::data_changes()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::data_changes()");
#endif

    if (p_visible==NULL) return;
    switch (p_visible->type())
    {
        case hk_visible::lineedit:;
        case hk_visible::image:;
        case hk_visible::boolean:;
        case hk_visible::memo:;
        case hk_visible::combobox:;
        case hk_visible::date:;
        set_objectdsdatavisible();
        case hk_visible::subform:
				set_objectsubform();
        case hk_visible::form:
        case hk_visible::grid:
        case hk_visible::button:
        case hk_visible::rowselector:
            set_objectdsvisible();
	    break;

        default:;

    }
}


void hk_kdeproperty::identifier_changed(void)
{
if (!p_visible) return;
hk_string t=u2l(identifierfield->text().toUtf8().data());
if (p_visible->identifier()!=t) p_visible->set_identifier(t);


}

void hk_kdeproperty::tooltipfield_changed()
{
hk_string t=u2l(tooltipfield->text().toUtf8().data());
if (p_visible->tooltip()!=t) p_visible->set_tooltip(t);

}


/*
 * public slot
 */
void hk_kdeproperty::format_changes()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::format_changes()");
#endif
    set_objectvisible();
}

void hk_kdeproperty::geometry_changes()
{
    if (!p_visible) return;
    if (p_visible->type()==hk_visible::form)
    {
        hk_form* f=dynamic_cast <hk_form*>(p_visible);
        if (f!=NULL)
        {
            f->set_designsize(widthfield->value(),heightfield->value());
        }
    }
    else
    {
	p_visible->set_size(xfield->value(),yfield->value(),widthfield->value(),heightfield->value());

    }

   if (p_visible->type()==hk_visible::image)set_objectimagevisible();
}

/*
 * public slot
 */
void hk_kdeproperty::set_object(hk_visible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_object");
#endif
    if (p_visible)
      identifier_changed();

    p_visible=v;

    if (p_visible)
    {
      QString num;
      num.setNum(p_visible->presentationnumber());
      idfield->setText(num);
    }

    tab_2->setEnabled(false);
    tab_3->setEnabled(false);
    tab_4->setEnabled(true);
    actionfield->hide();
    actionlabel->hide();
    localimagelabel->hide();
    localimagebutton->hide();
    localimagebutton->setText(notsettxt);

    xfield->show();
    xlabel->show();
    yfield->show();
    ylabel->show();
    scalelabel->hide();
    scalefield->hide();
    fontfield->show();
    fontlabel->show();
    fontsizefield->show();
    onvaluechangedactionbutton->hide();
    onvaluechangedactionlabel->hide();
    boldfield->show();
    italicfield->show();
    listdatasourcebutton->hide();
    conditionbutton->hide();
    usetextlistbutton->hide();
   istogglebuttonfield->hide();
   togglepushedfield->hide();
   istogglebuttonlabel->hide();
   togglepushedlabel->hide();

	frametab->setEnabled(false);
    if (p_visible==NULL) return;
    QString head;
    head.setNum(p_visible->presentationnumber());

    setWindowTitle(i18n("Property editor (")+head+")");
    tab_3->setEnabled(true);
    datasourcefield->show();
    datasourcebutton->show();
    datasourcelabel->show();
    readonlyfield->show();
    readonlylabel->show();
    columnfield->hide();
    columnlabel->hide();
    defaultfield->hide();
    defaultlabel->hide();
    listcolumnfield->hide();
    listcolumnlabel->hide();
    viewcolumnfield->hide();
    viewcolumnlabel->hide();
    combomodefield->hide();
    combomodelabel->hide();
    listdatasourcefield->hide();
    listdatasourcelabel->hide();
    listdatasourcebutton->hide();
    usetextlistfield->hide();
    usetextlistlabel->hide();
    separatorfield->hide();
    separatorlabel->hide();
    alignmentfield->hide();
    alignmentlabel->hide();
    digitfield->hide();
    digitlabel->hide();
    widthlabel->setText(i18n("Width:"));
    heightlabel->setText(i18n("Height:"));
    if (p_form->sizetype()==hk_presentation::relative)
    {
     widthfield->set_displaytype(hk_kdespinbox::percent);
     heightfield->set_displaytype(hk_kdespinbox::percent);
     xfield->set_displaytype(hk_kdespinbox::percent);
     yfield->set_displaytype(hk_kdespinbox::percent);
    }
    else
    {
     widthfield->set_displaytype(hk_kdespinbox::pixels);
     heightfield->set_displaytype(hk_kdespinbox::pixels);
     xfield->set_displaytype(hk_kdespinbox::pixels);
     yfield->set_displaytype(hk_kdespinbox::pixels);
    }
    beforeupdateactionbutton->hide();
    beforeupdateactionlabel->hide();
    afterupdateactionbutton->hide();
    afterupdatelabel->hide();
    beforerowchangeactionbutton->hide();
    beforerowchangelabel->hide();
    afterrowchangeactionbutton->hide();
    afterrowchangelabel->hide();
    beforedeleteactionbutton->hide();
    beforedeletelabel->hide();
    afterdeleteactionbutton->hide();
    afterdeletelabel->hide();
    beforeinsertactionbutton->hide();
    beforeinsertlabel->hide();
    afterinsertactionbutton->hide();
    afterinsertlabel->hide();
    onselectactionbutton->hide();
    onselectlabel->hide();
    subformfield->hide();
    subformlabel->hide();
    subformbutton->hide();

    defaultlabel->setText(i18n("Default value:"));

    set_visible();

    switch (p_visible->type())
    {
        case hk_visible::grid:  set_dsvisible();
        			tab_2->setEnabled(true);
        			set_gridvisible();
        			break;
        case hk_visible::rowselector: set_dsvisible();
        			tab_2->setEnabled(true);
        			break;
        case hk_visible::combobox:    listcolumnfield->show();
        			listcolumnlabel->show();
        			viewcolumnfield->show();
        			viewcolumnlabel->show();
        			if (usetextlistfield->currentIndex()==0)
        			  {
        			   listdatasourcefield->show();
           			   listdatasourcelabel->show();
           			   listdatasourcebutton->show();
           			   listcolumnfield->show();
           			   listcolumnlabel->show();
          			  }
        			onselectactionbutton->show();
        			onselectlabel->show();
	    			combomodelabel->setText(i18n("Combomode:"));
            			combomodefield->blockSignals(true);
	    			combomodefield->clear();
	    			combomodefield->addItem(i18n("Combo"));
	    			combomodefield->addItem(i18n("Combo (nonwritable)"));
            			combomodefield->addItem(i18n("Selector"));
            			combomodefield->blockSignals(false);
	    			usetextlistfield->show();
	    			usetextlistlabel->show();
            			use_textlist_changed();


        			combomodefield->show();
        			combomodelabel->show();
        case hk_visible::lineedit:;
        case hk_visible::memo:        alignmentfield->show();
        			      alignmentlabel->show();
        			digitfield->show();
        			digitlabel->show();
        			separatorfield->show();
        			separatorlabel->show();

        case hk_visible::date:
	case hk_visible::boolean:;
        			defaultfield->show();
        			defaultlabel->show();
        			columnfield->show();
        			columnlabel->show();
        			set_dsvisible();
        			set_dsdatavisible();
        			tab_2->setEnabled(true);

        			break;

	case hk_visible::subform:
	                subformfield->show();
	                subformlabel->show();
	                subformbutton->show();
			set_dsvisible();
			set_subform();
			tab_2->setEnabled(true);
			break;

        case  hk_visible::form:
            		{combomodefield->show();
             		combomodelabel->show();
	    		combomodefield->blockSignals(true);
	    		combomodefield->clear();
	    		combomodefield->addItem(i18n("relative"));
	    		combomodefield->addItem(i18n("absolute"));
	    		combomodelabel->setText(i18n("Sizetype:"));

	    		combomodefield->setCurrentIndex((p_form->sizetype()==hk_presentation::relative?0:1));
	    		combomodefield->blockSignals(false);

	    		combomodefield->blockSignals(false);
            		set_dsvisible();
            		readonlyfield->hide();
            		readonlylabel->hide();
            		widthlabel->setText(i18n("Form width:"));
            		heightlabel->setText(i18n("Form height:"));
            		xfield->hide();
            		xlabel->hide();
//fontfield->setEnabled(false);
//fontsizefield->setEnabled(false);
            		yfield->hide();
            		ylabel->hide();
	    		widthfield->set_displaytype(hk_kdespinbox::pixels);
     	    		heightfield->set_displaytype(hk_kdespinbox::pixels);
	    		tab_2->setEnabled(true);
            		break;
	    		}
        case  hk_visible::button:     set_button();
        		set_dsvisible();
        		tab_2->setEnabled(true);
        //tab_4->setEnabled(true);
			actionfield->show();
			actionlabel->show();
        		readonlyfield->hide();
        		readonlylabel->hide();
        		localimagelabel->setText(i18n("Icon:"));
        		localimagelabel->show();
    			localimagebutton->show();

        		break;

	case  hk_visible::textlabel:  tab_2->setEnabled(true);
        		datasourcefield->hide();
        		datasourcebutton->hide();
        		datasourcelabel->hide();
        		alignmentfield->show();
        		alignmentlabel->show();
        		columnfield->hide();
        		columnlabel->hide();
        		labelfield->show();
        		labellabel->show();
        		defaultfield->hide();
        		defaultlabel->hide();
        		listdatasourcefield->hide();
        		listdatasourcelabel->hide();
        		listdatasourcebutton->hide();
        		readonlyfield->hide();
        		readonlylabel->hide();
			frametab->setEnabled(true);
			set_labelvisible();
        		break;
        case hk_visible::image:
        		scalelabel->show();
        		scalefield->show();
        		localimagelabel->setText(i18n("Local image:"));
        		localimagelabel->show();
    			localimagebutton->show();
        		set_dsvisible();
        		set_dsdatavisible();
        		columnfield->show();
        		columnlabel->show();
        		tab_2->setEnabled(true);
        		defaultfield->show();
        		defaultlabel->show();
        		defaultlabel->setText(i18n("Image path:"));
        		set_imagevisible();

        	break;
        default:;

    }
}


void hk_kdeproperty::focus_resized(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::focus_resized");
#endif
    set_geometry();
}


void hk_kdeproperty::set_visible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_visible");
#endif
   if (p_visible==NULL)return;
   set_geometry();
     hk_font f=p_visible->font();
    fontsizefield->blockSignals(true);
    fontsizefield->setValue(f.fontsize());
    fontsizefield->blockSignals(false);
    fontfield->blockSignals(true);
    fontfield->setEditText(QString::fromUtf8 (l2u(f.fontname()).c_str()));
    fontfield->blockSignals(false);
    boldfield->blockSignals(true);
    boldfield->setCurrentIndex(f.bold()?0:1);
    boldfield->blockSignals(false);
    italicfield->blockSignals(true);
    italicfield->setCurrentIndex(f.italic()?0:1);
    italicfield->blockSignals(false);
    labelfield->blockSignals(true);
    labelfield->setText(QString::fromUtf8 (l2u(p_visible->label()).c_str()));
    labelfield->blockSignals(false);
    identifierfield->blockSignals(true);
    identifierfield->setText(QString::fromUtf8 (l2u(p_visible->identifier()).c_str()));
    identifierfield->blockSignals(false);

    tooltipfield->blockSignals(true);
    tooltipfield->setText(QString::fromUtf8 (l2u(p_visible->tooltip()).c_str()));
    tooltipfield->blockSignals(false);

    p_foregroundcolourbutton->blockSignals(true);
    hk_colour c=p_visible->foregroundcolour();
    QColor qcol(c.red(),c.green(),c.blue());
    p_foregroundcolourbutton->setColor(qcol);
    p_foregroundcolourbutton->blockSignals(false);

    p_backgroundcolourbutton->blockSignals(true);
    c=p_visible->backgroundcolour();
    qcol.setRgb(c.red(),c.green(),c.blue());
    p_backgroundcolourbutton->setColor(qcol);
    p_backgroundcolourbutton->blockSignals(false);

    enabledfield->blockSignals(true);
    enabledfield->setCurrentIndex(p_visible->is_enabled()?0:1);
    enabledfield->blockSignals(false);

    int a=0;
    switch (p_visible->alignment())
    {
        case hk_visible::alignleft :  a=0;
        break;
        case hk_visible::aligncenter :    a=1;
        break;
        case hk_visible::alignright : a=2;
        break;
        case hk_visible::aligndefault :   a=3;
        break;
        default:;//cerr<<"alignment==default!!!!"<<endl;

    }
    alignmentfield->blockSignals(true);
    alignmentfield->setCurrentIndex(a);
    alignmentfield->blockSignals(false);
    onpushactionbutton->setText(p_visible->on_click_action().size()>0?settxt:notsettxt);
    ongetfocusbutton->setText(p_visible->on_getfocus_action().size()>0?settxt:notsettxt);
    onloosefocusbutton->setText(p_visible->on_loosefocus_action().size()>0?settxt:notsettxt);
    onkeybutton->setText(p_visible->on_key_action().size()>0?settxt:notsettxt);
    onpushactionbutton->setText(p_visible->on_click_action().size()>0?settxt:notsettxt);
    ondoubleclickactionbutton->setText(p_visible->on_doubleclick_action().size()>0?settxt:notsettxt);
    onopenactionbutton->setText(p_visible->on_open_action().size()>0?settxt:notsettxt);
    oncloseactionbutton->setText(p_visible->on_close_action().size()>0?settxt:notsettxt);
    onselectactionbutton->setText(notsettxt);

}


void hk_kdeproperty::set_geometry(void)
{
   if (p_visible==NULL)return;
    int w=0;
    int h=0;
    if (p_visible->type()==hk_visible::form)
    {
        hk_form* f=dynamic_cast <hk_form*>(p_visible);
        if (f!=NULL)
        {
            w=f->designwidth();
            h=f->designheight();
        }
    }
    else
    {
        w=p_visible->width();
        h=p_visible->height();

    }
    widthfield->blockSignals(true);
    widthfield->setValue(w);
    widthfield->blockSignals(false);
    heightfield->blockSignals(true);
    heightfield->setValue(h);
    heightfield->blockSignals(false);
    xfield->blockSignals(true);
    xfield->setValue(p_visible->x());
    xfield->blockSignals(false);
    yfield->blockSignals(true);
    yfield->setValue(p_visible->y());
    yfield->blockSignals(false);



}



void hk_kdeproperty::set_dsvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_dsvisible");
#endif
    if (p_visible==NULL)return;
    set_datasourcelist();
    columnfield->blockSignals(true);
    columnfield->clear();
    columnfield->blockSignals(false);
    datasource_selected();
    listdatasource_selected();
    hk_dsvisible* ds= dynamic_cast <hk_dsvisible*>(p_visible);
    if (!ds)return;
    readonlyfield->blockSignals(true);
    readonlyfield->setCurrentIndex(ds->is_readonly()?0:1);
    readonlyfield->blockSignals(false);

    beforeupdateactionbutton->show();
    beforeupdateactionlabel->show();
    afterupdateactionbutton->show();
    afterupdatelabel->show();
    beforerowchangeactionbutton->show();
    beforerowchangelabel->show();
    afterrowchangeactionbutton->show();
    afterrowchangelabel->show();
    beforedeleteactionbutton->show();
    beforedeletelabel->show();
    afterdeleteactionbutton->show();
    afterdeletelabel->show();
    beforeinsertactionbutton->show();
    beforeinsertlabel->show();
    afterinsertactionbutton->show();
    afterinsertlabel->show();
    beforerowchangeactionbutton->setText(ds->before_row_change_action().size()>0?settxt:notsettxt);
    afterrowchangeactionbutton->setText(ds->after_row_change_action().size()>0?settxt:notsettxt);
    beforeupdateactionbutton->setText(ds->before_update_action().size()>0?settxt:notsettxt);
    afterupdateactionbutton->setText(ds->after_update_action().size()>0?settxt:notsettxt);
    beforedeleteactionbutton->setText(ds->before_delete_action().size()>0?settxt:notsettxt);
    afterdeleteactionbutton->setText(ds->after_delete_action().size()>0?settxt:notsettxt);
    beforeinsertactionbutton->setText(ds->before_insert_action().size()>0?settxt:notsettxt);
    afterinsertactionbutton->setText(ds->after_insert_action().size()>0?settxt:notsettxt);

}


void hk_kdeproperty::set_dsdatavisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_dsdatavisible");
#endif
    if (p_visible==NULL)return;
    hk_dsdatavisible* ds= dynamic_cast <hk_dsdatavisible*>(p_visible);
    if (ds==NULL) {return;}
    columnfield->blockSignals(true);
    columnfield->setEditText(QString::fromUtf8 (l2u(ds->columnname()).c_str()));
    columnfield->blockSignals(false);
    defaultfield->blockSignals(true);
    defaultfield->setText(QString::fromUtf8 (l2u(ds->raw_defaultvalue()).c_str()));
    defaultfield->blockSignals(false);
    digitfield->blockSignals(true);
    digitfield->setValue(ds->commadigits());
    digitfield->blockSignals(false);
    separatorfield->blockSignals(true);
    separatorfield->setCurrentIndex(ds->use_numberseparator()?0:1);
    separatorfield->blockSignals(false);
    onvaluechangedactionbutton->show();
    onvaluechangedactionlabel->show();
    onvaluechangedactionbutton->setText(ds->on_valuechanged_action().size()>0?settxt:notsettxt);




    hk_dscombobox* combo=dynamic_cast <hk_dscombobox*>(p_visible);
    if (combo==NULL) return;
    listcolumnfield->blockSignals(true);
    listcolumnfield->setEditText(QString::fromUtf8 (l2u(combo->listcolumnname()).c_str()));
    listcolumnfield->blockSignals(false);
    viewcolumnfield->blockSignals(true);
    viewcolumnfield->setEditText(QString::fromUtf8 (l2u(combo->viewcolumnname()).c_str()));
    viewcolumnfield->blockSignals(false);
    combomodefield->blockSignals(true);
    int p=0;
    switch (combo->mode())
    {
     case hk_dscombobox::combo : p=0;break;
     case hk_dscombobox::combo_noedit : p=1;break;
     case hk_dscombobox::selector : p=2;break;
    }
    combomodefield->setCurrentIndex(p);
    combomodefield->blockSignals(false);

    if (combo->mode()==hk_dscombobox::selector)
    {
        listdatasourcefield->hide();
        listdatasourcelabel->hide();
        listdatasourcebutton->hide();
        listcolumnfield->hide();
        listcolumnlabel->hide();
        columnfield->hide();
        columnlabel->hide();
    }
    else
    {
        listdatasourcefield->show();
        listdatasourcelabel->show();
        listdatasourcebutton->show();
        listcolumnfield->show();
        listcolumnlabel->show();
        columnfield->show();


    }
    onselectactionbutton->setText(combo->on_select_action().size()>0?settxt:notsettxt);

     usetextlistfield->blockSignals(true);
     usetextlistfield->setCurrentIndex(combo->use_textlist()?0:1);
     if (combo->use_textlist())
     {
     usetextlistbutton->show();
        listdatasourcefield->hide();
        listdatasourcelabel->hide();
        listdatasourcebutton->hide();
        listcolumnlabel->hide();
        listcolumnfield->hide();
        viewcolumnlabel->hide();
        viewcolumnfield->hide();
     }
     else
     {
     usetextlistbutton->hide();
        listdatasourcefield->show();
        listdatasourcelabel->show();
        listdatasourcebutton->show();
        listcolumnlabel->show();
        listcolumnfield->show();
        viewcolumnlabel->show();
        viewcolumnfield->show();
     }
     usetextlistfield->blockSignals(false);

}

void hk_kdeproperty::set_labelvisible(void)
{
   hk_kdelabel* label=dynamic_cast<hk_kdelabel*>(p_visible);
   if (!label) return;
    toplinefield->blockSignals(true);
    toplinefield->setValue(label->topline());
    toplinefield->blockSignals(false);

    bottomlinefield->blockSignals(true);
    bottomlinefield->setValue(label->bottomline());
    bottomlinefield->blockSignals(false);

    leftlinefield->blockSignals(true);
    leftlinefield->setValue(label->leftline());
    leftlinefield->blockSignals(false);

    rightlinefield->blockSignals(true);
    rightlinefield->setValue(label->rightline());
    rightlinefield->blockSignals(false);

    diagonallurofield->blockSignals(true);
    diagonallurofield->setValue(label->diagonalluro());
    diagonallurofield->blockSignals(false);

    diagonallorufield->blockSignals(true);
    diagonallorufield->setValue(label->diagonalloru());
    diagonallorufield->blockSignals(false);
}

void hk_kdeproperty::set_gridvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_gridvisible");
#endif
    if (p_visible==NULL)return;
}

void hk_kdeproperty::set_subform(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_subform");
#endif

    if (!p_form) return;
    hk_subform* subform=dynamic_cast<hk_subform*>(p_visible);
    if (!subform) return;
    subformfield->blockSignals(true);
    subformfield->clear();
    subformfield->addItem("");

    vector<hk_string>* dslist = p_form->database()->formlist();
    if (dslist==NULL)
    {
        subformfield->blockSignals(false);
        return;
    }
    vector<hk_string>::iterator it=dslist->begin();
    int item=1;
    while (it!=dslist->end())
    {
        QString name= QString::fromUtf8 (l2u((*it)).c_str());
        if ((*it)!=p_form->hk_form::name())
	   subformfield->addItem(name);
	   else --item;
        if (subform->name()==(*it))
	   subformfield->setCurrentIndex(item);
        it++; ++item;
    }
    subformfield->blockSignals(false);
subformbutton->setEnabled(!subformfield->currentText().isEmpty()
&&!datasourcefield->currentText().isEmpty()
);
}


void hk_kdeproperty::set_button(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_button");
#endif
    if (p_visible==NULL)return;
    hk_button* bt = dynamic_cast <hk_button*>(p_visible);
    if (bt==NULL) return;
    localimagebutton->setText(bt->icon()->data?settxt:notsettxt);
    int i = bt->action()+1;
    showmaximizedfield->blockSignals(true);
    showmaximizedfield->setCurrentIndex(bt->show_maximized()?1:0);
    showmaximizedfield->blockSignals(false);
    actionfield->setCurrentIndex(i);
    set_actionobjectlist();
    vector<hk_string>* liste=NULL;
    conditionbutton->hide();

    if (i==4||i==14)  liste=p_form->database()->querylist();
    else
    if (i==5||i==6)
    {
        liste=p_form->database()->reportlist();

    }
    else
    if (i==1||i==2)
      {
       liste=p_form->database()->formlist();
      }
        else
            liste=p_form->database()->tablelist();
    vector<hk_string>::iterator it =  liste->begin();
    int current=1;
    while (it!=liste->end())
    {

        if ((*it)==bt->object())
        {
            objectfield->setCurrentIndex(current);
        }
        it++;  current++;
    }
 if((i==1||i==5||i==6)&&!objectfield->currentText().isEmpty())
   conditionbutton->show();
   else
   conditionbutton->hide();
   istogglebuttonfield->show();
   istogglebuttonlabel->show();
   if(bt->is_togglebutton())
   {
   togglepushedfield->show();
   togglepushedlabel->show();
   }
    istogglebuttonfield->blockSignals(true);
    istogglebuttonfield->setCurrentIndex(bt->is_togglebutton()?0:1);
    istogglebuttonfield->blockSignals(false);

    togglepushedfield->blockSignals(true);
    togglepushedfield->setCurrentIndex(bt->is_pushed()?0:1);
    togglepushedfield->blockSignals(false);

}


void hk_kdeproperty::set_imagevisible(void)
{

  hk_dsimage* i=dynamic_cast<hk_dsimage*>(p_visible);
  if (!i) return;
    defaultfield->blockSignals(true);
    defaultfield->setText(QString::fromUtf8 (l2u(i->path()).c_str()));
    defaultfield->blockSignals(false);
    scalefield->blockSignals(true);
    scalefield->setValue(i->zoom());
    scalefield->blockSignals(false);
    localimagebutton->setText(i->localimage()->data?settxt:notsettxt);
}

void hk_kdeproperty::set_objectvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_objectvisible");
#endif
    if (p_visible==NULL)return;
    p_visible->set_label(u2l(labelfield->text().toUtf8().data()));
    hk_string ps =  u2l(fontfield->currentText().toUtf8().data());
    hk_font f(u2l(fontfield->currentText().toUtf8().data()),fontsizefield->value());
    f.set_bold(boldfield->currentIndex()==0);
    f.set_italic(italicfield->currentIndex()==0);
    geometry_changes();
//hk_string ps;
//if (s!=NULL)
//ps=s;cout <<"font: "<<ps<<endl;
    p_visible->set_font(f);
    int i=alignmentfield->currentIndex();
    hk_visible::alignmenttype at=hk_visible::alignleft;
    switch (i)
    {
        case 1 : at=hk_visible::aligncenter;break;
        case 2 : at=hk_visible::alignright;break;
        case 3 : at=hk_visible::aligndefault;break;
        default:;

    }
    p_visible->set_alignment(at);

    QColor qcol=p_foregroundcolourbutton->color();
    hk_colour c(qcol.red(),qcol.green(),qcol.blue());
    p_visible->set_foregroundcolour(c);

    qcol=p_backgroundcolourbutton->color();
    c.set_colour(qcol.red(),qcol.green(),qcol.blue());
    p_visible->set_backgroundcolour(c);
    p_visible->set_enabled((enabledfield->currentIndex()==0));

}


void hk_kdeproperty::set_objectdsvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_objectdsvisible");
#endif
    if (p_visible==NULL)return;

    list<hk_datasource*>* dslist = p_form->datasources();
    if (dslist==NULL) return;
    list<hk_datasource*>::iterator it=dslist->begin();
    hk_dsvisible* ds = dynamic_cast <hk_dsvisible*>(p_visible);
    hk_dscombobox* combo =dynamic_cast <hk_dscombobox*>(p_visible);
    if (ds==NULL) return;
    ds->set_readonly((readonlyfield->currentIndex()==0));

    if (datasourcefield->currentText()=="") ds->set_presentationdatasource(-1);
    if (combo!=NULL&&listdatasourcefield->currentText()=="") combo->set_listpresentationdatasource(-1);
    while (it!=dslist->end())
    {
        QString name= QString::fromUtf8 (l2u(p_form->unique_datasourcename((*it)->presentationnumber())).c_str());
        if (name==datasourcefield->currentText())
        {
            ds->set_presentationdatasource( (*it)->presentationnumber() );
        }
        if (combo!=NULL)
        {
            if (combomodefield->currentIndex()==2)
            {
                listdatasourcefield->hide();
                listdatasourcelabel->hide();
                listdatasourcebutton->hide();
                listcolumnfield->hide();
                listcolumnlabel->hide();
                columnfield->hide();
                columnlabel->hide();
            }
            else
            {
                listdatasourcefield->show();
                listdatasourcelabel->show();
                listdatasourcebutton->show();
                listcolumnfield->show();
                listcolumnlabel->show();
                columnfield->show();
                columnlabel->show();

            }
            if (name==listdatasourcefield->currentText())
            {
                if (combo->mode()==hk_dscombobox::selector)
                {                                 // combobox changes from selectormode to combomode
                    combo->set_listpresentationdatasource(-1 );
                    listdatasourcefield->blockSignals(true);
                    listdatasourcefield->setCurrentIndex(0);
                    listdatasourcefield->blockSignals(false);

                }
                else
                {
                    combo->set_listpresentationdatasource( (*it)->presentationnumber() );
                }
            }
        }
        it++;
    }
    if (combo)
        {
	hk_dscombobox::enum_mode m;
	switch(combomodefield->currentIndex())
	{
	  case 1 : m=hk_dscombobox::combo_noedit;break;
	  case 2 : m=hk_dscombobox::selector;break;
	  default: m=hk_dscombobox::combo;break;

	}
	combo->set_mode(m);
	combo->set_use_textlist(usetextlistfield->currentIndex()==0);
	use_textlist_changed();
	}
}


void hk_kdeproperty::set_objectdsdatavisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_objectdsdatavisible");
#endif
    if (p_visible==NULL)return;
    hk_dsdatavisible* ds= dynamic_cast <hk_dsdatavisible*>(p_visible);
    if (!ds) return;

    ds->set_columnname(u2l(columnfield->currentText().toUtf8().data()));
    if (defaultfield->text().isEmpty()) ds->reset_default();
    else ds->set_defaultvalue(u2l(defaultfield->text().toUtf8().data()));

    ds->set_numberformat(separatorfield->currentIndex()==0,digitfield->value());

    set_objectimagevisible();

    hk_dscombobox* combo =dynamic_cast <hk_dscombobox*>(p_visible);
    if (combo)
    {
    combo->set_viewcolumnname(u2l(viewcolumnfield->currentText().toUtf8().data()));
    combo->set_listcolumnname(u2l(listcolumnfield->currentText().toUtf8().data()));
    }
}


void hk_kdeproperty::set_objectsubform(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_objectsubform");
#endif
hk_subform* subform= dynamic_cast<hk_subform*>(p_visible);
if (!subform) return;
subform->set_name(u2l(subformfield->currentText().toUtf8().data()));
subformbutton->setEnabled(!subformfield->currentText().isNull());



}

void hk_kdeproperty::set_objectimagevisible(void)
{
  hk_dsimage* i=dynamic_cast<hk_dsimage*>(p_visible);
  if (!i) return;
    if (defaultfield->text().isEmpty()) i->set_path("");
    else i->set_path(u2l(defaultfield->text().toUtf8().data()));
    i->set_zoom(scalefield->value());
}


void hk_kdeproperty::set_objectaction(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_objectaction");
#endif
//cerr <<"set_objectaction"<<endl;
    if (p_visible==NULL)return;
    hk_button* bt = dynamic_cast <hk_button*>(p_visible);
    //cerr <<"bt="<<bt<<endl;
    if (bt==NULL) return;
    bt->set_action(
        actionfield->currentIndex()-1,
        objectfield->currentText().isEmpty()?"":u2l(objectfield->currentText().toUtf8().data()),
        (showmaximizedfield->currentIndex()==1?true:false )
        );

    int i = bt->action();
    if(!objectfield->currentText().isEmpty()&&(i==0||i==4||i==5))
    conditionbutton->show();
    else
    conditionbutton->hide();

}


void hk_kdeproperty::add_formdatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::add_formdatasource");
#endif
    hk_kdeformdatasourcedialog* d = new hk_kdeformdatasourcedialog(p_form,this,0,true);
    hk_datasource* ds=p_form->get_datasource(u2l(datasourcefield->currentText().toUtf8().data()));
    d->set_datasource(ds);
    if (ds)
    {
        d->alterbutton->setEnabled(true);
        d->deletebutton->setEnabled(true);
    }
    d->exec();
    delete d;
    set_datasourcelist();
}


void hk_kdeproperty::add_listformdatasource()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::add_listformdatasource");
#endif
    hk_kdeformdatasourcedialog* d = new hk_kdeformdatasourcedialog(p_form,this,0,true);
    hk_datasource* ds=p_form->get_datasource(u2l(listdatasourcefield->currentText().toUtf8().data()));
    d->set_datasource(ds);
    if (ds)
    {
        d->alterbutton->setEnabled(true);
        d->deletebutton->setEnabled(true);
    }
    d->exec();
    delete d;
    set_datasourcelist();
}


void hk_kdeproperty::set_datasourcelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::set_datasourcelist");
#endif
    if (p_form==NULL) return;
    datasourcefield->blockSignals(true);
    listdatasourcefield->blockSignals(true);
    datasourcefield->clear();
    datasourcefield->addItem("");
    listdatasourcefield->clear();
    listdatasourcefield->addItem("");

    list<hk_datasource*>* dslist = p_form->datasources();
    if (dslist==NULL)
    {
        datasourcefield->blockSignals(false);
        listdatasourcefield->blockSignals(false);
        return;
    }
    list<hk_datasource*>::iterator it=dslist->begin();
    int item=1;
    while (it!=dslist->end())
    {
        QString name= QString::fromUtf8 (l2u(p_form->unique_datasourcename((*it)->presentationnumber())).c_str());
        datasourcefield->addItem(name);
        listdatasourcefield->addItem(name);
        hk_dsvisible* ds = dynamic_cast <hk_dsvisible*>(p_visible);
        hk_dscombobox* dscombo = dynamic_cast <hk_dscombobox*>(p_visible);
        if (ds!=NULL)
        {
            if (ds->datasource()==(*it)) datasourcefield->setCurrentIndex(item);
            if (dscombo!=NULL)if (dscombo->listdatasource()==(*it))
                listdatasourcefield->setCurrentIndex(item);

        }

        it++; item++;
    }
    datasourcefield->blockSignals(false);
    listdatasourcefield->blockSignals(false);
}


void hk_kdeproperty::datasource_selected(void)
{
#ifdef HK_DEBUG
    hkdebug("datasource_selected");
#endif
    columnfield->blockSignals(true);
    QString currentvalue=columnfield->currentText();
    columnfield->clear();
    columnfield->blockSignals(false);
    if (p_form==NULL) return;
    hk_database* db=p_form->database();
    if (db==NULL) return;
    QString f=datasourcefield->currentText();
    if (f.isEmpty()) f="";
    hk_datasource* ds= p_form->get_datasource(u2l(f.toUtf8().data()));
    if (ds==NULL) return;
    list<hk_column*>* v=NULL;
    hk_datasource* tmpds=NULL;

    if (ds->type()==hk_datasource::ds_query)
    {
        tmpds=ds->database()->new_resultquery();
        if (tmpds)
        {
            tmpds->set_sql(ds->sql());
            tmpds->set_filter("1=0");
            tmpds->enable();
            v=tmpds->columns();

        }

    }
    else    v=ds->columns();
    if (v==NULL)
    {
        if (tmpds)
        {
            tmpds->disable();
            delete tmpds;
        }
        return;
    }
    columnfield->blockSignals(true);
    viewcolumnfield->blockSignals(true);
    columnfield->addItem("");
    list<hk_column*>::iterator it=v->begin();
    int i=1;
    while (it!=v->end())
    {
        columnfield->addItem(QString::fromUtf8 (l2u((*it)->name()).c_str()));
        if (QString::fromUtf8(l2u((*it)->name()).c_str())==currentvalue) columnfield->setCurrentIndex(i);

        it++;++i;
    }
    columnfield->blockSignals(false);
    viewcolumnfield->blockSignals(false);
    set_viewcolumnvalues();
    if (tmpds)
    {
        tmpds->disable();
        delete tmpds;
    }
}


void hk_kdeproperty::listdatasource_selected(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeproperty::listdatasource_selected");
#endif
    listcolumnfield->blockSignals(true);
    listcolumnfield->clear();
    listcolumnfield->blockSignals(false);
    viewcolumnfield->blockSignals(true);
    viewcolumnfield->clear();
    viewcolumnfield->blockSignals(false);
    if (p_form==NULL) return;
    hk_database* db=p_form->database();
    if (db==NULL) return;
    QString f=listdatasourcefield->currentText();
    if (f.isEmpty()) f="";
    hk_datasource* ds= p_form->get_datasource(u2l(f.toUtf8().data()));
    if (ds==NULL) return;
    list<hk_column*>* v=NULL;
    hk_datasource* tmpds=NULL;

    if (ds->type()==hk_datasource::ds_query)
    {
        tmpds=ds->database()->new_resultquery();
        if (tmpds)
        {
            tmpds->set_sql(ds->sql());
            tmpds->set_filter("1=0");
            tmpds->enable();
            v=tmpds->columns();

        }

    }
    else    v=ds->columns();
    if (v==NULL)
    {
        if (tmpds)
        {
            tmpds->disable();
            delete tmpds;
        }
        return;
    }
    listcolumnfield->addItem("");
    viewcolumnfield->addItem("");
    list<hk_column*>::iterator it=v->begin();
    while (it!=v->end())
    {
        listcolumnfield->addItem(QString::fromUtf8 (l2u((*it)->name()).c_str()));
        viewcolumnfield->addItem(QString::fromUtf8 (l2u((*it)->name()).c_str()));
        it++;
    }

    if (tmpds)
    {
        tmpds->disable();
        delete tmpds;
    }

}


void hk_kdeproperty::combomode_changed(void)
{
    if (p_visible->type()==hk_visible::form)
    {
	    p_form->set_sizetype(combomodefield->currentIndex()==0?hk_presentation::relative:hk_presentation::absolute);
    return;
    }
    set_viewcolumnvalues();
}


void hk_kdeproperty::set_viewcolumnvalues()
{

    viewcolumnfield->blockSignals(true);
    viewcolumnfield->clear();
    viewcolumnfield->addItem("");
    viewcolumnfield->blockSignals(false);

    QString f;
    if (combomodefield->currentIndex()==2&& p_visible&&p_visible->type()==hk_visible::combobox)
        f=datasourcefield->currentText();
    else
        f=listdatasourcefield->currentText();
    if (f.isEmpty()) f="";
    hk_datasource* ds=p_form->get_datasource(u2l(f.toUtf8().data()));
    if (ds==NULL)
    {
        return;
    }
    list<hk_column*>* v=ds->columns();
    if (!v) return;
    list<hk_column*>::iterator it=v->begin();
    while (it!=v->end())
    {

        viewcolumnfield->addItem(QString::fromUtf8(l2u((*it)->name()).c_str()));
        it++;
    }
    viewcolumnfield->blockSignals(false);

}


void hk_kdeproperty::conditionbutton_clicked()
{
  int i = actionfield->currentIndex()-1;
  hk_kdereportconditiondialog* d = new hk_kdereportconditiondialog(this,0);
  d->set_values(dynamic_cast <hk_button*>(p_visible),p_form,u2l(objectfield->currentText().toUtf8().data()),(i==0?hk_kdereportconditiondialog::form:hk_kdereportconditiondialog::report));
  d->exec();
  delete d;
}

void hk_kdeproperty::pushactionbutton_clicked(void)
{
  pushactionbutton_clicked(0,"");
}

void hk_kdeproperty::pushactionbutton_clicked(int rownumber,const hk_string& warning)
{
  visibleuploadimp u_pushbutton(*p_visible, &hk_visible::set_on_click_action);
  hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0, 0, 0, &u_pushbutton);
  d->setWindowModality(Qt::ApplicationModal);
  d->set_code(p_visible->on_click_action(),false);
  d->set_caption(p_visible,"on_click_action");
  int r=d->exec(rownumber,warning);
  if (r==hk_kdeinterpreterdialog::Accepted && d->has_changed())
    p_visible->set_on_click_action(d->code());
  set_visible();
  set_dsvisible();
  set_dsdatavisible();
  delete d;
}


void hk_kdeproperty::doubleclickactionbutton_clicked(void)
{
doubleclickactionbutton_clicked(0,"");
}

void hk_kdeproperty::doubleclickactionbutton_clicked(int rownumber,const hk_string& warning)
{
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(p_visible->on_doubleclick_action(),false);
    d->set_caption(p_visible,"on_doubleclick_action");
    int r=d->exec(rownumber,warning);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      p_visible->set_on_doubleclick_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();
    delete d;


}


void hk_kdeproperty::openactionbutton_clicked(int rownumber,const hk_string& warningmessage)
{
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(p_visible->on_open_action(),false);
    d->set_caption(p_visible,"on_open_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      p_visible->set_on_open_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();
    delete d;


}

void hk_kdeproperty::openactionbutton_clicked(void)
{
openactionbutton_clicked(0,"");
}

void hk_kdeproperty::closeactionbutton_clicked(int rownumber,const hk_string& warningmessage)
{
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(p_visible->on_close_action(),false);
    d->set_caption(p_visible,"on_close_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      p_visible->set_on_close_action(d->code());
    }
   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;


}

void hk_kdeproperty::closeactionbutton_clicked(void)
{
closeactionbutton_clicked(0,"");
}

void hk_kdeproperty::beforedeletebutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
  d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->before_delete_action(),false);
    d->set_caption(p_visible,"before_delete_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_before_delete_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;


}

void hk_kdeproperty::beforedeletebutton_clicked(void)
{
beforedeletebutton_clicked(0,"");
}

void hk_kdeproperty::afterdeletebutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->after_delete_action(),false);
    d->set_caption(p_visible,"after_delete_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_after_delete_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;

}

void hk_kdeproperty::afterdeletebutton_clicked(void)
{
afterdeletebutton_clicked(0,"");
}

void hk_kdeproperty::beforeinsertbutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->before_insert_action(),false);
    d->set_caption(p_visible,"before_insert_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_before_insert_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;


}

void hk_kdeproperty::beforeinsertbutton_clicked(void)
{
beforeinsertbutton_clicked(0,"");
}

void hk_kdeproperty::afterinsertbutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->after_insert_action(),false);
    d->set_caption(p_visible,"after_insert_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_after_insert_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;

}

void hk_kdeproperty::afterinsertbutton_clicked(void)
{
afterinsertbutton_clicked(0,"");
}

void hk_kdeproperty::beforeupdatebutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->before_update_action(),false);
    d->set_caption(p_visible,"before_update_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_before_update_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;

}

void hk_kdeproperty::beforeupdatebutton_clicked(void)
{
beforeupdatebutton_clicked(0,"");
}

void hk_kdeproperty::afterupdatebutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->after_update_action(),false);
    d->set_caption(p_visible,"after_update_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_after_update_action(d->code());
    }
   set_visible();
   set_dsvisible();
   set_dsdatavisible();


    delete d;

}

void hk_kdeproperty::afterupdatebutton_clicked(void)
{
afterupdatebutton_clicked(0,"");
}

void hk_kdeproperty::beforerowchangebutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->before_row_change_action(),false);
    d->set_caption(p_visible,"before_row_change_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_before_row_change_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;

}

void hk_kdeproperty::beforerowchangebutton_clicked(void)
{
beforerowchangebutton_clicked(0,"");
}

void hk_kdeproperty::afterrowchangebutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
  dsvisibleuploadimp u_afterrowchange(*ds, &hk_dsvisible::set_after_row_change_action);
  hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0, 0, 0, &u_afterrowchange);
  d->setWindowModality(Qt::ApplicationModal);
  d->set_caption(p_visible,"after_row_change_action");
  d->set_code(ds->after_row_change_action(),false);
  int r=d->exec(rownumber,warningmessage);
  if (r==hk_kdeinterpreterdialog::Accepted && d->has_changed())
    ds->set_after_row_change_action(d->code());

  set_visible();
  set_dsvisible();
  set_dsdatavisible();
  delete d;
}

void hk_kdeproperty::afterrowchangebutton_clicked(void)
{
afterrowchangebutton_clicked(0,"");
}

void hk_kdeproperty::ongetfocusbutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  
  if (!ds) return;
  dsvisibleuploadimp u_getfocus(*ds, &hk_dsvisible::set_on_getfocus_action);
  hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0, 0, 0, &u_getfocus);
  d->setWindowModality(Qt::ApplicationModal);
  d->set_code(ds->on_getfocus_action(),false);
  d->set_caption(p_visible,"on_getfocus_action");
  int r=d->exec(rownumber,warningmessage);
  if (r==hk_kdeinterpreterdialog::Accepted && d->has_changed())
    ds->set_on_getfocus_action(d->code());

  set_visible();
  set_dsvisible();
  set_dsdatavisible();

  delete d;
}

void hk_kdeproperty::ongetfocusbutton_clicked(void)
{
  ongetfocusbutton_clicked(0,"");
}

void hk_kdeproperty::onloosefocusbutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  
  if (!ds) return;
  dsvisibleuploadimp uloosefocus(*ds, &hk_dsvisible::set_on_loosefocus_action);
  hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0, 0, 0, &uloosefocus);
  d->setWindowModality(Qt::ApplicationModal);
  d->set_code(ds->on_loosefocus_action(),false);
  d->set_caption(p_visible,"on_loosefocus_action");
  int r=d->exec(rownumber,warningmessage);
  if (r==hk_kdeinterpreterdialog::Accepted && d->has_changed())
    ds->set_on_loosefocus_action(d->code());

  set_visible();
  set_dsvisible();
  set_dsdatavisible();

  delete d;
}

void hk_kdeproperty::onloosefocusbutton_clicked(void)
{
  onloosefocusbutton_clicked(0,"");
}

void hk_kdeproperty::onselectbutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dscombobox* ds=dynamic_cast<hk_dscombobox*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->on_select_action(),false);
    d->set_caption(p_visible,"on_select_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_on_select_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;
}

void hk_kdeproperty::onselectbutton_clicked(void)
{
onselectbutton_clicked(0,"");
}



void hk_kdeproperty::onkeybutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->on_key_action(),false);
    d->set_caption(p_visible,"on_key_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_on_key_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;


}



void hk_kdeproperty::onkeybutton_clicked(void)
{
onkeybutton_clicked(0,"");
}






void hk_kdeproperty::onvaluechangedbutton_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsdatavisible* ds=dynamic_cast<hk_dsdatavisible*>(p_visible);
  if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->on_valuechanged_action(),false);
    d->set_caption(p_visible,"on_valuechanged_action");
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_on_valuechanged_action(d->code());
    }

   set_visible();
   set_dsvisible();
   set_dsdatavisible();

    delete d;


}



void hk_kdeproperty::onvaluechangedbutton_clicked(void)
{
onvaluechangedbutton_clicked(0,"");
}

 void hk_kdeproperty::border_changes()
 {
   if (!p_visible) return;
   hk_kdelabel* label=dynamic_cast<hk_kdelabel*>(p_visible);
   if (!label) return;
   label->set_topline(toplinefield->value());
   label->set_bottomline(bottomlinefield->value());
   label->set_leftline(leftlinefield->value());
   label->set_rightline(rightlinefield->value());
   label->set_diagonalloru(diagonallorufield->value());
   label->set_diagonalluro(diagonallurofield->value());
 }

hk_visible* hk_kdeproperty::object(void)
{
return p_visible;
}


void hk_kdeproperty::subformbutton_clicked()
{
  hk_subform* subf=dynamic_cast<hk_subform*>(p_visible);
  if (!subf) return;
  hk_kdesubformdialog* d=new hk_kdesubformdialog(subf);
    if (d->exec()==QDialog::Accepted)
       {

       }

    delete d;
}


void hk_kdeproperty::use_editor(QLineEdit* e)
{
  if (!e) return;
    hk_kdefilterdialog* f = new hk_kdefilterdialog(hk_kdefilterdialog::editor,this,0,true);

    f->textfield->setText(e->text());
    if (f->exec()==QDialog::Accepted)
        e->setText(f->textfield->toPlainText());

    delete f;
}

bool hk_kdeproperty::eventFilter(QObject* object,QEvent* event)
{

 QLineEdit* edit=dynamic_cast<QLineEdit*>(object);

   if (edit&&event->type()==QEvent::KeyPress)
   {
     QKeyEvent*ev=(QKeyEvent*)event;
     if (ev->key()==Qt::Key_F2)
       use_editor(edit);
   }
   return hk_kdepropertyeditorbase::eventFilter(object,event);
}

void hk_kdeproperty::use_textlist_changed()
{
  bool e=usetextlistfield->currentIndex()==0;

  if (e)
  {
  usetextlistbutton->show();
  combomodefield->hide();
  combomodelabel->hide();
  listcolumnfield->hide();
  listcolumnlabel->hide();
  listdatasourcefield->hide();
  listdatasourcelabel->hide();
  listdatasourcebutton->hide();
  viewcolumnfield->hide();
  viewcolumnlabel->hide();
  }
  else
  {
  usetextlistbutton->hide();
  combomodefield->show();
  combomodelabel->show();
  listcolumnfield->show();
  listcolumnlabel->show();
  listdatasourcelabel->show();
  listdatasourcefield->show();
  listdatasourcebutton->show();
  viewcolumnfield->show();
  viewcolumnlabel->show();
  }
}

void hk_kdeproperty::use_textlistbutton_clicked()
{
  hk_dscombobox* c=dynamic_cast<hk_dscombobox*>(p_visible);
  if (!c) return;
  hk_kdecomboboxtextlist* d = new hk_kdecomboboxtextlist(this,c);
  d->exec();
  delete d;
}


void hk_kdeproperty::localimage_selected()
{
  hk_dsimage* i=dynamic_cast<hk_dsimage*>(p_visible);
  hk_button* b=dynamic_cast<hk_button*>(p_visible);
  if (!i &&!b ) return;
  QUrl url = KFileDialog::getImageOpenUrl( QString((b?"kfiledialog:///icon":"kfiledialog:///image")), this );
  if (!url.fileName().isEmpty())
  {
    hk_string v=u2l(url.path().toUtf8().data());
    hk_string error=replace_all("%1",hk_translate("Image '%1' could not be loaded"),v);
    if (i)
    {
      if (!i->load_localimage(v))
        show_warningmessage(error);
    }
    else
      if (b)
      {
        if (!b->load_icon(v))
          show_warningmessage(error);
      }
  }
}


void hk_kdeproperty::has_changed()
{
 if (p_form) p_form->has_changed(true);
}

void hk_kdeproperty::buttonformat_changed()
{
  hk_button* bt=dynamic_cast<hk_button*>(p_visible);
  if (!bt) return;
  bt->set_is_togglebutton(istogglebuttonfield->currentIndex()==0);
  bt->set_is_pushed(togglepushedfield->currentIndex()==0);

   if (bt->is_togglebutton())
   {
   togglepushedfield->show();
   togglepushedlabel->show();
   }
   else
   {
   togglepushedfield->hide();
   togglepushedlabel->hide();
   }

}

