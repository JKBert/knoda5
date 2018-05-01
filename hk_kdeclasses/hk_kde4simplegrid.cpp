// ****************************************************************************
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
// This file is part of the hk_kde4classes library.
//
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
//$Revision: 1.140 $

#include "hk_kde4simplegrid.h"
#include "hk_kde4simplegrid_p.h"
#include "hk_kde4simplegrid.moc"
#include <hk_database.h>
#include <hk_datasource.h>
#include <hk_connection.h>
#include "hk_kdegrid.h"

#include <list>
#include <qdrawutil.h>
#include <qcolor.h>
#include <qfontmetrics.h>
#include <qfontinfo.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qapplication.h>
#include <kmenu.h>
#include <QKeyEvent>
#include <qtooltip.h>
#include <qlistview.h>
#include <qstyle.h>
#include <qdatetime.h>
#include <qpainter.h>
#include <qprinter.h>
#include <qprintdialog.h>
#include <qheaderview.h>

#include <kglobal.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <klocale.h>
#include <qprinter.h>
#include <kfiledialog.h>
#include <kapplication.h>
#include <kcolorscheme.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include <QDrag>

class sectionclass
{
public:
    int start;       //starting row or column
    int end;         //final row or column
    int offset;      // needed offset
};
        
class hk_kde4simplegridprivate
{
public:
    hk_kde4simplegridprivate(hk_kde4simplegrid *parent): p_grid(parent), p_resizeverticalheader(false),
        autoresizeverticalheader(false), newsize(20), dragStartPos(0,0),dragInitPhase(false),hasFocus(false),
        pagenumber(1)
    {
    }
    hk_kde4simplegrid *p_grid;
    bool p_resizeverticalheader;
    bool autoresizeverticalheader;
    int newsize;
    QPoint dragStartPos;
    bool dragInitPhase;
    bool hasFocus;
    int pagenumber;
    bool inline leavingRow(bool next) {
        return ((!next && p_grid->horizontalHeader()->visualIndex(p_grid->currentIndex().column()) == 0)
                || (next && p_grid->horizontalHeader()->visualIndex(p_grid->currentIndex().column()) == p_grid->model()->columnCount() - 1));
    }
    void print_singlepage(QPainter& painter, const QStyleOptionViewItem& opt, int width, int height, sectionclass& vertical,
                          sectionclass& horizontal, int topborder, int bottomborder, int leftborder, int rightborder);
    int  print_header(QPainter& painter,int width,sectionclass& horizontal,int topborder,int leftborder,int rightborder);
    void print_frame(QPainter& painter,int width,int height,int topborder,int bottomborder,int leftborder,int rightborder);
    void inline print_cell(QPainter* p,int row, int col, const QStyleOptionViewItemV4& opt); 

};

void hk_kde4simplegridprivate::print_cell(QPainter* p,int row, int col, const QStyleOptionViewItemV4& opt)
{  
    // not using QStyleOptionViewItemV2::Alternate, it introduces not nice artefacts
    if (p_grid->alternatingRowColors() && (row & 1) )
        p->fillRect(opt. rect, opt.palette.alternateBase());
    p_grid->itemDelegate(p_grid->model()->index(row,col))->paint(p, opt, p_grid->model()->index(row,col));
}

void hk_kde4simplegridprivate::print_singlepage(QPainter& painter, const QStyleOptionViewItem& opt, int width, int height, sectionclass& vertical,
                                                sectionclass& horizontal, int topborder,int bottomborder,int leftborder,int rightborder)
{
    int vheaderoffset=print_header(painter,width,horizontal,topborder,leftborder,rightborder);
    int xpage;
    int pagewidth=width-leftborder-rightborder;
    QStyleOptionViewItemV4 opt4 = opt;
    
    for (int row=vertical.start;row<=vertical.end;++row)
    {
        int rowheight=0;
        int colwidth=0;
        
        for (int col=horizontal.start;col<=horizontal.end;++col)
        {
            int colpos=p_grid->columnViewportPosition(col);
            colwidth=p_grid->columnWidth(col);

            int rowpos=p_grid->rowViewportPosition(row);
            rowheight=p_grid->rowHeight(row);

            rowpos-=vertical.offset;
            colpos-=horizontal.offset;
            xpage = (colpos+colwidth>pagewidth?pagewidth-colpos:colwidth);
            
            painter.translate(colpos+leftborder,rowpos+topborder+vheaderoffset);    
            opt4.rect = QRect(0, 1, xpage - 1, rowheight - 1);
            print_cell(&painter, row, col, opt4);
            painter.drawLine( xpage - 1, 1, xpage - 1, rowheight); // vertical line
            painter.drawLine( 0, rowheight, xpage - 1, rowheight); // horizontal line
            painter.translate( -colpos-leftborder, -rowpos -topborder -vheaderoffset);
        }
    }
    print_frame(painter,width,height,topborder,bottomborder,leftborder,rightborder);

}

