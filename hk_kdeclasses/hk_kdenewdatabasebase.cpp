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

#include "hk_kdenewdatabasebase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <KLocalizedString>

/*
 *  Constructs a hk_kdenewdatabasedialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdenewdatabasedialogbase::hk_kdenewdatabasedialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdenewdatabasedialogbase":name));
    setModal(modal);
    setMinimumSize( QSize( 300, 0 ) );
    setSizeGripEnabled( true );
    hk_kdenewdatabasedialogbaseLayout = new QVBoxLayout(this);
    hk_kdenewdatabasedialogbaseLayout->setMargin(11);
    hk_kdenewdatabasedialogbaseLayout->setSpacing(6);
    hk_kdenewdatabasedialogbaseLayout->setObjectName("hk_kdenewdatabasedialogbaseLayout");

    databaselabel = new QLabel(this);
    databaselabel->setObjectName("databaselabel");
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHeightForWidth(databaselabel->sizePolicy().hasHeightForWidth());
    databaselabel->setSizePolicy(policy);
    hk_kdenewdatabasedialogbaseLayout->addWidget( databaselabel );

    layout2 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    layout2->setMargin(0);
    layout2->setSpacing(6);
    layout2->setObjectName("layout2");

    databasefield = new QLineEdit(this);
    databasefield->setObjectName("databasefield");
    layout2->addWidget( databasefield );

    directorybutton = new QPushButton(this);
    directorybutton->setObjectName("directorybutton");
    directorybutton->setEnabled( false );
    policy.setHorizontalPolicy(QSizePolicy::Maximum);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(directorybutton->sizePolicy().hasHeightForWidth());
    directorybutton->setSizePolicy(policy);
    directorybutton->setMaximumSize( QSize( 25, 32767 ) );
    layout2->addWidget( directorybutton );
    hk_kdenewdatabasedialogbaseLayout->addLayout( layout2 );

    Layout1 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    Layout1->setMargin(0);
    Layout1->setSpacing(6);
    Layout1->setObjectName("Layout1");
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    okbutton = new QPushButton(this);
    okbutton->setObjectName("okbutton");
    okbutton->setEnabled( false );
    okbutton->setAutoDefault( true );
    okbutton->setDefault( true );
    Layout1->addWidget( okbutton );

    cancelbutton = new QPushButton(this);
    cancelbutton->setObjectName("cancelbutton");
    cancelbutton->setAutoDefault( true );
    Layout1->addWidget( cancelbutton );
    hk_kdenewdatabasedialogbaseLayout->addLayout( Layout1 );
    languageChange();
    resize( QSize(300, 134).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( cancelbutton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( okbutton, SIGNAL( clicked() ), this, SLOT( ok_clicked() ) );
    connect( directorybutton, SIGNAL( clicked() ), this, SLOT( directory_clicked() ) );
    connect( databasefield, SIGNAL( textChanged(const QString&) ), this, SLOT( text_changed() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdenewdatabasedialogbase::~hk_kdenewdatabasedialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdenewdatabasedialogbase::languageChange()
{
    setWindowTitle( i18n( "New database dialog" ) );
    databaselabel->setText( i18n( "New database name:" ) );
    directorybutton->setText( i18n( "..." ) );
    okbutton->setText( i18n( "&OK" ) );
    okbutton->setShortcut( QKeySequence( QString::null ) );
    cancelbutton->setText( i18n( "&Cancel" ) );
    cancelbutton->setShortcut( QKeySequence( QString::null ) );
}

void hk_kdenewdatabasedialogbase::ok_clicked()
{
    qWarning( "hk_kdenewdatabasedialogbase::ok_clicked(): Not implemented yet" );
}

void hk_kdenewdatabasedialogbase::directory_clicked()
{
    qWarning( "hk_kdenewdatabasedialogbase::directory_clicked(): Not implemented yet" );
}

void hk_kdenewdatabasedialogbase::text_changed()
{
    qWarning( "hk_kdenewdatabasedialogbase::text_changed(): Not implemented yet" );
}

