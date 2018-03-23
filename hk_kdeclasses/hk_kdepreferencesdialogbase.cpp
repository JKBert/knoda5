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
** Form implementation generated from reading ui file 'preferencesdialog.ui'
**
** Created: Fr Jan 6 16:47:40 2006
**      by: The User Interface Compiler ($Id: hk_kdepreferencesdialogbase.cpp,v 1.10 2006/01/23 19:07:16 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdepreferencesdialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

void inline initSizePolicy(QWidget* w, QSizePolicy::Policy hp, QSizePolicy::Policy vp)
{
    QSizePolicy policy(hp,vp);
    
    policy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
    w->setSizePolicy(policy);
}

/*
 *  Constructs a hk_kdepreferencesdialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdepreferencesdialogbase::hk_kdepreferencesdialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdepreferencesdialogbase":name));
    setModal(modal);
    initSizePolicy(this, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMinimumSize( QSize( 400, 200 ) );
    setSizeGripEnabled( true );
    hk_kdepreferencesdialogbaseLayout = new QVBoxLayout(this);
    hk_kdepreferencesdialogbaseLayout->setMargin(11);
    hk_kdepreferencesdialogbaseLayout->setSpacing(6);
    hk_kdepreferencesdialogbaseLayout->setObjectName("hk_kdepreferencesdialogbaseLayout");

    regionaltab = new QTabWidget( this );
    regionaltab->setObjectName(QString::fromAscii("regionaltab"));
    initSizePolicy(regionaltab, QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    regionaltab->setMinimumSize( QSize( 400, 0 ) );

    generalpage = new QWidget(regionaltab);
    generalpage->setObjectName(QString::fromAscii("generalpage"));
    generalpageLayout = new QGridLayout(generalpage);
    generalpageLayout->setMargin(11);
    generalpageLayout->setSpacing(6);
    generalpageLayout->setObjectName(QString::fromAscii("generalpageLayout")); 

    driverpathlabel = new QLabel(generalpage);
    driverpathlabel->setObjectName("driverpathlabel");
    initSizePolicy(driverpathlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    generalpageLayout->addWidget( driverpathlabel, 0, 0 );

    driverpathbutton = new QPushButton(generalpage);
    driverpathbutton->setObjectName("driverpathbutton");
    initSizePolicy(driverpathbutton, QSizePolicy::Fixed, QSizePolicy::Fixed);
    driverpathbutton->setMinimumSize( QSize( 0, 0 ) );
    driverpathbutton->setMaximumSize( QSize( 30, 32767 ) );

    generalpageLayout->addWidget( driverpathbutton, 0, 2 );

    driverpathfield = new QLineEdit( generalpage );
    driverpathfield->setObjectName("driverpathfield");
    initSizePolicy(driverpathfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    driverpathfield->setMinimumSize( QSize( 0, 0 ) );

    generalpageLayout->addWidget( driverpathfield, 0, 1 );

    defaultdriverlabel = new QLabel(generalpage);
    defaultdriverlabel->setObjectName("defaultdriverlabel");
    initSizePolicy(defaultdriverlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    generalpageLayout->addWidget( defaultdriverlabel, 1, 0 );

    defaultdriverfield = new QComboBox(generalpage);
    defaultdriverfield->setObjectName(QString::fromAscii("defaultdriverfield"));

    generalpageLayout->addWidget( defaultdriverfield, 1, 1 );

    snap2gridxfield = new QSpinBox( generalpage );
    snap2gridxfield->setObjectName(QString::fromAscii("snap2gridxfield"));

    generalpageLayout->addWidget( snap2gridxfield, 2, 1 );

    snap2gridxlabel = new QLabel(generalpage);
    snap2gridxlabel->setObjectName("snap2gridxlabel");
    initSizePolicy(snap2gridxlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    generalpageLayout->addWidget( snap2gridxlabel, 2, 0 );

    snap2gridylabel = new QLabel(generalpage);
    snap2gridylabel->setObjectName("snap2gridylabel");
    initSizePolicy(snap2gridylabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    generalpageLayout->addWidget( snap2gridylabel, 3, 0 );

    snap2gridyfield = new QSpinBox( generalpage );
    snap2gridyfield->setObjectName(QString::fromAscii("snap2gridyfield"));

    generalpageLayout->addWidget( snap2gridyfield, 3, 1 );

    sizetypelabel = new QLabel(generalpage);
    sizetypelabel->setObjectName("sizetypelabel");
    initSizePolicy(sizetypelabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    generalpageLayout->addWidget( sizetypelabel, 4, 0 );

    sizetypefield = new QComboBox(generalpage);
    sizetypefield->setObjectName(QString::fromAscii("sizetypefield"));

    generalpageLayout->addWidget( sizetypefield, 4, 1 );

    storepasswordfield = new QComboBox(generalpage);
    storepasswordfield->setObjectName(QString::fromAscii("storepasswordfield"));

    generalpageLayout->addWidget( storepasswordfield, 5, 1 );

    storepasswordlabel = new QLabel(generalpage);
    storepasswordlabel->setObjectName("storepasswordlabel");
    initSizePolicy(storepasswordlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    generalpageLayout->addWidget( storepasswordlabel, 5, 0 );
    Spacer6 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    generalpageLayout->addItem( Spacer6, 7, 0 );
    Spacer7 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    generalpageLayout->addItem( Spacer7, 7, 1 );

    automaticupdatelabel = new QLabel(generalpage);
    automaticupdatelabel->setObjectName("automaticupdatelabel");

    generalpageLayout->addWidget( automaticupdatelabel, 6, 0 );

    automaticupdatefield = new QComboBox(generalpage);
    automaticupdatefield->setObjectName(QString::fromAscii("automaticupdatefield"));

    generalpageLayout->addWidget( automaticupdatefield, 6, 1 );
    regionaltab->addTab( generalpage, QString::fromLatin1("") );

    Appearance = new QWidget(regionaltab);
    Appearance->setObjectName(QString::fromAscii("Appearance"));
    AppearanceLayout = new QGridLayout(Appearance);
    AppearanceLayout->setMargin(11);
    AppearanceLayout->setSpacing(6);
    AppearanceLayout->setObjectName(QString::fromAscii("AppearanceLayout")); 

    maximizedlabel = new QLabel(Appearance);
    maximizedlabel->setObjectName("maximizedlabel");
    initSizePolicy(maximizedlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    AppearanceLayout->addWidget( maximizedlabel, 0, 0 );

    maximizedfield = new QComboBox(Appearance);
    maximizedfield->setObjectName(QString::fromAscii("maximizedfield"));
    initSizePolicy(maximizedfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    AppearanceLayout->addWidget( maximizedfield, 0, 1 );

    bepedanticlabel = new QLabel(Appearance);
    bepedanticlabel->setObjectName("bepedanticlabel");
    initSizePolicy(bepedanticlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    AppearanceLayout->addWidget( bepedanticlabel, 1, 0 );

    bepedanticfield = new QComboBox(Appearance);
    bepedanticfield->setObjectName(QString::fromAscii("bepedanticfield"));
    initSizePolicy(bepedanticfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    AppearanceLayout->addWidget( bepedanticfield, 1, 1 );

    use_singleclicklabel = new QLabel(Appearance);
    use_singleclicklabel->setObjectName("use_singleclicklabel");
    initSizePolicy(use_singleclicklabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    AppearanceLayout->addWidget( use_singleclicklabel, 2, 0 );

    use_singleclickfield = new QComboBox(Appearance);
    use_singleclickfield->setObjectName(QString::fromAscii("use_singleclickfield"));
    initSizePolicy(use_singleclickfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    AppearanceLayout->addWidget( use_singleclickfield, 2, 1 );
    spacer10 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    AppearanceLayout->addItem( spacer10, 3, 0 );
    spacer11 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    AppearanceLayout->addItem( spacer11, 3, 1 );
    regionaltab->addTab( Appearance, QString::fromLatin1("") );

    defaults = new QWidget(regionaltab);
    defaults->setObjectName(QString::fromAscii("defaults"));
    defaultsLayout = new QGridLayout( defaults);
    defaultsLayout->setMargin(11);
    defaultsLayout->setSpacing(6);
    defaultsLayout->setObjectName(QString::fromAscii("defaultsLayout")); 

    Layout3 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    Layout3->setMargin(0);
    Layout3->setSpacing(6);
    Layout3->setObjectName("Layout3");

    fontfield = new QComboBox(defaults);
    fontfield->setObjectName(QString::fromAscii("fontfield"));
    initSizePolicy(fontfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    fontfield->setMinimumSize( QSize( 139, 0 ) );
    fontfield->setEditable( true );
    fontfield->setAutoCompletion( true );
    fontfield->setDuplicatesEnabled( false );
    Layout3->addWidget( fontfield );

    fontsizefield = new QSpinBox( defaults );
    fontsizefield->setObjectName(QString::fromAscii("fontsizefield"));
    initSizePolicy(fontfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    Layout3->addWidget( fontsizefield );

    defaultsLayout->addLayout( Layout3, 0, 1 );

    fontlabel = new QLabel(defaults);
    fontlabel->setObjectName("fontlabel");
    initSizePolicy(fontlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    defaultsLayout->addWidget( fontlabel, 0, 0 );

    textalignmentfield = new QComboBox(defaults);
    textalignmentfield->setObjectName(QString::fromAscii("textalignmentfield"));
    initSizePolicy(textalignmentfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    defaultsLayout->addWidget( textalignmentfield, 1, 1 );

    numberalignmentfield = new QComboBox(defaults);
    numberalignmentfield->setObjectName(QString::fromAscii("numberalignmentfield"));
    initSizePolicy(numberalignmentfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    defaultsLayout->addWidget( numberalignmentfield, 2, 1 );

    textalignmentlabel = new QLabel(defaults);
    textalignmentlabel->setObjectName("textalignmentlabel");
    initSizePolicy(textalignmentlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    defaultsLayout->addWidget( textalignmentlabel, 1, 0 );

    numberalignmentlabel = new QLabel(defaults);
    numberalignmentlabel->setObjectName("numberalignmentlabel");
    initSizePolicy(numberalignmentlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    defaultsLayout->addWidget( numberalignmentlabel, 2, 0 );
    Spacer16 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    defaultsLayout->addItem( Spacer16, 6, 0 );
    Spacer15 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    defaultsLayout->addItem( Spacer15, 6, 1 );

    separatorlabel = new QLabel(defaults);
    separatorlabel->setObjectName("separatorlabel");
    initSizePolicy(separatorlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    defaultsLayout->addWidget( separatorlabel, 3, 0 );

    separatorfield = new QComboBox(defaults);
    separatorfield->setObjectName(QString::fromAscii("separatorfield"));
    initSizePolicy(separatorfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    defaultsLayout->addWidget( separatorfield, 3, 1, 2, 1 );

    precisionlabel = new QLabel(defaults);
    precisionlabel->setObjectName("precisionlabel");
    initSizePolicy(precisionlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    defaultsLayout->addWidget( precisionlabel, 4, 0, 2, 1 );

    precisionfield = new QSpinBox( defaults );
    precisionfield->setObjectName(QString::fromAscii("precisionfield"));
    initSizePolicy(precisionfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    precisionfield->setMaximum( 10 );

    defaultsLayout->addWidget( precisionfield, 5, 1 );
    regionaltab->addTab( defaults, QString::fromLatin1("") );

    regionalpage = new QWidget(regionaltab);
    regionalpage->setObjectName(QString::fromAscii("regionalpage"));
    regionalpageLayout = new QGridLayout(regionalpage);
    regionalpageLayout->setMargin(11);
    regionalpageLayout->setSpacing(6);
    regionalpageLayout->setObjectName(QString::fromAscii("regionalpageLayout")); 

    datelabel = new QLabel(regionalpage);
    datelabel->setObjectName("datelabel");

    regionalpageLayout->addWidget( datelabel, 1, 0 );

    datefield = new QLineEdit(regionalpage);
    datefield->setObjectName("datefield");
    initSizePolicy(datefield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    datefield->setMinimumSize( QSize( 30, 0 ) );

    regionalpageLayout->addWidget( datefield, 1, 1 );

    timefield = new QLineEdit(regionalpage);
    timefield->setObjectName("timefield");
    initSizePolicy(timefield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    timefield->setMinimumSize( QSize( 58, 0 ) );

    regionalpageLayout->addWidget( timefield, 0, 1 );

    timelabel = new QLabel(regionalpage);
    timelabel->setObjectName("timelabel");

    regionalpageLayout->addWidget( timelabel, 0, 0 );
    Spacer1 = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    regionalpageLayout->addItem( Spacer1, 5, 1 );
    Spacer2 = new QSpacerItem( 20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding );
    regionalpageLayout->addItem( Spacer2, 5, 0 );

    datetimelabel = new QLabel(regionalpage);
    datetimelabel->setObjectName("datetimelabel");

    regionalpageLayout->addWidget( datetimelabel, 2, 0 );

    measurefield = new QComboBox(regionalpage);
    measurefield->setObjectName(QString::fromAscii("measurefield"));
    initSizePolicy(measurefield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    regionalpageLayout->addWidget( measurefield, 4, 1 );

    measurelabel = new QLabel(regionalpage);
    measurelabel->setObjectName("measurelabel");

    regionalpageLayout->addWidget( measurelabel, 4, 0 );

    datetimefield = new QLineEdit(regionalpage);
    datetimefield->setObjectName("datetimefield");
    initSizePolicy(datetimefield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    datetimefield->setMinimumSize( QSize( 30, 0 ) );

    regionalpageLayout->addWidget( datetimefield, 2, 1 );

    localefield = new QLineEdit(regionalpage);
    localefield->setObjectName("localefield");

    regionalpageLayout->addWidget( localefield, 3, 1 );

    localelabel = new QLabel(regionalpage);
    localelabel->setObjectName("localelabel");

    regionalpageLayout->addWidget( localelabel, 3, 0 );
    regionaltab->addTab( regionalpage, QString::fromLatin1("") );

    printerpage = new QWidget(regionaltab);
    printerpage->setObjectName(QString::fromAscii("printerpage"));
    printerpageLayout = new QGridLayout(printerpage);
    printerpageLayout->setMargin(11);
    printerpageLayout->setSpacing(6);
    printerpageLayout->setObjectName(QString::fromAscii("printerpageLayout")); 

    printcommandlabel = new QLabel(printerpage);
    printcommandlabel->setObjectName("printcommandlabel");

    printerpageLayout->addWidget( printcommandlabel, 0, 0 );
    Spacer1_2 = new QSpacerItem( 20, 220, QSizePolicy::Minimum, QSizePolicy::Expanding );
    printerpageLayout->addItem( Spacer1_2, 2, 1 );
    Spacer2_2 = new QSpacerItem( 20, 230, QSizePolicy::Minimum, QSizePolicy::Expanding );
    printerpageLayout->addItem( Spacer2_2, 2, 0 );

    printcommandfield = new QLineEdit(printerpage);
    printcommandfield->setObjectName("printcommandfield");

    printerpageLayout->addWidget( printcommandfield, 0, 1 );

    embedfontsfield = new QComboBox(printerpage);
    embedfontsfield->setObjectName(QString::fromAscii("embedfontsfield"));

    printerpageLayout->addWidget( embedfontsfield, 1, 1 );

    embedfontslabel = new QLabel(printerpage);
    embedfontslabel->setObjectName("embedfontslabel");

    printerpageLayout->addWidget( embedfontslabel, 1, 0 );
    regionaltab->addTab( printerpage, QString::fromLatin1("") );
    hk_kdepreferencesdialogbaseLayout->addWidget( regionaltab );

    layout13 = new QHBoxLayout(static_cast<QWidget*>(NULL));
    layout13->setMargin(0);
    layout13->setSpacing(6);
    layout13->setObjectName("layout13");

    buttonHelp = new QPushButton(this);
    buttonHelp->setObjectName("buttonHelp");
    buttonHelp->setAutoDefault( true );
    layout13->addWidget( buttonHelp );
    Horizontal_Spacing2 = new QSpacerItem( 310, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout13->addItem( Horizontal_Spacing2 );

    buttonOk = new QPushButton(this);
    buttonOk->setObjectName("buttonOk");
    buttonOk->setAutoDefault( true );
    buttonOk->setDefault( true );
    layout13->addWidget( buttonOk );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName("buttonCancel");
    buttonCancel->setAutoDefault( true );
    layout13->addWidget( buttonCancel );
    hk_kdepreferencesdialogbaseLayout->addLayout( layout13 );
    languageChange();
    resize( QSize(617, 461).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( driverpathbutton, SIGNAL( clicked() ), this, SLOT( driverpathbutton_clicked() ) );
    connect( buttonHelp, SIGNAL( clicked() ), this, SLOT( help_clicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdepreferencesdialogbase::~hk_kdepreferencesdialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdepreferencesdialogbase::languageChange()
{
    setWindowTitle( tr( "Preferences" ) );
    //TBP TBT translation tr()->i18n()
    driverpathlabel->setText( tr( "Driverpath:" ) );
    driverpathbutton->setText( tr( "..." ) );
    defaultdriverlabel->setText( tr( "Default driver:" ) );
    snap2gridxlabel->setText( tr( "Snap to grid x:" ) );
    snap2gridylabel->setText( tr( "Snap to grid y:" ) );
    sizetypelabel->setText( tr( "Metric system:" ) );
    sizetypefield->clear();
    sizetypefield->addItem( tr( "Relative" ) );
    sizetypefield->addItem( tr( "Absolute" ) );
    storepasswordlabel->setText( tr( "Store password:" ) );
    automaticupdatelabel->setText( tr( "Automatic data update:" ) );
    automaticupdatefield->clear();
    automaticupdatefield->addItem( tr( "Yes" ) );
    automaticupdatefield->addItem( tr( "No" ) );
    regionaltab->setTabText( regionaltab->indexOf(generalpage), tr( "&General" ) );
    maximizedlabel->setText( tr( "show maximized: " ) );
    bepedanticlabel->setText( tr( "be pedantic:" ) );
    use_singleclicklabel->setText( tr( "Use single click:" ) );
    regionaltab->setTabText( regionaltab->indexOf(Appearance), tr( "&Appearance" ) );
    fontlabel->setText( tr( "Default font:" ) );
    textalignmentlabel->setText( tr( "Textalignment:" ) );
    numberalignmentlabel->setText( tr( "Numberalignment:" ) );
    separatorlabel->setText( tr( "Separator:" ) );
    precisionlabel->setText( tr( "Precision:" ) );
    regionaltab->setTabText( regionaltab->indexOf(defaults), tr( "&Defaults" ) );
    datelabel->setText( tr( "Date format:" ) );
    timelabel->setText( tr( "Time format:" ) );
    datetimelabel->setText( tr( "Date&Time format:" ) );
    measurefield->clear();
    measurefield->addItem( tr( "cm" ) );
    measurefield->addItem( tr( "inch" ) );
    measurelabel->setText( tr( "Measure system:" ) );
    localelabel->setText( tr( "Locale:" ) );
    regionaltab->setTabText( regionaltab->indexOf(regionalpage), tr( "&Regional" ) );
    printcommandlabel->setText( tr( "Print command:" ) );
    printcommandfield->setText( QString::null );
    embedfontsfield->clear();
    embedfontsfield->addItem( tr( "Yes" ) );
    embedfontsfield->addItem( tr( "No" ) );
    embedfontsfield->setCurrentIndex( 1 );
    embedfontslabel->setText( tr( "Embed fonts:" ) );
    regionaltab->setTabText( regionaltab->indexOf(printerpage), tr( "&Printer" ) );
    buttonHelp->setText( tr( "&Help" ) );
    buttonHelp->setShortcut( QKeySequence( tr( "F1" ) ) );
    buttonOk->setText( tr( "&OK" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    buttonCancel->setShortcut( QKeySequence( tr( "Alt+C" ) ) );
}

void hk_kdepreferencesdialogbase::driverpathbutton_clicked()
{
    qWarning( "hk_kdepreferencesdialogbase::driverpathbutton_clicked(): Not implemented yet" );
}

void hk_kdepreferencesdialogbase::help_clicked()
{
    qWarning( "hk_kdepreferencesdialogbase::help_clicked(): Not implemented yet" );
}

