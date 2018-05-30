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
** Form implementation generated from reading ui file 'hk_kdecsvimportdialog.ui'
**
** Created: Do Sep 1 10:04:45 2005
**      by: The User Interface Compiler ($Id: hk_kdecsvimportdialogbase.cpp,v 1.12 2005/09/01 08:27:12 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdecsvimportdialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtablewidget.h>
#include <qframe.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <KLocalizedString>

/*
 *  Constructs a hk_kdecsvimportdialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdecsvimportdialogbase::hk_kdecsvimportdialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdecsvimportdialogbase":name));
    setModal(modal);
    setMinimumSize( QSize( 600, 600 ) );
    setMaximumSize( QSize( 32767, 32767 ) );
    setSizeGripEnabled( true );
    hk_kdecsvimportdialogbaseLayout = new QGridLayout(this);
    hk_kdecsvimportdialogbaseLayout->setMargin(11);
    hk_kdecsvimportdialogbaseLayout->setSpacing(6);
    hk_kdecsvimportdialogbaseLayout->setObjectName(QString::fromAscii("hk_kdecsvimportdialogbaseLayout")); 

    layout6 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout6->setMargin(0);
    layout6->setSpacing(6);
    layout6->setObjectName("layout6");

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName(QString::fromAscii("buttonOk"));
    buttonOk->setEnabled( false );
    buttonOk->setAutoDefault( true );
    buttonOk->setDefault( true );
    layout6->addWidget( buttonOk );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName(QString::fromAscii("buttonCancel"));
    buttonCancel->setAutoDefault( true );
    layout6->addWidget( buttonCancel );

    morebutton = new QPushButton(this);
    morebutton->setObjectName(QString::fromAscii("morebutton"));
    morebutton->setCheckable( true );
    layout6->addWidget( morebutton );

    buttonHelp = new QPushButton(this);
    buttonHelp->setObjectName(QString::fromAscii("buttonHelp"));
    buttonHelp->setEnabled( true );
    buttonHelp->setAutoDefault( true );
    layout6->addWidget( buttonHelp );
    Spacer1 = new QSpacerItem( 20, 290, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout6->addItem( Spacer1 );

    hk_kdecsvimportdialogbaseLayout->addLayout( layout6, 0, 1 );

    layout5 = new QGridLayout();
    layout5->setMargin(0);
    layout5->setSpacing(6);
    layout5->setObjectName(QString::fromAscii("layout5")); 

    layout8 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout8->setMargin(0);
    layout8->setSpacing(6);
    layout8->setObjectName("layout8");

    textLabel1 = new QLabel(this);
    textLabel1->setObjectName("textLabel1");
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    policy.setHeightForWidth(textLabel1->sizePolicy().hasHeightForWidth());   
    textLabel1->setSizePolicy(policy);
    layout8->addWidget( textLabel1 );

    previewtable = new QTableWidget( this);
    previewtable -> setObjectName("previewtable");
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    policy.setHeightForWidth(previewtable->sizePolicy().hasHeightForWidth());
    previewtable->setSizePolicy(policy);
    previewtable->setMinimumSize( QSize( 0, 200 ) );
    previewtable->setRowCount( 0 );
    previewtable->setRowCount( 0 );

    layout8->addWidget( previewtable );

    layout5->addLayout( layout8, 7, 0, 1, 3 );

    moreframe = new QFrame(this);
    moreframe->setObjectName(QString::fromAscii("moreframe"));
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHeightForWidth(moreframe->sizePolicy().hasHeightForWidth());
    moreframe->setSizePolicy(policy);
    moreframe->setFrameShape( QFrame::StyledPanel );
    moreframe->setFrameShadow( QFrame::Raised );
    moreframeLayout = new QGridLayout(moreframe);
    moreframeLayout->setMargin(11);
    moreframeLayout->setSpacing(1);
    moreframeLayout->setObjectName(QString::fromAscii("moreframeLayout")); 

    autoincfield = new QCheckBox(moreframe);
    autoincfield->setObjectName(QString::fromAscii("autoincfield"));
    autoincfield->setChecked( true );

    moreframeLayout->addWidget( autoincfield, 3, 1 );

    datetimeformatfield = new QLineEdit(moreframe);
    datetimeformatfield->setObjectName("datetimeformatfield");

    moreframeLayout->addWidget( datetimeformatfield, 2, 1, 1, 2 );

    dateformatfield = new QLineEdit(moreframe);
    dateformatfield->setObjectName("dateformatfield");

    moreframeLayout->addWidget( dateformatfield, 0, 1, 1, 2 );

    datetimeformatlabel = new QLabel(moreframe);
    datetimeformatlabel->setObjectName("datetimeformatlabel");

    moreframeLayout->addWidget( datetimeformatlabel, 2, 0 );

    localelabel = new QLabel(moreframe);
    localelabel->setObjectName("localelabel");

    moreframeLayout->addWidget( localelabel, 5, 0 );

    charsetfield = new QComboBox(moreframe);
    charsetfield->setObjectName(QString::fromAscii("charsetfield"));
    charsetfield->setEditable( true );
    charsetfield->setAutoCompletion( true );

    moreframeLayout->addWidget( charsetfield, 4, 1, 1, 2 );

    localefield = new QComboBox(moreframe);
    localefield->setObjectName("localefield");
    localefield->setEditable( true );
    localefield->setAutoCompletion( true );

    moreframeLayout->addWidget( localefield, 5, 1, 1, 2 );

    timeformatfield = new QLineEdit(moreframe);
    timeformatfield->setObjectName("timeformatfield");

    moreframeLayout->addWidget( timeformatfield, 1, 1, 1, 2 );

    charsetlabel = new QLabel(moreframe);
    charsetlabel->setObjectName("charsetlabel");

    moreframeLayout->addWidget( charsetlabel, 4, 0 );

    timeformatlabel = new QLabel(moreframe);
    timeformatlabel->setObjectName("timeformatlabel");

    moreframeLayout->addWidget( timeformatlabel, 1, 0 );
    Spacer1_3_3 = new QSpacerItem( 206, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    moreframeLayout->addItem( Spacer1_3_3, 3, 2 );

    autoinclabel = new QLabel(moreframe);
    autoinclabel->setObjectName("autoinclabel");

    moreframeLayout->addWidget( autoinclabel, 3, 0 );

    dateformatlabel = new QLabel(moreframe);
    dateformatlabel->setObjectName("dateformatlabel");

    moreframeLayout->addWidget( dateformatlabel, 0, 0 );

    layout5->addWidget( moreframe, 6, 0, 1, 3 );

    TextLabel1_3 = new QLabel(this);
    TextLabel1_3->setObjectName("TextLabel1_3");

    layout5->addWidget( TextLabel1_3, 5, 0 );

    TextLabel3 = new QLabel(this);
    TextLabel3->setObjectName("TextLabel3");
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::Minimum);
    policy.setHeightForWidth(TextLabel3->sizePolicy().hasHeightForWidth());
    TextLabel3->setSizePolicy(policy);
    layout5->addWidget( TextLabel3, 4, 0 );

    tablename = new QComboBox(this);
    tablename->setObjectName("tablename");
    tablename->setEditable( true );
    tablename->setAutoCompletion( true );

    layout5->addWidget( tablename, 1, 1, 1, 2 );

    lblAppend = new QLabel(this);
    lblAppend->setObjectName("lblAppend");

    layout5->addWidget( lblAppend, 2, 0 );
    Spacer1_3 = new QSpacerItem( 475, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout5->addItem( Spacer1_3, 5, 2 );

    TextLabel1_2 = new QLabel(this);
    TextLabel1_2->setObjectName("TextLabel1_2");

    layout5->addWidget( TextLabel1_2, 1, 0 );

    textdelimiterfield = new QLineEdit(this);
    textdelimiterfield->setObjectName("textdelimiterfield");

    layout5->addWidget( textdelimiterfield, 4, 1, 1, 2 );

    firstrow = new QCheckBox(this);
    firstrow->setObjectName(QString::fromAscii("firstrow"));
    firstrow->setChecked( true );

    layout5->addWidget( firstrow, 5, 1 );
    Spacer1_3_2 = new QSpacerItem( 475, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout5->addItem( Spacer1_3_2, 2, 2 );

    appendrows = new QCheckBox(this);
    appendrows->setObjectName(QString::fromAscii("appendrows"));
    appendrows->setChecked( false );

    layout5->addWidget( appendrows, 2, 1 );

    Layout10 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    Layout10->setMargin(0);
    Layout10->setSpacing(1);
    Layout10->setObjectName("Layout10");

    TextLabel1 = new QLabel(this);
    TextLabel1->setObjectName("TextLabel1");
    Layout10->addWidget( TextLabel1 );

    filefield = new QLineEdit(this);
    filefield->setObjectName("filefield");
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(filefield->sizePolicy().hasHeightForWidth());
    filefield->setSizePolicy(policy);
    Layout10->addWidget( filefield );

    filebutton = new QToolButton(this); 
    filebutton->setObjectName(QString::fromAscii("filebutton"));
    Layout10->addWidget( filebutton );

    layout5->addLayout( Layout10, 0, 0, 1, 3 );

    TextLabel2 = new QLabel(this);
    TextLabel2->setObjectName("TextLabel2");

    layout5->addWidget( TextLabel2, 3, 0 );

    columnseparatorfield = new QComboBox(this);
    columnseparatorfield->setObjectName("columnseparatorfield");
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(columnseparatorfield->sizePolicy().hasHeightForWidth());
    filefield->setSizePolicy(policy);
    layout5->addWidget( columnseparatorfield, 3, 1, 1, 2 );

    hk_kdecsvimportdialogbaseLayout->addLayout( layout5, 0, 0 );
    languageChange();
    resize( QSize(891, 729).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( filebutton, SIGNAL( clicked() ), this, SLOT( filebutton_clicked() ) );
    connect( filefield, SIGNAL( textChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( columnseparatorfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( ok_clicked() ) );
    connect( buttonHelp, SIGNAL( clicked() ), this, SLOT( help_clicked() ) );
    connect( firstrow, SIGNAL( clicked() ), this, SLOT( buttons_enabled() ) );
    connect( tablename, SIGNAL( editTextChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( appendrows, SIGNAL( clicked() ), this, SLOT( buttons_enabled() ) );
    connect( textdelimiterfield, SIGNAL( textChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( morebutton, SIGNAL( toggled(bool) ), this, SLOT( morebutton_clicked() ) );

    // tab order
    setTabOrder( filefield, appendrows );
    setTabOrder( appendrows, columnseparatorfield );
    setTabOrder( columnseparatorfield, textdelimiterfield );
    setTabOrder( textdelimiterfield, firstrow );
    setTabOrder( firstrow, buttonOk );
    setTabOrder( buttonOk, buttonCancel );
    setTabOrder( buttonCancel, buttonHelp );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdecsvimportdialogbase::~hk_kdecsvimportdialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdecsvimportdialogbase::languageChange()
{
    setWindowTitle( i18n( "Text File Import Dialog" ) );
    buttonOk->setText( i18n( "&OK" ) );
    buttonOk->setShortcut( QKeySequence( i18n( "Alt+O" ) ) );
    buttonCancel->setText( i18n( "&Cancel" ) );
    buttonCancel->setShortcut( QKeySequence( i18n( "Alt+C" ) ) );
    morebutton->setText( i18n( "O&ptions" ) );
    morebutton->setShortcut( QKeySequence( i18n( "Alt+P" ) ) );
    buttonHelp->setText( i18n( "&Help" ) );
    buttonHelp->setShortcut( QKeySequence( i18n( "Alt+H" ) ) );
    textLabel1->setText( i18n( "Preview:" ) );
    autoincfield->setText( QString::null );
    autoincfield -> setToolTip( i18n( "First row contains field names." ) );
    datetimeformatfield -> setToolTip( i18n( "Name of new or existing table." ) );
    dateformatfield -> setToolTip( i18n( "Name of new or existing table." ) );
    datetimeformatlabel->setText( i18n( "Datetime format" ) );
    localelabel->setText( i18n( "Locale" ) );
    timeformatfield -> setToolTip( i18n( "Name of new or existing table." ) );
    charsetlabel->setText( i18n( "Charset" ) );
    timeformatlabel->setText( i18n( "Time format" ) );
    autoinclabel->setText( i18n( "Detect \"Autoincrement\" " ) );
    dateformatlabel->setText( i18n( "Date format" ) );
    TextLabel1_3->setText( i18n( "Field names in first row" ) );
    TextLabel3->setText( i18n( "Text delimiter" ) );
    lblAppend->setText( i18n( "Append data" ) );
    TextLabel1_2->setText( i18n( "Tablename" ) );
    textdelimiterfield->setText( i18n( "\"" ) );
    textdelimiterfield -> setToolTip( i18n( "The character that surrounds text (can be blank)." ) );
    firstrow->setText( QString::null );
    firstrow -> setToolTip( i18n( "First row contains field names." ) );
    appendrows->setText( QString::null );
    appendrows -> setToolTip( i18n( "Don't overwrite data in table." ) );
    TextLabel1->setText( i18n( "Filename" ) );
    filefield -> setToolTip( i18n( "The data to import." ) );
    filebutton->setText( i18n( "..." ) );
    TextLabel2->setText( i18n( "Column Separator" ) );
    columnseparatorfield -> setToolTip( i18n( "Pick the character that separates columns in text file." ) );
}

void hk_kdecsvimportdialogbase::buttons_enabled()
{
    qWarning( "hk_kdecsvimportdialogbase::buttons_enabled(): Not implemented yet" );
}

void hk_kdecsvimportdialogbase::filebutton_clicked()
{
    qWarning( "hk_kdecsvimportdialogbase::filebutton_clicked(): Not implemented yet" );
}

void hk_kdecsvimportdialogbase::ok_clicked()
{
    qWarning( "hk_kdecsvimportdialogbase::ok_clicked(): Not implemented yet" );
}

void hk_kdecsvimportdialogbase::help_clicked()
{
    qWarning( "hk_kdecsvimportdialogbase::help_clicked(): Not implemented yet" );
}

void hk_kdecsvimportdialogbase::morebutton_clicked()
{
    qWarning( "hk_kdecsvimportdialogbase::morebutton_clicked(): Not implemented yet" );
}

