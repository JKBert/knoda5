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
/****************************************************************************
** Form implementation generated from reading ui file 'newpassworddialog.ui'
**
** Created: Sa Jul 30 19:27:47 2005
**      by: The User Interface Compiler ($Id: hk_kdenewpassworddialogbase.cpp,v 1.3 2005/07/31 12:39:05 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdenewpassworddialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
//TBP TBT translation tr()->i18n()
/*
 *  Constructs a hk_kdenewpassworddialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdenewpassworddialogbase::hk_kdenewpassworddialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdenewpassworddialogbase":name));
    setModal(modal);
    setSizeGripEnabled( TRUE );
    hk_kdenewpassworddialogbaseLayout = new QGridLayout(this);
    hk_kdenewpassworddialogbaseLayout->setMargin(11);
    hk_kdenewpassworddialogbaseLayout->setSpacing(6);
    hk_kdenewpassworddialogbaseLayout->setObjectName(QString::fromAscii("hk_kdenewpassworddialogbaseLayout")); 

    Layout3 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout3->setMargin(0);
    Layout3->setSpacing(6);
    Layout3->setObjectName("Layout3");

    Layout2 = new QGridLayout( static_cast<QWidget*>(NULL));
    Layout2->setMargin(0);
    Layout2->setSpacing(6);
    Layout2->setObjectName(QString::fromAscii("Layout2")); 

    repeatlabel = new QLabel(this);
    repeatlabel->setObjectName("repeatlabel");

    Layout2->addWidget( repeatlabel, 1, 0 );

    passwordfield = new QLineEdit(this);
    passwordfield->setObjectName("passwordfield");
    passwordfield->setEchoMode( QLineEdit::Password );

    Layout2->addWidget( passwordfield, 0, 1 );

    repeatfield = new QLineEdit(this);
    repeatfield->setObjectName("repeatfield");
    repeatfield->setEchoMode( QLineEdit::Password );

    Layout2->addWidget( repeatfield, 1, 1 );

    passwordlabel = new QLabel(this);
    passwordlabel->setObjectName("passwordlabel");

    Layout2->addWidget( passwordlabel, 0, 0 );
    Layout3->addLayout( Layout2 );
    Spacer1_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout3->addItem( Spacer1_2 );

    hk_kdenewpassworddialogbaseLayout->addLayout( Layout3, 1, 0 );

    Layout5 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout5->setMargin(0);
    Layout5->setSpacing(6);
    Layout5->setObjectName("Layout5");

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName("buttonOk");
    buttonOk->setAutoDefault( TRUE );
    buttonOk->setDefault( TRUE );
    buttonOk->setEnabled( FALSE );
    Layout5->addWidget( buttonOk );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName("buttonCancel");
    buttonCancel->setAutoDefault( TRUE );
    Layout5->addWidget( buttonCancel );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout5->addItem( Spacer1 );

    hk_kdenewpassworddialogbaseLayout->addLayout( Layout5, 1, 1 );

    headlinelabel = new QLabel(this);
    headlinelabel->setObjectName("headlinelabel");

    hk_kdenewpassworddialogbaseLayout->addWidget( headlinelabel, 0, 0 );
    languageChange();
    resize( QSize(316, 133).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( passwordfield, SIGNAL( textChanged(const QString&) ), this, SLOT( data_changed() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( ok_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdenewpassworddialogbase::~hk_kdenewpassworddialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdenewpassworddialogbase::languageChange()
{
    setWindowTitle( tr( "Password dialog" ) );
    repeatlabel->setText( tr( "Repeat: " ) );
    passwordlabel->setText( tr( "New Password: " ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    headlinelabel->setText( tr( "Enter new password" ) );
}

void hk_kdenewpassworddialogbase::data_changed()
{
    qWarning( "hk_kdenewpassworddialogbase::data_changed(): Not implemented yet" );
}

void hk_kdenewpassworddialogbase::ok_clicked()
{
    qWarning( "hk_kdenewpassworddialogbase::ok_clicked(): Not implemented yet" );
}

