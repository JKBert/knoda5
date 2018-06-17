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

#include "hk_kdeqbe.h"
#include "hk_kdedbdesigner.h"
#include <hk_column.h>
#include <hk_database.h>
#include <hk_connection.h>
#include <qtablewidget.h>
#include <qitemdelegate.h>
#include <qsplitter.h>
#include <qstringlist.h>
#include <qlayout.h>
#include <qcombobox.h>
#include <QKeyEvent>
#include <QDropEvent>
#include <QMimeData>
#include <klocale.h>
#include <qapplication.h>

const int maxrows=15;
const int maxcolumns=64;

const int R_TABLE=0;
const int R_FIELDNAME=1;
const int R_ALIAS=2;
const int R_FUNCTION=3;
const int R_ORDER=4;
const int R_SHOW=5;
const int R_UPDATEVALUE=6;
const int R_CRITERIA=7;
const int R_OR=8;

class ComboRowDelegate;
class internalgrid;

class hk_kdeqbeprivate
{
public:

hk_kdeqbeprivate():p_designer(NULL)
{
	p_functionnames << i18n("Group") << i18n("Sum") << i18n("Count") << i18n("Min");
	p_functionnames << i18n("Max") << i18n("Average") << i18n("Condition");
	p_orderoptions << i18n("none") << i18n("ascending") << i18n("descending");
	p_showoptions << i18n("Yes") << i18n("No");
}
void addFieldsList(const QString& key, const QStringList& ls)
{
	mapFields[key]=ls;
}
const QStringList& itemList(int row, int col);
hk_kdedbdesigner* p_designer;
internalgrid* p_table;
QSplitter* p_splitter;
QVBoxLayout* p_layout;
QStringList p_tablenames;
QStringList p_functionnames;
QStringList p_orderoptions;
QStringList p_showoptions;
QStringList p_empty;
QMap<QString,QStringList> mapFields;
};

class internalgrid:public QTableWidget
{
public:
internalgrid(QWidget* parent,hk_kdeqbe* qbe, hk_kdeqbeprivate* datap, const char* name=0);

hk_kdeqbe* p_qbe;
ComboRowDelegate* p_tablerow;
ComboRowDelegate* p_fieldrow;
ComboRowDelegate* p_functionrow;
ComboRowDelegate* p_orderrow;
ComboRowDelegate* p_showrow;
void setValue(int row, int col, int intvalue, const QString& strvalue);
const QString textValue(int row, int col) const;
QComboBox* cellComboBox(int row, int col) const;
void clearCell(int row,int col);

protected:
bool delete_selected(void)
	{
		QList<QTableWidgetItem*> selList = selectedItems();
		
		for(int j=0;j < selList.size();j++) 
			clearCell(selList[j]->row(),selList[j]->column());
 	 p_qbe->slot_has_changed(); 
	 return true;
	}


bool   eventFilter( QObject *o, QEvent *e )
	{
	  if (e->type()==QEvent::KeyPress) {
            QKeyEvent *ke = (QKeyEvent*)e;

            if (ke->key() == Qt::Key_Delete&& o->isWidgetType()) {
	       QWidget* w=(QWidget*)o;
	       while (w) {
	         if ( w==this) {
	           if (delete_selected()) return true;
		 }
		 w=w->parentWidget();
	       }
	     }
	  }
	 return QTableWidget::eventFilter(o,e);
       };

void dragEnterEvent(QDragEnterEvent* event);
void dropEvent(QDropEvent* event);
}; //  internalgrid END



class ComboRowDelegate:public QItemDelegate
{
  Q_OBJECT
public:
  ComboRowDelegate(internalgrid *parent,hk_kdeqbeprivate* datap, int row);
 
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
  int rowid;
  hk_kdeqbeprivate* dataprovider;
};


const QStringList& hk_kdeqbeprivate::itemList(int row, int col)
{
	switch(row){
		case R_FIELDNAME:
			return mapFields[p_table->textValue(R_TABLE,col)];
		case R_TABLE:
			return p_tablenames;
		case R_FUNCTION:
			return p_functionnames;
		case R_ORDER:
			return p_orderoptions;
		case R_SHOW:
			return p_showoptions;
	}
	return p_empty;
}

