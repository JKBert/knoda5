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

#include "hk_kdereportproperty.h"
#include "hk_kdesimplereport.h"
#include "hk_kdesubreportdialog.h"
#include "hk_kdereportsection.h"
#include "hk_kdeinterpreterdialog.h"
#include "hk_kdespinbox.h"
#include "hk_kdeformdatasourcedialog.h"
#include "hk_kdereportdata.h"
#include "hk_kdefilterdialog.h"

#include <hk_datasource.h>
#include <hk_database.h>
#include <hk_colour.h>
#include <hk_visible.h>
#include <hk_reportdata.h>
#include <hk_reportsection.h>
#include <hk_reportsectionpair.h>

#include <qcombobox.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qfontdatabase.h>
#include <qlayout.h>
#include <qtabwidget.h>
#include <qcolor.h>
#include <qstringlist.h>
#include <qtooltip.h>
#include <qevent.h>
#include <qscrollarea.h>

#include <klocale.h>
#include <kcolorbutton.h>

void inline initSizePolicy( QWidget* w, QSizePolicy::Policy hp, QSizePolicy::Policy vp)
{
    QSizePolicy policy(hp,vp);
    
    policy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
    w->setSizePolicy(policy);
}

hk_kdereportproperty::hk_kdereportproperty( hk_kdesimplereport* parent,  const char* name, Qt::WFlags fl )
: hk_kdereportpropertybase( parent, name,  fl )  , hk_class()
{

#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::hk_kdereportproperty()");
//wanna_debug(true);
#endif
    tab->removeTab(tab->indexOf(datatab));
    
    p_datascrollview=new QScrollArea(tab);
    p_datascrollview->setWidgetResizable(true);
    tab->addTab(p_datascrollview, "");
    p_datascrollview->setWidget(datatab);
    datatab -> setParent(p_datascrollview);
    
    tab->removeTab(tab->indexOf(formattab));
    
    p_formatscrollview=new QScrollArea(tab);
    p_formatscrollview->setWidgetResizable(true);
    tab->addTab(p_formatscrollview, "");
    p_formatscrollview->setWidget(formattab);
    formattab->setParent(p_formatscrollview);
    
    tab->removeTab(tab->indexOf(frametab));
    
    p_framescrollview=new QScrollArea(tab);
    tab->addTab(p_framescrollview, "");
    p_framescrollview->setWidget(frametab);
    frametab->setParent(p_framescrollview);
    
    tab->removeTab(tab->indexOf(actiontab));
    
    p_actionscrollview=new QScrollArea(tab);
    tab->addTab(p_actionscrollview, "");
    p_actionscrollview->setWidget(actiontab);
    actiontab->setParent(p_actionscrollview);
      
    pageformatfield=NULL;
    orientationfield=NULL;
    firstpagenumberfield=NULL;
    widthfield->setRange(0,10000);
    heightfield->setRange(0,10000);
    xfield->setRange(0,10000);
    yfield->setRange(0,10000);
    p_report=parent;
    p_visible=NULL;
    p_fontdatabase = new QFontDatabase();
    list<hk_string>* flist=hk_font::fontlist();
    QStringList newstrlist;
    list<hk_string>::iterator it=flist->begin();
    while(it!=flist->end())
    {
        QString f=QString::fromUtf8(l2u(*it).c_str());

        newstrlist.append(f);
        ++it;
    }

    fontfield->insertItems(fontfield->count(), newstrlist);
    QStringList yesno(i18n("yes"));
    yesno << i18n("no");

    wordbreakfield->addItems(yesno);
    dynamicheightfield->addItems(yesno);
    toplinefield->addItems(yesno);
    bottomlinefield->addItems(yesno);
    leftlinefield->addItems(yesno);
    rightlinefield->addItems(yesno);
    diagonallurofield->addItems(yesno);
    diagonallorufield->addItems(yesno);
    separatorfield->addItems(yesno);
    boldfield->addItems(yesno);
    italicfield->addItems(yesno);
    
    sortingfield->addItem(i18n("Ascending"));
    sortingfield->addItem(i18n("Descending"));

    list<hk_string>* l=p_report->reporttypelist();
    it=l->begin();
    int i=0;
    while (it!=l->end())
    {
        reporttypefield->addItem(QString::fromUtf8(l2u((*it)).c_str()));
        if (p_report->reporttype()==(*it)) reporttypefield->setCurrentIndex(i);
        it++; i++;
    }
    reporttypefield->setEnabled(false);

    toplinelabel->setText(i18n("Top line:"));
    bottomlinelabel->setText(i18n("Bottom line:"));
    leftlinelabel->setText(i18n("Left line:"));
    rightlinelabel->setText(i18n("Right line:"));
    diagonallurolabel->setText(i18n("DiagonalLURO:"));
    diagonallorulabel->setText(i18n("DiagonalLORU:"));
    bordertoplabel->setText(i18n("Border Top:"));
    borderbottomlabel->setText(i18n("Border Bottom:"));
    borderleftlabel->setText(i18n("Border Left:"));
    borderrightlabel->setText(i18n("Border Right:"));
    reporttypelabel->setText(i18n("Reporttype:"));
    subreportlabel->setText(i18n("Subreport:"));
    valuelabel->setText(i18n("Value:"));
    wordbreaklabel->setText(i18n("Wordbreak:"));
    runningcountlabel->setText(i18n("Running count:"));
    tab->setTabText(tab->indexOf(formattab),i18n("&Format"));
    tab->setTabText(tab->indexOf(datatab),i18n("&Data"));
    tab->setTabText(tab->indexOf(frametab),i18n("F&rame"));
    digitlabel->setText(i18n("Precision:"));
    separatorlabel->setText("");
    sortinglabel->setText(i18n("Sorting:"));
    heightlabel->setText(i18n("Height:"));

    p_foregroundcolourlabel = new QLabel(formattab);
    p_foregroundcolourlabel->setObjectName("foregroundcolourlabel");
    initSizePolicy(p_foregroundcolourlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    
    p_foregroundcolourlabel->setText( i18n( "Foregroundcolour:" ) );
    
    formattabLayout->addWidget( p_foregroundcolourlabel, 13, 0 );

    p_foregroundcolourbutton = new KColorButton( formattab );
    p_foregroundcolourbutton->setObjectName( "foregroundcolourfield" );
    initSizePolicy(p_foregroundcolourbutton, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    formattabLayout->addWidget( p_foregroundcolourbutton, 13, 1 );

    p_backgroundcolourlabel = new QLabel(formattab);
    p_backgroundcolourlabel->setObjectName("backgroundcolourlabel");
    initSizePolicy(p_backgroundcolourlabel, QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    p_backgroundcolourlabel->setText( i18n( "Backgroundcolour:" ) );

    formattabLayout->addWidget( p_backgroundcolourlabel, 14, 0 );

    p_backgroundcolourbutton = new KColorButton( formattab );
    p_backgroundcolourbutton->setObjectName( "backgroundcolourfield" );
    initSizePolicy(p_backgroundcolourbutton, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    formattabLayout->addWidget( p_backgroundcolourbutton, 14, 1 );

    connect( p_foregroundcolourbutton, SIGNAL( changed(const QColor&) ), this, SLOT( format_changes() ) );
    connect( p_backgroundcolourbutton, SIGNAL( changed(const QColor&) ), this, SLOT( format_changes() ) );
    QSpacerItem* spacer_3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    formattabLayout->addItem( spacer_3, 15, 1 );
    QSpacerItem* spacer_4 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    formattabLayout->addItem( spacer_4, 15, 0 );
    settxt=i18n("set");
    notsettxt=i18n("not set");

    identifierfield->installEventFilter(this);
    valuefield->installEventFilter(this);

    QString info=i18n("Press <F2> for a larger editor window");
    identifierfield->setToolTip(info);
    valuefield->setToolTip(info);

    tab->setTabText(tab->indexOf(p_formatscrollview),i18n("&Format"));
    tab->setTabText(tab->indexOf(p_datascrollview),i18n("&Data"));
    tab->setTabText(tab->indexOf(p_actionscrollview),i18n("&Action"));
    tab->setTabText(tab->indexOf(p_framescrollview),i18n("F&rame"));
    
    QSize s=actiontab->geometry().size();
    if (frametab->width()>s.width())s.setWidth(frametab->width());
    if (formattab->width()>s.width())s.setWidth(formattab->width());
    if (datatab->width()>s.width())s.setWidth(datatab->width());
    
    if (frametab->height()>s.height())s.setHeight(frametab->height());
    if (formattab->height()>s.height())s.setHeight(formattab->height());
    if (datatab->height()>s.height())s.setHeight(datatab->height());
    
    QSizePolicy sp=QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    frametab->setSizePolicy(sp);
    formattab->setSizePolicy(sp);
    actiontab->setSizePolicy(sp);
    datatab->setSizePolicy(sp);
    
    frametab->setMinimumSize(s);
    formattab->setMinimumSize(s);
    actiontab->setMinimumSize(s);
    datatab->setMinimumSize(s);
}


hk_kdereportproperty::~hk_kdereportproperty()
{
// no need to delete child widgets, Qt does it all for us
 delete p_fontdatabase;
}


void hk_kdereportproperty::action_changes()
{
    qWarning( "hk_kdereportproperty::action_changes() not yet implemented!" );
}


void hk_kdereportproperty::actionobject_changes()
{
    qWarning( "hk_kdereportproperty::actionobject_changes() not yet implemented!" );
}


void hk_kdereportproperty::add_propertydatasource()
{
    qWarning( "hk_kdereportproperty::add_propertydatasource() not yet implemented!" );
}


void hk_kdereportproperty::data_changes()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::data_changes()");
#endif
    if (p_visible==NULL) return;
    switch (p_visible->type())
    {

        case hk_visible::reportdata:      set_objectreportdatavisible();
        break;
        case hk_visible::reportsection:   set_objectreportsectionvisible();
        break;
        case hk_visible::report:
            set_objectdsvisible();
            set_objectreportvisible();
            break;
        default:;

    }
}


void hk_kdereportproperty::datasource_selected()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::datasource_selected");
#endif
    columnfield->blockSignals(true);
    QString currentvalue=columnfield->currentText();
    columnfield->clear();
    columnfield->blockSignals(false);
    if (p_report==NULL) return;
    hk_database* db=p_report->database();
    if (db==NULL) return;
    QString f=datasourcefield->currentText();
    if (f.isEmpty()) f="";
    hk_datasource* ds= p_report->get_datasource(u2l(f.toUtf8().data()));
    if (ds==NULL) return;
    list<hk_column*>* v=NULL;
    hk_datasource* tmpds=NULL;

    if (ds->type()==hk_datasource::ds_query)
    {
        tmpds=ds->database()->new_resultquery();
        if (tmpds)
        {
            tmpds->set_sql(ds->sql());
            tmpds->set_filter("1=0");
            tmpds->enable();
            v=tmpds->columns();

        }

    }
    else    v=ds->columns();
    if (v==NULL)
    {
        if (tmpds)
        {
            tmpds->disable();
            delete tmpds;
        }
        return;
    }
    columnfield->addItem("");
    list<hk_column*>::iterator it=v->begin();
    int i=1;
    while (it!=v->end())
    {
        columnfield->addItem(QString::fromUtf8(l2u((*it)->name()).c_str()));
        if (QString::fromUtf8(l2u((*it)->name()).c_str())==currentvalue) columnfield->setCurrentIndex(i);
        it++;i++;
    }
    if (tmpds)
    {
        tmpds->disable();
        delete tmpds;
    }

}


void hk_kdereportproperty::format_changes()
{
    if (p_visible==NULL) return;
    switch (p_visible->type())
    {

        case hk_visible::reportdata:      set_objectreportdatavisible();
        break;
        case hk_visible::reportsection:   set_objectreportsectionvisible();
        break;
        case hk_visible::report:
            set_objectdsvisible();
            set_objectreportvisible();
            break;
        default:;

    }
    set_objectvisible();
}

void hk_kdereportproperty::font_changes()
{

  set_objectfontvisible();
}


void hk_kdereportproperty::size_changes()
{
  set_objectsizevisible();
}

void hk_kdereportproperty::position_changes()
{
  set_objectpositionvisible();
}


void hk_kdereportproperty::datasourcebutton_clicked()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::datasourcebutton_clicked");
#endif
    hk_kdeformdatasourcedialog* d = new hk_kdeformdatasourcedialog(p_report,this,0,true);
    d->set_displaytype(hk_kdeformdatasourcedialog::d_report);
    hk_datasource* ds=p_report->get_datasource(u2l(datasourcefield->currentText().toUtf8().data()));
    d->set_datasource(ds);
    if (ds)
    {
        d->alterbutton->setEnabled(true);
        d->deletebutton->setEnabled(true);
    }
    d->exec();
    delete d;
    set_datasourcelist();
    if (datasourcefield->currentIndex()==-1)
        datasourcefield->setCurrentIndex(0);
}


void hk_kdereportproperty::set_object(hk_visible* v)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_object()");
#endif

    if (p_visible!=NULL && p_visible->type()==hk_visible::reportsection)
    {
        hk_kdereportsection* s=dynamic_cast<hk_kdereportsection*>(p_visible);
        if (s!=NULL)
            s->set_activesection(false);

    }

    p_visible=v;
    if (p_visible != NULL)
    { 
      QString num;
      num.setNum(p_visible->presentationnumber());
      idfield->setText(num);
    }
    
    identifierfield->setEnabled(p_visible!=NULL && p_visible->type() == hk_visible::reportdata);
    
    if (p_visible==NULL) return;
    QString head;
    
    head.setNum(p_visible->presentationnumber());
    beforerowchangeaction->setEnabled(false);
    afterrowchangeaction->setEnabled(false);
    onprintaction->setEnabled(false);
    onnewpageaction->setEnabled(false);
    setWindowTitle(i18n("Property editor (")+head+")");
    // manage access to datasource edit controls
    datasourcefield->setEnabled(p_visible->type() == hk_visible::report);
    datasourcebutton->setEnabled(p_visible->type() == hk_visible::report);
    // manage access to value & value format edit controls
    separatorfield->setEnabled(p_visible->type() == hk_visible::reportdata);
    digitfield->setEnabled(p_visible->type() == hk_visible::reportdata);
    columnfield->setEnabled(p_visible->type() == hk_visible::reportdata);
    valuefield->setEnabled(p_visible->type() == hk_visible::reportdata);
    dynamicheightfield->setEnabled(p_visible->type() == hk_visible::reportdata);
    // manage access to object size edit controls
    widthfield->setEnabled(p_visible->type() == hk_visible::reportdata);
    heightfield->setEnabled(p_visible->type() != hk_visible::report);
    // manage access to object frame line edit controls
    toplinefield->setEnabled(p_visible->type() != hk_visible::reportsection);
    leftlinefield->setEnabled(p_visible->type() != hk_visible::reportsection);
    rightlinefield->setEnabled(p_visible->type() != hk_visible::reportsection);
    bottomlinefield->setEnabled(p_visible->type() != hk_visible::reportsection);
    diagonallurofield->setEnabled(p_visible->type() != hk_visible::reportsection);
    diagonallorufield->setEnabled(p_visible->type() != hk_visible::reportsection);
    // manage access to page border edit controls
    bordertopfield->setEnabled(p_visible->type() == hk_visible::report);
    borderbottomfield->setEnabled(p_visible->type() == hk_visible::report);
    borderleftfield->setEnabled(p_visible->type() == hk_visible::report);
    borderrightfield->setEnabled(p_visible->type() == hk_visible::report);
    
    subreportbutton->setEnabled(p_visible->type() == hk_visible::reportsection);
    runningcountfield->setEnabled(p_visible->type() != hk_visible::reportsection);
    sortingfield->setEnabled(p_visible->type() == hk_visible::reportsection);
    
    p_foregroundcolourbutton->setEnabled(p_visible->type() != hk_visible::reportsection);
    p_backgroundcolourbutton->setEnabled(p_visible->type() != hk_visible::reportsection);
    
    heightlabel->setText(i18n("Height:"));

    switch (p_visible->type())
    {
        case hk_visible::reportdata :
            delete pageformatfield;
            pageformatfield=NULL;
            delete orientationfield;
            orientationfield=NULL;
            delete firstpagenumberfield;
            firstpagenumberfield=NULL;
            runningcountfield->blockSignals(true);
            runningcountfield->clear();
            runningcountfield->addItem(i18n("yes"));
            runningcountfield->addItem(i18n("no"));
            runningcountlabel->setText(i18n("Running count:"));
            runningcountfield->blockSignals(false);

            if (xfield==NULL)
            {
                xfield=new hk_kdespinbox( formattab, "xfield" );
                xfield->setSingleStep( 10 );
                xfield->setRange(0,10000);
                formattabLayout->addWidget( xfield, 1, 1 );
                xfield->show();
                connect( xfield, SIGNAL( valueChanged(int) ), this, SLOT( position_changes() ) );
            }
            xlabel->setText(i18n("X:"));
            xfield->setEnabled(true);

            if (yfield==NULL)
            {
                yfield=new hk_kdespinbox( formattab, "yfield" );
                yfield->setSingleStep( 10 );
                yfield->setRange(0,10000);
                formattabLayout->addWidget( yfield, 2, 1 );
                yfield->show();
                connect( yfield, SIGNAL( valueChanged(int) ), this, SLOT( position_changes() ) );
            }
            ylabel->setText(i18n("Y:"));
            yfield->setEnabled(true);

            alignmentlabel->setText(i18n("Alignment:"));
            if (alignmentfield==NULL)
            {
                alignmentfield= new QComboBox(formattab);
                alignmentfield->setObjectName(QString::fromAscii("alignmentfield"));
                alignmentfield->addItems(QStringList(i18n("Left")) << i18n("Center") 
                    << i18n("Right") << i18n("Default"));
                formattabLayout->addWidget( alignmentfield, 9, 1 );
                connect( alignmentfield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
                connect( alignmentfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( data_changes() ) );
                alignmentfield->show();
            }
            alignmentlabel->setText(i18n("Alignment:"));
            alignmentfield->setEnabled(true);

            wordbreaklabel->setText(i18n("Wordbreak:"));
            runningcountlabel->setText(i18n("Running count:"));
            digitfield->setRange(-1,10);
            set_reportdatavisible();
            break;
        case hk_visible::reportsection:
            if (alignmentfield!=NULL) alignmentfield->setEnabled(false);
            if (firstpagenumberfield!=NULL)firstpagenumberfield->setEnabled(false);
            wordbreaklabel->setText(i18n("New page:"));
            heightlabel->setText(i18n("Offset:"));

            if (xfield!=NULL) xfield->setEnabled(false);
            if (pageformatfield!=NULL) pageformatfield->setEnabled(false);

            if (yfield!=NULL) yfield->setEnabled(false);
            if (orientationfield!=NULL) orientationfield->setEnabled(false);
            set_reportsectionvisible();
            break;
        case hk_visible::report:
        {
            delete xfield;
            xfield=NULL;
            delete yfield;
            yfield=NULL;
            delete alignmentfield;
            alignmentfield=NULL;
            runningcountfield->blockSignals(true);
            runningcountfield->clear();
            runningcountfield->addItem(i18n("relative"));
            runningcountfield->addItem(i18n("absolute"));
            hk_report* r=dynamic_cast <hk_report*>(p_visible);
            runningcountfield->setCurrentIndex((r->sizetype()==hk_presentation::relative?0:1));
            runningcountfield->blockSignals(false);
            runningcountlabel->setText(i18n("Sizetype:"));
            if ( pageformatfield==NULL)
            {
                pageformatfield=new QComboBox(formattab);
                pageformatfield->setObjectName(QString::fromAscii("pageformatfield"));
                pageformatfield->addItems(QStringList("A0") << "A1" << "A2"<< "A3" << "A4" << "A5" << "A6"
                    << "A7" << "A8" << "A9" << "A10" << "letter" << "legal" << "executive"
                    << "tabloid" << i18n("Userdefined"));
                connect( pageformatfield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
                connect( pageformatfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( data_changes() ) );
                pageformatfield->show();
                
            }
            xlabel->setText(i18n("Pageformat:"));
            pageformatfield->setEnabled(true);
            formattabLayout->addWidget(pageformatfield,1,1);
            if ( orientationfield==NULL)
            {
                orientationfield=new QComboBox(formattab);
                orientationfield->setObjectName(QString::fromAscii("orientationfield"));
                orientationfield->addItem(i18n("Portrait"));
                orientationfield->addItem(i18n("Landscape"));
                orientationfield->show();
                connect( orientationfield, SIGNAL( activated(const QString&) ), this, SLOT( data_changes() ) );
                connect( orientationfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( data_changes() ) );
            }

            orientationfield->setEnabled(p_report->pageformat()!=hk_report::userdefined);

            if (firstpagenumberfield==NULL)
            {
                firstpagenumberfield=new QSpinBox( formattab );
                firstpagenumberfield->setObjectName(QString::fromAscii("firstpagenumberfield"));
                firstpagenumberfield->setRange(0,100000);
                formattabLayout->addWidget( firstpagenumberfield, 9, 1 );
                firstpagenumberfield->show();
                connect( firstpagenumberfield, SIGNAL( valueChanged(int) ), this, SLOT( format_changes() ) );
                
            }
            firstpagenumberfield->setEnabled(true);
            firstpagenumberfield->show();
            alignmentlabel->setText(i18n("First pagenumber:"));
            ylabel->setText(i18n("Orientation:"));
            formattabLayout->addWidget(orientationfield,2,1);

            wordbreaklabel->setText(i18n("Multiple files:"));
            set_dsvisible();
            set_reportvisible();
            break;
        }
        default:;
    }
    if (p_report->sizetype()==hk_presentation::relative)
    {
     if (widthfield) widthfield->set_displaytype(hk_kdespinbox::percent);
     if (heightfield) heightfield->set_displaytype(hk_kdespinbox::percent);
     if (xfield) xfield->set_displaytype(hk_kdespinbox::percent);
     if (yfield) yfield->set_displaytype(hk_kdespinbox::percent);
    }
    else
    {
     if (widthfield) widthfield->set_displaytype(hk_kdespinbox::cm);
     if (heightfield) heightfield->set_displaytype(hk_kdespinbox::cm);
     if (xfield) xfield->set_displaytype(hk_kdespinbox::cm);
     if (yfield) yfield->set_displaytype(hk_kdespinbox::cm);
    }
    
    set_visible();
}


void hk_kdereportproperty::set_visible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_visible");
#endif
    if (p_visible==NULL)
    {
        return;
    }

    identifierfield->blockSignals(true);
    identifierfield->setText(QString::fromUtf8 (l2u(p_visible->identifier()).c_str()));
    identifierfield->blockSignals(false);
    set_sizevisible();
    set_fontvisible();
    p_foregroundcolourbutton->blockSignals(true);
    hk_colour c=p_visible->foregroundcolour();
    QColor qcol(c.red(),c.green(),c.blue());
    p_foregroundcolourbutton->setColor(qcol);
    p_foregroundcolourbutton->blockSignals(false);

    p_backgroundcolourbutton->blockSignals(true);
    c=p_visible->backgroundcolour();
    qcol.setRgb(c.red(),c.green(),c.blue());
    p_backgroundcolourbutton->setColor(qcol);
    p_backgroundcolourbutton->blockSignals(false);
    onopenactionbutton->setText(p_visible->on_open_action().size()>0?settxt:notsettxt);
    oncloseactionbutton->setText(p_visible->on_close_action().size()>0?settxt:notsettxt);

}


void hk_kdereportproperty::set_sizevisible(void)
{
    if (p_visible==NULL)
    {
        return;
    }
    int w=0;
    int h=0;
    if (p_visible->type()!=hk_visible::report)
    {
        w=p_visible->width();
        h=p_visible->height();
    }
    else
    {
        w=p_report->designwidth();
        h=p_report->designheight();
    }
    widthfield->blockSignals(true);
    if (p_report->sizetype()==hk_presentation::absolute&&hk_class::measuresystem()==hk_class::inch)
    widthfield->setValue((int)(cm2inch(w)+0.5));
    else     widthfield->setValue(w);
    widthfield->blockSignals(false);
    if (p_visible->type()!=hk_visible::reportsection)
    {
        heightfield->blockSignals(true);
        if (p_report->sizetype()==hk_presentation::absolute&&hk_class::measuresystem()==hk_class::inch)
            heightfield->setValue((int)(cm2inch(h)+0.5));
        else
            heightfield->setValue(h);
        heightfield->blockSignals(false);
    }

    if (xfield!=NULL)
    {
        xfield->blockSignals(true);
        if (p_report->sizetype()==hk_presentation::absolute&&hk_class::measuresystem()==hk_class::inch)
            xfield->setValue((int)(cm2inch(p_visible->x())+0.5));
        else
            xfield->setValue(p_visible->x());
        xfield->blockSignals(false);
    }

    if (yfield!=NULL)
    {
        yfield->blockSignals(true);
        if (p_report->sizetype()==hk_presentation::absolute&&hk_class::measuresystem()==hk_class::inch)
            yfield->setValue((int)(cm2inch(p_visible->y())+0.5));
        else
            yfield->setValue(p_visible->y());
        yfield->blockSignals(false);
    }
 

}


void hk_kdereportproperty::set_fontvisible(void)
{

    if (p_visible==NULL)
    {
        return;
    }
    hk_font f=p_visible->font();

    fontsizefield->blockSignals(true);
    fontsizefield->setValue(f.fontsize());
    fontsizefield->blockSignals(false);

    fontfield->blockSignals(true);
    fontfield->setEditText(QString::fromUtf8(l2u(f.fontname()).c_str()));
    fontfield->blockSignals(false);
    boldfield->blockSignals(true);
    boldfield->setCurrentIndex(f.bold()?0:1);
    boldfield->blockSignals(false);
    italicfield->blockSignals(true);
    italicfield->setCurrentIndex(f.italic()?0:1);
    italicfield->blockSignals(false);


}


void hk_kdereportproperty::set_reportdatavisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_reportdatavisible");
#endif
    hk_reportdata* d =dynamic_cast <hk_reportdata*>(p_visible);
    if (d==NULL) return;
    beforerowchangeaction->setText(d->before_row_change_action().size()>0?settxt:notsettxt);
    afterrowchangeaction->setText(d->after_row_change_action().size()>0?settxt:notsettxt);
    onprintaction->setText(d->on_print_action().size()>0?settxt:notsettxt);
    beforerowchangeaction->setEnabled(true);
    afterrowchangeaction->setEnabled(true);
    onprintaction->setEnabled(true);

    valuefield->blockSignals(true);
    valuefield->setText(QString::fromUtf8(l2u(d->data()).c_str()));
    valuefield->blockSignals(false);
    alignmentfield->blockSignals(true);
    int a=0;
    switch (d->alignment())
    {
        case hk_dsdatavisible::alignleft :    a=0;
        break;
        case hk_dsdatavisible::aligncenter :      a=1;
        break;
        case hk_dsdatavisible::alignright :   a=2;
        break;
        case hk_dsdatavisible::aligndefault:      a=3;break;
        default:;

    }
    alignmentfield->setCurrentIndex(a);
    alignmentfield->blockSignals(false);

    wordbreakfield->blockSignals(true);
    wordbreakfield->setCurrentIndex(d->linebreak()?0:1);
    wordbreakfield->blockSignals(false);

    dynamicheightfield->blockSignals(true);
    dynamicheightfield->setCurrentIndex(d->dynamic_height()?0:1);
    dynamicheightfield->blockSignals(false);

    leftlinefield->blockSignals(true);
    leftlinefield->setCurrentIndex(d->leftline()?0:1);
    leftlinefield->blockSignals(false);

    rightlinefield->blockSignals(true);
    rightlinefield->setCurrentIndex(d->rightline()?0:1);
    rightlinefield->blockSignals(false);

    toplinefield->blockSignals(true);
    toplinefield->setCurrentIndex(d->topline()?0:1);
    toplinefield->blockSignals(false);

    bottomlinefield->blockSignals(true);
    bottomlinefield->setCurrentIndex(d->bottomline()?0:1);
    bottomlinefield->blockSignals(false);

    diagonallorufield->blockSignals(true);
    diagonallorufield->setCurrentIndex(d->diagonalloru()?0:1);
    diagonallorufield->blockSignals(false);

    diagonallurofield->blockSignals(true);
    diagonallurofield->setCurrentIndex(d->diagonalluro()?0:1);
    diagonallurofield->blockSignals(false);

    columnfield->blockSignals(true);
    columnfield->setEditText(QString::fromUtf8(l2u(d->columnname()).c_str()));
    columnfield->blockSignals(false);

    runningcountfield->blockSignals(true);
    runningcountfield->setCurrentIndex(d->runningcount()?0:1);
    runningcountfield->blockSignals(false);

    separatorfield->blockSignals(true);
    separatorfield->setCurrentIndex(d->use_numberseparator()?0:1);
    separatorfield->blockSignals(false);

    digitfield->blockSignals(true);
    digitfield->setValue(d->commadigits());
    digitfield->blockSignals(false);
    onprintaction->setText(d->on_print_action().size()>0?settxt:notsettxt);

}


