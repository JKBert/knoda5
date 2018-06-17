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
#include "hk_kdenewdatabase.h"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qstring.h>
#include <qwhatsthis.h>
#include <kfiledialog.h>
#include <klocale.h>

/*
 *  Constructs a hk_kdenewdatabase which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdenewdatabase::hk_kdenewdatabase( const hk_string& drivername,QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
    : hk_kdenewdatabasedialogbase( parent, name, modal, fl )
{
  p_drivername=drivername;
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdenewdatabase::~hk_kdenewdatabase()
{
    // no need to delete child widgets, Qt does it all for us
}

hk_string hk_kdenewdatabase::show_dialog(const hk_string& drivername,bool local) // static
{
  hk_string result;
  hk_kdenewdatabase* n=new hk_kdenewdatabase(drivername);
  n->directorybutton->setEnabled(local);
  if (n->exec()==QDialog::Accepted)
   {
     if (!n->databasefield->text().isNull())
        result=u2l(n->databasefield->text().toUtf8().data());
   }
  delete n;
  return result;
}

void hk_kdenewdatabase::ok_clicked()
{
 accept();
}

void hk_kdenewdatabase::directory_clicked()
{
  QString filename=KFileDialog::getSaveFileName(":"+QString::fromUtf8(l2u(p_drivername).c_str()),QString::null,0,i18n("New local database file"));
  databasefield->setText(filename);
}

void hk_kdenewdatabase::text_changed()
{
  okbutton->setEnabled(!databasefield->text().isEmpty());
}
