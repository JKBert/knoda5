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
#include "hk_kdequerypartwidget.h"
#include "hk_kdequerypartwidget.moc"
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qclipboard.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qprogressdialog.h>
#include <qcombobox.h>
#include <qapplication.h>
#include <klocale.h>
#include <hk_actionquery.h>
#include <hk_connection.h>
#include "hk_kdegrid.h"
#include "hk_kdeqbe.h"
#include <kservice.h>
#include <locale.h>

#include <kmenubar.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <ktoggleaction.h>
#include <kstandardaction.h>
#include <kparts/partmanager.h>
#include "../hk_kdequerypart/hk_kdequerypart.h"
#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <ktexteditor/highlightinterface.h>
#include <kmessagebox.h>

class hk_kdequerypartwidgetprivate
{
public:
  hk_kdequerypartwidgetprivate():p_autoclose(true), p_nodesignmode(true), p_grid(NULL), p_qbe(NULL),
    p_part(NULL), p_qbepart(NULL), p_view(NULL), p_document(NULL), highlighting_set(false), 
    p_asview(false), p_querypart(NULL) { }
  bool          p_autoclose;
  bool 	      p_nodesignmode;
  hk_kdegrid*   p_grid;
  hk_kdeqbe*      p_qbe;
  KParts::ReadWritePart* p_part;
  KParts::ReadWritePart* p_qbepart;
  KTextEditor::View*   p_view;
  KTextEditor::Document* p_document;
  bool          highlighting_set;
  bool          p_asview;
  hk_kdequerypart* p_querypart;
};


bool    hk_kdequerypartwidget::p_cancel=false;
QProgressDialog* hk_kdequerypartwidget::p_progressdialog=NULL;

/*
 *  Constructs a hk_kdequerypartwidget which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */

