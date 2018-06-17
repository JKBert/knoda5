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

#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#error config.h is needed but not included 
#endif
#include "hk_kdeformpartwidget.h"
#include "hk_kdetoolbar.h"
#include "hk_kdesimpleform.h"
#include "hk_kderowselector.h"
#include "hk_kde4simplegrid.h"
#include "../hk_kdegridpart/hk_kdegridpart.h"
#include "../hk_kdeformpart/hk_kdeformpart.h"
#include "hk_kdeformfocus.h"
#include "hk_kdetaborder.h"
#include "hk_kdeproperty.h"

#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qclipboard.h>
#include <kstatusbar.h>
#include <qstatusbar.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qscrollarea.h>
#include <qevent.h>

#include <kglobal.h>
#include <kiconloader.h>
#include <kapplication.h>
#include <ktoggleaction.h>
#include <kactionmenu.h>
#include <KActionCollection>

#include <hk_class.h>
#include <hk_database.h>
#include <hk_dsvisible.h>
#include <hk_visible.h>
#include <hk_dsdatavisible.h>
#include <hk_datasource.h>
#include <hk_connection.h>

#include <kmenubar.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <klocale.h>

/*
 *  Constructs a hk_kdeformpartwidget which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
hk_kdeformpartwidget::hk_kdeformpartwidget( hk_kdeformpart* formpart,QWidget* parent,  const char* name, Qt::WFlags fl )
: KXmlGuiWindow( parent, fl ) ,hk_class()
{
#ifdef HK_DEBUG
    hkclassname("hk_kdeformpartwidget");
    hkdebug("hk_kdeformpartwidget::hk_kdeformpartwidget");
#endif
    p_formpart=formpart;
    p_dontclose=false;
    setFocusPolicy(Qt::StrongFocus);
    KIconLoader loader  (LIB_MODULE_NAME);
    setObjectName( name == NULL ? "hk_kdeformpartwidget":name );
    setWindowTitle( i18n( "Form"  ) );
    resize( 800, 600 );
    QPixmap p;
    p_scrollview = new QScrollArea(this);
    p_form = new hk_kdesimpleform( p_scrollview);   
    
    setCentralWidget(p_scrollview);
    p_scrollview->setWidget(p_form); 
    p_form->set_formpartwidget(this);
    QWidget* wid= new QWidget(statusBar());
    QHBoxLayout* l=new QHBoxLayout(wid);
    p_rowselector = new hk_kderowselector( wid ,p_form);
    p_rowselector->set_neverregisterchange(true);
    l->addWidget(p_rowselector);
    int hbheight= 20;
    p_rowselector->setMaximumHeight(hbheight);
    p_rowselector->setMaximumWidth(240);
    p_rowselector->setMinimumHeight(hbheight);
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding ,QSizePolicy::Minimum );
    l->addItem( spacer );

    statusBar()->addWidget(wid);
    QSizePolicy fixedpolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    p_rowselector->setSizePolicy( fixedpolicy );
    setFocusPolicy(Qt::ClickFocus);
    QActionGroup* pag;
    
    if (runtime_only())
    {
      p_designaction=NULL;
      p_viewaction=NULL;
    }
    else
    {
      p_designaction=new KToggleAction(QIcon::fromTheme("document-edit"),i18n("&Design mode"),formpart->actionCollection());
      formpart->actionCollection()->addAction("designmode",p_designaction);
      connect(p_designaction,SIGNAL(triggered()),this,SLOT(designbutton_clicked()));
      p_designaction->setEnabled(!runtime_only());
      p_viewaction=new KToggleAction(QIcon::fromTheme("system-run"),i18n("&View mode"),formpart->actionCollection());
      formpart->actionCollection()->addAction("viewmode",p_viewaction);
      connect(p_viewaction,SIGNAL(triggered()),this,SLOT(formbutton_clicked()));
      pag = new QActionGroup(this);
      p_designaction->setActionGroup(pag);
      p_viewaction->setActionGroup(pag);
      p_designaction->setChecked(true);
    }
    p_saveaction=new KAction(QIcon::fromTheme("document-save"),i18n("&Save"),formpart->actionCollection());
    formpart->actionCollection()->addAction("save",p_saveaction);
    connect(p_saveaction,SIGNAL(triggered()),this,SLOT(save_form()));
    p_saveaction->setEnabled(!runtime_only());
    
    p_saveasaction=new KAction(QIcon::fromTheme("document-save-as"),i18n("Save &as"),formpart->actionCollection());
    formpart->actionCollection()->addAction("saveas",p_saveasaction);
    connect(p_saveasaction,SIGNAL(triggered()),this,SLOT(saveas_form()));
    p_saveasaction->setEnabled(!runtime_only());

    p_lineeditaction=new KToggleAction(QIcon(loader.iconPath("editline",KIconLoader::User)),i18n("Lineedit"),formpart->actionCollection());
    formpart->actionCollection()->addAction("lineedit", p_lineeditaction);
    connect(p_lineeditaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_pointeraction=new KToggleAction(QIcon(loader.iconPath("pfeil",KIconLoader::User)),i18n("Pointer"),formpart->actionCollection());
    formpart->actionCollection()->addAction("pointer",p_pointeraction);
    connect(p_pointeraction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_labelaction=new KToggleAction(QIcon(loader.iconPath("label",KIconLoader::User)),i18n("Label"),formpart->actionCollection());
    formpart->actionCollection()->addAction("label",p_labelaction);
    connect(p_labelaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_multilineeditaction=new KToggleAction(QIcon(loader.iconPath("multiline",KIconLoader::User)),i18n("Multiline"),formpart->actionCollection());
    formpart->actionCollection()->addAction("multiline",p_multilineeditaction);
    connect(p_multilineeditaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_booleanaction=new KToggleAction(QIcon(loader.iconPath("boolean22x22",KIconLoader::User)),i18n("Boolean"),formpart->actionCollection());
    formpart->actionCollection()->addAction("boolean",p_booleanaction);
    connect(p_booleanaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_comboboxaction=new KToggleAction(QIcon(loader.iconPath("combobox22x22",KIconLoader::User)),i18n("Combobox"),formpart->actionCollection());
    formpart->actionCollection()->addAction("combobox",p_comboboxaction);
    connect(p_comboboxaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_gridaction=new KToggleAction(QIcon(loader.iconPath("grid22x22",KIconLoader::User)),i18n("Grid"),formpart->actionCollection());
    formpart->actionCollection()->addAction("grid",p_gridaction);
    connect(p_gridaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_selectoraction=new KToggleAction(QIcon(loader.iconPath("rowselector22x22",KIconLoader::User)),i18n("Selector"),formpart->actionCollection());
    formpart->actionCollection()->addAction("selector",p_selectoraction);
    connect(p_selectoraction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_buttonaction=new KToggleAction(QIcon(loader.iconPath("button",KIconLoader::User)),i18n("Button"),formpart->actionCollection());
    formpart->actionCollection()->addAction("button",p_buttonaction);
    connect(p_buttonaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_subformaction=new KToggleAction(QIcon::fromTheme("utilities-terminal"),i18n("Subform"),formpart->actionCollection());
    formpart->actionCollection()->addAction("subform",p_subformaction);
    connect(p_subformaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_imageaction=new KToggleAction(QIcon(loader.iconPath("mimage",KIconLoader::User)),i18n("Image"),formpart->actionCollection());
    formpart->actionCollection()->addAction("image",p_imageaction);
    connect(p_imageaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_tabwidgetaction=new KToggleAction(QIcon(""),i18n("Tabwidget"),formpart->actionCollection());
    formpart->actionCollection()->addAction("tabwidget",p_tabwidgetaction);
    connect(p_tabwidgetaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_dateaction=new KToggleAction(QIcon::fromTheme("view-pim-calendar"),i18n("date"),formpart->actionCollection());
    formpart->actionCollection()->addAction("date",p_dateaction);
    connect(p_dateaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));

    pag = new QActionGroup(this);
    p_pointeraction->setActionGroup(pag);
    p_labelaction->setActionGroup(pag);
    p_lineeditaction->setActionGroup(pag);
    p_multilineeditaction->setActionGroup(pag);
    p_booleanaction->setActionGroup(pag);
    p_comboboxaction->setActionGroup(pag);
    p_gridaction->setActionGroup(pag);
    p_selectoraction->setActionGroup(pag);
    p_buttonaction->setActionGroup(pag);
    p_subformaction->setActionGroup(pag);
    p_imageaction->setActionGroup(pag);
    p_tabwidgetaction->setActionGroup(pag);
    p_dateaction->setActionGroup(pag);

    p_pointeraction->setToolTip( i18n( "select" ) );
    p_labelaction->setToolTip( i18n( "textlabel" ));
    p_lineeditaction->setToolTip( i18n( "lineeditfield" ));
    p_multilineeditaction->setToolTip( i18n( "memofield" ));
    p_booleanaction->setToolTip( i18n( "booleanfield" ));
    p_comboboxaction->setToolTip( i18n( "comboboxfield" ));
    p_gridaction->setToolTip( i18n( "gridfield" ));
    p_selectoraction->setToolTip( i18n( "rowselector" ));
    p_buttonaction->setToolTip( i18n( "button" ));

    p_pointeraction->setChecked(true);
    
    p_bulkaction=new KActionMenu(i18n("Set to all"),formpart->actionCollection());
    formpart->actionCollection()-> addAction("bulk",p_bulkaction);
    p_bulkfontaction=new KAction(i18n("Font"),formpart->actionCollection());
    formpart->actionCollection() -> addAction("bulkfont",p_bulkfontaction);
    connect(p_bulkfontaction,SIGNAL(triggered()),this,SLOT(bulkfont_clicked()));
    p_bulkforegroundcolouraction=new KAction(i18n("Foregroundcolour"),formpart->actionCollection());
    formpart->actionCollection()-> addAction("bulkforeground",p_bulkforegroundcolouraction);
    connect(p_bulkforegroundcolouraction,SIGNAL(triggered()),this,SLOT(bulkforegroundcolour_clicked()));
    p_bulkbackgroundcolouraction=new KAction(i18n("Backgroundcolour"),formpart->actionCollection());
    formpart->actionCollection()->addAction("bulkbackground",p_bulkbackgroundcolouraction);
    connect(p_bulkbackgroundcolouraction,SIGNAL(triggered()),this,SLOT(bulkbackgroundcolour_clicked()));

    p_bulkaction->addAction(p_bulkfontaction);
    p_bulkaction->addAction(p_bulkforegroundcolouraction);
    p_bulkaction->addAction(p_bulkbackgroundcolouraction);    
    
    p_taborderaction=new KAction(QIcon::fromTheme("go-bottom"),i18n("Tab order"),formpart->actionCollection());
    formpart->actionCollection()->addAction("taborder",p_taborderaction);
    connect(p_taborderaction,SIGNAL(triggered()),this,SLOT(taborder_clicked()));

    p_reloadaction=new KAction(QIcon::fromTheme("view-refresh"),i18n("Reload"),formpart->actionCollection());
    formpart->actionCollection()->addAction("reload",p_reloadaction);
    connect(p_reloadaction,SIGNAL(triggered()),this,SLOT(reload_form()));

    connect( p_form, SIGNAL( field_created() ), this, SLOT( field_created() ) );
    connect( p_form, SIGNAL( signal_focuswidget_changed() ), this, SLOT( slot_focuswidget_changed() ) );

    p_deleteaction=new KAction(i18n("&Delete"),formpart->actionCollection());
    formpart->actionCollection()->addAction("deleteclicked",p_deleteaction);
    p_deleteaction->setShortcut(Qt::Key_Delete);
    connect(p_deleteaction,SIGNAL(triggered()),p_form,SLOT(delete_widgets()));
    p_copyaction=new KAction(i18n("&Copy"),formpart->actionCollection());
    formpart->actionCollection()->addAction("copy",p_copyaction);
    p_copyaction->setShortcut(Qt::ControlModifier+Qt::Key_C);
    connect(p_copyaction,SIGNAL(triggered()),p_form,SLOT(copy()));
    p_pasteaction=new KAction(i18n("&Paste"),formpart->actionCollection());
    formpart->actionCollection()->addAction("paste",p_pasteaction);
    p_pasteaction->setShortcut(Qt::ControlModifier+Qt::Key_V);
    connect(p_pasteaction,SIGNAL(triggered()),p_form,SLOT(paste()));
    p_cutaction=new KAction(i18n("Cu&t"),formpart->actionCollection());
    formpart->actionCollection()->addAction("cutclicked",p_cutaction);
    p_cutaction->setShortcut(Qt::ControlModifier+Qt::Key_X);
    connect(p_cutaction,SIGNAL(triggered()),p_form,SLOT(cut()));
    p_formpropertyaction=new KAction(QIcon(loader.iconPath("propertyeditor",KIconLoader::User)),i18n("&Propertyeditor"),formpart->actionCollection());
    formpart->actionCollection()->addAction("viewproperty",p_formpropertyaction);
    connect(p_formpropertyaction,SIGNAL(triggered()),p_form,SLOT(show_property()));

    p_resizeaction=new KActionMenu(i18n("Adjust &size"),formpart->actionCollection());
    formpart->actionCollection()->addAction("size",p_resizeaction);
    p_minwidthaction=new KAction(i18n("Minimum width"),formpart->actionCollection());
    formpart->actionCollection()->addAction("minwidth",p_minwidthaction);
    connect(p_minwidthaction,SIGNAL(triggered()),p_form,SLOT(adjust_minw()));
    p_maxwidthaction=new KAction(i18n("Maximum width"),formpart->actionCollection());
    formpart->actionCollection()->addAction("maxwidth",p_maxwidthaction);
    connect(p_maxwidthaction,SIGNAL(triggered()),p_form,SLOT(adjust_maxw()));
    p_minheightaction=new KAction(i18n("Minimum height"),formpart->actionCollection());
    formpart->actionCollection()->addAction("minheight",p_minheightaction);
    connect(p_minheightaction,SIGNAL(triggered()),p_form,SLOT(adjust_minh()));
    p_maxheightaction=new KAction(i18n("Maximum height"),formpart->actionCollection());
    formpart->actionCollection()->addAction("maxheight",p_maxheightaction);
    connect(p_maxheightaction,SIGNAL(triggered()),p_form,SLOT(adjust_maxh()));
    p_minsizeaction=new KAction(i18n("Minimum size"),formpart->actionCollection());
    formpart->actionCollection()->addAction("minsize",p_minsizeaction);
    connect(p_minsizeaction,SIGNAL(triggered()),p_form,SLOT(adjust_mins()));
    p_maxsizeaction=new KAction(i18n("Maximum size"),formpart->actionCollection());
    formpart->actionCollection()->addAction("maxsize",p_maxsizeaction);
    connect(p_maxsizeaction,SIGNAL(triggered()),p_form,SLOT(adjust_maxs()));
    p_resizeaction->addAction(p_minwidthaction);
    p_resizeaction->addAction(p_maxwidthaction);
    p_resizeaction->addAction(p_minheightaction);
    p_resizeaction->addAction(p_maxheightaction);
    p_resizeaction->addAction(p_minsizeaction);
    p_resizeaction->addAction(p_maxsizeaction);

    p_alignaction=new KActionMenu(i18n("&Align"),formpart->actionCollection());
    formpart->actionCollection()->addAction("align",p_alignaction);
    p_alignleftaction=new KAction(i18n("&Left"),formpart->actionCollection());
    formpart->actionCollection()->addAction("alignleft",p_alignleftaction);
    connect(p_alignleftaction,SIGNAL(triggered()),p_form,SLOT(align_left()));
    p_alignrightaction=new KAction(i18n("&Right"),formpart->actionCollection());
    formpart->actionCollection()->addAction("alignright",p_alignrightaction);
    connect(p_alignrightaction,SIGNAL(triggered()),p_form,SLOT(align_right()));
    p_aligntopaction=new KAction(i18n("&Top"),formpart->actionCollection());
    formpart->actionCollection()->addAction("aligntop",p_aligntopaction);
    connect(p_aligntopaction,SIGNAL(triggered()),p_form,SLOT(align_top()));
    p_alignbottomaction=new KAction(i18n("&Bottom"),formpart->actionCollection());
    formpart->actionCollection()->addAction("alignbottom",p_alignbottomaction);
    connect(p_alignbottomaction,SIGNAL(triggered()),p_form,SLOT(align_bottom()));
    p_alignaction->addAction(p_alignleftaction);
    p_alignaction->addAction(p_alignrightaction);
    p_alignaction->addAction(p_aligntopaction);
    p_alignaction->addAction(p_alignbottomaction);
    p_dbdesigneraction=new KAction(QIcon(loader.iconPath("dbdesigner",KIconLoader::User)),i18n("Database designer"),formpart->actionCollection());
    formpart->actionCollection()->addAction("dbdesigner",p_dbdesigneraction);
    connect(p_dbdesigneraction,SIGNAL(triggered()),p_form,SLOT(dbdesignaction()));

    p_withlabelaction=new KToggleAction(QIcon::fromTheme("mail-attachment"),i18n("with label"),formpart->actionCollection());
    formpart->actionCollection()->addAction("withlabel",p_withlabelaction);
    connect(p_withlabelaction,SIGNAL(triggered()),this,SLOT(withlabelbutton_clicked()));

    p_definegridcolumnaction=new KAction(i18n("Define columns"),formpart->actionCollection());
    formpart->actionCollection()->addAction("gridcolumndialog",p_definegridcolumnaction);
    connect(p_definegridcolumnaction,SIGNAL(triggered()),this,SLOT(show_gridcolumndialog()));
    p_definegridcolumnaction->setEnabled(false);

    toolbar = new hk_kdetoolbar( this, "toolbar" );
    p_filterdefinitionaction=new KToggleAction(QIcon(loader.iconPath("filter",KIconLoader::User)),i18n("Filterdefinition"),formpart->actionCollection());
    formpart->actionCollection()->addAction("filterdefinition",p_filterdefinitionaction);
    connect(p_filterdefinitionaction,SIGNAL(triggered()),this,SLOT(filterdefinebutton_clicked()));
    p_filterexecaction=new KToggleAction(QIcon(loader.iconPath("filterexec",KIconLoader::User)),i18n("Filterexecution"),formpart->actionCollection());
    formpart->actionCollection()->addAction("filterexec",p_filterexecaction);
    connect(p_filterexecaction,SIGNAL(triggered()),toolbar,SLOT(filterexecbutton_clicked()));
    toolbar->set_filteractions(p_filterdefinitionaction,p_filterexecaction);
    
    p_clearfilteraction=new KAction(i18n("Clear filter"),formpart->actionCollection());
    formpart->actionCollection()->addAction("filterclear",p_clearfilteraction);
    connect(p_clearfilteraction,SIGNAL(triggered()),this,SLOT(clear_filter()));

    set_nodesignmode(runtime_only());
    //setCentralWidget(p_scrollview);
    //createGUI(0L);
    //toolBar("designtoolbar")->setBarPos(KToolBar::Left);
    set_viewmode();
    connect(p_form,SIGNAL(signal_has_changed()),this,SLOT(slot_has_changed()));
    connect(p_form,SIGNAL(signal_name_changed()),this,SLOT(set_caption()));
    connect (QApplication::clipboard(), SIGNAL(dataChanged()),this, SLOT(clipboard_changed()));
    p_form->reset_has_changed();
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeformpartwidget::~hk_kdeformpartwidget()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::~hk_kdeformpartwidget");
#endif
//    cerr <<p_form->hk_form::name()<<" callerform:"<<caller_form()<<endl;
     emit signal_raise_form(caller_form());
    delete p_rowselector;
}


hk_presentation::enum_mode  hk_kdeformpartwidget::mode(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::mode");
#endif

    return p_form->mode();
}


void    hk_kdeformpartwidget::set_mode(hk_presentation::enum_mode s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::set_mode(s)");
#endif

if (!p_form->set_mode(s))return;
p_clearfilteraction->setEnabled(s==hk_presentation::filtermode);
    if ( s!=hk_presentation::filtermode )
      {
      p_filterdefinitionaction->setChecked(false);
       }
    if (mode()==hk_presentation::viewmode ||mode()==hk_presentation::filtermode)
    {
        p_copyaction->setEnabled(true);
        p_cutaction->setEnabled(false);
        p_pasteaction->setEnabled(true);
        p_deleteaction->setEnabled(false);
	p_dbdesigneraction->setEnabled(false);
    	p_imageaction->setEnabled(false);
    	p_tabwidgetaction->setEnabled(false);
    	p_dateaction->setEnabled(false);

    }
    else
    {                                             //designmode
        slot_focuswidget_changed();
	p_deleteaction->setEnabled(p_form->formfocus()&&p_form->formfocus()->widget());
        p_pasteaction->setEnabled(QApplication::clipboard()->mimeData() &&
            QApplication::clipboard()->mimeData()->hasFormat("application/x-hk_kdesimpleformcopy"));
        p_dbdesigneraction->setEnabled(true);
    }
    
    p_resizeaction->setEnabled(mode()==hk_presentation::designmode&&p_form->focus_multipleselected());
    p_alignaction->setEnabled(mode()==hk_presentation::designmode&&p_form->focus_multipleselected());

    if (s==hk_presentation::viewmode ||mode()==hk_presentation::filtermode)
    {
        //toolBar("designtoolbar")->hide();
        if (p_viewaction)p_viewaction->setChecked(true);
        if (s==hk_presentation::viewmode )toolbar->set_viewmode();
        p_bulkaction->setEnabled(false);
        p_formpart->setXMLFile("hk_kdeformpartview.rc");
        p_pointeraction->setEnabled(false);
        p_lineeditaction->setEnabled(false);
        p_labelaction->setEnabled(false);
        p_multilineeditaction->setEnabled(false);
        p_booleanaction->setEnabled(false);
        p_comboboxaction->setEnabled(false);
        p_gridaction->setEnabled(false);
        p_selectoraction->setEnabled(false);
        p_buttonaction->setEnabled(false);
        p_subformaction->setEnabled(false);
	p_formpropertyaction->setEnabled(false);
	p_taborderaction->setEnabled(false);
    }
    else
    {
        //toolBar("designtoolbar")->show();
        if (p_designaction)p_designaction->setChecked(true);
        toolbar->set_designmode();
        p_bulkaction->setEnabled(true);
        p_formpart->setXMLFile("hk_kdeformpartdesign.rc");
        p_pointeraction->setEnabled(true);
        p_lineeditaction->setEnabled(true);
        p_labelaction->setEnabled(true);
        p_multilineeditaction->setEnabled(true);
        p_booleanaction->setEnabled(true);
        p_comboboxaction->setEnabled(true);
        p_gridaction->setEnabled(true);
        p_selectoraction->setEnabled(true);
        p_buttonaction->setEnabled(true);
        p_subformaction->setEnabled(true);
	p_formpropertyaction->setEnabled(true);
	p_taborderaction->setEnabled(true);
    	p_imageaction->setEnabled(true);
    	p_tabwidgetaction->setEnabled(true);
    	p_dateaction->setEnabled(true);
    	p_form->setEnabled(true);
    	if (propertyeditor())propertyeditor()->setEnabled(true);
    }
    p_form->repaint(0,0,p_form->QWidget::width(),p_form->QWidget::height());

}


void hk_kdeformpartwidget::set_designmode(void)
{
    set_mode(hk_presentation::designmode);
}


void hk_kdeformpartwidget::set_viewmode(void)
{
    set_mode(hk_presentation::viewmode);
}


hk_dsgrid*      hk_kdeformpartwidget::new_grid(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::new_grid");
#endif
    hk_dsgrid* d=p_form->new_grid();
    return d;
}


hk_dslineedit*  hk_kdeformpartwidget::new_lineedit(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::new_field");
#endif
    hk_dslineedit* d=p_form->new_lineedit();
    return d;
}


hk_dsboolean*   hk_kdeformpartwidget::new_bool(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::new_bool");
#endif
    hk_dsboolean* d=p_form->new_bool();
    return d;
}


hk_dsmemo*  hk_kdeformpartwidget::new_memo(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::new_memo");
#endif
    hk_dsmemo* d=p_form->new_memo();
    return d;
}


hk_button*      hk_kdeformpartwidget::new_button(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::new_button");
#endif
    hk_button* d=p_form->new_button();
    return d;
}


hk_dsrowselector*       hk_kdeformpartwidget::new_rowselector(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::new_rowselector");
#endif
    hk_dsrowselector* d=p_form->new_rowselector();
    return d;
}


hk_subform*       hk_kdeformpartwidget::new_subform(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::new_subform");
#endif
    return p_form->new_subform();
}


void        hk_kdeformpartwidget::set_formsize(unsigned int width, unsigned int height)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::set_formsize");
#endif
    p_form->set_designsize(width,height);
}


void    hk_kdeformpartwidget::set_database(hk_database* db)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::database(db)");
#endif
    p_form->set_database(db);
}


hk_database*        hk_kdeformpartwidget::database(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::database");
#endif
    return p_form->database();
}


list<hk_datasource*>*   hk_kdeformpartwidget::datasources(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::datasources");
#endif
    return p_form->datasources();
}


long    hk_kdeformpartwidget::new_datasource(const hk_string& name,datasourcetype dt)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::new_datasource(name,query)");
#endif
    return p_form->new_datasource(name,dt);
}


hk_datasource*  hk_kdeformpartwidget::get_datasource(long nr)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::get:datasource(nr)");
#endif
    return p_form->get_datasource(nr);
}


void hk_kdeformpartwidget::formbutton_clicked(void)
{
    set_mode(hk_presentation::viewmode);

}


void hk_kdeformpartwidget::designbutton_clicked(void)
{
    if (p_nodesignmode)  set_mode(hk_form::viewmode);
    else set_mode(hk_form::designmode);

}


bool    hk_kdeformpartwidget::set_presentationdatasource(long n, bool r)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::set_presentationdatasource(n)");
#endif
    p_rowselector->set_presentationdatasource(n,false);
    bool res=true;
    toolbar->set_datasource(p_form->get_datasource(n));
    if (r) res= p_form->set_presentationdatasource(n,false);
    return res;
}


long    hk_kdeformpartwidget::presentationdatasource(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::presentationdatasource(n)");
#endif
    return p_form->presentationdatasource();
}


void hk_kdeformpartwidget::closeEvent ( QCloseEvent* e)
{

#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::closeEvent");
//set_generaldebug(true);
#endif
if (p_dontclose || p_form->while_load_form()) 
  {
    e->ignore();
    show_warningmessage("close while_loadform");
    return;
  }
save_form_when_changed();
 #ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::closeEvent nach abspeichern");
#endif
   emit signal_closed(this);
    QWidget::closeEvent(e);
}


void hk_kdeformpartwidget::save_form_when_changed(void)
{

   if (p_form &&p_form->has_changed())
    {

    if (propertyeditor())propertyeditor()->identifier_changed();
        if (toolbar->is_filter_activated())
            toolbar->deactivate_filter();
        p_form->save_form();
    }

}



void hk_kdeformpartwidget::close_form(void)
{

    close();
}


void hk_kdeformpartwidget::savedata(ofstream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::savedata(s)");
#endif
    p_form->savedata(s);

}


void hk_kdeformpartwidget::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::loaddata");
#endif
    p_form->loaddata(definition);
    p_rowselector->set_presentationdatasource(p_form->presentationdatasource());
}


void hk_kdeformpartwidget::load_form(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::load_form");
#endif

    if (database()==NULL)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Form error: No database defined!"));
        return ;

    }
//  p_form->formname(n);
    p_dontclose=true;
    p_form->setAutoFillBackground(true);
    p_form->load_form(n);    
    p_rowselector->set_presentationdatasource(p_form->presentationdatasource(),false);
    if (p_form->datasource())toolbar->set_filter(QString::fromUtf8(l2u(p_form->datasource()->temporaryfilter()).c_str()));
    set_caption();
    p_form -> enable_datasources(true);
    p_dontclose=false;
}


void hk_kdeformpartwidget::set_autoclose(bool c)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::set_autoclose");
#endif
    p_form->set_autoclose(c);
}


void hk_kdeformpartwidget::fieldbutton_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::fieldbutton_clicked");
#endif

    hk_visible::enum_visibletype thisfield =hk_visible::other;
    if (p_lineeditaction->isChecked()) thisfield=hk_visible::lineedit;
    else
    if (p_labelaction->isChecked()) thisfield=hk_visible::textlabel;
        else
        if (p_multilineeditaction->isChecked()) thisfield=hk_visible::memo;
            else
            if (p_comboboxaction->isChecked()) thisfield=hk_visible::combobox;
                else
                if (p_gridaction->isChecked()) thisfield=hk_visible::grid;
                    else
                    if (p_booleanaction->isChecked()) thisfield=hk_visible::boolean;
                        else
                        if (p_buttonaction->isChecked()) thisfield=hk_visible::button;
                            else
                            if (p_selectoraction->isChecked()) thisfield=hk_visible::rowselector;
                            else
                            if (p_subformaction->isChecked()) thisfield=hk_visible::subform;
                            else
                            if (p_imageaction->isChecked()) thisfield=hk_visible::image;
                            else
                            if (p_tabwidgetaction->isChecked()) thisfield=hk_visible::tabvisible;
                            else
                            if (p_dateaction->isChecked()) thisfield=hk_visible::date;
  p_form->set_field2create(thisfield);
}


void    hk_kdeformpartwidget::mousePressEvent(QMouseEvent*m)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::mousePressEvent");
#endif
    QWidget::mousePressEvent(m);

}


void hk_kdeformpartwidget::field_created(void)
{
    p_pointeraction->setChecked(true);
}


hk_kdesimpleform* hk_kdeformpartwidget::simpleform(void)
{
    return p_form;
}


void hk_kdeformpartwidget::set_nodesignmode(bool d)
{
    p_nodesignmode=d;
    toolbar->set_nodesignmode(d);

}


void hk_kdeformpartwidget::save_form(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::save_form");
#endif
/* if (toolbar->is_filter_activated())
    toolbar->deactivate_filter();
*/
    if (propertyeditor())propertyeditor()->identifier_changed();
    p_form->save_form("",false);
    set_caption();

}