void hk_kdereportproperty::set_reportsectionvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_reportsectionvisible");
#endif
    hk_kdereportsection* s =dynamic_cast <hk_kdereportsection*>(p_visible);
    if (s==NULL) return;
    wordbreakfield->blockSignals(true);
    wordbreakfield->setCurrentIndex(s->new_page_after_section()?0:1);
    wordbreakfield->blockSignals(false);
    QString n=QString::fromUtf8(l2u(s->subreportname()).c_str());
    subreportbutton->setText(s->subreportname().size()>0?n:i18n("None"));
    columnfield->blockSignals(true);
    columnfield->setEditText(QString::fromUtf8(l2u(s->columnname()).c_str()));
    columnfield->blockSignals(false);
    sortingfield->blockSignals(true);
    if (s->reportsectionpair())
        sortingfield->setCurrentIndex(s->reportsectionpair()->ascending_order()?0:1);
    else sortingfield->setEnabled(false);
    sortingfield->blockSignals(false);
    heightfield->blockSignals(true);
    if (p_report->sizetype()==hk_presentation::absolute&&hk_class::measuresystem()==hk_class::inch)
    heightfield->setValue((int)(cm2inch(s->offset())));
    else    heightfield->setValue(s->offset());
    heightfield->blockSignals(false);

    s->set_activesection(true);
}