hk_kdequerypartwidget::hk_kdequerypartwidget(hk_kdequerypart* part,QWidget* w,const char* n,Qt::WFlags f):QStackedWidget(w),
  hk_dsquery(), p_designaction(NULL), p_viewaction(NULL), p_saveaction(NULL), p_saveasaction(NULL), p_printaction(NULL),
  p_reloadaction(NULL), p_copyaction(NULL), p_copybackendsqlaction(NULL), p_cutaction(NULL), p_undoaction(NULL),
  p_redoaction(NULL), p_pasteaction(NULL), p_replaceaction(NULL), p_findaction(NULL), p_findnextaction(NULL),
  p_findpreviousaction(NULL), p_selectallaction(NULL), p_storeresultaction(NULL), p_configeditoraction(NULL), 
  p_qbeaction(NULL), p_addaction(NULL), p_distinctaction(NULL), p_qbetypeselectaction(NULL),
  p_qbetypeselect(NULL), p_private(new hk_kdequerypartwidgetprivate)
{
#ifdef HK_DEBUG
    hkclassname("hk_kdequerypartwidget");
    hkdebug("hk_kdequerypartwidget::hk_kdequerypartwidget");
#endif
    setObjectName(n);
    setWindowFlags(windowFlags() | f); 
    p_private->p_querypart=part;
    setFocusPolicy(Qt::StrongFocus);
    
    KTextEditor::Editor* p_ed = KTextEditor::Editor::instance();
    if (p_ed != NULL) {
        p_private->p_document = p_ed->createDocument(0);
        if (p_private->p_document != NULL) {
          p_private->p_view = p_private->p_document->createView(this);
          if (p_private->p_view != NULL) 
              p_private->p_view->setFocus();
        }
    }
    
    setObjectName( "hk_kdequerypartwidget" );
    resize( 596, 480 );
    KService::Ptr service = KService::serviceByDesktopName("hk_kde5gridpart");
    if (!service || 
      !(p_private->p_part=service->createInstance<KParts::ReadWritePart>(this, this, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Grid part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
    }
    service = KService::serviceByDesktopName("hk_kde5qbepart");
    if (!service || 
     !(p_private->p_qbepart=service->createInstance<KParts::ReadWritePart>(w,w, QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! QBE part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
    }
    p_private->p_grid = (hk_kdegrid*)p_private->p_part->widget();
    p_private->p_grid->set_enablingbehaviour(true,true);
    addWidget(p_private->p_grid);
    p_private->p_qbe = (hk_kdeqbe*)p_private->p_qbepart->widget();
#ifdef HK_DEBUG
    p_private->p_qbe->hkclassname("QBE");
    p_private->p_grid->hkclassname("grid");
#endif
    if (p_private->p_view) {
        addWidget(p_private->p_view);
        connect( p_private->p_document, SIGNAL( textChanged(KTextEditor::Document*)), this, SLOT( query_changed() ) );
    }
    addWidget(p_private->p_qbe);
    set_nodesignmode(runtime_only());
    connect (p_private->p_qbe,SIGNAL(signal_qbe_has_changed()),this, SLOT(qbe_has_changed()));
    p_private->p_qbe->hide();
}


hk_kdequerypartwidget::~hk_kdequerypartwidget()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdequerypartwidget::~hk_kdequerypartwidget");
#endif
    if (p_private->p_view != NULL)
        removeWidget(p_private->p_view);
    if (p_private -> p_document) {
      delete p_private -> p_document;
      p_private -> p_document = NULL;
    }
    hk_datasource* d=p_private->p_grid->datasource();

    if (d!=NULL)
    {
        if (!d->presentation())d->disable();
        if (p_private->p_asview ) p_private->p_grid->save_view(d->name(),false);
    }
    delete p_private->p_qbepart;
    p_private->p_grid=NULL;
    p_private->p_qbe=NULL;
    delete p_designaction;
    delete p_viewaction;
    delete p_saveaction;
    delete p_saveasaction;
    delete p_private;
#ifdef HK_DEBUG
    hkdebug("hk_kdequerypartwidget::~hk_kdequerypartwidget END");
#endif

}

void hk_kdequerypartwidget::setupActions(KActionCollection* pac)
{
  QActionGroup* pag;
  KIconLoader loader (LIB_MODULE_NAME);  
  QIcon::setThemeName("oxygen");
  
  if (runtime_only())
  {
      p_designaction=NULL;
      p_viewaction=NULL;
  }
  else
  {
      p_designaction=new KToggleAction(QIcon::fromTheme("document-edit"),i18n("&Design mode"),pac);
      pac->addAction("designmode",p_designaction);
      connect(p_designaction,SIGNAL(triggered()),this,SLOT(designbutton_clicked()));
      p_designaction->setEnabled(!runtime_only());
      p_viewaction=new KToggleAction(QIcon::fromTheme("system-run"),i18n("&View mode"),pac);
      pac->addAction("viewmode",p_viewaction);
      connect(p_viewaction,SIGNAL(triggered()),this,SLOT(querybutton_clicked()));
      pag = new QActionGroup(this); // exclusive by default
      p_designaction->setActionGroup(pag);
      p_viewaction->setActionGroup(pag); 
    }
    
    p_printaction=new KAction(QIcon::fromTheme("document-print"),i18n("&Print"),pac);
    pac->addAction("print",p_printaction);
    connect(p_printaction,SIGNAL(triggered()),this,SLOT(print()));    
    p_reloadaction=new KAction(QIcon::fromTheme("view-refresh"),i18n("Reload"),pac);
    pac->addAction("reload",p_reloadaction);
    connect(p_reloadaction,SIGNAL(triggered()),this,SLOT(reload_query()));

    p_qbeaction=new KToggleAction(QIcon(loader.iconPath("dbdesigner",KIconLoader::User)),i18n("use &QBE"),pac);
    pac->addAction("useqbemode",p_qbeaction);
    connect(p_qbeaction,SIGNAL(triggered()),this,SLOT(action_useqbe()));
    p_qbeaction->blockSignals(true);
    p_qbeaction->setChecked(true);
    p_qbeaction->setEnabled(!runtime_only());
    p_qbeaction->blockSignals(false);

    p_saveaction=new KAction(QIcon::fromTheme("document-save"),i18n("&Save"),pac);
    pac->addAction("save",p_saveaction);
    connect(p_saveaction,SIGNAL(triggered()),this,SLOT(savebutton_clicked()));
    p_saveaction->setEnabled(false);

    p_saveasaction=new KAction(QIcon::fromTheme("document-save-as"),i18n("Save &as"),pac);
    pac->addAction("saveas",p_saveasaction);
    connect(p_saveasaction,SIGNAL(triggered()),this,SLOT(saveasbutton_clicked()));    
    p_saveasaction->setEnabled(!runtime_only());
    p_addaction = new KAction(QIcon(loader.iconPath("gridadd22x22",KIconLoader::User)),i18n("&Add datasource"),pac);
    pac->addAction("add",p_addaction);
    p_copybackendsqlaction=new KAction(i18n("&Copy backend SQL"),pac);
    pac->addAction("copybackendsql",p_copybackendsqlaction);
    p_copybackendsqlaction->setShortcut(Qt::AltModifier+Qt::Key_C);
    connect(p_copybackendsqlaction,SIGNAL(triggered()),this,SLOT(copybackendsql_clicked()));

    p_storeresultaction=new KAction(QIcon::fromTheme("document-save-as"),i18n("Store result"),pac);
    pac->addAction("storeresult",p_storeresultaction);
    connect(p_storeresultaction,SIGNAL(triggered()),this,SLOT(storeresult_clicked()));	
    
    p_distinctaction = new KToggleAction(QIcon::fromTheme("edit-table-cell-merge"),i18n("&Distinct rows"),pac);
    pac->addAction("distinct",p_distinctaction);
    connect(p_distinctaction,SIGNAL(triggered()),kdeqbe(),SLOT(distinct_changed()));     

    if (p_private->p_view != NULL) {
        p_cutaction=new KAction(QIcon::fromTheme("edit-cut"),i18n("Cu&t"),pac);
        pac->addAction("cut",p_cutaction);
        if (p_private->p_view->action("edit_cut") 
            && (Qt::ControlModifier+Qt::Key_X == p_private->p_view->action("edit_cut")->shortcut()))
            p_private->p_view->action("edit_cut")->setShortcut(QKeySequence());
        p_cutaction->setShortcut(Qt::ControlModifier+Qt::Key_X);
        connect(p_cutaction,SIGNAL(triggered()),this,SLOT(cut_clicked()));

        p_undoaction=new KAction(QIcon::fromTheme("edit-undo"),i18n("&Undo"),pac);
        pac->addAction("undo",p_undoaction);
        if (p_private->p_view->action("edit_undo")
            && (Qt::ControlModifier+Qt::Key_Z == p_private->p_view->action("edit_undo")->shortcut()))
            p_private->p_view->action("edit_undo")->setShortcut(QKeySequence());
        p_undoaction->setShortcut(Qt::ControlModifier+Qt::Key_Z);
        connect(p_undoaction,SIGNAL(triggered()),this,SLOT(undo_clicked()));     

        p_redoaction=new KAction(QIcon::fromTheme("edit-redo"), i18n("Re&do"),pac);
        pac->addAction("redo",p_redoaction);
        if (p_private->p_view->action("edit_redo")
            && (Qt::ControlModifier+Qt::ShiftModifier+Qt::Key_Z == p_private->p_view->action("edit_redo")->shortcut()))
            p_private->p_view->action("edit_redo")->setShortcut(QKeySequence());
        p_redoaction->setShortcut(Qt::ControlModifier+Qt::ShiftModifier+Qt::Key_Z);
        connect(p_redoaction,SIGNAL(triggered()),this,SLOT(redo_clicked())); 
        p_copyaction=new KAction(QIcon::fromTheme("edit-copy"),i18n("&Copy"),pac);
        pac->addAction("copy",p_copyaction);
        if (p_private->p_view->action("edit_copy")
            && (Qt::ControlModifier+Qt::Key_C == p_private->p_view->action("edit_copy")->shortcut()))
            p_private->p_view->action("edit_copy")->setShortcut(QKeySequence());
        p_copyaction->setShortcut(Qt::ControlModifier+Qt::Key_C);
        connect(p_copyaction,SIGNAL(triggered()),this,SLOT(copy_clicked()));	
	
        p_pasteaction=new KAction(QIcon::fromTheme("edit-paste"),i18n("&Paste"),pac);
        pac->addAction("paste",p_pasteaction);
        if (p_private->p_view->action("edit_paste")
            && (Qt::ControlModifier+Qt::Key_V == p_private->p_view->action("edit_paste")->shortcut()))
            p_private->p_view->action("edit_paste")->setShortcut(QKeySequence());
        p_pasteaction->setShortcut(Qt::ControlModifier+Qt::Key_V);
        connect(p_pasteaction,SIGNAL(triggered()),this,SLOT(paste_clicked()));	
        p_replaceaction=new KAction(i18n("&Replace"),pac);
        pac->addAction("replace",p_replaceaction);
        if (p_private->p_view->action("edit_replace")
            && (Qt::ControlModifier+Qt::Key_R == p_private->p_view->action("edit_replace")->shortcut()))
            p_private->p_view->action("edit_replace")->setShortcut(QKeySequence());
        p_replaceaction->setShortcut(Qt::ControlModifier+Qt::Key_R);
        connect(p_replaceaction,SIGNAL(triggered()),this,SLOT(replace_clicked()));	
        p_findaction=new KAction(QIcon::fromTheme("edit-find"),i18n("&Find"),pac);
        pac->addAction("find",p_findaction);
        if (p_private->p_view->action("edit_find")
            && (Qt::ControlModifier+Qt::Key_F == p_private->p_view->action("edit_find")->shortcut()))
            p_private->p_view->action("edit_find")->setShortcut(QKeySequence());    
        p_findaction->setShortcut(Qt::ControlModifier+Qt::Key_F);
        connect(p_findaction,SIGNAL(triggered()),this,SLOT(find_clicked()));	
	
        p_findnextaction=new KAction(QIcon::fromTheme("go-next"),i18n("Find &Next"),pac);
        pac->addAction("findnext",p_findnextaction);
        if (p_private->p_view->action("edit_find_next")
            && (Qt::Key_F3 == p_private->p_view->action("edit_find_next")->shortcut()))
            p_private->p_view->action("edit_find_next")->setShortcut(QKeySequence());     
        p_findnextaction->setShortcut(Qt::Key_F3);
        connect(p_findnextaction,SIGNAL(triggered()),this,SLOT(findnext_clicked()));
        p_findpreviousaction=new KAction(QIcon::fromTheme("go-previous"),i18n("Find Pre&vious"),pac);
        pac->addAction("findprevious",p_findpreviousaction);
        if (p_private->p_view->action("edit_find_prev")
            && (Qt::ShiftModifier+Qt::Key_F3 == p_private -> p_view->action("edit_find_prev")->shortcut()))
            p_private->p_view->action("edit_find_prev")->setShortcut(QKeySequence()); 
        p_findpreviousaction->setShortcut(Qt::ShiftModifier+Qt::Key_F3);
        connect(p_findpreviousaction,SIGNAL(triggered()),this,SLOT(findprevious_clicked()));

        p_selectallaction=new KAction(i18n("Select &All"),pac);
        pac->addAction("selectall",p_selectallaction);
        if (p_private->p_view->action("edit_select_all")
            && (Qt::ControlModifier+Qt::Key_A == p_private->p_view->action("edit_select_all")->shortcut()))
            p_private->p_view->action("edit_select_all")->setShortcut(QKeySequence());      
        p_selectallaction->setShortcut(Qt::ControlModifier+Qt::Key_A);
        connect(p_selectallaction,SIGNAL(triggered()),this,SLOT(selectall_clicked()));

        p_configeditoraction=new KAction(i18n("Configure &editor"),pac);
        pac->addAction("configeditor",p_configeditoraction);
        connect(p_configeditoraction,SIGNAL(triggered()),this,SLOT(configeditor_clicked()));
    }

    p_qbetypeselect= new QComboBox(NULL);
    p_qbetypeselect->setObjectName(QString::fromAscii("p_qbetypeselect"));
    p_qbetypeselect->addItem(i18n("Select query"));
    p_qbetypeselect->addItem(i18n("Groupselect query"));
    p_qbetypeselect->addItem(i18n("Update query"));
    p_qbetypeselect->addItem(i18n("Delete query"));
    p_qbetypeselectaction=new KAction(i18n("&Add datasource"),pac);
    p_qbetypeselectaction->setDefaultWidget(p_qbetypeselect);
    pac->addAction("qbetypeselect",p_qbetypeselectaction);
}


void hk_kdequerypartwidget::set_datasource(hk_datasource* d)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_datasource");
#endif
    hk_dsquery::set_datasource(d);
    p_private->p_grid->set_datasource(d);
    hk_database* db=NULL;
    p_private->p_asview=false;
    if (d) {
        db=d->database();

        if (d->type()==hk_datasource::ds_view) {
           p_private->p_asview=true;
           p_private->p_grid->load_view();
        }
    }
    p_private->p_qbe->set_database(db);
    if (d!=NULL) {
        if (d->sql().c_str() && p_private->p_document != NULL) {
            p_private->p_document->setText(QString::fromUtf8(l2u(d->sql()).c_str()));
       // To be fixed: when text loaded, undo action needs to be disabled
        }
    }
    reset_has_changed();

}

void hk_kdequerypartwidget::designbutton_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::designbutton_clicked");
#endif
    if (!set_mode(hk_dsmodevisible::designmode)){
        p_designaction->setChecked(false);
        p_viewaction->setChecked(true);
    }
}

void hk_kdequerypartwidget::internal_set_designmode(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::internal_set_designmode");
#endif

    if (p_private->p_nodesignmode)
    {
        querybutton_clicked();
        return;
    }
    hk_datasource* d=p_private->p_grid->datasource();
    if (d!=NULL) d->disable();

    if (p_qbetypeselectaction) p_qbetypeselectaction->setEnabled(use_qbe());
    if (p_qbetypeselect) p_qbetypeselect->setEnabled(use_qbe());
    if (p_distinctaction) p_distinctaction->setEnabled(use_qbe());
    if (p_addaction) p_addaction->setEnabled(use_qbe());
    if (p_copyaction) p_copyaction->setEnabled(!use_qbe());
    if (p_cutaction) p_cutaction->setEnabled(!use_qbe());
    if (p_undoaction) p_undoaction->setEnabled(!use_qbe());
    if (p_redoaction) p_redoaction->setEnabled(!use_qbe());
    if (p_pasteaction) p_pasteaction->setEnabled(!use_qbe());
    if (p_replaceaction) p_replaceaction->setEnabled(!use_qbe());
    if (p_findaction) p_findaction->setEnabled(!use_qbe());
    if (p_findnextaction) p_findnextaction->setEnabled(!use_qbe());
    if (p_findpreviousaction) p_findpreviousaction->setEnabled(!use_qbe());
    if (p_selectallaction) p_selectallaction->setEnabled(!use_qbe());
    if (p_configeditoraction) p_configeditoraction->setEnabled(!use_qbe());
    if (p_storeresultaction) p_storeresultaction->setEnabled(false);
    if (p_printaction) p_printaction->setEnabled(p_private->p_view != NULL);

    if (use_qbe())
    {
        p_private->p_querypart->setXMLFile("hk_kdequerypartqbe.rc");
        set_block_has_changed(true);
        setCurrentWidget(p_private->p_qbe);
        set_block_has_changed(false);
    }
    else
    {
        p_private->p_querypart->setXMLFile("hk_kdequerypartsql.rc");
        setCurrentWidget(p_private->p_view);
        p_private->p_view->setFocus();

        if (p_designaction)p_designaction->setChecked(true);

        if (!p_private->highlighting_set)
        {
            p_private->p_document->setHighlightingMode("SQL");
            p_private->highlighting_set=true;
        }
    } // end else if (use_qbe())
    if (p_designaction)
        p_designaction->setChecked(true);
    if (p_qbeaction)
    {
        p_qbeaction->blockSignals(true);
        p_qbeaction->setChecked(use_qbe());
        p_qbeaction->blockSignals(false);
        p_qbeaction->setEnabled(true);
    }
    set_caption();
}


void hk_kdequerypartwidget::querybutton_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::querybutton_clicked");
#endif
  set_mode(hk_dsmodevisible::viewmode);
}