void hk_kde4simplegridprivate::print_frame(QPainter& painter,int width,int height,int topborder,
                                           int bottomborder,int leftborder,int rightborder)
{
    QPen pen( painter.pen() );
    QString pgnr=i18n("Page")+" -"+QString::number(pagenumber)+"-";
    hk_datetime dt;
    
    dt.set_now();
    painter.drawRect( leftborder, topborder, width-leftborder-rightborder ,height-topborder-bottomborder);
    painter.setPen( pen );
    painter.drawText(leftborder,height-bottomborder/2,width-leftborder-rightborder,bottomborder,Qt::AlignRight,pgnr);   
    painter.drawText(leftborder,height-bottomborder/2,width-leftborder-rightborder,bottomborder,Qt::AlignLeft,
                     QString::fromUtf8(l2u(dt.datetime_asstring()).c_str()));
    QString tablename=(p_grid->kdegrid()->datasource()->type()==hk_datasource::ds_query?i18n("Query: "):i18n("Table: "))+
                       QString::fromUtf8(l2u(p_grid->kdegrid()->datasource()->name()).c_str());
    painter.drawText(leftborder,topborder/2,width-leftborder-rightborder,topborder,Qt::AlignRight,tablename);

    QString databasename=i18n("Database: ")+QString::fromUtf8(l2u(p_grid->kdegrid()->datasource()->database()->name()).c_str());
    painter.drawText(leftborder,topborder/2,width-leftborder-rightborder,topborder,Qt::AlignLeft,databasename);

} 


int  hk_kde4simplegridprivate::print_header(QPainter& painter,int width,sectionclass& horizontal,
                                            int topborder,int leftborder,int rightborder)
{
    int pagewidth=width-leftborder-rightborder;
    int rowheight=18;
    for (int col=horizontal.start;col<=horizontal.end;++col)
    {
        int colpos=p_grid->columnViewportPosition(col);
        painter.translate(colpos+leftborder-horizontal.offset,topborder);
        int colwidth=p_grid->columnWidth(col);
        int w = (colpos+colwidth-horizontal.offset>pagewidth?pagewidth-colpos+horizontal.offset:colwidth);
        int h = rowheight;
        int x2 = w - 1;
        int y2 = h - 1;
        painter.fillRect(0, 0, x2, y2 + 1, p_grid->palette().brush(QPalette::Button));
        painter.drawLine( x2, 0, x2, y2 + 1); // vertical line
        painter.drawLine( 0, y2 + 1, x2, y2 + 1); // horizontal line
        if (p_grid->kdegrid()->gridcolumn(col))
        {
        painter.drawText(2,3,w-7,
            rowheight-3,Qt::AlignLeft
            ,QString::fromUtf8 (p_grid->kdegrid()->gridcolumn(col)->displayname().size()==0?
            l2u(p_grid->kdegrid()->gridcolumn(col)->columnname()).c_str():
        l2u(p_grid->kdegrid()->gridcolumn(col)->displayname()).c_str()));
        }
        painter.translate(-colpos-leftborder+horizontal.offset,-topborder);
    }
    return rowheight;
}

const QString hk_kde4simplegrid::mimePreferredFormat = QString::fromAscii("text/tab-separated-values");
const QString hk_kde4simplegrid::mimeDefaultFormat = QString::fromAscii("text/plain");