internalgrid::internalgrid(QWidget* parent,hk_kdeqbe* qbe, hk_kdeqbeprivate* datap, const char* name):
QTableWidget(parent),p_qbe(qbe),p_tablerow(new ComboRowDelegate(this,datap,R_TABLE)),
p_fieldrow(new ComboRowDelegate(this,datap,R_FIELDNAME)),p_functionrow(new ComboRowDelegate(this,datap,R_FUNCTION)),
p_orderrow(new ComboRowDelegate(this,datap,R_ORDER)),p_showrow(new ComboRowDelegate(this,datap,R_SHOW))
{
     if (name != NULL) 
	     setObjectName(name);
     setItemDelegateForRow(R_TABLE,p_tablerow);
     setItemDelegateForRow(R_FIELDNAME,p_fieldrow);
     setItemDelegateForRow(R_FUNCTION,p_functionrow);
     setItemDelegateForRow(R_ORDER,p_orderrow);
     setItemDelegateForRow(R_SHOW,p_showrow);
     setAcceptDrops(true);
     setDragDropMode(QAbstractItemView::InternalMove);
     installEventFilter(this);
}

inline void internalgrid::setValue(int row, int col, int intvalue, const QString& strvalue)
{
	item(row,col)->setData(Qt::EditRole, QVariant(intvalue));
	item(row,col)->setData(Qt::UserRole, QVariant(strvalue));
}

inline const QString internalgrid::textValue(int row, int col) const
{ 
	return item(row,col)->data(Qt::UserRole).toString();
}

void internalgrid::clearCell(int row,int col)
{
	switch(row){
		case R_TABLE:
		case R_FIELDNAME:
		case R_ORDER:
		case R_FUNCTION:
		case R_SHOW:{
			setValue(row,col,0,cellComboBox(row,col)->itemText(0));
			cellComboBox(row,col)->setCurrentIndex(0);
			break;
		}
		default:
			item(row,col)->setText("");
	}
}

inline QComboBox* internalgrid::cellComboBox(int row, int col) const
{
	return dynamic_cast<QComboBox*>(cellWidget(row,col));
}

void internalgrid::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat("application/x-hk_kdedbdesigner"))
		event->acceptProposedAction();
}

void internalgrid::dropEvent(QDropEvent* event)
{
  if (!event->mimeData()->hasFormat("application/x-hk_kdedbdesigner"))
      return;
  
   hk_string eventtxt=u2l(event->mimeData()->data("application/x-hk_kdedbdesigner").data());

   xmlDocPtr doc=xmlParseMemory(eventtxt.c_str(),eventtxt.size());
   xmlNodePtr node=xmlDocGetRootElement(doc);
      long vupn;
   hk_string columnname;
   if (!(     hk_class::get_tagvalue(node,"VUPN",vupn)
         && hk_class::get_tagvalue(node,"VALUE",columnname)))
	 {
            hk_class::show_warningmessage("Error in drag&drop protocol");
            cerr <<eventtxt<<endl;
	    return;
	 }

    int dcol=columnAt(event->pos().x());
    p_qbe->add_column(dcol,vupn,columnname);
    event->accept();
}


ComboRowDelegate::ComboRowDelegate(internalgrid *parent,hk_kdeqbeprivate* datap,int row):QItemDelegate(parent),
rowid(row),dataprovider(datap)
{
}

QWidget* ComboRowDelegate::createEditor(QWidget *par, const QStyleOptionViewItem &/*unused*/, const QModelIndex &index) const
{
  QComboBox* editor = new QComboBox(par);
  QStringList items =  dataprovider->itemList(rowid,index.column());
  
  editor->addItems(items);
  // to get Delete key press event to event filter
  editor->installEventFilter(parent());
  return editor;
}

void ComboRowDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  int value = index.model()->data(index, Qt::EditRole).toInt();
  if (value > -1)
    comboBox->setCurrentIndex(value);
  else
    comboBox->setEditText(index.model()->data(index, Qt::UserRole).toString());
}
 
void ComboRowDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  bool edited = (comboBox->findText(comboBox->currentText()) == -1);
  QString strval(comboBox->currentText());

  if (edited) {
	  comboBox->addItem(strval);
	  comboBox->setCurrentIndex(comboBox->findText(strval));
  }
  model->setData(index, comboBox->currentIndex(), Qt::EditRole);
  model->setData(index, QVariant( strval ), Qt::UserRole);  
}
 
void ComboRowDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}
 
hk_kdeqbe::hk_kdeqbe(QWidget* w,const char* n,Qt::WFlags f) :KParts::MainWindow(w, f),hk_qbe()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::hk_kdeqbe");
#endif
  if (n)
    setObjectName(QString::fromAscii(n));
  p_private= new hk_kdeqbeprivate;
  p_private->p_splitter=new QSplitter(this);
  setCentralWidget(p_private->p_splitter);
  p_private->p_splitter->setOrientation(Qt::Vertical);
  p_private->p_designer=new hk_kdedbdesigner(p_private->p_splitter);
  p_private->p_designer->set_presentation(this);
  p_private->p_table=new internalgrid(p_private->p_splitter,this,p_private);
  p_private->p_table->setRowCount(maxrows);
  p_private->p_table->setColumnCount(maxcolumns);  
  p_private->p_table->setVerticalHeaderItem(R_TABLE,new QTableWidgetItem(i18n("Table:")));
  p_private->p_table->setVerticalHeaderItem(R_FIELDNAME, new QTableWidgetItem(i18n("Fieldname:")));
  p_private->p_table->setVerticalHeaderItem(R_ALIAS, new QTableWidgetItem(i18n("Alias:")));
  p_private->p_table->setVerticalHeaderItem(R_FUNCTION, new QTableWidgetItem(i18n("Function:")));
  p_private->p_table->setVerticalHeaderItem(R_ORDER, new QTableWidgetItem(i18n("Order:")));
  p_private->p_table->setVerticalHeaderItem(R_SHOW, new QTableWidgetItem(i18n("Show:")));
  p_private->p_table->setVerticalHeaderItem(R_UPDATEVALUE, new QTableWidgetItem(i18n("Updatevalue:")));
  p_private->p_table->setVerticalHeaderItem(R_CRITERIA, new QTableWidgetItem(i18n("Criteria:")));
  p_private->p_table->setVerticalHeaderItem(R_OR, new QTableWidgetItem(i18n("Or:")));
  for (int k=R_TABLE;k<maxrows;++k)
     p_private->p_table->setRowHeight(k,(int)(p_private->p_table->rowHeight(k)*1.5+0.5));
   
  for (int k=R_OR+1;k<maxrows;++k)
	p_private->p_table->setVerticalHeaderItem(k,new QTableWidgetItem(""));

  for (int k=0;k<p_private->p_table->columnCount();++k)
     init_column(k);
  
  p_private->p_table->resizeRowsToContents();  
  widget_specific_querytype_has_changed(hk_qbe::qt_select,hk_qbe::qt_select);
  connect(p_private->p_table, SIGNAL(cellChanged(int,int)),this,SLOT(value_changed(int,int))); 
  connect(p_private->p_designer,SIGNAL(signal_definition_has_changed()),this, SLOT (datasourcedefiniton_changed()));
  connect(p_private->p_designer,SIGNAL(signal_field_doubleclicked(int,const hk_string&)),
  this, SLOT (add_column(int , const hk_string&)));
}



hk_kdeqbe::~hk_kdeqbe()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::~hk_kdeqbe");
#endif
delete p_private;
}

