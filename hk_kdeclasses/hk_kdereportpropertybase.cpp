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

#include "hk_kdereportpropertybase.h"

#include <qvariant.h>
#include <hk_kdespinbox.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

void inline initSizePolicy( QWidget* w, QSizePolicy::Policy hp, QSizePolicy::Policy vp)
{
    QSizePolicy policy(hp,vp);
    
    policy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
    w->setSizePolicy(policy);
}

/*
 *  Constructs a hk_kdereportpropertybase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
hk_kdereportpropertybase::hk_kdereportpropertybase( QWidget* parent, const char* name, Qt::WFlags fl )
    : QWidget( parent, fl )
{
    setObjectName(QString::fromAscii(name==NULL?"hk_kdereportpropertybase":name));
    setMinimumSize(QSize(310, 0));
    initSizePolicy(this, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    hk_kdereportpropertybaseLayout = new QGridLayout(this);
    hk_kdereportpropertybaseLayout->setMargin(0);
    hk_kdereportpropertybaseLayout->setSpacing(0);
    hk_kdereportpropertybaseLayout->setObjectName(QString::fromAscii("hk_kdereportpropertybaseLayout")); 

    tab = new QTabWidget( this );
    tab->setObjectName(QString::fromAscii("tab"));
    initSizePolicy(tab, QSizePolicy::Expanding, QSizePolicy::Expanding);
    tab->setMinimumSize( QSize( 0, 0 ) );

    datatab = new QWidget(tab);
    datatab->setObjectName(QString::fromAscii("datatab"));
    datatabLayout = new QGridLayout(datatab);
    datatabLayout->setMargin(11);
    datatabLayout->setSpacing(6);
    datatabLayout->setObjectName(QString::fromAscii("datatabLayout")); 
    Spacer3 = new QSpacerItem( 20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding );
    datatabLayout->addItem( Spacer3, 8, 0 );
    Spacer4 = new QSpacerItem( 20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding );
    datatabLayout->addItem( Spacer4, 8, 1 );

    subreportlabel = new QLabel(datatab);
    subreportlabel->setObjectName("subreportlabel");

    datatabLayout->addWidget( subreportlabel, 7, 0 );

    identifierfield = new QLineEdit(datatab);
    identifierfield->setObjectName("identifierfield");

    datatabLayout->addWidget( identifierfield, 2, 1 );

    datasourcebutton = new QToolButton(datatab);
    datasourcebutton->setObjectName(QString::fromAscii("datasourcebutton"));
    datasourcebutton->setMinimumSize( QSize( 20, 21 ) );
    datasourcebutton->setMaximumSize( QSize( 20, 21 ) );

    datatabLayout->addWidget( datasourcebutton, 1, 2 );

    runningcountfield = new QComboBox(datatab);
    runningcountfield->setObjectName(QString::fromAscii("runningcountfield"));

    datatabLayout->addWidget( runningcountfield, 6, 1 );

    columnlabel = new QLabel(datatab);
    columnlabel->setObjectName("columnlabel");

    datatabLayout->addWidget( columnlabel, 4, 0 );

    sortingfield = new QComboBox(datatab);
    sortingfield->setObjectName(QString::fromAscii("sortingfield"));

    datatabLayout->addWidget( sortingfield, 3, 1 );

    subreportbutton = new QPushButton(datatab);
    subreportbutton->setObjectName("subreportbutton");

    datatabLayout->addWidget( subreportbutton, 7, 1 );

    valuefield = new QLineEdit(datatab);
    valuefield->setObjectName("valuefield");

    datatabLayout->addWidget( valuefield, 5, 1 );

    identifierlabel = new QLabel(datatab);
    identifierlabel->setObjectName("identifierlabel");

    datatabLayout->addWidget( identifierlabel, 2, 0 );

    datasourcefield = new QComboBox(datatab);
    datasourcefield->setObjectName(QString::fromAscii("datasourcefield"));
    datasourcefield->setMinimumSize( QSize( 120, 0 ) );

    datatabLayout->addWidget( datasourcefield, 1, 1 );

    runningcountlabel = new QLabel(datatab);
    runningcountlabel->setObjectName("runningcountlabel");

    datatabLayout->addWidget( runningcountlabel, 6, 0 );

    valuelabel = new QLabel(datatab);
    valuelabel->setObjectName("valuelabel");

    datatabLayout->addWidget( valuelabel, 5, 0 );

    sortinglabel = new QLabel(datatab);
    sortinglabel->setObjectName("sortinglabel");

    datatabLayout->addWidget( sortinglabel, 3, 0 );

    columnfield = new QComboBox(datatab);
    columnfield->setObjectName(QString::fromAscii("columnfield"));
    columnfield->setEditable(true);
    columnfield->setAutoCompletion(true);
    columnfield->setDuplicatesEnabled(false);

    datatabLayout->addWidget( columnfield, 4, 1 );

    datasourcelabel = new QLabel(datatab);
    datasourcelabel->setObjectName("datasourcelabel");

    datatabLayout->addWidget( datasourcelabel, 1, 0 );

    Idlabel = new QLabel(datatab);
    Idlabel->setObjectName("Idlabel");
    initSizePolicy(Idlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    datatabLayout->addWidget( Idlabel, 0, 0 );

    idfield = new QLineEdit(datatab);
    idfield->setObjectName("idfield");
    idfield->setEnabled(false);

    datatabLayout->addWidget( idfield, 0, 1 );
    tab->addTab( datatab, QString::fromLatin1("") );

    formattab = new QWidget(tab);
    formattab->setObjectName(QString::fromAscii("formattab"));
    formattabLayout = new QGridLayout(formattab);
    formattabLayout->setMargin(11);
    formattabLayout->setSpacing(6);
    formattabLayout->setObjectName(QString::fromAscii("formattabLayout")); 

    widthlabel = new QLabel(formattab);
    widthlabel->setObjectName("widthlabel");
    initSizePolicy(widthlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    formattabLayout->addWidget( widthlabel, 3, 0 );

    fontlabel = new QLabel(formattab);
    fontlabel->setObjectName("fontlabel");
    initSizePolicy(fontlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    formattabLayout->addWidget( fontlabel, 5, 0 );

    fontfield = new QComboBox(formattab);
    fontfield->setObjectName(QString::fromAscii("fontfield"));
    initSizePolicy(fontfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    fontfield->setEditable(true);
    fontfield->setAutoCompletion(true);
    fontfield->setDuplicatesEnabled(false);

    formattabLayout->addWidget( fontfield, 5, 1 );

    fontsizefield = new QSpinBox( formattab );
    fontsizefield->setObjectName(QString::fromAscii("fontsizefield"));
    initSizePolicy(fontsizefield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    fontsizefield->setValue( 10 );

    formattabLayout->addWidget( fontsizefield, 6, 1 );

    ylabel = new QLabel(formattab);
    ylabel->setObjectName("ylabel");
    initSizePolicy(ylabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    formattabLayout->addWidget( ylabel, 2, 0 );

    fontsizelabel = new QLabel(formattab);
    fontsizelabel->setObjectName("fontsizelabel");
    initSizePolicy(fontsizelabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    formattabLayout->addWidget( fontsizelabel, 6, 0 );

    reporttypelabel = new QLabel(formattab);
    reporttypelabel->setObjectName("reporttypelabel");
    initSizePolicy(reporttypelabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    formattabLayout->addWidget( reporttypelabel, 0, 0 );

    reporttypefield = new QComboBox(formattab);
    reporttypefield->setObjectName(QString::fromAscii("reporttypefield"));
    initSizePolicy(reporttypefield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    reporttypefield->setMinimumSize( QSize( 0, 0 ) );

    formattabLayout->addWidget( reporttypefield, 0, 1 );

    alignmentlabel = new QLabel(formattab);
    alignmentlabel->setObjectName("alignmentlabel");
    initSizePolicy(alignmentlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    formattabLayout->addWidget( alignmentlabel, 9, 0 );

    alignmentfield = new QComboBox(formattab);
    alignmentfield->setObjectName(QString::fromAscii("alignmentfield"));
    initSizePolicy(alignmentfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    formattabLayout->addWidget( alignmentfield, 9, 1 );

    boldlabel = new QLabel(formattab);
    boldlabel->setObjectName("boldlabel");
    initSizePolicy(boldlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    formattabLayout->addWidget( boldlabel, 7, 0 );

    italiclabel = new QLabel(formattab);
    italiclabel->setObjectName("italiclabel");
    initSizePolicy(italiclabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    formattabLayout->addWidget( italiclabel, 8, 0 );

    boldfield = new QComboBox(formattab);
    boldfield->setObjectName(QString::fromAscii("boldfield"));
    initSizePolicy(boldfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    formattabLayout->addWidget( boldfield, 7, 1 );

    italicfield = new QComboBox(formattab);
    italicfield->setObjectName(QString::fromAscii("italicfield"));
    initSizePolicy(italicfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    
    formattabLayout->addWidget( italicfield, 8, 1 );

    xlabel = new QLabel(formattab);
    xlabel->setObjectName("xlabel");
    initSizePolicy(xlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    formattabLayout->addWidget( xlabel, 1, 0 );

    xfield = new hk_kdespinbox( formattab, "xfield" );
    initSizePolicy(xfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    xfield->setMinimumSize( QSize( 0, 20 ) );
    
    formattabLayout->addWidget( xfield, 1, 1 );

    heightlabel = new QLabel(formattab);
    heightlabel->setObjectName("heightlabel");
    initSizePolicy(heightlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    formattabLayout->addWidget( heightlabel, 4, 0 );

    heightfield = new hk_kdespinbox( formattab, "heightfield" );
    initSizePolicy(heightfield, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    heightfield->setMinimumSize( QSize( 0, 20 ) );
    
    formattabLayout->addWidget( heightfield, 4, 1 );

    widthfield = new hk_kdespinbox( formattab, "widthfield" );
    initSizePolicy(widthfield, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    widthfield->setMinimumSize( QSize( 0, 20 ) );

    formattabLayout->addWidget( widthfield, 3, 1 );

    yfield = new hk_kdespinbox( formattab, "yfield" );
    initSizePolicy(yfield, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    yfield->setMinimumSize( QSize( 0, 20 ) );

    formattabLayout->addWidget( yfield, 2, 1 );

    digitfield = new QSpinBox( formattab );
    digitfield->setObjectName(QString::fromAscii("digitfield"));
    initSizePolicy(digitfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    digitfield->setMaximum( 15 );

    formattabLayout->addWidget( digitfield, 12, 1 );

    separatorfield = new QComboBox(formattab);
    separatorfield->setObjectName(QString::fromAscii("separatorfield"));
    initSizePolicy(separatorfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    formattabLayout->addWidget( separatorfield, 13, 1 );

    digitlabel = new QLabel(formattab);
    digitlabel->setObjectName("digitlabel");
    initSizePolicy(digitlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    formattabLayout->addWidget( digitlabel, 12, 0 );

    separatorlabel = new QLabel(formattab);
    separatorlabel->setObjectName("separatorlabel");
    initSizePolicy(separatorlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    formattabLayout->addWidget( separatorlabel, 13, 0 );

    wordbreaklabel = new QLabel(formattab);
    wordbreaklabel->setObjectName("wordbreaklabel");
    initSizePolicy(wordbreaklabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    formattabLayout->addWidget( wordbreaklabel, 10, 0 );

    wordbreakfield = new QComboBox(formattab);
    wordbreakfield->setObjectName(QString::fromAscii("wordbreakfield"));
    initSizePolicy(wordbreakfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    formattabLayout->addWidget( wordbreakfield, 10, 1 );

    dynamicheightlabel = new QLabel(formattab);
    dynamicheightlabel->setObjectName("dynamicheightlabel");
    initSizePolicy(dynamicheightlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    formattabLayout->addWidget( dynamicheightlabel, 11, 0 );

    dynamicheightfield = new QComboBox(formattab);
    dynamicheightfield->setObjectName(QString::fromAscii("dynamicheightfield"));
    initSizePolicy(dynamicheightfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    formattabLayout->addWidget( dynamicheightfield, 11, 1 );
    tab->addTab( formattab, QString::fromLatin1("") );

    frametab = new QWidget(tab);
    frametab->setObjectName(QString::fromAscii("frametab"));
    frametabLayout = new QGridLayout(frametab);
    frametabLayout->setMargin(11);
    frametabLayout->setSpacing(6);
    frametabLayout->setObjectName(QString::fromAscii("frametabLayout")); 

    toplinefield = new QComboBox(frametab);
    toplinefield->setObjectName(QString::fromAscii("toplinefield"));

    frametabLayout->addWidget( toplinefield, 0, 1 );

    bottomlinefield = new QComboBox(frametab);
    bottomlinefield->setObjectName(QString::fromAscii("bottomlinefield"));

    frametabLayout->addWidget( bottomlinefield, 1, 1 );

    leftlinefield = new QComboBox(frametab);
    leftlinefield->setObjectName(QString::fromAscii("leftlinefield"));

    frametabLayout->addWidget( leftlinefield, 2, 1 );

    rightlinefield = new QComboBox(frametab);
    rightlinefield->setObjectName(QString::fromAscii("rightlinefield"));

    frametabLayout->addWidget( rightlinefield, 3, 1 );

    diagonallurofield = new QComboBox(frametab);
    diagonallurofield->setObjectName(QString::fromAscii("diagonallurofield"));

    frametabLayout->addWidget( diagonallurofield, 4, 1 );

    diagonallorufield = new QComboBox(frametab);
    diagonallorufield->setObjectName(QString::fromAscii("diagonallorufield"));

    frametabLayout->addWidget( diagonallorufield, 5, 1 );

    toplinelabel = new QLabel(frametab);
    toplinelabel->setObjectName("toplinelabel");

    frametabLayout->addWidget( toplinelabel, 0, 0 );

    bottomlinelabel = new QLabel(frametab);
    bottomlinelabel->setObjectName("bottomlinelabel");

    frametabLayout->addWidget( bottomlinelabel, 1, 0 );

    leftlinelabel = new QLabel(frametab);
    leftlinelabel->setObjectName("leftlinelabel");

    frametabLayout->addWidget( leftlinelabel, 2, 0 );

    rightlinelabel = new QLabel(frametab);
    rightlinelabel->setObjectName("rightlinelabel");

    frametabLayout->addWidget( rightlinelabel, 3, 0 );

    diagonallurolabel = new QLabel(frametab);
    diagonallurolabel->setObjectName("diagonallurolabel");

    frametabLayout->addWidget( diagonallurolabel, 4, 0 );

    diagonallorulabel = new QLabel(frametab);
    diagonallorulabel->setObjectName("diagonallorulabel");

    frametabLayout->addWidget( diagonallorulabel, 5, 0 );

    bordertopfield = new hk_kdespinbox( frametab, "bordertopfield" );

    frametabLayout->addWidget( bordertopfield, 6, 1 );

    borderbottomfield = new hk_kdespinbox( frametab, "borderbottomfield" );

    frametabLayout->addWidget( borderbottomfield, 7, 1 );

    borderleftfield = new hk_kdespinbox( frametab, "borderleftfield" );

    frametabLayout->addWidget( borderleftfield, 8, 1 );

    borderrightfield = new hk_kdespinbox( frametab, "borderrightfield" );

    frametabLayout->addWidget( borderrightfield, 9, 1 );
    Spacer1_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    frametabLayout->addItem( Spacer1_2, 10, 0 );
    Spacer2_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    frametabLayout->addItem( Spacer2_2, 10, 1 );

    borderrightlabel = new QLabel(frametab);
    borderrightlabel->setObjectName("borderrightlabel");

    frametabLayout->addWidget( borderrightlabel, 9, 0 );

    borderleftlabel = new QLabel(frametab);
    borderleftlabel->setObjectName("borderleftlabel");

    frametabLayout->addWidget( borderleftlabel, 8, 0 );

    borderbottomlabel = new QLabel(frametab);
    borderbottomlabel->setObjectName("borderbottomlabel");

    frametabLayout->addWidget( borderbottomlabel, 7, 0 );

    bordertoplabel = new QLabel(frametab);
    bordertoplabel->setObjectName("bordertoplabel");

    frametabLayout->addWidget( bordertoplabel, 6, 0 );
    tab->addTab( frametab, QString::fromLatin1("") );

    actiontab = new QWidget(tab);
    actiontab->setObjectName(QString::fromAscii("actiontab"));
    actiontabLayout = new QGridLayout(actiontab);
    actiontabLayout->setMargin(11);
    actiontabLayout->setSpacing(6);
    actiontabLayout->setObjectName(QString::fromAscii("actiontabLayout")); 

    onprintlabel = new QLabel(actiontab);
    onprintlabel->setObjectName("onprintlabel");

    actiontabLayout->addWidget( onprintlabel, 0, 0 );

    onprintaction = new QPushButton(actiontab);
    onprintaction->setObjectName("onprintaction");

    actiontabLayout->addWidget( onprintaction, 0, 1 );

    afterrowchangelabel_3 = new QLabel(actiontab);
    afterrowchangelabel_3->setObjectName("afterrowchangelabel_3");

    actiontabLayout->addWidget( afterrowchangelabel_3, 5, 0 );

    beforerowchangelabel_3 = new QLabel(actiontab);
    beforerowchangelabel_3->setObjectName("beforerowchangelabel_3");

    actiontabLayout->addWidget( beforerowchangelabel_3, 4, 0 );

    afterrowchangeaction = new QPushButton(actiontab);
    afterrowchangeaction->setObjectName("afterrowchangeaction");

    actiontabLayout->addWidget( afterrowchangeaction, 5, 1 );

    openactionlabel = new QLabel(actiontab);
    openactionlabel->setObjectName("openactionlabel");

    actiontabLayout->addWidget( openactionlabel, 2, 0 );

    closeactionlabel = new QLabel(actiontab);
    closeactionlabel->setObjectName("closeactionlabel");

    actiontabLayout->addWidget( closeactionlabel, 3, 0 );

    onnewpagelabel = new QLabel(actiontab);
    onnewpagelabel->setObjectName("onnewpagelabel");

    actiontabLayout->addWidget( onnewpagelabel, 1, 0 );

    onnewpageaction = new QPushButton(actiontab);
    onnewpageaction->setObjectName("onnewpageaction");

    actiontabLayout->addWidget( onnewpageaction, 1, 1 );

    onopenactionbutton = new QPushButton(actiontab);
    onopenactionbutton->setObjectName("onopenactionbutton");

    actiontabLayout->addWidget( onopenactionbutton, 2, 1 );

    oncloseactionbutton = new QPushButton(actiontab);
    oncloseactionbutton->setObjectName("oncloseactionbutton");

    actiontabLayout->addWidget( oncloseactionbutton, 3, 1 );

    beforerowchangeaction = new QPushButton(actiontab);
    beforerowchangeaction->setObjectName("beforerowchangeaction");

    actiontabLayout->addWidget( beforerowchangeaction, 4, 1 );
    spacer23 = new QSpacerItem( 20, 140, QSizePolicy::Minimum, QSizePolicy::Expanding );
    actiontabLayout->addItem( spacer23, 6, 0 );
    spacer24 = new QSpacerItem( 20, 140, QSizePolicy::Minimum, QSizePolicy::Expanding );
    actiontabLayout->addItem( spacer24, 6, 1 );
    tab->addTab( actiontab, QString::fromLatin1("") );

    hk_kdereportpropertybaseLayout->addWidget( tab, 0, 0 );
    languageChange();
    resize( QSize(508, 578).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( afterrowchangeaction, SIGNAL( clicked() ), this, SLOT( afterrowchange_action_clicked() ) );
    connect( alignmentfield, SIGNAL( activated(const QString&) ), this, SLOT( format_changes() ) );
    connect( alignmentfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( beforerowchangeaction, SIGNAL( clicked() ), this, SLOT( beforerowchange_action_clicked() ) );
    connect( boldfield, SIGNAL( activated(int) ), this, SLOT( font_changes() ) );
    connect( borderbottomfield, SIGNAL( valueChanged(int) ), this, SLOT( data_changes() ) );
    connect( borderleftfield, SIGNAL( valueChanged(int) ), this, SLOT( data_changes() ) );
    connect( borderrightfield, SIGNAL( valueChanged(int) ), this, SLOT( data_changes() ) );
    connect( bordertopfield, SIGNAL( valueChanged(int) ), this, SLOT( data_changes() ) );
    connect( bottomlinefield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( bottomlinefield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( columnfield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
    connect( columnfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( data_changes() ) );
    connect( datasourcebutton, SIGNAL( clicked() ), this, SLOT( datasourcebutton_clicked() ) );
    connect( datasourcefield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( datasource_selected() ) );
    connect( datasourcefield, SIGNAL( activated(int) ), this, SLOT( datasource_selected() ) );
    connect( datasourcefield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
    connect( diagonallorufield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( diagonallorufield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( diagonallurofield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( diagonallurofield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( digitfield, SIGNAL( valueChanged(int) ), this, SLOT( format_changes() ) );
    connect( fontfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( font_changes() ) );
    connect( fontsizefield, SIGNAL( valueChanged(int) ), this, SLOT( font_changes() ) );
    connect( heightfield, SIGNAL( valueChanged(int) ), this, SLOT( size_changes() ) );
    connect( identifierfield, SIGNAL( returnPressed() ), this, SLOT( identifier_changed() ) );
    connect( identifierfield, SIGNAL( editingFinished() ), this, SLOT( identifier_changed() ) );
    connect( italicfield, SIGNAL( activated(int) ), this, SLOT( font_changes() ) );
    connect( leftlinefield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( leftlinefield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( oncloseactionbutton, SIGNAL( clicked() ), this, SLOT( closeactionbutton_clicked() ) );
    connect( onnewpageaction, SIGNAL( clicked() ), this, SLOT( onprintnewpage_action_clicked() ) );
    connect( onopenactionbutton, SIGNAL( clicked() ), this, SLOT( openactionbutton_clicked() ) );
    connect( onprintaction, SIGNAL( clicked() ), this, SLOT( onprint_action_clicked() ) );
    connect( rightlinefield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( rightlinefield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( runningcountfield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
    connect( separatorfield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( sortingfield, SIGNAL( activated(int) ), this, SLOT( data_changes() ) );
    connect( subreportbutton, SIGNAL( clicked() ), this, SLOT( subreportbutton_clicked() ) );
    connect( toplinefield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( toplinefield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( valuefield, SIGNAL( textChanged(const QString&) ), this, SLOT( data_changes() ) );
    connect( widthfield, SIGNAL( valueChanged(int) ), this, SLOT( size_changes() ) );
    connect( wordbreakfield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( wordbreakfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( xfield, SIGNAL( valueChanged(int) ), this, SLOT( position_changes() ) );
    connect( yfield, SIGNAL( valueChanged(int) ), this, SLOT( position_changes() ) );
    connect( dynamicheightfield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( dynamicheightfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( format_changes() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdereportpropertybase::~hk_kdereportpropertybase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdereportpropertybase::languageChange()
{
    setWindowTitle( tr( "Property Editor" ) );
    subreportlabel->setText( tr( "Subreport:" ) );
    datasourcebutton->setText( tr( "..." ) );
    columnlabel->setText( tr( "Column:" ) );
    subreportbutton->setText( QString::null );
    identifierlabel->setText( tr( "Identifier:" ) );
    runningcountlabel->setText( tr( "RunningCount:" ) );
    valuelabel->setText( tr( "Value:" ) );
    sortinglabel->setText( tr( "Sorting:" ) );
    datasourcelabel->setText( tr( "Datasource:" ) );
    Idlabel->setText( tr( "Id:" ) );
    tab->setTabText( tab->indexOf(datatab), tr( "&Data" ) );
    widthlabel->setText( tr( "Width:" ) );
    fontlabel->setText( tr( "Font:" ) );
    ylabel->setText( tr( "Y:" ) );
    fontsizelabel->setText( tr( "Fontsize:" ) );
    reporttypelabel->setText( tr( "Reporttype:" ) );
    alignmentlabel->setText( tr( "Alignment:" ) );
    boldlabel->setText( tr( "Bold:" ) );
    italiclabel->setText( tr( "Italic:" ) );
    xlabel->setText( tr( "X:" ) );
    heightlabel->setText( tr( "Height:" ) );
    digitlabel->setText( tr( "TextLabel1" ) );
    separatorlabel->setText( tr( "TextLabel2" ) );
    wordbreaklabel->setText( tr( "Linebreak:" ) );
    dynamicheightlabel->setText( tr( "Dynamic height:" ) );
    tab->setTabText( tab->indexOf(formattab), tr( "&Format" ) );
    toplinelabel->setText( tr( "TopLine:" ) );
    bottomlinelabel->setText( tr( "BottomLine:" ) );
    leftlinelabel->setText( tr( "LeftLine:" ) );
    rightlinelabel->setText( tr( "RightLine:" ) );
    diagonallurolabel->setText( tr( "DiagonalLURO:" ) );
    diagonallorulabel->setText( tr( "DiagonalLORU:" ) );
    borderrightlabel->setText( tr( "Border Right:" ) );
    borderleftlabel->setText( tr( "Border Left:" ) );
    borderbottomlabel->setText( tr( "Border Bottom:" ) );
    bordertoplabel->setText( tr( "Border Top:" ) );
    tab->setTabText( tab->indexOf(frametab), tr( "F&rame" ) );
    onprintlabel->setText( tr( "On print:" ) );
    onprintaction->setText( QString::null );
    afterrowchangelabel_3->setText( tr( "After row change:" ) );
    beforerowchangelabel_3->setText( tr( "Before row change:" ) );
    afterrowchangeaction->setText( QString::null );
    openactionlabel->setText( tr( "On open:" ) );
    closeactionlabel->setText( tr( "On close:" ) );
    onnewpagelabel->setText( tr( "On new page:" ) );
    onnewpageaction->setText( QString::null );
    onopenactionbutton->setText( QString::null );
    oncloseactionbutton->setText( QString::null );
    beforerowchangeaction->setText( QString::null );
    tab->setTabText( tab->indexOf(actiontab), tr( "&Action" ) );
}

void hk_kdereportpropertybase::identifier_changed()
{
    qWarning( "hk_kdereportpropertybase::identifier_changed(): Not implemented yet" );
}

void hk_kdereportpropertybase::actionobject_changes()
{
    qWarning( "hk_kdereportpropertybase::actionobject_changes(): Not implemented yet" );
}

void hk_kdereportpropertybase::add_propertydatasource()
{
    qWarning( "hk_kdereportpropertybase::add_propertydatasource(): Not implemented yet" );
}

void hk_kdereportpropertybase::data_changes()
{
    qWarning( "hk_kdereportpropertybase::data_changes(): Not implemented yet" );
}

void hk_kdereportpropertybase::datasource_selected()
{
    qWarning( "hk_kdereportpropertybase::datasource_selected(): Not implemented yet" );
}

void hk_kdereportpropertybase::datasourcebutton_clicked()
{
    qWarning( "hk_kdereportpropertybase::datasourcebutton_clicked(): Not implemented yet" );
}

void hk_kdereportpropertybase::format_changes()
{
    qWarning( "hk_kdereportpropertybase::format_changes(): Not implemented yet" );
}

void hk_kdereportpropertybase::subreportbutton_clicked()
{
    qWarning( "hk_kdereportpropertybase::subreportbutton_clicked(): Not implemented yet" );
}

void hk_kdereportpropertybase::set_object(hk_visible*)
{
    qWarning( "hk_kdereportpropertybase::set_object(hk_visible*): Not implemented yet" );
}

void hk_kdereportpropertybase::onprint_action_clicked()
{
    qWarning( "hk_kdereportpropertybase::onprint_action_clicked(): Not implemented yet" );
}

void hk_kdereportpropertybase::beforerowchange_action_clicked()
{
    qWarning( "hk_kdereportpropertybase::beforerowchange_action_clicked(): Not implemented yet" );
}

void hk_kdereportpropertybase::afterrowchange_action_clicked()
{
    qWarning( "hk_kdereportpropertybase::afterrowchange_action_clicked(): Not implemented yet" );
}

void hk_kdereportpropertybase::onprintnewpage_action_clicked()
{
    qWarning( "hk_kdereportpropertybase::onprintnewpage_action_clicked(): Not implemented yet" );
}

void hk_kdereportpropertybase::closeactionbutton_clicked()
{
    qWarning( "hk_kdereportpropertybase::closeactionbutton_clicked(): Not implemented yet" );
}

void hk_kdereportpropertybase::openactionbutton_clicked()
{
    qWarning( "hk_kdereportpropertybase::openactionbutton_clicked(): Not implemented yet" );
}

void hk_kdereportpropertybase::font_changes()
{
    qWarning( "hk_kdereportpropertybase::font_changes(): Not implemented yet" );
}

void hk_kdereportpropertybase::position_changes()
{
    qWarning( "hk_kdereportpropertybase::position_changes(): Not implemented yet" );
}

void hk_kdereportpropertybase::size_changes()
{
    qWarning( "hk_kdereportpropertybase::size_changes(): Not implemented yet" );
}

