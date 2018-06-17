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
/****************************************************************************
** Form implementation generated from reading ui file 'xmlexportdialog.ui'
**
** Created: Di Jul 4 17:49:50 2006
**      by: The User Interface Compiler ($Id: hk_kdexmlexportdialogbase.cpp,v 1.10 2006/07/04 16:06:23 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdexmlexportdialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <KLocalizedString>

void inline initSizePolicy( QWidget* w, QSizePolicy::Policy hp, QSizePolicy::Policy vp)
{
    QSizePolicy policy(hp,vp);
    
    policy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
    w->setSizePolicy(policy);
}

/*
 *  Constructs a hk_kdexmlexportdialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdexmlexportdialogbase::hk_kdexmlexportdialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdexmlexportdialogbase":name));
    setModal(modal);
    setMaximumSize( QSize( 32767, 32767 ) );
    setSizeGripEnabled( true );
    hk_kdexmlexportdialogbaseLayout = new QGridLayout(this);
    hk_kdexmlexportdialogbaseLayout->setMargin(11);
    hk_kdexmlexportdialogbaseLayout->setSpacing(6);
    hk_kdexmlexportdialogbaseLayout->setObjectName(QString::fromAscii("hk_kdexmlexportdialogbaseLayout")); 

    layout7 = new QGridLayout();
    layout7->setMargin(0);
    layout7->setSpacing(6);
    layout7->setObjectName(QString::fromAscii("layout7")); 

    maindocumenttagfield = new QLineEdit(this);
    maindocumenttagfield->setObjectName("maindocumenttagfield");
    initSizePolicy(maindocumenttagfield, QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout7->addWidget( maindocumenttagfield, 2, 1 );

    rowelementfield = new QLineEdit(this);
    rowelementfield->setObjectName("rowelementfield");

    layout7->addWidget( rowelementfield, 3, 1 );

    typefield = new QComboBox(this);
    typefield->setObjectName(QString::fromAscii("typefield"));
    typefield->setAutoCompletion( true );

    layout7->addWidget( typefield, 0, 1 );

    typelabel = new QLabel(this);
    typelabel->setObjectName("typelabel");

    layout7->addWidget( typelabel, 0, 0 );

    tablenamefield = new QComboBox(this);
    tablenamefield->setObjectName(QString::fromAscii("tablenamefield"));
    tablenamefield->setMinimumSize( QSize( 150, 0 ) );

    layout7->addWidget( tablenamefield, 1, 1 );

    maindocumenttaglabel = new QLabel(this);
    maindocumenttaglabel->setObjectName("maindocumenttaglabel");

    layout7->addWidget( maindocumenttaglabel, 2, 0 );

    tablenamelabel = new QLabel(this);
    tablenamelabel->setObjectName("tablenamelabel");

    layout7->addWidget( tablenamelabel, 1, 0 );

    rowelementlabel = new QLabel(this);
    rowelementlabel->setObjectName("rowelementlabel");
    initSizePolicy(rowelementlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    layout7->addWidget( rowelementlabel, 3, 0 );

    hk_kdexmlexportdialogbaseLayout->addLayout( layout7, 1, 0 );

    Layout10 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    Layout10->setMargin(0);
    Layout10->setSpacing(1);
    Layout10->setObjectName("Layout10");

    filelable = new QLabel(this);
    filelable->setObjectName("filelable");
    Layout10->addWidget( filelable );

    filefield = new QLineEdit(this);
    filefield->setObjectName("filefield");
    initSizePolicy(filefield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    Layout10->addWidget( filefield );

    filebutton = new QToolButton(this);
    filebutton->setObjectName(QString::fromAscii("filebutton"));
    Layout10->addWidget( filebutton );

    hk_kdexmlexportdialogbaseLayout->addLayout( Layout10, 0, 0 );

    layout5 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    layout5->setMargin(0);
    layout5->setSpacing(6);
    layout5->setObjectName("layout5");

    structurefield = new QCheckBox(this);
    structurefield->setObjectName(QString::fromAscii("structurefield"));
    layout5->addWidget( structurefield );

    attributefield = new QCheckBox(this);
    attributefield->setObjectName(QString::fromAscii("attributefield"));
    layout5->addWidget( attributefield );

    excelxmlfield = new QCheckBox(this);
    excelxmlfield->setObjectName(QString::fromAscii("excelxmlfield"));
    layout5->addWidget( excelxmlfield );
    Spacer1_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout5->addItem( Spacer1_2 );

    hk_kdexmlexportdialogbaseLayout->addLayout( layout5, 2, 0 );
    Spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    hk_kdexmlexportdialogbaseLayout->addItem( Spacer2, 1, 1 );

    Layout5 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout5->setMargin(0);
    Layout5->setSpacing(6);
    Layout5->setObjectName("Layout5");

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName("buttonOk");
    buttonOk->setEnabled( false );
    buttonOk->setAutoDefault( true );
    buttonOk->setDefault( true );
    Layout5->addWidget( buttonOk );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName("buttonCancel");
    buttonCancel->setAutoDefault( true );
    Layout5->addWidget( buttonCancel );

    buttonHelp = new QPushButton(this);
    buttonHelp->setObjectName("buttonHelp");
    buttonHelp->setEnabled( true );
    buttonHelp->setAutoDefault( true );
    Layout5->addWidget( buttonHelp );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout5->addItem( Spacer1 );

    hk_kdexmlexportdialogbaseLayout->addLayout( Layout5, 0, 2, 3, 1 );
    languageChange();
    resize( QSize(585, 345).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( filebutton, SIGNAL( clicked() ), this, SLOT( filebutton_clicked() ) );
    connect( filefield, SIGNAL( textChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( tablenamefield, SIGNAL( currentTextChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( maindocumenttagfield, SIGNAL( textChanged(const QString&) ), this, SLOT( buttons_enabled() ) );
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( ok_clicked() ) );
    connect( typefield, SIGNAL( activated(int) ), this, SLOT( listtype_changed() ) );
    connect( buttonHelp, SIGNAL( clicked() ), this, SLOT( help_clicked() ) );
    connect( excelxmlfield, SIGNAL( clicked() ), this, SLOT( excel_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdexmlexportdialogbase::~hk_kdexmlexportdialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdexmlexportdialogbase::languageChange()
{
    setWindowTitle( i18n( "XML Export Dialog" ) );
    rowelementfield->setText( QString::null );
    typelabel->setText( i18n( "Datasource type" ) );
    maindocumenttaglabel->setText( i18n( "Main document tag:" ) );
    tablenamelabel->setText( i18n( "Tablename:" ) );
    rowelementlabel->setText( i18n( "Row element tag:" ) );
    filelable->setText( i18n( "Filename:" ) );
    filebutton->setText( i18n( "..." ) );
    structurefield->setText( i18n( "include tableschema?" ) );
    attributefield->setText( i18n( "fieldname as attribute?" ) );
    excelxmlfield->setText( i18n( "Export as Excel XML?" ) );
    buttonOk->setText( i18n( "&OK" ) );
    buttonOk->setShortcut( QKeySequence( i18n( "Alt+O" ) ) );
    buttonCancel->setText( i18n( "&Cancel" ) );
    buttonHelp->setText( i18n( "&Help" ) );
    buttonHelp->setShortcut( QKeySequence( i18n( "Alt+H" ) ) );
}

void hk_kdexmlexportdialogbase::excel_clicked()
{
    qWarning( "hk_kdexmlexportdialogbase::excel_clicked(): Not implemented yet" );
}

void hk_kdexmlexportdialogbase::filebutton_clicked()
{
    qWarning( "hk_kdexmlexportdialogbase::filebutton_clicked(): Not implemented yet" );
}

void hk_kdexmlexportdialogbase::ok_clicked()
{
    qWarning( "hk_kdexmlexportdialogbase::ok_clicked(): Not implemented yet" );
}

void hk_kdexmlexportdialogbase::listtype_changed()
{
    qWarning( "hk_kdexmlexportdialogbase::listtype_changed(): Not implemented yet" );
}

void hk_kdexmlexportdialogbase::help_clicked()
{
    qWarning( "hk_kdexmlexportdialogbase::help_clicked(): Not implemented yet" );
}

void hk_kdexmlexportdialogbase::buttons_enabled()
{
    qWarning( "hk_kdexmlexportdialogbase::buttons_enabled(): Not implemented yet" );
}

