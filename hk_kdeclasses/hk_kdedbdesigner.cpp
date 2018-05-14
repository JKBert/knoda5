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
//$Revision: 1.53 $

#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#error config.h is needed but not included 
#endif
#include <assert.h>
#include <list>
#include "hk_kdedbdesigner.h"
#include "hk_kdeformdatasourcedialog.h"
#include "hk_kderelationdialog.h"
#include "hk_kdeaddtabledialog.h"
#include <hk_presentation.h>
#include <hk_datasource.h>
#include <hk_dsdatavisible.h>
#include <hk_referentialintegrity.h>
#include <qscrollarea.h>
#include <qlayout.h>
#include <qcolor.h>
#include <qpalette.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qbitmap.h>
#include <qmenu.h>
#include <QMouseEvent>
#include <qcursor.h>
#include <qpushbutton.h>
#include <qtablewidget.h>
#include <qcombobox.h>
#include <qapplication.h>
#include <qtooltip.h>
#include <klocale.h>
#include <kglobalsettings.h>
#include <klibloader.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kconfiggroup.h>
#include <kacceleratormanager.h>
#include <kglobal.h>
#include <kactioncollection.h>
#include <kservice.h>
#include <QDrag>
#include <KParts/ReadWritePart>
#include <KAction>

const int designwidth=3000;
const int designheight=3000;

class hk_kdescrollview: public QScrollArea
{
public:
  hk_kdescrollview(hk_kdedbdesigner* parent=0):QScrollArea(parent)
  ,p_designer(parent),p_context(NULL),p_addAction(NULL)
  {
   QWidget* pW = new QWidget(this);

   pW -> resize(designwidth,designheight);
   pW -> setMinimumHeight(designheight);
   pW -> setMinimumWidth(designwidth);
   setWidget(pW);
  }

  virtual void contextMenuEvent(QContextMenuEvent* q)
  {
    if (p_context == NULL)
    {
      p_context = new QMenu(this);
      p_addAction = new QAction(i18n("Add datasource"),this);
      p_context -> addAction(p_addAction);
    }
    QAction* p_res = p_context -> exec(QCursor::pos());
    if (p_res == p_addAction)
      p_designer -> add_datasource();
    q->accept();
  }
  hk_kdedbdesigner* p_designer;
  QMenu* p_context;
  QAction* p_addAction;
};

class hk_kdedbdesignerprivate
{
public:
hk_kdedbdesignerprivate( hk_kdedbdesigner* designer)
{
    p_designer=designer;
    p_layout=new QVBoxLayout(designer);
    p_scrollview=new hk_kdescrollview(designer);
    p_layout->addWidget(p_scrollview);

    QPalette p(p_scrollview->viewport()->palette());
    p.setColor(p_scrollview->viewport()->backgroundRole(),p.mid().color());
    p_scrollview->viewport()->setPalette(p);
    
    p_scrollview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    p_scrollview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    p_presentation=NULL;
    p_datasources=NULL;
}
virtual ~hk_kdedbdesignerprivate()
	{

	}
QVBoxLayout*	p_layout;
hk_kdescrollview* p_scrollview;
hk_kdedbdesigner* p_designer;
hk_presentation* p_presentation;
		list<hk_datasource*>* p_datasources;
list<hk_kdedatasourceframe*> p_windowlist;
list<hk_kdedbrelation*> p_relationlist;
};

class hk_kdefieldlistPrivate
{
public:
  hk_kdefieldlistPrivate():p_cont(NULL),p_Edit(NULL),p_Del(NULL)
  {
  }
  QMenu* p_cont;
  QAction* p_Edit;
  QAction* p_Del;
};


kdedatasourcelabel::kdedatasourcelabel(hk_kdedatasourceframe* w):QLabel(w)
{
  p_drag=false;
  p_startx=p_starty=p_offsetx=p_offsety=0;
  setFocusPolicy(Qt::ClickFocus);
  QPalette p(palette());

  p.setColor(backgroundRole(),p.color(QPalette::Inactive,backgroundRole()));
  p.setColor(foregroundRole(),p.color(QPalette::Inactive,foregroundRole()));
  setPalette(p);
 
  p_datasourceframe=w;
  setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
  KAcceleratorManager::setNoAccel(this);
  setAutoFillBackground(true);
}


void kdedatasourcelabel::mouseMoveEvent(QMouseEvent* event)
{

if (p_drag && event->buttons()==Qt::LeftButton)
  {
   QPoint localPos=event->globalPos();
   int xdiff=localPos.x()-p_startx;
   int ydiff=localPos.y()-p_starty;
   if ((abs(xdiff)>2) || (abs(ydiff)>2))
     {
        QPoint newPos=parentWidget()->pos()+QPoint(xdiff,ydiff);
	if (newPos.x()<0)
	{
	  p_offsetx+=newPos.x();
	  newPos.setX(0);
	}
	else
	if (p_offsetx<0)
	 {
	   p_offsetx+=newPos.x();
	   if (p_offsetx>0)
		{
		newPos.setX(p_offsetx);
		p_offsetx=0;
		}
	   else newPos.setX(0);
	 }
	if (newPos.y()<0)
	 {
		p_offsety+=newPos.y();
		newPos.setY(0);
	 }
	else
	 if (p_offsety<0)
	  {
		p_offsety+=newPos.y();
		if (p_offsety>0)
		 {
		   newPos.setY(p_offsety);
		   p_offsety=0;
		 }
		else newPos.setY(0);
	  }
	parentWidget()->move(newPos.x(),newPos.y());
	p_startx=localPos.x();
	p_starty=localPos.y(); 
    }

   }
}


void kdedatasourcelabel::mousePressEvent(QMouseEvent* event)
	{
	p_datasourceframe->designer()->new_focus(p_datasourceframe);
	if (event->button()==Qt::LeftButton)
	   {
		p_drag=true;
		p_startx=event->globalPos().x();
		p_starty=event->globalPos().y();
		p_offsetx=p_offsety=0;
	   }

	}