hk_kde4simplegrid::hk_kde4simplegrid(hk_kdegrid *grid, hk_kdesimpleform */*form*/): QTableView(grid),
    p_grid(grid), p_private(new hk_kde4simplegridprivate(this))
{
    Q_ASSERT(grid != NULL);
    setObjectName("simplegrid");
    setSelectionMode(QAbstractItemView::ContiguousSelection);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setTabKeyNavigation(false);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setAlternatingRowColors(true);
    verticalHeader()->setDefaultAlignment(Qt::AlignRight | Qt::AlignVCenter);
    verticalHeader()->setResizeMode(QHeaderView::Interactive);
    verticalHeader()->setClickable(true);
    horizontalHeader()->setMovable(true);
    viewport()->setAcceptDrops(true);
    viewport()->setBackgroundRole(QPalette::Dark);

    connect(verticalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(rowHeightChanged(int, int, int)));
    connect(horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(columnWidthChanged(int, int, int)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(show_contextmenu(const QPoint &)));
    connect(verticalHeader(), SIGNAL(sectionPressed(int)), this, SLOT(rowHeaderPressed(int)));
}

hk_kde4simplegrid::~hk_kde4simplegrid(void)
{
    delete p_private;
}

int hk_kde4simplegrid::currentColumn(void)
{
    return 0;
}

bool hk_kde4simplegrid::isParentWidgetOf(QWidget* child)
{
    while(child) {
        if (child == this)
            return true;
        child = child->parentWidget();
    }
    return false;
}

void 	hk_kde4simplegrid::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    // set header foreground color directly
    if (model != NULL) {
        QVariant fBrush = model->headerData(0, Qt::Horizontal, Qt::ForegroundRole);
        if (fBrush.canConvert<QBrush>()) {
            QPalette pal = horizontalHeader()->palette();
            pal.setColor(QPalette::WindowText, fBrush.value<QBrush>().color());
            horizontalHeader()->setPalette(pal);
            verticalHeader()->setPalette(pal);
        }
        connect(model, SIGNAL(layoutChanged()), this, SLOT(modelLayoutChanged()));
    }

}

void hk_kde4simplegrid::columns_resize(void)
{
    for (int i = 0; i < model()->columnCount(); i++) {
        setColumnWidth(i, p_grid->gridcolumn(i)->columnwidth());
    }
}

void hk_kde4simplegrid::reset()
{
    horizontalHeader()->setSortIndicator(-1, Qt::DescendingOrder);
    setSortingEnabled(p_grid->datasource() != NULL && p_grid->datasource()->is_enabled()
                      && p_grid->datasource()->database()->connection()->server_supports(hk_connection::SUPPORTS_SQL_ORDER_BY)
                      && (!p_grid->datasource()->is_rawsql() || p_grid->datasource()->type() == hk_data::ds_view));

    QTableView::reset();
    if (p_grid->datasource() != NULL && p_private->hasFocus)
        setCurrentIndex(model()->index(0, 0));
    setEnabled(p_grid->datasource() != NULL);

    for (int i = 0; i < model()->columnCount(); i++) {
        switch (model()->data(model()->index(0, i), hk_kdegridmodel::ColumnTypeRole).toInt()) {
        case hk_kdegridmodel::columnedit :
            setItemDelegateForColumn(i, new LineEditDelegate(this, kdegrid()));
            break;
        case hk_kdegridmodel::columnbool:
            setItemDelegateForColumn(i, new CheckBoxDelegate(this, kdegrid()));
            break;
        case hk_kdegridmodel::columncombo:
            setItemDelegateForColumn(i, new ComboBoxDelegate(this, kdegrid()));
            break;
        case hk_kdegridmodel::columnmemo:
            setItemDelegateForColumn(i, new MemoDelegate(this, kdegrid()));
            break;
        default:
            ;
        }
    }
}

void hk_kde4simplegrid::modelLayoutChanged()
{
    selectionModel()->reset();
}


