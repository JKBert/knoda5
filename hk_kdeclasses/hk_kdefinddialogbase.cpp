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

#include "hk_kdefinddialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a hk_kdefinddialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdefinddialogbase::hk_kdefinddialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdefinddialogbase":name));
    setModal(modal);
    setSizeGripEnabled( true );
    hk_kdefinddialogbaseLayout = new QGridLayout( this);
    hk_kdefinddialogbaseLayout->setMargin(11);
    hk_kdefinddialogbaseLayout->setSpacing(6);
    hk_kdefinddialogbaseLayout->setObjectName(QString::fromAscii("hk_kdefinddialogbaseLayout")); 

    searchlabel = new QLabel(this);
    searchlabel->setObjectName("searchlabel");

    hk_kdefinddialogbaseLayout->addWidget( searchlabel, 0, 0 );

    Layout5 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout5->setMargin(0);
    Layout5->setSpacing(6);
    Layout5->setObjectName("Layout5");

    ok_button = new QPushButton(this);
    ok_button->setObjectName("ok_button");
    ok_button->setAutoDefault( true );
    ok_button->setDefault( true );
    Layout5->addWidget( ok_button );

    cancel_button = new QPushButton(this);
    cancel_button->setObjectName("cancel_button");
    cancel_button->setAutoDefault( true );
    Layout5->addWidget( cancel_button );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout5->addItem( Spacer1 );

    hk_kdefinddialogbaseLayout->addLayout( Layout5, 0, 2, 2, 1 );

    optionsgroup = new QGroupBox( this );
    optionsgroup -> setObjectName( "optionsgroup" );
    optionsgroupLayout = new QGridLayout( optionsgroup );
    optionsgroupLayout->setAlignment( Qt::AlignTop );

    case_sensitivebox = new QCheckBox(optionsgroup);
    case_sensitivebox->setObjectName(QString::fromAscii("case_sensitivebox"));

    optionsgroupLayout->addWidget( case_sensitivebox, 0, 0 );

    all_columnsbox = new QCheckBox(optionsgroup);
    all_columnsbox->setObjectName(QString::fromAscii("all_columnsbox"));

    optionsgroupLayout->addWidget( all_columnsbox, 1, 0 );

    part_of_columnbox = new QCheckBox(optionsgroup);
    part_of_columnbox->setObjectName(QString::fromAscii("part_of_columnbox"));

    optionsgroupLayout->addWidget( part_of_columnbox, 2, 0 );

    find_backwardsbox = new QCheckBox(optionsgroup);
    find_backwardsbox->setObjectName(QString::fromAscii("find_backwardsbox"));

    optionsgroupLayout->addWidget( find_backwardsbox, 0, 1 );

    hk_kdefinddialogbaseLayout->addWidget( optionsgroup, 1, 0, 1, 2 );

    searchfield = new QComboBox(this);
    searchfield->setObjectName(QString::fromAscii("searchfield"));
    searchfield->setMinimumSize( QSize( 200, 0 ) );
    searchfield->setEditable( true );

    hk_kdefinddialogbaseLayout->addWidget( searchfield, 0, 1 );
    languageChange();
    resize( QSize(356, 149).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( ok_button, SIGNAL( clicked() ), this, SLOT( find_slot() ) );
    connect( searchfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( new_findargument() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdefinddialogbase::~hk_kdefinddialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdefinddialogbase::languageChange()
{
    setWindowTitle( tr( "MyDialog" ) );
    searchlabel->setText( tr( "Search" ) );
    ok_button->setText( tr( "&OK" ) );
    cancel_button->setText( tr( "&Cancel" ) );
    optionsgroup->setTitle( tr( "Options" ) );
    case_sensitivebox->setText( tr( "C&ase sensitive" ) );
    all_columnsbox->setText( tr( "All &columns" ) );
    part_of_columnbox->setText( tr( "&Part of column" ) );
    find_backwardsbox->setText( tr( "Find &backwards" ) );
}

void hk_kdefinddialogbase::find_slot()
{
    qWarning( "hk_kdefinddialogbase::find_slot(): Not implemented yet" );
}

void hk_kdefinddialogbase::new_findargument()
{
    qWarning( "hk_kdefinddialogbase::new_findargument(): Not implemented yet" );
}

