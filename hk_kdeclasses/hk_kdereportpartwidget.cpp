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
//$Revision: 1.23 $

#include "hk_kdereportpartwidget.h"
#include "hk_kdetoolbar.h"
#include "hk_kdesimplereport.h"
#include "hk_kdereportsectiondialog.h"
#include "../hk_kdereportpart/hk_kdereportpart.h"
#include "hk_kdeformfocus.h"

#include <hk_database.h>
#include <hk_connection.h>
#include <hk_datasource.h>

#include <qscrollarea.h>
#include <qbuttongroup.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qstackedwidget.h>
#include <qclipboard.h>
#include <qapplication.h>
#include <qwidget.h>
#include <qfile.h>
#include <qactiongroup.h>
#include <QCloseEvent>
#include <QDomNodeList>

#include <kmenubar.h>
#include <kmenu.h>
#include <kaction.h>
#include <ktoggleaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <ktoolbar.h>
#include <kmimetypetrader.h>
#include <kmainwindow.h>
#include <kactionmenu.h>
#include <kxmlguifactory.h>
#include <KParts/ReadWritePart>

//TBP icons
 class hk_kdepreviewwindow: public KMainWindow
{
public:
    hk_kdepreviewwindow(QWidget * parent , const char * name = 0)
      :KMainWindow(parent)
    {
        if (name)
            setObjectName(QString::fromAscii(name));
        p_toolbar = toolBar("gvtoolbar");
        create_previewpart();
        add_actions();
    }
virtual ~hk_kdepreviewwindow()
    {
	    delete p_previewpart;
	}
    
void add_actions(void)
    {
        if(!p_previewpart) return;
        QDomNodeList l= p_previewpart->domDocument().elementsByTagName("ToolBar");
        if (l.length()==0) return;
        QDomNodeList a=l.item(0).toElement().elementsByTagName("Action");
        for (int i=0;i<a.length();++i)
        {
            QDomElement n=a.item(i).toElement();
            QAction* newact=p_previewpart->action(n);
            if (newact) p_toolbar -> addAction(newact);
        }
    }

void create_previewpart()
    {
	    QSizePolicy policy2(QSizePolicy::Expanding,QSizePolicy::Expanding);   
        KService::List offers=KMimeTypeTrader::self()->query("application/postscript","KParts/ReadOnlyPart"/* in ServiceTypes"*/);
        KService::List::Iterator it(offers.begin());
        
        for (;it!=offers.end();++it)
        {
            KService::Ptr service=(*it);
	        p_previewpart = service -> createInstance<KParts::ReadOnlyPart>(0);
            if (p_previewpart) break;
        }

        if (p_previewpart)
	    {
            setCentralWidget(p_previewpart->widget());
	        p_previewpart->widget()->setSizePolicy(policy2);
	    }
        else
            show_warningmessage(hk_class::hk_translate("Could not find a preview part!"));
    }

    KParts::ReadOnlyPart* p_previewpart;
    KToolBar* p_toolbar;
};


class hk_kdereportpartwidgetscrollview :public QScrollArea
{
public:
    hk_kdereportpartwidgetscrollview ( QWidget * parent=0 )
            :QScrollArea(parent),p_report(NULL)
    {
            
    }
	~hk_kdereportpartwidgetscrollview()
	{

	}
    
    void set_report(hk_kdesimplereport* r){ p_report=r; }

protected:
     virtual void viewportMousePressEvent ( QMouseEvent * )
     {
         if (p_report!=NULL) p_report->set_focus2property();
     }
     hk_kdesimplereport* p_report;
}; 

