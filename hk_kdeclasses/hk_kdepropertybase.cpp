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
** Form implementation generated from reading ui file 'propertyeditor.ui'
**
** Created: Sa Jul 30 16:59:14 2005
**      by: The User Interface Compiler ($Id: hk_kdepropertybase.cpp,v 1.51 2005/07/30 15:07:58 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "hk_kdepropertybase.h"

#include <qvariant.h>
#include <hk_kdespinbox.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qspinbox.h>
#include <kcolorbutton.h>
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
 *  Constructs a hk_kdepropertyeditorbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
hk_kdepropertyeditorbase::hk_kdepropertyeditorbase( QWidget* parent, const char* name, Qt::WFlags fl )
    : QWidget( parent, fl )
{
    setObjectName(QString::fromAscii(name == NULL?"hk_kdepropertyeditorbase":name));
    initSizePolicy(this, QSizePolicy::Preferred, QSizePolicy::Preferred);
    hk_kdepropertyeditorbaseLayout = new QGridLayout(this);
    hk_kdepropertyeditorbaseLayout->setMargin(2);
    hk_kdepropertyeditorbaseLayout->setSpacing(1);
    hk_kdepropertyeditorbaseLayout->setObjectName(QString::fromAscii("hk_kdepropertyeditorbaseLayout")); 

    tab = new QTabWidget( this );
    tab->setObjectName(QString::fromAscii("tab"));
    initSizePolicy(tab, QSizePolicy::Expanding, QSizePolicy::Expanding);

    tab_2 = new QWidget(tab);
    tab_2->setObjectName(QString::fromAscii("tab_2"));
    tabLayout = new QGridLayout(tab_2);
    tabLayout->setMargin(11);
    tabLayout->setSpacing(6);
    tabLayout->setObjectName(QString::fromAscii("tabLayout")); 

    labelfield = new QLineEdit(tab_2);
    labelfield->setObjectName("labelfield");

    tabLayout->addWidget( labelfield, 4, 1 );

    combomodelabel = new QLabel(tab_2);
    combomodelabel->setObjectName("combomodelabel");
    initSizePolicy(combomodelabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( combomodelabel, 8, 0 );

    labellabel = new QLabel(tab_2);
    labellabel->setObjectName("labellabel");
    initSizePolicy(labellabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( labellabel, 4, 0 );

    combomodefield = new QComboBox(tab_2);
    combomodefield->setObjectName(QString::fromAscii("combomodefield"));
    combomodefield->setEnabled( TRUE );
    initSizePolicy(combomodefield, QSizePolicy::Preferred, QSizePolicy::Fixed);


    tabLayout->addWidget( combomodefield, 8, 1 );

    defaultlabel = new QLabel(tab_2);
    defaultlabel->setObjectName("defaultlabel");
    initSizePolicy(defaultlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    tabLayout->addWidget( defaultlabel, 6, 0 );

    defaultfield = new QLineEdit(tab_2);
    defaultfield->setObjectName("defaultfield");

    tabLayout->addWidget( defaultfield, 6, 1 );

    datasourcebutton = new QToolButton(tab_2);
    datasourcebutton->setObjectName(QString::fromAscii("datasourcebutton"));
    datasourcebutton->setMinimumSize( QSize( 20, 21 ) );
    datasourcebutton->setMaximumSize( QSize( 20, 21 ) );

    tabLayout->addWidget( datasourcebutton, 1, 2 );

    datasourcelabel = new QLabel(tab_2);
    datasourcelabel->setObjectName("datasourcelabel");
    initSizePolicy(datasourcelabel, QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    tabLayout->addWidget( datasourcelabel, 1, 0 );

    datasourcefield = new QComboBox(tab_2);
    datasourcefield->setObjectName(QString::fromAscii("datasourcefield"));
    initSizePolicy(datasourcefield, QSizePolicy::Preferred, QSizePolicy::Fixed);
    
    datasourcefield->setMinimumSize( QSize( 0, 0 ) );

    tabLayout->addWidget( datasourcefield, 1, 1 );

    columnfield = new QComboBox(tab_2);
    columnfield->setObjectName(QString::fromAscii("columnfield"));
    initSizePolicy(columnfield, QSizePolicy::Preferred, QSizePolicy::Fixed);
    columnfield->setEditable( TRUE );
    columnfield->setAutoCompletion( TRUE );
    columnfield->setDuplicatesEnabled( FALSE );

    tabLayout->addWidget( columnfield, 2, 1 );

    tooltiplabel = new QLabel(tab_2);
    tooltiplabel->setObjectName("tooltiplabel");
    initSizePolicy(tooltiplabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( tooltiplabel, 5, 0 );

    tooltipfield = new QLineEdit(tab_2);
    tooltipfield->setObjectName("tooltipfield");

    tabLayout->addWidget( tooltipfield, 5, 1 );

    columnlabel = new QLabel(tab_2);
    columnlabel->setObjectName("columnlabel");
    initSizePolicy(columnlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( columnlabel, 2, 0 );

    identifierlabel = new QLabel(tab_2);
    identifierlabel->setObjectName("identifierlabel");
    initSizePolicy(identifierlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( identifierlabel, 3, 0 );

    Idlabel = new QLabel(tab_2);
    Idlabel->setObjectName("Idlabel");
    initSizePolicy(Idlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    tabLayout->addWidget( Idlabel, 0, 0 );

    identifierfield = new QLineEdit(tab_2);
    identifierfield->setObjectName("identifierfield");

    tabLayout->addWidget( identifierfield, 3, 1 );

    idfield = new QLineEdit(tab_2);
    idfield->setObjectName("idfield");
    idfield->setEnabled( FALSE );

    tabLayout->addWidget( idfield, 0, 1 );

    subformlabel = new QLabel(tab_2);
    subformlabel->setObjectName("subformlabel");
    initSizePolicy(subformlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    tabLayout->addWidget( subformlabel, 13, 0 );

    listcolumnfield = new QComboBox(tab_2);
    listcolumnfield->setObjectName(QString::fromAscii("listcolumnfield"));
    listcolumnfield->setEnabled( TRUE );
    listcolumnfield->setEditable( TRUE );
    listcolumnfield->setAutoCompletion( TRUE );

    tabLayout->addWidget( listcolumnfield, 11, 1 );

    listdatasourcefield = new QComboBox(tab_2);
    listdatasourcefield->setObjectName(QString::fromAscii("listdatasourcefield"));
    listdatasourcefield->setEnabled( TRUE );
    initSizePolicy(listdatasourcefield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout->addWidget( listdatasourcefield, 10, 1 );

    listcolumnlabel = new QLabel(tab_2);
    listcolumnlabel->setObjectName("listcolumnlabel");
    initSizePolicy(listcolumnlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( listcolumnlabel, 11, 0 );

    subformbutton = new QToolButton(tab_2);
    subformbutton->setObjectName(QString::fromAscii("subformbutton"));
    subformbutton->setMinimumSize( QSize( 20, 21 ) );
    subformbutton->setMaximumSize( QSize( 20, 21 ) );

    tabLayout->addWidget( subformbutton, 13, 2 );

    listdatasourcelabel = new QLabel(tab_2);
    listdatasourcelabel->setObjectName("listdatasourcelabel");
    initSizePolicy(listdatasourcelabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( listdatasourcelabel, 10, 0 );

    viewcolumnlabel = new QLabel(tab_2);
    viewcolumnlabel->setObjectName("viewcolumnlabel");
    initSizePolicy(viewcolumnlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( viewcolumnlabel, 12, 0 );

    viewcolumnfield = new QComboBox(tab_2);
    viewcolumnfield->setObjectName(QString::fromAscii("viewcolumnfield"));
    viewcolumnfield->setEnabled( TRUE );
    viewcolumnfield->setEditable( TRUE );
    viewcolumnfield->setAutoCompletion( TRUE );

    tabLayout->addWidget( viewcolumnfield, 12, 1 );

    subformfield = new QComboBox(tab_2);
    subformfield->setObjectName(QString::fromAscii("subformfield"));
    subformfield->setEnabled( TRUE );
    initSizePolicy(subformfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout->addWidget( subformfield, 13, 1 );

    readonlylabel = new QLabel(tab_2);
    readonlylabel->setObjectName("readonlylabel");
    initSizePolicy(readonlylabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( readonlylabel, 7, 0 );

    readonlyfield = new QComboBox(tab_2);
    readonlyfield->setObjectName(QString::fromAscii("readonlyfield"));
    initSizePolicy(readonlyfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout->addWidget( readonlyfield, 7, 1 );

    usetextlistfield = new QComboBox(tab_2);
    usetextlistfield->setObjectName(QString::fromAscii("usetextlistfield"));
    usetextlistfield->setEnabled( TRUE );
    initSizePolicy(usetextlistfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout->addWidget( usetextlistfield, 9, 1 );

    usetextlistlabel = new QLabel(tab_2);
    usetextlistlabel->setObjectName("usetextlistlabel");
    initSizePolicy(usetextlistlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout->addWidget( usetextlistlabel, 9, 0 );

    listdatasourcebutton = new QToolButton(tab_2);
    listdatasourcebutton->setObjectName(QString::fromAscii("listdatasourcebutton"));
    listdatasourcebutton->setMinimumSize( QSize( 20, 21 ) );
    listdatasourcebutton->setMaximumSize( QSize( 20, 21 ) );

    tabLayout->addWidget( listdatasourcebutton, 10, 2 );

    usetextlistbutton = new QToolButton(tab_2);
    usetextlistbutton->setObjectName(QString::fromAscii("usetextlistbutton"));
    usetextlistbutton->setEnabled( FALSE );
    usetextlistbutton->setMinimumSize( QSize( 20, 21 ) );
    usetextlistbutton->setMaximumSize( QSize( 20, 21 ) );

    tabLayout->addWidget( usetextlistbutton, 9, 2 );
    spacer9 = new QSpacerItem( 16, 20, QSizePolicy::Minimum, QSizePolicy::Minimum );
    tabLayout->addItem( spacer9, 1, 3 );
    Spacer4 = new QSpacerItem( 20, 190, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( Spacer4, 17, 1 );
    Spacer3 = new QSpacerItem( 20, 180, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout->addItem( Spacer3, 17, 0 ); 
    tab->addTab( tab_2, QString::fromLatin1("") );

    tab_3 = new QWidget(tab);
    tab_3->setObjectName(QString::fromAscii("tab_3"));
    tabLayout_2 = new QGridLayout(tab_3);
    tabLayout_2->setMargin(11);
    tabLayout_2->setSpacing(6);
    tabLayout_2->setObjectName(QString::fromAscii("tabLayout_2")); 

    fontsizefield = new QSpinBox( tab_3 );
    fontsizefield->setObjectName(QString::fromAscii("fontsizefield"));
    initSizePolicy(fontsizefield, QSizePolicy::Preferred, QSizePolicy::Fixed);
    fontsizefield->setValue( 10 );

    tabLayout_2->addWidget( fontsizefield, 5, 1 );

    boldfield = new QComboBox(tab_3);
    boldfield->setObjectName(QString::fromAscii("boldfield"));
    initSizePolicy(boldfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( boldfield, 6, 1 );

    italicfield = new QComboBox(tab_3);
    italicfield->setObjectName(QString::fromAscii("italicfield"));
    initSizePolicy(italicfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( italicfield, 7, 1 );

    separatorfield = new QComboBox(tab_3);
    separatorfield->setObjectName(QString::fromAscii("separatorfield"));
    initSizePolicy(separatorfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( separatorfield, 10, 1 );

    p_backgroundcolourbutton = new KColorButton( tab_3 );
    p_backgroundcolourbutton->setObjectName( "p_backgroundcolourbutton" );
    initSizePolicy(p_backgroundcolourbutton, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( p_backgroundcolourbutton, 12, 1 );

    fontfield = new QComboBox(tab_3);
    fontfield->setObjectName(QString::fromAscii("fontfield"));
    initSizePolicy(fontfield, QSizePolicy::Preferred, QSizePolicy::Fixed);
    fontfield->setMaximumSize( QSize( 300, 32767 ) );
    fontfield->setEditable( TRUE );
    fontfield->setAutoCompletion( TRUE );
    fontfield->setDuplicatesEnabled( FALSE );

    tabLayout_2->addWidget( fontfield, 4, 1 );

    xlabel = new QLabel(tab_3);
    xlabel->setObjectName("xlabel");
    initSizePolicy(xlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout_2->addWidget( xlabel, 0, 0 );

    boldlabel = new QLabel(tab_3);
    boldlabel->setObjectName("boldlabel");

    tabLayout_2->addWidget( boldlabel, 6, 0 );

    p_backgroundcolourlabel = new QLabel(tab_3);
    p_backgroundcolourlabel->setObjectName("p_backgroundcolourlabel");

    tabLayout_2->addWidget( p_backgroundcolourlabel, 12, 0 );

    separatorlabel = new QLabel(tab_3);
    separatorlabel->setObjectName("separatorlabel");
    initSizePolicy(separatorlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout_2->addWidget( separatorlabel, 10, 0 );

    yfield = new hk_kdespinbox( tab_3, "yfield" );
    initSizePolicy(yfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( yfield, 1, 1 );

    heightfield = new hk_kdespinbox( tab_3, "heightfield" );
    initSizePolicy(heightfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( heightfield, 3, 1 );

    fontsizelabel = new QLabel(tab_3);
    fontsizelabel->setObjectName("fontsizelabel");

    tabLayout_2->addWidget( fontsizelabel, 5, 0 );

    xfield = new hk_kdespinbox( tab_3, "xfield" );
    initSizePolicy(xfield, QSizePolicy::Preferred, QSizePolicy::Fixed);
    xfield->setMinimumSize( QSize( 0, 0 ) );

    tabLayout_2->addWidget( xfield, 0, 1 );

    fontlabel = new QLabel(tab_3);
    fontlabel->setObjectName("fontlabel");

    tabLayout_2->addWidget( fontlabel, 4, 0 );

    heightlabel = new QLabel(tab_3);
    heightlabel->setObjectName("heightlabel");

    tabLayout_2->addWidget( heightlabel, 3, 0 );

    ylabel = new QLabel(tab_3);
    ylabel->setObjectName("ylabel");
    initSizePolicy(ylabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout_2->addWidget( ylabel, 1, 0 );

    widthfield = new hk_kdespinbox( tab_3, "widthfield" );
    initSizePolicy(widthfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( widthfield, 2, 1 );

    p_foregroundcolourbutton = new KColorButton( tab_3 );
    p_foregroundcolourbutton->setObjectName( "p_foregroundcolourbutton" );
    initSizePolicy(p_foregroundcolourbutton, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( p_foregroundcolourbutton, 11, 1 );

    p_foregroundcolourlabel = new QLabel(tab_3);
    p_foregroundcolourlabel->setObjectName("p_foregroundcolourlabel");

    tabLayout_2->addWidget( p_foregroundcolourlabel, 11, 0 );

    widthlabel = new QLabel(tab_3);
    widthlabel->setObjectName("widthlabel");

    tabLayout_2->addWidget( widthlabel, 2, 0 );

    italiclabel = new QLabel(tab_3);
    italiclabel->setObjectName("italiclabel");

    tabLayout_2->addWidget( italiclabel, 7, 0 );

    digitlabel = new QLabel(tab_3);
    digitlabel->setObjectName("digitlabel");
    initSizePolicy(digitlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout_2->addWidget( digitlabel, 9, 0 );

    digitfield = new QSpinBox( tab_3 );
    digitfield->setObjectName(QString::fromAscii("digitfield"));
    initSizePolicy(digitfield, QSizePolicy::Preferred, QSizePolicy::Fixed);
    digitfield->setMaximum( 15 );

    tabLayout_2->addWidget( digitfield, 9, 1 );

    scalelabel = new QLabel(tab_3);
    scalelabel->setObjectName("scalelabel");
    initSizePolicy(scalelabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout_2->addWidget( scalelabel, 13, 0 );

    scalefield = new QSpinBox( tab_3 );
    scalefield->setObjectName(QString::fromAscii("scalefield"));
    initSizePolicy(scalefield, QSizePolicy::Preferred, QSizePolicy::Fixed);
    scalefield->setMaximum( 500 );
    scalefield->setSingleStep( 25 );

    tabLayout_2->addWidget( scalefield, 13, 1 );
    spacer10 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_2->addItem( spacer10, 7, 2 );

    alignmentlabel = new QLabel(tab_3);
    alignmentlabel->setObjectName("alignmentlabel");
    initSizePolicy(alignmentlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout_2->addWidget( alignmentlabel, 8, 0 );

    alignmentfield = new QComboBox(tab_3);
    alignmentfield->setObjectName(QString::fromAscii("alignmentfield"));
    initSizePolicy(alignmentfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( alignmentfield, 8, 1 );

    enabledlabel = new QLabel(tab_3);
    enabledlabel->setObjectName("enabledlabel");
    initSizePolicy(enabledlabel, QSizePolicy::Preferred, QSizePolicy::Preferred);

    tabLayout_2->addWidget( enabledlabel, 14, 0 );
    Spacer1 = new QSpacerItem( 20, 180, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout_2->addItem( Spacer1, 18, 0 );
    Spacer2 = new QSpacerItem( 20, 200, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout_2->addItem( Spacer2, 18, 1 );

    localimagelabel = new QLabel(tab_3);
    localimagelabel->setObjectName("localimagelabel");

    tabLayout_2->addWidget( localimagelabel, 15, 0 );

    localimagebutton = new QPushButton(tab_3);
    localimagebutton->setObjectName("localimagebutton");

    tabLayout_2->addWidget( localimagebutton, 15, 1 );

    enabledfield = new QComboBox(tab_3);
    enabledfield->setObjectName(QString::fromAscii("enabledfield"));
    initSizePolicy(enabledfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( enabledfield, 14, 1 );

    istogglebuttonlabel = new QLabel(tab_3);
    istogglebuttonlabel->setObjectName("istogglebuttonlabel");

    tabLayout_2->addWidget( istogglebuttonlabel, 16, 0 );

    istogglebuttonfield = new QComboBox(tab_3);
    istogglebuttonfield->setObjectName(QString::fromAscii("istogglebuttonfield"));
    initSizePolicy(istogglebuttonfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( istogglebuttonfield, 16, 1 );

    togglepushedlabel = new QLabel(tab_3);
    togglepushedlabel->setObjectName("togglepushedlabel");

    tabLayout_2->addWidget( togglepushedlabel, 17, 0 );

    togglepushedfield = new QComboBox(tab_3);
    togglepushedfield->setObjectName(QString::fromAscii("togglepushedfield"));
    initSizePolicy(togglepushedfield, QSizePolicy::Preferred, QSizePolicy::Fixed);

    tabLayout_2->addWidget( togglepushedfield, 17, 1 );
    tab->addTab( tab_3, QString::fromLatin1("") );

    frametab = new QWidget(tab);
    frametab->setObjectName(QString::fromAscii("frametab"));
    frametabLayout = new QGridLayout(frametab);
    frametabLayout->setMargin(11);
    frametabLayout->setSpacing(6);
    frametabLayout->setObjectName(QString::fromAscii("frametabLayout")); 
    spacer11 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    frametabLayout->addItem( spacer11, 0, 1 );

    layout3 = new QGridLayout(static_cast<QWidget*>(NULL));
    layout3->setMargin(0);
    layout3->setSpacing(6);
    layout3->setObjectName(QString::fromAscii("layout3")); 
    Spacer2_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout3->addItem( Spacer2_2, 6, 1 );
    Spacer1_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    layout3->addItem( Spacer1_2, 6, 0 );

    leftlinefield = new hk_kdespinbox( frametab, "leftlinefield" );
    initSizePolicy(leftlinefield, QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout3->addWidget( leftlinefield, 2, 1 );

    toplinelabel = new QLabel(frametab);
    toplinelabel->setObjectName("toplinelabel");

    layout3->addWidget( toplinelabel, 0, 0 );

    bottomlinelabel = new QLabel(frametab);
    bottomlinelabel->setObjectName("bottomlinelabel");

    layout3->addWidget( bottomlinelabel, 1, 0 );

    diagonallurolabel = new QLabel(frametab);
    diagonallurolabel->setObjectName("diagonallurolabel");

    layout3->addWidget( diagonallurolabel, 4, 0 );

    rightlinefield = new hk_kdespinbox( frametab, "rightlinefield" );
    initSizePolicy(rightlinefield, QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout3->addWidget( rightlinefield, 3, 1 );

    diagonallorulabel = new QLabel(frametab);
    diagonallorulabel->setObjectName("diagonallorulabel");

    layout3->addWidget( diagonallorulabel, 5, 0 );

    toplinefield = new hk_kdespinbox( frametab, "toplinefield" );
    initSizePolicy(toplinefield, QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout3->addWidget( toplinefield, 0, 1 );

    diagonallurofield = new hk_kdespinbox( frametab, "diagonallurofield" );
    initSizePolicy(diagonallurofield, QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout3->addWidget( diagonallurofield, 4, 1 );

    bottomlinefield = new hk_kdespinbox( frametab, "bottomlinefield" );
    initSizePolicy(bottomlinefield, QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout3->addWidget( bottomlinefield, 1, 1 );

    leftlinelabel = new QLabel(frametab);
    leftlinelabel->setObjectName("leftlinelabel");

    layout3->addWidget( leftlinelabel, 2, 0 );

    rightlinelabel = new QLabel(frametab);
    rightlinelabel->setObjectName("rightlinelabel");

    layout3->addWidget( rightlinelabel, 3, 0 );

    diagonallorufield = new hk_kdespinbox( frametab, "diagonallorufield" );
    initSizePolicy(diagonallorufield, QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout3->addWidget( diagonallorufield, 5, 1 );

    frametabLayout->addLayout( layout3, 0, 0 );
    tab->addTab( frametab, QString::fromLatin1("") );

    tab_4 = new QWidget(tab);
    tab_4->setObjectName(QString::fromAscii("tab_4"));
    tabLayout_3 = new QGridLayout(tab_4);
    tabLayout_3->setMargin(11);
    tabLayout_3->setSpacing(6);
    tabLayout_3->setObjectName(QString::fromAscii("tabLayout_3")); 
    spacer12 = new QSpacerItem( 16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    tabLayout_3->addItem(spacer12, 8, 3, 2, 1);

    actionlabel = new QLabel(tab_4);
    actionlabel->setObjectName("actionlabel");

    tabLayout_3->addWidget( actionlabel, 0, 0 );

    objectlabel = new QLabel(tab_4);
    objectlabel->setObjectName("objectlabel");

    tabLayout_3->addWidget( objectlabel, 1, 0 );

    objectfield = new QComboBox(tab_4);
    objectfield->setObjectName(QString::fromAscii("objectfield"));

    tabLayout_3->addWidget( objectfield, 1, 1 );

    actionfield = new QComboBox(tab_4);
    actionfield->setObjectName(QString::fromAscii("actionfield"));
    actionfield->setMinimumSize( QSize( 0, 0 ) );

    tabLayout_3->addWidget( actionfield, 0, 1 );

    showmaximizedfield = new QComboBox(tab_4);
    showmaximizedfield->setObjectName(QString::fromAscii("showmaximizedfield"));
    showmaximizedfield->setDuplicatesEnabled( FALSE );

    tabLayout_3->addWidget( showmaximizedfield, 2, 1 );

    conditionbutton = new QPushButton(tab_4);
    conditionbutton->setObjectName("conditionbutton");
    conditionbutton->setEnabled( TRUE );
    initSizePolicy(conditionbutton, QSizePolicy::Minimum, QSizePolicy::Minimum);
    conditionbutton->setMinimumSize( QSize( 20, 21 ) );
    conditionbutton->setMaximumSize( QSize( 20, 21 ) );

    tabLayout_3->addWidget( conditionbutton, 1, 2 );

    showmaximizedlabel = new QLabel(tab_4);
    showmaximizedlabel->setObjectName("showmaximizedlabel");

    tabLayout_3->addWidget( showmaximizedlabel, 2, 0 );

    onpushactionbutton = new QPushButton(tab_4);
    onpushactionbutton->setObjectName("onpushactionbutton");

    tabLayout_3->addWidget( onpushactionbutton, 3, 1 );

    pushactionlabel = new QLabel(tab_4);
    pushactionlabel->setObjectName("pushactionlabel");

    tabLayout_3->addWidget( pushactionlabel, 3, 0 );

    doubleclickactionlabel = new QLabel(tab_4);
    doubleclickactionlabel->setObjectName("doubleclickactionlabel");

    tabLayout_3->addWidget( doubleclickactionlabel, 4, 0 );

    ondoubleclickactionbutton = new QPushButton(tab_4);
    ondoubleclickactionbutton->setObjectName("ondoubleclickactionbutton");

    tabLayout_3->addWidget( ondoubleclickactionbutton, 4, 1 );

    ongetfocuslabel = new QLabel(tab_4);
    ongetfocuslabel->setObjectName("ongetfocuslabel");

    tabLayout_3->addWidget( ongetfocuslabel, 5, 0 );

    ongetfocusbutton = new QPushButton(tab_4);
    ongetfocusbutton->setObjectName("ongetfocusbutton");

    tabLayout_3->addWidget( ongetfocusbutton, 5, 1 );

    beforedeletelabel = new QLabel(tab_4);
    beforedeletelabel->setObjectName("beforedeletelabel");

    tabLayout_3->addWidget( beforedeletelabel, 14, 0 );

    oncloseactionbutton = new QPushButton(tab_4);
    oncloseactionbutton->setObjectName("oncloseactionbutton");

    tabLayout_3->addWidget( oncloseactionbutton, 9, 1 );

    beforeupdateactionbutton = new QPushButton(tab_4);
    beforeupdateactionbutton->setObjectName("beforeupdateactionbutton");

    tabLayout_3->addWidget( beforeupdateactionbutton, 10, 1 );

    afterupdateactionbutton = new QPushButton(tab_4);
    afterupdateactionbutton->setObjectName("afterupdateactionbutton");

    tabLayout_3->addWidget( afterupdateactionbutton, 11, 1 );

    afterupdatelabel = new QLabel(tab_4);
    afterupdatelabel->setObjectName("afterupdatelabel");

    tabLayout_3->addWidget( afterupdatelabel, 11, 0 );

    beforeupdateactionlabel = new QLabel(tab_4);
    beforeupdateactionlabel->setObjectName("beforeupdateactionlabel");

    tabLayout_3->addWidget( beforeupdateactionlabel, 10, 0 );

    afterdeletelabel = new QLabel(tab_4);
    afterdeletelabel->setObjectName("afterdeletelabel");

    tabLayout_3->addWidget( afterdeletelabel, 15, 0 );

    afterrowchangelabel = new QLabel(tab_4);
    afterrowchangelabel->setObjectName("afterrowchangelabel");

    tabLayout_3->addWidget( afterrowchangelabel, 13, 0 );

    afterrowchangeactionbutton = new QPushButton(tab_4);
    afterrowchangeactionbutton->setObjectName("afterrowchangeactionbutton");

    tabLayout_3->addWidget( afterrowchangeactionbutton, 13, 1 );

    beforeinsertlabel = new QLabel(tab_4);
    beforeinsertlabel->setObjectName("beforeinsertlabel");

    tabLayout_3->addWidget( beforeinsertlabel, 16, 0 );

    beforeinsertactionbutton = new QPushButton(tab_4);
    beforeinsertactionbutton->setObjectName("beforeinsertactionbutton");

    tabLayout_3->addWidget( beforeinsertactionbutton, 16, 1 );

    afterdeleteactionbutton = new QPushButton(tab_4);
    afterdeleteactionbutton->setObjectName("afterdeleteactionbutton");

    tabLayout_3->addWidget( afterdeleteactionbutton, 15, 1 );

    closeactionlabel = new QLabel(tab_4);
    closeactionlabel->setObjectName("closeactionlabel");

    tabLayout_3->addWidget( closeactionlabel, 9, 0 );

    beforedeleteactionbutton = new QPushButton(tab_4);
    beforedeleteactionbutton->setObjectName("beforedeleteactionbutton");

    tabLayout_3->addWidget( beforedeleteactionbutton, 14, 1 );

    onopenactionbutton = new QPushButton(tab_4);
    onopenactionbutton->setObjectName("onopenactionbutton");

    tabLayout_3->addWidget( onopenactionbutton, 8, 1 );

    beforerowchangeactionbutton = new QPushButton(tab_4);
    beforerowchangeactionbutton->setObjectName("beforerowchangeactionbutton");

    tabLayout_3->addWidget( beforerowchangeactionbutton, 12, 1 );

    beforerowchangelabel = new QLabel(tab_4);
    beforerowchangelabel->setObjectName("beforerowchangelabel");

    tabLayout_3->addWidget( beforerowchangelabel, 12, 0 );

    openactionlabel = new QLabel(tab_4);
    openactionlabel->setObjectName("openactionlabel");

    tabLayout_3->addWidget( openactionlabel, 8, 0 );

    onloosefocuslabel = new QLabel(tab_4);
    onloosefocuslabel->setObjectName("onloosefocuslabel");

    tabLayout_3->addWidget( onloosefocuslabel, 6, 0 );

    onloosefocusbutton = new QPushButton(tab_4);
    onloosefocusbutton->setObjectName("onloosefocusbutton");

    tabLayout_3->addWidget( onloosefocusbutton, 6, 1 );

    onkeybutton = new QPushButton(tab_4);
    onkeybutton->setObjectName("onkeybutton");

    tabLayout_3->addWidget( onkeybutton, 7, 1 );

    onkeylabel = new QLabel(tab_4);
    onkeylabel->setObjectName("onkeylabel");

    tabLayout_3->addWidget( onkeylabel, 7, 0 );
    Spacer6 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout_3->addItem( Spacer6, 20, 0 );
    Spacer7 = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    tabLayout_3->addItem( Spacer7, 20, 1 );

    onselectlabel = new QLabel(tab_4);
    onselectlabel->setObjectName("onselectlabel");

    tabLayout_3->addWidget( onselectlabel, 19, 0 );

    onselectactionbutton = new QPushButton(tab_4);
    onselectactionbutton->setObjectName("onselectactionbutton");

    tabLayout_3->addWidget( onselectactionbutton, 19, 1 );

    afterinsertlabel = new QLabel(tab_4);
    afterinsertlabel->setObjectName("afterinsertlabel");

    tabLayout_3->addWidget( afterinsertlabel, 17, 0 );

    afterinsertactionbutton = new QPushButton(tab_4);
    afterinsertactionbutton->setObjectName("afterinsertactionbutton");

    tabLayout_3->addWidget( afterinsertactionbutton, 17, 1 );

    onvaluechangedactionlabel = new QLabel(tab_4);
    onvaluechangedactionlabel->setObjectName("onvaluechangedactionlabel");

    tabLayout_3->addWidget( onvaluechangedactionlabel, 18, 0 );

    onvaluechangedactionbutton = new QPushButton(tab_4);
    onvaluechangedactionbutton->setObjectName("onvaluechangedactionbutton");

    tabLayout_3->addWidget( onvaluechangedactionbutton, 18, 1 );
    tab->addTab( tab_4, QString::fromLatin1("") ); 

    hk_kdepropertyeditorbaseLayout->addWidget( tab, 0, 0 );
     languageChange();
    resize( QSize(426, 956).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( actionfield, SIGNAL( activated(int) ), this, SLOT( action_changes() ) );
    connect( afterdeleteactionbutton, SIGNAL( clicked() ), this, SLOT( afterdeletebutton_clicked() ) );
    connect( afterinsertactionbutton, SIGNAL( clicked() ), this, SLOT( afterinsertbutton_clicked() ) );
    connect( afterrowchangeactionbutton, SIGNAL( clicked() ), this, SLOT( afterrowchangebutton_clicked() ) );
    connect( afterupdateactionbutton, SIGNAL( clicked() ), this, SLOT( afterupdatebutton_clicked() ) );
    connect( alignmentfield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( beforedeleteactionbutton, SIGNAL( clicked() ), this, SLOT( beforedeletebutton_clicked() ) );
    connect( beforeinsertactionbutton, SIGNAL( clicked() ), this, SLOT( beforeinsertbutton_clicked() ) );
    connect( beforerowchangeactionbutton, SIGNAL( clicked() ), this, SLOT( beforerowchangebutton_clicked() ) );
    connect( beforeupdateactionbutton, SIGNAL( clicked() ), this, SLOT( beforeupdatebutton_clicked() ) );
    connect( boldfield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( bottomlinefield, SIGNAL( valueChanged(int) ), this, SLOT( border_changes() ) );
    connect( columnfield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
    connect( columnfield, SIGNAL( textChanged(const QString&) ), this, SLOT( data_changes() ) );
    connect( combomodefield, SIGNAL( activated(int) ), this, SLOT( combomode_changed() ) );
    connect( combomodefield, SIGNAL( activated(int) ), this, SLOT( data_changes() ) );
    connect( conditionbutton, SIGNAL( clicked() ), this, SLOT( conditionbutton_clicked() ) );
    connect( datasourcebutton, SIGNAL( clicked() ), this, SLOT( add_formdatasource() ) );
    connect( datasourcefield, SIGNAL( textChanged(const QString&) ), this, SLOT( datasource_selected() ) );
    connect( datasourcefield, SIGNAL( activated(int) ), this, SLOT( datasource_selected() ) );
    connect( datasourcefield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
    connect( defaultfield, SIGNAL( textChanged(const QString&) ), this, SLOT( data_changes() ) );
    connect( diagonallorufield, SIGNAL( valueChanged(int) ), this, SLOT( border_changes() ) );
    connect( diagonallurofield, SIGNAL( valueChanged(int) ), this, SLOT( border_changes() ) );
    connect( digitfield, SIGNAL( valueChanged(int) ), this, SLOT( data_changes() ) );
    connect( fontfield, SIGNAL( textChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( fontsizefield, SIGNAL( valueChanged(int) ), this, SLOT( format_changes() ) );
    connect( heightfield, SIGNAL( valueChanged(int) ), this, SLOT( geometry_changes() ) );
    connect( identifierfield, SIGNAL( returnPressed() ), this, SLOT( identifier_changed() ) );
    connect( italicfield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( labelfield, SIGNAL( textChanged(const QString&) ), this, SLOT( format_changes() ) );
    connect( leftlinefield, SIGNAL( valueChanged(int) ), this, SLOT( border_changes() ) );
    connect( listcolumnfield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
    connect( listcolumnfield, SIGNAL( textChanged(const QString&) ), this, SLOT( data_changes() ) );
    connect( listdatasourcebutton, SIGNAL( clicked() ), this, SLOT( add_listformdatasource() ) );
    connect( listdatasourcefield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
    connect( listdatasourcefield, SIGNAL( activated(const QString&) ), this, SLOT( listdatasource_selected() ) );
    connect( listdatasourcefield, SIGNAL( textChanged(const QString&) ), this, SLOT( listdatasource_selected() ) );
    connect( listdatasourcefield, SIGNAL( textChanged(const QString&) ), this, SLOT( data_changes() ) );
    connect( objectfield, SIGNAL( activated(int) ), this, SLOT( actionobject_changes() ) );
    connect( oncloseactionbutton, SIGNAL( clicked() ), this, SLOT( closeactionbutton_clicked() ) );
    connect( ondoubleclickactionbutton, SIGNAL( clicked() ), this, SLOT( doubleclickactionbutton_clicked() ) );
    connect( onopenactionbutton, SIGNAL( clicked() ), this, SLOT( openactionbutton_clicked() ) );
    connect( onpushactionbutton, SIGNAL( clicked() ), this, SLOT( pushactionbutton_clicked() ) );
    connect( readonlyfield, SIGNAL( activated(int) ), this, SLOT( data_changes() ) );
    connect( rightlinefield, SIGNAL( valueChanged(int) ), this, SLOT( border_changes() ) );
    connect( separatorfield, SIGNAL( activated(int) ), this, SLOT( data_changes() ) );
    connect( showmaximizedfield, SIGNAL( activated(int) ), this, SLOT( actionobject_changes() ) );
    connect( subformbutton, SIGNAL( clicked() ), this, SLOT( subformbutton_clicked() ) );
    connect( subformfield, SIGNAL( textChanged(const QString&) ), this, SLOT( data_changes() ) );
    connect( toplinefield, SIGNAL( valueChanged(int) ), this, SLOT( border_changes() ) );
    connect( viewcolumnfield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
    connect( viewcolumnfield, SIGNAL( textChanged(const QString&) ), this, SLOT( data_changes() ) );
    connect( widthfield, SIGNAL( valueChanged(int) ), this, SLOT( geometry_changes() ) );
    connect( xfield, SIGNAL( valueChanged(int) ), this, SLOT( geometry_changes() ) );
    connect( yfield, SIGNAL( valueChanged(int) ), this, SLOT( geometry_changes() ) );
    connect( subformfield, SIGNAL( activated(int) ), this, SLOT( data_changes() ) );
    connect( ongetfocusbutton, SIGNAL( clicked() ), this, SLOT( ongetfocusbutton_clicked() ) );
    connect( onloosefocusbutton, SIGNAL( clicked() ), this, SLOT( onloosefocusbutton_clicked() ) );
    connect( tooltipfield, SIGNAL( textChanged(const QString&) ), this, SLOT( tooltipfield_changed() ) );
    connect( onkeybutton, SIGNAL( clicked() ), this, SLOT( onkeybutton_clicked() ) );
    connect( usetextlistfield, SIGNAL( activated(int) ), this, SLOT( data_changes() ) );
    connect( usetextlistbutton, SIGNAL( clicked() ), this, SLOT( use_textlistbutton_clicked() ) );
    connect( onselectactionbutton, SIGNAL( clicked() ), this, SLOT( onselectbutton_clicked() ) );
    connect( scalefield, SIGNAL( valueChanged(int) ), this, SLOT( format_changes() ) );
    connect( identifierfield, SIGNAL( textChanged(const QString&) ), this, SLOT( has_changed() ) );
    connect( onvaluechangedactionbutton, SIGNAL( clicked() ), this, SLOT( onvaluechangedbutton_clicked() ) );
    connect( enabledfield, SIGNAL( activated(int) ), this, SLOT( format_changes() ) );
    connect( istogglebuttonfield, SIGNAL( activated(int) ), this, SLOT( buttonformat_changed() ) );
    connect( togglepushedfield, SIGNAL( activated(int) ), this, SLOT( buttonformat_changed() ) );
    connect( localimagebutton, SIGNAL( clicked() ), this, SLOT( localimage_selected() ) );
    connect( istogglebuttonfield, SIGNAL( activated(int) ), this, SLOT( buttonformat_changed() ) );
    connect( togglepushedfield, SIGNAL( activated(int) ), this, SLOT( buttonformat_changed() ) );

    // tab order
    setTabOrder( tab_4, datasourcefield );
    setTabOrder( datasourcefield, columnfield );
    setTabOrder( columnfield, identifierfield );
    setTabOrder( identifierfield, labelfield );
    setTabOrder( labelfield, tooltipfield );
    setTabOrder( tooltipfield, defaultfield );
    setTabOrder( defaultfield, readonlyfield );
    setTabOrder( readonlyfield, combomodefield );
    setTabOrder( combomodefield, listdatasourcefield );
    setTabOrder( listdatasourcefield, listcolumnfield );
    setTabOrder( listcolumnfield, viewcolumnfield );
    setTabOrder( viewcolumnfield, subformfield );
    setTabOrder( subformfield, fontfield );
    setTabOrder( fontfield, fontsizefield );
    setTabOrder( fontsizefield, boldfield );
    setTabOrder( boldfield, italicfield );
    setTabOrder( italicfield, alignmentfield );
    setTabOrder( alignmentfield, digitfield );
    setTabOrder( digitfield, separatorfield );
    setTabOrder( separatorfield, actionfield );
    setTabOrder( actionfield, objectfield );
    setTabOrder( objectfield, conditionbutton );
    setTabOrder( conditionbutton, showmaximizedfield );
    setTabOrder( showmaximizedfield, onpushactionbutton );
    setTabOrder( onpushactionbutton, ondoubleclickactionbutton );
    setTabOrder( ondoubleclickactionbutton, onopenactionbutton );
    setTabOrder( onopenactionbutton, oncloseactionbutton );
    setTabOrder( oncloseactionbutton, beforeupdateactionbutton );
    setTabOrder( beforeupdateactionbutton, afterupdateactionbutton );
    setTabOrder( afterupdateactionbutton, beforerowchangeactionbutton );
    setTabOrder( beforerowchangeactionbutton, afterrowchangeactionbutton );
    setTabOrder( afterrowchangeactionbutton, beforedeleteactionbutton );
    setTabOrder( beforedeleteactionbutton, afterdeleteactionbutton );
    setTabOrder( afterdeleteactionbutton, beforeinsertactionbutton );
    setTabOrder( beforeinsertactionbutton, afterinsertactionbutton );
    setTabOrder( afterinsertactionbutton, p_backgroundcolourbutton );
    setTabOrder( p_backgroundcolourbutton, p_foregroundcolourbutton );
    setTabOrder( p_foregroundcolourbutton, ongetfocusbutton );
    setTabOrder( ongetfocusbutton, onloosefocusbutton ); 
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdepropertyeditorbase::~hk_kdepropertyeditorbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdepropertyeditorbase::languageChange()
{
    setWindowTitle( tr( "Property Editor" ) );
    combomodelabel->setText( tr( "Combomode:" ) );
    labellabel->setText( tr( "Label:" ) );
    defaultlabel->setText( tr( "Default value:" ) );
    datasourcebutton->setText( tr( "..." ) );
    datasourcelabel->setText( tr( "Datasource:" ) );
    tooltiplabel->setText( tr( "Tooltip:" ) );
    columnlabel->setText( tr( "Column:" ) );
    identifierlabel->setText( tr( "Identifier:" ) );
    Idlabel->setText( tr( "Id:" ) );
    subformlabel->setText( tr( "Subform:" ) );
    listcolumnlabel->setText( tr( "Listcolumn:" ) );
    subformbutton->setText( tr( "..." ) );
    listdatasourcelabel->setText( tr( "Listdatasource:" ) );
    viewcolumnlabel->setText( tr( "Viewcolumn:" ) );
    readonlylabel->setText( tr( "Readonly:" ) );
    usetextlistlabel->setText( tr( "Use textlist:" ) );
    listdatasourcebutton->setText( tr( "..." ) );
    usetextlistbutton->setText( tr( "..." ) );
    tab->setTabText( tab->indexOf(tab_2), tr( "&Data" ) );
    p_backgroundcolourbutton->setText( QString::null );
    xlabel->setText( tr( "X:" ) );
    boldlabel->setText( tr( "Bold:" ) );
    p_backgroundcolourlabel->setText( tr( "Backgroundcolour:" ) );
    separatorlabel->setText( tr( "TextLabel2" ) );
    fontsizelabel->setText( tr( "Fontsize:" ) );
    fontlabel->setText( tr( "Font:" ) );
    heightlabel->setText( tr( "Height:" ) );
    ylabel->setText( tr( "Y:" ) );
    p_foregroundcolourbutton->setText( QString::null );
    p_foregroundcolourlabel->setText( tr( "Foregroundcolour:" ) );
    widthlabel->setText( tr( "Width:" ) );
    italiclabel->setText( tr( "Italic:" ) );
    digitlabel->setText( tr( "TextLabel1" ) );
    scalelabel->setText( tr( "Scale:" ) );
    scalefield->setSuffix( tr( "%" ) );
    scalefield->setSpecialValueText( tr( "Fit to frame" ) );
    alignmentlabel->setText( tr( "Alignment:" ) );
    enabledlabel->setText( tr( "Enabled:" ) );
    localimagelabel->setText( tr( "Local image:" ) );
    localimagebutton->setText( QString::null );
    istogglebuttonlabel->setText( tr( "Is togglebutton:" ) );
    togglepushedlabel->setText( tr( "Is pushed:" ) );
    tab->setTabText( tab->indexOf(tab_3), tr( "&Format" ) );
    toplinelabel->setText( tr( "TopLine:" ) );
    bottomlinelabel->setText( tr( "BottomLine:" ) );
    diagonallurolabel->setText( tr( "DiagonalLURO:" ) );
    diagonallorulabel->setText( tr( "DiagonalLORU:" ) );
    leftlinelabel->setText( tr( "LeftLine:" ) );
    rightlinelabel->setText( tr( "RightLine:" ) );
    tab->setTabText( tab->indexOf(frametab), tr( "F&rame" ) );
    actionlabel->setText( tr( "Action" ) );
    objectlabel->setText( tr( "Object" ) );
    conditionbutton->setText( tr( "..." ) );
    showmaximizedlabel->setText( tr( "show maximized:" ) );
    onpushactionbutton->setText( QString::null );
    pushactionlabel->setText( tr( "On click:" ) );
    doubleclickactionlabel->setText( tr( "On doubleclick:" ) );
    ondoubleclickactionbutton->setText( QString::null );
    ongetfocuslabel->setText( tr( "On getfocus:" ) );
    ongetfocusbutton->setText( QString::null );
    beforedeletelabel->setText( tr( "Before delete:" ) );
    oncloseactionbutton->setText( QString::null );
    beforeupdateactionbutton->setText( QString::null );
    afterupdateactionbutton->setText( QString::null );
    afterupdatelabel->setText( tr( "After update:" ) );
    beforeupdateactionlabel->setText( tr( "Before update:" ) );
    afterdeletelabel->setText( tr( "After delete:" ) );
    afterrowchangelabel->setText( tr( "After row change:" ) );
    afterrowchangeactionbutton->setText( QString::null );
    beforeinsertlabel->setText( tr( "Before insert:" ) );
    beforeinsertactionbutton->setText( QString::null );
    afterdeleteactionbutton->setText( QString::null );
    closeactionlabel->setText( tr( "On close:" ) );
    beforedeleteactionbutton->setText( QString::null );
    onopenactionbutton->setText( QString::null );
    beforerowchangeactionbutton->setText( QString::null );
    beforerowchangelabel->setText( tr( "Before row change:" ) );
    openactionlabel->setText( tr( "On open:" ) );
    onloosefocuslabel->setText( tr( "On loosefocus:" ) );
    onloosefocusbutton->setText( QString::null );
    onkeybutton->setText( QString::null );
    onkeylabel->setText( tr( "On key:" ) );
    onselectlabel->setText( tr( "On select:" ) );
    onselectactionbutton->setText( QString::null );
    afterinsertlabel->setText( tr( "After insert:" ) );
    afterinsertactionbutton->setText( QString::null );
    onvaluechangedactionlabel->setText( tr( "On value changed:" ) );
    onvaluechangedactionbutton->setText( QString::null );
    tab->setTabText( tab->indexOf(tab_4), tr( "&Action" ) );
}

void hk_kdepropertyeditorbase::pushactionbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::pushactionbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::actionobject_changes()
{
    qWarning( "hk_kdepropertyeditorbase::actionobject_changes(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::add_formdatasource()
{
    qWarning( "hk_kdepropertyeditorbase::add_formdatasource(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::add_listformdatasource()
{
    qWarning( "hk_kdepropertyeditorbase::add_listformdatasource(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::combomode_changed()
{
    qWarning( "hk_kdepropertyeditorbase::combomode_changed(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::data_changes()
{
    qWarning( "hk_kdepropertyeditorbase::data_changes(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::datasource_selected()
{
    qWarning( "hk_kdepropertyeditorbase::datasource_selected(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::format_changes()
{
    qWarning( "hk_kdepropertyeditorbase::format_changes(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::listdatasource_selected()
{
    qWarning( "hk_kdepropertyeditorbase::listdatasource_selected(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::conditionbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::conditionbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::set_object(hk_visible*)
{
    qWarning( "hk_kdepropertyeditorbase::set_object(hk_visible*): Not implemented yet" );
}

void hk_kdepropertyeditorbase::openactionbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::openactionbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::closeactionbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::closeactionbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::beforeupdatebutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::beforeupdatebutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::afterupdatebutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::afterupdatebutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::beforedeletebutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::beforedeletebutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::afterdeletebutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::afterdeletebutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::beforerowchangebutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::beforerowchangebutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::afterrowchangebutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::afterrowchangebutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::action_changes()
{
    qWarning( "hk_kdepropertyeditorbase::action_changes(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::beforeinsertbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::beforeinsertbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::afterinsertbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::afterinsertbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::identifier_changed()
{
    qWarning( "hk_kdepropertyeditorbase::identifier_changed(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::doubleclickactionbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::doubleclickactionbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::border_changes()
{
    qWarning( "hk_kdepropertyeditorbase::border_changes(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::geometry_changes()
{
    qWarning( "hk_kdepropertyeditorbase::geometry_changes(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::subformbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::subformbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::ongetfocusbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::ongetfocusbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::onloosefocusbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::onloosefocusbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::tooltipfield_changed()
{
    qWarning( "hk_kdepropertyeditorbase::tooltipfield_changed(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::onkeybutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::onkeybutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::use_textlist_changed()
{
    qWarning( "hk_kdepropertyeditorbase::use_textlist_changed(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::use_textlistbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::use_textlistbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::onselectbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::onselectbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::localimage_selected()
{
    qWarning( "hk_kdepropertyeditorbase::localimage_selected(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::has_changed()
{
    qWarning( "hk_kdepropertyeditorbase::has_changed(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::onvaluechangedbutton_clicked()
{
    qWarning( "hk_kdepropertyeditorbase::onvaluechangedbutton_clicked(): Not implemented yet" );
}

void hk_kdepropertyeditorbase::buttonformat_changed()
{
    qWarning( "hk_kdepropertyeditorbase::buttonformat_changed(): Not implemented yet" );
}