void hk_kdeqbe::init_column(int col)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::init_column");
#endif
  QStringList sl;
  QTableWidgetItem* item;
  p_private->p_table->setHorizontalHeaderItem(col,new QTableWidgetItem(""));
  
  item = new QTableWidgetItem();
  p_private->p_table->setItem(R_TABLE,col,item);
  p_private->p_table->setValue(R_TABLE,col,-1,"");
  p_private->p_table->openPersistentEditor(item);

  item = new QTableWidgetItem();
  p_private->p_table->setItem(R_FIELDNAME,col,item);
  p_private->p_table->setValue(R_FIELDNAME,col,-1,"");
  p_private->p_table->openPersistentEditor(item);  
  if (p_private->p_table->cellComboBox(R_FIELDNAME,col)!= NULL)
    p_private->p_table->cellComboBox(R_FIELDNAME,col)->setEditable(true);
  p_private->p_table->setItem(R_ALIAS,col,new QTableWidgetItem());

  item = new QTableWidgetItem();
  p_private->p_table->setItem(R_FUNCTION,col,item);
  p_private->p_table->setValue(R_FUNCTION,col,0,p_private->p_functionnames[0]);
  p_private->p_table->openPersistentEditor(item);   

  item = new QTableWidgetItem();
  p_private->p_table->setItem(R_ORDER,col,item);
  p_private->p_table->setValue(R_ORDER,col,0,p_private->p_orderoptions[0]);
  p_private->p_table->openPersistentEditor(item);   

  item = new QTableWidgetItem();
  p_private->p_table->setItem(R_SHOW,col,item);
  p_private->p_table->setValue(R_SHOW,col,0,p_private->p_showoptions[0]);
  p_private->p_table->openPersistentEditor(item);  
  p_private->p_table->setItem(R_UPDATEVALUE,col,new QTableWidgetItem());
}



void hk_kdeqbe::set_datasource(hk_datasource* ds)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_datasource");
#endif
     hk_qbe::set_datasource(ds);
     hk_database* db=NULL;
     if (ds) db=ds->database();
     p_private->p_designer->set_database(db);
     p_private->p_designer->set_presentation(this);
}

void hk_kdeqbe::set_database(hk_database* db)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_datasource");
#endif
     hk_qbe::set_database(db);
     p_private->p_designer->set_presentation(this);
     widget_specific_querytype_has_changed(querytype(),querytype());
}




void hk_kdeqbe::add_datasource(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::add_datasource");
#endif
   p_private->p_designer->add_datasource();
}




void hk_kdeqbe::set_has_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_has_changed");
#endif
  hk_qbe::set_has_changed();
  if (! block_has_changed()) 
   {
    emit signal_qbe_has_changed();
   }
}




void hk_kdeqbe::slot_has_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::slot_has_changed");
#endif
  set_has_changed();

}



void hk_kdeqbe::datasourcedefiniton_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::datasourcedefinition_changed");
#endif
  set_tablenames();
  set_has_changed();
}



void hk_kdeqbe::set_tablenamelist(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_tablenamelist");
#endif
  p_private->p_tablenames.clear();
  p_private->p_tablenames.append("");
  list<hk_datasource*>* dslist=datasources();
  list<hk_datasource*>::iterator it=dslist->begin();
  while (it!=dslist->end())
  {
    p_private->p_tablenames.append(QString::fromUtf8(l2u(
    unique_shortdatasourcename((*it)->presentationnumber())).c_str()));
    ++it;
  }
}

void hk_kdeqbe::set_tablenames(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_tablenames");
#endif
  set_tablenamelist();
  for (int k=0;k<p_private->p_table->columnCount();++k)
  {
    QString current = p_private->p_table->item(R_TABLE,k)->data(Qt::UserRole).toString();

    init_tablenames(k);
    int i=p_private->p_tablenames.indexOf(current);
    if (i>-1) {
	    p_private->p_table->setValue(R_TABLE,k,i,current);
	    p_private->p_table->cellComboBox(R_TABLE,k)->setCurrentIndex(i);
    }
  }
}

void hk_kdeqbe::init_tablenames(int col) 
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::init_tablenames");
#endif
  QComboBox* combo = p_private->p_table->cellComboBox(R_TABLE,col);

  if (combo != NULL) {
	 combo->clear();
	 combo->addItems(p_private->itemList(R_TABLE,col));
  }
}