hk_kdereportpartwidget::hk_kdereportpartwidget (hk_kdereportpart* reportpart,QWidget* parent,  const char* name, Qt::WFlags fl )
: KXmlGuiWindow( parent, fl ) ,hk_class()
{
#ifdef HK_DEBUG
    //wanna_debug(true);
    hkclassname("hk_kdereportpartwidget");
    hkdebug("hk_kdereportpartwidget::hk_kdereportpartwidget");
#endif
    if (name)
        setObjectName(QString::fromAscii(name));
    setFocusPolicy(Qt::StrongFocus);
    p_reportpart=reportpart;
    p_previewwindow=NULL;
    resize( 800, 600 );
    KIconLoader* loader=KIconLoader::global();
    loader->addAppDir("hk_kde4classes");
    p_whilepreview=false;
    p_closewindow=false;
    QActionGroup* pag;
    QIcon::setThemeName("oxygen");
    
    if (runtime_only())
    {
      p_designaction=NULL;
      p_viewaction=NULL;
    }
    else
    {
      p_designaction=new KToggleAction(QIcon::fromTheme("document-edit"),i18n("&Design mode"),p_reportpart->actionCollection());
      p_reportpart->actionCollection()->addAction("designmode",p_designaction); 
      connect(p_designaction,SIGNAL(triggered()),this,SLOT(set_designmode()));
      p_designaction->setEnabled(!runtime_only());
      p_viewaction=new KToggleAction(QIcon::fromTheme("system-run"),i18n("&View mode"),p_reportpart->actionCollection());
      p_reportpart -> actionCollection() -> addAction("viewmode", p_viewaction);
      connect(p_viewaction,SIGNAL(triggered()),this,SLOT(set_viewmode()));
      pag = new QActionGroup(this);
      p_designaction -> setActionGroup(pag);
      p_viewaction -> setActionGroup(pag);
      p_designaction->setChecked(true);
    }
    p_printaction=new KAction(QIcon::fromTheme("document-print"),i18n("&Print"),p_reportpart->actionCollection());
    p_reportpart->actionCollection() -> addAction("printreport",p_printaction);
    connect(p_printaction,SIGNAL(triggered()),this,SLOT(print_report()));
    p_printaction->setEnabled(false);
    // TBP opravit ikonu p_sectionaction=new KAction(KIcon("reportsection",KIconLoader::global()),i18n("Sectionselect"),p_reportpart->actionCollection());
    p_sectionaction=new KAction(QIcon(),i18n("Sectionselect"),p_reportpart->actionCollection());
    p_reportpart->actionCollection() -> addAction("sectionselect",p_sectionaction);
    connect(p_sectionaction,SIGNAL(triggered()),SLOT(select_section()));
    p_sectionaction->setToolTip(i18n("sections"));
    p_saveaction=new KAction(QIcon::fromTheme("document-save"),i18n("&Save"),p_reportpart->actionCollection());
    p_reportpart->actionCollection() -> addAction("savereport",p_saveaction);
    connect(p_saveaction,SIGNAL(triggered()),this,SLOT(save_report()));
    p_saveaction->setEnabled(!runtime_only());
    p_saveasaction=new KAction(QIcon::fromTheme("document-save-as"),i18n("Save &as"),p_reportpart->actionCollection());
    p_reportpart->actionCollection() -> addAction("saveas",p_saveasaction);
    connect(p_saveasaction,SIGNAL(triggered()),this,SLOT(saveas_report()));
    p_saveasaction->setEnabled(!runtime_only());

    // TBP opravit ikonu p_pointeraction=new KToggleAction(KIcon("pfeil",KIconLoader::global()),i18n("Pointer"),p_reportpart->actionCollection());
    p_pointeraction=new KToggleAction(QIcon(),i18n("Pointer"),p_reportpart->actionCollection());
    p_reportpart->actionCollection() -> addAction("pointer",p_pointeraction);
    connect(p_pointeraction,SIGNAL(triggered()),this,SLOT(pointerbutton_clicked()));
// TBP opravit ikonu    p_fieldaction=new KToggleAction(KIcon("editline",KIconLoader::global()),i18n("Field"),p_reportpart->actionCollection());
    p_fieldaction=new KToggleAction(QIcon(),i18n("Field"),p_reportpart->actionCollection());
    p_reportpart->actionCollection() -> addAction("fieldbutton",p_fieldaction);
    connect(p_fieldaction,SIGNAL(triggered()),this,SLOT(fieldbutton_clicked()));
    p_pointeraction->setToolTip(i18n("select"));
    pag = new QActionGroup(this);
    p_pointeraction->setActionGroup(pag);
    p_fieldaction->setActionGroup(pag);
    p_fieldaction->setToolTip(i18n("new field"));
    p_pointeraction->setChecked(true);

    p_bulkaction=new KActionMenu(i18n("Set to all"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()-> addAction("bulk",p_bulkaction);
    p_bulkfontaction=new KAction(i18n("Font"),p_reportpart->actionCollection());
    p_reportpart->actionCollection() -> addAction("bulkfont",p_bulkfontaction);
    connect(p_bulkfontaction,SIGNAL(triggered()),this,SLOT(bulkfont_clicked()));
    p_bulkforegroundcolouraction=new KAction(i18n("Foregroundcolour"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()-> addAction("bulkforeground",p_bulkforegroundcolouraction);
    connect(p_bulkforegroundcolouraction,SIGNAL(triggered()),this,SLOT(bulkforegroundcolour_clicked()));
    p_bulkbackgroundcolouraction=new KAction(i18n("Backgroundcolour"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("bulkbackground",p_bulkbackgroundcolouraction);
    connect(p_bulkbackgroundcolouraction,SIGNAL(triggered()),this,SLOT(bulkbackgroundcolour_clicked()));    
      
    p_bulkaction->addAction(p_bulkfontaction);
    p_bulkaction->addAction(p_bulkforegroundcolouraction);
    p_bulkaction->addAction(p_bulkbackgroundcolouraction); 
   
    p_stack= new QStackedWidget(this);
    setCentralWidget(p_stack);

    p_scrollview = new hk_kdereportpartwidgetscrollview(p_stack);
    p_stack->addWidget(p_scrollview);
    p_report = new hk_kdesimplereport( p_scrollview->viewport() );
    p_report->set_reportpartwidget(this); 
    p_scrollview->set_report(p_report);
    set_caption();
// TBP opravit ikonu    p_reportpropertyaction=new KAction(KIcon("propertyeditor",KIconLoader::global()),i18n("&Propertyeditor"),p_reportpart->actionCollection());
    p_reportpropertyaction=new KAction(QIcon(),i18n("&Propertyeditor"),p_reportpart->actionCollection());
    p_reportpart->actionCollection() -> addAction("viewproperty",p_reportpropertyaction);
    connect(p_reportpropertyaction,SIGNAL(triggered()),p_report,SLOT(show_property()));

    p_deleteaction=new KAction(i18n("&Delete"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("delete",p_deleteaction);
    p_deleteaction->setShortcut(Qt::Key_Delete);
    connect(p_deleteaction,SIGNAL(triggered()),p_report,SLOT(delete_widgets()));
    p_copyaction= new KAction(i18n("&Copy"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("copy",p_copyaction);
    p_copyaction->setShortcut(Qt::CTRL+Qt::Key_C);
    connect(p_copyaction,SIGNAL(triggered()),p_report,SLOT(copy_widgets()));
    p_pasteaction=new KAction(i18n("&Paste"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("paste",p_pasteaction);
    p_pasteaction->setShortcut(Qt::CTRL+Qt::Key_V);
    connect(p_pasteaction,SIGNAL(triggered()),p_report,SLOT(paste_widgets()));
    p_cutaction=new KAction(i18n("Cu&t"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("cut",p_cutaction);
    p_cutaction->setShortcut(Qt::CTRL+Qt::Key_X);
    connect(p_cutaction,SIGNAL(triggered()),p_report,SLOT(cut_widgets()));
    p_resizeaction=new KActionMenu(i18n("Adjust &size"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("size",p_resizeaction);
    p_minwidthaction=new KAction(i18n("Minimum width"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("minwidth",p_minwidthaction);
    connect(p_minwidthaction,SIGNAL(triggered()),p_report,SLOT(adjust_minw()));
    p_maxwidthaction=new KAction(i18n("Maximum width"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("maxwidth",p_maxwidthaction);
    connect(p_maxwidthaction,SIGNAL(triggered()),p_report,SLOT(adjust_maxw()));
    p_minheightaction=new KAction(i18n("Minimum height"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("minheight",p_minheightaction);
    connect(p_minheightaction,SIGNAL(triggered()),p_report,SLOT(adjust_minh()));
    p_maxheightaction=new KAction(i18n("Maximum height"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("maxheight",p_maxheightaction);
    connect(p_maxheightaction,SIGNAL(triggered()),p_report,SLOT(adjust_maxh()));
    p_minsizeaction=new KAction(i18n("Minimum size"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("minsize",p_minsizeaction);
    connect(p_minsizeaction,SIGNAL(triggered()),p_report,SLOT(adjust_mins()));
    p_maxsizeaction=new KAction(i18n("Maximum size"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()-> addAction("maxsize",p_maxsizeaction);
    connect(p_maxsizeaction,SIGNAL(triggered()),p_report,SLOT(adjust_maxs()));
    p_resizeaction->addAction(p_minwidthaction);
    p_resizeaction->addAction(p_maxwidthaction);
    p_resizeaction->addAction(p_minheightaction);
    p_resizeaction->addAction(p_maxheightaction);
    p_resizeaction->addAction(p_minsizeaction);
    p_resizeaction->addAction(p_maxsizeaction);

    p_alignaction=new KActionMenu(i18n("&Align"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("align",p_alignaction);
    p_alignleftaction=new KAction(i18n("&Left"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("alignleft",p_alignleftaction);
    connect(p_alignleftaction,SIGNAL(triggered()),p_report,SLOT(align_left()));
    p_alignrightaction=new KAction(i18n("&Right"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("alignright",p_alignrightaction);
    connect(p_alignrightaction,SIGNAL(triggered()),p_report,SLOT(align_right()));
    p_aligntopaction=new KAction(i18n("&Top"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("aligntop",p_aligntopaction);
    connect(p_aligntopaction,SIGNAL(triggered()),p_report,SLOT(align_top()));
    p_alignbottomaction=new KAction(i18n("&Bottom"),p_reportpart->actionCollection());
    p_reportpart->actionCollection()->addAction("alignbottom",p_alignbottomaction);
    connect(p_alignbottomaction,SIGNAL(triggered()),p_report,SLOT(align_bottom()));
    p_alignaction->addAction(p_alignleftaction);
    p_alignaction->addAction(p_alignrightaction);
    p_alignaction->addAction(p_aligntopaction);
    p_alignaction->addAction(p_alignbottomaction); 

    connect(p_report,SIGNAL(signal_has_changed()),this,SLOT(slot_has_changed()));
    connect (QApplication::clipboard(), SIGNAL(dataChanged()),this, SLOT(clipboard_changed()));
    connect( p_report, SIGNAL( signal_focuswidget_changed() ), this, SLOT( slot_focuswidget_changed() ) );

    set_reporttoolbar();
    p_scrollview->setWidget(p_report);
    set_nodesignmode(runtime_only());
    p_report->set_reporttype("Postscript",false);
}


hk_kdereportpartwidget::~hk_kdereportpartwidget(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportpartwidget::~hk_kdereportpartwidget");
    hkdebug(QString("report callerform: %1").arg(caller_form().c_str()).toStdString());
#endif
     emit signal_raise_form(caller_form());
}


void hk_kdereportpartwidget::viewbutton_clicked(void)
{
    set_mode(hk_presentation::viewmode);
}


void hk_kdereportpartwidget::designbutton_clicked(void)
{
    if (p_nodesignmode)  set_mode(hk_presentation::viewmode);
    else set_mode(hk_presentation::designmode);

}


void hk_kdereportpartwidget::set_reporttoolbar(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportpartwidget::set_reporttoolbar");
#endif

    connect( p_report, SIGNAL( signalfield_created() ), this, SLOT( field_created() ) );

}


bool    hk_kdereportpartwidget::set_presentationdatasource(long n, bool r)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportpartwidget::set_presentationdatasource(n)");
#endif
    bool res=true;
    if (r) res= p_report->set_presentationdatasource(n,false);
    return res;
}


long    hk_kdereportpartwidget::presentationdatasource(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportpartwidget::presentationdatasource(n)");
#endif
   return p_report->presentationdatasource();
}


void hk_kdereportpartwidget::set_database(hk_database* db)
{
    if (!p_report) return;
    p_report->set_database(db);
    set_caption();
}


hk_database* hk_kdereportpartwidget::database(void)
{
    if (!p_report) return NULL;
    return p_report->database();
}

bool hk_kdereportpartwidget::while_executing(void) const
{
  return p_report->while_executing();
}

void hk_kdereportpartwidget::stop_execution(void)
{
   p_report->stop_execution();
}


void hk_kdereportpartwidget::closeEvent ( QCloseEvent* e)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportpartwidget::closeEvent");
#endif

 if (p_report->while_executing())
    {
      show_warningmessage(replace_all("%1",hk_translate("Cannot close now, because the report '%1' is executing. Cancel it first"),p_report->hk_report::name()));
      e->ignore();
      return;
    }
    if (p_report->has_changed())
    {
        p_report->save_report();
    }
    
    if(!p_whilepreview)
    {
        //setCentralWidget(NULL);
        if(p_previewwindow!=NULL)
        {
            p_stack->removeWidget(p_previewwindow);
	    delete p_previewwindow;
	    p_previewwindow=NULL;
        }
        QWidget::closeEvent(e);
    } else p_closewindow=true;
   emit signal_closed(this);
}

void hk_kdereportpartwidget::fieldbutton_clicked(void)
{
    p_report->set_field2create(p_fieldaction->isChecked());
}

void hk_kdereportpartwidget::set_nodesignmode(bool d)
{
    p_nodesignmode=d;

}

hk_presentation::enum_mode  hk_kdereportpartwidget::mode(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportpartwidget::mode");
#endif

  return p_report->mode();
}

void    hk_kdereportpartwidget::set_mode(hk_presentation::enum_mode s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportpartwidget::set_mode(s)");
#endif
    //if (s==p_report->mode()&&!runtime_only())return
    if (p_report->while_executing() && s==hk_presentation::designmode) {
        if (p_viewaction) p_viewaction->setChecked(true);
        return;
    }
    if (p_report->while_executing() && s==hk_presentation::viewmode) return;
    if (s==hk_presentation::viewmode) {
        if (!p_report->set_mode(s)) {
            if (p_designaction) p_designaction->setChecked(true);
            return;
        }
        //toolBar("designtoolbar")->hide();
        p_reportpropertyaction->setEnabled(false);
        p_pointeraction->setEnabled(false);
        p_fieldaction->setEnabled(false);
        p_sectionaction->setEnabled(false);

        if (p_viewaction) p_viewaction->setChecked(true);
        if (p_previewwindow!=NULL) {
	        //createGUI(0L);
	        p_stack->removeWidget(p_previewwindow);
	        delete p_previewwindow;
	        p_previewwindow=NULL;
        }
        p_previewwindow=new hk_kdepreviewwindow(this);
        if (p_previewwindow!=NULL) {
            p_previewwindow->setAttribute(Qt::WA_DeleteOnClose);
            p_whilepreview=true;
            p_stack->addWidget(p_previewwindow);
	        p_stack->setCurrentWidget(p_previewwindow);
            //createGUI(p_previewpart);
            if (p_previewwindow->p_previewpart) {
                p_previewwindow->p_previewpart->openUrl(QUrl::fromLocalFile(
                    QString::fromUtf8(l2u(p_report->outputfile()).c_str())) );
	        }  
            p_previewwindow->show();
	     //hkdebug("preview after show");
            p_whilepreview=false;
          if (p_closewindow) close();
          p_printaction->setEnabled(true);
          set_caption();
	     //hkdebug("preview after set caption");
        }

        if (p_reportpart -> factory()) {
	        QMenu* p_editmenu = dynamic_cast<QMenu*>(p_reportpart->factory()->container("edit",p_reportpart));
	
	        if (p_editmenu) p_editmenu->menuAction()->setVisible(false);
	        p_reportpropertyaction->setVisible(false);
	        p_pointeraction->setVisible(false);
	        p_fieldaction->setVisible(false);
	        p_sectionaction->setVisible(false);
        }
        p_copyaction->setEnabled(false);
        p_cutaction->setEnabled(false);
        p_pasteaction->setEnabled(false);
        p_deleteaction->setEnabled(false);
        p_bulkaction->setEnabled(false); 
    }
    else {                                             //design mode
        if (p_report->while_executing())
        p_report->stop_execution();
        p_report->set_mode(s);
        p_printaction->setEnabled(false);
        //createGUI(NULL);
        p_reportpart->setXMLFile("hk_kdereportpart.rc");
        if (p_reportpart -> factory()) {
            QMenu* p_editmenu = dynamic_cast<QMenu*>(p_reportpart->factory()->container("edit",p_reportpart));
	
            if (p_editmenu) p_editmenu->menuAction()->setVisible(true);
            p_reportpropertyaction -> setVisible(true);
            p_pointeraction -> setVisible(true);
            p_fieldaction -> setVisible(true);
            p_sectionaction -> setVisible(true);
        }
 
    if (p_designaction) p_designaction->setChecked(true);
        p_bulkaction->setEnabled(true);
    if (!p_whilepreview) {
        //toolBar("designtoolbar")->show();
      p_reportpropertyaction->setEnabled(true);
      p_pointeraction->setEnabled(true);
      p_fieldaction->setEnabled(true);
      p_sectionaction->setEnabled(true);
      p_stack->setCurrentWidget(p_scrollview);
     //p_report->set_mode(s);
    }
    else if (p_viewaction) p_viewaction->setChecked(true);
      p_copyaction->setEnabled(p_report->formfocus()&&p_report->formfocus()->widget());
      p_cutaction->setEnabled(p_report->formfocus()&&p_report->formfocus()->widget());
      p_deleteaction->setEnabled(p_report->formfocus()&&p_report->formfocus()->widget());
      p_pasteaction->setEnabled(p_report->canProcessClipboard());
  }
  p_resizeaction->setEnabled(p_report->mode()==hk_presentation::designmode&&p_report->focus_multipleselected());
  p_alignaction->setEnabled(p_report->mode()==hk_presentation::designmode&&p_report->focus_multipleselected()); 
  p_report->repaint(0,0,p_report->QWidget::width(),p_report->QWidget::height());
}


void hk_kdereportpartwidget::set_designmode(void)
{
    set_mode(hk_presentation::designmode);
}


void hk_kdereportpartwidget::set_viewmode(void)
{
    set_mode(hk_presentation::viewmode);
}


void hk_kdereportpartwidget::field_created(void)
{
    p_pointeraction->setChecked(true);
}


hk_kdesimplereport* hk_kdereportpartwidget::simplereport(void)
{
    return p_report;
}


void    hk_kdereportpartwidget::save_report(void)
{
    p_report->save_report("",false);
    set_caption();
}


void hk_kdereportpartwidget::saveas_report(void)
{
    hk_string oldname=p_report->hk_presentation::name();
    p_report->set_name("");
    if (p_report->save_report("",true))
    {
        set_caption();
    }
    else
    {

        p_report->set_name(oldname);
    } 

}


bool hk_kdereportpartwidget::load_report(const hk_string& name)
{
    bool res=p_report->load_report(name);
    p_report->reset_has_changed();
    set_caption();
    return res;
}


void hk_kdereportpartwidget::close_report(void)
{
    close();
}


void hk_kdereportpartwidget::slot_showtoolbar(void)
{

}


void hk_kdereportpartwidget::pointerbutton_clicked(void)
{

}


void hk_kdereportpartwidget::print_report(void)
{
    p_report->print_report(false);
}


void hk_kdereportpartwidget::select_section(void)
{
    p_report->clearfocus();
    /* TBP hk_kdereportsectiondialog* d= new hk_kdereportsectiondialog(p_report,this,0,true);
    d->exec();

    delete d; */
}


void hk_kdereportpartwidget::set_caption(void)
{

    QString reportname=i18n( "Report - "  );
    reportname+=QString::fromUtf8 (l2u(p_report->hk_presentation::name()).c_str());

    setWindowTitle( reportname  );
   emit signal_setcaption(reportname);
   emit signal_captionChanged(this, reportname);
}


void hk_kdereportpartwidget::bulkfont_clicked(void)
{
   p_report->set_for_all(hk_presentation::bulkfont);
}


void hk_kdereportpartwidget::bulkforegroundcolour_clicked(void)
{
    p_report->set_for_all(hk_presentation::bulkforeground);
}


void hk_kdereportpartwidget::bulkbackgroundcolour_clicked(void)
{
    p_report->set_for_all(hk_presentation::bulkbackground);
}


void hk_kdereportpartwidget::slot_has_changed()
{
  if (p_saveaction)p_saveaction->setEnabled(!runtime_only()&& p_report->has_changed());
}


void hk_kdereportpartwidget::clipboard_changed()
{
    if (p_pasteaction)
    p_pasteaction->setEnabled(p_report->canProcessClipboard());
}


void hk_kdereportpartwidget::slot_focuswidget_changed(void)
{
    p_copyaction->setEnabled(p_report->formfocus()&&p_report->formfocus()->widget());
    p_cutaction->setEnabled(p_report->formfocus()&&p_report->formfocus()->widget()); 
    p_deleteaction->setEnabled(p_report->formfocus()&&p_report->formfocus()->widget());
    if (p_resizeaction)
        p_resizeaction->setEnabled(p_report->mode()==hk_presentation::designmode&&p_report->focus_multipleselected());
    if (p_alignaction)
      p_alignaction->setEnabled(p_report->mode()==hk_presentation::designmode&&p_report->focus_multipleselected());
}

void hk_kdereportpartwidget::show_widget(void)
{
  show();
}

void hk_kdereportpartwidget::hide_widget(void)
{
  hide();
}

bool hk_kdereportpartwidget::close_widget(void)
{
  return close();
}

void hk_kdereportpartwidget::set_caller_form(const hk_string&f)
{
 p_callerform=f;
}

hk_string hk_kdereportpartwidget::caller_form(void) const
{
    return p_callerform;
}

