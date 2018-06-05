// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for knoda -Knorr's Datenbank.
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for knoda4 -Knorr's Datenbank.
// copyright (c) 2018 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE5 port of the file for knoda5 -Knorr's Datenbank.
//
// This file is part of the knoda5 -Knorr's Datenbank.
// This file may be distributed and/or modified under the terms of the
// GNU General Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#include "hk_kdedriverselect.h"
#include "hk_kdedatabasesetting.h"
#include "hk_kdepreferencesdialog.h"
#include <qlistwidget.h>
#include <qnamespace.h>
#include <qpushbutton.h>
#include <QKeyEvent>
#include <qtabwidget.h>
#include <kapplication.h>
#include <hk_drivermanager.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kpushbutton.h>
#include <qlayout.h>
#include <kglobal.h>
#include <kconfiggroup.h>
#include <khelpclient.h>
#include "hk_kdedriverselect.moc"

internalfiledialog::internalfiledialog(const QUrl& startDir, const QString& filter,
	QWidget *parent): KFileDialog(startDir,filter,parent)
{
  QStringList l;
  QString xmime="application/x-hk_connection";
  l.append(xmime);
  setMimeFilter(l,xmime);
  okButton()->hide();
  cancelButton()->hide();

}
void internalfiledialog::accept()
{
  KFileDialog::accept();
}

void internalfiledialog::reject()
{

}

void internalfiledialog::keyPressEvent(QKeyEvent* event)
{
    if ( event->key() == Qt::Key_Escape )
    {
    event->accept();
    }
    else KFileDialog::keyPressEvent(event);

}

/*
 *  Constructs a hk_kdedriverselect which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdedriverselect::hk_kdedriverselect( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : hk_kdedriverselectbase( parent, name, modal, fl )
{
  p_driverselected=true;
  vector<hk_string>* drv=hk_drivermanager::driverlist();
  vector<hk_string>::iterator it;
  QListWidgetItem* p_item;
  int i=0;
  for(it=drv->begin();it!=drv->end();it++)
  {
    connectbutton->setEnabled(true);
    p_item = new QListWidgetItem(QString::fromUtf8(l2u((*it)).c_str()),driverlist);
    driverlist->addItem(p_item);
    if ((*it)==hk_class::defaultdriver())
      driverlist->setCurrentItem(p_item);
    i++;
  }
  driverlist->installEventFilter(this);
  driverlist->setFocus();
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg=c->group("Driverselectdialog");
  const QRect& rrect = QRect(0,0,500,300);
  QRect g;
  
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g);

  tab2Layout = new QVBoxLayout(tab_2);
  tab2Layout->setMargin(11);
  tab2Layout->setSpacing(6);
  tab2Layout->setObjectName("tab2Layout");


  p_filedialog=new internalfiledialog(QUrl("kfiledialog:///hkc"),QString::null,this);

  p_filedialog->setParent( tab_2, windowFlags() & ~Qt::WindowType_Mask );
  p_filedialog->setMinimumSize( QSize( 10, 10 ) );

  tab2Layout->addWidget( p_filedialog );

  connect(p_filedialog,SIGNAL(fileSelected(const QUrl&)),this,SLOT(connection_clicked()));
  connect(p_filedialog,SIGNAL(fileHighlighted(const QUrl&)),this,SLOT(file_selected(const QUrl&)));
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdedriverselect::~hk_kdedriverselect()
{
    // no need to delete child widgets, Qt does it all for us
}

QString hk_kdedriverselect::currentDriver()
{
  return driverlist -> currentItem() -> text();
}

void hk_kdedriverselect::list_clicked()
{
   if (driverlist->currentItem() != NULL) accept();
}


void hk_kdedriverselect::connection_clicked()
{
   if (p_filedialog->selectedFile()!=QString::null)
   {
    p_driverselected=false;
    p_selectedfile=p_filedialog->selectedFile();
    accept();
   }

}


bool hk_kdedriverselect::eventFilter(QObject* o,QEvent* e)
{
  if (o==driverlist)
  {
     if (e->type()==QEvent::KeyPress)
     {
       QKeyEvent* k=(QKeyEvent*)(e);
       if (k->key()==Qt::Key_Return)
         {
	   accept();
	   return true;
	 }
     }
  }

  return hk_kdedriverselectbase::eventFilter(o,e);
}



void hk_kdedriverselect::keyPressEvent ( QKeyEvent * e )
{
  if (e->key()==Qt::Key_F1) help_clicked();
}

void hk_kdedriverselect::help_clicked(void)
{
  KHelpClient::invokeHelp("introduction");
}

void hk_kdedriverselect::accept(void)
{
  if (tabwidget->currentWidget()==tab_2/*file dialog*/){
    p_driverselected=false;
    if (!p_filedialog->selectedFile().isEmpty())
      p_selectedfile=p_filedialog->selectedFile();
  }

  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c -> group("Driverselectdialog");

  cg.writeEntry("Geometry",geometry());
  QDialog::accept();
}


void hk_kdedriverselect::tabwidget_changed()
{
    if (tabwidget->currentWidget()==tab_2/*file dialog*/)
        connectbutton->setEnabled(!p_filedialog->selectedFile().isEmpty());
    else
        connectbutton->setEnabled(hk_drivermanager::driverlist()->size() > 0);
}

void hk_kdedriverselect::file_selected(const QUrl& f)
{
    p_selectedfile=f.path();
    connectbutton->setEnabled(!p_selectedfile.isEmpty());
}

void hk_kdedriverselect::option_clicked()
{
    hk_kdepreferencesdialog* p_dialog= new hk_kdepreferencesdialog(0,0,true);
    
    p_dialog->exec();
    delete p_dialog; 
}

