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
//$Revision: 1.20 $

#include "hk_kdeinterpreterdialog.h"
#include <qbuttongroup.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qevent.h>
#include <klocale.h>
#include <hk_actionquery.h>
#include <hk_connection.h>
#include <hk_visible.h>
#include <qapplication.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <locale.h>

#include <kmenubar.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <kglobal.h>
#include <ktoolinvocation.h>
#include <KTextEditor/Editor>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <ktexteditor/highlightinterface.h>
#include <kxmlguifactory.h>
#include <kactioncollection.h>
#include <kmessagebox.h>
#include <KHelpClient>
// TBP icons
class hk_kdeinterpreterdialogprivate
{
public:
    hk_kdeinterpreterdialogprivate():p_closeaction(NULL), p_document(NULL), p_view(NULL)
    , eventloop(NULL) { }
    KAction*  p_closeaction;
    KTextEditor::Document* p_document;
    KTextEditor::View*  p_view;
    QEventLoop* eventloop;
};

/*
 *  Constructs a hk_kdeinterpreterdialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */

hk_kdeinterpreterdialog::hk_kdeinterpreterdialog(QWidget* w,const char* /* n */,Qt::WFlags f):KParts::MainWindow(w, f|Qt::Dialog),
p_autoclose(true),p_has_changed(false), rescode(Accepted), p_private(new hk_kdeinterpreterdialogprivate)
{
  setAttribute(Qt::WA_DeleteOnClose,false);
  setObjectName( "hk_kdeinterpreterdialog" );
  setComponentName("hk_kde5classes", "Script editor");
  setXMLFile("hk_kdeinterpreterdialog.rc");
  setWindowModality(Qt::ApplicationModal);

  KTextEditor::Editor* p_ed = KTextEditor::Editor::instance();
  if (p_ed != NULL) {
      p_private->p_document = p_ed->createDocument(0);
      if (p_private->p_document != NULL) {
          connect( p_private -> p_document, SIGNAL( textChanged(KTextEditor::Document*)), this, SLOT( slot_has_changed() ) );
          p_private -> p_view = p_private->p_document->createView(this);
      }
  } else
      KMessageBox::error(w,i18n("A KDE text-editor component could not be found;\n"
                                  "please check your KDE installation."));
  QIcon::setThemeName("oxygen");
  p_private->p_closeaction=new KAction(QIcon::fromTheme("window-close"),i18n("&Close"),actionCollection());
  actionCollection() -> addAction("closedialog",p_private->p_closeaction);
  connect(p_private->p_closeaction,SIGNAL(triggered()),this,SLOT(accept()));  
  createGUI(NULL);
  if (p_private->p_view != NULL) {
      setCentralWidget(p_private->p_view);
      p_private -> p_view -> setFocus();
      guiFactory()-> addClient(p_private -> p_view);
  }
  resize( 596, 480 );

  KSharedConfigPtr c=KGlobal::config();
  const QRect& rrect=QRect(0,0,500,300);
  KConfigGroup cg=c->group("Interpreter");
  QRect g;
  
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g);
}


/*
 *  oys the object and frees any allocated resources
 */
hk_kdeinterpreterdialog::~hk_kdeinterpreterdialog()
{
// no need to delete child widgets, Qt does it all for us
  hide();
  if (p_private->p_view != NULL)
      guiFactory()->removeClient( p_private -> p_view );
  setCentralWidget(NULL);
  delete p_private->p_closeaction;
}

void hk_kdeinterpreterdialog::closeEvent ( QCloseEvent* e)
{
  e->accept();
  accept();
}