void hk_kdequerypartwidget::internal_set_viewmode(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::internal_set_viewmode");
#endif
    if (p_qbetypeselectaction) p_qbetypeselectaction->setEnabled(false);
    if (p_qbetypeselect) p_qbetypeselect->setEnabled(false);
    if (p_distinctaction) p_distinctaction->setEnabled(false);
    if (p_addaction) p_addaction->setEnabled(false);

    if (p_copyaction) p_copyaction->setEnabled(false);
    if (p_cutaction) p_cutaction->setEnabled(false);
    if (p_undoaction) p_undoaction->setEnabled(false);
    if (p_redoaction) p_redoaction->setEnabled(false);
    if (p_pasteaction) p_pasteaction->setEnabled(false);
    if (p_replaceaction) p_replaceaction->setEnabled(false);
    if (p_findaction) p_findaction->setEnabled(false);
    if (p_findnextaction) p_findnextaction->setEnabled(false);
    if (p_findpreviousaction) p_findpreviousaction->setEnabled(false);
    if (p_selectallaction) p_selectallaction->setEnabled(false);
    if (p_configeditoraction) p_configeditoraction->setEnabled(false);
    if (p_qbeaction) p_qbeaction->setEnabled(false);
    if (p_storeresultaction) p_storeresultaction->setEnabled(true);
    if (p_printaction) p_printaction->setEnabled(true);

    hk_datasource* d=p_private->p_grid->datasource();
    bool actionquery=false;
    const QString& docutext = (p_private->p_document == NULL ? QString():p_private->p_document->text());

    if (!use_qbe() && p_private->p_document != NULL) { 
        hk_string tst_sql=u2l(docutext.toUtf8().data());
        hk_string separator=" \t\n";
        hk_string::size_type startpos=hk_string::npos;
        for (unsigned int tt=0;tt<tst_sql.size();tt++)
            tst_sql[tt]=toupper(tst_sql[tt]);
        startpos=tst_sql.find_first_not_of(separator);
        if (startpos == hk_string::npos) {
            set_designmode();
	    return;
        }
        hk_string::size_type endpos=tst_sql.find_first_of(separator,startpos+1);
        hk_string value=tst_sql.substr(startpos,endpos-startpos);

        actionquery=(value=="ALTER");
        if (!actionquery) actionquery=(value=="DROP");
        if (!actionquery) actionquery=(value=="INSERT");
        if (!actionquery) actionquery=(value=="UPDATE");
        if (!actionquery) actionquery=(value=="CREATE");
        if (!actionquery) actionquery=(value=="GRANT");
        if (!actionquery) actionquery=(value=="REVOKE");
        if (!actionquery) actionquery=(value=="FLUSH");
    }// !use_qbe end

    if (d!=NULL) {
        if ((!use_qbe() && !actionquery) ||
	    ((use_qbe()&& p_private->p_qbe &&
	    (p_private->p_qbe->querytype()==hk_qbe::qt_select
	    || p_private->p_qbe->querytype()==hk_qbe::qt_groupselect)))) {
            if (use_qbe() && p_private->p_qbe) {
                p_private->p_qbe->set_columnvalues();
	        if (d->set_query(p_private->p_qbe)) {
	            
                    if (d->sql().c_str() && p_private->p_document != NULL) {
                        p_private->p_document->blockSignals(true);
                        p_private->p_document->setText(QString::fromUtf8(l2u(d->sql()).c_str()));
                        p_private->p_document->blockSignals(false);
                    }
                    
	        } else {
	            if (!runtime_only()) {
		        set_designmode();
		        return;
		    }
	        }
	    } else {
	      if (p_private->p_document != NULL)
		d->set_sql(u2l(docutext.toUtf8().data()),d->type()!=hk_data::ds_view);
	    }
            d->set_progressdialog(&hk_kdequerypartwidget::set_progress);
            before_execute();
            d->disable();
            if ( !d->enable()) {
                after_execute();
                hk_string reason=hk_translate("Query could not be executed")+"\n"+hk_translate("Servermessage: ")+d->database()->connection()->last_servermessage();
                show_warningmessage(reason);
                if (!runtime_only()) {
                    set_designmode();
                    return;
                }
            }
            after_execute();
        } else {
            hk_actionquery* q=d->database()->new_actionquery();
            if (q!=NULL) {
                hk_string sql=u2l(docutext.toUtf8().data());
	        if (p_private->p_qbe && use_qbe()) {
	            p_private->p_qbe->set_columnvalues();
	            if(p_private->p_qbe->querytype()==hk_qbe::qt_update
	                || p_private->p_qbe->querytype()==hk_qbe::qt_delete)
                        if (!p_private->p_qbe->create_sql(sql)) {
	                    set_designmode();
		            return;
		        }
	            if (!show_yesnodialog(hk_translate(
"Warning: This function is not stable:\nAre you sure that you want to execute the following SQL code?\n\n")+sql,true)) {
	                set_designmode();
		        return;
	      	    }
  	        }
		q->set_sql(sql,true);
                if (q->execute())
                    show_warningmessage(hk_class::hk_translate("Query was successful"));
                else
                    show_warningmessage(hk_class::hk_translate("Query could not be executed")+"\n"+hk_translate("Servermessage: ")+d->database()->connection()->last_servermessage());
                delete q;
                if (!runtime_only()) {
                    set_designmode();
                    return;
                }
            }

        }
    }
    setCurrentWidget(p_private->p_grid);

    set_caption();
    if (p_viewaction)p_viewaction->setChecked(true);
        p_private->p_querypart->setXMLFile("hk_kdequerypart.rc");
}


