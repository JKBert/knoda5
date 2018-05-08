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
** Form implementation generated from reading ui file 'taborder.ui'
**
** Created: Sa Jul 1 14:50:12 2006
**      by: The User Interface Compiler ($Id: hk_kdetaborderbase.cpp,v 1.4 2006/07/01 13:32:21 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdetaborderbase.h"
#include <KLocalizedString>
#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qlabel.h>
#include <qtreewidget.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a hk_kdetaborderdialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
hk_kdetaborderdialogbase::hk_kdetaborderdialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdetaborderdialogbase":name));
    setModal(modal);
    setSizeGripEnabled( true );
    hk_kdetaborderdialogbaseLayout = new QHBoxLayout(this);
    hk_kdetaborderdialogbaseLayout->setMargin(11);
    hk_kdetaborderdialogbaseLayout->setSpacing(6);
    hk_kdetaborderdialogbaseLayout->setObjectName("hk_kdetaborderdialogbaseLayout");

    Frame14 = new QFrame(this);
    Frame14->setObjectName(QString::fromAscii("Frame14"));
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
    
    policy.setHeightForWidth(Frame14->sizePolicy().hasHeightForWidth());
    Frame14->setSizePolicy(policy);
    Frame14->setFrameShape( QFrame::Box );
    Frame14->setFrameShadow( QFrame::Sunken );
    Frame14Layout = new QHBoxLayout(Frame14);
    Frame14Layout->setMargin(11);
    Frame14Layout->setSpacing(6);
    Frame14Layout->setObjectName("Frame14Layout");

    layout7 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout7->setMargin(0);
    layout7->setSpacing(6);
    layout7->setObjectName("layout7");

    baselistlabel = new QLabel(Frame14);
    baselistlabel->setObjectName("baselistlabel");
    layout7->addWidget( baselistlabel );

    baselist = new QTreeWidget( Frame14);
    baselist -> setObjectName("baselist");
    baselist -> setColumnCount(4);
    baselist -> setRootIsDecorated(false);
    baselist -> setSelectionBehavior(QAbstractItemView::SelectRows);
    baselist -> setSelectionMode(QAbstractItemView::SingleSelection);    
    QStringList header;
    header << tr( "Id:" ) << tr( "Name:" ) << tr( "Type:" ) << tr( "Column:" );
    baselist -> setHeaderLabels(header);
    layout7->addWidget( baselist );
    Frame14Layout->addLayout( layout7 );

    Layout4 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout4->setMargin(0);
    Layout4->setSpacing(6);
    Layout4->setObjectName("Layout4");
    Spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout4->addItem( Spacer2 );

    addbutton = new QToolButton(Frame14);
    addbutton->setObjectName(QString::fromAscii("addbutton"));
    Layout4->addWidget( addbutton );

    deletebutton = new QToolButton(Frame14);
    deletebutton->setObjectName(QString::fromAscii("deletebutton"));
    deletebutton->setEnabled( false );
    Layout4->addWidget( deletebutton );

    upbutton = new QToolButton(Frame14);
    upbutton->setObjectName(QString::fromAscii("upbutton"));
    upbutton->setEnabled( false );
    Layout4->addWidget( upbutton );

    downbutton = new QToolButton(Frame14);
    downbutton->setObjectName(QString::fromAscii("downbutton"));
    downbutton->setEnabled( false );
    Layout4->addWidget( downbutton );
    Spacer1_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout4->addItem( Spacer1_2 );
    Frame14Layout->addLayout( Layout4 );

    taborderlistlabel = new QVBoxLayout(static_cast<QWidget*>(NULL));
    taborderlistlabel->setMargin(0);
    taborderlistlabel->setSpacing(6);
    taborderlistlabel->setObjectName("taborderlistlabel");

    boxlabel_2_2 = new QLabel(Frame14);
    boxlabel_2_2->setObjectName("boxlabel_2_2");
    taborderlistlabel->addWidget( boxlabel_2_2 );

    taborderlist = new QTreeWidget( Frame14);
    taborderlist -> setObjectName("taborderlist" );
    taborderlist -> setRootIsDecorated(false);
    taborderlist -> setColumnCount(4);
    taborderlist -> setSelectionBehavior(QAbstractItemView::SelectRows);
    taborderlist -> setSelectionMode(QAbstractItemView::SingleSelection);
    taborderlist -> setHeaderLabels(header);
    taborderlistlabel->addWidget( taborderlist );
    Frame14Layout->addLayout( taborderlistlabel );
    hk_kdetaborderdialogbaseLayout->addWidget( Frame14 );

    layout6 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout6->setMargin(0);
    layout6->setSpacing(6);
    layout6->setObjectName("layout6");

    okbutton = new QPushButton(this);
    okbutton->setObjectName("okbutton");
    okbutton->setAutoDefault( true );
    okbutton->setDefault( true );
    layout6->addWidget( okbutton );

    cancelbutton = new QPushButton(this);
    cancelbutton->setObjectName("cancelbutton");
    cancelbutton->setAutoDefault( true );
    cancelbutton->setDefault( true );
    layout6->addWidget( cancelbutton );
    Spacer1 = new QSpacerItem( 20, 130, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout6->addItem( Spacer1 );
    hk_kdetaborderdialogbaseLayout->addLayout( layout6 );
    languageChange();
    resize( QSize(651, 228).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( deletebutton, SIGNAL( clicked() ), this, SLOT( delete_clicked() ) );
    connect( upbutton, SIGNAL( clicked() ), this, SLOT( up_clicked() ) );
    connect( downbutton, SIGNAL( clicked() ), this, SLOT( down_clicked() ) );
    connect( cancelbutton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( okbutton, SIGNAL( clicked() ), this, SLOT( ok_clicked() ) );
    connect( baselist, SIGNAL( itemSelectionChanged()), this, SLOT( check_buttons() ) );
    connect( taborderlist, SIGNAL( itemSelectionChanged()), this, SLOT( check_buttons() ) );    
    connect( addbutton, SIGNAL( clicked() ), this, SLOT( add_clicked() ) );

    // tab order
    setTabOrder( taborderlist, okbutton );
    setTabOrder( okbutton, cancelbutton );
    setTabOrder( cancelbutton, baselist );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdetaborderdialogbase::~hk_kdetaborderdialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdetaborderdialogbase::languageChange()
{
    setWindowTitle( i18n( "Taborder selection dialog" ) );
    baselistlabel->setText( ki18n( "Base list:" ).toString() );
    baselist->model()->setHeaderData(0,Qt::Horizontal,i18n( "Id:"),Qt::DisplayRole);
    baselist->model()->setHeaderData(1,Qt::Horizontal,i18n( "Name:"),Qt::DisplayRole);
    baselist->model()->setHeaderData(2,Qt::Horizontal,i18n( "Type:"),Qt::DisplayRole);
    baselist->model()->setHeaderData(3,Qt::Horizontal,i18n( "Column:"),Qt::DisplayRole);
    addbutton->setText( QString::null );
    deletebutton->setText( QString::null );
    upbutton->setText( QString::null );
    downbutton->setText( QString::null );
    boxlabel_2_2->setText( i18n( "Taborder list:" ) );
    taborderlist->model()->setHeaderData(0,Qt::Horizontal,i18n( "Id:"),Qt::DisplayRole);
    taborderlist->model()->setHeaderData(1,Qt::Horizontal,i18n( "Name:"),Qt::DisplayRole);
    taborderlist->model()->setHeaderData(2,Qt::Horizontal,i18n( "Type:"),Qt::DisplayRole);
    taborderlist->model()->setHeaderData(3,Qt::Horizontal,i18n( "Column:"),Qt::DisplayRole);
    okbutton->setText( i18n( "&Ok" ) );
    okbutton->setShortcut( QKeySequence( i18n( "Alt+O" ) ) );
    cancelbutton->setText( i18n( "&Cancel" ));
    cancelbutton->setShortcut( QKeySequence( i18n( "Alt+C" ) ) );
}

void hk_kdetaborderdialogbase::add_clicked()
{
    qWarning( "hk_kdetaborderdialogbase::add_clicked(): Not implemented yet" );
}

void hk_kdetaborderdialogbase::check_buttons()
{
    qWarning( "hk_kdetaborderdialogbase::check_buttons(): Not implemented yet" );
}

void hk_kdetaborderdialogbase::delete_clicked()
{
    qWarning( "hk_kdetaborderdialogbase::delete_clicked(): Not implemented yet" );
}

void hk_kdetaborderdialogbase::down_clicked()
{
    qWarning( "hk_kdetaborderdialogbase::down_clicked(): Not implemented yet" );
}

void hk_kdetaborderdialogbase::up_clicked()
{
    qWarning( "hk_kdetaborderdialogbase::up_clicked(): Not implemented yet" );
}

void hk_kdetaborderdialogbase::ok_clicked()
{
    qWarning( "hk_kdetaborderdialogbase::ok_clicked(): Not implemented yet" );
}