void hk_kdeqbe::value_changed(int row, int col)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::value_changed");
#endif
   set_has_changed();
   if (row==R_TABLE/*table*/)
      {
         set_columnnames(col);
      }
    else
      {
        if (row>=R_CRITERIA)
        {
  	  hk_connection* c=NULL;
          if (database())c=database()->connection();
          hk_string result=u2l(p_private->p_table->item(row,col)->text().toUtf8().data());
          if ((result.size()>0) && !hk_dsdatavisible::parsed_value(result,c)) return ;
          result=replace_all("%VALEND%",replace_all("%COLNAME%",result,""),"");
          blockSignals(true);
	  p_private->p_table->item(row,col)->setText(QString::fromUtf8(l2u(result).c_str()));
          blockSignals(false);
          if (! block_has_changed()) emit signal_qbe_has_changed();
        
        }
      }
}



void hk_kdeqbe::set_columnnames(int col)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_columnnames("+longint2string(col)+")");
#endif
   set_has_changed();
   QString str(p_private->p_table->textValue(R_TABLE,col));
   hk_string tbl=u2l(p_private->p_table->textValue(R_TABLE,col).toUtf8().data());
   hk_datasource* ds= get_datasource_by_shortname(tbl);
   QComboBox* combo = p_private->p_table->cellComboBox(R_FIELDNAME,col);

   if(combo != NULL) {
     QStringList strlist;
     
     combo -> setEditable(tbl=="");
     strlist.append("");
     if (ds) {
       list<hk_column*>* collist=ds->columns();
       if (collist) {
         strlist.append("*");
         list<hk_column*>::iterator it=collist->begin();
         while (it!=collist->end()) {
           strlist.append(QString::fromUtf8(l2u((*it)->name()).c_str()));
           ++it;
         }
       }
     }
     strlist.sort();
     p_private->addFieldsList(str,strlist); 
     combo->clear();
     combo->addItems(p_private->itemList(R_FIELDNAME,col));
   }
}


void hk_kdeqbe::widget_specific_querytype_has_changed(enum_querytype ,enum_querytype nt)
  {
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::widget_specific_querytype_has_changed");
#endif

  if (!database())
     return;
    
    bool s_alias=database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_ALIAS);
    bool s_where=database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_WHERE);
    bool s_groupby=database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_GROUP_BY);
    bool s_orderby=database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_ORDER_BY);
       switch (nt)
      {
        case qt_select:
			p_private->p_table->showRow(R_ALIAS);//Alias
			p_private->p_table->hideRow(R_FUNCTION);//Function
			p_private->p_table->showRow(R_ORDER);//Order
			p_private->p_table->showRow(R_SHOW);//show
			p_private->p_table->hideRow(R_UPDATEVALUE);//UpdateValue
			break;
        case qt_groupselect:
			p_private->p_table->showRow(R_ALIAS);//Alias
			p_private->p_table->showRow(R_FUNCTION);//Function
			p_private->p_table->showRow(R_ORDER);//Order
			p_private->p_table->hideRow(R_SHOW);//show
			p_private->p_table->hideRow(R_UPDATEVALUE);//UpdateValue
			break;
	case qt_update:
			p_private->p_table->hideRow(R_ALIAS);//Alias
			p_private->p_table->hideRow(R_FUNCTION);//Function
			p_private->p_table->hideRow(R_ORDER);//Order
			p_private->p_table->hideRow(R_SHOW);//show
			p_private->p_table->showRow(R_UPDATEVALUE);//UpdateValue
			break;
	case qt_delete:
			p_private->p_table->hideRow(R_ALIAS);//Alias
			p_private->p_table->hideRow(R_FUNCTION);//Function
			p_private->p_table->hideRow(R_ORDER);//Order
			p_private->p_table->hideRow(R_SHOW);//show
			p_private->p_table->hideRow(R_UPDATEVALUE);//UpdateValue
      }

   if (!s_alias)   p_private->p_table->hideRow(R_ALIAS);
   if (!s_groupby)   p_private->p_table->hideRow(R_FUNCTION);
   if (!s_orderby)   p_private->p_table->hideRow(R_ORDER);
   if (!s_where)
     {
         for(int i=R_CRITERIA;i<maxrows;++i)
		p_private->p_table->hideRow(i);//WHERE Fields

     }
  emit signal_qbetype_has_changed();
  }