bool  hk_kdequerypartwidget::set_mode(enum_mode s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_mode");
#endif
    if (s == hk_dsmodevisible::designmode && datasource()->type() == hk_data::ds_view
      && p_private->p_document == NULL) {
      // do not allow design mode for view if we do not have text editor
        KMessageBox::error(this,i18n("A KDE text-editor component could not be found;\n"
                                  "please check your KDE installation."));
            return false;      
    }
      
    hk_dsmodevisible::set_mode(s);
    switch (s){
        case hk_dsmodevisible::designmode :
        case hk_dsmodevisible::filtermode :
            internal_set_designmode();
            break;
        case hk_dsmodevisible::viewmode :
            internal_set_viewmode();
            break;
    }

    return true;
}

void hk_kdequerypartwidget::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdequerypartwidget::before_source_vanishes");
#endif
    if (has_changed())
    {
        save_query();
        reset_has_changed();

    }

    if (p_private->p_autoclose)
        close();
    else
    {
        hk_datasource* d=datasource();
        if (d!=NULL)d->disable();
        set_datasource(NULL);

    }
    //hk_dsquery::before_source_vanishes();
}


void hk_kdequerypartwidget::query_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::query_changed");
#endif
    set_has_changed();
    //p_private->p_saveaction->setEnabled(true);

    if (datasource()!=NULL)  datasource()->set_sql(u2l(p_private->p_document->text().toUtf8().data()),true);
}