void hk_kde4simplegrid::print_grid(void)
{
// this is a workaround: otherwise float numbers in the postscript file
// would be created with locale dots (e.g. a comma in Germany) from Qt
    hk_string origlocale=setlocale(LC_NUMERIC,NULL);
    hk_string origmonetarylocale=setlocale(LC_MONETARY,NULL);
    int vheaderoffset=18;
    setlocale(LC_NUMERIC,"C");
    setlocale(LC_MONETARY,"C");
//workaround part 1 end
    QPrinter printer; 
    QPrintDialog pdlg(&printer,this);
    
    if (pdlg.exec() != QDialog::Accepted) {
//workaround part 2
        setlocale(LC_NUMERIC,origlocale.c_str());
        setlocale(LC_MONETARY,origmonetarylocale.c_str());
//workaround part 2 end
        return;        
    }
    QPainter painter;
    
    if (!painter.begin(&printer)) 
           return;

    const int topborder=50;
    const int bottomborder=50;
    const int leftborder=20;
    const int rightborder=20;
    p_private->pagenumber=1;    
    list<sectionclass> verticalpages;
    list<sectionclass> horizontalpages;
    int start=0;
    int offset=0;
    int oldoffset=0;
    sectionclass newpage;
    
    for (int row=0;row<model()->rowCount();++row)
    {
        int rowheight=rowHeight(row);
        if (offset+rowheight>=printer.height()-topborder-bottomborder-vheaderoffset&&offset!=0)
        {
            newpage.start=start;
            newpage.end=row-1;
            newpage.offset=oldoffset;
            verticalpages.insert(verticalpages.end(),newpage);
            oldoffset+=offset;
            offset=0;
            start=row;
        }
        offset+=rowheight;
    }
    newpage.start=start;
    newpage.end=model()->rowCount()-1;
    newpage.offset=oldoffset;
    verticalpages.insert(verticalpages.end(),newpage);
    list<sectionclass>::iterator it=verticalpages.begin();

    start=0;
    offset=0;
    oldoffset=0;
    for (int col=0;col<model()->columnCount();col++)
    {
        int colwidth=columnWidth(col);
        if (offset+colwidth>=printer.width()-leftborder-rightborder&&offset!=0)
        {
            sectionclass newpage;
            newpage.start=start;
            newpage.end=col-1;
            newpage.offset=oldoffset;
            horizontalpages.insert(horizontalpages.end(),newpage);
            oldoffset+=offset;
            offset=0;
            start=col;
        }
        offset+=colwidth;
    }

    newpage.start=start;
    newpage.end=model()->columnCount()-1;
    newpage.offset=oldoffset;
    horizontalpages.insert(horizontalpages.end(),newpage);

    list<sectionclass>::iterator colit=horizontalpages.begin();
    it=verticalpages.begin();
    unsigned int v=1;
    colit=horizontalpages.begin();
    while (it!=verticalpages.end())
    {
        unsigned int h=1;
        colit=horizontalpages.begin();
        while (colit!=horizontalpages.end())
        {
            p_private->print_singlepage(painter, viewOptions(), printer.width(), printer.height(), ((*it)),
                                        ((*colit)), topborder, bottomborder, leftborder, rightborder);
            ++p_private->pagenumber;
            ++colit;
            if (!(h==horizontalpages.size()&&v==verticalpages.size()))
            {
                printer.newPage();
            }
            ++h;
        }
        ++it;++v;
    } 
    painter.end();
//workaround part 2
    setlocale(LC_NUMERIC,origlocale.c_str());
    setlocale(LC_MONETARY,origmonetarylocale.c_str());
//workaround part 2 end    
}

bool hk_kde4simplegrid::canProcessClipboard() const
{
    const QMimeData *clipboarddata = QApplication::clipboard()->mimeData();
    return clipboarddata != NULL &&
           (clipboarddata->hasFormat(mimePreferredFormat)  ||
            clipboarddata->hasFormat(mimeDefaultFormat));
}

void hk_kde4simplegrid::copy(void)
{
    QMimeData *mimedata = new QMimeData();
    const QByteArray &copydata = marked_text().toUtf8();

    mimedata->setData(mimePreferredFormat, copydata);
    mimedata->setData(mimeDefaultFormat, copydata);
    QApplication::clipboard()->setMimeData(mimedata);
}

void hk_kde4simplegrid::paste(void)
{
    const QMimeData *clipmime = QApplication::clipboard()->mimeData();

    if (clipmime == NULL) return;

    if (clipmime->hasFormat(mimePreferredFormat)) {
        QString clipText = QString::fromUtf8(clipmime->data(mimePreferredFormat));

        paste_tsv(clipText);
        return;
    }
    if (clipmime->hasFormat(mimeDefaultFormat)) {
        QString clipText = QString::fromUtf8(clipmime->data(mimeDefaultFormat));

        model()->setData(currentIndex(), QVariant(clipText), hk_kdegridmodel::PasteRole);
    }
}

void hk_kde4simplegrid::cut(void)
{
    //Not implemented as not reachable in knoda
}