void hk_kdereportproperty::set_reportvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_reportvisible");
#endif
    list<hk_string>* l=p_report->reporttypelist();
    list<hk_string>::iterator it=l->begin();
    int i=0;
    reporttypefield->blockSignals(true);
    reporttypefield->clear();
    while (it!=l->end())
    {
        reporttypefield->addItem(QString::fromUtf8(l2u((*it)).c_str()));
        if (p_report->reporttype()==(*it)) reporttypefield->setCurrentIndex(i);
        it++; i++;
    }
    reporttypefield->blockSignals(false);
    
    
    orientationfield->blockSignals(true);
    orientationfield->setCurrentIndex(p_report->orientation()==hk_report::portrait?0:1);
    orientationfield->setEnabled(p_report->pageformat()!=hk_report::userdefined);
    orientationfield->blockSignals(false);

    pageformatfield->blockSignals(true);
    pageformatfield->setCurrentIndex(p_report->pageformat());
    pageformatfield->blockSignals(false);
    widthfield->setEnabled(p_report->pageformat()==hk_report::userdefined);
    heightfield->setEnabled(p_report->pageformat()==hk_report::userdefined);

    firstpagenumberfield->blockSignals(true);
    firstpagenumberfield->setValue(p_report->firstpagenumber());
    firstpagenumberfield->blockSignals(false);

    leftlinefield->blockSignals(true);
    leftlinefield->setCurrentIndex(p_report->leftline()?0:1);
    leftlinefield->blockSignals(false);

    rightlinefield->blockSignals(true);
    rightlinefield->setCurrentIndex(p_report->rightline()?0:1);
    rightlinefield->blockSignals(false);

    toplinefield->blockSignals(true);
    toplinefield->setCurrentIndex(p_report->topline()?0:1);
    toplinefield->blockSignals(false);

    bottomlinefield->blockSignals(true);
    bottomlinefield->setCurrentIndex(p_report->bottomline()?0:1);
    bottomlinefield->blockSignals(false);

    diagonallorufield->blockSignals(true);
    diagonallorufield->setCurrentIndex(p_report->diagonalloru()?0:1);
    diagonallorufield->blockSignals(false);

    diagonallurofield->blockSignals(true);
    diagonallurofield->setCurrentIndex(p_report->diagonalluro()?0:1);
    diagonallurofield->blockSignals(false);

    set_bordervisible();
    wordbreakfield->blockSignals(true);
    wordbreakfield->setCurrentIndex(p_report->multiplefiles()?0:1);
    wordbreakfield->blockSignals(false);
    onnewpageaction->setText(p_report->on_new_page_action().size()>0?settxt:notsettxt);
    onnewpageaction->setEnabled(true);
}


