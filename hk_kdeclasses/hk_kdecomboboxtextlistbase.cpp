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
** Form implementation generated from reading ui file 'combotextlist.ui'
**
** Created: Son Feb 6 15:05:31 2005
**      by: The User Interface Compiler ($Id: hk_kdecomboboxtextlistbase.cpp,v 1.1 2005/02/11 22:22:22 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdecomboboxtextlistbase.h"
#include <KLocalizedString>
#include <qvariant.h>
#include <qpushbutton.h>
#include <qtablewidget.h>
#include <qheaderview.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a hk_kdecomboboxtextlistbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdecomboboxtextlistbase::hk_kdecomboboxtextlistbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName( QString::fromAscii(name == NULL ? "hk_kdecomboboxtextlistbase":name) );
    setModal(modal);
    setSizeGripEnabled( true );
    hk_kdecomboboxtextlistbaseLayout = new QGridLayout(this);
    hk_kdecomboboxtextlistbaseLayout->setMargin(11);
    hk_kdecomboboxtextlistbaseLayout->setSpacing(6);
    hk_kdecomboboxtextlistbaseLayout->setObjectName(QString::fromAscii("hk_kdecomboboxtextlistbaseLayout")); 

    Layout5 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout5->setMargin(0);
    Layout5->setSpacing(6);
    Layout5->setObjectName("Layout5");

    ok_button = new QPushButton(this);
    ok_button->setObjectName(QString::fromAscii("ok_button"));
    ok_button->setAutoDefault( true );
    ok_button->setDefault( true );
    Layout5->addWidget( ok_button );

    cancel_button = new QPushButton(this);
    cancel_button->setObjectName(QString::fromAscii("cancel_button"));
    cancel_button->setAutoDefault( true );
    Layout5->addWidget( cancel_button );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout5->addItem( Spacer1 );

    hk_kdecomboboxtextlistbaseLayout->addLayout(Layout5, 0, 1, 2, 1);

    grid = new QTableWidget(this);
    grid -> setObjectName("grid");
    grid->setColumnCount(1);
    grid->setHorizontalHeaderLabels(QStringList() << i18n( "Entry" ));
    grid->setRowCount(1);
    grid->setHorizontalHeaderLabels(QStringList() << i18n( "1" ));

    hk_kdecomboboxtextlistbaseLayout->addWidget( grid, 1, 0 );

    descriptionlabel = new QLabel(this);
    descriptionlabel->setObjectName("descriptionlabel");

    hk_kdecomboboxtextlistbaseLayout->addWidget( descriptionlabel, 0, 0 );
    languageChange();
    resize( QSize(592, 470).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( ok_button, SIGNAL( clicked() ), this, SLOT( ok_clicked() ) );
    connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( grid, SIGNAL(cellChanged(int,int)), this, SLOT( slot_data_changed(int,int) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdecomboboxtextlistbase::~hk_kdecomboboxtextlistbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdecomboboxtextlistbase::languageChange()
{
    setWindowTitle( i18n( "Combobox textlist dialog" ) );
    ok_button->setText( i18n( "&OK" ) );
    ok_button->setShortcut( QKeySequence( QString::null ) );
    cancel_button->setText( i18n( "&Cancel" ) );
    cancel_button->setShortcut( QKeySequence( QString::null ) );
    grid->setHorizontalHeaderLabels(QStringList() << i18n( "Entry" ));
    grid->setVerticalHeaderLabels(QStringList() << i18n( "1" ));
    descriptionlabel->setText( i18n( "Combobox sourcetextlist" ) );
}

void hk_kdecomboboxtextlistbase::slot_data_changed(int,int)
{
    qWarning( "hk_kdecomboboxtextlistbase::slot_data_changed(int,int): Not implemented yet" );
}

void hk_kdecomboboxtextlistbase::ok_clicked()
{
    qWarning( "hk_kdecomboboxtextlistbase::ok_clicked(): Not implemented yet" );
}

