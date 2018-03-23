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
** Form implementation generated from reading ui file 'databaseconfig.ui'
**
** Created: Sa Aug 26 22:49:32 2006
**      by: The User Interface Compiler ($Id: hk_kdedatabasesettingbase.cpp,v 1.7 2006/08/27 07:41:31 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdedatabasesettingbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qstackedwidget.h>
#include <qwidget.h>
#include <qcombobox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qtreewidget.h>
#include <qheaderview.h>
//TBP TBT translation tr()->i18n()
/*
 *  Constructs a hk_kdedatabasesettingbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdedatabasesettingbase::hk_kdedatabasesettingbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdedatabasesettingbase":name));
    setModal(modal);
    setMinimumSize( QSize( 600, 0 ) );
    setSizeGripEnabled( TRUE );
    hk_kdedatabasesettingbaseLayout = new QGridLayout(this);
    hk_kdedatabasesettingbaseLayout->setMargin(11);
    hk_kdedatabasesettingbaseLayout->setSpacing(6);
    hk_kdedatabasesettingbaseLayout->setObjectName(QString::fromAscii("hk_kdedatabasesettingbaseLayout")); 

    headertext = new QLabel(this);
    headertext->setObjectName("headertext");
    QFont headertext_font(  headertext->font() );
    headertext_font.setBold( TRUE );
    headertext->setFont( headertext_font ); 

    hk_kdedatabasesettingbaseLayout->addWidget( headertext, 0, 1 );

    textLabel2 = new QLabel( this);
    textLabel2->setFrameShape( QLabel::HLine );

    hk_kdedatabasesettingbaseLayout->addWidget( textLabel2, 1, 1 );

    Layout1 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    Layout1->setMargin(0);
    Layout1->setSpacing(6);
    Layout1->setObjectName("Layout1");

    buttonHelp = new QPushButton(this);
    buttonHelp->setObjectName(QString::fromAscii("buttonHelp"));
    buttonHelp->setAutoDefault( TRUE );
    Layout1->addWidget( buttonHelp );
    Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout1->addItem( Horizontal_Spacing2 );

    ok_button = new QPushButton(this);
    ok_button->setObjectName(QString::fromAscii("ok_button"));
    ok_button->setEnabled( FALSE );
    ok_button->setAutoDefault( TRUE );
    ok_button->setDefault( TRUE );
    Layout1->addWidget( ok_button );

    cancel_button = new QPushButton(this);
    cancel_button->setObjectName(QString::fromAscii("cancel_button"));
    cancel_button->setAutoDefault( TRUE );
    Layout1->addWidget( cancel_button );

    hk_kdedatabasesettingbaseLayout->addLayout( Layout1, 4, 0, 1, 2 );

    listview = new QTreeWidget( this);
    listview -> headerItem() -> setText(0,QString::null);
    listview->setRootIsDecorated(false);
    listview -> header() -> setClickable(false);
    listview -> header() -> setResizeMode(QHeaderView::Fixed);
    QFont listview_font(  listview->font() );
    listview_font.setBold( TRUE );
    listview->setFont( listview_font ); 

    hk_kdedatabasesettingbaseLayout->addWidget( listview, 0, 0, 4, 1 );
    spacer2 = new QSpacerItem( 20, 440, QSizePolicy::Minimum, QSizePolicy::Expanding );
    hk_kdedatabasesettingbaseLayout->addItem( spacer2, 3, 1 );

    widgetstack = new QStackedWidget( this  ); 
    widgetstack->setObjectName("widgetstack");
    widgetstack->setLineWidth( 0 );

    systemtablepage = new QWidget( widgetstack);
    systemtablepage->setObjectName(QString::fromAscii("systemtablepage"));
    systemtablepageLayout = new QGridLayout(systemtablepage);
    systemtablepageLayout->setMargin(11);
    systemtablepageLayout->setSpacing(6);
    systemtablepageLayout->setObjectName(QString::fromAscii("systemtablepageLayout")); 

    createlabel = new QLabel(systemtablepage);
    createlabel->setObjectName("createlabel");

    systemtablepageLayout->addWidget( createlabel, 1, 0 );

    createbutton = new QPushButton(systemtablepage);
    createbutton->setObjectName(QString::fromAscii("createbutton"));

    systemtablepageLayout->addWidget( createbutton, 1, 1 );

    automaticupdatefield = new QComboBox(systemtablepage);
    automaticupdatefield->setObjectName("automaticupdatefield");

    systemtablepageLayout->addWidget( automaticupdatefield, 0, 1 );

    automaticupdatelabel = new QLabel(systemtablepage);
    automaticupdatelabel->setObjectName("automaticupdatelabel");

    systemtablepageLayout->addWidget( automaticupdatelabel, 0, 0 );

    storageframe = new QFrame(systemtablepage);
    storageframe->setObjectName(QString::fromAscii("storageframe"));
    storageframe->setEnabled( FALSE );
    storageframe->setFrameShape( QFrame::StyledPanel );
    storageframe->setFrameShadow( QFrame::Sunken );
    storageframeLayout = new QGridLayout( storageframe);
    storageframeLayout->setMargin(11);
    storageframeLayout->setSpacing(6);
    storageframeLayout->setObjectName(QString::fromAscii("storageframeLayout")); 

    loadlabel = new QLabel(storageframe);
    loadlabel->setObjectName("loadlabel");
    loadlabel->setAlignment(Qt::AlignCenter);

    storageframeLayout->addWidget( loadlabel, 0, 1 );

    querylabel = new QLabel(storageframe);
    querylabel->setObjectName("querylabel");
    querylabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    storageframeLayout->addWidget( querylabel, 1, 0 );

    formlabel = new QLabel(storageframe);
    formlabel->setObjectName("formlabel");
    formlabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    storageframeLayout->addWidget( formlabel, 2, 0 );

    formload = new QComboBox(storageframe);
    formload->setObjectName("formload");

    storageframeLayout->addWidget( formload, 2, 1 );

    queryload = new QComboBox(storageframe);
    queryload->setObjectName("queryload");

    storageframeLayout->addWidget( queryload, 1, 1 );

    allload = new QComboBox(storageframe);
    allload->setObjectName("allload");

    storageframeLayout->addWidget( allload, 5, 1 );

    alllabel = new QLabel(storageframe);
    alllabel->setObjectName("alllabel");
    alllabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    storageframeLayout->addWidget( alllabel, 5, 0 );

    reportlabel = new QLabel(storageframe);
    reportlabel->setObjectName("reportlabel");
    reportlabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    storageframeLayout->addWidget( reportlabel, 3, 0 );

    reportload = new QComboBox(storageframe);
    reportload->setObjectName("reportload");

    storageframeLayout->addWidget( reportload, 3, 1 );

    moduleload = new QComboBox(storageframe);
    moduleload->setObjectName("moduleload");

    storageframeLayout->addWidget( moduleload, 4, 1 );

    modulelabel = new QLabel(storageframe);
    modulelabel->setObjectName("modulelabel");
    modulelabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    storageframeLayout->addWidget( modulelabel, 4, 0 );

    systemtablepageLayout->addWidget( storageframe, 2, 0, 1, 2 );
    widgetstack->addWidget( systemtablepage);

    encodingpage = new QWidget(widgetstack);
    encodingpage->setObjectName(QString::fromAscii("encodingpage"));
    encodingpageLayout = new QGridLayout(encodingpage);
    encodingpageLayout->setMargin(11);
    encodingpageLayout->setSpacing(6);
    encodingpageLayout->setObjectName(QString::fromAscii("encodingpageLayout")); 

    databaseencodingfield = new QComboBox(encodingpage);
    databaseencodingfield->setObjectName("databaseencodingfield");
    databaseencodingfield->setEditable( TRUE );
    databaseencodingfield->setAutoCompletion( TRUE );

    encodingpageLayout->addWidget( databaseencodingfield, 0, 1 );

    databaseencodinglabel = new QLabel(encodingpage);
    databaseencodinglabel->setObjectName("databaseencodinglabel");

    encodingpageLayout->addWidget( databaseencodinglabel, 0, 0 );
    spacer3 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    encodingpageLayout->addItem( spacer3, 1, 0 );
    spacer4 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    encodingpageLayout->addItem( spacer4, 1, 1 );
    widgetstack->addWidget( encodingpage );

    hk_kdedatabasesettingbaseLayout->addWidget( widgetstack, 2, 1 );
    languageChange();
    resize( QSize(600, 483).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( ok_button, SIGNAL( clicked() ), this, SLOT( ok_clicked() ) );
    connect( cancel_button, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( listview, SIGNAL( itemSelectionChanged()), this, SLOT( listview_changed() ) );
    connect( formload, SIGNAL( activated(int) ), this, SLOT( selection_changed() ) );
    connect( queryload, SIGNAL( activated(int) ), this, SLOT( selection_changed() ) );
    connect( reportload, SIGNAL( activated(int) ), this, SLOT( selection_changed() ) );
    connect( allload, SIGNAL( activated(int) ), this, SLOT( alload_changed() ) );
    connect( createbutton, SIGNAL( clicked() ), this, SLOT( create_clicked() ) );
    connect( databaseencodingfield, SIGNAL( textChanged(const QString&) ), this, SLOT( encoding_changed() ) );
    connect( automaticupdatefield, SIGNAL( activated(int) ), this, SLOT( selection_changed() ) );
    connect( moduleload, SIGNAL( activated(int) ), this, SLOT( selection_changed() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdedatabasesettingbase::~hk_kdedatabasesettingbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdedatabasesettingbase::languageChange()
{
    setWindowTitle( tr( "Database settings" ) );
    headertext->setText( tr( "Open and store files:" ) );
    buttonHelp->setText( tr( "&Help" ) );
    buttonHelp->setShortcut( QKeySequence( tr( "F1" ) ) );
    ok_button->setText( tr( "&OK" ) );
    ok_button->setShortcut( QKeySequence( QString::null ) );
    cancel_button->setText( tr( "&Cancel" ) );
    cancel_button->setShortcut( QKeySequence( QString::null ) );
    listview -> headerItem() -> setText(0,QString::null);
    createlabel->setText( tr( "Create system table:" ) );
    createbutton->setText( tr( "C&reate" ) );
    createbutton->setShortcut( QKeySequence( tr( "Alt+R" ) ) );
    automaticupdatefield->clear();
    automaticupdatefield->addItem( tr( "Yes" ) );
    automaticupdatefield->addItem( tr( "No" ) );
    automaticupdatelabel->setText( tr( "Automatic data update:" ) );
    loadlabel->setText( tr( "Load/Store" ) );
    querylabel->setText( tr( "Queries:" ) );
    formlabel->setText( tr( "Forms:" ) );
    alllabel->setText( tr( "Set all:" ) );
    reportlabel->setText( tr( "Reports:" ) );
    modulelabel->setText( tr( "Modules:" ) );
    databaseencodinglabel->setText( tr( "Encoding:" ) );
}

void hk_kdedatabasesettingbase::ok_clicked()
{
    qWarning( "hk_kdedatabasesettingbase::ok_clicked(): Not implemented yet" );
}

void hk_kdedatabasesettingbase::selection_changed()
{
    qWarning( "hk_kdedatabasesettingbase::selection_changed(): Not implemented yet" );
}

void hk_kdedatabasesettingbase::listview_changed()
{
    qWarning( "hk_kdedatabasesettingbase::listview_changed(): Not implemented yet" );
}

void hk_kdedatabasesettingbase::create_clicked()
{
    qWarning( "hk_kdedatabasesettingbase::create_clicked(): Not implemented yet" );
}

void hk_kdedatabasesettingbase::alload_changed()
{
    qWarning( "hk_kdedatabasesettingbase::alload_changed(): Not implemented yet" );
}

void hk_kdedatabasesettingbase::allstore_changed()
{
    qWarning( "hk_kdedatabasesettingbase::allstore_changed(): Not implemented yet" );
}

void hk_kdedatabasesettingbase::encoding_changed()
{
    qWarning( "hk_kdedatabasesettingbase::encoding_changed(): Not implemented yet" );
}