void hk_kdereportproperty::set_datasourcelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_datasourcelist");
#endif
    if (p_report==NULL) return;
    datasourcefield->blockSignals(true);
    datasourcefield->clear();
    datasourcefield->addItem("");

    list<hk_datasource*>* dslist = p_report->datasources();
    if (dslist==NULL)
    {
        datasourcefield->blockSignals(false);
        return;
    }
    list<hk_datasource*>::iterator it=dslist->begin();
    int item=1;
    while (it!=dslist->end())
    {
        QString name= QString::fromUtf8(l2u(p_report->unique_datasourcename((*it)->presentationnumber())).c_str());
        datasourcefield->addItem(name);
        hk_dsvisible* ds = dynamic_cast <hk_dsvisible*>(p_visible);
        if (ds!=NULL)
        {
            if (ds->datasource()==(*it)) datasourcefield->setCurrentIndex(item);

        }

        it++; item++;
    }
    datasourcefield->blockSignals(false);
}


void hk_kdereportproperty::set_dsvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_dsvisible");
#endif
    set_datasourcelist();
    hk_dsvisible* ds= dynamic_cast <hk_dsvisible*>(p_visible);
    if (!ds)return;
    beforerowchangeaction->setText(ds->before_row_change_action().size()>0?settxt:notsettxt);
    afterrowchangeaction->setText(ds->after_row_change_action().size()>0?settxt:notsettxt);

}


