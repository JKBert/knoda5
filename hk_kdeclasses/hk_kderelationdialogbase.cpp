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

#include "hk_kderelationdialogbase.h"
#include <KLocalizedString>
#include <qvariant.h>
#include <qpushbutton.h>
#include <qstackedwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <qtablewidget.h>
#include <qheaderview.h>
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
 *  Constructs a hk_kderelationdialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kderelationdialogbase::hk_kderelationdialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kderelationdialogbase":name));
    setModal(modal);
    initSizePolicy(this, QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    setMinimumSize( QSize( 490, 245 ) );
    setSizeGripEnabled( true );
    hk_kderelationdialogbaseLayout = new QGridLayout(this); 
    hk_kderelationdialogbaseLayout->setMargin(2);
    hk_kderelationdialogbaseLayout->setSpacing(3);
    hk_kderelationdialogbaseLayout->setObjectName(QString::fromAscii("hk_kderelationdialogbaseLayout")); 

    detailbox = new QStackedWidget( this  );
    detailbox -> setObjectName("detailbox");
    initSizePolicy(detailbox, QSizePolicy::Preferred, QSizePolicy::Preferred);
    detailbox->setMinimumSize( QSize( 0, 120 ) );

    WStackPage = new QWidget(detailbox);
    WStackPage->setObjectName(QString::fromAscii("WStackPage"));
    WStackPageLayout = new QGridLayout(WStackPage);
    WStackPageLayout->setMargin(11);
    WStackPageLayout->setSpacing(6);
    WStackPageLayout->setObjectName(QString::fromAscii("WStackPageLayout")); 

    integritybox = new QGroupBox(WStackPage);
    integritybox->setObjectName(QString::fromAscii("integritybox"));
    initSizePolicy(integritybox, QSizePolicy::Expanding, QSizePolicy::Preferred);
    integrityboxLayout = new QGridLayout();
    integrityboxLayout->setAlignment( Qt::AlignTop );
    integrityboxLayout->setSpacing( 6 );
    integrityboxLayout->setMargin( 11 );
    integritybox->setLayout(integrityboxLayout);

    onupdatelabel = new QLabel(integritybox);
    onupdatelabel->setObjectName("onupdatelabel");

    integrityboxLayout->addWidget( onupdatelabel, 0, 0 );

    ondeletelabel = new QLabel(integritybox);
    ondeletelabel->setObjectName("ondeletelabel");

    integrityboxLayout->addWidget( ondeletelabel, 1, 0 );

    onupdatefield = new QComboBox(integritybox);
    onupdatefield->setObjectName(QString::fromAscii("onupdatefield"));

    integrityboxLayout->addWidget( onupdatefield, 0, 1 );

    ondeletefield = new QComboBox(integritybox);
    ondeletefield->setObjectName(QString::fromAscii("ondeletefield"));

    integrityboxLayout->addWidget( ondeletefield, 1, 1 );

    WStackPageLayout->addWidget( integritybox, 0, 0 );
    detailbox->addWidget( WStackPage );  

    WStackPage_2 = new QWidget(detailbox);
    WStackPage_2->setObjectName(QString::fromAscii("WStackPage_2"));
    WStackPageLayout_2 = new QHBoxLayout(WStackPage_2);
    WStackPageLayout_2->setMargin(11);
    WStackPageLayout_2->setSpacing(6);
    WStackPageLayout_2->setObjectName("WStackPageLayout_2");

    joingroup = new QGroupBox( WStackPage_2 );
    joingroup -> setObjectName(QString::fromAscii("joingroup"));
    joingroupLayout = new QVBoxLayout( joingroup );
    joingroup->layout()->setSpacing( 6 );
    joingroup->layout()->setMargin( 11 );    
    joingroupLayout->setAlignment( Qt::AlignTop );

    button_all = new QRadioButton(joingroup);
    button_all->setObjectName(QString::fromAscii("button_all"));
    button_all->setChecked( true );
    joingroupLayout->addWidget( button_all );

    button_equivalent = new QRadioButton(joingroup);
    button_equivalent->setObjectName(QString::fromAscii("button_equivalent"));
    button_equivalent->setEnabled( false );
    joingroupLayout->addWidget( button_equivalent );
    WStackPageLayout_2->addWidget( joingroup );
    detailbox->addWidget( WStackPage_2);

    hk_kderelationdialogbaseLayout->addWidget( detailbox, 1, 0 );

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

    hk_kderelationdialogbaseLayout->addLayout( Layout5, 0, 1, 2, 1 );

    grid = new QTableWidget( this );
    grid -> setObjectName( "grid" );
    grid->setColumnCount( grid->columnCount() + 2 );
    grid->setHorizontalHeaderLabels(QStringList() << i18n( "Master" )<< i18n( "Slave" ) );
    grid->setRowCount( 0 );
    grid->setColumnCount( 2 );

    hk_kderelationdialogbaseLayout->addWidget( grid, 0, 0 );
    languageChange();
    resize( QSize(631, 245).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( ok_button, SIGNAL( clicked() ), this, SLOT( ok_button_clicked() ) );
    connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kderelationdialogbase::~hk_kderelationdialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kderelationdialogbase::languageChange()
{
    setWindowTitle( i18n( "hk_kderelationdialog" ) );
    integritybox->setTitle( i18n( "Referential Integrity" ) );
    onupdatelabel->setText( i18n( "On update:" ) );
    ondeletelabel->setText( i18n( "On delete:" ) );
    joingroup->setTitle( i18n( "Relationship method" ) );
    button_all->setText( i18n( "Equivalent rows in both datasources" ) );
    button_equivalent->setText( i18n( "All of 'master' and equivalent rows of 'slave'" ) );
    ok_button->setText( i18n( "&OK" ) );
    ok_button->setShortcut( QKeySequence( QString::null ) );
    cancel_button->setText( i18n( "&Cancel" ) );
    cancel_button->setShortcut( QKeySequence( QString::null ) );
    grid->horizontalHeaderItem(SLAVE_COL)->setText(i18n( "Slave" ) );
    grid->horizontalHeaderItem(MASTER_COL)->setText(i18n( "Master" ) );
}

void hk_kderelationdialogbase::ok_button_clicked()
{
    qWarning( "hk_kderelationdialogbase::ok_button_clicked(): Not implemented yet" );
}

