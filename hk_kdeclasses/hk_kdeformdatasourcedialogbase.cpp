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
** Form implementation generated from reading ui file 'formdatasourcedialog.ui'
**
** Created: Sa Sep 17 17:17:56 2005
**      by: The User Interface Compiler ($Id: hk_kdeformdatasourcedialogbase.cpp,v 1.22 2005/09/17 19:11:18 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdeformdatasourcedialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qtreewidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qheaderview.h>

/*
 *  Constructs a hk_kdeformdatasourcebasedialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdeformdatasourcebasedialog::hk_kdeformdatasourcebasedialog( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdeformdatasourcebasedialog":name));
    setModal(modal);
    setSizeGripEnabled( TRUE );
    hk_kdeformdatasourcebasedialogLayout = new QGridLayout(this);
    hk_kdeformdatasourcebasedialogLayout->setMargin(3);
    hk_kdeformdatasourcebasedialogLayout->setSpacing(6);
    setObjectName(QString::fromAscii("hk_kdeformdatasourcebasedialogLayout")); 

    Layout8 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout8->setMargin(0);
    Layout8->setSpacing(6);
    Layout8->setObjectName("Layout8");

    Addbutton = new QPushButton(this);
    Addbutton->setObjectName("Addbutton");
    Addbutton->setEnabled( FALSE );
    Addbutton->setAutoDefault( TRUE );
    Addbutton->setDefault( TRUE );
    Layout8->addWidget( Addbutton );

    alterbutton = new QPushButton(this);
    alterbutton->setObjectName("alterbutton");
    alterbutton->setEnabled( FALSE );
    Layout8->addWidget( alterbutton );

    deletebutton = new QPushButton(this);
    deletebutton->setObjectName("deletebutton");
    deletebutton->setEnabled( FALSE );
    Layout8->addWidget( deletebutton );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName("buttonCancel");
    buttonCancel->setAutoDefault( TRUE );
    Layout8->addWidget( buttonCancel );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout8->addItem( Spacer1 );

    hk_kdeformdatasourcebasedialogLayout->addLayout( Layout8, 0, 1 );

    Frame15 = new QFrame(this);
    Frame15->setObjectName(QString::fromAscii("Frame15"));
    Frame15->setFrameShape( QFrame::Box );
    Frame15->setFrameShadow( QFrame::Sunken );
    Frame15Layout = new QGridLayout( Frame15);
    Frame15Layout->setMargin(5);
    Frame15Layout->setSpacing(6);
    Frame15Layout->setObjectName(QString::fromAscii("Frame15Layout")); 

    datasourcefield = new QComboBox(Frame15);
    datasourcefield->setObjectName(QString::fromAscii("datasourcefield"));

    Frame15Layout->addWidget( datasourcefield, 0, 2 );

    sourcetypefield = new QComboBox(Frame15);
    sourcetypefield->setObjectName(QString::fromAscii("sourcetypefield"));

    Frame15Layout->addWidget( sourcetypefield, 0, 1 );

    basedontext = new QLabel(Frame15);
    basedontext->setObjectName("basedontext");

    Frame15Layout->addWidget( basedontext, 0, 0 );

    sqlfield = new QTextEdit(Frame15);
    sqlfield->setObjectName(QString::fromAscii("sqlfield"));

    Frame15Layout->addWidget(sqlfield, 1, 1, 1, 2);
    
    sqltext = new QLabel(Frame15);
    sqltext->setObjectName("sqltext");

    Frame15Layout->addWidget( sqltext, 1, 0 );

    hk_kdeformdatasourcebasedialogLayout->addWidget( Frame15, 0, 0 );

    Frame17 = new QFrame(this);
    Frame17->setObjectName(QString::fromAscii("Frame17"));
    Frame17->setFrameShape( QFrame::Box );
    Frame17->setFrameShadow( QFrame::Sunken );
    Frame17Layout = new QHBoxLayout(Frame17);
    Frame17Layout->setMargin(11);
    Frame17Layout->setSpacing(6);
    Frame17Layout->setObjectName("Frame17Layout");

    filtertext = new QLabel(Frame17);
    filtertext->setObjectName("filtertext");
    Frame17Layout->addWidget( filtertext );

    filterfield = new QTextEdit( Frame17 );
    filterfield->setObjectName("filterfield");
    Frame17Layout->addWidget( filterfield );

    hk_kdeformdatasourcebasedialogLayout->addWidget( Frame17, 3, 0 );

    Frame6 = new QFrame(this);
    Frame6->setObjectName(QString::fromAscii("Frame6"));
    Frame6->setFrameShape( QFrame::Box );
    Frame6->setFrameShadow( QFrame::Sunken );
    Frame6Layout = new QHBoxLayout(Frame6);
    Frame6Layout->setMargin(11);
    Frame6Layout->setSpacing(6);
    Frame6Layout->setObjectName("Frame6Layout");

    sortingtext = new QLabel(Frame6);
    sortingtext->setObjectName("sortingtext");
    Frame6Layout->addWidget( sortingtext );

    sortingfield = new QLineEdit(Frame6);
    sortingfield->setObjectName("sortingfield");
    Frame6Layout->addWidget( sortingfield );

    hk_kdeformdatasourcebasedialogLayout->addWidget( Frame6, 2, 0 );

    Frame3 = new QFrame(this);
    Frame3->setObjectName(QString::fromAscii("Frame3"));
    Frame3->setFrameShape( QFrame::Box );
    Frame3->setFrameShadow( QFrame::Sunken );
    Frame3Layout = new QGridLayout(Frame3);
    Frame3Layout->setMargin(11);
    Frame3Layout->setSpacing(6);
    Frame3Layout->setObjectName(QString::fromAscii("Frame3Layout")); 

    Layout12 = new QGridLayout(static_cast<QWidget*>(NULL));
    Layout12->setMargin(0);
    Layout12->setSpacing(6);
    Layout12->setObjectName(QString::fromAscii("Layout12")); 

    deletefieldbutton = new QToolButton(Frame3);
    deletefieldbutton->setObjectName(QString::fromAscii("deletefieldbutton"));
    deletefieldbutton->setEnabled( FALSE );
    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    policy.setHeightForWidth(deletefieldbutton->sizePolicy().hasHeightForWidth());
    deletefieldbutton->setSizePolicy(policy);
        
    Layout12->addWidget(deletefieldbutton, 1, 2, 2, 1 );
    
    Spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout12->addItem( Spacer2, 3, 2 );

    dependingmodelabel = new QLabel(Frame3);
    dependingmodelabel->setObjectName("dependingmodelabel");

    Layout12->addWidget( dependingmodelabel, 6, 0 );

    addfieldbutton = new QToolButton(Frame3);
    addfieldbutton->setObjectName(QString::fromAscii("addfieldbutton"));
    addfieldbutton->setEnabled( FALSE );
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(addfieldbutton->sizePolicy().hasHeightForWidth());
    addfieldbutton->setSizePolicy(policy);
    
    Layout12->addWidget( addfieldbutton, 0, 2 );

    dependingontext = new QLabel(Frame3);
    dependingontext->setObjectName("dependingontext");

    Layout12->addWidget( dependingontext, 0, 0 );

    thistext = new QLabel(Frame3);
    thistext->setObjectName("thistext");

    Layout12->addWidget( thistext, 3, 0 );
    Spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout12->addItem(Spacer3, 4, 0, 2, 1);
    Spacer2_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout12->addItem( Spacer2_2, 4, 1 );
    Spacer2_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout12->addItem( Spacer2_3, 4, 2 );
    Spacer1_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout12->addItem( Spacer1_2, 6, 2 );

    mastertext = new QLabel(Frame3);
    mastertext->setObjectName("mastertext");

    Layout12->addWidget( mastertext, 1, 0 );

    thisfield = new QComboBox(Frame3);
    thisfield->setObjectName(QString::fromAscii("thisfield"));
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(thisfield->sizePolicy().hasHeightForWidth());    
    thisfield->setSizePolicy(policy);
    thisfield->setEditable( TRUE );
    thisfield->setInsertPolicy( QComboBox::NoInsert );
    thisfield->setAutoCompletion( TRUE );

    Layout12->addWidget(thisfield, 2, 1, 2, 1 );

    masterfield = new QComboBox(Frame3);
    masterfield->setObjectName(QString::fromAscii("masterfield"));
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(masterfield->sizePolicy().hasHeightForWidth());     
    masterfield->setSizePolicy(policy);
    masterfield->setEditable( TRUE );
    masterfield->setInsertPolicy( QComboBox::NoInsert );
    masterfield->setAutoCompletion( TRUE );

    Layout12->addWidget( masterfield, 1, 1 );

    dependingonfield = new QComboBox(Frame3);
    dependingonfield->setObjectName(QString::fromAscii("dependingonfield"));
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(dependingonfield->sizePolicy().hasHeightForWidth());     
    dependingonfield->setSizePolicy(policy);
    dependingonfield->setMinimumSize( QSize( 125, 0 ) );
    dependingonfield->setEditable( TRUE );
    dependingonfield->setInsertPolicy( QComboBox::NoInsert );
    dependingonfield->setAutoCompletion( TRUE );

    Layout12->addWidget( dependingonfield, 0, 1 );

    dependingmodefield = new QComboBox(Frame3);
    dependingmodefield->setObjectName(QString::fromAscii("dependingmodefield"));
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(dependingmodefield->sizePolicy().hasHeightForWidth());     
    dependingmodefield->setSizePolicy(policy);
        
    Layout12->addWidget(dependingmodefield, 5, 1, 2, 1 );

    Frame3Layout->addLayout( Layout12, 0, 0 );

    react_on_changesfield = new QCheckBox(Frame3);
    react_on_changesfield->setObjectName(QString::fromAscii("react_on_changesfield"));

    Frame3Layout->addWidget( react_on_changesfield, 1, 0 );

    readonlyfield = new QCheckBox(Frame3);
    readonlyfield->setObjectName(QString::fromAscii("readonlyfield"));

    Frame3Layout->addWidget( readonlyfield, 2, 0 );

    dependingfieldlist = new QTreeWidget( Frame3);
    dependingfieldlist -> setObjectName( "dependingfieldlist" );
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    policy.setHeightForWidth(dependingfieldlist->sizePolicy().hasHeightForWidth());
    dependingfieldlist->setSizePolicy(policy);
    dependingfieldlist->setMinimumSize( QSize( 200, 0 ) );
    dependingfieldlist->setSelectionMode(QAbstractItemView::SingleSelection);
    dependingfieldlist->setAllColumnsShowFocus( TRUE );
    dependingfieldlist->setRootIsDecorated(false);
    dependingfieldlist->header()->setMovable(false);
    Frame3Layout->addWidget(dependingfieldlist, 0, 1, 4, 1 );

    automaticupdatefield = new QCheckBox(Frame3);
    automaticupdatefield->setObjectName(QString::fromAscii("automaticupdatefield"));

    Frame3Layout->addWidget( automaticupdatefield, 3, 0 );

    hk_kdeformdatasourcebasedialogLayout->addWidget( Frame3, 1, 0 );
    languageChange();
    resize( QSize(681, 668).expandedTo(minimumSizeHint()) );
    

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( addfieldbutton, SIGNAL( clicked() ), this, SLOT( add_dependingfields() ) );
    connect( deletefieldbutton, SIGNAL( clicked() ), this, SLOT( delete_dependingfields() ) );
    connect( dependingfieldlist, SIGNAL( currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
	    this, SLOT( check_buttons() ) );
    connect( Addbutton, SIGNAL( clicked() ), this, SLOT( new_datasource() ) );
    connect( alterbutton, SIGNAL( clicked() ), this, SLOT( alter_datasource() ) );
    connect( deletebutton, SIGNAL( clicked() ), this, SLOT( delete_datasource() ) );
    connect( sourcetypefield, SIGNAL( activated(const QString&) ), this, SLOT( set_datasourcelist() ) );
    connect( datasourcefield, SIGNAL( activated(const QString&) ), this, SLOT( datasource_selected() ) );
    connect( dependingonfield, SIGNAL( activated(const QString&) ), this, SLOT( dependingon_selected() ) );
    connect( dependingonfield, SIGNAL( textChanged(const QString&) ), this, SLOT( check_buttons() ) );
    connect( dependingonfield, SIGNAL( activated(int) ), this, SLOT( check_buttons() ) );
    connect( dependingonfield, SIGNAL( textChanged(const QString&) ), this, SLOT( dependingon_selected() ) );
    connect( masterfield, SIGNAL( textChanged(const QString&) ), this, SLOT( check_buttons() ) );
    connect( masterfield, SIGNAL( activated(int) ), this, SLOT( check_buttons() ) );
    connect( thisfield, SIGNAL( textChanged(const QString&) ), this, SLOT( check_buttons() ) );
    connect( thisfield, SIGNAL( highlighted(int) ), this, SLOT( check_buttons() ) );
    connect( sqlfield, SIGNAL( textChanged() ), this, SLOT( sql_changed() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeformdatasourcebasedialog::~hk_kdeformdatasourcebasedialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdeformdatasourcebasedialog::languageChange()
{
    setWindowTitle( tr( "new formdatasource dialog" ) );
    Addbutton->setText( tr( "&Add" ) );
    alterbutton->setText( tr( "A&lter" ) );
    deletebutton->setText( tr( "&Delete" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    basedontext->setText( tr( "based on:" ) );
    sqltext->setText( tr( "Sql:" ) );
    filtertext->setText( tr( "filter:" ) );
    filterfield->setPlainText( QString::null );
    sortingtext->setText( tr( "sortingfield" ) );
    deletefieldbutton->setText( QString::null );
    dependingmodelabel->setText( tr( "depending mode:" ) );
    addfieldbutton->setText( QString::null );
    dependingontext->setText( tr( "depending on" ) );
    thistext->setText( tr( "thisfield" ) );
    mastertext->setText( tr( "masterfield" ) );
    react_on_changesfield->setText( tr( "react on master changes?" ) );
    readonlyfield->setText( tr( "Is read only?" ) );
    automaticupdatefield->setText( tr( "Automatic data update?" ) );
}

void hk_kdeformdatasourcebasedialog::add_dependingfields()
{
    qWarning( "hk_kdeformdatasourcebasedialog::add_dependingfields(): Not implemented yet" );
}

void hk_kdeformdatasourcebasedialog::alter_datasource()
{
    qWarning( "hk_kdeformdatasourcebasedialog::alter_datasource(): Not implemented yet" );
}

void hk_kdeformdatasourcebasedialog::check_buttons()
{
    qWarning( "hk_kdeformdatasourcebasedialog::check_buttons(): Not implemented yet" );
}

void hk_kdeformdatasourcebasedialog::datasource_selected()
{
    qWarning( "hk_kdeformdatasourcebasedialog::datasource_selected(): Not implemented yet" );
}

void hk_kdeformdatasourcebasedialog::delete_dependingfields()
{
    qWarning( "hk_kdeformdatasourcebasedialog::delete_dependingfields(): Not implemented yet" );
}

void hk_kdeformdatasourcebasedialog::dependingon_selected()
{
    qWarning( "hk_kdeformdatasourcebasedialog::dependingon_selected(): Not implemented yet" );
}

void hk_kdeformdatasourcebasedialog::new_datasource()
{
    qWarning( "hk_kdeformdatasourcebasedialog::new_datasource(): Not implemented yet" );
}

void hk_kdeformdatasourcebasedialog::delete_datasource()
{
    qWarning( "hk_kdeformdatasourcebasedialog::delete_datasource(): Not implemented yet" );
}

void hk_kdeformdatasourcebasedialog::set_datasourcelist()
{
    qWarning( "hk_kdeformdatasourcebasedialog::set_datasourcelist(): Not implemented yet" );
}

void hk_kdeformdatasourcebasedialog::sql_changed()
{
    qWarning( "hk_kdeformdatasourcebasedialog::sql_changed(): Not implemented yet" );
}

