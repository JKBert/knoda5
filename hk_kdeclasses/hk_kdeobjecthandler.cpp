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
 //$Revision: 1.7 $
#include "hk_kdeobjecthandler.h"
#include "hk_kdedblistview.h"
#include <hk_database.h>

#include <qpushbutton.h>
#include <qpixmap.h>
#include <qiconset.h>
#include <qwhatsthis.h>
#include <qlabel.h>
#include <kiconloader.h>
#include <kicon.h>
#include <kstandarddirs.h>
#include <kapplication.h>
#include <kconfig.h>

#include <klocale.h>
#include <kglobal.h>
#include <kconfiggroup.h>


hk_kdeobjecthandler::hk_kdeobjecthandler( hk_database* db,QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : hk_kdeobjecthandlerbase( parent, name, modal, fl )
{
  p_database=db;
  centrallistview->set_database(db);
  centrallistview->set_showmode(hk_kdedblistview::sm_central);
  locallistview->set_database(db);
  locallistview->set_showmode(hk_kdedblistview::sm_local);
  KIconLoader* loader=KIconLoader::global();
  loader->addAppDir("hk_kde4classes");
  slot_selection_changed();
  uploadbutton->setIcon( loader->loadIcon("go-next",KIconLoader::Small));
  downloadbutton->setIcon( loader->loadIcon("go-previous",KIconLoader::Small));
  centrallistview->headerItem() -> setText(0,i18n("Central"));
  locallistview->headerItem() -> setText(0,i18n("Local"));
  setWindowTitle(i18n("Database object handler"));
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Objecthandler");
  QRect const & rrec = QRect(0,0,500,300);
  QRect g;
  g=cg.readEntry("Geometry",rrec);
  setGeometry(g);
}


hk_kdeobjecthandler::~hk_kdeobjecthandler()
{
}



void hk_kdeobjecthandler::download_clicked()
{
  filetype f=ft_report;
  if (centrallistview->is_queryitem()) f=ft_query;
  else
  if (centrallistview->is_formitem()) f=ft_form;
  else
  if (centrallistview->is_moduleitem()) f=ft_module;
  
  hk_string data=p_database->load_central(u2l(centrallistview->currentItem()->text(0).toUtf8().data()),f);
  p_database->save_local(data,u2l(centrallistview->currentItem()->text(0).toUtf8().data()),f,true,true); 
}



void hk_kdeobjecthandler::upload_clicked()
{
  filetype f=ft_report;
  if (locallistview->is_queryitem()) f=ft_query;
  else
  if (locallistview->is_formitem()) f=ft_form;
  else
  if (locallistview->is_moduleitem()) f=ft_module;
  
  hk_string data=p_database->load_local(u2l(locallistview->currentItem()->text(0).toUtf8().data()),f);
  p_database->save_central(data,u2l(locallistview->currentItem()->text(0).toUtf8().data()),f,true,true); 

}


void hk_kdeobjecthandler::slot_selection_changed()
{
 downloadbutton->setEnabled(centrallistview ->currentItem()  &&  !centrallistview->is_headeritem());
 uploadbutton->setEnabled(locallistview->currentItem() && !locallistview->is_headeritem());
}


void hk_kdeobjecthandler::accept(void)
{
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Objecthandler");
  
  cg.writeEntry("Geometry",geometry());
  QDialog::accept();
}
