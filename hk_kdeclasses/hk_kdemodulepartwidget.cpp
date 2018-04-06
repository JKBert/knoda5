// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
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
//$Revision: 1.3 $

#include "hk_kdemodulepartwidget.h"
#include "hk_kdemodulepartwidget.moc"
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
#include <klocale.h>
#include <hk_connection.h>
#include <locale.h>

#include <kmenubar.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <kparts/partmanager.h>
#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <ktexteditor/highlightinterface.h>
#include "../hk_kdemodulepart/hk_kdemodulepart.h"
#include <kactioncollection.h>
#include <kmessagebox.h>

class hk_kdemodulepartwidgetprivate
{
public:
  hk_kdemodulepartwidgetprivate(): p_autoclose(true), p_document(NULL), p_view(NULL),
     p_modulepart(NULL) {}
        bool          p_autoclose;
	KTextEditor::Document* p_document;
	KTextEditor::View*  p_view;
	hk_kdemodulepart* p_modulepart;
};

/*
 *  Constructs a hk_kdemodulepartwidget which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */

hk_kdemodulepartwidget::hk_kdemodulepartwidget(hk_kdemodulepart* part,QWidget* w,const char* n,Qt::WFlags f):QWidget(w, f),hk_module()
  ,p_saveaction(NULL), p_saveasaction(NULL), p_printaction(NULL), p_copyaction(NULL), p_cutaction(NULL),
  p_undoaction(NULL), p_redoaction(NULL), p_pasteaction(NULL), p_replaceaction(NULL), p_findaction(NULL),
  p_findnextaction(NULL), p_findpreviousaction(NULL), p_selectallaction(NULL), p_storeresultaction(NULL),
  p_configeditoraction(NULL),p_private(new hk_kdemodulepartwidgetprivate) 
{
#ifdef HK_DEBUG
  hkdebug("hk_kdemodulepartwidget::hk_kdemodulepartwidget");
  hkclassname("hk_kdemodulepartwidget");
#endif
  p_private->p_modulepart=part;
  KIconLoader* loader=KIconLoader::global();
  loader->addAppDir("hk_kde5classes");
  setFocusPolicy(Qt::StrongFocus);
  setLayout(new QHBoxLayout(this));

  setObjectName(QString::fromAscii(n == NULL?"hk_kdemodulepartwidget":n));
  resize( 596, 480 );
  KTextEditor::Editor* p_ed = KTextEditor::Editor::instance();
  if (p_ed != NULL) {
    p_private->p_document = p_ed->createDocument(0);
    
    if (p_private->p_document != NULL) {
      p_private->p_view = p_private-> p_document->createView(this);
      layout()->addWidget(p_private -> p_view);
      setFocusProxy(p_private -> p_view);

      connect( p_private->p_document, SIGNAL( textChanged(KTextEditor::Document*)) , this, SLOT( module_changed() ) );
      p_private->p_document->setHighlightingMode("Python");
    }
  } else
    KMessageBox::error(w,i18n("A KDE text-editor component could not be found;\n"
                                  "please check your KDE installation."));
}


hk_kdemodulepartwidget::~hk_kdemodulepartwidget()
{
#ifdef HK_DEBUG
  hkdebug("hk_kdemodulepartwidget::~hk_kdemodulepartwidget");
#endif
  if (p_private -> p_document ) {
    delete p_private -> p_document ;
    p_private -> p_document  = NULL;
  }
  delete p_private;
}

