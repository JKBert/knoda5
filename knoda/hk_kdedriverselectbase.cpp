// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for knoda -Knorr's Datenbank.
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for knoda4 -Knorr's Datenbank.
//
// This file is part of the knoda4 -Knorr's Datenbank.
// This file may be distributed and/or modified under the terms of the
// GNU General Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
/****************************************************************************
** Form implementation generated from reading ui file 'hk_kdedriverselect.ui'
**
** Created: Mo Aug 29 07:49:24 2005
**      by: The User Interface Compiler ($Id: hk_kdedriverselectbase.cpp,v 1.5 2006/07/15 18:45:57 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdedriverselectbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlistwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "hk_kdedriverselectbase.moc"
#include <KLocalizedString>

/*
 *  Constructs a hk_kdedriverselectbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdedriverselectbase::hk_kdedriverselectbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdedriverselectbase":name));
    setModal(modal);
    setMinimumSize( QSize( 250, 0 ) );
    hk_kdedriverselectbaseLayout = new QGridLayout(this);
    hk_kdedriverselectbaseLayout->setMargin(11);
    hk_kdedriverselectbaseLayout->setSpacing(6);
    hk_kdedriverselectbaseLayout->setObjectName(QString::fromAscii("hk_kdedriverselectbaseLayout")); 

    connectbutton = new QPushButton(this);
    connectbutton->setObjectName("connectbutton");
    connectbutton->setEnabled( false );

    hk_kdedriverselectbaseLayout->addWidget( connectbutton, 0, 1 );

    cancelbutton = new QPushButton(this);
    cancelbutton->setObjectName("cancelbutton");

    hk_kdedriverselectbaseLayout->addWidget( cancelbutton, 1, 1 );

    tabwidget = new QTabWidget( this );
    tabwidget->setObjectName(QString::fromAscii("tabwidget"));

    tab = new QWidget(tabwidget);
    tab->setObjectName(QString::fromAscii("tab"));
    tabLayout = new QVBoxLayout(tab);
    tabLayout->setMargin(11);
    tabLayout->setSpacing(6);
    tabLayout->setObjectName("tabLayout");

    driverlist = new QListWidget( tab);
    driverlist -> setObjectName( "driverlist" );
    driverlist->setMinimumSize( QSize( 200, 150 ) );
    tabLayout->addWidget( driverlist );
    tabwidget->addTab( tab, QString::fromLatin1("") );

    tab_2 = new QWidget(tabwidget);
    tab_2->setObjectName(QString::fromAscii("tab_2"));
    tabwidget->addTab( tab_2, QString::fromLatin1("") );

    hk_kdedriverselectbaseLayout->addWidget( tabwidget, 0, 0, 5, 1 );

    optionbutton = new QPushButton(this);
    optionbutton->setObjectName("optionbutton");
    optionbutton->setEnabled(true);
    optionbutton->setAutoDefault(true);

    hk_kdedriverselectbaseLayout->addWidget( optionbutton, 2, 1 );

    buttonHelp = new QPushButton(this);
    buttonHelp->setObjectName("buttonHelp");
    buttonHelp->setEnabled(true);
    buttonHelp->setAutoDefault(true);

    hk_kdedriverselectbaseLayout->addWidget( buttonHelp, 3, 1 );
    spacer1 = new QSpacerItem( 20, 340, QSizePolicy::Minimum, QSizePolicy::Expanding );
    hk_kdedriverselectbaseLayout->addItem( spacer1, 4, 1 );
    languageChange();
    resize( QSize(710, 530).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( connectbutton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( cancelbutton, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( driverlist, SIGNAL( itemDoubleClicked(QListWidgetItem*)) , this, SLOT( accept() ) );
    connect( tabwidget, SIGNAL( currentChanged(int) ), this, SLOT( tabwidget_changed() ) );
    connect( buttonHelp, SIGNAL( clicked() ), this, SLOT( help_clicked() ) );
    connect( optionbutton, SIGNAL( clicked() ), this, SLOT( option_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdedriverselectbase::~hk_kdedriverselectbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdedriverselectbase::languageChange()
{
    setWindowTitle(i18n("Driver select dialog"));
    connectbutton->setText(i18n("C&onnect"));
    connectbutton->setShortcut(QKeySequence(i18n("Alt+O")));
    cancelbutton->setText(i18n("&Cancel"));
    cancelbutton->setShortcut(QKeySequence(i18n("Alt+C")));
    tabwidget->setTabText(tabwidget->indexOf(tab), i18n("&Drivers"));
    tabwidget->setTabText(tabwidget->indexOf(tab_2), i18n("&Connections"));
    optionbutton->setText(i18n("O&ptions"));
    optionbutton->setShortcut(QKeySequence(i18n("Alt+P")));
    buttonHelp->setText(i18n("&Help"));
    buttonHelp->setShortcut(QKeySequence(i18n("Alt+H")));
}

void hk_kdedriverselectbase::list_clicked()
{
    qWarning( "hk_kdedriverselectbase::list_clicked(): Not implemented yet" );
}

void hk_kdedriverselectbase::tabwidget_changed()
{
    qWarning( "hk_kdedriverselectbase::tabwidget_changed(): Not implemented yet" );
}

void hk_kdedriverselectbase::help_clicked()
{
    qWarning( "hk_kdedriverselectbase::help_clicked(): Not implemented yet" );
}

void hk_kdedriverselectbase::option_clicked()
{
    qWarning( "hk_kdedriverselectbase::option_clicked(): Not implemented yet" );
}
