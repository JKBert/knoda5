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
//$Revision: 1.8 $
#include "hk_kdenewpassworddialog.h"
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <hk_class.h>
#include <klocale.h>

/*
 *  Constructs a hk_kdenewpassworddialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdenewpassworddialog::hk_kdenewpassworddialog( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdenewpassworddialogbase( parent, name, modal, fl )
{
    headlinelabel->setText(i18n("Enter new password"));
    passwordlabel->setText(i18n("New password:"));
    repeatlabel->setText(i18n("Repeat:"));
    setWindowTitle(i18n("Password dialog"));
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdenewpassworddialog::~hk_kdenewpassworddialog()
{
// no need to delete child widgets, Qt does it all for us
}


/*
 * public slot
 */
void hk_kdenewpassworddialog::data_changed()
{
    buttonOk->setEnabled(passwordfield->text().length()>0);
}


void hk_kdenewpassworddialog::ok_clicked()
{
    if (passwordfield->text()==repeatfield->text()) accept();
    else
    {
        hk_class::show_warningmessage(hk_class::hk_translate("The two entries are different!"));
    }
}