void hk_kdeinterpreterdialog::print(void)
{

// this is a workaround: otherwise float numbers in the postscript file
// would be created with locale dots (e.g. a comma in Germany) from Qt
  hk_string origlocale=setlocale(LC_NUMERIC,NULL);
  hk_string origmonetarylocale=setlocale(LC_MONETARY,NULL);
  setlocale(LC_NUMERIC,"C");
  setlocale(LC_MONETARY,"C");
  p_private->p_view->action("file_print") ->trigger();
//workaround part 2
  setlocale(LC_NUMERIC,origlocale.c_str());
  setlocale(LC_MONETARY,origmonetarylocale.c_str());
//workaround part 2 end
}

void    hk_kdeinterpreterdialog::keyPressEvent ( QKeyEvent * e )
{
  KParts::MainWindow::keyPressEvent(e);
}

void hk_kdeinterpreterdialog::done( int r )
{
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Interpreter");
  
  cg.writeEntry("Geometry",geometry());
  hide();
  setResult( r );
  if ( testAttribute(Qt::WA_DeleteOnClose) ) {
	setAttribute(Qt::WA_DeleteOnClose,false);
	deleteLater();
  }

}


int hk_kdeinterpreterdialog::exec(int rownumber,const hk_string& w)
{
     if ( p_private->eventloop != NULL ) {
	qWarning( "hk_kdeinterpreterdialog::exec: Recursive call detected." );
	return -1;
    }
    bool destructiveClose = testAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_DeleteOnClose,false );

    Qt::WindowModality wModality = windowModality();
    setWindowModality( Qt::ApplicationModal );
    setResult( 0 );

    if (hk_visible::open_maximized_windows())
    	showMaximized();
    else show();
    
    if (p_private->p_view != NULL) {
        p_private->p_document->setHighlightingMode("Python");
        p_private->p_view->setCursorPosition(KTextEditor::Cursor(rownumber,0));
    }

    if (w.size()>0) hk_class::show_warningmessage(w);
    QEventLoop eventloop;
    p_private->eventloop = &eventloop;
    eventloop.exec();
    p_private->eventloop = NULL;
    if ( (wModality & Qt::ApplicationModal) == 0 )
	setWindowModality( wModality );

    int res = result();
    if ( destructiveClose )
      delete this;
  return res; 
}


void hk_kdeinterpreterdialog::accept()
{

    done( Accepted );
}

/*!
  Hides the modal dialog and sets the result code to \c Rejected.
  \sa accept() done()
*/

void hk_kdeinterpreterdialog::reject()
{
    done( Rejected );
}



hk_string hk_kdeinterpreterdialog::code(void)
{
    const QString& txt = (p_private->p_document == NULL ? QString():p_private->p_document->text());
    
    return u2l(txt.toUtf8().data());
}

void hk_kdeinterpreterdialog::set_code(const hk_string& c,bool registerchange)
{
    if (p_private -> p_document == NULL)
        return;
    if(!registerchange) p_private->p_document->blockSignals(true);
    p_private -> p_document->setText(QString::fromUtf8(l2u(c).c_str()));
  // To be fixed: when text loaded, undo action needs to be disabled
    if(!registerchange) p_private -> p_document->blockSignals(false);
}

void hk_kdeinterpreterdialog::hide()
{
    if ( isHidden() )
	return;

    QWidget::hide();
    if (p_private->eventloop != NULL)
        p_private->eventloop->quit();
}

void hk_kdeinterpreterdialog::slot_has_changed(void)
{
 p_has_changed=true;
}

void hk_kdeinterpreterdialog::set_caption(hk_visible* v,const hk_string& action)
{
    QString txt;
    if (v) {
        txt=QString::fromUtf8(l2u(v->identifier()).c_str());
        QString n;n.setNum(v->presentationnumber());
        txt+=" ("+n+")";
        txt+=" - ";
    }

    txt+=QString::fromUtf8(l2u(action).c_str());
    setWindowTitle(txt);
}

void hk_kdeinterpreterdialog::slot_undo_changed(void)
{
}

void hk_kdeinterpreterdialog::help_clicked()
{
    //TBP TBT KToolInvocation::invokeHelp("pschapter1");
    KHelpClient::invokeHelp("pschapter1");
}