void kdedatasourcelabel::mouseReleaseEvent(QMouseEvent* event)
	{
	if (event->button()==Qt::LeftButton)
	   {
		p_drag=false;
	   }

	}
		
	
	
	
	
hk_kdefieldlist::hk_kdefieldlist(hk_kdedatasourceframe* parent):QListWidget(parent),dptr(new hk_kdefieldlistPrivate())
{
  p_datasourceframe=parent;
  setAcceptDrops(true);
  setDragDropMode(QAbstractItemView::InternalMove);
  setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred/*QSizePolicy::Ignored,QSizePolicy::Ignored*/);
  connect(this,SIGNAL(signal_field_doubleclicked(int, const hk_string&)),p_datasourceframe->designer(),SLOT(slot_field_doubleclicked(int, const hk_string&)));
}

hk_kdefieldlist::~hk_kdefieldlist()
{
    delete dptr;
}

QSize hk_kdefieldlist::sizeHint() const
{
  QSize s(sizeHintForColumn(0),QListWidget::sizeHint().height());
  
  return s;
}

void hk_kdefieldlist::clearFocus(void)
{
    parentWidget()->clearFocus();
    QListWidget::clearFocus();
}

void hk_kdefieldlist::focusInEvent ( QFocusEvent * event )
{
    p_datasourceframe->designer()->new_focus(p_datasourceframe);
    QListWidget::focusInEvent(event);
}


void hk_kdefieldlist::mouseDoubleClickEvent(QMouseEvent* event)
{
QString field;
QListWidgetItem* item=itemAt(event->pos());
if (item)
{
   field=item->text();
}
if (p_datasourceframe->designer()->presentation()->presentationtype()!=hk_presentation::qbe)
   p_datasourceframe->edit();
if (!field.isEmpty())
   emit signal_field_doubleclicked(p_datasourceframe->datasource()->presentationnumber(),u2l(field.toUtf8().data()));
}

void hk_kdefieldlist::mouseMoveEvent(QMouseEvent* event)
{
   int mindragdist =KGlobalSettings::dndEventDelay();
    if (event->buttons() & Qt::LeftButton
        &&(
        event->pos().x() >p_dragstartposition.x() +mindragdist
        ||event->pos().x() <p_dragstartposition.x() -mindragdist
        ||event->pos().y() >p_dragstartposition.y() +mindragdist
        ||event->pos().y() <p_dragstartposition.y() -mindragdist

        ))
    {
// dragging begins
        QDrag *p_drag = new QDrag(this);
	    QMimeData *mData = new QMimeData();
	    QString n;
	    n.setNum(p_datasourceframe->datasource()->presentationnumber());
	    n="<DRAG><VUPN>"+n+"</VUPN>\n<VALUE>"+( itemAt(event->pos())?itemAt(event->pos())->text():QString("")) +"</VALUE></DRAG>\n";
	    mData -> setData("application/x-hk_kdedbdesigner",QByteArray(n.toAscii()));
	    p_drag -> setMimeData(mData);
	    p_drag -> exec(Qt::CopyAction);
    }
}

void hk_kdefieldlist::dragEnterEvent(QDragEnterEvent* event)
{
  if(event->mimeData()->hasFormat("application/x-hk_kdedbdesigner")&&
    (p_datasourceframe->designer()->presentation()->presentationtype()==hk_presentation::referentialintegrity
     ||(p_datasourceframe->designer()->presentation()->presentationtype()!=hk_presentation::referentialintegrity &&event->source()!=this )))
    event->acceptProposedAction();
}

void hk_kdefieldlist::dropEvent(QDropEvent* event)
{
  if(!event->mimeData()->hasFormat("application/x-hk_kdedbdesigner")) return;
  hk_string eventtxt=event->mimeData()->data("application/x-hk_kdedbdesigner").data();
  xmlDocPtr doc=xmlParseMemory(eventtxt.c_str(),eventtxt.size());
  xmlNodePtr node=xmlDocGetRootElement(doc);
  long mastervupn;
  hk_string mastertxt;
  if (!(hk_class::get_tagvalue(node,"VUPN",mastervupn)
         && hk_class::get_tagvalue(node,"VALUE",mastertxt)))
  {
    hk_class::show_warningmessage("Error in drag&drop protocol");
    cerr <<eventtxt<<endl;
    return;
  }

  hk_datasource* masterds=p_datasourceframe->designer()->presentation()->get_datasource(mastervupn);
  hk_kdedbrelation* r= p_datasourceframe->designer()->get_relation(
                        p_datasourceframe->designer()->get_dsframe(masterds), p_datasourceframe);
  if (r)
  {
    r->edit();
    return;
  }

  hk_kderelationdialog* d = new hk_kderelationdialog(p_datasourceframe->designer()->get_dsframe(masterds),
         p_datasourceframe,p_datasourceframe->designer());

  (static_cast<QComboBox*>(d->grid->cellWidget(0,hk_kderelationdialogbase::MASTER_COL)))->
    setCurrentIndex(d->masterindex(QString::fromUtf8(l2u(mastertxt).c_str())));
  if(itemAt(event->pos()))
    (static_cast<QComboBox*>(d->grid->cellWidget(0,hk_kderelationdialogbase::SLAVE_COL)))->
    setCurrentIndex(d->slaveindex(itemAt(event->pos())->text()));
  d->slot_data_changed(d->grid->cellWidget(0,hk_kderelationdialogbase::MASTER_COL));

  if (d->exec()==QDialog::Accepted)
  {
    if (p_datasourceframe->designer()->presentation()->presentationtype()==hk_presentation::referentialintegrity)
    {
      p_datasourceframe->designer()->set_all_relations();
    }
    else
      p_datasourceframe->designer()->add_relation(masterds,p_datasourceframe->datasource());
        // Click on table box forces the link to be painted
  } 
  delete d;
}