void hk_kdereportproperty::focus_resized(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::focus_resized");
#endif
    set_visible();
}


void hk_kdereportproperty::set_objectvisible(void)
{
// sets values from the property editor  in the object
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_objectvisible");
#endif
    if (p_visible==NULL)return;
    if (p_visible->type()!=hk_visible::reportsection)
    {
        QColor qcol=p_foregroundcolourbutton->color();
        hk_colour c(qcol.red(),qcol.green(),qcol.blue());
        p_visible->set_foregroundcolour(c);

        qcol=p_backgroundcolourbutton->color();
        c.set_colour(qcol.red(),qcol.green(),qcol.blue());
        p_visible->set_backgroundcolour(c);

    }

   // set_visible();

}


void hk_kdereportproperty::set_objectfontvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_objectfontvisible");
#endif
    if (p_visible==NULL)return;
    hk_string ps =  u2l(fontfield->currentText().toUtf8().data());
    hk_font f(u2l(fontfield->currentText().toUtf8().data()),fontsizefield->value());
    f.set_bold(boldfield->currentIndex()==0);
    f.set_italic(italicfield->currentIndex()==0);
    p_visible->set_font(f);

}


void hk_kdereportproperty::set_objectsizevisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_objectsizevisible");
#endif
    if (p_visible==NULL)return;
    hk_reportsection* rs = dynamic_cast <hk_reportsection*>(p_visible);
    if (rs) 
    {
       if (p_report->sizetype()==hk_presentation::absolute&&hk_class::measuresystem()==hk_class::inch)
           rs->set_offset(int(inch2cm(heightfield->value())));
       else
           rs->set_offset(heightfield->value());
    }
    else
    {   
        if (p_report->sizetype()==hk_presentation::absolute&&hk_class::measuresystem()==hk_class::inch)
            p_visible->set_size((unsigned int)(inch2cm(widthfield->value())+0.5),
                                (unsigned int)(inch2cm(heightfield->value())+0.5));
        else
            p_visible->set_size(widthfield->value(),heightfield->value());
    }
} 


