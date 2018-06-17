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

#include "hk_kdeform.h"
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <kstatusbar.h>
#include <qstatusbar.h>
#include <hk_kdesimpleform.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include <kglobal.h>
#include <kiconloader.h>
#include <qpushbutton.h>
#include "hk_kderowselector.h"
#include "hk_kdeproperty.h"
#include <hk_class.h>
#include <hk_database.h>
#include <hk_dsvisible.h>
#include <hk_visible.h>
#include <hk_dsdatavisible.h>
#include <hk_datasource.h>
#include <hk_connection.h>
#include <kconfiggroup.h>
#include <kmenubar.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <klocale.h>
#include <kconfig.h>
#include <kapplication.h>
#include <kservice.h>

/*
 *  Constructs a hk_kdeform which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
hk_kdeform::hk_kdeform( QWidget* parent,  const char* /* name */, Qt::WFlags fl)
: KParts::MainWindow( parent, fl ) ,hk_class()
{
#ifdef HK_DEBUG
    hkclassname("Hauptformular");
    hkdebug("hk_kdeform::hk_kdeform");
#endif
    p_partmanager=new KParts::PartManager(this);
    setObjectName("hk_kdeform");
    KSharedConfigPtr c=KGlobal::config();
    const QRect& rrect = QRect(0,0,800,600);
    KConfigGroup cg = c->group("Form-SDI");
    QRect g;
    g=cg.readEntry("Geometry",rrect);
    setGeometry(g);

    setXMLFile("hk_kdeform.rc");
    KService::Ptr service = KService::serviceByDesktopName("hk_kde5formpart");
   
    if (!service || 
      !(p_part=service->createInstance<KParts::ReadWritePart>(this,this,QVariantList())))
    {
     show_warningmessage(hk_translate("Fatal error! Form part could not be loaded!\nThis is a installation error. Check your installation!\
 Did you install knoda into the correct directory? Program will exit now..."));
     exit(1);
    }
    p_partmanager->addPart(p_part);
    p_form = (hk_kdeformpartwidget*)p_part->widget();
    setCentralWidget(p_form);
    
    connect(p_partmanager,SIGNAL(partRemoved(KParts::Part*)),this, SLOT(part_removed()));
    connect(p_partmanager,SIGNAL(activePartChanged(KParts::Part*)),this, SLOT(createGUI(KParts::Part*)));
    p_closeaction=new KAction(QIcon::fromTheme("window-close"),i18n("&Close"),actionCollection());
    actionCollection()->addAction("closeform",p_closeaction);
    connect(p_closeaction,SIGNAL(triggered()),this,SLOT(close_form()));
    createGUI(p_part);
    setAutoSaveSettings("Form-SDI");
}


/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdeform::~hk_kdeform()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::~hk_kdeform");
#endif
 if (p_form) delete p_form;
 p_form=NULL;
 delete p_partmanager;
}

void hk_kdeform::part_removed(void)
{
   cerr <<"hk_kdeform::part_removed"<<endl;

   p_part=NULL;
   p_form=NULL;
   close();
}

hk_presentation::enum_mode  hk_kdeform::mode(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::mode");
#endif
    return p_form->mode();
}


void    hk_kdeform::set_mode(hk_presentation::enum_mode s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::set_mode(s)");
#endif
    p_form->set_mode(s);
}


void hk_kdeform::set_designmode(void)
{
    set_mode(hk_presentation::designmode);
}


void hk_kdeform::set_viewmode(void)
{
    set_mode(hk_presentation::viewmode);
}


hk_dsgrid*      hk_kdeform::new_grid(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::new_grid");
#endif
    if (!p_form) return NULL;
    hk_dsgrid* d=p_form->new_grid();
    return d;
}


hk_dslineedit*  hk_kdeform::new_lineedit(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::new_field");
#endif
    if (!p_form) return NULL;

    hk_dslineedit* d=p_form->new_lineedit();
    return d;
}


hk_dsboolean*   hk_kdeform::new_bool(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::new_bool");
#endif
    if (!p_form) return NULL;
    hk_dsboolean* d=p_form->new_bool();
    return d;
}


hk_dsmemo*  hk_kdeform::new_memo(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::new_memo");
#endif
    if (!p_form) return NULL;
    hk_dsmemo* d=p_form->new_memo();
    return d;
}


hk_button*      hk_kdeform::new_button(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::new_button");
#endif
    if (!p_form) return NULL;
    hk_button* d=p_form->new_button();
    return d;
}