void hk_kdequerypartwidget::closeEvent ( QCloseEvent* e)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::closeEvent");
#endif
    if (has_changed())
    {
        save_query();
        reset_has_changed();

    }

  QStackedWidget::closeEvent(e);
  emit signal_closed(this);

}


void hk_kdequerypartwidget::set_nodesignmode(bool d)
{
    p_private->p_nodesignmode=d;

}


bool hk_kdequerypartwidget::save_query(const hk_string& n,bool ask)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::save_query");
#endif
    set_caption();
    if (p_private->p_qbe) p_private->p_qbe->set_columnvalues();
    if (use_qbe()&& p_private->p_qbe)
	    {
	      p_private->p_grid->datasource()->set_query(p_private->p_qbe);
	    }
     return hk_dsquery::save_query(n,ask);
}


bool hk_kdequerypartwidget::load_query(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::load_query");
#endif
    set_block_has_changed(true);

    bool r= hk_dsquery::load_query();
    hk_datasource* d=datasource();
    if (d==NULL) return false;

    if (d->sql().c_str() && p_private->p_document != NULL) {
      p_private->p_document->setText(QString::fromUtf8(l2u(d->sql()).c_str()));
      // To be fixed: when text loaded, undo action needs to be disabled
    }
    p_private->p_grid->set_font(hk_font());
    set_caption();
    reset_has_changed();
    set_block_has_changed(false);
    p_saveaction->setEnabled(false);
    return r;
}


