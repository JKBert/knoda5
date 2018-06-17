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

#include "hk_kdepreferencesdialog.h"
#include <hk_class.h>
#include <hk_drivermanager.h>
#include <hk_report.h>
#include <hk_visible.h>
#include <hk_dsdatavisible.h>
#include <hk_kdedblistview.h>
#include <qfontdatabase.h>

#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qtabwidget.h>
#include <qspinbox.h>
#include <QKeyEvent>
#include <klocale.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <kglobal.h>
#include <khelpclient.h>

hk_kdepreferencesdialog::hk_kdepreferencesdialog( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl )
: hk_kdepreferencesdialogbase( parent, name, modal, fl )
{

    datelabel->setText( i18n( "Date format:"  ) );
    timelabel->setText( i18n( "Time format:"  ) );
    datetimelabel->setText( i18n( "Date&Time format:"  ) );
    setWindowTitle( i18n( "Preferences"  ) );
    regionaltab->setTabText(regionaltab->indexOf(regionalpage),i18n("&Regional"));
    regionaltab->setTabText(regionaltab->indexOf(printerpage),i18n("&Printer"));
    regionaltab->setTabText(regionaltab->indexOf(generalpage),i18n("&General"));
    driverpathlabel->setText( i18n( "Driverpath:"  ) );
    printcommandlabel->setText( i18n( "Printcommand:"  ) );
    bepedanticlabel->setText( i18n( "Show all questions:"  ) );
    maximizedlabel->setText( i18n( "Show maximized:"  ) );
    storepasswordlabel->setText( i18n( "Store password:"  ) );
    fontlabel->setText(i18n("Default Font:"));
    defaultdriverlabel->setText(i18n("Default driver:"));
    QFontDatabase*p_fontdatabase = new QFontDatabase();
    list<hk_string>* flist=hk_font::fontlist();
    QStringList newstrlist;
    list<hk_string>::iterator fit=flist->begin();
    while(fit!=flist->end())
    {
        QString f=QString::fromUtf8(l2u(*fit).c_str());
        newstrlist.append(f);
        ++fit;
    }

    fontfield->insertItems(fontfield->count(), newstrlist);
    delete p_fontdatabase;
    QString left=i18n("Left");
    QString right=i18n("Right");
    QString center=i18n("Center");
    textalignmentfield->addItem(left);
    textalignmentfield->addItem(center);
    textalignmentfield->addItem(right);

    numberalignmentfield->addItem(left);
    numberalignmentfield->addItem(center);
    numberalignmentfield->addItem(right);
    textalignmentlabel->setText(i18n("Text alignment:"));
    numberalignmentlabel->setText(i18n("Number alignment:"));
    separatorlabel->setText(i18n("Separator:"));
    precisionlabel->setText(i18n("Precision:"));
    QString yes=i18n("Yes");
    QString no=i18n("No");
    bepedanticfield->addItem(yes);
    bepedanticfield->addItem(no);
    storepasswordfield->addItem(yes);
    storepasswordfield->addItem(no);
    maximizedfield->addItem(yes);
    maximizedfield->addItem(no);
    separatorfield->addItem(yes);
    separatorfield->addItem(no);

    use_singleclickfield->addItem(i18n("Singleclick"));
    use_singleclickfield->addItem(i18n("Doubleclick"));
    KSharedConfigPtr c=KGlobal::config();
    KConfigGroup cg = c->group("Preferencesdialog");
    const QRect& rrect=QRect(0,0,500,300);   
    QRect g;
    g=cg.readEntry("Geometry",rrect);
    setGeometry(g);
    
    timefield->setText(QString::fromUtf8(l2u(hk_class::defaulttimeformat()).c_str())),
    datefield->setText(QString::fromUtf8(l2u(hk_class::defaultdateformat()).c_str())),
    datetimefield->setText(QString::fromUtf8(l2u(hk_class::defaultdatetimeformat()).c_str()));
    localefield->setText(QString::fromUtf8(l2u(hk_class::locale()).c_str()));
    printcommandfield->setText(QString::fromUtf8(l2u(hk_report::printcommand()).c_str()));
    embedfontsfield->setCurrentIndex(hk_report::embedfonts()?0:1);
    driverpathfield->setText(QString::fromUtf8(l2u(hk_drivermanager::path()).c_str()));
    bepedanticfield->setCurrentIndex(hk_class::showpedantic()?0:1);
    maximizedfield->setCurrentIndex(hk_visible::open_maximized_windows()?0:1);
    fontfield->setEditText(QString::fromUtf8(l2u(hk_font::defaultfontname()).c_str()));
    fontsizefield->setValue(hk_font::defaultfontsize());
    precisionfield->setValue(hk_dsdatavisible::defaultprecision());
    separatorfield->setCurrentIndex(hk_dsdatavisible::defaultuse_numberseparator()?0:1);
    sizetypefield->setCurrentIndex(hk_presentation::defaultsizetype()==hk_presentation::relative?0:1);
    measurefield->setCurrentIndex(hk_class::measuresystem()==hk_class::cm?0:1);
    use_singleclickfield->setCurrentIndex(hk_kdedblistview::use_singleclick()?0:1);
    snap2gridxfield->setValue(hk_presentation::snap2gridx());
    snap2gridyfield->setValue(hk_presentation::snap2gridy());
    automaticupdatefield->setCurrentIndex(hk_class::default_automatic_data_update()?0:1);
    cg = c->group("Preferences");
    bool gc;
    gc=cg.readEntry("StorePassword",false);
    storepasswordfield->setCurrentIndex(gc?0:1);

    vector<hk_string>* drv=hk_drivermanager::driverlist();
    vector<hk_string>::iterator it;
    int di=0;
    for(it=drv->begin();it!=drv->end();it++)
    {
        defaultdriverfield->addItem(QString::fromUtf8(l2u((*it)).c_str()));
        if ((*it)==hk_class::defaultdriver())
            defaultdriverfield->setCurrentIndex(di);
        di++;
    }
    int i=0;
    switch (hk_visible::defaulttextalignment())
    {
        case hk_visible::aligncenter: i=1;break;
        case hk_visible::alignright: i=2;break;
        default: i=0;
    }
    textalignmentfield->setCurrentIndex(i);

    switch (hk_visible::defaultnumberalignment())
    {
        case hk_visible::aligncenter: i=1;break;
        case hk_visible::alignright: i=2;break;
        default: i=0;
    }
    numberalignmentfield->setCurrentIndex(i);
}