void hk_kdefieldlist::contextMenuEvent(QContextMenuEvent* event)
{
  if ( dptr -> p_cont == NULL ) {
    dptr -> p_cont = new QMenu(this);
    dptr -> p_Edit = new QAction(i18n("Edit"),this);
    dptr -> p_cont -> addAction(dptr->p_Edit);
    dptr -> p_Del = new QAction(i18n("Delete"),this);
    dptr -> p_cont -> addAction(dptr->p_Del);    
  }
  QAction* p_res = dptr -> p_cont -> exec(QCursor::pos());

  if ( p_res == dptr->p_Edit ) {
      p_datasourceframe->edit();
  }else 
    if (p_res == dptr -> p_Del) 
      p_datasourceframe->designer()->delete_datasource(p_datasourceframe); 
  event->accept();
}


hk_kdedatasourceframe::hk_kdedatasourceframe(hk_kdedbdesigner* designer,QWidget* parent,hk_datasource* ds):
	QFrame(parent)
	,hk_dsdatavisible()
{
  p_designer = designer;
  set_datasource(ds);
  setFrameStyle( QFrame::WinPanel | QFrame::Raised );
  p_layout=new QGridLayout(this);
  p_layout->setMargin(1);
  p_layout->setSpacing(0);
  QSize s=QSize(150,150);
  setMinimumSize(s);
  setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding); 
  
  p_listbox=new hk_kdefieldlist(this);
  p_header=new kdedatasourcelabel(this);
  p_listbox->installEventFilter(this);
  p_header->installEventFilter(this);
  installEventFilter(this);
  p_layout->addWidget(p_header,0,0,1,2);
  p_layout->addWidget(p_listbox,1,0);
  hk_kdemovewidget* mv=new  hk_kdemovewidget(hk_kdemovewidget::horizontal,this);
  p_layout->addWidget(mv,2,0);
  mv=new  hk_kdemovewidget(hk_kdemovewidget::vertical,this);
  p_layout->addWidget(mv,0,1);
  mv=new  hk_kdemovewidget(hk_kdemovewidget::vertical,this);
  p_layout->addWidget(mv,1,1);
  mv=new  hk_kdemovewidget(hk_kdemovewidget::diagonal,this);
  p_layout->addWidget(mv,2,1);

  p_positionupdate=true;
  KIconLoader loader (LIB_MODULE_NAME);
  keyicon = QIcon(loader.iconPath("key", KIconLoader::User)).pixmap(8,8);
  set_fields();
  if (ds) {
    setGeometry(ds->x(),ds->y(),ds->width(),ds->height());
  }
  else 	setGeometry(20,20,150,150);
  move(QWidget::x(),QWidget::y());
  p_positionupdate=false;
  setAutoFillBackground(true); 
}



bool hk_kdedatasourceframe::eventFilter(QObject* object,QEvent* e)
{
 QKeyEvent* event=dynamic_cast<QKeyEvent*>(e);
 if (event  && event->key()==Qt::Key_Delete && event->type()==QEvent::KeyRelease && datasource())
   {
       if (show_yesnodialog(replace_all("%1",hk_translate("Remove datasource '%1'?"),
		p_designer->presentation()->unique_datasourcename(datasource()->presentationnumber()))
		,true))
       {
       		designer()->delete_datasource(this);
       return true;
       }
   }
return QFrame::eventFilter(object,e);
}



void hk_kdedatasourceframe::set_fields(void)
{
	  if (!datasource())
	   {
	    cerr <<"hk_kdedatasourceframe::set_fields, NO DATASOURCE SET !!!"<<endl;
	    return;
	   }
	  list<hk_string>* cols=datasource()->columnnames();
	  if (!cols) return;
	  if (p_designer->presentation()->presentationtype()==hk_presentation::qbe)
	    p_listbox->addItem("*");
	  list<hk_string>::iterator it=cols->begin();
	  while (it!=cols->end())
	  {
	  	hk_column* c=datasource()->column_by_name(*it);
	  	if (c &&c->is_primary())
	  	{
		  p_listbox->addItem(new QListWidgetItem(keyicon,QString::fromUtf8(l2u((*it)).c_str())));
	  	}
	  	else
		  p_listbox->addItem(QString::fromUtf8(l2u((*it)).c_str()));
		++it;
	  }
	  set_header();
}




void hk_kdedatasourceframe::set_header(void)
{
	 if (!datasource()) return;
	 QString headertext;
	 switch(p_designer->presentation()->presentationtype())
	 {
	  case hk_presentation::qbe 	:headertext=QString::fromUtf8(l2u(p_designer->presentation()->unique_shortdatasourcename(datasource()->presentationnumber())).c_str());
	  				break;
	  default : headertext=QString::fromUtf8(l2u(p_designer->presentation()->unique_datasourcename(datasource()->presentationnumber())).c_str());
	 }
	 p_header->setText(headertext);
	 int w=QFrame::width();
	 adjustSize();
	 if (w<QFrame::width())
	    setMinimumWidth(QFrame::width());

}




void hk_kdedatasourceframe::set_focus(hk_kdedatasourceframe* f)
{
   if (f==this)
   {
	QPalette pal = p_header -> palette();
	pal.setColor(QPalette::Window, palette().color(QPalette::Active,QPalette::Highlight));
	pal.setColor(QPalette::WindowText, palette().color(QPalette::Active,QPalette::HighlightedText));
	p_header -> setPalette(pal);
	setFocus();
   }
   else
   {
	QPalette pal = p_header -> palette();
	pal.setColor(QPalette::Window, palette().color(QPalette::Inactive,QPalette::Window));
	pal.setColor(QPalette::WindowText, palette().color(QPalette::Inactive,QPalette::WindowText));
	p_header -> setPalette(pal);
   }
}