void hk_kdereportproperty::set_objectpositionvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_objectpositionvisible");
#endif
    if (p_visible==NULL)return;
   if (xfield!=NULL)
    {
        if (p_report->sizetype()==hk_presentation::absolute&&hk_class::measuresystem()==hk_class::inch)
            p_visible->set_position((unsigned int)(inch2cm(xfield->value())+0.5),
                                    (unsigned int)(inch2cm(yfield->value())+0.5));
        else
            p_visible->set_position(xfield->value(),yfield->value());
    }
 }

void hk_kdereportproperty::set_objectdsvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_objectdsvisible");
#endif
    if (p_visible==NULL)return;

    list<hk_datasource*>* dslist = p_report->datasources();
    if (dslist==NULL) return;
    list<hk_datasource*>::iterator it=dslist->begin();
    hk_dsvisible* ds = dynamic_cast <hk_dsvisible*>(p_visible);
    if (ds==NULL) return;
    if (datasourcefield->currentText()=="") ds->set_presentationdatasource(-1);
    while (it!=dslist->end())
    {
        QString name= QString::fromUtf8(l2u(p_report->unique_datasourcename((*it)->presentationnumber())).c_str());
        if (name==datasourcefield->currentText())
        {
            ds->set_presentationdatasource( (*it)->presentationnumber() );
        }
        it++;
    }

}