void hk_kdeformpartwidget::saveas_form(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::save_form");
#endif
    if (toolbar->is_filter_activated())
        toolbar->deactivate_filter();

    hk_string oldname=p_form->hk_presentation::name();
    p_form->set_name("");
    if (p_form->save_form("",true))
    {
        set_caption();
    }
    else
    {
        p_form->set_name(oldname);

    }

}


void hk_kdeformpartwidget::set_caption(void)
{
    QString formname=i18n( "Form - "  );
    hk_datasource* d=p_form->datasource();
    
    if (d!=NULL)
    {
      // Note: The prefix of formname was driver name in knoda if parent was not KMDIWidget
       if(0) formname=QString::fromUtf8(l2u(d->database()->connection()->drivername()).c_str())+" "+formname;
    }
     formname+=QString::fromUtf8 (l2u(
        p_form->label().size()>0?
	p_form->label():
	p_form->hk_presentation::name()).c_str()
     );
    setWindowTitle( formname);
    emit signal_captionChanged(this, formname);
}


void hk_kdeformpartwidget::bulkfont_clicked(void)
{
    p_form->set_for_all(hk_presentation::bulkfont);
}


void hk_kdeformpartwidget::bulkforegroundcolour_clicked(void)
{
    p_form->set_for_all(hk_presentation::bulkforeground);

}