void hk_kdemodulepartwidget::setupActions(KActionCollection* ac)
{
  if ( !p_private->p_view ) return;
  
  QAction* pAct;
  KActionCollection *viewActions = p_private->p_view->actionCollection();
  
  QIcon::setThemeName("oxygen");
  
  // remove colliding actions
  viewActions->removeAction(viewActions->action(KStandardAction::stdName(KStandardAction::Save)));
  viewActions->removeAction(viewActions->action(KStandardAction::stdName(KStandardAction::SaveAs)));
  
  p_printaction=new KAction(QIcon::fromTheme("document-print"),i18n("&Print"),ac);
  ac->addAction("print",p_printaction);
  connect(p_printaction,SIGNAL(triggered()),this,SLOT(print()));    
   
  p_saveaction=new KAction(QIcon::fromTheme("document-save"),i18n("&Save"),ac);
  p_saveaction -> setShortcut(QKeySequence(QKeySequence::Save)); 
  ac->addAction("save",p_saveaction);
  connect(p_saveaction,SIGNAL(triggered()),this,SLOT(savebutton_clicked()));
  p_saveaction->setEnabled(false);
 
  p_saveasaction=new KAction(QIcon::fromTheme("document-save-as"),i18n("Save &as"),ac);
  p_saveasaction -> setShortcut(QKeySequence(QKeySequence::SaveAs));
  ac->addAction("saveas",p_saveasaction);
  connect(p_saveasaction,SIGNAL(triggered()),this,SLOT(saveasbutton_clicked()));
  p_saveasaction->setEnabled(!runtime_only());

  p_cutaction=new KAction(QIcon::fromTheme("edit-cut"),i18n("Cu&t"),ac);
  p_cutaction->setShortcut(Qt::CTRL+Qt::Key_X); 
  ac->addAction("cut",p_cutaction);
  pAct = p_private -> p_view->action("edit_cut");
  if(pAct) {
    pAct-> setShortcut(QKeySequence());
    connect(p_cutaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_cutaction->setEnabled(false);

  p_undoaction=new KAction(QIcon::fromTheme("edit-undo"),i18n("&Undo"),ac);
  p_undoaction -> setShortcut(Qt::CTRL+Qt::Key_Z); 
  ac->addAction("undo",p_undoaction);
  pAct = p_private -> p_view->action("edit_undo");
  if(pAct) {
    pAct-> setShortcut(QKeySequence());
   connect(p_undoaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_undoaction->setEnabled(false);
  
  p_redoaction=new KAction(QIcon::fromTheme("edit-redo"),i18n("Re&do"),ac);
  p_redoaction -> setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_Z); 
  ac-> addAction("redo",p_redoaction);
  pAct = p_private->p_view->action("edit_redo");
  if(pAct) {
    pAct-> setShortcut(QKeySequence());
    connect(p_redoaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_redoaction->setEnabled(false);

  p_copyaction=new KAction(QIcon::fromTheme("edit-copy"),i18n("&Copy"),ac);
  p_copyaction -> setShortcut(Qt::CTRL+Qt::Key_C); 
  ac-> addAction("copy",p_copyaction);
  pAct = p_private -> p_view->action("edit_copy");
  if(pAct) {
    pAct-> setShortcut(QKeySequence());
    connect(p_copyaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_copyaction -> setEnabled(false);
  
  p_pasteaction=new KAction(QIcon::fromTheme("edit-paste"),i18n("&Paste"),ac);
  p_pasteaction -> setShortcut(Qt::CTRL+Qt::Key_V); 
  ac-> addAction("paste",p_pasteaction);
  pAct = p_private -> p_view -> action("edit_paste");
  if(pAct){
    pAct-> setShortcut(QKeySequence());
    connect(p_pasteaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_pasteaction->setEnabled(false);

  p_replaceaction=new KAction(QIcon::fromTheme("edit-find-replace"),i18n("&Replace"),ac);
  p_replaceaction -> setShortcut(Qt::CTRL+Qt::Key_R); 
  ac-> addAction("replace",p_replaceaction);
  pAct = p_private -> p_view -> action("edit_replace");
  if(pAct){
    pAct-> setShortcut(QKeySequence());  
    connect(p_replaceaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_pasteaction->setEnabled(false);

  p_findaction=new KAction(QIcon::fromTheme("edit-find"),i18n("&Find"),ac); 
  p_findaction->setShortcut(Qt::CTRL+Qt::Key_F); 
  ac-> addAction("find",p_findaction);
  pAct = p_private -> p_view -> action("edit_find");
  if(pAct){
    pAct-> setShortcut(QKeySequence());  
    connect(p_findaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_findaction->setEnabled(false);
  
  p_findnextaction=new KAction(QIcon::fromTheme("go-next"),i18n("Find &Next"),ac);
  p_findnextaction -> setShortcut(Qt::Key_F3); 
  ac-> addAction("findnext",p_findnextaction);
  pAct = p_private -> p_view -> action("edit_find_next");
  if(pAct){
    pAct-> setShortcut(QKeySequence()); 
    connect(p_findnextaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_findnextaction->setEnabled(false);
  
  p_findpreviousaction=new KAction(QIcon::fromTheme("go-previous"),i18n("Find Pre&vious"),ac);
  p_findpreviousaction -> setShortcut(Qt::SHIFT+Qt::Key_F3); 
  ac-> addAction("findprevious",p_findpreviousaction);
  pAct = p_private -> p_view -> action("edit_find_prev");
  if(pAct){
    pAct-> setShortcut(QKeySequence()); 
    connect(p_findpreviousaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_findpreviousaction->setEnabled(false);
 
  p_selectallaction=new KAction(i18n("Select &All"),ac);
  p_selectallaction -> setShortcut(Qt::CTRL+Qt::Key_A); 
  ac-> addAction("selectall",p_selectallaction);
  pAct = p_private -> p_view -> action("edit_select_all");
  if(pAct){
    pAct-> setShortcut(QKeySequence()); 
    connect(p_selectallaction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  }
  else
    p_selectallaction->setEnabled(false);
 
  p_configeditoraction=new KAction(i18n("Configure &editor"),ac);
  ac-> addAction("configeditor",p_configeditoraction);
  pAct = p_private -> p_view -> action("set_confdlg");
  if(pAct) 
    connect(p_configeditoraction,SIGNAL(triggered()),pAct,SLOT(trigger()));
  else
    p_configeditoraction->setEnabled(false);
}

void hk_kdemodulepartwidget::before_source_vanishes(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodulepartwidget::before_source_vanishes");
#endif
    if (has_changed())
    {
        save_module();
        reset_has_changed();

    }

    if (p_private->p_autoclose)
        close();
}


void hk_kdemodulepartwidget::module_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::module_changed");
#endif
    set_has_changed();
    if (p_saveaction != NULL)
        p_saveaction->setEnabled(true);
    set_script(u2l(p_private->p_document->text().toUtf8().data()));

}

bool hk_kdemodulepartwidget::save_module(const hk_string& n,bool ask)
{
  bool result=hk_module::save_module(n,ask);
  
  return result;
}

bool hk_kdemodulepartwidget::load_module(const hk_string& n)
{
  set_block_has_changed(true);
  bool result=hk_module::load_module(n);
  
  if (script().c_str() && p_private->p_document != NULL) {
    p_private->p_document->setText(QString::fromUtf8(l2u(script()).c_str()));
   // To be fixed: when text loaded, undo action needs to be disabled
  }
  set_caption();
  reset_has_changed();
  set_block_has_changed(false);
  return result;
}

void hk_kdemodulepartwidget::closeEvent ( QCloseEvent* e)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::closeEvent");
#endif
    if (has_changed())
    {
        save_module();
        reset_has_changed();

    }

    QWidget::closeEvent(e);
}





void hk_kdemodulepartwidget::savebutton_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdemodule::savebutton_clicked");
#endif
    if (save_module("",false))
    {
        reset_has_changed();
        //p_private->p_saveaction->setEnabled(false);
    }
    set_caption();
}


void hk_kdemodulepartwidget::saveasbutton_clicked(void)
{
  if (save_module("",true))
    {
        reset_has_changed();

        set_caption();

    }

}


void hk_kdemodulepartwidget::close_module(void)
{
    close();
}


void hk_kdemodulepartwidget::set_caption(void)
{
#ifdef HK_DEBUG
  hkdebug("hk_kdemodule::set_caption");
#endif
  QString n=i18n("Module - ");
  n+=QString::fromUtf8(l2u(hk_module::name()).c_str());
  setWindowTitle(n);
  emit signal_captionChanged(this, n);
}


void hk_kdemodulepartwidget::print(void)
{
#ifdef HK_DEBUG
  hkdebug("hk_kdemodule::print");
#endif
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


void    hk_kdemodulepartwidget::keyPressEvent ( QKeyEvent * e )
{
  QWidget::keyPressEvent(e);
}




void hk_kdemodulepartwidget::set_autoclose(bool c)
{
  p_private->p_autoclose=c;
}


bool hk_kdemodulepartwidget::autoclose(void) const
{
  return p_private->p_autoclose;
}



void hk_kdemodulepartwidget::show()
{
  set_block_has_changed(true);
  QWidget::show();
  set_block_has_changed(false);
}

void hk_kdemodulepartwidget::showMaximized()
{
  set_block_has_changed(true);
  QWidget::showMaximized();
  set_block_has_changed(false);

}

void hk_kdemodulepartwidget::set_has_changed(enum_has_changed /*forcesetting*/)
{
//cerr <<"hk_kdemodulepartwidget::set_has_changed"<<endl;
// hk_module::set_has_changed(forcesetting);
 if (p_saveaction)p_saveaction->setEnabled(!runtime_only()&& has_changed());
}



void hk_kdemodulepartwidget::reset_has_changed(void)
{
   hk_module::reset_has_changed();
  if (p_saveaction)p_saveaction->setEnabled(!runtime_only()&& has_changed());
}





void hk_kdemodulepartwidget::show_widget(void)
{
  show();
}

void hk_kdemodulepartwidget::hide_widget(void)
{
  hide();
}

bool hk_kdemodulepartwidget::close_widget(void)
{
  return close();
}

bool hk_kdemodulepartwidget::has_changed(void)
{
return hk_module::has_changed();
}

void hk_kdemodulepartwidget::set_block_has_changed(bool)
{

}