void hk_kdereportproperty::set_objectreportvisible(void)
{
   if (firstpagenumberfield) p_report->set_firstpagenumber(firstpagenumberfield->value());
   if (!pageformatfield) return;
   if ((hk_report::pageformattype)pageformatfield->currentIndex()==hk_report::userdefined)
   {
       p_report->set_pageformat(widthfield->value(),heightfield->value(),true);
       p_report->set_orientation(hk_report::portrait);

    }
    else
    {
        if (pageformatfield)p_report->set_pageformat((hk_report::pageformattype)pageformatfield->currentIndex());
        if (orientationfield)p_report->set_orientation(orientationfield->currentIndex()==0?hk_report::portrait:hk_report::landscape);
    }
    
    widthfield->setEnabled(p_report->pageformat()==hk_report::userdefined);
    heightfield->setEnabled(p_report->pageformat()==hk_report::userdefined);
    if (orientationfield) orientationfield->setEnabled(p_report->pageformat()!=hk_report::userdefined);
    p_report->set_leftline(leftlinefield->currentIndex()==0);
    p_report->set_rightline(rightlinefield->currentIndex()==0);
    p_report->set_topline(toplinefield->currentIndex()==0);
    p_report->set_bottomline(bottomlinefield->currentIndex()==0);
    p_report->set_diagonalluro(diagonallurofield->currentIndex()==0);
    p_report->set_diagonalloru(diagonallorufield->currentIndex()==0);
    p_report->set_multiplefiles(wordbreakfield->currentIndex()==0);
    p_report->set_borders(borderleftfield->value(),borderrightfield->value(),bordertopfield->value(),borderbottomfield->value());
    p_report->set_sizetype(runningcountfield->currentIndex()==0?hk_presentation::relative:hk_presentation::absolute);
    
    set_bordervisible();
}


void hk_kdereportproperty::set_objectreportsectionvisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_objectreportsectionvisible");
#endif
    if (p_visible==NULL) return;
    hk_reportsection* rs = dynamic_cast <hk_reportsection*>(p_visible);
    if (rs==NULL) return;
    rs->set_new_page_after_section(wordbreakfield->currentIndex()==0);
    if (rs->reportsectionpair())
        rs->reportsectionpair()->set_ascending_order(sortingfield->currentIndex()==0);
   
}


void hk_kdereportproperty::set_objectreportdatavisible(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdereportproperty::set_objectreportdatavisible");
#endif
    if (p_visible==NULL) return;
    hk_kdereportdata* rd = dynamic_cast <hk_kdereportdata*>(p_visible);
    if (rd==NULL) return;
    rd->set_data(u2l(valuefield->text().toUtf8().data()));
    int i=alignmentfield->currentIndex();
    hk_dsdatavisible::alignmenttype at=hk_dsdatavisible::alignleft;
    switch (i)
    {
        case 1 : at=hk_dsdatavisible::aligncenter;break;
        case 2 : at=hk_dsdatavisible::alignright;break;
        case 3 : at=hk_dsdatavisible::aligndefault;break;
        default:;

    }
    rd->set_alignment(at);

    rd->set_linebreak(wordbreakfield->currentIndex()==0);
    rd->set_dynamic_height(dynamicheightfield->currentIndex()==0);
    rd->set_leftline(leftlinefield->currentIndex()==0);
    rd->set_rightline(rightlinefield->currentIndex()==0);
    rd->set_topline(toplinefield->currentIndex()==0);
    rd->set_bottomline(bottomlinefield->currentIndex()==0);
    rd->set_diagonalluro(diagonallurofield->currentIndex()==0);
    rd->set_diagonalloru(diagonallorufield->currentIndex()==0);
    rd->set_columnname(u2l(columnfield->currentText().toUtf8().data()));
    rd->set_runningcount(runningcountfield->currentIndex()==0);
    rd->set_numberformat(separatorfield->currentIndex()==0,digitfield->value());

    rd->repaint();
}