void hk_kdedatasourceframe::clearFocus(void)
{
    p_header->clearFocus();
    QFrame::clearFocus();
}

void hk_kdedatasourceframe::mousePressEvent(QMouseEvent*)
{

}

void hk_kdedatasourceframe::moveEvent(QMoveEvent*)
{
 set_new_position();
}


void hk_kdedatasourceframe::resizeEvent(QResizeEvent*)
{
 set_new_position();
}

void hk_kdedatasourceframe::set_new_position(void)
{
hk_datasource* ds=datasource();
if (!ds||p_positionupdate) return;
ds->set_position(QWidget::x(),QWidget::y(),false);
ds->set_size(QWidget::width(),QWidget::height());
emit signal_moved();
}



hk_kdedbdesigner* hk_kdedatasourceframe::designer(void) const
{
  return p_designer;
}



void hk_kdedatasourceframe::edit(void)
{
    hk_kdeformdatasourcedialog* d = new hk_kdeformdatasourcedialog(designer()->presentation(),this,0,true);
    hk_kdeformdatasourcedialog::enum_displaytype dp=hk_kdeformdatasourcedialog::d_form;
    if (designer()->presentation()->presentationtype()!=hk_presentation::form)
    dp=(designer()->presentation()->presentationtype()==hk_presentation::report?
        hk_kdeformdatasourcedialog::d_report:
	hk_kdeformdatasourcedialog::d_query);
    d->set_displaytype(dp);
    d->set_edittype(hk_kdeformdatasourcedialog::e_modify);
    d->set_datasource(datasource());
    hk_datasource* depdatasource=NULL;
    if (datasource())depdatasource=datasource()->depending_on();
    d->alterbutton->setEnabled(true);
    d->Addbutton->setEnabled(false);
    d->deletebutton->setEnabled(true);
    d->set_allow_delete_datasource(false);
    d->exec();
    if (d->clicked_button()==hk_kdeformdatasourcedialog::cb_delete)
        {
	   designer()->delete_datasource(this);
	   delete datasource();
	   return;
	}
	else
	// datasource still exists, so set the perhaps changed relation
    if (depdatasource &&!d->datasource()->depending_on())
       { //old relation is not needed any more
	   designer()->delete_relation(designer()->get_relation(depdatasource,datasource()));
       }
    else
    if (depdatasource && depdatasource!=d->datasource()->depending_on())
    	{ //old relation exists, but linked to the wrong master datasource
           hk_kdedbrelation* relation=designer()->get_relation(depdatasource,datasource());
	   if (relation)
	   	{
			relation->set_datasources(designer()->get_dsframe(datasource()->depending_on()),this);
		}
	}
     else
     if (!depdatasource && d->datasource()->depending_on())
     	{//no relation exists, but a new one is set
		designer()->add_relation(d->datasource()->depending_on(),datasource());
	}

    delete d;


}




//   HK_KDEDBDESIGNER BEGINN


 hk_kdedbdesigner::hk_kdedbdesigner(QWidget* parent):QWidget(parent),hk_dbvisible()
{
  p_private=new hk_kdedbdesignerprivate(this);
  setGeometry(QWidget::x(),QWidget::y(),600,500);
}



hk_kdedbdesigner::~hk_kdedbdesigner()
{
delete p_private->p_layout;
delete p_private->p_scrollview;
delete p_private;
}

QScrollArea* hk_kdedbdesigner::scrollview(void) const
{
return p_private->p_scrollview;
}




void hk_kdedbdesigner::set_presentation(hk_presentation * p)
{
p_private->p_presentation =p;
p_private->p_datasources=NULL;
clear_datasources();
if (!p_private->p_presentation) return;
p_private->p_datasources=p_private->p_presentation->datasources();
set_datasources();
}




hk_presentation* hk_kdedbdesigner::presentation(void)
{
return p_private->p_presentation;
}





void hk_kdedbdesigner::clear_datasources()
{
 if (!p_private->p_datasources)return;
}





hk_kdedatasourceframe* hk_kdedbdesigner::get_dsframe(hk_datasource* ds)
{
  if (!ds) return NULL;
  list<hk_kdedatasourceframe*>::iterator it=p_private->p_windowlist.begin();
while (it!=p_private->p_windowlist.end())
  	{
	   if ((*it)->datasource()==ds)
	   	{
		  return (*it);
		}
	   ++it;
	}
return NULL;
}





hk_kdedbrelation* hk_kdedbdesigner::get_relation(hk_datasource* master,hk_datasource* slave)
{
  if (!master ||!slave) return NULL;
  list<hk_kdedbrelation*>::iterator it=p_private->p_relationlist.begin();
while (it!=p_private->p_relationlist.end())
  	{
	   if ((*it)->masterdatasource()->datasource()==master && (*it)->slavedatasource()->datasource()==slave)
	   	{
		  return (*it);
		}
	   ++it;
	}
return NULL;
}


hk_kdedbrelation* hk_kdedbdesigner::get_relation(hk_datasource* ds)
{
  if (!ds) return NULL; 
  list<hk_kdedbrelation*>::iterator it=p_private->p_relationlist.begin();
while (it!=p_private->p_relationlist.end())
  	{
	   if ((*it)->masterdatasource()->datasource()==ds|| (*it)->slavedatasource()->datasource()==ds)
	   	{
		  return (*it);
		}
	   ++it;
	}
return NULL;

}


referentialclass* hk_kdedbdesigner::get_referentialintegrity(hk_datasource* master,hk_datasource* slave)
{
  if (!master ||!slave) return NULL;
list <referentialclass>* rl= slave->referenceslist();
list <referentialclass>::iterator rit=rl->begin();
 while (rit!=rl->end())
 {
     if ((*rit).p_masterdatasource==master->name())
     {
       referentialclass* r=new referentialclass;
       *r=*rit;
       return r;
      }
   ++rit;
 }
return NULL;
}