hk_kdepreferencesdialog::~hk_kdepreferencesdialog()
{
// no need to delete child widgets, Qt does it all for us
}


void hk_kdepreferencesdialog::driverpathbutton_clicked()
{
//cout <<"driverpathbutton_clicked"<<endl;
    hk_drivermanager::show_driverselectdialog();
    driverpathfield->setText(hk_drivermanager::path().c_str());

}



void hk_kdepreferencesdialog::accept(void)
{
    KSharedConfigPtr c=KGlobal::config();
    KConfigGroup cg =  c->group("Preferencesdialog");
    cg.writeEntry("Geometry",geometry());

    hk_class::set_defaultdatetimeformat(
            u2l(timefield->text().toUtf8().data()),
            u2l(datefield->text().toUtf8().data()),
            u2l(datetimefield->text().toUtf8().data())
            );
    hk_class::set_locale(u2l(localefield->text().toUtf8().data()));
        hk_class::set_showpedantic( bepedanticfield->currentIndex()==0);
        hk_visible::set_open_maximized_windows(maximizedfield->currentIndex()==0);
        hk_report::set_printcommand(u2l(printcommandfield->text().toUtf8().data()));
        hk_report::set_embedfonts(embedfontsfield->currentIndex()==0);
        hk_drivermanager::set_path(u2l(driverpathfield->text().toUtf8().data()));
        hk_font::set_defaultfont(u2l(fontfield->currentText().toUtf8().data()),fontsizefield->value());
    hk_presentation::set_snap2gridx(snap2gridxfield->value());
    hk_presentation::set_snap2gridy(snap2gridyfield->value());
    hk_visible::alignmenttype a=hk_visible::alignleft;
    hk_class::set_default_automatic_data_update(automaticupdatefield->currentIndex()==0);

    switch (textalignmentfield->currentIndex())
        {
            case 1: a=hk_visible::aligncenter;break;
            case 2: a=hk_visible::alignright;break;
            default:;
        }
    hk_visible::set_defaulttextalignment(a);
    hk_presentation::set_defaultsizetype(sizetypefield->currentIndex()==0?hk_presentation::relative:hk_presentation::absolute);
    hk_class::set_measuresystem(measurefield->currentIndex()==0?hk_class::cm:hk_class::inch);

    a=hk_visible::alignleft;
    switch (numberalignmentfield->currentIndex())
        {
            case 1: a=hk_visible::aligncenter;break;
            case 2: a=hk_visible::alignright;break;
            default:;
        }
    hk_visible::set_defaultnumberalignment(a);
    hk_dsdatavisible::set_defaultnumberformat(separatorfield->currentIndex()==0,precisionfield->value());
    hk_class::set_defaultdriver(u2l(defaultdriverfield->currentText().toUtf8().data()));

    hk_class::save_preferences();
    hk_kdedblistview::set_use_singleclick(use_singleclickfield->currentIndex()==0);
    cg=c->group("Preferences");
    cg.writeEntry("StorePassword",(storepasswordfield->currentIndex()==0));
    cg.writeEntry("UseSingleclick",(use_singleclickfield->currentIndex()==0));

    QDialog::accept();
}


void hk_kdepreferencesdialog::keyPressEvent ( QKeyEvent * e )
{
  if (e->key()==Qt::Key_F1) help_clicked();
}



void hk_kdepreferencesdialog::help_clicked()
{
  KHelpClient::invokeHelp("options");
}

