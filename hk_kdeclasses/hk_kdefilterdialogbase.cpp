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

#include "hk_kdefilterdialogbase.h"

#include <qvariant.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a hk_kdefilterdialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdefilterdialogbase::hk_kdefilterdialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdefilterdialogbase":name));
    setModal(modal);
    setSizeGripEnabled( true );
    hk_kdefilterdialogbaseLayout = new QHBoxLayout(this);
    hk_kdefilterdialogbaseLayout->setMargin(11);
    hk_kdefilterdialogbaseLayout->setSpacing(6);
    hk_kdefilterdialogbaseLayout->setObjectName("hk_kdefilterdialogbaseLayout");

    textfield = new QTextEdit( this );
    textfield->setObjectName( "textfield" );
    hk_kdefilterdialogbaseLayout->addWidget( textfield );

    Layout5 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout5->setMargin(0);
    Layout5->setSpacing(6);
    Layout5->setObjectName("Layout5");

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName("buttonOk");
    buttonOk->setAutoDefault( true );
    buttonOk->setDefault( true );
    buttonOk->setEnabled( false );
    Layout5->addWidget( buttonOk );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName("buttonCancel");
    buttonCancel->setAutoDefault( true );
    Layout5->addWidget( buttonCancel );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout5->addItem( Spacer1 );
    hk_kdefilterdialogbaseLayout->addLayout( Layout5 );
    languageChange();
    resize( QSize(391, 247).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( textfield, SIGNAL( textChanged() ), this, SLOT( text_changed() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdefilterdialogbase::~hk_kdefilterdialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdefilterdialogbase::languageChange()
{
    setWindowTitle( tr( "MyDialog" ) );
    textfield->setText( QString::null );
    textfield->setToolTip( QString::null );
    buttonOk->setText( tr( "&OK" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
}

void hk_kdefilterdialogbase::text_changed()
{
    qWarning( "hk_kdefilterdialogbase::text_changed(): Not implemented yet" );
}