hk_kdedbrelation* hk_kdedbdesigner::get_relation(hk_kdedatasourceframe* master, hk_kdedatasourceframe* slave)
{
 if (!master ||!slave) return NULL;
  return get_relation(master->datasource(),slave->datasource());

}

void hk_kdedbdesigner::new_focus(hk_kdedatasourceframe* f)
{
  list<hk_kdedatasourceframe*>::iterator it=p_private->p_windowlist.begin();
while (it!=p_private->p_windowlist.end())
  	{
	   (*it)->set_focus(f);
	   ++it;
	}

	if (f)
	  {
	    f->raise();
	    relation_clicked(NULL);
	  }

}






void hk_kdedbdesigner::set_datasources()
{
 if (!p_private->p_datasources)
 	{
	 return;
	}
  bool bf=p_private->p_presentation->block_has_changed();
 p_private->p_presentation->set_block_has_changed(true);
 list<hk_datasource*>::iterator it=p_private->p_datasources->begin();
 hk_kdedatasourceframe* firstframe=NULL;
 while (it!=p_private->p_datasources->end())
 {
    hk_kdedatasourceframe* f=add_dsframe((*it));
    if (!firstframe) firstframe=f;

  ++it;
  }
   set_all_relations();

   p_private->p_presentation->set_block_has_changed(bf);
   new_focus(firstframe);
}


void hk_kdedbdesigner::set_all_relations(void)
{
  list<hk_kdedbrelation*>::iterator relit=p_private->p_relationlist.begin();
  while (relit!=p_private->p_relationlist.end())
  {
  hk_kdedbrelation* i=(*relit);
  ++relit;
  delete i;
  }
  p_private->p_relationlist.clear();

  list<hk_kdedatasourceframe*>::iterator sit=p_private->p_windowlist.begin();
  while (sit!=p_private->p_windowlist.end())
  	{
	 hk_datasource* ds=(*sit)->datasource();
         if(p_private->p_presentation->presentationtype()==hk_presentation::referentialintegrity)
         {
          list <referentialclass>* rl= ds->referenceslist();

          list <referentialclass>::iterator rit=rl->begin();
          while (rit!=rl->end())
          {
                add_referentialintegrity((*rit),(*sit));
            ++rit;
          }
         }
         else
         {
	   if (ds && ds->depending_on())
	   {

                add_relation(get_dsframe(ds->depending_on()),(*sit));
	   }
	 }
	++sit;
	}

raise_datasources();

}


hk_datasource* hk_kdedbdesigner::already_added_table(const hk_string&t)
{
list<hk_datasource*>*l=p_private->p_presentation->datasources();
list<hk_datasource*>::iterator it=l->begin();
while (it!=l->end())
{
 hk_datasource* d=(*it);
 if (d->type()==hk_datasource::ds_table && d->name()==t) return d;
++it;
}

return NULL;
}








hk_kdedatasourceframe* hk_kdedbdesigner::add_dsframe(hk_datasource* ds)
{
 if (!ds) return NULL;

  hk_kdedatasourceframe* k=new hk_kdedatasourceframe(this,p_private->p_scrollview->widget(), ds);
  k->set_datasource(ds);
  p_private->p_windowlist.insert(p_private->p_windowlist.end(),k);
  k->show();
  return k;

}

hk_kdedbrelation* hk_kdedbdesigner::add_relation(hk_kdedatasourceframe* master, hk_kdedatasourceframe* slave)
{
  if (!master||!slave) return NULL;
  if (slave&&slave->datasource()&&slave->datasource()->depending_on())
    {
       hk_kdedbrelation* r=get_relation(slave->datasource()->depending_on(),slave->datasource());
       if (r) 
       {

        delete_relation(r);
       }

    }

  hk_kdedbrelation* relation=new hk_kdedbrelation(p_private->p_scrollview->widget(),
                                p_private->p_presentation && 
                                p_private->p_presentation->presentationtype()==hk_presentation::referentialintegrity);
  relation->set_datasources(master,slave);
  relation->setGeometry(10,50,100,70);
  p_private->p_relationlist.insert(p_private->p_relationlist.end(),relation);
  connect(relation,SIGNAL(signal_relation_clicked(hk_kdedbrelation*)),this,SLOT(relation_clicked(hk_kdedbrelation*)));
  relation->show();
  relation->raise();
  relation->lower();
  slave->set_header();
  emit signal_definition_has_changed();

 return relation;

}

hk_kdedbrelation* hk_kdedbdesigner::add_relation(hk_datasource* master, hk_datasource* slave)
{
  return add_relation(get_dsframe(master),get_dsframe(slave));
}



hk_kdedbrelation* hk_kdedbdesigner::add_referentialintegrity(referentialclass ref, hk_kdedatasourceframe* ds)
{
  hk_kdedatasourceframe* ms=get_dsframe(already_added_table(ref.p_masterdatasource));
  if (!ds ||!ms) return NULL;

  hk_kdedbrelation* relation=new hk_kdedbrelation(p_private->p_scrollview->widget(),
                                 p_private->p_presentation && 
                                 p_private->p_presentation->presentationtype()==hk_presentation::referentialintegrity);
  relation->set_referentialintegrity(ref,ms,ds);
  relation->setGeometry(10,50,100,70);
  p_private->p_relationlist.insert(p_private->p_relationlist.end(),relation);
  connect(relation,SIGNAL(signal_relation_clicked(hk_kdedbrelation*)),this,SLOT(relation_clicked(hk_kdedbrelation*)));
  relation->show();
  relation->raise();
  relation->lower();
  ds->set_header();
  emit signal_definition_has_changed();

 return relation;

}









