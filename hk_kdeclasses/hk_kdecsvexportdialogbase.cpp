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
** Form implementation generated from reading ui file 'hk_kdecsvexportdialog.ui'
**
** Created: Do Sep 1 10:22:17 2005
**      by: The User Interface Compiler ($Id: hk_kdecsvexportdialogbase.cpp,v 1.2 2005/09/01 08:27:11 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdecsvexportdialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <KLocalizedString>

/*
 *  Constructs a hk_kdecsvexportdialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdecsvexportdialogbase::hk_kdecsvexportdialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdecsvexportdialogbase":name));
    setModal(modal);
    setMinimumSize( QSize( 600, 500 ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    setSizeGripEnabled( true );
    hk_kdecsvexportdialogbaseLayout = new QGridLayout(this);
    hk_kdecsvexportdialogbaseLayout->setMargin(11);
    hk_kdecsvexportdialogbaseLayout->setSpacing(6);
    hk_kdecsvexportdialogbaseLayout->setObjectName(QString::fromAscii("hk_kdecsvexportdialogbaseLayout")); 

    Layout10 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    Layout10->setMargin(0);
    Layout10->setSpacing(1);
    Layout10->setObjectName("Layout10");

    TextLabel1 = new QLabel(this);
    TextLabel1->setObjectName("TextLabel1");
    Layout10->addWidget( TextLabel1 );

    filefield = new QLineEdit(this);
    filefield->setObjectName("filefield");
    
    QSizePolicy policy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    policy.setHeightForWidth(filefield->sizePolicy().hasHeightForWidth());
    filefield->setSizePolicy(policy);
    Layout10->addWidget( filefield );

    filebutton = new QToolButton(this);
    filebutton->setObjectName(QString::fromAscii("filebutton"));
    Layout10->addWidget( filebutton );

    hk_kdecsvexportdialogbaseLayout->addLayout( Layout10, 0, 0, 1, 3 );

    tablefield = new QComboBox(this);
    tablefield->setObjectName(QString::fromAscii("tablefield"));
    tablefield->setEditable( true );
    tablefield->setAutoCompletion( true );

    hk_kdecsvexportdialogbaseLayout->addWidget( tablefield, 2, 1, 1, 2);
    
    typefield = new QComboBox(this);
    typefield->setObjectName(QString::fromAscii("typefield"));
    typefield->setAutoCompletion( true );

    hk_kdecsvexportdialogbaseLayout->addWidget(typefield, 1, 1, 1, 2);

    typelabel = new QLabel(this);
    typelabel->setObjectName("typelabel");

    hk_kdecsvexportdialogbaseLayout->addWidget( typelabel, 1, 0 );

    TextLabel1_2_2 = new QLabel(this);
    TextLabel1_2_2->setObjectName("TextLabel1_2_2");

    hk_kdecsvexportdialogbaseLayout->addWidget( TextLabel1_2_2, 2, 0 );

    columnseparatorfield = new QComboBox(this);
    columnseparatorfield->setObjectName(QString::fromAscii("columnseparatorfield"));
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(columnseparatorfield->sizePolicy().hasHeightForWidth());
    columnseparatorfield->setSizePolicy(policy);
    
    hk_kdecsvexportdialogbaseLayout->addWidget( columnseparatorfield, 3, 1, 1, 2 );

    TextLabel2 = new QLabel(this);
    TextLabel2->setObjectName("TextLabel2");

    hk_kdecsvexportdialogbaseLayout->addWidget( TextLabel2, 3, 0 );

    firstrow = new QCheckBox( this );
    firstrow->setObjectName(QString::fromAscii("firstrow"));
    firstrow->setChecked( true );

    hk_kdecsvexportdialogbaseLayout->addWidget( firstrow, 5, 1 );

    TextLabel3 = new QLabel(this);
    TextLabel3->setObjectName("TextLabel3");
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setHorizontalPolicy(QSizePolicy::Minimum);
    policy.setHeightForWidth(TextLabel3->sizePolicy().hasHeightForWidth());
    TextLabel3->setSizePolicy(policy);
    
    hk_kdecsvexportdialogbaseLayout->addWidget( TextLabel3, 4, 0 );

    TextLabel1_3 = new QLabel(this);
    TextLabel1_3->setObjectName("TextLabel1_3");

    hk_kdecsvexportdialogbaseLayout->addWidget( TextLabel1_3, 5, 0 );

    textdelimiterfield = new QLineEdit(this);
    textdelimiterfield->setObjectName("textdelimiterfield");

    hk_kdecsvexportdialogbaseLayout->addWidget( textdelimiterfield, 4, 1, 1, 2 );
    Spacer1_3 = new QSpacerItem( 206, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    hk_kdecsvexportdialogbaseLayout->addItem( Spacer1_3, 5, 2 );

    moreframe = new QFrame(this);
    moreframe->setObjectName(QString::fromAscii("moreframe"));
    policy.setHorizontalPolicy(QSizePolicy::Preferred);
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(moreframe->sizePolicy().hasHeightForWidth());
    moreframe->setSizePolicy(policy);
    moreframe->setFrameShape( QFrame::StyledPanel );
    moreframe->setFrameShadow( QFrame::Raised );
    moreframeLayout = new QGridLayout(moreframe);
    moreframeLayout->setMargin(11);
    moreframeLayout->setSpacing(6);
    moreframeLayout->setObjectName(QString::fromAscii("moreframeLayout")); 

    dateformatlabel = new QLabel(moreframe);
    dateformatlabel->setObjectName("dateformatlabel");

    moreframeLayout->addWidget( dateformatlabel, 0, 0 );

    localefield = new QComboBox(moreframe);
    localefield->setObjectName(QString::fromAscii("localefield"));
    localefield->setEditable( true );
    localefield->setAutoCompletion( true );

    moreframeLayout->addWidget( localefield, 4, 1 );

    timeformatfield = new QLineEdit(moreframe);
    timeformatfield->setObjectName("timeformatfield");

    moreframeLayout->addWidget( timeformatfield, 1, 1 );

    dateformatfield = new QLineEdit(moreframe);
    dateformatfield->setObjectName("dateformatfield");

    moreframeLayout->addWidget( dateformatfield, 0, 1 );

    datetimeformatfield = new QLineEdit(moreframe);
    datetimeformatfield->setObjectName("datetimeformatfield");

    moreframeLayout->addWidget( datetimeformatfield, 2, 1 );

    charsetfield = new QComboBox(moreframe);
    charsetfield->setObjectName("charsetfield");
    charsetfield->setEditable( true );
    charsetfield->setAutoCompletion( true );

    moreframeLayout->addWidget( charsetfield, 3, 1 );

    charsetlabel = new QLabel(moreframe);
    charsetlabel->setObjectName("charsetlabel");

    moreframeLayout->addWidget( charsetlabel, 3, 0 );

    localelabel = new QLabel(moreframe);
    localelabel->setObjectName("localelabel");

    moreframeLayout->addWidget( localelabel, 4, 0 );

    timeformatlabel = new QLabel(moreframe);
    timeformatlabel->setObjectName("timeformatlabel");

    moreframeLayout->addWidget( timeformatlabel, 1, 0 );

    datetimeformatlabel = new QLabel(moreframe);
    datetimeformatlabel->setObjectName("datetimeformatlabel");

    moreframeLayout->addWidget( datetimeformatlabel, 2, 0 );

    hk_kdecsvexportdialogbaseLayout->addWidget( moreframe, 6, 0, 1, 3 );
    hk_kdecsvexportdialogbaseLayout->addItem( new QSpacerItem( 20, 20), 7, 0, 1,3 );
    hk_kdecsvexportdialogbaseLayout->setRowMinimumHeight(7,200);

    layout7 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout7->setMargin(0);
    layout7->setSpacing(6);
    layout7->setObjectName("layout7");

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName(QString::fromAscii("buttonOk"));
    buttonOk->setEnabled( false );
    buttonOk->setAutoDefault( true );
    buttonOk->setDefault( true );
    layout7->addWidget( buttonOk );

    buttonCancel = new QPushButton( this);
    buttonCancel->setObjectName(QString::fromAscii("buttonCancel"));
    buttonCancel->setAutoDefault( true );
    layout7->addWidget( buttonCancel );

    morebutton = new QPushButton(this);
    morebutton->setObjectName(QString::fromAscii("morebutton"));
    morebutton->setCheckable( true );
    layout7->addWidget( morebutton );

    buttonHelp = new QPushButton(this);
    buttonHelp->setObjectName(QString::fromAscii("buttonHelp"));
    buttonHelp->setEnabled( true );
    buttonHelp->setAutoDefault( true );
    layout7->addWidget( buttonHelp );
    Spacer1 = new QSpacerItem( 20, 270, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout7->addItem( Spacer1 );

    hk_kdecsvexportdialogbaseLayout->addLayout( layout7, 0, 3, 7, 1 );
    languageChange();
    resize( QSize(614, 500).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( filebutton, SIGNAL( clicked() ), this, SLOT( filebutton_clicked() ) );
    connect( filefield, SIGNAL( textChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( columnseparatorfield, SIGNAL( currentTextChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( ok_clicked() ) );
    connect( buttonHelp, SIGNAL( clicked() ), this, SLOT( help_clicked() ) );
    connect( firstrow, SIGNAL( clicked() ), this, SLOT( buttons_enabled() ) );
    connect( tablefield, SIGNAL( currentTextChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( textdelimiterfield, SIGNAL( textChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( typefield, SIGNAL( activated(int) ), this, SLOT( listtype_changed() ) );
    connect( morebutton, SIGNAL( toggled(bool) ), this, SLOT( morebutton_clicked() ) );

    // tab order
    setTabOrder( filefield, columnseparatorfield );
    setTabOrder( columnseparatorfield, textdelimiterfield );
    setTabOrder( textdelimiterfield, firstrow );
    setTabOrder( firstrow, buttonOk );
    setTabOrder( buttonOk, buttonCancel );
    setTabOrder( buttonCancel, buttonHelp );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdecsvexportdialogbase::~hk_kdecsvexportdialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdecsvexportdialogbase::languageChange()
{
    setWindowTitle( i18n( "Text File Export Dialog" ) );
    TextLabel1->setText( i18n( "Filename" ) );
    filefield -> setToolTip( i18n( "The data to import." ) );
    filebutton->setText( i18n( "..." ) );
    typelabel->setText( i18n( "Datasource type" ) );
    TextLabel1_2_2->setText( i18n( "Tablename" ) );
    columnseparatorfield -> setToolTip( i18n( "Pick the character that separates columns in text file." ) );
    TextLabel2->setText( i18n( "Column Separator" ) );
    firstrow->setText( QString::null );
    firstrow -> setToolTip( i18n( "First row contains field names." ) );
    TextLabel3->setText( i18n( "Text delimiter" ) );
    TextLabel1_3->setText( i18n( "Field names in first row" ) );
    textdelimiterfield->setText( i18n( "\"" ) );
    textdelimiterfield -> setToolTip( i18n( "The character that surrounds text (can be blank)." ) );
    dateformatlabel->setText( i18n( "Date format" ) );
    timeformatfield -> setToolTip( i18n( "Name of new or existing table." ) );
    dateformatfield -> setToolTip( i18n( "Name of new or existing table." ) );
    datetimeformatfield -> setToolTip( i18n( "Name of new or existing table." ) );
    charsetlabel->setText( i18n( "Charset" ) );
    localelabel->setText( i18n( "Locale" ) );
    timeformatlabel->setText( i18n( "Time format" ) );
    datetimeformatlabel->setText( i18n( "Datetime format" ) );
    buttonOk->setText( i18n( "&OK" ) );
    buttonOk->setShortcut( QKeySequence( i18n( "Alt+O" ) ) );
    buttonCancel->setText( i18n( "&Cancel" ) );
    buttonCancel->setShortcut( QKeySequence( i18n( "Alt+C" ) ) );
    morebutton->setText( i18n( "O&ptions" ) );
    morebutton->setShortcut( QKeySequence( i18n( "Alt+P" ) ) );
    buttonHelp->setText( i18n( "&Help" ) );
    buttonHelp->setShortcut( QKeySequence( i18n( "Alt+H" ) ) );
}

void hk_kdecsvexportdialogbase::buttons_enabled()
{
    qWarning( "hk_kdecsvexportdialogbase::buttons_enabled(): Not implemented yet" );
}

void hk_kdecsvexportdialogbase::filebutton_clicked()
{
    qWarning( "hk_kdecsvexportdialogbase::filebutton_clicked(): Not implemented yet" );
}

void hk_kdecsvexportdialogbase::ok_clicked()
{
    qWarning( "hk_kdecsvexportdialogbase::ok_clicked(): Not implemented yet" );
}

void hk_kdecsvexportdialogbase::help_clicked()
{
    qWarning( "hk_kdecsvexportdialogbase::help_clicked(): Not implemented yet" );
}

void hk_kdecsvexportdialogbase::listtype_changed()
{
    qWarning( "hk_kdecsvexportdialogbase::listtype_changed(): Not implemented yet" );
}

void hk_kdecsvexportdialogbase::morebutton_clicked()
{
    qWarning( "hk_kdecsvexportdialogbase::morebutton_clicked(): Not implemented yet" );
}

