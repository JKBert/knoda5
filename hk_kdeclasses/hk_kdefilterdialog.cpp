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
//Version $Revision: 1.9 $

#include "hk_kdefilterdialog.h"
#include <qtextedit.h>
#include <qpushbutton.h>
#include <hk_class.h>
#include <klocale.h>
#include <kconfig.h>
#include <kapplication.h>
#include <kglobal.h>
#include <kconfiggroup.h>

hk_kdefilterdialog::hk_kdefilterdialog(enum_type type, QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdefilterdialogbase( parent, name, modal, fl )
{
    p_type=type;
    buttonOk->setEnabled(true);
    setWindowTitle(i18n("Filter Dialog"));
    buttonOk->setText(i18n("&Ok"));
    buttonCancel->setText(i18n("&Cancel"));
    textfield->setFocus();
    KSharedConfigPtr c=KGlobal::config();
    KConfigGroup cg = c->group(type==filter?"Filter":"Editor");
    const QRect& rrect = QRect(0,0,200,150);
    QRect g;
    
    g = cg.readEntry("Geometry",rrect);
    setGeometry(g);
   if (type==editor) setWindowTitle(i18n("Editor"));
    
}


hk_kdefilterdialog::~hk_kdefilterdialog()
{
// no need to delete child widgets, Qt does it all for us
}


void hk_kdefilterdialog::text_changed()
{

}


void hk_kdefilterdialog::closeEvent(QCloseEvent*e)
{


 hk_kdefilterdialogbase::closeEvent(e);
}

void hk_kdefilterdialog::accept()
{
  store_size();
  hk_kdefilterdialogbase::accept();
}


void hk_kdefilterdialog::reject()
{
  store_size();
  hk_kdefilterdialogbase::reject();
}

void hk_kdefilterdialog::store_size()
{
  KSharedConfigPtr c = KGlobal::config();
  KConfigGroup cg = c-> group(p_type==filter?"Filter":"Editor");
  
  cg.writeEntry("Geometry",geometry());
}



