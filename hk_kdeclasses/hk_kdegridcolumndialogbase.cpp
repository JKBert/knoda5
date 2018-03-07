// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
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
** Form implementation generated from reading ui file 'gridcolumnselect2.ui'
**
** Created: Fr Okt 27 08:25:04 2006
**      by: The User Interface Compiler ($Id: hk_kdegridcolumndialogbase.cpp,v 1.16 2006/10/27 06:38:24 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdegridcolumndialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qtreewidget.h>
#include <qlistwidget.h>
#include <qtoolbutton.h>
#include <qgroupbox.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a hk_kdegridcolumndialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdegridcolumndialogbase::hk_kdegridcolumndialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdegridcolumndialogbase":name));
    setModal(modal);
    hk_kdegridcolumndialogbaseLayout = new QGridLayout(this);
    hk_kdegridcolumndialogbaseLayout->setMargin(11);
    hk_kdegridcolumndialogbaseLayout->setSpacing(6);
    hk_kdegridcolumndialogbaseLayout->setObjectName(QString::fromAscii("hk_kdegridcolumndialogbaseLayout")); 

    Layout6 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout6->setMargin(0);
    Layout6->setSpacing(6);
    Layout6->setObjectName("Layout6");

    rightlabel = new QLabel(this);
    rightlabel->setObjectName("rightlabel");
    rightlabel->setFrameShape( QLabel::NoFrame );
    rightlabel->setFrameShadow( QLabel::Plain );
    Layout6->addWidget( rightlabel );

    rightbox = new QTreeWidget( this);
    rightbox -> setObjectName("rightbox" );
    rightbox -> setRootIsDecorated(false);
    QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    policy.setHeightForWidth(rightbox->sizePolicy().hasHeightForWidth());
    rightbox->setSizePolicy(policy);
    rightbox->setMinimumSize( QSize( 200, 75 ) );
    Layout6->addWidget( rightbox );

    hk_kdegridcolumndialogbaseLayout->addLayout( Layout6, 0, 2, 4, 1 );

    Layout4 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout4->setMargin(0);
    Layout4->setSpacing(6);
    Layout4->setObjectName("Layout4");

    leftlabel = new QLabel(this);
    leftlabel->setObjectName("leftlabel");
    Layout4->addWidget( leftlabel );

    leftbox = new QListWidget( this );
    leftbox -> setObjectName("leftbox" );
    policy.setHorizontalPolicy(QSizePolicy::Minimum);
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    policy.setHeightForWidth(leftbox->sizePolicy().hasHeightForWidth());
    leftbox->setSizePolicy(policy);
    leftbox->setMinimumSize( QSize( 150, 75 ) );
    Layout4->addWidget( leftbox );

    hk_kdegridcolumndialogbaseLayout->addLayout( Layout4, 0, 0, 4, 1 );

    Layout7 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout7->setMargin(0);
    Layout7->setSpacing(6);
    Layout7->setObjectName("Layout7");
    Spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout7->addItem( Spacer2 );

    Layout2 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout2->setMargin(0);
    Layout2->setSpacing(6);
    Layout2->setObjectName("Layout2");

    addbutton = new QToolButton(this);
    addbutton->setObjectName(QString::fromAscii("addbutton"));
    Layout2->addWidget( addbutton );

    deletebutton = new QToolButton(this);
    deletebutton->setObjectName(QString::fromAscii("deletebutton"));
    Layout2->addWidget( deletebutton );
    Layout7->addLayout( Layout2 );
    Spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout7->addItem( Spacer3 );

    hk_kdegridcolumndialogbaseLayout->addLayout( Layout7, 0, 1, 4, 1 );
    Spacer5 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    hk_kdegridcolumndialogbaseLayout->addItem(Spacer5, 0, 3, 4, 1);

    definitionlabel = new QLabel(this);
    definitionlabel->setObjectName("definitionlabel");

    hk_kdegridcolumndialogbaseLayout->addWidget( definitionlabel, 0, 4, 1, 2 );

    ButtonGroup7 = new QGroupBox( this );
    ButtonGroup7 -> setObjectName( "ButtonGroup7" );
    policy.setHorizontalPolicy(QSizePolicy::Maximum);
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHeightForWidth(ButtonGroup7->sizePolicy().hasHeightForWidth());
    ButtonGroup7->setSizePolicy(policy);
    ButtonGroup7->setMaximumSize( QSize( 150, 32767 ) );
    ButtonGroup7Layout = new QVBoxLayout( ButtonGroup7 );
    ButtonGroup7Layout->setAlignment( Qt::AlignTop );

    storebutton = new QPushButton(ButtonGroup7);
    storebutton->setObjectName("storebutton");
    storebutton->setEnabled( FALSE );
    ButtonGroup7Layout->addWidget( storebutton );

    exitbutton = new QPushButton(ButtonGroup7);
    exitbutton->setObjectName("exitbutton");
    ButtonGroup7Layout->addWidget( exitbutton );

    hk_kdegridcolumndialogbaseLayout->addWidget( ButtonGroup7, 3, 5 );
    Spacer5_2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    hk_kdegridcolumndialogbaseLayout->addItem( Spacer5_2, 3, 4 );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    hk_kdegridcolumndialogbaseLayout->addItem( Spacer1, 2, 5 );

    TabWidget = new QTabWidget( this );
    TabWidget->setObjectName(QString::fromAscii("TabWidget"));
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    policy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    policy.setHeightForWidth(TabWidget->sizePolicy().hasHeightForWidth());
    TabWidget->setSizePolicy(policy);

    formattab = new QWidget(TabWidget);
    formattab->setObjectName(QString::fromAscii("formattab"));
    formattabLayout = new QGridLayout(formattab);
    formattabLayout->setMargin(11);
    formattabLayout->setSpacing(6);
    formattabLayout->setObjectName(QString::fromAscii("formattabLayout")); 

    displayfield = new QLineEdit(formattab);
    displayfield->setObjectName("displayfield");

    formattabLayout->addWidget( displayfield, 0, 1 );

    displaylabel = new QLabel(formattab);
    displaylabel->setObjectName("displaylabel");

    formattabLayout->addWidget( displaylabel, 0, 0 );
    Spacer22 = new QSpacerItem( 20, 270, QSizePolicy::Minimum, QSizePolicy::Expanding );
    formattabLayout->addItem( Spacer22, 8, 0 );
    Spacer23 = new QSpacerItem( 20, 270, QSizePolicy::Minimum, QSizePolicy::Expanding );
    formattabLayout->addItem( Spacer23, 8, 1 );

    columntypelabel = new QLabel(formattab);
    columntypelabel->setObjectName("columntypelabel");

    formattabLayout->addWidget( columntypelabel, 3, 0 );

    digitlabel = new QLabel(formattab);
    digitlabel->setObjectName("digitlabel");
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHeightForWidth(digitlabel->sizePolicy().hasHeightForWidth());    
    digitlabel->setSizePolicy(policy);

    formattabLayout->addWidget( digitlabel, 6, 0 );

    alignmentlabel = new QLabel(formattab);
    alignmentlabel->setObjectName("alignmentlabel");
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHeightForWidth(alignmentlabel->sizePolicy().hasHeightForWidth());
    alignmentlabel->setSizePolicy(policy);
    
    formattabLayout->addWidget( alignmentlabel, 4, 0 );

    alignmentfield = new QComboBox(formattab);
    alignmentfield->setObjectName(QString::fromAscii("alignmentfield"));
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(alignmentfield->sizePolicy().hasHeightForWidth());    
    alignmentfield->setSizePolicy(policy);

    formattabLayout->addWidget( alignmentfield, 4, 1 );

    columntypefield = new QComboBox(formattab);
    columntypefield->setObjectName(QString::fromAscii("columntypefield"));

    formattabLayout->addWidget( columntypefield, 3, 1 );

    separatorfield = new QComboBox(formattab);
    separatorfield->setObjectName(QString::fromAscii("separatorfield"));
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(separatorfield->sizePolicy().hasHeightForWidth()); 
    separatorfield->setSizePolicy(policy);
    
    formattabLayout->addWidget( separatorfield, 5, 1 );

    separatorlabel = new QLabel(formattab);
    separatorlabel->setObjectName("separatorlabel");
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHeightForWidth(separatorlabel->sizePolicy().hasHeightForWidth()); 
    separatorlabel->setSizePolicy(policy);
        
    formattabLayout->addWidget( separatorlabel, 5, 0 );

    readonlyfield = new QComboBox(formattab);
    readonlyfield->setObjectName(QString::fromAscii("readonlyfield"));

    formattabLayout->addWidget( readonlyfield, 7, 1 );

    digitfield = new QSpinBox( formattab );
    digitfield->setObjectName(QString::fromAscii("digitfield"));
    policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(digitfield->sizePolicy().hasHeightForWidth()); 
    digitfield->setSizePolicy(policy);
    digitfield->setMaximum( 15 );
    digitfield->setMinimum( -1 );

    formattabLayout->addWidget( digitfield, 6, 1 );

    readonlylabel = new QLabel(formattab);
    readonlylabel->setObjectName("readonlylabel");

    formattabLayout->addWidget( readonlylabel, 7, 0 );

    Defaultlabel = new QLabel(formattab);
    Defaultlabel->setObjectName("Defaultlabel");

    formattabLayout->addWidget( Defaultlabel, 1, 0 );

    defaultfield = new QLineEdit(formattab);
    defaultfield->setObjectName("defaultfield");
    policy.setHorizontalPolicy(QSizePolicy::Minimum);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(defaultfield->sizePolicy().hasHeightForWidth());
    defaultfield->setSizePolicy(policy);
    defaultfield->setMinimumSize( QSize( 150, 0 ) );

    formattabLayout->addWidget( defaultfield, 1, 1 );

    identifierfield = new QLineEdit(formattab);
    identifierfield->setObjectName("identifierfield");
    policy.setHorizontalPolicy(QSizePolicy::Minimum);
    policy.setVerticalPolicy(QSizePolicy::Fixed);
    policy.setHeightForWidth(identifierfield->sizePolicy().hasHeightForWidth());    
    identifierfield->setSizePolicy(policy);
    identifierfield->setMinimumSize( QSize( 150, 0 ) );

    formattabLayout->addWidget( identifierfield, 2, 1 );

    identifierlabel = new QLabel(formattab);
    identifierlabel->setObjectName("identifierlabel");

    formattabLayout->addWidget( identifierlabel, 2, 0 );
    TabWidget->addTab( formattab, QString::fromLatin1("") );

    combotab = new QWidget(TabWidget);
    combotab->setObjectName(QString::fromAscii("combotab"));
    combotabLayout = new QGridLayout(combotab);
    combotabLayout->setMargin(11);
    combotabLayout->setSpacing(6);
    combotabLayout->setObjectName(QString::fromAscii("combotabLayout")); 

    viewcolumnlabel = new QLabel(combotab);
    viewcolumnlabel->setObjectName("viewcolumnlabel");

    combotabLayout->addWidget( viewcolumnlabel, 3, 0 );

    viewcolumnfield = new QComboBox(combotab);
    viewcolumnfield->setObjectName(QString::fromAscii("viewcolumnfield"));
    viewcolumnfield->setEditable( TRUE );
    viewcolumnfield->setAutoCompletion( TRUE );
    viewcolumnfield->setMinimumSize( QSize( 150, 0 ) );

    combotabLayout->addWidget( viewcolumnfield, 3, 1 );

    listdatasourcetypelabel = new QLabel(combotab);
    listdatasourcetypelabel->setObjectName("listdatasourcetypelabel");

    combotabLayout->addWidget( listdatasourcetypelabel, 0, 0 );

    listcolumnlabel = new QLabel(combotab);
    listcolumnlabel->setObjectName("listcolumnlabel");

    combotabLayout->addWidget( listcolumnlabel, 2, 0 );

    listcolumnfield = new QComboBox(combotab);
    listcolumnfield->setObjectName(QString::fromAscii("listcolumnfield"));
    listcolumnfield->setEditable( TRUE );
    listcolumnfield->setAutoCompletion( TRUE );

    combotabLayout->addWidget( listcolumnfield, 2, 1 );

    listdatasourcetypefield = new QComboBox(combotab);
    listdatasourcetypefield->setObjectName(QString::fromAscii("listdatasourcetypefield"));

    combotabLayout->addWidget( listdatasourcetypefield, 0, 1 );

    listdatasourcelabel = new QLabel(combotab);
    listdatasourcelabel->setObjectName("listdatasourcelabel");

    combotabLayout->addWidget( listdatasourcelabel, 1, 0 );

    listdatasourcefield = new QComboBox(combotab);
    listdatasourcefield->setObjectName(QString::fromAscii("listdatasourcefield"));

    combotabLayout->addWidget( listdatasourcefield, 1, 1 );
    Spacer20 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    combotabLayout->addItem( Spacer20, 4, 0 );
    Spacer21 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    combotabLayout->addItem( Spacer21, 4, 1 );
    TabWidget->addTab( combotab, QString::fromLatin1("") );

    actiontab = new QWidget(TabWidget);
    actiontab->setObjectName(QString::fromAscii("actiontab"));
    actiontabLayout = new QGridLayout(actiontab);
    actiontabLayout->setMargin(11);
    actiontabLayout->setSpacing(6);
    actiontabLayout->setObjectName(QString::fromAscii("actiontabLayout")); 

    pushactionlabel = new QLabel(actiontab);
    pushactionlabel->setObjectName("pushactionlabel");

    actiontabLayout->addWidget( pushactionlabel, 0, 0 );

    onpushactionbutton = new QPushButton(actiontab);
    onpushactionbutton->setObjectName("onpushactionbutton");

    actiontabLayout->addWidget( onpushactionbutton, 0, 1 );

    ondoubleclickactionbutton = new QPushButton(actiontab);
    ondoubleclickactionbutton->setObjectName("ondoubleclickactionbutton");

    actiontabLayout->addWidget( ondoubleclickactionbutton, 1, 1 );

    doubleclickactionlabel = new QLabel(actiontab);
    doubleclickactionlabel->setObjectName("doubleclickactionlabel");

    actiontabLayout->addWidget( doubleclickactionlabel, 1, 0 );

    onloosefocuslabel = new QLabel(actiontab);
    onloosefocuslabel->setObjectName("onloosefocuslabel");

    actiontabLayout->addWidget( onloosefocuslabel, 3, 0 );

    ongetfocusbutton = new QPushButton(actiontab);
    ongetfocusbutton->setObjectName("ongetfocusbutton");

    actiontabLayout->addWidget( ongetfocusbutton, 2, 1 );

    onloosefocusbutton = new QPushButton(actiontab);
    onloosefocusbutton->setObjectName("onloosefocusbutton");

    actiontabLayout->addWidget( onloosefocusbutton, 3, 1 );

    ongetfocuslabel = new QLabel(actiontab);
    ongetfocuslabel->setObjectName("ongetfocuslabel");

    actiontabLayout->addWidget( ongetfocuslabel, 2, 0 );

    beforeupdateactionbutton = new QPushButton(actiontab);
    beforeupdateactionbutton->setObjectName("beforeupdateactionbutton");

    actiontabLayout->addWidget( beforeupdateactionbutton, 7, 1 );

    afterrowchangeactionbutton = new QPushButton(actiontab);
    afterrowchangeactionbutton->setObjectName("afterrowchangeactionbutton");

    actiontabLayout->addWidget( afterrowchangeactionbutton, 10, 1 );

    beforeinsertlabel = new QLabel(actiontab);
    beforeinsertlabel->setObjectName("beforeinsertlabel");

    actiontabLayout->addWidget( beforeinsertlabel, 13, 0 );

    afterdeleteactionbutton = new QPushButton(actiontab);
    afterdeleteactionbutton->setObjectName("afterdeleteactionbutton");

    actiontabLayout->addWidget( afterdeleteactionbutton, 12, 1 );

    beforeupdateactionlabel = new QLabel(actiontab);
    beforeupdateactionlabel->setObjectName("beforeupdateactionlabel");

    actiontabLayout->addWidget( beforeupdateactionlabel, 7, 0 );

    openactionlabel = new QLabel(actiontab);
    openactionlabel->setObjectName("openactionlabel");

    actiontabLayout->addWidget( openactionlabel, 5, 0 );

    afterupdateactionbutton = new QPushButton(actiontab);
    afterupdateactionbutton->setObjectName("afterupdateactionbutton");

    actiontabLayout->addWidget( afterupdateactionbutton, 8, 1 );

    beforeinsertactionbutton = new QPushButton(actiontab);
    beforeinsertactionbutton->setObjectName("beforeinsertactionbutton");

    actiontabLayout->addWidget( beforeinsertactionbutton, 13, 1 );

    beforedeletelabel = new QLabel(actiontab);
    beforedeletelabel->setObjectName("beforedeletelabel");

    actiontabLayout->addWidget( beforedeletelabel, 11, 0 );

    afterupdatelabel = new QLabel(actiontab);
    afterupdatelabel->setObjectName("afterupdatelabel");

    actiontabLayout->addWidget( afterupdatelabel, 8, 0 );

    afterdeletelabel = new QLabel(actiontab);
    afterdeletelabel->setObjectName("afterdeletelabel");

    actiontabLayout->addWidget( afterdeletelabel, 12, 0 );

    beforerowchangelabel = new QLabel(actiontab);
    beforerowchangelabel->setObjectName("beforerowchangelabel");

    actiontabLayout->addWidget( beforerowchangelabel, 9, 0 );

    closeactionlabel = new QLabel(actiontab);
    closeactionlabel->setObjectName("closeactionlabel");

    actiontabLayout->addWidget( closeactionlabel, 6, 0 );

    oncloseactionbutton = new QPushButton(actiontab);
    oncloseactionbutton->setObjectName("oncloseactionbutton");

    actiontabLayout->addWidget( oncloseactionbutton, 6, 1 );

    onopenactionbutton = new QPushButton(actiontab);
    onopenactionbutton->setObjectName("onopenactionbutton");

    actiontabLayout->addWidget( onopenactionbutton, 5, 1 );

    beforerowchangeactionbutton = new QPushButton(actiontab);
    beforerowchangeactionbutton->setObjectName("beforerowchangeactionbutton");

    actiontabLayout->addWidget( beforerowchangeactionbutton, 9, 1 );

    beforedeleteactionbutton = new QPushButton(actiontab);
    beforedeleteactionbutton->setObjectName("beforedeleteactionbutton");

    actiontabLayout->addWidget( beforedeleteactionbutton, 11, 1 );

    afterrowchangelabel = new QLabel(actiontab);
    afterrowchangelabel->setObjectName("afterrowchangelabel");

    actiontabLayout->addWidget( afterrowchangelabel, 10, 0 );

    onkeybutton = new QPushButton(actiontab);
    onkeybutton->setObjectName("onkeybutton");

    actiontabLayout->addWidget( onkeybutton, 4, 1 );

    onkeylabel = new QLabel(actiontab);
    onkeylabel->setObjectName("onkeylabel");

    actiontabLayout->addWidget( onkeylabel, 4, 0 );


    afterinsertlabel = new QLabel(actiontab);
    afterinsertlabel->setObjectName("afterinsertlabel");

    actiontabLayout->addWidget( afterinsertlabel, 14, 0 );

    afterinsertactionbutton = new QPushButton(actiontab);
    afterinsertactionbutton->setObjectName("afterinsertactionbutton");

    actiontabLayout->addWidget( afterinsertactionbutton, 14, 1 );

    onselectlabel = new QLabel(actiontab);
    onselectlabel->setObjectName("onselectlabel");

    actiontabLayout->addWidget( onselectlabel, 15, 0 );

    onselectactionbutton = new QPushButton(actiontab);
    onselectactionbutton->setObjectName("onselectactionbutton");

    spacer23 = new QSpacerItem( 30, 1, QSizePolicy::Expanding, QSizePolicy::Minimum );
    actiontabLayout->addItem( spacer23, 16, 0 );    
    Spacer6 = new QSpacerItem( 20, 1, QSizePolicy::Minimum, QSizePolicy::Minimum );
    actiontabLayout->addItem( Spacer6, 16, 1 );

    actiontabLayout->addWidget( onselectactionbutton, 15, 1 );
    TabWidget->addTab(actiontab, QString::fromLatin1("") );

    hk_kdegridcolumndialogbaseLayout->addWidget( TabWidget, 1, 4, 1, 2 );
    languageChange();
    resize( QSize(846, 100).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( addbutton, SIGNAL( clicked() ), this, SLOT( addbutton_clicked() ) );
    connect( deletebutton, SIGNAL( clicked() ), this, SLOT( deletebutton_clicked() ) );
    connect( storebutton, SIGNAL( clicked() ), this, SLOT( okbutton_clicked() ) );
    connect( defaultfield, SIGNAL( textChanged(const QString&) ), this, SLOT( defaultvalue_changed() ) );
    connect( rightbox, SIGNAL( itemClicked(QTreeWidgetItem*,int)) , this, SLOT( rightbox_selectionchanged(QTreeWidgetItem*)));
    connect( rightbox, SIGNAL( currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)) , this, SLOT( rightbox_selectionchanged(QTreeWidgetItem*)));
    connect( columntypefield, SIGNAL( activated(int) ), this, SLOT( columntype_changed() ) );
    connect( exitbutton, SIGNAL( clicked() ), this, SLOT( exitbutton_clicked() ) );
    connect( displayfield, SIGNAL( textChanged(const QString&) ), this, SLOT( displayname_changed() ) );
    connect( readonlyfield, SIGNAL( activated(int) ), this, SLOT( readonly_changed() ) );
    connect( alignmentfield, SIGNAL( activated(int) ), this, SLOT( alignment_changed() ) );
    connect( separatorfield, SIGNAL( activated(int) ), this, SLOT( separator_changed() ) );
    connect( digitfield, SIGNAL( valueChanged(int) ), this, SLOT( digits_changed() ) );
    connect( listdatasourcefield, SIGNAL( activated(int) ), this, SLOT( listdatasource_changed() ) );
    connect( listdatasourcetypefield, SIGNAL( activated(int) ), this, SLOT( listdatasourcetype_changed() ) );
    connect( listcolumnfield, SIGNAL( activated(int) ), this, SLOT( listcolumn_changed() ) );
    connect( listcolumnfield, SIGNAL( textChanged(const QString&) ), this, SLOT( listcolumn_changed() ) );
    connect( viewcolumnfield, SIGNAL( activated(int) ), this, SLOT( viewcolumn_changed() ) );
    connect( viewcolumnfield, SIGNAL( textChanged(const QString&) ), this, SLOT( viewcolumn_changed() ) );
    connect( afterdeleteactionbutton, SIGNAL( clicked() ), this, SLOT( afterdeleteaction_clicked() ) );
    connect( onpushactionbutton, SIGNAL( clicked() ), this, SLOT( onclickaction_clicked() ) );
    connect( ondoubleclickactionbutton, SIGNAL( clicked() ), this, SLOT( ondoubleclickaction_clicked() ) );
    connect( beforedeleteactionbutton, SIGNAL( clicked() ), this, SLOT( beforedeleteaction_clicked() ) );
    connect( afterinsertactionbutton, SIGNAL( clicked() ), this, SLOT( afterinsertaction_clicked() ) );
    connect( beforeinsertactionbutton, SIGNAL( clicked() ), this, SLOT( beforeinsertaction_clicked() ) );
    connect( oncloseactionbutton, SIGNAL( clicked() ), this, SLOT( oncloseaction_clicked() ) );
    connect( onopenactionbutton, SIGNAL( clicked() ), this, SLOT( onopenaction_clicked() ) );
    connect( beforerowchangeactionbutton, SIGNAL( clicked() ), this, SLOT( beforerowchangeaction_changed() ) );
    connect( afterrowchangeactionbutton, SIGNAL( clicked() ), this, SLOT( afterrowchangeaction_changed() ) );
    connect( beforeupdateactionbutton, SIGNAL( clicked() ), this, SLOT( beforeupdateaction_clicked() ) );
    connect( afterupdateactionbutton, SIGNAL( clicked() ), this, SLOT( afterupdateaction_clicked() ) );
    connect( ongetfocusbutton, SIGNAL( clicked() ), this, SLOT( on_getfocusaction_clicked() ) );
    connect( onloosefocusbutton, SIGNAL( clicked() ), this, SLOT( on_loosefocusaction_clicked() ) );
    connect( onkeybutton, SIGNAL( clicked() ), this, SLOT( onkeybutton_clicked() ) );
    connect( onselectactionbutton, SIGNAL( clicked() ), this, SLOT( onselectaction_clicked() ) );
    connect( identifierfield, SIGNAL( textChanged(const QString&) ), this, SLOT( identifier_changed() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdegridcolumndialogbase::~hk_kdegridcolumndialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdegridcolumndialogbase::languageChange()
{
    setWindowTitle( tr( "Column Select" ) );
    rightlabel->setText( tr( "Visible columns:" ) );
    leftlabel->setText( tr( "Invisible columns:" ) );
    addbutton->setText( QString::null );
    deletebutton->setText( QString::null );
    definitionlabel->setText( tr( "Columndefinition:" ) );
    ButtonGroup7->setTitle( QString::null );
    storebutton->setText( tr( "&OK" ) );
    exitbutton->setText( tr( "&Cancel" ) );
    displaylabel->setText( tr( "Displayname:" ) );
    columntypelabel->setText( tr( "Columntype:" ) );
    digitlabel->setText( tr( "Digits:" ) );
    alignmentlabel->setText( tr( "Alignment:" ) );
    separatorlabel->setText( tr( "Separators:" ) );
    readonlylabel->setText( tr( "Readonly:" ) );
    Defaultlabel->setText( tr( "Default value:" ) );
    identifierlabel->setText( tr( "Identifier:" ) );
    TabWidget->setTabText( TabWidget->indexOf(formattab), tr( "&Format" ) );
    viewcolumnlabel->setText( tr( "Viewcolumn:" ) );
    listdatasourcetypelabel->setText( tr( "Listdatasourcetype:" ) );
    listcolumnlabel->setText( tr( "Listcolumn:" ) );
    listdatasourcelabel->setText( tr( "Listdatasource:" ) );
    TabWidget->setTabText( TabWidget->indexOf(combotab), tr( "&Combobox" ) );
    pushactionlabel->setText( tr( "On click:" ) );
    onpushactionbutton->setText( QString::null );
    ondoubleclickactionbutton->setText( QString::null );
    doubleclickactionlabel->setText( tr( "On doubleclick:" ) );
    onloosefocuslabel->setText( tr( "On loosefocus:" ) );
    ongetfocusbutton->setText( QString::null );
    onloosefocusbutton->setText( QString::null );
    ongetfocuslabel->setText( tr( "On getfocus:" ) );
    beforeupdateactionbutton->setText( QString::null );
    afterrowchangeactionbutton->setText( QString::null );
    beforeinsertlabel->setText( tr( "Before insert:" ) );
    afterdeleteactionbutton->setText( QString::null );
    beforeupdateactionlabel->setText( tr( "Before update:" ) );
    openactionlabel->setText( tr( "On open:" ) );
    afterupdateactionbutton->setText( QString::null );
    beforeinsertactionbutton->setText( QString::null );
    beforedeletelabel->setText( tr( "Before delete:" ) );
    afterupdatelabel->setText( tr( "After update:" ) );
    afterdeletelabel->setText( tr( "After delete:" ) );
    beforerowchangelabel->setText( tr( "Before row change:" ) );
    closeactionlabel->setText( tr( "On close:" ) );
    oncloseactionbutton->setText( QString::null );
    onopenactionbutton->setText( QString::null );
    beforerowchangeactionbutton->setText( QString::null );
    beforedeleteactionbutton->setText( QString::null );
    afterrowchangelabel->setText( tr( "After row change:" ) );
    onkeybutton->setText( QString::null );
    onkeylabel->setText( tr( "On key:" ) );
    afterinsertlabel->setText( tr( "After insert:" ) );
    afterinsertactionbutton->setText( QString::null );
    onselectlabel->setText( tr( "On select:" ) );
    onselectactionbutton->setText( QString::null );
    TabWidget->setTabText( TabWidget->indexOf(actiontab), tr( "&Action" ) );
}

void hk_kdegridcolumndialogbase::addbutton_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::addbutton_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::alignment_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::alignment_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::columntype_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::columntype_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::defaultvalue_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::defaultvalue_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::deletebutton_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::deletebutton_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::digits_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::digits_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::displayname_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::displayname_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::exitbutton_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::exitbutton_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::listcolumn_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::listcolumn_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::listdatasource_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::listdatasource_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::listdatasourcetype_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::listdatasourcetype_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::okbutton_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::okbutton_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::readonly_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::readonly_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::rightbox_selectionchanged(QTreeWidgetItem*)
{
    qWarning( "hk_kdegridcolumndialogbase::rightbox_selectionchanged(QTreeWidgetItem*): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::separator_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::separator_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::viewcolumn_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::viewcolumn_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::onclickaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::onclickaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::ondoubleclickaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::ondoubleclickaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::onopenaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::onopenaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::oncloseaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::oncloseaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::beforeupdateaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::beforeupdateaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::afterupdateaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::afterupdateaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::beforerowchangeaction_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::beforerowchangeaction_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::afterrowchangeaction_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::afterrowchangeaction_changed(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::beforedeleteaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::beforedeleteaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::afterdeleteaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::afterdeleteaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::beforeinsertaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::beforeinsertaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::afterinsertaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::afterinsertaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::on_getfocusaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::on_getfocusaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::on_loosefocusaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::on_loosefocusaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::onkeybutton_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::onkeybutton_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::onselectaction_clicked()
{
    qWarning( "hk_kdegridcolumndialogbase::onselectaction_clicked(): Not implemented yet" );
}

void hk_kdegridcolumndialogbase::identifier_changed()
{
    qWarning( "hk_kdegridcolumndialogbase::identifier_changed(): Not implemented yet" );
}