void hk_kdeformpartwidget::bulkbackgroundcolour_clicked(void)
{
    p_form->set_for_all(hk_presentation::bulkbackground);

}

hk_kdeproperty* hk_kdeformpartwidget::propertyeditor(void)
{
   return p_form->propertyeditor();
}



void hk_kdeformpartwidget::grid_partinfocus(hk_kdegridpart* )
{
//createGUI(p);
}

void hk_kdeformpartwidget::grid_partoutfocus(hk_kdegridpart*)
{
//createGUI(0L);

}


void hk_kdeformpartwidget::slot_focuswidget_changed(void)
{
    p_copyaction->setEnabled(p_form->formfocus()&&p_form->formfocus()->widget());
    p_cutaction->setEnabled(p_form->formfocus()&&p_form->formfocus()->widget());
    p_deleteaction->setEnabled(p_form->formfocus()&&p_form->formfocus()->widget());
    p_resizeaction->setEnabled(mode()==hk_presentation::designmode&&p_form->focus_multipleselected());
    p_alignaction->setEnabled(mode()==hk_presentation::designmode&&p_form->focus_multipleselected());
    QWidget* gridwidget=NULL;
    QWidget* object =p_form->formfocus()->widget();
    while (object!=NULL)
    {
       if ( dynamic_cast <hk_kdegrid*>(object)!=NULL  )
       {
        gridwidget=object;
        object=NULL;
       }
       else    object=object->parentWidget();
    }
    hk_kdegrid* gr=dynamic_cast<hk_kdegrid*>(gridwidget);
    p_definegridcolumnaction->setEnabled(mode()==hk_presentation::designmode&&gr);
}

