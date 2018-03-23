// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
//
// This file is part of the hk_kde4classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
/****************************************************************************
** Form implementation generated from reading ui file 'hk_kdeeximportdatabasebase.ui'
**
** Created: Mi Aug 3 15:21:04 2005
**      by: The User Interface Compiler ($Id: hk_kdeeximportdatabasebase.cpp,v 1.5 2005/08/03 14:28:49 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdeeximportdatabasebase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtreewidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
//TBP TBT translation tr()->i18n()
/*
 *  Constructs a hk_kdeeximportdatabasebase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdeeximportdatabasebase::hk_kdeeximportdatabasebase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdeeximportdatabasebase":name));
    setModal(modal);
    hk_kdeeximportdatabasebaseLayout = new QGridLayout( this);
    hk_kdeeximportdatabasebaseLayout->setMargin(11);
    hk_kdeeximportdatabasebaseLayout->setSpacing(6);
    hk_kdeeximportdatabasebaseLayout->setObjectName(QString::fromAscii("hk_kdeeximportdatabasebaseLayout")); 

    exitbutton = new QPushButton( this);
    exitbutton->setObjectName(QString::fromAscii("exitbutton"));

    hk_kdeeximportdatabasebaseLayout->addWidget( exitbutton, 0, 3 );

    copyfield = new QCheckBox(this);
    copyfield->setObjectName(QString::fromAscii("copyfield"));
    copyfield->setChecked( TRUE );

    hk_kdeeximportdatabasebaseLayout->addWidget( copyfield, 3, 2 );

    layout1 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout1->setMargin(0);
    layout1->setSpacing(6);
    layout1->setObjectName("layout1");
    spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout1->addItem( spacer1 );

    uploadbutton = new QPushButton( this);
    uploadbutton->setObjectName(QString::fromAscii("uploadbutton"));
    layout1->addWidget( uploadbutton );
    spacer2 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout1->addItem( spacer2 );

    hk_kdeeximportdatabasebaseLayout->addLayout( layout1, 0, 1, 4, 1 );

    outputfield = new QTextEdit(this);
    outputfield->setObjectName(QString::fromAscii("outputfield"));
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    policy.setHeightForWidth(outputfield->sizePolicy().hasHeightForWidth());
    outputfield->setSizePolicy(policy);
    outputfield->setReadOnly( TRUE );

    hk_kdeeximportdatabasebaseLayout->addWidget( outputfield, 4, 0, 1, 3 );

    layout7 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout7->setMargin(0);
    layout7->setSpacing(6);
    layout7->setObjectName("layout7");

    rightlabel = new QLabel(this);
    rightlabel->setObjectName("rightlabel");
    layout7->addWidget( rightlabel );

    layout14 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    layout14->setMargin(0);
    layout14->setSpacing(6);
    layout14->setObjectName("layout14");

    rightdatabasefield = new QComboBox(this);
    rightdatabasefield->setObjectName(QString::fromAscii("rightdatabasefield"));
    policy.setHorizontalPolicy(QSizePolicy::Preferred);
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHeightForWidth(rightdatabasefield->sizePolicy().hasHeightForWidth());
    rightdatabasefield->setSizePolicy(policy);
    layout14->addWidget( rightdatabasefield );

    rightnewbutton = new QPushButton( this);
    rightnewbutton->setObjectName(QString::fromAscii("rightnewbutton"));
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(rightnewbutton->sizePolicy().hasHeightForWidth());
    rightnewbutton->setSizePolicy(policy);
    rightnewbutton->setMinimumSize( QSize( 30, 30 ) );
    rightnewbutton->setMaximumSize( QSize( 30, 30 ) );
    layout14->addWidget( rightnewbutton );
    layout7->addLayout( layout14 );

    right_listview = new QTreeWidget( this);
    right_listview -> setObjectName( "right_listview" );
    layout7->addWidget( right_listview );

    hk_kdeeximportdatabasebaseLayout->addLayout( layout7, 0, 2, 3, 1 );

    overwritefield = new QCheckBox(this);
    overwritefield->setObjectName(QString::fromAscii("overwritefield"));
    overwritefield->setChecked( TRUE );

    hk_kdeeximportdatabasebaseLayout->addWidget( overwritefield, 3, 0 );

    layout6 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout6->setMargin(0);
    layout6->setSpacing(6);
    layout6->setObjectName("layout6");

    leftlabel = new QLabel(this);
    leftlabel->setObjectName("leftlabel");
    layout6->addWidget( leftlabel );

    layout14_2 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    layout14_2->setMargin(0);
    layout14_2->setSpacing(6);
    layout14_2->setObjectName("layout14_2");

    leftdatabasefield = new QComboBox(this);
    leftdatabasefield->setObjectName(QString::fromAscii("leftdatabasefield"));
    policy.setHorizontalPolicy(QSizePolicy::Preferred);
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHeightForWidth(leftdatabasefield->sizePolicy().hasHeightForWidth());
    leftdatabasefield->setSizePolicy(policy);
    layout14_2->addWidget( leftdatabasefield );

    leftnewbutton = new QPushButton( this);
    leftnewbutton->setObjectName(QString::fromAscii("leftnewbutton"));
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(leftnewbutton->sizePolicy().hasHeightForWidth());
    leftnewbutton->setSizePolicy(policy);
    leftnewbutton->setMinimumSize( QSize( 30, 30 ) );
    leftnewbutton->setMaximumSize( QSize( 30, 30 ) );
    layout14_2->addWidget( leftnewbutton );
    layout6->addLayout( layout14_2 );

    left_listview = new QTreeWidget( this );
    left_listview -> setObjectName( "left_listview" );
    layout6->addWidget( left_listview );

    hk_kdeeximportdatabasebaseLayout->addLayout( layout6, 0, 0, 3, 1 );
    
    spacer4 = new QSpacerItem( 20, 460, QSizePolicy::Minimum, QSizePolicy::Expanding );
    hk_kdeeximportdatabasebaseLayout->addItem(spacer4, 2, 3, 3, 1);

    buttonHelp = new QPushButton( this);
    buttonHelp->setObjectName(QString::fromAscii("buttonHelp"));
    buttonHelp->setEnabled( TRUE );
    buttonHelp->setAutoDefault( TRUE );

    hk_kdeeximportdatabasebaseLayout->addWidget( buttonHelp, 1, 3 );
    languageChange();
    resize( QSize(644, 564).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( exitbutton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( uploadbutton, SIGNAL( clicked() ), this, SLOT( upload_clicked() ) );
    connect( left_listview, SIGNAL( itemSelectionChanged()), this, SLOT( slot_selection_changed() ) );
    connect( right_listview, SIGNAL( itemSelectionChanged()) , this, SLOT( slot_selection_changed() ) );
    connect( leftnewbutton, SIGNAL( clicked() ), this, SLOT( leftnewbutton_clicked() ) );
    connect( rightnewbutton, SIGNAL( clicked() ), this, SLOT( rightnewbutton_clicked() ) );
    connect( buttonHelp, SIGNAL( clicked() ), this, SLOT( help_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeeximportdatabasebase::~hk_kdeeximportdatabasebase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdeeximportdatabasebase::languageChange()
{
    setWindowTitle( tr( "Form1" ) );
    exitbutton->setText( tr( "E&xit" ) );
    exitbutton->setShortcut( QKeySequence( tr( "Alt+X" ) ) );
    copyfield->setText( tr( "Copy structure and data" ) );
    uploadbutton->setText( QString::null );
    rightlabel->setText( QString::null );
    rightnewbutton->setText( tr( "..." ) );
    overwritefield->setText( tr( "Overwrite" ) );
    leftlabel->setText( QString::null );
    leftnewbutton->setText( tr( "..." ) );
    buttonHelp->setText( tr( "&Help" ) );
    buttonHelp->setShortcut( QKeySequence( tr( "Alt+H" ) ) );
}

void hk_kdeeximportdatabasebase::download_clicked()
{
    qWarning( "hk_kdeeximportdatabasebase::download_clicked(): Not implemented yet" );
}

void hk_kdeeximportdatabasebase::upload_clicked()
{
    qWarning( "hk_kdeeximportdatabasebase::upload_clicked(): Not implemented yet" );
}

void hk_kdeeximportdatabasebase::slot_selection_changed()
{
    qWarning( "hk_kdeeximportdatabasebase::slot_selection_changed(): Not implemented yet" );
}

void hk_kdeeximportdatabasebase::leftnewbutton_clicked()
{
    qWarning( "hk_kdeeximportdatabasebase::leftnewbutton_clicked(): Not implemented yet" );
}

void hk_kdeeximportdatabasebase::rightnewbutton_clicked()
{
    qWarning( "hk_kdeeximportdatabasebase::rightnewbutton_clicked(): Not implemented yet" );
}

void hk_kdeeximportdatabasebase::help_clicked()
{
    qWarning( "hk_kdeeximportdatabasebase::help_clicked(): Not implemented yet" );
}