void hk_kdedbdesigner::raise_datasources(void)
{
  list<hk_kdedatasourceframe*>::iterator sit=p_private->p_windowlist.begin();
  sit=p_private->p_windowlist.begin();
  while (sit!=p_private->p_windowlist.end())
  	{
	  (*sit)->raise();
	  ++sit;
	}

}



void hk_kdedbdesigner::delete_datasource(hk_kdedatasourceframe* f)
{
 if (!f) return;
  if (f->datasource())
  {
  	list<hk_datasource*>* l=f->datasource()->dependinglist();
  	list<hk_datasource*>::iterator it=l->begin();
	hk_kdedbrelation* rel=NULL;
   while((rel=get_relation(f->datasource())))
   {  
      if (rel) remove_relation(rel);
   }
  }
  else cerr <<"leere Datasource!!!"<<endl;
  p_private->p_windowlist.remove(f);
  f->deleteLater();
  delete f->datasource();
emit signal_definition_has_changed();
}



void hk_kdedbdesigner::delete_relation(hk_kdedbrelation* r)
{
  if (presentation()->presentationtype()==hk_presentation::referentialintegrity)
  {
     r->slavedatasource()->datasource()->drop_reference(r->referentialname());
  }
  remove_relation(r);
emit signal_definition_has_changed();
}

void hk_kdedbdesigner::remove_relation(hk_kdedbrelation* r)
{
  p_private->p_relationlist.remove(r);
  r->slavedatasource()->datasource()->set_depending_on_presentationdatasource(-1);
  r->slavedatasource()->datasource()->clear_depending_fields();
  r->slavedatasource()->set_header();
  r->deleteLater();

}


void hk_kdedbdesigner::relation_clicked(hk_kdedbrelation* cr)
{
list<hk_kdedbrelation*>::iterator it=p_private->p_relationlist.begin();
while (it!=p_private->p_relationlist.end())
  	{
	  (*it)->slot_relation_clicked(cr);
	  ++it;
	}


	if (cr) new_focus(NULL);
}





void hk_kdedbdesigner::add_datasource(void)
{
  assert(presentation()!=NULL);
  if (!presentation()) {
     show_warningmessage("No presentation set!");
     return;
   }
   hk_kdeaddtabledialog* addtabledialog=new hk_kdeaddtabledialog(
		this,
		presentation()->presentationtype()!=hk_presentation::qbe,this
		);
   addtabledialog->exec();
   if (addtabledialog->datasource_added())
         emit signal_definition_has_changed();
   delete addtabledialog;
}


void hk_kdedbdesigner::slot_field_doubleclicked(int t, const hk_string& c)
{
emit signal_field_doubleclicked(t,c);
}



// hk_kdeDBRELATION BEGIN
  const  int hlength=10;//horizontal line length
  const  int  arrowlength=5;
  const  int  arrowheight=5;
  
class hk_kdedbrelationPrivate
{
public:
  hk_kdedbrelationPrivate():p_context(NULL),p_Edit(NULL),p_Del(NULL)
  {
  }
  QMenu* p_context;
  QAction* p_Edit;
  QAction* p_Del;
};

hk_kdedbrelation::hk_kdedbrelation(QWidget* parent,bool is_integrity):QWidget(parent),hk_class(),
dptr(new hk_kdedbrelationPrivate())
{
    p_tox=p_toy=p_fromx=p_fromy=0;
    p_masterdatasource=p_slavedatasource=NULL;
    p_is_focused=false;
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    p_is_referentialintegrity=is_integrity;
    if (is_integrity) {
        QPalette p(palette());

        p.setColor(foregroundRole(),Qt::blue);
        setPalette(p);
    }
    p_deletecascade=false;
    p_updatecascade=false;
}

hk_kdedbrelation::~hk_kdedbrelation()
{
    delete dptr;
}

void hk_kdedbrelation::paint_relation(QPainter* paint)
{
   paint->save();
   hk_datasource* sd=(p_slavedatasource?p_slavedatasource->datasource():NULL);
   bool paint_arrow=(sd && sd->depending_on_is_left_join());

    
    QPen p=paint->pen();
   if (p_is_focused)
   {
    p.setWidth(2);
    paint->setPen(p);
   }
   paint->drawLine(p_fromx+hlength,p_fromy+1,p_tox-hlength,p_toy+arrowheight); //diagonal


   int pw=p.width();
   p.setWidth(pw*2+3);
   paint->setPen(p);
   paint->drawLine(p_fromx,p_fromy+1,p_fromx+hlength,p_fromy+1);//master horizontal
   p.setWidth(pw);
   paint->setPen(p);
   paint->drawLine(p_tox-hlength,p_toy+arrowheight,  p_tox ,p_toy+arrowheight);//slave horizontal
   if (p_is_focused)
   {
    QPen p=paint->pen();
    p.setWidth(0);
    paint->setPen(p);
   }
   if (paint_arrow)
   {
     QPolygon points;
     
     points.setPoints(3,p_tox-1,p_toy+arrowheight, p_tox-arrowlength-1,p_toy,p_tox-arrowlength-1,p_toy+2*arrowheight);
     paint->drawPolygon(points);
   }
   else
       paint->drawEllipse(p_tox-hlength/2,p_toy,hlength,hlength);
   
   paint->restore();
}



void hk_kdedbrelation::paintEvent(QPaintEvent*)
{
  QPainter paint(this);
  
  paint.setBrush(palette().color(foregroundRole()));
  paint_relation(&paint);
}

hk_string hk_kdedbrelation::referentialname()const
{
  return p_refname;
}

void hk_kdedbrelation::set_datasources(hk_kdedatasourceframe* master,hk_kdedatasourceframe* slave)
{
if (!master||!slave) return;


 p_masterdatasource=master;
 p_slavedatasource=slave;
 setToolTip(tooltipfields());
 connect(p_masterdatasource,SIGNAL(signal_moved()),this,SLOT(datasource_moved()));
 connect(p_slavedatasource,SIGNAL(signal_moved()),this,SLOT(datasource_moved()));
 datasource_moved();
}