void hk_kdeformpartwidget::slot_has_changed()
{
 if (p_saveaction)p_saveaction->setEnabled(!runtime_only()&& p_form->has_changed());

}

void hk_kdeformpartwidget::clipboard_changed()
{
        if (p_pasteaction) {
		QString mimeformat = p_form->mode() == hk_form::designmode ? 
		  QString::fromLatin1("application/x-hk_kdesimpleformcopy") : QString::fromLatin1("text/plain");
		
		p_pasteaction->setEnabled(QApplication::clipboard()->mimeData()
		  && QApplication::clipboard()->mimeData()->hasFormat(mimeformat));
	}
}


void hk_kdeformpartwidget::taborder_clicked()
{
  hk_kdetaborder* taborder=new hk_kdetaborder(p_form,0,0,true);
  
  if (!taborder) {
     show_warningmessage(hk_translate("hk_kdeformpartwidget::taborder_clicked error: taborder dialog could not be created!"));
     return;
  }
  taborder->exec();
  delete taborder;
}


void hk_kdeformpartwidget::show_widget(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::show");
#endif
  show();
}

void hk_kdeformpartwidget::hide_widget(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::hide");
#endif  
  hide();
}

bool hk_kdeformpartwidget::close_widget(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeformpartwidget::close");
#endif
  return close();
}