void hk_kdeqbe::set_columnvalues(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::set_columnvalues");
#endif
  set_block_has_changed(true);
  clear_definition(false);
  for (int i=0;i<p_private->p_table->columnCount();++i)	  
  {
    if ( p_private->p_table->cellComboBox(R_FIELDNAME,i)->currentIndex()>0 // field
          || !(p_private->p_table->cellComboBox(R_FIELDNAME,i)->currentText().isEmpty())
       )
    {

        hk_qbedataclass cl;
	hk_datasource* ds=get_datasource_by_shortname(u2l(p_private->p_table->textValue(R_TABLE,i).toUtf8().data()));
	
	if (ds)
	  cl.table=ds->presentationnumber();
	else 
	  cl.table=-1;
	cl.field=u2l(p_private->p_table->textValue(R_FIELDNAME,i).toUtf8().data());

	if (p_private->p_table->item(R_ALIAS,i) && !(p_private->p_table->item(R_ALIAS,i)->text().isEmpty())) 
		cl.alias=u2l(p_private->p_table->item(R_ALIAS,i)->text().toUtf8().data());

	switch(p_private->p_table->cellComboBox(R_FUNCTION,i)->currentIndex())
	{
	  case 0: cl.functiontype=ft_group;break;
	  case 1: cl.functiontype=ft_sum;break;
	  case 2: cl.functiontype=ft_count;break;
	  case 3: cl.functiontype=ft_min;break;
	  case 4: cl.functiontype=ft_max;break;
	  case 5: cl.functiontype=ft_condition;break;
	}

	switch(p_private->p_table->cellComboBox(R_ORDER,i)->currentIndex())
	{
	  case 0: cl.order=none;break;
	  case 1: cl.order=ascending;break;
	  case 2: cl.order=descending;break;
	}

	cl.show=(p_private->p_table->cellComboBox(R_SHOW,i)->currentIndex()==0);

	if (p_private->p_table->item(R_UPDATEVALUE,i) && !(p_private->p_table->item(R_UPDATEVALUE,i)->text().isEmpty())) 
		cl.updatevalue=u2l(p_private->p_table->item(R_UPDATEVALUE,i)->text().toUtf8().data());
	
	int endconditions=R_CRITERIA;
	for (int c=R_CRITERIA;c<maxrows;++c)
	  {
	    hk_string condi = p_private->p_table->item(c,i)? trim(u2l(p_private->p_table->item(c,i)->text().toUtf8().data())):
	    trim("");
	    if (condi.size()>0) endconditions=c;
	  }
        for(int c=R_CRITERIA;c<=endconditions;++c) {
		  hk_string condi= trim(u2l(p_private->p_table->item(c,i) != NULL ? 
		  p_private->p_table->item(c,i)->text().toUtf8().data():""));
		cl.conditions.insert(cl.conditions.end(),condi); 
	}
        add_definition(&cl); 
     }
  }
  set_block_has_changed(false); 
}