void hk_kdedbrelation::set_referentialintegrity(referentialclass c,hk_kdedatasourceframe* master,hk_kdedatasourceframe* slave)
{
if (!master||!slave) return;


 p_masterdatasource=master;
 p_slavedatasource=slave;
 p_refname=c.p_name;
 p_deletecascade=c.p_deletecascade;
 p_updatecascade=c.p_updatecascade;
 this -> setToolTip(tooltipfields(&c));
 connect(p_masterdatasource,SIGNAL(signal_moved()),this,SLOT(datasource_moved()));
 connect(p_slavedatasource,SIGNAL(signal_moved()),this,SLOT(datasource_moved()));
 datasource_moved();
}


bool hk_kdedbrelation::updatecascade(void) const
{
  return p_updatecascade;
}

bool hk_kdedbrelation::deletecascade(void) const
{
  return p_deletecascade;
}

QString hk_kdedbrelation::tooltipfields(referentialclass* c)
{
   QString result;
   if (c)
   {
     list<dependingclass>::iterator it= c->p_fields.begin();
     while (it!= c->p_fields.end())
     {
       if (!result.isEmpty()) result+="\n";
       result+=	QString::fromUtf8(l2u((*it).masterfield).c_str())
       		+"\t = " + QString::fromUtf8(l2u((*it).dependingfield).c_str());
      ++it;
     }
       if (!result.isEmpty()) result+="\n________________\n";
       result+=i18n("On update:") +(c->p_updatecascade?i18n("Cascade"):i18n("Restrict"))+"\n";
       result+=i18n("On delete:") +(c->p_deletecascade?i18n("Cascade"):i18n("Restrict"));

    return result;
   }


   list<hk_string>* masterlist=p_slavedatasource->datasource()->depending_on_masterfields();
   list<hk_string>* slavelist=p_slavedatasource->datasource()->depending_on_thisfields();
   list<hk_string>::iterator masterit= masterlist->begin();
   list<hk_string>::iterator slaveit= slavelist->begin();
   while (masterit!=masterlist->end())
   {
       if (!result.isEmpty()) result+="\n";
       result+=	QString::fromUtf8(l2u((*masterit)).c_str())
       		+"\t = " + QString::fromUtf8(l2u((*slaveit)).c_str());
     ++masterit;
     ++slaveit;
   }
return result;
}




hk_kdedatasourceframe* hk_kdedbrelation::masterdatasource(void) const
{
  return p_masterdatasource;
}

hk_kdedatasourceframe* hk_kdedbrelation::slavedatasource(void) const
{
  return p_slavedatasource;
}



void hk_kdedbrelation::datasource_moved(void)
{
int x,y,w,h,mx,my,mh,sx,sy,sh;
mx=p_masterdatasource->QWidget::x()+p_masterdatasource->QWidget::width();
sx=p_slavedatasource->QWidget::x();
if (mx+hlength<sx)
{ //standard case;
   x=mx;
   w=sx-mx;
   p_fromx=0;
   p_tox=w;
}
else
{
   x=sx-hlength;
   w=mx-sx+2*hlength;
   p_fromx=w-hlength;
   p_tox=hlength;

}
if (w<hlength)w=hlength;

my=p_masterdatasource->QWidget::y();
mh=p_slavedatasource->QWidget::y()-p_masterdatasource->QWidget::y();
sy=p_slavedatasource->QWidget::y();
sh=-mh;
if (my<sy)
{//standard case
y=my;
h=mh;
p_fromy=0;
p_toy=h;
}
else
{
y=sy;
h=sh;
if (h>0)p_fromy=h;
else p_fromy=0;
p_toy=0;
}

if (h<3)h=3;
if (w<3)w=3;

setGeometry(x,y+10,w+10,h+20);

}


void hk_kdedbrelation::mouseMoveEvent(QMouseEvent* e)
{
      QWidget::mouseMoveEvent(e);
}




void hk_kdedbrelation::mousePressEvent(QMouseEvent* e)
{
 setFocus();
 QWidget::mousePressEvent(e);
}


void hk_kdedbrelation::keyPressEvent(QKeyEvent* event)
{
if (event->key()==Qt::Key_Delete)
	p_masterdatasource->designer()->delete_relation(this);

}

void hk_kdedbrelation::mouseDoubleClickEvent(QMouseEvent*)
{
 edit();
}

void hk_kdedbrelation::slot_relation_clicked(hk_kdedbrelation* cr)
{  
   if (cr==this )
        {if (!p_is_focused) setFocus();}
     else
       p_is_focused=false;
   set_focus();
}

void hk_kdedbrelation::set_focus(void)
{
 p_masterdatasource->designer()->presentation()->set_block_has_changed(true);
 p_masterdatasource->move(p_masterdatasource->QWidget::x()+1,p_masterdatasource->QWidget::y());
 p_masterdatasource->move(p_masterdatasource->QWidget::x()-1,p_masterdatasource->QWidget::y());
 p_masterdatasource->designer()->presentation()->set_block_has_changed(false);
}



void hk_kdedbrelation::setFocus(void)
{
QWidget::setFocus();
p_is_focused=true;
p_masterdatasource->designer()->relation_clicked(this);
}




void hk_kdedbrelation::contextMenuEvent(QContextMenuEvent* event)
{
  if (dptr -> p_context == NULL)
  {
    dptr -> p_context = new QMenu(this);
    dptr -> p_Edit = new QAction(i18n("Edit"),this);
    dptr -> p_context -> addAction(dptr -> p_Edit);
    dptr -> p_Del = new QAction(i18n("Delete"),this);
    dptr -> p_context -> addAction(dptr -> p_Del);  
  }
  QAction* p_res = dptr -> p_context -> exec(QCursor::pos());
  if (p_res == dptr -> p_Edit)
    edit();
  else 
    if (p_res == dptr ->p_Del)
      p_masterdatasource->designer()->delete_relation(this);
  event->accept();
}


