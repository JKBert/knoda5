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
 //$Revision: 1.3 $

#include "hk_kdeobjecthandlerbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "hk_kdedblistview.h"
/*
 *  Constructs a hk_kdeobjecthandlerbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdeobjecthandlerbase::hk_kdeobjecthandlerbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdeobjecthandlerbase":name));
    setModal(modal);
    hk_kdeobjecthandlerbaseLayout = new QGridLayout(this);
    hk_kdeobjecthandlerbaseLayout->setMargin(11);
    hk_kdeobjecthandlerbaseLayout->setSpacing(6);
    hk_kdeobjecthandlerbaseLayout->setObjectName(QString::fromAscii("hk_kdeobjecthandlerbaseLayout")); 

    layout1 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout1->setMargin(0);
    layout1->setSpacing(6);
    layout1->setObjectName("layout1");
    spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout1->addItem( spacer1 );

    uploadbutton = new QPushButton(this);
    uploadbutton->setObjectName("uploadbutton");
    layout1->addWidget( uploadbutton );

    downloadbutton = new QPushButton(this);
    downloadbutton->setObjectName("downloadbutton");
    layout1->addWidget( downloadbutton );
    spacer2 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout1->addItem( spacer2 );

    hk_kdeobjecthandlerbaseLayout->addLayout( layout1, 0, 1 );

    layout2 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    layout2->setMargin(0);
    layout2->setSpacing(6);
    layout2->setObjectName("layout2");
    spacer3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout2->addItem( spacer3 );

    exitbutton = new QPushButton(this);
    exitbutton->setObjectName("exitbutton");
    layout2->addWidget( exitbutton );

    hk_kdeobjecthandlerbaseLayout->addLayout( layout2, 1, 2 );

    layout3 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout3->setMargin(0);
    layout3->setSpacing(6);
    layout3->setObjectName("layout3");


    centrallistview = new hk_kdedblistview( this, "centrallistview" );
    layout3->addWidget( centrallistview );

    hk_kdeobjecthandlerbaseLayout->addLayout( layout3, 0, 2 );

    layout4 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout4->setMargin(0);
    layout4->setSpacing(6);
    layout4->setObjectName("layout4");


    locallistview = new hk_kdedblistview( this, "locallistview" );
    layout4->addWidget( locallistview );

    hk_kdeobjecthandlerbaseLayout->addLayout( layout4, 0, 0 );
    languageChange();
    resize( QSize(600, 480).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( exitbutton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( downloadbutton, SIGNAL( clicked() ), this, SLOT( download_clicked() ) );
    connect( uploadbutton, SIGNAL( clicked() ), this, SLOT( upload_clicked() ) );
    connect( locallistview, SIGNAL( itemSelectionChanged()), this, SLOT( slot_selection_changed() ) );
    connect( centrallistview, SIGNAL( itemSelectionChanged()), this, SLOT( slot_selection_changed() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeobjecthandlerbase::~hk_kdeobjecthandlerbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdeobjecthandlerbase::languageChange()
{
    setWindowTitle( tr( "Form1" ) );
    uploadbutton->setText( QString::null );
    downloadbutton->setText( QString::null );
    exitbutton->setText( tr( "E&xit" ) );
}

void hk_kdeobjecthandlerbase::download_clicked()
{
    qWarning( "hk_kdeobjecthandlerbase::download_clicked(): Not implemented yet" );
}

void hk_kdeobjecthandlerbase::upload_clicked()
{
    qWarning( "hk_kdeobjecthandlerbase::upload_clicked(): Not implemented yet" );
}

void hk_kdeobjecthandlerbase::slot_selection_changed()
{
    qWarning( "hk_kdeobjecthandlerbase::slot_selection_changed(): Not implemented yet" );
}

