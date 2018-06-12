// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
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

/****************************************************************************
** Form implementation generated from reading ui file 'hk_kdeaddtabledialogbase.ui'
**
** Created: Sa Jul 30 18:12:32 2005
**      by: The User Interface Compiler ($Id: hk_kdeaddtabledialogbase.cpp,v 1.2 2005/07/31 12:39:05 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdeaddtabledialogbase.h"

#include <qvariant.h>
#include <qlistwidget.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <KLocalizedString>


/*
 *  Constructs a hk_kdeaddtabledialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdeaddtabledialogbase::hk_kdeaddtabledialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdeaddtabledialogbase":name));
    setModal(modal);
    setSizeGripEnabled( true );
    hk_kdeaddtabledialogbaseLayout = new QGridLayout(this);
    hk_kdeaddtabledialogbaseLayout->setMargin(11);
    hk_kdeaddtabledialogbaseLayout->setSpacing(6);
    hk_kdeaddtabledialogbaseLayout->setObjectName(QString::fromAscii("hk_kdeaddtabledialogbaseLayout")); 

    tablelist = new QListWidget( this );
    tablelist -> setObjectName("tablelist" );

    hk_kdeaddtabledialogbaseLayout->addWidget(tablelist, 1, 0, 2, 1);
    
    datasourcetypefield = new QComboBox( this );
    datasourcetypefield->setObjectName(QString::fromAscii("datasourcetypefield"));

    hk_kdeaddtabledialogbaseLayout->addWidget( datasourcetypefield, 0, 0 );

    addbutton = new QPushButton( this);
    addbutton->setObjectName(QString::fromAscii("addbutton"));
    addbutton->setDefault( true );

    hk_kdeaddtabledialogbaseLayout->addWidget( addbutton, 0, 1 );

    buttonClose = new QPushButton(this);
    buttonClose->setObjectName(QString::fromAscii("buttonClose"));

    hk_kdeaddtabledialogbaseLayout->addWidget( buttonClose, 1, 1 );
    spacer1 = new QSpacerItem( 20, 300, QSizePolicy::Minimum, QSizePolicy::Expanding );
    hk_kdeaddtabledialogbaseLayout->addItem( spacer1, 2, 1 );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( addbutton, SIGNAL( clicked() ), this, SLOT( add_clicked() ) );
    connect( buttonClose, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( tablelist, SIGNAL( itemDoubleClicked(QListWidgetItem*)), this, SLOT( add_clicked() ) );
    connect( datasourcetypefield, SIGNAL( activated(int) ), this, SLOT( set_datasources() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeaddtabledialogbase::~hk_kdeaddtabledialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdeaddtabledialogbase::languageChange()
{
    setWindowTitle( i18n( "Add-table dialog" ) );
    addbutton->setText( i18n( "&Add" ) );
    addbutton->setShortcut( QKeySequence( i18n( "Alt+A" ) ) );
    buttonClose->setText( i18n( "&Close" ) );
    buttonClose->setShortcut( QKeySequence( i18n( "Alt+C" ) ) );
}

void hk_kdeaddtabledialogbase::add_clicked()
{
    qWarning( "hk_kdeaddtabledialogbase::add_clicked(): Not implemented yet" );
}

void hk_kdeaddtabledialogbase::set_datasources()
{
    qWarning( "hk_kdeaddtabledialogbase::set_datasources(): Not implemented yet" );
}

void hk_kdeaddtabledialogbase::check_buttons()
{
    qWarning( "hk_kdeaddtabledialogbase::check_buttons(): Not implemented yet" );
}

