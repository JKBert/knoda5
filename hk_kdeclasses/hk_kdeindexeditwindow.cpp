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
//$Revision: 1.16 $

#include "hk_kdeindexeditwindow.h"
#include "hk_kdeindexeditwindow.moc"
#include <hk_database.h>
#include <hk_connection.h>

#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlistwidget.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <klocale.h>

/*
 *  Constructs a hk_kdeindexeditdialog which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
hk_kdeindexeditdialog::hk_kdeindexeditdialog( QWidget* parent,  const char* name, Qt::WFlags fl )
: QWidget( parent, fl ), hk_dsvisible()
{
    QIcon::setThemeName("oxygen");
    setObjectName( QString::fromAscii(name == NULL? "hk_kdeindexeditdialog":name ) );
    resize( 486, 245 );
    setWindowTitle( i18n( "Index Edit - Table:"  ) );
    hk_kdeindexeditdialogLayout = new QHBoxLayout( this );
    hk_kdeindexeditdialogLayout->setSpacing( 6 );
    hk_kdeindexeditdialogLayout->setMargin( 11 );

    Layout4 = new QVBoxLayout;
    Layout4->setSpacing( 6 );
    Layout4->setMargin( 0 );
    hk_kdeindexeditdialogLayout->addLayout( Layout4 );

    TextLabel1_2 = new QLabel(this);
    TextLabel1_2->setObjectName("TextLabel1_2");
    TextLabel1_2->setText( i18n( "Free fields:"  ));
    Layout4->addWidget( TextLabel1_2 );

    dsfieldlist = new QListWidget( this );
    dsfieldlist -> setObjectName( "dsfieldlist" );
    dsfieldlist->setMinimumSize( QSize( 150, 200 ) );
    Layout4->addWidget( dsfieldlist );

    Layout7 = new QVBoxLayout;
    Layout7->setSpacing( 6 );
    Layout7->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout7->addItem( spacer );

    Layout2 = new QVBoxLayout;
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 0 );

    addbutton = new QToolButton(this);
    addbutton->setObjectName(QString::fromAscii("addbutton"));
    addbutton->setIcon(QIcon::fromTheme("go-next"));
    Layout2->addWidget( addbutton );

    deletebutton = new QToolButton(this);
    deletebutton->setObjectName(QString::fromAscii("deletebutton"));
    deletebutton->setIcon(QIcon::fromTheme("go-previous"));
    deletebutton->setEnabled(false);
    Layout2->addWidget( deletebutton );
    Layout7->addLayout( Layout2 );
    QSpacerItem* spacer_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout7->addItem( spacer_2 );
    hk_kdeindexeditdialogLayout->addLayout( Layout7 );

    Layout5 = new QVBoxLayout;
    Layout5->setSpacing( 6 );
    Layout5->setMargin( 0 );

    TextLabel2_2 = new QLabel(this);
    TextLabel2_2->setObjectName("TextLabel2_2");
    TextLabel2_2->setText( i18n( "Used fields:"  ));
    Layout5->addWidget( TextLabel2_2 );

    indexfieldlist = new QListWidget( this );
    indexfieldlist -> setObjectName( "ListBox3" );
    indexfieldlist->setMinimumSize( QSize( 150, 200 ) );
    Layout5->addWidget( indexfieldlist );
    hk_kdeindexeditdialogLayout->addLayout( Layout5 );

    Layout13 = new QVBoxLayout;
    Layout13->setSpacing( 6 );
    Layout13->setMargin( 0 );

    Layout11 = new QVBoxLayout;
    Layout11->setSpacing( 6 );
    Layout11->setMargin( 0 );

    TextLabel1 = new QLabel(this);
    TextLabel1->setObjectName("TextLabel1");
    TextLabel1->setText( i18n( "Indexname:"  ));
    Layout11->addWidget( TextLabel1 );

    indexnamefield = new QLineEdit(this);
    indexnamefield->setObjectName("indexnamefield");
    Layout11->addWidget( indexnamefield );
    Layout13->addLayout( Layout11 );

    Layout12 = new QHBoxLayout;
    Layout12->setSpacing( 6 );
    Layout12->setMargin( 0 );

    TextLabel2 = new QLabel(this);
    TextLabel2->setObjectName("TextLabel2");
    TextLabel2->setText( i18n( "Unique"  ));
    Layout12->addWidget( TextLabel2 );

    uniquefield = new QCheckBox(this);
    uniquefield->setObjectName(QString::fromAscii("uniquefield"));
    QSizePolicy policy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    policy.setHeightForWidth(uniquefield->sizePolicy().hasHeightForWidth());
    uniquefield->setSizePolicy(policy);
    Layout12->addWidget( uniquefield );
    Layout13->addLayout( Layout12 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout13->addItem( spacer_3 );

    ButtonGroup7 = new QGroupBox( this );
    ButtonGroup7 -> setObjectName( "ButtonGroup7" );
    policy.setHorizontalPolicy(QSizePolicy::Maximum);
    policy.setVerticalPolicy(QSizePolicy::Preferred);
    policy.setHeightForWidth(ButtonGroup7->sizePolicy().hasHeightForWidth());
    ButtonGroup7->setSizePolicy(policy);
    ButtonGroup7->setMaximumSize( QSize( 150, 32767 ) );
    
    ButtonGroup7Layout = new QVBoxLayout( ButtonGroup7 );
    ButtonGroup7Layout->setAlignment( Qt::AlignTop );
    ButtonGroup7Layout->setSpacing( 6 );
    ButtonGroup7Layout->setMargin( 11 );

    storebutton = new QPushButton(ButtonGroup7);
    storebutton->setObjectName("storebutton");
    storebutton->setText( i18n( "OK"  ));
    storebutton->setEnabled( false );
    ButtonGroup7Layout->addWidget( storebutton );

    exitbutton = new QPushButton(ButtonGroup7);
    exitbutton->setObjectName("exitbutton");
    exitbutton->setText( i18n( "Exit"  ));
    ButtonGroup7Layout->addWidget( exitbutton );
    Layout13->addWidget( ButtonGroup7 );
    hk_kdeindexeditdialogLayout->addLayout( Layout13 );

// signals and slots connections
    connect( addbutton, SIGNAL( clicked() ), this, SLOT( addbutton_clicked() ) );
    connect( deletebutton, SIGNAL( clicked() ), this, SLOT( deletebutton_clicked() ) );
    connect( storebutton, SIGNAL( clicked() ), this, SLOT( okbutton_clicked() ) );
    connect( exitbutton, SIGNAL( clicked() ), this, SLOT( exitbutton_clicked() ) );
    connect( uniquefield, SIGNAL( stateChanged(int) ), this, SLOT( check_enablebuttons() ) );
    connect( indexnamefield, SIGNAL( textChanged(const QString&)), this, SLOT( check_enablebuttons() ) );
    connect( indexfieldlist, SIGNAL(itemSelectionChanged()), this, SLOT(listentries_changed()));
    connect( dsfieldlist, SIGNAL(itemSelectionChanged()), this, SLOT(listentries_changed()));
    p_changed=false;
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeindexeditdialog::~hk_kdeindexeditdialog()
{
// no need to delete child widgets, Qt does it all for us
}


void hk_kdeindexeditdialog::addbutton_clicked()
{
    indexfieldlist->insertItem(indexfieldlist->currentRow()+1,dsfieldlist->currentItem() -> text());
    QListWidgetItem* p_cur = dsfieldlist->takeItem(dsfieldlist->currentRow());
    delete p_cur;
    check_enablebuttons();
    listentries_changed();
}


void hk_kdeindexeditdialog::deletebutton_clicked()
{
    dsfieldlist->insertItem(dsfieldlist->currentRow()+1,indexfieldlist->currentItem()->text());   
    QListWidgetItem* p_cur = indexfieldlist->takeItem(indexfieldlist->currentRow());    
    delete p_cur;
    check_enablebuttons();
    listentries_changed() ;
}


void hk_kdeindexeditdialog::exitbutton_clicked()
{
    close();
}


void hk_kdeindexeditdialog::okbutton_clicked()
{
    list<hk_string> f;
    int n=0;
    while (n<indexfieldlist->count())
    {
      f.insert(f.end(),u2l(indexfieldlist->item(n)->text().toUtf8().data()));
      n++;
    }
    if (datasource()!=NULL)

    {
        if (datasource()->alter_index(u2l(indexnamefield->text().toUtf8().data()),uniquefield->isChecked(),f))  
            close();
        else
        {
            hk_string reason=hk_translate("Index could not be altered")+"\n"+hk_translate("Servermessage: ")+datasource()->database()->connection()->last_servermessage();
            show_warningmessage(reason);
        }
    }
    else close();
}


void hk_kdeindexeditdialog::check_enablebuttons(void)
{
    storebutton->setEnabled( indexnamefield->text().length()>0 && indexfieldlist->count()>0);
    p_changed=true;
}


void hk_kdeindexeditdialog::set_indexvalues(const hk_string& name,bool unique,list<hk_string>& fields,bool newindex)
{
    indexnamefield->setText(QString::fromUtf8 (l2u(name).c_str()));
    indexnamefield->setEnabled(newindex);
    uniquefield->setChecked(unique);
    list<hk_string>::iterator it=fields.begin();
    bool idxbuttonenabled=false;
    while (it!=fields.end())
    {
	indexfieldlist->addItem(QString::fromUtf8 (l2u((*it)).c_str()));
        idxbuttonenabled=true;
        it++;
    }
    if (datasource()==NULL) {return;}
    list<hk_column*>* cols=datasource()->columns();
    if (cols==NULL)
    {
        return;
    }
    bool fieldbuttonenabled=false;
    list<hk_column*>::iterator cit=cols->begin();
    while (cit!=cols->end())
    {
        if (!in_list((*cit)->name(),fields)&&(*cit)->is_notnull())
        {
	    dsfieldlist->addItem(QString::fromUtf8 (l2u((*cit)->name()).c_str()));
            fieldbuttonenabled=true;
        }
        cit++;
    }
    dsfieldlist->setCurrentRow(0);
    indexfieldlist->setCurrentRow(0);    
    deletebutton->setEnabled(idxbuttonenabled);
    addbutton->setEnabled(fieldbuttonenabled);
}


bool  hk_kdeindexeditdialog::in_list(const hk_string& f,list<hk_string>& l)
{
    list<hk_string>::iterator it=l.begin();
    while ( it!=l.end())
    {
        if ((*it)==f)return true;
        it++;
    }
    return false;
}


void hk_kdeindexeditdialog::before_source_vanishes(void)
{
    close();
}


void hk_kdeindexeditdialog::listentries_changed(void)
{
    addbutton->setEnabled(dsfieldlist->count()>0 && dsfieldlist->currentRow()>=0);
    deletebutton->setEnabled(indexfieldlist->count()>0 && indexfieldlist->currentRow()>=0 );
}