void hk_kdequerypartwidget::savebutton_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::savebutton_clicked");
#endif
    if (save_query("",false))
    {
        reset_has_changed();
        //p_private->p_saveaction->setEnabled(false);
    }
    set_caption();
}


void hk_kdequerypartwidget::saveasbutton_clicked(void)
{
    if (!p_private->p_grid->datasource()) return;
    hk_string oldname=p_private->p_grid->datasource()->name();
    p_private->p_grid->datasource()->set_name("");
    if (save_query("",true))
    {
        reset_has_changed();

        set_caption();

    }
    else
    {
        p_private->p_grid->datasource()->set_name(oldname);

    }

}


void hk_kdequerypartwidget::close_query(void)
{
    close();
}


void hk_kdequerypartwidget::set_caption(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_caption");
#endif
    hk_datasource* d=datasource();
    if (d!=NULL)
    {
        QString name=((d->type()==hk_datasource::ds_view)? i18n("View - "):i18n("Query - "));
        name+=QString::fromUtf8(l2u(d->name()).c_str());
	//
	if(0) name=QString::fromUtf8(l2u(d->database()->connection()->drivername()).c_str())+" "+name;
        setWindowTitle(name);
	emit signal_captionChanged(this, name);
     }

}


void hk_kdequerypartwidget::print(void)
{

#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::print");
#endif
    if (in_designmode())
    {
// this is a workaround: otherwise float numbers in the postscript file
// would be created with locale dots (e.g. a comma in Germany) from Qt
      hk_string origlocale=setlocale(LC_NUMERIC,NULL);
      hk_string origmonetarylocale=setlocale(LC_MONETARY,NULL);
      setlocale(LC_NUMERIC,"C");
      setlocale(LC_MONETARY,"C");
      p_private -> p_view->action("file_print") ->trigger();
//workaround part 2
      setlocale(LC_NUMERIC,origlocale.c_str());
      setlocale(LC_MONETARY,origmonetarylocale.c_str());
//workaround part 2 end
    }
    else
        p_private->p_grid->print_grid();
}