void hk_kde4simplegrid::paste_tsv(const QString &src)
{
    // handle special case: one cell with NULL value
    if (src.isEmpty()) {
        model()->setData(currentIndex(), QVariant(src), hk_kdegridmodel::PasteRole);
        return;
    }

    QStringList rows = src.split('\n', QString::SkipEmptyParts);
    QStringList::const_iterator iterRows, iterCols;
    int startCol = currentIndex().column();

    for (iterRows = rows.constBegin(); iterRows != rows.constEnd(); iterRows++) {
        QStringList cols = (*iterRows).split('\t', QString::KeepEmptyParts);

        for (iterCols = cols.constBegin(); iterCols != cols.constEnd(); iterCols++) {
            QString cellvalue = (*iterCols);

            cellvalue.replace("\\t", "\t").replace("\\n", "\n").replace("\\/", "\\");
            model()->setData(currentIndex(), QVariant(cellvalue), hk_kdegridmodel::PasteRole);

            if (horizontalHeader()->visualIndex(currentIndex().column()) + 1 < model()->columnCount())
                setCurrentIndex(model()->index(currentIndex().row(),
                                horizontalHeader()->logicalIndex(
                                horizontalHeader()->visualIndex(currentIndex().column()) + 1)));
            else
                break;
        }
        if (currentIndex().row() + 1 < model()->rowCount())
            setCurrentIndex(model()->index(currentIndex().row() + 1 , startCol));
        else
            break;
    }
}

void hk_kde4simplegrid::currentChanged(const QModelIndex & curr, const QModelIndex & prev )
{
    if (p_private->hasFocus)
        emit focusedCellChanged(prev.row(), prev.column(), curr.row(), curr.column());
    QTableView::currentChanged(curr, prev);
}


void hk_kde4simplegrid::rowHeightChanged(int /*Index*/, int /*oldheight*/, int newheight)
{
    p_grid->set_rowheight(newheight);
}

void hk_kde4simplegrid::columnWidthChanged(int Index, int /*oldwidth*/, int newwidth)
{
    p_grid->set_gridcolumnwidth(Index, newwidth);
}

void hk_kde4simplegrid::rowHeaderPressed(int Index)
{
    selectRow(Index);
}

void hk_kde4simplegrid::updateRowHeight(void)
{
    verticalHeader()->setDefaultSectionSize(p_grid->rowheight());
}

void hk_kde4simplegrid::keyPressEvent(QKeyEvent *event)
{
    // cursor movement when Enter is pressed
    if (event->key() == Qt::Key_Return
            && moveNextPrevCell(event->modifiers() & Qt::ControlModifier ? QAbstractItemView::MovePrevious 
            : QAbstractItemView::MoveNext)) {
        event->accept();
        return;
    }
    if (event->key() == Qt::Key_Delete
            && (currentIndex().row() < model()->rowCount() - 1)) {
        p_grid->delete_selected_rows();
        event->accept();
        return;
    }

    QTableView::keyPressEvent(event);
}

void hk_kde4simplegrid::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        p_private->dragStartPos = event->pos();
        p_private->dragInitPhase = selectionModel()->isSelected(indexAt(p_private->dragStartPos));
    }
    QTableView::mousePressEvent(event);
}

void hk_kde4simplegrid::mouseMoveEvent(QMouseEvent* event)
{
    if (p_private->dragInitPhase && (event->buttons() & Qt::LeftButton)
        && ((event->pos() - p_private->dragStartPos).manhattanLength() >= QApplication::startDragDistance())) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
    
        p_private->dragInitPhase = false;
        // finish selection operation we started in mousePressEvent()
        mouseReleaseEvent(event);
        selectionModel()->clearSelection();
        selectionModel()->select(indexAt(p_private->dragStartPos), QItemSelectionModel::Select);
        mimeData->setText(currentIndex().data(Qt::DisplayRole).toString());
        drag->setMimeData(mimeData);
        drag->exec(Qt::CopyAction);
        event->accept();
    } else 
        QTableView::mouseMoveEvent(event);
}

void hk_kde4simplegrid::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat(mimeDefaultFormat)
        && (indexAt(event->pos()).flags() & Qt::ItemIsDropEnabled))
       event->acceptProposedAction();
}

void hk_kde4simplegrid::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasFormat(mimeDefaultFormat)
        && (indexAt(event->pos()).flags() & Qt::ItemIsDropEnabled)
        && indexAt(event->pos()) != indexAt(p_private->dragStartPos))
       event->acceptProposedAction();
    else
        event->ignore();
}