hk_dsrowselector*       hk_kdeform::new_rowselector(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::new_rowselector");
#endif
    if (!p_form) return NULL;
    hk_dsrowselector* d=p_form->new_rowselector();
    return d;
}


hk_subform*       hk_kdeform::new_subform(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::new_subform");
#endif
    if (!p_form) return NULL;
    return p_form->new_subform();
}


void        hk_kdeform::set_formsize(unsigned int width, unsigned int height)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::set_formsize");
#endif
    if (!p_form) return;
    p_form->set_formsize(width,height);
}


void    hk_kdeform::set_database(hk_database* db)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::database(db)");
#endif
    if (!p_form) return;

    p_form->set_database(db);
}


hk_database*        hk_kdeform::database(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::database");
#endif
    if (!p_form) return NULL;
    return p_form->database();
}


list<hk_datasource*>*   hk_kdeform::datasources(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::datasources");
#endif
    if (!p_form) return NULL;
    return p_form->datasources();
}


long    hk_kdeform::new_datasource(const hk_string& name,datasourcetype dt)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::new_datasource(name,query)");
#endif
    if (!p_form) return -1;
    return p_form->new_datasource(name,dt);
}


hk_datasource*  hk_kdeform::get_datasource(long nr)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::get:datasource(nr)");
#endif
    if (!p_form) return NULL;
    return p_form->get_datasource(nr);
}






bool    hk_kdeform::set_presentationdatasource(long n, bool r)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::set_presentationdatasource(n)");
#endif
    if (!p_form) return false;
    bool res=true;
    if (r) res= p_form->set_presentationdatasource(n,false);
    return res;
}


long    hk_kdeform::presentationdatasource(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::presentationdatasource(n)");
#endif
    if (!p_form) return -1;
    return p_form->presentationdatasource();
}


void hk_kdeform::closeEvent ( QCloseEvent* e)
{

#ifdef HK_DEBUG
    hkdebug("hk_kdeform::closeEvent");
//set_generaldebug(true);
#endif
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("Form-SDI");
  cg.writeEntry("Geometry",geometry());

   emit signal_closed(this);

    KParts::MainWindow::closeEvent(e);
}







void hk_kdeform::savedata(ofstream& s)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::savedata(s)");
#endif
    if (!p_form) return;
    p_form->savedata(s);

}


void hk_kdeform::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::loaddata");
#endif
    if (!p_form) return;
    p_form->loaddata(definition);
}


void hk_kdeform::load_form(const hk_string& n)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::load_form");
#endif
    if (!p_form) return;

    if (database()==NULL)
    {
        hk_class::show_warningmessage(hk_class::hk_translate("Form error: No database defined!"));
        return ;

    }
//  p_form->formname(n);
    p_form->load_form(n);
    set_caption();
}


void hk_kdeform::set_autoclose(bool c)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::set_autoclose");
#endif
    if (!p_form) return;
    p_form->set_autoclose(c);
}


void hk_kdeform::save_form_when_changed(void)
{

   if (p_form &&p_form->simpleform()->has_changed())
    {
    if (propertyeditor())propertyeditor()->identifier_changed();
        p_form->simpleform()->save_form();
    }

}


void    hk_kdeform::mousePressEvent(QMouseEvent*m)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeform::mousePressEvent");
#endif
    QWidget::mousePressEvent(m);

}




hk_kdesimpleform* hk_kdeform::simpleform(void)
{
    if (!p_form) return NULL;
    return p_form->simpleform();
}







void hk_kdeform::set_caption(void)
{

    if (!p_form) return;
    QString formname=i18n( "Form - "  );
    hk_datasource* d=p_form->simpleform()->datasource();
    if (d!=NULL)
    {
        if (0)formname=QString::fromUtf8(l2u(d->database()->connection()->drivername()).c_str())+" "+formname;


    }
     formname+=QString::fromUtf8 (l2u(
	p_form->simpleform()->label().size()>0?
	p_form->simpleform()->label():
	p_form->simpleform()->hk_presentation::name()).c_str()
);
    setWindowTitle( formname  );
    emit signal_captionChanged(this, formname);


}



hk_kdeproperty* hk_kdeform::propertyeditor(void)
{
    if (!p_form) return NULL;
   return p_form->propertyeditor();
}


void hk_kdeform::grid_partinfocus(hk_kdegridpart*)
{

}

void hk_kdeform::grid_partoutfocus(hk_kdegridpart*)
{

}

void hk_kdeform::close_form(void)
{

    close();
}