hk_dsgrid *hk_kdequerypartwidget::grid(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::grid");
#endif
return p_private->p_grid;
}



bool hk_kdequerypartwidget::in_designmode(void) const
{
return mode()==hk_dsmodevisible::designmode;
}


void hk_kdequerypartwidget::set_autoclose(bool c)
{
p_private->p_autoclose=c;
}


bool hk_kdequerypartwidget::autoclose(void) const
{
return p_private->p_autoclose;

}

void hk_kdequerypartwidget::action_useqbe(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::action_useqbe");
#endif
    if (use_qbe()) {
        // toggle QBE->Text editor
        if (p_private->p_view == NULL) {
            KMessageBox::error(this,i18n("A KDE text-editor component could not be found;\n"
                                  "please check your KDE installation."));
            p_qbeaction->setChecked(true);
            return;
        }
    } else {
        // toggle Text editor->QBE
        if (!show_yesnodialog(
          hk_translate("If you change to the QBE window your SQL statement will be lost. Continue?"),true)) {
            p_qbeaction->blockSignals(true);
            p_qbeaction->setChecked(use_qbe());
            p_qbeaction->blockSignals(false);
            return;
        }
    }

    set_use_qbe(!use_qbe());

    if (in_designmode()) designbutton_clicked();
    p_qbeaction->blockSignals(true);
    p_qbeaction->setChecked(use_qbe());
    p_qbeaction->blockSignals(false);
    emit signal_qbetypechanged();
}


hk_qbe*  hk_kdequerypartwidget::qbe(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdequerypartwidget::qbe()");
#endif
 return p_private->p_qbe;
}