void hk_kdeqbe::loaddata(xmlNodePtr definition)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::loaddata");
#endif
   hk_qbe::loaddata(definition);
   list<hk_qbe::hk_qbedataclass>::const_iterator it=definitionlist()->begin();
   int counter=0;
   set_tablenames();
   p_private->mapFields.clear();
   while (it!=definitionlist()->end())
   {
	int i=p_private->p_tablenames.indexOf(QString::fromUtf8(l2u(unique_shortdatasourcename((*it).table)).c_str()));
        if (i>-1)
	   {
	     p_private->p_table->setValue(R_TABLE,counter,i,p_private->p_tablenames[i]); //set tablename
	     set_columnnames(counter);
	   }

	QComboBox* item = p_private->p_table->cellComboBox(R_FIELDNAME,counter);

	i=-1;
	i = item->findText(QString::fromUtf8(l2u((*it).field).c_str()));
        if (i>-1)
	      p_private->p_table->setValue(R_FIELDNAME,counter,i,item->itemText(i));
	   
	 if ((*it).table==-1)
	  {
	    QStringList strlist;
            strlist.append("");
	    strlist.append(QString::fromUtf8(l2u((*it).field).c_str()));
            item->setEditable(true);
	    item->clear();
	    item->addItems(strlist);
	    p_private->p_table->setValue(R_FIELDNAME,counter,1,item->itemText(1));
	  }
	//set alias
	 p_private->p_table->item(R_ALIAS,counter)->setText(QString::fromUtf8(l2u((*it).alias).c_str()));
	//set functiontype
	 i=0;
	switch ((*it).functiontype)
	{
	case ft_group:i=0;break;
	case ft_sum:i=1;break;
	case ft_count:i=2;break;
	case ft_min:i=3;break;
	case ft_max:i=4;break;
	case ft_avg:i=5;break;
	case ft_condition:i=6;break;
        }
	p_private->p_table->setValue(R_FUNCTION,counter,i,p_private->p_functionnames[i]);
	

	// set order
	i=0;
	switch ((*it).order)
	{
	case none: i=0;break;
	case ascending: i=1;break;
	case descending: i=2;break;
	}
	p_private->p_table->setValue(R_ORDER,counter,i,p_private->p_orderoptions[i]);

	//set show
	if ((*it).show) i=0; else i=1;
	p_private->p_table->setValue(R_SHOW,counter,i,p_private->p_showoptions[i]);
	//set updatevalue
	p_private->p_table->item(R_UPDATEVALUE,counter)->setText(QString::fromUtf8(l2u((*it).updatevalue).c_str()));
        vector<hk_string>::iterator condit;
	vector<hk_string> l=(*it).conditions;
	condit=l.begin();
	i=R_CRITERIA;
	while( condit!=l.end())
	{
		if (p_private->p_table->item(i,counter) != NULL)
			p_private->p_table->item(i,counter)->setText(QString::fromUtf8(l2u((*condit)).c_str()));
		else
			p_private->p_table->setItem(i,counter,new QTableWidgetItem(QString::fromUtf8(l2u((*condit)).c_str())));
	 ++condit;++i;
	}

    ++it;++counter;
   }
  emit (signal_distinct_has_changed());
}

void hk_kdeqbe::add_column(int table, const hk_string& columnname)
{
  int pos=0;
   while(!p_private->p_table->textValue(R_TABLE,pos).isEmpty() && pos<p_private->p_table->columnCount())
     ++pos;

  add_column(pos,table,columnname); 
}

void hk_kdeqbe::add_column(int pos, int table, const hk_string& columnname)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::add_column");
#endif
  p_private->p_table->insertColumn(pos);
  init_column(pos);
  init_tablenames(pos);
  int i=p_private->p_tablenames.indexOf(QString::fromUtf8(l2u(unique_shortdatasourcename(table)).c_str()));
  if (i>-1)
   {
      p_private->p_table->setValue(R_TABLE,pos,i,p_private->p_tablenames[i]);  // set tablename
      set_columnnames(pos);
   }

  i = p_private->p_table->cellComboBox(R_FIELDNAME,pos)->findText(QString::fromUtf8(l2u(columnname).c_str()));
  if (i>-1)
	p_private->p_table->setValue(R_FIELDNAME,pos,i,QString::fromUtf8(l2u(columnname).c_str()));  
  
  p_private->p_table->setCurrentCell(p_private->p_table->currentRow(),pos); 
}



void hk_kdeqbe::distinct_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdeqbe::distinct_changed");
#endif
  set_distinct(!distinct());
}

#include "moc_hk_kdeqbe.cpp"
#include "hk_kdeqbe.moc"
