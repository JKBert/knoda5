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

#include "hk_kdedriverdialog.h"
#include "hk_kdedriverdialog.moc"
#include <list>
#include <klocale.h>
#include <hk_kdemessages.h>
#include <hk_classes.h>
#include <hk_drivermanager.h>

#ifndef DIRVIEW_H
#define DIRVIEW_H

#include <qtreewidget.h>
#include <qlistwidget.h>
#include <qfilesystemmodel.h>
#include <qstring.h>
#include <qfile.h>
#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <kglobal.h>
#include <qheaderview.h>
#include <qtimer.h>

#endif

class DirectoryModel: public QFileSystemModel
{
public:
  DirectoryModel(QObject* parent=0);
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  
};

DirectoryModel::DirectoryModel(QObject* parent)
:QFileSystemModel(parent)
{
  setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
}

QVariant DirectoryModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
  if (section == 0 && role == Qt::DisplayRole)
    return i18n("Directory");
  else
    return QFileSystemModel::headerData(section,orientation,role);
}  

hk_kdedriverdialog::hk_kdedriverdialog(QWidget *parent, const char *name) : QDialog(parent)
{
    if (name)
        setObjectName(QString::fromAscii(name));
    setModal(true);
    driverlist =new QListWidget(this);
    ok_button  =new QPushButton(this);
    exit_button=new QPushButton(this);
    label= new QLabel(this);
    directorylist = new QTreeView(this);
    p_originaldriverpath=hk_drivermanager::path();
    label->setText(i18n("Found database drivers:"));
    setWindowTitle(i18n("SQL Driver Selection"));
    int top = 30;
    int left= 20;
    int buttonwidth=100;
    int buttonheight=30;
    int listwidth=150;
    int vspace=20;
    int offset=0;
    int hspace=20;
    int add=buttonheight+vspace;
    int directorylistwidth=200;
    int buttonleft=left+directorylistwidth+listwidth+2*hspace;
    int listheight=200;
    label->setGeometry(left+directorylistwidth+hspace,0,listwidth+hspace+buttonwidth,buttonheight);
    ok_button->setGeometry(buttonleft,top+offset,buttonwidth,buttonheight);
    offset+=add;
    exit_button->setGeometry(buttonleft,top+offset,buttonwidth,buttonheight);
    offset+=add;
    DirectoryModel* p_fs = new DirectoryModel(directorylist);
    p_fs -> setRootPath("/");
    directorylist -> setModel(p_fs);
    directorylist->header()-> setResizeMode(QHeaderView::Fixed);
    directorylist -> setColumnHidden(1,true);
    directorylist -> setColumnHidden(2,true);
    directorylist -> setColumnHidden(3,true);    
    directorylist -> setCurrentIndex(p_fs-> index(QString::fromUtf8(l2u(hk_drivermanager::path()).c_str())));
    directorylist->setFocus();
    
    driverlist->setGeometry(left+directorylistwidth+hspace,top,listwidth,listheight);
    directorylist->setGeometry(left,top,directorylistwidth,listheight);

    ok_button->setText(i18n("Connect"));
    exit_button->setText(i18n("Exit"));
    connect(ok_button,SIGNAL(clicked()),this,SLOT(ok_clicked()));
    connect(exit_button,SIGNAL(clicked()),this,SLOT(exit_clicked()));
    connect(directorylist->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(directory_changed()));
    connect(driverlist,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(ok_clicked()));
    directory_changed();
    driverlist->setFocus();
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg=c->group("Driverdialog");
  const QRect& rrect = QRect(0,0,buttonleft+hspace+buttonwidth,top+listheight+vspace);
  QRect g;
  
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g); 
}


hk_kdedriverdialog::~hk_kdedriverdialog()
{
    delete ok_button;
    delete exit_button;
    delete driverlist;
    delete label;
}

void hk_kdedriverdialog::showEvent(QShowEvent* ev)
{
  QDialog::showEvent(ev);
  QTimer::singleShot(500, this, SLOT(scroll_to_directory()));
}

void hk_kdedriverdialog::ok_clicked(void)
{
  if (driverlist->currentItem()<0)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Please select a driver"));
        return;
    }
    
    QString d=driverlist->currentItem()->text();
    p_drivername=(d.isNull()?"":u2l(d.toUtf8().data()));
    
    KSharedConfigPtr c=KGlobal::config();
    KConfigGroup cg=c->group("Driverdialog");
  
    cg.writeEntry("Geometry",geometry());
  accept();
}


void hk_kdedriverdialog::exit_clicked(void)
{
    hk_drivermanager::set_path(p_originaldriverpath);
    reject();
}


void  hk_kdedriverdialog::directory_changed()
{
    DirectoryModel* p_fs = static_cast<DirectoryModel*>(directorylist->model());
    
    driverlist->clear();
    ok_button->setEnabled(false);
    hk_drivermanager::set_path(u2l(p_fs->filePath(directorylist->currentIndex()).toUtf8().data()));
    vector<hk_string>* drv=hk_drivermanager::driverlist();
    vector<hk_string>::iterator it;
    for(it=drv->begin();it!=drv->end();it++)
    {
        ok_button->setEnabled(true);
        driverlist->addItem(QString::fromUtf8 (l2u((*it)).c_str()));
    }

    driverlist->repaint(); 
}

void hk_kdedriverdialog::scroll_to_directory(void )
{
   directorylist -> scrollTo(directorylist->currentIndex(), QAbstractItemView::PositionAtCenter);    
}
