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
 //$Revision: 1.10 $
#include "hk_kdedatabasesetting.h"
#include <klocale.h>
#include <kapplication.h>
#include <kaction.h>
#include <kiconloader.h>
#include <kstandarddirs.h>
#include <kcombobox.h>
#include <kpushbutton.h>
#include <kconfiggroup.h>

#include <qframe.h>
#include <qpixmap.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qstackedwidget.h>
#include <qtreewidget.h>
#include <kapplication.h>
#include <kconfig.h>

#include <hk_class.h>
#include <hk_database.h>
#include <hk_connection.h>
#include <kglobal.h>
//TBP TBT translation tr()->i18n()
//TBP icons
hk_kdedatabasesetting::hk_kdedatabasesetting( hk_database*db, QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : hk_kdedatabasesettingbase( parent, name, modal, fl )
{
 p_database=db;
 QStringList list;
 list.append(i18n("Local"));
 list.append(i18n("Central"));
 
 queryload->insertItems(queryload->count(), list);
 formload->insertItems(formload->count(), list);
 reportload->insertItems(reportload->count(), list);
 moduleload->insertItems(moduleload->count(), list);
 

 list.prepend("");
 allload->insertItems(allload->count(), list);
   
  KIconLoader* loader=KIconLoader::global();
  loader->addAppDir("hk_kde4classes");
  
  p_systemtableitem= new QTreeWidgetItem(listview,QStringList() << i18n("Load/Save"));
  p_systemtableitem->setIcon(0,loader->loadIcon("document-save",KIconLoader::Dialog));
  
  if (p_database && p_database->connection()->server_needs(hk_connection::NEEDS_MANUAL_CHARSET))
  {
    p_localeitem=new QTreeWidgetItem(listview,QStringList() << i18n("Regional"));
    p_localeitem->setIcon(0,loader->loadIcon("preferences-desktop-locale",KIconLoader::Dialog));
    for (unsigned int i=0;i<sizeof(charsets)/sizeof(hk_string);++i)
      databaseencodingfield->addItem(QString::fromUtf8(l2u(charsets[i]).c_str()));
    const QString defaultText = QString::fromUtf8(l2u(p_database->databasecharset()).c_str());
    int ind = databaseencodingfield->findText(defaultText);
    if (ind != -1)
        databaseencodingfield->setCurrentIndex(ind);
    else
        // is editable
        databaseencodingfield->setEditText(defaultText);
  }
  else 
    p_localeitem=NULL;
  listview->setCurrentItem(p_systemtableitem);
  listview -> setIconSize(QSize(32,32));  
  p_systemtableitem->setSelected(true);
  if (!p_database) return;
  

  queryload->setCurrentIndex(p_database->loadmode(ft_query)==hk_database::central?1:0);
  queryload->setCurrentIndex(p_database->storagemode(ft_query)==hk_database::central?1:0);
  
  formload->setCurrentIndex(p_database->loadmode(ft_form)==hk_database::central?1:0);
  formload->setCurrentIndex(p_database->storagemode(ft_form)==hk_database::central?1:0);
  
  reportload->setCurrentIndex(p_database->loadmode(ft_report)==hk_database::central?1:0);
  reportload->setCurrentIndex(p_database->storagemode(ft_report)==hk_database::central?1:0);
  
  moduleload->setCurrentIndex(p_database->loadmode(ft_module)==hk_database::central?1:0);
  moduleload->setCurrentIndex(p_database->storagemode(ft_module)==hk_database::central?1:0);
  
  automaticupdatefield->setCurrentIndex(p_database->default_automatic_data_update()?0:1);
  selection_changed();
  ok_button->setEnabled(false);
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Databasesetting");
  const QRect& rrect=QRect(0,0,500,300);
  QRect g;
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g);
}

hk_kdedatabasesetting::~hk_kdedatabasesetting()
{
}


void hk_kdedatabasesetting::create_clicked()
{
  if (!p_database->create_centralstoragetable())
    {
      hk_class::show_warningmessage(hk_class::hk_translate("Error: could not create central storage table\n Check your permissions"));
      return;
    }
  selection_changed();
}

void hk_kdedatabasesetting::selection_changed()
{
bool hasstorage=p_database->has_centralstoragetable();
createbutton->setEnabled(!hasstorage && p_database&&p_database->connection()->server_supports(hk_connection::SUPPORTS_NEW_TABLE)
);
storageframe->setEnabled(hasstorage);
ok_button->setEnabled(true);
}

void hk_kdedatabasesetting::alload_changed()
{
if (allload->currentIndex()==0) return;
int p=allload->currentIndex()-1;
queryload->setCurrentIndex(p);
formload->setCurrentIndex(p);
reportload->setCurrentIndex(p);
moduleload->setCurrentIndex(p);
ok_button->setEnabled(true);
}

void hk_kdedatabasesetting::allstore_changed()
{
if (allload->currentIndex()==0) return;
int p=allload->currentIndex()-1;
queryload->setCurrentIndex(p);
formload->setCurrentIndex(p);
reportload->setCurrentIndex(p);
moduleload->setCurrentIndex(p);
ok_button->setEnabled(true);

}



void hk_kdedatabasesetting::listview_changed()
{
    widgetstack->setCurrentIndex(listview->currentItem()==p_localeitem?1:0);
    headertext->setText( listview->currentItem()==p_localeitem?tr("Regional"):tr( "Open and store files:" ) );

}

void hk_kdedatabasesetting::ok_clicked()
{
 p_database->set_storagemode(ft_query, ((queryload->currentIndex()==1)?hk_database::central:hk_database::local),
                                  ((queryload->currentIndex()==1)?hk_database::central:hk_database::local));
 p_database->set_storagemode(ft_form, ((formload->currentIndex()==1)?hk_database::central:hk_database::local),
                                  ((formload->currentIndex()==1)?hk_database::central:hk_database::local));
 p_database->set_storagemode(ft_report, ((reportload->currentIndex()==1)?hk_database::central:hk_database::local),
                                  ((reportload->currentIndex()==1)?hk_database::central:hk_database::local));
 p_database->set_storagemode(ft_module, ((moduleload->currentIndex()==1)?hk_database::central:hk_database::local),
                                  ((moduleload->currentIndex()==1)?hk_database::central:hk_database::local));
 p_database->set_automatic_data_update(automaticupdatefield->currentIndex()==0); 
    if (p_database && p_database->connection()->server_needs(hk_connection::NEEDS_MANUAL_CHARSET))
  {
  p_database->set_databasecharset(l2u(databaseencodingfield->currentText().toUtf8().data()));
  }
  
 p_database->save_configuration(); 
 KSharedConfigPtr c=KGlobal::config();
 KConfigGroup cg = c->group("Databasesetting");
 cg.writeEntry("Geometry",geometry());
 accept();
}

void hk_kdedatabasesetting::encoding_changed()
{
 ok_button->setEnabled(true);
}