hk_kdeqbe*  hk_kdequerypartwidget::kdeqbe(void) const
{
#ifdef HK_DEBUG
    hkdebug("hk_kdequerypartwidget::kdeqbe()");
#endif
 return p_private->p_qbe;
}


void hk_kdequerypartwidget::qbe_has_changed(void)
{
  set_has_changed();
}


void hk_kdequerypartwidget::show()
{
  set_block_has_changed(true);
  QStackedWidget::show();
  set_block_has_changed(false);
}

void hk_kdequerypartwidget::showMaximized()
{
  set_block_has_changed(true);
  QStackedWidget::showMaximized();
  set_block_has_changed(false);

}

void hk_kdequerypartwidget::set_has_changed(enum_has_changed forcesetting)
{
   hk_dsquery::set_has_changed(forcesetting);
   if (p_saveaction)p_saveaction->setEnabled(!runtime_only()&& has_changed());
}



void hk_kdequerypartwidget::reset_has_changed(void)
{
  hk_dsquery::reset_has_changed();
  if (p_saveaction)p_saveaction->setEnabled(!runtime_only()&& has_changed());
}


void hk_kdequerypartwidget::copy_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_copy"))
    p_private -> p_view->action("edit_copy") -> trigger();
}


void hk_kdequerypartwidget::copybackendsql_clicked(void)
{
if (datasource())
   {
	QApplication::clipboard()->setText(QString::fromUtf8(l2u(datasource()->backendsql()).c_str()));
   }
}

void hk_kdequerypartwidget::paste_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_paste"))
    p_private -> p_view->action("edit_paste") -> trigger();
}

void hk_kdequerypartwidget::replace_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_replace"))
    p_private -> p_view->action("edit_replace") -> trigger();
}

void hk_kdequerypartwidget::find_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_find"))
    p_private -> p_view->action("edit_find") -> trigger();
}

void hk_kdequerypartwidget::findnext_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_find_next"))
    p_private -> p_view->action("edit_find_next") -> trigger();
}

void hk_kdequerypartwidget::findprevious_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_find_prev"))
    p_private -> p_view->action("edit_find_prev") -> trigger();
}

void hk_kdequerypartwidget::cut_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_cut"))
    p_private -> p_view->action("edit_cut") -> trigger();
}

void hk_kdequerypartwidget::undo_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_undo"))
    p_private -> p_view->action("edit_undo") -> trigger();
}

void hk_kdequerypartwidget::redo_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_redo"))
    p_private -> p_view->action("edit_redo") -> trigger();
}

void hk_kdequerypartwidget::selectall_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("edit_select_all"))
    p_private -> p_view->action("edit_select_all") -> trigger();
}

void hk_kdequerypartwidget::configeditor_clicked(void)
{
  if (p_private -> p_view && p_private -> p_view->action("set_confdlg"))
    p_private -> p_view->action("set_confdlg") -> trigger();
}


void hk_kdequerypartwidget::show_widget(void)
{
  show();
}

void hk_kdequerypartwidget::hide_widget(void)
{
  hide();
}

bool hk_kdequerypartwidget::close_widget(void)
{
  return close();
}


void hk_kdequerypartwidget::storeresult_clicked(void)
{
  if (!datasource()) return;
  datasource()->database()->copy_table(datasource(),true,false,true);
}




void    hk_kdequerypartwidget::before_execute(void)
{
   p_cancel=false;
   p_progressdialog= new  QProgressDialog(this);
   p_progressdialog->setWindowTitle(i18n("Executing query ..."));
   p_progressdialog->setMaximum(100);
   p_progressdialog->setValue(0);
   connect(p_progressdialog,SIGNAL(canceled()),this,SLOT(exec_cancelled()));
   p_progressdialog->show();
   qApp->processEvents();
}


void    hk_kdequerypartwidget::after_execute(void)
{
  delete p_progressdialog;
  p_progressdialog=NULL;
}


bool hk_kdequerypartwidget::set_progress(long int position,long int total,const hk_string&txt)
{
    if (p_progressdialog)
    {

        p_progressdialog->setWindowTitle(QString::fromUtf8(l2u(txt).c_str()));
        p_progressdialog->setMaximum(total);
        p_progressdialog->setValue(position);
        p_progressdialog->raise();
        qApp->processEvents();
    }
    return p_cancel;
}

void hk_kdequerypartwidget::exec_cancelled(void)
{
    p_cancel=true;

}



void hk_kdequerypartwidget::reload_query()
{

    hk_datasource* d=p_private->p_grid->datasource();

    if (d!=NULL)
    {
     unsigned long rp=d->row_position();

     d->disable();
     d->enable();
     if (rp>0)
       d->goto_row(rp);
    }
}