void hk_kdereportproperty::subreportbutton_clicked()
{
    hk_reportsection* s=dynamic_cast<hk_reportsection*>(p_visible);
    
    if (s==NULL) return;
    hk_kdesubreportdialog* d = new hk_kdesubreportdialog(s,p_report,0,true);
    d->exec();
    delete d;
    QString n=QString::fromUtf8(l2u(s->subreportname()).c_str());
    subreportbutton->setText(s->subreportname().size()>0?n:i18n("None"));
}

void hk_kdereportproperty::set_bordervisible(void)
{
    if (p_report->sizetype()==hk_presentation::relative)
    {
     if (borderleftfield) borderleftfield->set_displaytype(hk_kdespinbox::percent);
     if (borderrightfield) borderrightfield->set_displaytype(hk_kdespinbox::percent);
     if (bordertopfield) bordertopfield->set_displaytype(hk_kdespinbox::percent);
     if (borderbottomfield) borderbottomfield->set_displaytype(hk_kdespinbox::percent);
    }
    else
    {
     if (borderleftfield) borderleftfield->set_displaytype(hk_kdespinbox::cm);
     if (borderrightfield) borderrightfield->set_displaytype(hk_kdespinbox::cm);
     if (bordertopfield) bordertopfield->set_displaytype(hk_kdespinbox::cm);
     if (borderbottomfield) borderbottomfield->set_displaytype(hk_kdespinbox::cm);
    }
    borderleftfield->blockSignals(true);
    borderleftfield->setValue(p_report->border_left());
    borderleftfield->blockSignals(false);

    borderrightfield->blockSignals(true);
    borderrightfield->setValue(p_report->border_right());
    borderrightfield->blockSignals(false);

    bordertopfield->blockSignals(true);
    bordertopfield->setValue(p_report->border_top());
    bordertopfield->blockSignals(false);

    borderbottomfield->blockSignals(true);
    borderbottomfield->setValue(p_report->border_bottom());
    borderbottomfield->blockSignals(false);

}

void hk_kdereportproperty::identifier_changed(void)
{

hk_string t=u2l(identifierfield->text().toUtf8().data());
if (p_visible->identifier()!=t) p_visible->set_identifier(t);


}


void hk_kdereportproperty::beforerowchange_action_clicked(void)
{
  beforerowchange_action_clicked(0,"");
}

void hk_kdereportproperty::beforerowchange_action_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
  if (!ds) return;
  hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
  d->setWindowModality(Qt::ApplicationModal);
  d->set_code(ds->before_row_change_action(),false);
  int r=d->exec(rownumber,warningmessage);
  if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
  {
    ds->set_before_row_change_action(d->code());
  }
  set_dsvisible();
  delete d;
}


void hk_kdereportproperty::afterrowchange_action_clicked(int rownumber,const hk_string& warningmessage)
{
    hk_dsvisible* ds=dynamic_cast<hk_dsvisible*>(p_visible);
    if (!ds) return;
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(ds->after_row_change_action(),false);
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      ds->set_after_row_change_action(d->code());
    }
    set_dsvisible();
    delete d;
}

void hk_kdereportproperty::afterrowchange_action_clicked(void)
{
  afterrowchange_action_clicked(0,"");
}

void hk_kdereportproperty::onprint_action_clicked(int rownumber,const hk_string& warningmessage)
{
  hk_reportdata* ds=dynamic_cast<hk_reportdata*>(p_visible);
  if (!ds) return;
  hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
  d->setWindowModality(Qt::ApplicationModal);
  d->set_code(ds->on_print_action(),false);
  int r=d->exec(rownumber,warningmessage);
  if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
  {
    ds->set_on_print_action(d->code());
  }
  set_dsvisible();
  set_reportdatavisible();
  delete d;
}

void hk_kdereportproperty::onprint_action_clicked(void)
{
  onprint_action_clicked(0,"");
}

void hk_kdereportproperty::onprintnewpage_action_clicked(int rownumber, const hk_string& warningmessage)
{
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(p_report->on_new_page_action(),false);
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      p_report->set_on_new_page_action(d->code());
    }
    set_reportvisible();
    delete d;
}

void hk_kdereportproperty::onprintnewpage_action_clicked(void)
{
  onprintnewpage_action_clicked(0,"");
}

void hk_kdereportproperty::closeactionbutton_clicked(void)
{
  closeactionbutton_clicked(0,"");
}

void hk_kdereportproperty::closeactionbutton_clicked(int rownumber,const hk_string& warningmessage)
{
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(p_visible->on_close_action(),false);
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      p_visible->set_on_close_action(d->code());
    }
    set_visible();
    delete d;
}

void hk_kdereportproperty::openactionbutton_clicked(void)
{
  openactionbutton_clicked(0,"");
}

void hk_kdereportproperty::openactionbutton_clicked(int rownumber,const hk_string& warningmessage)
{
    hk_kdeinterpreterdialog* d = new hk_kdeinterpreterdialog(0,0);
    d->setWindowModality(Qt::ApplicationModal);
    d->set_code(p_visible->on_open_action(),false);
    int r=d->exec(rownumber,warningmessage);
    if (r==hk_kdeinterpreterdialog::Accepted&&d->has_changed())
    {
      p_visible->set_on_open_action(d->code());
    }
    set_visible();
    delete d;
}

hk_visible* hk_kdereportproperty::object()
{
  return p_visible;
}


bool hk_kdereportproperty::eventFilter(QObject* object,QEvent* event)
{
   QLineEdit* edit=dynamic_cast<QLineEdit*>(object);
   
   if (edit&&event->type()==QEvent::KeyPress)
   {
     QKeyEvent*ev=(QKeyEvent*)event;
     if (ev->key()==Qt::Key_F2)
       use_editor(edit);
   } 

   return hk_kdereportpropertybase::eventFilter(object,event);
}

void hk_kdereportproperty::use_editor(QLineEdit* e)
{
    if (!e) return;
    hk_kdefilterdialog* f = new hk_kdefilterdialog(hk_kdefilterdialog::editor,this,0,true);
    
    f->textfield->setPlainText(e->text());
    if (f->exec()==QDialog::Accepted)
        e->setText(f->textfield->toPlainText());
    delete f;
}