void hk_kde4simplegrid::dropEvent(QDropEvent* event)
{
    QModelIndex target = indexAt(event->pos());
    
    if (event->mimeData()->hasFormat(mimeDefaultFormat)
        && (target.flags() & Qt::ItemIsDropEnabled)
        && target != indexAt(p_private->dragStartPos)) {    
        setCurrentIndex(target);
        model()->setData(currentIndex(), QVariant(event->mimeData()->text()), hk_kdegridmodel::PasteRole);
        event->acceptProposedAction();
    } else
        event->ignore();
}

void hk_kde4simplegrid::focusChanged(QWidget* oldWidget, QWidget* newWidget)
{

    if (isParentWidgetOf(newWidget) && !isParentWidgetOf(oldWidget)) {
        //enter grid  
        emit signal_infocus();
    } 

    if (isParentWidgetOf(oldWidget) && !isParentWidgetOf(newWidget)) {
        //leave grid  
        p_private->hasFocus = false;
        disconnect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));
        if (currentIndex().isValid())
            emit focusedCellChanged(currentIndex().row(),currentIndex().column(),-1,-1);
        emit signal_outfocus();
    }  
}

void hk_kde4simplegrid::focusInEvent ( QFocusEvent * /*e*/)
{
    if (!p_private->hasFocus) {
        p_private->hasFocus = true;
        connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));
        if (currentIndex().isValid())
            emit focusedCellChanged(-1,-1,currentIndex().row(),currentIndex().column());
    }  
}

void hk_kde4simplegrid::focusOutEvent ( QFocusEvent * /*e*/)
{
}

bool hk_kde4simplegrid::focusNextPrevChild(bool next)
{
    // next/previous child while editing means goto next cell
    if (isAncestorOf(QApplication::focusWidget())
            && state() == QAbstractItemView::EditingState) {
        if (p_private->leavingRow(next))
            p_grid->save_changed();
        moveNextPrevCell(next ? QAbstractItemView::MoveNext : QAbstractItemView::MovePrevious);
        return true;
    }
    return QTableView::focusNextPrevChild(next);
}

bool hk_kde4simplegrid::moveNextPrevCell(QAbstractItemView::CursorAction direction)
{
    QPersistentModelIndex newCur;

    newCur = moveCursor(direction, 0);
    if (newCur != currentIndex() && newCur.isValid()) {
        if (!hasFocus() && QApplication::focusWidget() == indexWidget(currentIndex()))
            setFocus();
        setCurrentIndex(newCur);
        return true;
    }
    return false;
}

QString hk_kde4simplegrid::marked_text(void)
{
    QString result;

    if (selectionModel()->hasSelection()) {
        QItemSelection sel = selectionModel()->selection();

        for (int r = sel[0].topLeft().row(); r <= sel[0].bottomRight().row(); r++) {
            for (int i = 0; i < sel.size(); i++) {
                QItemSelectionRange s = sel[i];

                for (int c = s.topLeft().column(); c <= s.bottomRight().column(); c++) {
                    QString d = model()->data(model()->index(r, c)).toString();

                    d.replace('\\', "\\/").replace('\n', "\\n").replace('\t', "\\t");
                    if (i != 0 || c != s.topLeft().column())
                        result += '\t';
                    result += d;
                }
            }
            if (r < sel[0].bottomRight().row())
                result += '\n';
        }
        return result;
    }
    return result;
}

void hk_kde4simplegrid::show_contextmenu(const QPoint &/*pos*/)
{
    KMenu *popup = new KMenu(this);

    p_grid->populate_context_menu(this, popup);
    if (popup->actions().count() > 0)
        popup->exec(QCursor::pos());

    delete popup;
}

void hk_kde4simplegrid::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    // handle Tab/BackTab in cell editor
    if ((hint == QAbstractItemDelegate::EditNextItem
            || hint == QAbstractItemDelegate::EditPreviousItem)
            && p_private->leavingRow(hint == QAbstractItemDelegate::EditNextItem)) {
        p_grid->save_changed();
        moveNextPrevCell(hint == QAbstractItemDelegate::EditNextItem ? QAbstractItemView::MoveNext 
            : QAbstractItemView::MovePrevious);
        edit(currentIndex());
    } else
        QTableView::closeEditor(editor, hint);
}
