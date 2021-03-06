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

#include "hk_kdereportconditiondialogbase.h"
#include <KLocalizedString> 
#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qtreewidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qheaderview.h>

void inline initSizePolicy( QWidget* w, QSizePolicy::Policy hp, QSizePolicy::Policy vp)
{
    QSizePolicy policy(hp,vp);
    
    policy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
    w->setSizePolicy(policy);
}

/*
 *  Constructs a hk_kdereportconditiondialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdereportconditiondialogbase::hk_kdereportconditiondialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdereportconditiondialogbase":name));
    setModal(modal);
    setSizeGripEnabled( true );
    hk_kdereportconditiondialogbaseLayout = new QGridLayout(this);
    hk_kdereportconditiondialogbaseLayout->setMargin(11);
    hk_kdereportconditiondialogbaseLayout->setSpacing(6);
    hk_kdereportconditiondialogbaseLayout->setObjectName(QString::fromAscii("hk_kdereportconditiondialogbaseLayout")); 

    Layout5 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout5->setMargin(0);
    Layout5->setSpacing(6);
    Layout5->setObjectName("Layout5");

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName("buttonOk");
    buttonOk->setAutoDefault( true );
    buttonOk->setDefault( true );
    Layout5->addWidget( buttonOk );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName("buttonCancel");
    buttonCancel->setAutoDefault( true );
    Layout5->addWidget( buttonCancel );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout5->addItem( Spacer1 );

    hk_kdereportconditiondialogbaseLayout->addLayout( Layout5, 0, 1 );

    GroupBox2 = new QGroupBox(this);
    GroupBox2->setObjectName(QString::fromAscii("GroupBox2"));
    GroupBox2Layout = new QGridLayout( );
    GroupBox2Layout->setSpacing( 6 );
    GroupBox2Layout->setMargin( 11 );   
    GroupBox2Layout->setAlignment( Qt::AlignTop );
    GroupBox2->setLayout(GroupBox2Layout);

    conditionlist = new QTreeWidget( GroupBox2 );
    conditionlist->setObjectName("conditionlist" );
    conditionlist->setMinimumSize( QSize( 300, 200 ) );
    conditionlist->setAllColumnsShowFocus( true );
    conditionlist->setRootIsDecorated(false);
    conditionlist->header()->setStretchLastSection(false);
    conditionlist->header()->setMovable(false);

    GroupBox2Layout->addWidget( conditionlist, 0, 1 );

    Layout17 = new QGridLayout();
    Layout17->setMargin(0);
    Layout17->setSpacing(6);
    Layout17->setObjectName(QString::fromAscii("Layout17")); 

    conditionlabel = new QLabel(GroupBox2);
    conditionlabel->setObjectName("conditionlabel");

    Layout17->addWidget( conditionlabel, 2, 0 );
    Spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout17->addItem(Spacer2, 2, 1, 5, 1);

    addbutton = new QPushButton(GroupBox2);
    addbutton->setObjectName("addbutton");
    initSizePolicy(addbutton, QSizePolicy::Fixed, QSizePolicy::Fixed);

    Layout17->addWidget( addbutton, 0, 1 );
    Spacer5_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout17->addItem( Spacer5_2, 8, 0 );

    formdatasourcefield = new QComboBox(GroupBox2);
    formdatasourcefield->setObjectName(QString::fromAscii("formdatasourcefield"));
    initSizePolicy(formdatasourcefield, QSizePolicy::Expanding, QSizePolicy::Fixed);

    Layout17->addWidget( formdatasourcefield, 5, 0 );

    conditionfield = new QComboBox(GroupBox2);
    conditionfield->setObjectName(QString::fromAscii("conditionfield"));
    initSizePolicy(conditionfield, QSizePolicy::Expanding, QSizePolicy::Fixed);

    Layout17->addWidget( conditionfield, 3, 0 );

    reportlabel = new QLabel(GroupBox2);
    reportlabel->setObjectName("reportlabel");

    Layout17->addWidget( reportlabel, 0, 0 );

    reportfield = new QComboBox(GroupBox2);
    reportfield->setObjectName(QString::fromAscii("reportfield"));
    initSizePolicy(reportfield, QSizePolicy::Expanding, QSizePolicy::Fixed);
    reportfield->setMinimumSize( QSize( 150, 0 ) );
    reportfield->setEditable( true );

    Layout17->addWidget( reportfield, 1, 0 );

    formdatasourcelabel = new QLabel(GroupBox2);
    formdatasourcelabel->setObjectName("formdatasourcelabel");

    Layout17->addWidget( formdatasourcelabel, 4, 0 );

    formfieldlabel = new QLabel(GroupBox2);
    formfieldlabel->setObjectName("formfieldlabel");

    Layout17->addWidget( formfieldlabel, 6, 0 );

    deletebutton = new QPushButton(GroupBox2);
    deletebutton->setObjectName("deletebutton");
    initSizePolicy(deletebutton, QSizePolicy::Fixed, QSizePolicy::Fixed);

    Layout17->addWidget( deletebutton, 1, 1 );

    formfieldfield = new QComboBox(GroupBox2);
    formfieldfield->setObjectName(QString::fromAscii("formfieldfield"));
    initSizePolicy(formfieldfield, QSizePolicy::Expanding, QSizePolicy::Fixed);
    formfieldfield->setEditable( true );

    Layout17->addWidget( formfieldfield, 7, 0 );
    Spacer23 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout17->addItem( Spacer23, 7, 1 );

    GroupBox2Layout->addLayout( Layout17, 0, 0 );

    hk_kdereportconditiondialogbaseLayout->addWidget( GroupBox2, 0, 0 );
    languageChange();
    resize( QSize(200, 342).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( addbutton, SIGNAL( pressed() ), this, SLOT( add_clicked() ) );
    connect( deletebutton, SIGNAL( pressed() ), this, SLOT( delete_clicked() ) );
    connect( formdatasourcefield, SIGNAL( activated(int) ), this, SLOT( formdatasource_changed() ) );
    connect( reportfield, SIGNAL( activated(int) ), this, SLOT( reportfield_changed() ) );
    connect( formfieldfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( formfield_change() ) );
    connect( formfieldfield, SIGNAL( activated(int) ), this, SLOT( formfield_change() ) );
    connect( conditionlist, SIGNAL( itemSelectionChanged()) , this, SLOT( check_buttons() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdereportconditiondialogbase::~hk_kdereportconditiondialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdereportconditiondialogbase::languageChange()
{
    setWindowTitle( i18n( "hk_kdereportconditiondialogbase" ) );
    buttonOk->setText( i18n( "&OK" ) );
    buttonOk->setShortcut( QKeySequence( i18n( "Alt+O" ) ) );
    buttonCancel->setText( i18n( "&Cancel" ) );
    buttonCancel->setShortcut( QKeySequence( i18n( "Alt+C" ) ) );
    GroupBox2->setTitle( QString::null );
    conditionlabel->setText( i18n( "Condition:" ) );
    addbutton->setText( QString::null );
    conditionfield->clear();
    conditionfield->addItem( i18n( "=" ) );
    conditionfield->addItem( i18n( "<" ) );
    conditionfield->addItem( i18n( "<=" ) );
    conditionfield->addItem( i18n( ">" ) );
    conditionfield->addItem( i18n( ">=" ) );
    conditionfield->addItem( i18n( "<>" ) );
    reportlabel->setText( i18n( "Reportfield:" ) );
    formdatasourcelabel->setText( i18n( "Formdatasource:" ) );
    formfieldlabel->setText( i18n( "Formfield:" ) );
    deletebutton->setText( QString::null );
}

void hk_kdereportconditiondialogbase::add_clicked()
{
    qWarning( "hk_kdereportconditiondialogbase::add_clicked(): Not implemented yet" );
}

void hk_kdereportconditiondialogbase::delete_clicked()
{
    qWarning( "hk_kdereportconditiondialogbase::delete_clicked(): Not implemented yet" );
}

void hk_kdereportconditiondialogbase::formdatasource_changed()
{
    qWarning( "hk_kdereportconditiondialogbase::formdatasource_changed(): Not implemented yet" );
}

void hk_kdereportconditiondialogbase::formfield_change()
{
    qWarning( "hk_kdereportconditiondialogbase::formfield_change(): Not implemented yet" );
}

void hk_kdereportconditiondialogbase::check_buttons()
{
    qWarning( "hk_kdereportconditiondialogbase::check_buttons(): Not implemented yet" );
}

void hk_kdereportconditiondialogbase::reportfield_changed()
{
    qWarning( "hk_kdereportconditiondialogbase::reportfield_changed(): Not implemented yet" );
}