void hk_kdeformpartwidget::withlabelbutton_clicked(void)
{
  p_form->set_create_with_label(!p_form->create_with_label());
}

void hk_kdeformpartwidget::show_gridcolumndialog()
{
QWidget* gridwidget=NULL;
QWidget* object =p_form->formfocus()->widget();
while (object!=NULL)
{
   if ( dynamic_cast <hk_kdegrid*>(object)!=NULL  )
   {
        gridwidget=object;
        object=NULL;
   }
   else    object=object->parentWidget();
}
hk_kdegrid* gr=dynamic_cast<hk_kdegrid*>(gridwidget);
if (gr)
   gr->show_gridcolumndialog();

}



void hk_kdeformpartwidget::set_caller_form(const hk_string&f)
{
 p_callerform=f;
}

hk_string hk_kdeformpartwidget::caller_form(void) const
{
    return p_callerform;
}



void hk_kdeformpartwidget::filterdefinebutton_clicked(void)
{
 if (p_form->datasource() &&!p_filterdefinitionaction->isChecked())
 {
 bool r=p_form->set_formbasedfilter();
 if (toolbar)toolbar->set_filter(r);
//  cerr <<"SET FILTER: #"<<r<<"#"<<endl;
 }
//  if (p_filterdefinitionaction->isChecked()) p_filterexecaction->setEnabled(false);
p_form->set_mode(p_filterdefinitionaction->isChecked()?hk_form::filtermode:hk_form::viewmode);
}



void hk_kdeformpartwidget::clear_filter()
{
list<hk_visible*>* v=p_form->visibles();
list<hk_visible*>::iterator vit=v->begin();
while (vit!=v->end())
{
  hk_dsdatavisible* ds=dynamic_cast<hk_dsdatavisible*>(*vit);
  if (ds)
  {  
    ds->set_filtervalue("");
    ds->set_value("");
    ds->update_filter();
  }
  ++vit;
}

}

void hk_kdeformpartwidget::reload_form(void)
{
  unsigned long rp=0;
  if (p_form->datasource()) rp=p_form->datasource()->row_position();
  p_form->enable_datasources(false);
  p_form->enable_datasources(true);
  if (p_form->datasource() && rp>0) p_form->datasource()->goto_row(rp);
}

