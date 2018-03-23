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
** Form implementation generated from reading ui file 'reportsection.ui'
**
** Created: Sa Jul 30 22:09:41 2005
**      by: The User Interface Compiler ($Id: hk_kdereportsectiondialogbase.cpp,v 1.5 2005/07/31 12:39:05 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdereportsectiondialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qlistwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

void inline initSizePolicy( QWidget* w, QSizePolicy::Policy hp, QSizePolicy::Policy vp)
{
    QSizePolicy policy(hp,vp);
    
    policy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
    w->setSizePolicy(policy);
}

/*
 *  Constructs a hk_kdereportsectiondialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdereportsectiondialogbase::hk_kdereportsectiondialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdereportsectiondialogbase":name));
    setModal(modal);
    setSizeGripEnabled( TRUE );
    hk_kdereportsectiondialogbaseLayout = new QHBoxLayout(this);
    hk_kdereportsectiondialogbaseLayout->setMargin(11);
    hk_kdereportsectiondialogbaseLayout->setSpacing(6);
    hk_kdereportsectiondialogbaseLayout->setObjectName("hk_kdereportsectiondialogbaseLayout");

    Frame14 = new QFrame(this);
    Frame14->setObjectName(QString::fromAscii("Frame14"));
    initSizePolicy(Frame14, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    Frame14->setFrameShape( QFrame::Box );
    Frame14->setFrameShadow( QFrame::Sunken );
    Frame14Layout = new QHBoxLayout(Frame14);
    Frame14Layout->setMargin(11);
    Frame14Layout->setSpacing(6);
    Frame14Layout->setObjectName("Frame14Layout");

    Layout5 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout5->setMargin(0);
    Layout5->setSpacing(6);
    Layout5->setObjectName("Layout5");

    fieldlabel = new QLabel(Frame14);
    fieldlabel->setObjectName("fieldlabel");
    fieldlabel->setFrameShape( QLabel::NoFrame );
    fieldlabel->setFrameShadow( QLabel::Plain );
    Layout5->addWidget( fieldlabel );

    sectionfield = new QComboBox(Frame14);
    sectionfield->setObjectName(QString::fromAscii("sectionfield"));
    initSizePolicy(sectionfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    sectionfield->setEditable( TRUE );
    Layout5->addWidget( sectionfield );
    Spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout5->addItem( Spacer3 );
    Frame14Layout->addLayout( Layout5 );

    Layout4 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout4->setMargin(0);
    Layout4->setSpacing(6);
    Layout4->setObjectName("Layout4");
    Spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout4->addItem( Spacer2 );

    addbutton = new QToolButton(Frame14);
    addbutton->setObjectName(QString::fromAscii("addbutton"));
    Layout4->addWidget( addbutton );

    deletebutton = new QToolButton( Frame14);
    deletebutton->setObjectName(QString::fromAscii("deletebutton"));
    deletebutton->setEnabled( FALSE );
    Layout4->addWidget( deletebutton );

    upbutton = new QToolButton(Frame14);
    upbutton->setObjectName(QString::fromAscii("upbutton"));
    upbutton->setEnabled( FALSE );
    Layout4->addWidget( upbutton );

    downbutton = new QToolButton(Frame14);
    downbutton->setObjectName(QString::fromAscii("downbutton"));
    downbutton->setEnabled( FALSE );
    Layout4->addWidget( downbutton );
    Spacer1_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout4->addItem( Spacer1_2 );
    Frame14Layout->addLayout( Layout4 );

    Layout10 = new QGridLayout();
    Layout10->setMargin(0);
    Layout10->setSpacing(0);
    Layout10->setObjectName(QString::fromAscii("Layout10")); 

    sectionbox = new QListWidget( Frame14 );
    sectionbox -> setObjectName ( "sectionbox" );
    initSizePolicy(sectionbox, QSizePolicy::Expanding, QSizePolicy::Expanding);
    sectionbox->setMinimumSize( QSize( 55, 0 ) );

    Layout10->addWidget( sectionbox, 1, 0 );

    boxlabel = new QLabel(Frame14);
    boxlabel->setObjectName("boxlabel");

    Layout10->addWidget( boxlabel, 0, 0 );
    Frame14Layout->addLayout( Layout10 );
    hk_kdereportsectiondialogbaseLayout->addWidget( Frame14 );

    Layout11 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout11->setMargin(0);
    Layout11->setSpacing(4);
    Layout11->setObjectName("Layout11");

    exitbutton = new QPushButton(this);
    exitbutton->setObjectName("exitbutton");
    exitbutton->setAutoDefault( TRUE );
    exitbutton->setDefault( TRUE );
    Layout11->addWidget( exitbutton );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout11->addItem( Spacer1 );
    hk_kdereportsectiondialogbaseLayout->addLayout( Layout11 );
    languageChange();
    resize( QSize(528, 228).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( exitbutton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( addbutton, SIGNAL( clicked() ), this, SLOT( add_clicked() ) );
    connect( deletebutton, SIGNAL( clicked() ), this, SLOT( delete_clicked() ) );
    connect( upbutton, SIGNAL( clicked() ), this, SLOT( up_clicked() ) );
    connect( sectionbox, SIGNAL( itemSelectionChanged() ), this, SLOT( check_buttons() ) );
    connect( sectionfield, SIGNAL( textChanged(const QString&) ), this, SLOT( check_buttons() ) );
    connect( downbutton, SIGNAL( clicked() ), this, SLOT( down_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdereportsectiondialogbase::~hk_kdereportsectiondialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdereportsectiondialogbase::languageChange()
{
    setWindowTitle( tr( "hk_kdereportsectiondialogbase" ) );
    //TBP TBT translation tr()->i18n()
    fieldlabel->setText( tr( "TextLabel1" ) );
    addbutton->setText( QString::null );
    deletebutton->setText( QString::null );
    upbutton->setText( QString::null );
    downbutton->setText( QString::null );
    boxlabel->setText( tr( "TextLabel2" ) );
    exitbutton->setText( tr( "&Exit" ) );
}

void hk_kdereportsectiondialogbase::add_clicked()
{
    qWarning( "hk_kdereportsectiondialogbase::add_clicked(): Not implemented yet" );
}

void hk_kdereportsectiondialogbase::check_buttons()
{
    qWarning( "hk_kdereportsectiondialogbase::check_buttons(): Not implemented yet" );
}

void hk_kdereportsectiondialogbase::delete_clicked()
{
    qWarning( "hk_kdereportsectiondialogbase::delete_clicked(): Not implemented yet" );
}

void hk_kdereportsectiondialogbase::down_clicked()
{
    qWarning( "hk_kdereportsectiondialogbase::down_clicked(): Not implemented yet" );
}

void hk_kdereportsectiondialogbase::up_clicked()
{
    qWarning( "hk_kdereportsectiondialogbase::up_clicked(): Not implemented yet" );
}

