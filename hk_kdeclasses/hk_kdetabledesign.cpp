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
//$Revision: 1.46 $

#include "hk_kdetabledesign.h"
#include "hk_kdetabledesign.moc"
#include "hk_kdeindexeditwindow.h"
#include <hk_database.h>
#include <hk_connection.h>
#include <hk_actionquery.h>

#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtreewidget.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <klocale.h>

void inline initSizePolicy( QWidget* w, QSizePolicy::Policy hp, QSizePolicy::Policy vp)
{
    QSizePolicy policy(hp,vp);
    
    policy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
    w->setSizePolicy(policy);
}

/*
 *  Constructs a hk_kdetabledesign which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
hk_kdetabledesign::hk_kdetabledesign( QWidget* parent,  const char* name, Qt::WFlags fl )
: QWidget( parent, fl )  ,hk_dsvisible()
{
#ifdef HK_DEBUG
//wanna_debug(true);
    hkclassname("hk_kdetabledesign");
    hkdebug("hk_kdetabledesign::hk_kdetabledesign");
#endif
    setObjectName(QString::fromAscii(name == NULL? "hk_kdetabledesign": name));
    resize( 500, 301 );
    setWindowTitle( i18n( "Tabledesign"  ) );
    hk_kdetabledesignLayout = new QGridLayout( this );
    hk_kdetabledesignLayout->setSpacing( 6 );
    hk_kdetabledesignLayout->setMargin( 11 );
    actualfieldit=fields.end();

    indexwindow=new QTreeWidget(this);
    indexwindow->setObjectName("indexwindow");
    QStringList hdr;
    hdr << i18n( "Index" ) << i18n( "Unique" ) ;
    indexwindow->setHeaderLabels( hdr );
    indexwindow->setAllColumnsShowFocus(true);
    indexwindow -> setRootIsDecorated(false);

    fieldlist = new QTreeWidget( this);
    fieldlist -> setObjectName("fieldlist" );
    fieldlist -> setSortingEnabled(false);
    fieldlist -> setRootIsDecorated(false);
    hdr.clear();
    hdr << i18n( "Name" ) << i18n( "Type" ) <<  i18n( "Size" ) << i18n( "Primary" ) << i18n( "Not Null" );
    fieldlist->setHeaderLabels( hdr );
    initSizePolicy(fieldlist, QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    fieldlist->setMinimumSize( QSize( 200, 0 ) );
    fieldlist->setAllColumnsShowFocus( true );

    hk_kdetabledesignLayout->addWidget(fieldlist, 0, 0, 3, 1 );

    Layoutindexwindow = new QHBoxLayout;
    Layoutindexwindow->setSpacing( 6 );
    Layoutindexwindow->setMargin( 0 );
    Layoutindexwindow->addWidget(indexwindow);

    buttonlayout = new QVBoxLayout;
    buttonlayout->setSpacing( 6 );
    buttonlayout->setMargin( 0 );

    editlayout = new QVBoxLayout;
    editlayout->setSpacing( 6 );
    editlayout->setMargin( 0 );

    fieldbuttonlayout = new QVBoxLayout;
    fieldbuttonlayout->setSpacing( 6 );
    fieldbuttonlayout->setMargin( 0 );

    fieldtoplayout = new QHBoxLayout;
    fieldtoplayout->setSpacing( 6 );
    fieldtoplayout->setMargin( 0 );
    editlayout->addLayout( fieldtoplayout );
    editlayout->addLayout(Layoutindexwindow);
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    fieldtoplayout->addItem( spacer );
    fieldtoplayout->addLayout( fieldbuttonlayout);

    alterbutton = new QPushButton(this);
    alterbutton->setObjectName("alterbutton");
    alterbutton->setText( i18n( "Alter Table"  ) );
    fieldbuttonlayout->addWidget( alterbutton );

    newbutton = new QPushButton(this);
    newbutton->setObjectName("newbutton");
    newbutton->setText( i18n( "New Field"  ) );
    fieldbuttonlayout->addWidget( newbutton );

    deletebutton = new QPushButton(this);
    deletebutton->setObjectName("deletebutton");
    deletebutton->setText( i18n( "Delete Field"  ) );
    deletebutton->setEnabled( false );
    fieldbuttonlayout->addWidget( deletebutton );

    QLabel* b = new QLabel(this);
    b->setObjectName("b");
    buttonlayout->addWidget( b );

    indexnewbutton = new QPushButton(this);
    indexnewbutton->setObjectName("indexnewbutton");
    indexnewbutton->setText( i18n( "New Index"  ) );
    indexnewbutton->setEnabled( true );
    buttonlayout->addWidget( indexnewbutton );

    indexdeletebutton = new QPushButton(this);
    indexdeletebutton->setObjectName("indexdeletebutton");
    indexdeletebutton->setText( i18n( "Delete Index"  ) );
    indexdeletebutton->setEnabled( false );
    buttonlayout->addWidget( indexdeletebutton );

    indexalterbutton = new QPushButton(this);
    indexalterbutton->setObjectName("indexalterbutton");
    indexalterbutton->setText( i18n( "Alter Index"  ) );
    indexalterbutton->setEnabled( false );
    buttonlayout->addWidget( indexalterbutton );

    Layoutindexwindow->addLayout( buttonlayout );
    hk_kdetabledesignLayout->addLayout( editlayout, 2, 1 );

    Layout8 = new QHBoxLayout;
    Layout8->setSpacing( 6 );
    Layout8->setMargin( 0 );

    Layout7 = new QHBoxLayout;
    Layout7->setSpacing( 6 );
    Layout7->setMargin( 0 );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout7->addItem( spacer_3 );

    Layout22 = new QVBoxLayout;
    Layout22->setSpacing( 6 );
    Layout22->setMargin( 0 );

    fieldnamelabel = new QLabel(this);
    fieldnamelabel->setObjectName("fieldnamelabel");
    fieldnamelabel->setText( i18n( "Fieldname:"  ) );
    Layout22->addWidget( fieldnamelabel );

    columntypelabel = new QLabel(this);
    columntypelabel->setObjectName("columntypelabel");
    columntypelabel->setText( i18n( "columntype:"  ) );
    columntypelabel->setMinimumSize( QSize( 99, 0 ) );
    Layout22->addWidget( columntypelabel );

    sizelabel = new QLabel(this);
    sizelabel->setObjectName("sizelabel");
    sizelabel->setText( i18n( "Size:"  ) );
    Layout22->addWidget( sizelabel );

    primaryindexlabel = new QLabel(this);
    primaryindexlabel->setObjectName("primaryindexlabel");
    primaryindexlabel->setText( i18n( "Primary Index"  ) );
    initSizePolicy(primaryindexlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    notnulllabel = new QLabel(this);
    notnulllabel->setObjectName("notnulllabel");
    notnulllabel->setText( i18n( "Not Null"  ) );
    initSizePolicy(notnulllabel, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    Layout22->addWidget( primaryindexlabel );
    Layout22->addWidget( notnulllabel );
    Layout7->addLayout( Layout22 );
    Layout8->addLayout( Layout7 );

    Layout24 = new QVBoxLayout;
    Layout24->setSpacing( 6 );
    Layout24->setMargin( 0 );

    namefield = new QLineEdit(this);
    namefield->setObjectName("namefield");
    namefield->setMinimumSize( QSize( 100, 0 ) );
    initSizePolicy(namefield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    namefield->setEnabled(false);
    Layout24->addWidget( namefield );

    typefield = new QComboBox( this );
    typefield -> setObjectName( "typefield" );
    initSizePolicy(typefield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    typefield->setMinimumSize( QSize( 100, 0 ) );
    typefield->setEnabled(false);
    Layout24->addWidget( typefield );

    sizefield = new QSpinBox( this );
    sizefield->setObjectName(QString::fromAscii("sizefield"));
    sizefield->setMaximum(65535);
    sizefield->setEnabled(false);
    Layout24->addWidget( sizefield );

    Layout23 = new QHBoxLayout;
    Layout23->setSpacing( 6 );
    Layout23->setMargin( 0 );

    primaryfield = new QCheckBox(this);
    primaryfield->setObjectName(QString::fromAscii("primaryfield"));
    initSizePolicy(primaryfield, QSizePolicy::Fixed, QSizePolicy::Fixed);
    primaryfield->setEnabled(true);
    Layout23->addWidget( primaryfield );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout23->addItem( spacer_4 );
    Layout24->addLayout( Layout23 );

    Layout25 = new QHBoxLayout;
    Layout25->setSpacing( 6 );
    Layout25->setMargin( 0 );
    notnullfield = new QCheckBox(this);
    notnullfield->setObjectName(QString::fromAscii("notnullfield"));
    initSizePolicy(notnullfield, QSizePolicy::Fixed, QSizePolicy::Fixed);
    notnullfield->setEnabled(true);
    QSpacerItem* spacer_5 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    Layout25->addWidget( notnullfield );
    Layout25->addItem( spacer_5 );
    Layout24->addLayout( Layout25);
    Layout8->addLayout( Layout24 );

    hk_kdetabledesignLayout->addLayout( Layout8, 0, 1 );
    p_fieldlist_created=false;
    structure_changes(false);
// signals and slots connections
    connect( namefield, SIGNAL( textChanged(const QString&) ), this, SLOT( name_changed() ) );
    connect( typefield, SIGNAL( activated(const QString&) ), this, SLOT( type_changed() ) );
    connect( sizefield, SIGNAL( valueChanged(const QString&) ), this, SLOT( size_changed() ) );
    connect( fieldlist, SIGNAL( itemSelectionChanged()), this, SLOT( selection_changed() ) );
    connect( primaryfield, SIGNAL( stateChanged(int) ), this, SLOT( primary_changed() ) );
    connect( notnullfield, SIGNAL( stateChanged(int) ), this, SLOT( notnull_changed() ) );
    connect( alterbutton, SIGNAL( clicked() ), this, SLOT( alterbutton_clicked() ) );
    connect( newbutton, SIGNAL( clicked() ), this, SLOT( new_clicked() ) );
    connect( deletebutton, SIGNAL( clicked() ), this, SLOT( delete_clicked() ) );

    connect( indexdeletebutton, SIGNAL( clicked() ), this, SLOT( delete_index() ) );
    connect( indexalterbutton, SIGNAL( clicked() ), this, SLOT( alter_index() ) );
    connect( indexwindow, SIGNAL( itemDoubleClicked(QTreeWidgetItem*,int) ), this, SLOT( alter_index() ) );
    connect( indexnewbutton, SIGNAL( clicked() ), this, SLOT( new_index() ) );
    connect( indexwindow, SIGNAL( itemSelectionChanged()), this, SLOT( indexselection_changed() ) );

    lastmarkedItem=NULL;
    fieldnamecheck=true;
    create_columntypelist();
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdetabledesign::~hk_kdetabledesign()
{
    columntype_dictionary.erase(columntype_dictionary.begin(),columntype_dictionary.end());
    fields.erase(fields.begin(),fields.end());
// no need to delete child widgets, Qt does it all for us
}


void hk_kdetabledesign::create_columntypelist(void)
{
    dictclass dict;
    columntype_dictionary.erase(columntype_dictionary.begin(),columntype_dictionary.end());
    typefield->clear();
    QString ftypename;

    ftypename= i18n( "Text" );
    typefield->addItem(ftypename  );
    dict.name=ftypename;
    dict.type=hk_column::textcolumn;
    columntype_dictionary.insert(columntype_dictionary.end(),dict);

    if (datasource()&&
        !(datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_NEW_TABLE)
        )
        )
    {
        fieldlist->setEnabled(false);
	buttonlayout->setEnabled(false);
	typefield->setEnabled(false);
	sizefield->setEnabled(false);
	primaryfield->setEnabled(false);
	notnullfield->setEnabled(false);
	namefield->setEnabled(false);
	deletebutton->setEnabled(false);



    }
    if (datasource()!=NULL)
    {
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_MEMOCOLUMN))
    {

        ftypename= i18n( "Memo" );
        typefield->addItem(ftypename  );
        dict.name=ftypename;
        dict.type=hk_column::memocolumn;
        columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }

        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_INTEGERCOLUMN))
    {
    ftypename= i18n( "Integer" );
    typefield->addItem(ftypename  );
    dict.name=ftypename;
    dict.type=hk_column::integercolumn;
    columntype_dictionary.insert(columntype_dictionary.end(),dict);
}
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_SMALLINTEGERCOLUMN))
    {
    ftypename= i18n( "Small Integer" );
    typefield->addItem(ftypename  );
    dict.name=ftypename;
    dict.type=hk_column::smallintegercolumn;
    columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }
    
    if (datasource()!=NULL)
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_AUTOINCCOLUMN))
    {
        ftypename= i18n( "Auto Increment" );
        typefield->addItem(ftypename  );
        dict.name=ftypename;
        dict.type=hk_column::auto_inccolumn;
        columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }

        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_FLOATINGCOLUMN))
    {
    ftypename= i18n( "Float" );
    typefield->addItem(ftypename  );
    dict.name=ftypename;
    dict.type=hk_column::floatingcolumn;
    columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }
    
    
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_SMALLFLOATINGCOLUMN))
    {
    ftypename= i18n( "Small Float" );
    typefield->addItem(ftypename  );
    dict.name=ftypename;
    dict.type=hk_column::smallfloatingcolumn;
    columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }
    }
    if (datasource()!=NULL)
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_BOOLCOLUMN))
    {
        ftypename=i18n( "Bool" );
        typefield->addItem( ftypename );
        dict.name=ftypename;
        dict.type=hk_column::boolcolumn;
        columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }

    if (datasource()!=NULL)
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_DATECOLUMN))
    {

        ftypename= i18n( "Date" );
        typefield->addItem(ftypename  );
        dict.name=ftypename;
        dict.type=hk_column::datecolumn;
        columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }

    if (datasource()!=NULL)
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_TIMECOLUMN))
    {

        ftypename= i18n( "Time" );
        typefield->addItem(ftypename  );
        dict.name=ftypename;
        dict.type=hk_column::timecolumn;
        columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }

    if (datasource()!=NULL)
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_DATETIMECOLUMN))
    {

        ftypename= i18n( "Datetime" );
        typefield->addItem(ftypename  );
        dict.name=ftypename;
        dict.type=hk_column::datetimecolumn;
        columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }

    if (datasource()!=NULL)
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_TIMESTAMPCOLUMN))
    {

        ftypename= i18n( "Timestamp" );
        typefield->addItem(ftypename  );
        dict.name=ftypename;
        dict.type=hk_column::timestampcolumn;
        columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }

    if (datasource()!=NULL)
        if (datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_BINARYCOLUMN))
    {

        ftypename= i18n( "Binary" );
        typefield->addItem(ftypename  );
        dict.name=ftypename;
        dict.type=hk_column::binarycolumn;
        columntype_dictionary.insert(columntype_dictionary.end(),dict);
    }
   typefield -> setMaxVisibleItems(12);

}


void hk_kdetabledesign::set_datasource(hk_datasource* d)
{
    hk_dsvisible::set_datasource(d);
    create_columntypelist();
    set_indexlist();
}


hk_datasource* hk_kdetabledesign::datasource(void)
{
    return hk_dsvisible::datasource();
}


hk_column::enum_columntype hk_kdetabledesign::ftype(const QString& f)
{
    list<dictclass>::iterator it=columntype_dictionary.begin();

    while (it!=columntype_dictionary.end())
    {
        if ((*it).name==f)  return (*it).type;
        it++;
    }
    return hk_column::othercolumn;
}


bool hk_kdetabledesign::alter_table(bool ask)
{
    bool res=true;

    if (!p_has_changed ) return true;

    if (datasource()==NULL)
    {
//show_warningmessage(hk_translate("No datasource defined!"));
        return true;
    }
//the following tests if the database driver is still working, neccessary workaround when closing
//the window, because database changes
    hk_actionquery*a=datasource()->database()->new_actionquery();
    if (!a)return false;
    delete a;
//end workaround

    if (ask && !show_yesnodialog(hk_translate("Do you want to alter the table?"),true))
    {
        clear_fieldlist();
        return false;
    }
    bool createtable=false;
    if (datasource()->name().size()==0)
    {
        hk_string res=show_stringvaluedialog(hk_translate("Please enter the tablename: "));
        datasource()->set_name(res);
	emit signal_tablename_changed();
        createtable=true;
    }
    if (datasource()->name().size()==0) return false;
    if (datasource()->database()->table_exists(datasource()->name()))
    {
        if (!createtable) datasource()->setmode_altertable();
        else
        {
            show_warningmessage(hk_translate("Table already exists"));
            datasource()->set_name("");
            return false;
        }
    }
    else      datasource()->setmode_createtable();

    list<fieldstruct>::iterator it=fields.begin();
    while (it!=fields.end())
    {
        switch ((*it).state)
        {
            case altered:
            {
                datasource()->alter_column(
                    (*it).oldname,
                    ((*it).oldname!=(*it).newname)?&((*it).newname):NULL,
                    ((*it).oldtype!=(*it).newtype)?&((*it).newtype):NULL,
                    ((*it).oldsize!=(*it).newsize)?&((*it).newsize):NULL,
                    NULL,
                    ((*it).oldprimary!=(*it).newprimary)?&((*it).newprimary):NULL,
                    ((*it).oldnotnull!=(*it).newnotnull)?&((*it).newnotnull):NULL
                    );

            }
            break
                ;
            case delfield:           datasource()->delete_column((*it).oldname);
            break
                ;
            case newfield:
            {
                hk_column* c=datasource()->new_column();
                c->set_name((*it).newname);
                c->set_columntype((*it).newtype);
                c->set_size((*it).newsize);
                c->set_primary((*it).newprimary);
                c->set_notnull((*it).newnotnull);
            }
            break
                ;

            default:;
        }
        it++;
    }
    if (datasource()->mode()==hk_datasource::mode_altertable) res=datasource()->alter_table_now();
    else
    if (datasource()->mode()==hk_datasource::mode_createtable) res=datasource()->create_table_now();

        if (res) clear_fieldlist();
    else
    {
        hk_string reason=replace_all("%NAME%",hk_translate("Table %NAME% could not be changed!"),datasource()->name())+"\n"+hk_translate("Servermessage: ")+datasource()->database()->connection()->last_servermessage();
        show_warningmessage(reason);
    }

    return res;
}


void hk_kdetabledesign::alterbutton_clicked()
{
    if (alter_table(false) )  load_fieldinfo();
}


void hk_kdetabledesign::delete_clicked()
{
    QTreeWidgetItem* f=fieldlist->currentItem();

    QTreeWidgetItem* p=NULL;
    if (f==NULL||actualfieldit==fields.end())  return;

    if ((*actualfieldit).state!=newfield)
    {
        (*actualfieldit).state=delfield;
        lastmarkedItem=NULL;
	if (p==NULL) p=fieldlist->itemAbove(f);
    }
    else
    {
        if (actualfieldit!=fields.end())
        {
            fields.erase(actualfieldit);
            actualfieldit=fields.end();
        }

    }

    fieldlist->takeTopLevelItem(fieldlist->indexOfTopLevelItem(fieldlist->currentItem()));
    structure_changes(true);
    if (p!=NULL)
        fieldlist->setCurrentItem(p);
}


void hk_kdetabledesign::index_clicked()
{
}


void hk_kdetabledesign::new_clicked()
{
    QString n;
    QString nn=i18n("newfield_");
    QString num;
    int numint=0;
    do
    {
        num=num.setNum(numint);
        numint++;
        n=nn+num;
    }
    while (fieldname_count(u2l(n.toUtf8().data()))!=0);
    QString t=i18n("Text");
    QTreeWidgetItem* feld=new QTreeWidgetItem(fieldlist,QStringList() << n << t << "50" << i18n("no") << i18n("no"));
    fieldstruct f;
    f.oldname=f.newname=u2l(n.toUtf8().data());
    f.oldtype=f.newtype=hk_column::textcolumn;
    f.oldsize=f.newsize=50;
    f.oldprimary=f.newprimary=false;
    f.oldnotnull=f.newnotnull=false;
    f.state=newfield;
    fields.insert(fields.end(),f);
    structure_changes(true);
    fieldlist->setCurrentItem(feld,true);

}


void hk_kdetabledesign::name_changed()
{
    QTreeWidgetItem* i=fieldlist->currentItem();
    if (i==NULL||actualfieldit==fields.end())return;
    (*actualfieldit).newname=u2l(namefield->text().toUtf8().data());
    if ((*actualfieldit).state!=newfield&&(*actualfieldit).state!=delfield) (*actualfieldit).state=altered;
    i->setText(0,namefield->text());
    structure_changes(true);
}


void hk_kdetabledesign::primary_changed()
{
    QTreeWidgetItem* i=fieldlist->currentItem();
    if (i==NULL||actualfieldit==fields.end())return;
    (*actualfieldit).newprimary=primaryfield->isChecked();
    if ((*actualfieldit).state!=newfield&&(*actualfieldit).state!=delfield) (*actualfieldit).state=altered;
    QString a=primaryfield->isChecked()?i18n("yes"):i18n("no");
    i->setText(3,a);
    structure_changes(true);
}


void hk_kdetabledesign::notnull_changed()
{
    QTreeWidgetItem* i=fieldlist->currentItem();
    if (i==NULL||actualfieldit==fields.end())return;
    (*actualfieldit).newnotnull=notnullfield->isChecked();
    if ((*actualfieldit).state!=newfield&&(*actualfieldit).state!=delfield) (*actualfieldit).state=altered;
    QString a=notnullfield->isChecked()?i18n("yes"):i18n("no");
    i->setText(4,a);
    structure_changes(true);
}


void hk_kdetabledesign::size_changed()
{
    QTreeWidgetItem* i=fieldlist->currentItem();
    if (i==NULL||actualfieldit==fields.end())return;
    (*actualfieldit).newsize=sizefield->value();
    i->setText(2,sizefield->text());
    structure_changes(true);
    if ((*actualfieldit).state!=newfield&&(*actualfieldit).state!=delfield) (*actualfieldit).state=altered;
}


void hk_kdetabledesign::type_changed()
{
    QTreeWidgetItem* i=fieldlist->currentItem();
    if (i==NULL||actualfieldit==fields.end())return;
    if ((*actualfieldit).state!=newfield&&(*actualfieldit).state!=delfield) (*actualfieldit).state=altered;
    (*actualfieldit).newtype=ftype(typefield->currentText());
    i->setText(1,typefield->currentText());
    structure_changes(true);
    set_fields_enabled();
}


int hk_kdetabledesign::fieldname_count(const hk_string& f)
{
    int res=0;
    list<fieldstruct>::iterator it=fields.begin();
    while (it!=fields.end())
    {
        if (string2lower((*it).newname)==string2lower(f))res++;

        it++;
    }
    return res;
}


void hk_kdetabledesign::indexselection_changed()
{
    QTreeWidgetItem* i=indexwindow->currentItem();
    
    if (i==NULL)
    {
        indexdeletebutton->setEnabled( false );
        indexalterbutton->setEnabled( false );
        return;
    }
    indexdeletebutton->setEnabled( true );
    indexalterbutton->setEnabled( true );
}


void hk_kdetabledesign::selection_changed()
{
    QTreeWidgetItem* i=fieldlist->currentItem();
    if (i==NULL)return;

    if (lastmarkedItem!=NULL &&lastmarkedItem!=i)
    {
        if ((fieldname_count(u2l(lastmarkedItem->text(0).toUtf8().data()))>1||lastmarkedItem->text(0).length()==0)
            &&fieldnamecheck)
        {
            fieldnamecheck=false;
            show_warningmessage(hk_translate("Fieldname already exists or is of length 0. Alter it to a unique one!"));
            fieldlist->setCurrentItem(lastmarkedItem);
            fieldnamecheck=true;
            return;
        }

    }
    namefield->blockSignals(true);
    sizefield->blockSignals(true);
    primaryfield->blockSignals(true);
    notnullfield->blockSignals(true);
    typefield->blockSignals(true);

    namefield->setText(i->text(0));
    int v=i->text(2).toInt();
    sizefield->setValue(v);
    primaryfield->setChecked(i->text(3)==(QString) i18n("yes"));
    int cur = typefield -> findText(i->text(1));
    if (cur != -1 ) typefield -> setCurrentIndex(cur);

    list<fieldstruct>::iterator it=fields.begin();
    bool found=false;
    hk_string nn=u2l(i->text(0).toUtf8().data());
    while (it!=fields.end()&&!found)
    {
        if ((*it).newname==nn)
        {
            actualfieldit=it;
            found=true;
        }
        it++;
    }

    if (actualfieldit!=fields.end())
        notnullfield->setChecked((*actualfieldit).newnotnull);

    deletebutton->setEnabled(namefield->isEnabled());
    namefield->blockSignals(false);
    sizefield->blockSignals(false);
    primaryfield->blockSignals(false);
    notnullfield->blockSignals(false);
    typefield->blockSignals(false);
    lastmarkedItem=fieldlist->currentItem();
    if (datasource())namefield->setMaxLength(datasource()->database()->connection()->maxfieldnamesize());
    set_fields_enabled();
}


void hk_kdetabledesign::showEvent(QShowEvent* e)
{
    QWidget::showEvent(e);
}


void hk_kdetabledesign::load_fieldinfo()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdetabledesign::load_fieldinfo()");
#endif
    fieldlist->blockSignals(true);
    fieldlist->clear();
    lastmarkedItem=NULL;
    fields.erase(fields.begin(),fields.end());
    actualfieldit=fields.end();
    hk_datasource* d=datasource();
    if (d==NULL)
    {
        structure_changes(false);
        fieldlist->blockSignals(false);
        return;
    }


    list<hk_column*>* cols=d->columns();
    if (cols==NULL)
    {
        structure_changes(false);
        fieldlist->blockSignals(false);
        return;
    }
    list<hk_column*>::iterator it=cols->begin();
    while (it!=cols->end())
    {
        fieldstruct f;

        QString n=QString::fromUtf8(l2u((*it)->name()).c_str());
        f.oldname=f.newname=(*it)->name().c_str();
        QString t=QString::fromUtf8(l2u((*it)->columntype_name()).c_str());
        QString s;
        s=s.setNum((*it)->size());
        f.oldsize=f.newsize=(*it)->size();
        f.oldprimary=f.newprimary=(*it)->is_primary();
        f.oldnotnull=f.newnotnull=(*it)->is_notnull();
        f.oldtype=f.newtype=(*it)->columntype();
        QString a=(*it)->is_primary()?i18n("yes"):i18n("no");
        QString notn=(*it)->is_notnull()?i18n("yes"):i18n("no");
        new QTreeWidgetItem(fieldlist,QStringList() << n << t << s << a << notn);

        f.state=unchanged;
        fields.insert(fields.end(),f);
        it++;
    }
    p_fieldlist_created=true;
    structure_changes(false);
    fieldlist->blockSignals(false);
    fieldlist->setFocus();
    fieldlist -> resizeColumnToContents(0);
    fieldlist -> resizeColumnToContents(1);
    fieldlist -> resizeColumnToContents(2);
    fieldlist -> resizeColumnToContents(3);
    fieldlist -> resizeColumnToContents(4);
    fieldlist -> setCurrentItem(fieldlist->invisibleRootItem()->child(0));
    lastmarkedItem = fieldlist->invisibleRootItem()->child(0);
    fieldnamecheck=true;
    set_indexlist();
    if (fields.size()>0)
    {
        bool res=datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_ALTER_TABLE);
        fieldlist->setEnabled(res);
	buttonlayout->setEnabled(res);
	typefield->setEnabled(res);
	sizefield->setEnabled(res);
	primaryfield->setEnabled(res);
	notnullfield->setEnabled(res);
	namefield->setEnabled(res);
	newbutton->setEnabled(res);
	deletebutton->setEnabled(res);
	if (res)
	{
	 primaryfield->setEnabled(datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_ALTER_PRIMARY_KEY));
	 notnullfield->setEnabled(datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_ALTER_NOT_NULL));
	}

    }
    else
    {
        bool res=datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_NEW_TABLE);
        fieldlist->setEnabled(res);
	buttonlayout->setEnabled(res);
	typefield->setEnabled(res);
	sizefield->setEnabled(res);
	primaryfield->setEnabled(res);
	notnullfield->setEnabled(res);
	namefield->setEnabled(res);
	newbutton->setEnabled(res);
    }

}


void hk_kdetabledesign::alter_index()
{
    if (datasource()==NULL) { return; }
    list<hk_datasource::indexclass>* ilist =datasource()->indices();
    if (ilist==NULL){return; }
    list<hk_datasource::indexclass>::iterator it=ilist->begin();
    bool found=false;

    while (it!=ilist->end() &&!found)
    {
        hk_string r= u2l(indexwindow->currentItem()->text(0).toUtf8().data());
        if ((*it).name==r) found=true;
        else  it++;
    }
    if (found)
    {
        hk_kdeindexeditdialog* d = new hk_kdeindexeditdialog(0,0);
        if (d==NULL) return;
        d->setAttribute(Qt::WA_DeleteOnClose);
        d->set_datasource(datasource());
        if (it!=ilist->end())d->set_indexvalues((*it).name,(*it).unique,(*it).fields,false);
        d->show(); 
    }
}


void hk_kdetabledesign::delete_index()
{
    if (datasource()==NULL){ return; }
    hk_string r= u2l(indexwindow->currentItem()->text(0).toUtf8().data());
    datasource()->drop_index(r.c_str());

}


void hk_kdetabledesign::new_index()
{
    if (datasource()==NULL){ return; }

    hk_kdeindexeditdialog* d = new hk_kdeindexeditdialog(0,0);
    if (d==NULL) return;
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->set_datasource(datasource());
    list<hk_string> f;
    d->set_indexvalues("",false,f,true);
    d->show();
}


void hk_kdetabledesign::set_indexlist(void)
{
    indexwindow->clear();
    hk_datasource* d=datasource();
    if (d==NULL)
    {
        return;
    }
    list<hk_datasource::indexclass>* ilist =d->indices();
    if (ilist==NULL)
    {
        return;
    }
    list<hk_datasource::indexclass>::iterator it=ilist->begin();
    while (it!=ilist->end())
    {
     new QTreeWidgetItem(indexwindow, 
	      QStringList() << QString::fromUtf8(l2u((*it).name).c_str()) << QString((*it).unique?i18n("yes"):i18n("no")));
     it++;
    }
    indexwindow->setCurrentItem(indexwindow->invisibleRootItem()->child(0));
}


void hk_kdetabledesign::list_changes(listtype type)
{
    if (type==lt_index) set_indexlist();
}


void hk_kdetabledesign::structure_changes(bool c)
{
    p_has_changed=c;
    alterbutton->setEnabled(c);
    if (c) emit signal_has_changed();
    indexnewbutton->setEnabled(!c);
    indexalterbutton->setEnabled(!c);

    bool res;
    if ( fieldlist->invisibleRootItem()->childCount() == 0)
    {
        res=false;
        primaryfield->setChecked(false);
        notnullfield->setChecked(false);
        namefield->setText("");
    }
    else
    {
       if (fields.size()==0||c)
        res=(datasource()&&
        (datasource()->database()->connection()->
	server_supports(hk_connection::SUPPORTS_NEW_TABLE)));
      else
        res=(datasource()&&
        (datasource()->database()->connection()->
	server_supports(hk_connection::SUPPORTS_ALTER_TABLE)));
    }
    namefield->setEnabled(res);
    typefield->setEnabled(res);
    primaryfield->setEnabled(res);
    notnullfield->setEnabled(res);
    sizefield->setEnabled(res);
    deletebutton->setEnabled(res);
   if (res)
   {
        if (datasource() && datasource()->name().size()>0)
        {
	 primaryfield->setEnabled(datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_ALTER_PRIMARY_KEY));
	 notnullfield->setEnabled(datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_ALTER_NOT_NULL));
        }
        else
        {
	 primaryfield->setEnabled(true);
	 notnullfield->setEnabled(true);
        }
   }
}

void hk_kdetabledesign::clear_fieldlist(void)
{
    fields.erase(fields.begin(),fields.end());
    p_fieldlist_created=false;
    fieldnamecheck=true;
    lastmarkedItem=NULL;
    load_fieldinfo();
    structure_changes(false);
}


void hk_kdetabledesign::set_designmode()
{
    clear_fieldlist();
}


void hk_kdetabledesign::show_fieldinfos(fieldstruct* f)
{
    if (!f) return;

    hk_string s;
    switch (f->state)
    {
        case unchanged:s="UNCHANGED";break;
        case delfield:s="DELFIELD";break;
        case newfield:s="NEWFIELD";break;
        default:s="ALTERED";
    }

    cout <<endl<<"STATE:  "<<s<<endl;
    cout <<"oldname: "<<f->oldname<<endl;
    cout <<"oldprimary: "<<(f->oldprimary?"JA":"NEIN")<<endl;
    cout <<"oldnotnull: "<<(f->oldnotnull?"JA":"NEIN")<<endl;
    cout <<"oldtype: "<<f->oldtype<<endl;
    cout <<"oldsize: "<<f->oldsize<<endl;
    cout <<"newname: "<<f->newname<<endl;
    cout <<"newprimary: "<<(f->newprimary?"JA":"NEIN")<<endl;
    cout <<"newnotnull: "<<(f->newnotnull?"JA":"NEIN")<<endl;
    cout <<"newtype: "<<f->newtype<<endl;
    cout <<"newsize: "<<f->newsize<<endl<<endl;

}



void hk_kdetabledesign::set_fields_enabled(void)
{
    if (actualfieldit==fields.end())return;
    sizefield->setEnabled((*actualfieldit).newtype==hk_column::textcolumn);

}


bool hk_kdetabledesign::has_changed(void) const
{
   return p_has_changed;
}


