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

#include "hk_kdefinddialog.h"
#include <hk_class.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <klocale.h>

hk_kdefinddialog::hk_kdefinddialog( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdefinddialogbase( parent, name, modal, fl )
{
    part_of_columnbox->setChecked(true);
    part_of_columnbox->setText(i18n("&Part of column"));
    searchlabel->setText(i18n("Search"));
    optionsgroup->setTitle(i18n("Options"));
    case_sensitivebox->setText(i18n("C&ase sensitive"));
    all_columnsbox->setText(i18n("All c&olumns"));
    find_backwardsbox->setText(i18n("Find &backwards"));
    all_columnsbox->hide();
    setWindowTitle(i18n("Search Dialog"));
    ok_button->setText(i18n("&Ok"));
    cancel_button->setText(i18n("&Cancel"));
    searchfield->setFocus();

}


hk_kdefinddialog::~hk_kdefinddialog()
{
// no need to delete child widgets, Qt does it all for us
}


void hk_kdefinddialog::find_slot()
{

    emit signal_findbutton_clicked();
}


void hk_kdefinddialog::new_findargument()
{
    emit signal_findargument_changed();
}