void hk_kdedbrelation::edit(void)
{
  hk_kderelationdialog* d = new hk_kderelationdialog(p_masterdatasource, p_slavedatasource,
    p_masterdatasource->designer(), this);
  d->exec();
  setToolTip(tooltipfields());
  delete d;
}

class hk_kdedbdesignerwindowprivate
{
  public:
    KParts::ReadWritePart* p_part;
    KAction* p_closeaction;
};

hk_kdedbdesignerwindow::hk_kdedbdesignerwindow( QWidget* parent, const char* name, Qt::WFlags fl )
: KParts::MainWindow( parent, fl|Qt::Dialog),p_private(new hk_kdedbdesignerwindowprivate)
{
  if (name)
      setObjectName(QString::fromAscii(name));
  setWindowModality(Qt::ApplicationModal);
  setComponentName(LIB_MODULE_NAME, componentData().displayName());
  QIcon::setThemeName("oxygen");
  setXMLFile("hk_kdedbdesigner.rc");
  setGeometry(x(),y(),600,500);
  
  KService::Ptr service = KService::serviceByDesktopName("hk_kde5dbdesignerpart");
  p_private->p_part = service->createInstance<KParts::ReadWritePart>(this, this, QVariantList());
  p_private->p_closeaction=new KAction(i18n("&Close"),actionCollection()); 
  p_private->p_closeaction -> setIcon(QIcon::fromTheme("window-close"));
  actionCollection() -> addAction("closedesigner",p_private->p_closeaction);
  connect(p_private->p_closeaction,SIGNAL(triggered()),this,SLOT(close()));
  setCentralWidget(designer());
  createGUI(p_private->p_part);

  KSharedConfigPtr c=KGlobal::config();
  const QRect& rrect=QRect(0,0,500,300);
  KConfigGroup cg = c->group("DatabaseDesigner");
  QRect g;
  g=cg.readEntry("Geometry",rrect);
  setGeometry(g);
}

hk_kdedbdesignerwindow::~hk_kdedbdesignerwindow()
{
   delete p_private;
}


hk_kdedbdesigner* hk_kdedbdesignerwindow::designer(void) const
{
   return (hk_kdedbdesigner*)p_private->p_part->widget();
}


void hk_kdedbdesignerwindow::closeEvent(QCloseEvent* e)
{
  KSharedConfigPtr c=KGlobal::config();
  KConfigGroup cg = c->group("DatabaseDesigner");
  cg.writeEntry("Geometry",geometry());
  if (designer()->presentation()&&
    designer()->presentation()->presentationtype()==hk_presentation::referentialintegrity)
    {
    ((hk_referentialintegrity*)(designer()->presentation()))->save_referentialintegrity();
    }
  emit signal_closed();
  KParts::MainWindow::closeEvent(e);
}

const int mvfixed=3;

hk_kdemovewidget::hk_kdemovewidget(enum_orientation orientation,QWidget*parent,const char* name,Qt::WFlags fl)
: QWidget(parent,fl)
{
   setObjectName(QString::fromAscii(name));
   p_drag=false;
   p_startx=p_starty=p_offsetx=p_offsety=0;
   p_orientation=orientation;
   QSize s=QSize(mvfixed,mvfixed);
   setMinimumSize(s);
   switch (orientation)
   {
     case horizontal:
   		setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
   		setCursor(Qt::SizeVerCursor);
		break;

     case vertical:
   		setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
   		setCursor(Qt::SizeHorCursor);
		break;
     case diagonal:
   		setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
   		setCursor(Qt::SizeFDiagCursor);
		break;

   }
}

hk_kdemovewidget::~hk_kdemovewidget()
{

}


void hk_kdemovewidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button()==Qt::LeftButton)
	   {
		p_drag=true;
		p_startx=event->globalPos().x();
		p_starty=event->globalPos().y();
		p_offsetx=p_offsety=0;
	   }
}

void hk_kdemovewidget::mouseMoveEvent(QMouseEvent* event)
{
  const int minwidth=150;
  const int minheight=150;
  if (p_drag && event->button()==Qt::LeftButton)
  {
    int xdiff=event->globalPos().x()-p_startx;
    int ydiff=event->globalPos().y()-p_starty;
    if (p_orientation==horizontal) xdiff=0;
     else
     if (p_orientation==vertical) ydiff=0;
    if ((abs(xdiff)>2) || (abs(ydiff)>2))
    {
	QSize newsize=parentWidget()->size()+QSize(xdiff,ydiff);
	if (newsize.width()<minwidth)
	{
		p_offsetx+=newsize.width();
		newsize.setWidth(minwidth);
	}
	else if (p_offsetx<0)
	{
		p_offsetx+=newsize.width();
		if (p_offsetx>0)
		{
			newsize.setWidth(p_offsetx);
			p_offsetx=0;
		}
		else newsize.setWidth(0);
	}
	if (newsize.height()<minheight)
	{
		p_offsety+=newsize.height();
		newsize.setHeight(minheight);
	}
	else
	if (p_offsety<0)
	{
		p_offsety+=newsize.height();
		if (p_offsety>0)
		{
			newsize.setHeight(p_offsety);
			p_offsety=0;
		}
		else newsize.setHeight(0);
	}
	if (newsize.width()<minwidth) newsize.setWidth(minwidth);
	if (newsize.height()<minheight) newsize.setHeight(minheight);
	parentWidget()->resize(newsize);
	p_startx=event->globalPos().x();
	p_starty=event->globalPos().y();
  	}
  }
 }
