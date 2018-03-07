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
//$Revision: 1.63 $
#include "hk_class.h"
#include <hk_kdegrid.h>
#include <hk_database.h>
#include <hk_kdegrid.moc>
#include <hk_kde4simplegrid.h>
#include <hk_kde4simplegrid_p.h>
#include <hk_kderowselector.h>
#include <hk_dscombobox.h>
#include "hk_kdegridcolumndialog.h"
#include <qpainter.h>
#include <qpalette.h>
#include <qdrawutil.h>
#include <qscrollbar.h>
#include <qsizepolicy.h>
#include <hk_kdesimpleform.h>
#include "hk_dsgridcolumn.h"
#include "hk_kdefinddialog.h"
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qitemselectionmodel.h>
#include <qheaderview.h>
#include <qclipboard.h>

#include <kcomponentdata.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kaction.h>
#include <klocale.h>
#include <kdebug.h>
#include <kapplication.h>
#include <kmenu.h>
#include <kfiledialog.h>

class hk_kdegridprivate
{
    
 public:
     
    enum Source
    { Undef, Datasource, Gridview };
    
    hk_kdegridprivate(hk_kdegrid *grid,hk_kdesimpleform *form): p_finddialog(NULL), p_findfirstview(true), 
        columnmergeneeded(false), whileactionexecuting(false), ignorefocus(false), hbottomlayout(new QHBoxLayout), hdatalayout(new QHBoxLayout), 
        htoplayout(new QHBoxLayout), vbasiclayout(new QVBoxLayout(grid)), p_grid(grid), 
        p_view(new hk_kde4simplegrid(grid, form)), p_model(new hk_kdegridmodel(grid)), 
        selector(new hk_kderowselector(grid, form)), statbar(new QLabel(grid)),p_part(NULL), 
        p_definecolumnsaction(NULL), p_reloadaction(NULL), p_deleterowaction(NULL), p_findaction(NULL), 
        p_setnullaction(NULL), p_loadaction(NULL), p_saveaction(NULL), 
        p_copyaction(new QAction(i18n("Copy"), grid)), p_pasteaction(NULL), previousrow(0), 
        initiator(Undef) {
        Q_ASSERT(grid);
        vbasiclayout->setMargin(3);
        vbasiclayout->setSpacing(0);
    }

    void mergeColumnOrder() {
        Q_ASSERT(columnmergeneeded);       
        Q_ASSERT((uint)p_view->horizontalHeader()->count() == p_grid->columnscount());

        vector<hk_dsgridcolumn *> targetColOrder(p_view->horizontalHeader()->count());


        // store target column order
        for (int i = 0; i < p_view->horizontalHeader()->count(); i++)
            targetColOrder[i] = p_grid->gridcolumn(p_view->horizontalHeader()->logicalIndex(i));

        for (int j = 0; j < p_view->horizontalHeader()->count() - 1; j++) {
            int fromPosition = j;
            
            // find current position of a column 
            while (targetColOrder[j] != p_grid->gridcolumn(fromPosition))
                fromPosition++;
            // if current position does not match with target position, move column
            if (j != fromPosition)
                p_grid->change_columnposition(fromPosition, j);
            // fix visual to logical index mapping, it must be identity function
            p_view->horizontalHeader()->moveSection(p_view->horizontalHeader()->visualIndex(j), j);
        }
        columnmergeneeded = false;
    }

    void setStatusText(void) {
        Q_ASSERT(statbar);
        QString stattext = i18n("Datasource: ");

        if (p_grid->datasource() != NULL) {
            int col = p_view->currentIndex().column();

            if (p_grid->datasource()->is_readonly() || p_grid->is_readonly())
                stattext += i18n("readonly");
            else {
                stattext += i18n("readwrite");
                stattext += i18n(" Column: ");
                if (p_grid->gridcolumn(col) && p_grid->gridcolumn(col)->column())
                    stattext += p_grid->gridcolumn(col)->is_readonly()
                                || p_grid->gridcolumn(col)->column()->is_readonly() ?
                                i18n("readonly") : i18n("readwrite");
                else
                    stattext += i18n("not set");

            }
        } else
            stattext += i18n("not set");
        statbar->setText(stattext);
    }

    inline void safeSetInitiator( Source src) 
    {
        if (initiator == hk_kdegridprivate::Undef)
            initiator = src;
    }

    inline void safeClearInitiator( Source src) 
    {    
        if (initiator == src)
            initiator = hk_kdegridprivate::Undef;
    }

    hk_kdefinddialog *p_finddialog;
    bool          p_findfirstview;
    bool          columnmergeneeded;
    bool          whileactionexecuting;
    bool          ignorefocus;
    QHBoxLayout *hbottomlayout;
    QHBoxLayout *hdatalayout;
    QHBoxLayout *htoplayout;
    QVBoxLayout *vbasiclayout;

    hk_kdegrid *p_grid;
    hk_kde4simplegrid *p_view;
    hk_kdegridmodel *p_model;
    hk_kderowselector *selector;
    QLabel     *statbar;
    hk_kdegridpart *p_part;
    QAction *p_definecolumnsaction;
    QAction *p_reloadaction;
    QAction *p_deleterowaction;
    QAction *p_findaction;
    QAction *p_setnullaction;
    QAction *p_loadaction;
    QAction *p_saveaction;
    QAction *p_copyaction;
    QAction *p_pasteaction;
    unsigned long previousrow;
    Source initiator;
};


//*******************************
//***       hk_kdegrid        ***
//*******************************

hk_kdegrid::hk_kdegrid(QWidget *wid, const char *n, Qt::WFlags f, hk_kdesimpleform *form)
    : QFrame(wid, f), hk_dsgrid(form), p(new hk_kdegridprivate(this,form))
{
#ifdef HK_DEBUG
    hkclassname("hk_kdegrid");
    hkdebug("hk_kdegrid::hk_kdegrid");
#endif
    setObjectName(QString::fromAscii(n));
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSizePolicy fixedpolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSizePolicy labelpolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    if (wid != NULL) setFrameStyle(Box | Sunken);
    setAutoFillBackground(true); 
    p-> p_view->setModel(p->p_model);
    setFocusProxy(p->p_view);
    p->selector->setFocusPolicy(Qt::ClickFocus);
    setFocusPolicy(Qt::StrongFocus);
    p->selector->setSizePolicy(fixedpolicy);
    p->statbar->setSizePolicy(labelpolicy);
    p->p_view->setSizePolicy(policy);
    p->vbasiclayout->addLayout(p->htoplayout, 0);
    p->vbasiclayout->addLayout(p->hdatalayout, 1);
    p->vbasiclayout->addLayout(p->hbottomlayout, 0);
    int hbheight = 20;
    p->hdatalayout->addWidget(p->p_view);
    p->hbottomlayout->addWidget(p->selector);
    p->hbottomlayout->addWidget(p->statbar);
    p->hbottomlayout->setEnabled(true);
    p->statbar->setFrameStyle(WinPanel | Sunken);
    p->statbar->setMidLineWidth(0);
    p->statbar->setLineWidth(1);
    p->statbar->setText("STATUS");
    p->selector->setMaximumHeight(hbheight);
    p->selector->setMaximumWidth(200);
    p->selector->setMinimumWidth(200);
    p->selector->setMinimumHeight(hbheight);
    p->statbar->setMaximumHeight(hbheight);
    p->statbar->setMinimumWidth(0);
    p->statbar->setAutoFillBackground(true);
    setLayout(p->vbasiclayout);
    p->vbasiclayout->setEnabled(true);
    p->vbasiclayout->activate();
    p_visibletype = hk_visible::grid;
    QColor color = KApplication::palette().color(QPalette::Base);
    hk_colour colour(color.red(), color.green(), color.red());
    set_backgroundcolour(colour, false, true);
    color = KApplication::palette().color(QPalette::Text);
    colour.set_colour(color.red(), color.green(), color.blue());
    set_foregroundcolour(colour, false, true);
    widget_specific_font_changed();
    connect(p->p_view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            this, SLOT(current_row_changed()));
    connect(p->p_view->selectionModel(), SIGNAL(currentColumnChanged(QModelIndex, QModelIndex)),
            this, SLOT(current_column_changed()));
    connect(p->p_view->horizontalHeader(), SIGNAL(sectionMoved(int, int, int)),
            this, SLOT(columns_moved(int, int, int)));
    connect(p->p_view, SIGNAL(clicked(const QModelIndex&)), this, SLOT(slot_mouse_clicked()));
    connect(p->p_view, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(slot_mouse_doubleclicked()));
    connect(p->p_view, SIGNAL(signal_infocus()), this, SLOT(slot_simplegrid_focusin()));
    connect(p->p_view, SIGNAL(signal_outfocus()), this, SLOT(slot_simplegrid_focusout()));
    connect(p->p_view, SIGNAL(focusedCellChanged(int,int,int,int)), this, SLOT(slot_simplegrid_focuschanged(int,int,int,int)));
}


hk_kdegrid::~hk_kdegrid()
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::~hk_kdegrid");
#endif
    p_presentation = NULL;
    delete p;
}

hk_kde4simplegrid *hk_kdegrid::simplegrid(void)
{
    return p->p_view;
}

hk_kdegridpart   *hk_kdegrid::part(void)
{
    return p->p_part;
}

void hk_kdegrid::setpart(hk_kdegridpart *part)
{
    p->p_part = part;
}

void hk_kdegrid::set_datasource(hk_datasource *d)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::datasource(hk_datasource* d)");
#endif
    if ((d != datasource()) && p->columnmergeneeded) 
        p->mergeColumnOrder();
    p->p_model->set_datasource(d);
    p->selector->set_datasource(d);
    hk_dsgrid::set_datasource(d);
    p->previousrow = 0;
}

hk_string hk_kdegrid::combo_value_at(uint column, ulong row)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::combo_value_at");
#endif
    hk_dscombobox *combo = gridcolumn(column)->combobox();

    if (combo == NULL || combo->datasource() == NULL || combo->column() == NULL) return "";

    bool isCurrentRow = combo->datasource()->row_position() == row;
    bool isInsertMode = combo->datasource()->mode() == hk_datasource::mode_insertrow;

    if ((combo->listcolumn() == NULL) || (combo->listdatasource() == NULL) || (combo->viewcolumn() == NULL)) {
        if (combo->column()->has_changed() && isCurrentRow)
            return (!combo->column()->changed_data_is_nullvalue() && is_numerictype(combo->column()) ?
                    format_number(combo->column()->changed_data_asstring(), true, combo->use_numberseparator(), combo->commadigits(), combo->locale())
                    : combo->column()->changed_data_asstring());
        return (!combo->column()->is_nullvalue_at(row) && is_numerictype(combo->column()) ?
                format_number(combo->column()->asstring_at(row, true), true, combo->use_numberseparator(), combo->commadigits(), combo->locale())
                : combo->column()->asstring_at(row, true));
    }
    unsigned int r = 0;
    if (combo->column()->has_changed() && ((isCurrentRow && !isInsertMode)
                                           || row >= combo->datasource()->max_rows()))
        r = combo->listcolumn()->find(combo->column()->changed_data_asstring(true), true);
    else
        r = combo->listcolumn()->find(combo->column()->asstring_at(row, true), true);
    if (r <= combo->listdatasource()->max_rows()) {
        if (is_numerictype(combo->viewcolumn()))
            return format_number(combo->viewcolumn()->asstring_at(r, true), true, combo->use_numberseparator(), combo->commadigits(), combo->locale());
        else
            return combo->viewcolumn()->asstring_at(r, true);
    } else
        return "";
}


bool hk_kdegrid::widget_specific_coordinates(uint px, uint py, uint pwidth, uint pheight)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::widget_specific_coordinates");
#endif
    unsigned int ppx = px;
    unsigned int ppy = py;
    unsigned int ppw = pwidth;
    unsigned int pph = pheight;
    if (p_presentation != NULL) {

        ppx = p_presentation->horizontal2relativ(ppx);
        ppy = p_presentation->vertical2relativ(ppy);
        pph = p_presentation->vertical2relativ(pph);
        ppw = p_presentation->horizontal2relativ(ppw);

    }
    blockSignals(true);
    setGeometry(px, py, pwidth, pheight);
    blockSignals(false);
    return true;
}


void hk_kdegrid::widget_specific_presentationdatasource(long n)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::widget_specific_presentationdatasource");
#endif
    p->selector->set_presentationdatasource(n, false);

}


void hk_kdegrid::widget_specific_enable_disable(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::widget_specific_enable_disable");
#endif
    if (p_presentation != NULL) {
        if (p_presentation->mode() == hk_form::designmode) {
            setFocusPolicy(Qt::NoFocus);
            setEnabled(false);
        } else {
            setFocusPolicy(Qt::StrongFocus);
            setEnabled(true);
        }
    }
    if (p->columnmergeneeded)
        p->mergeColumnOrder();
    p->p_model->grid_reset();

}

bool hk_kdegrid::widget_specific_row_change(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::widget_specific_row_change");
#endif
    p->safeSetInitiator(hk_kdegridprivate::Datasource);

    p->p_model->currentRowChanged(p->previousrow);
    if (p->initiator != hk_kdegridprivate::Gridview)
        p->p_view->setCurrentIndex(p->p_model->index(datasource()->row_position(),
                                   p->p_view->currentIndex().column()));

    p->safeClearInitiator(hk_kdegridprivate::Datasource);
    return true;
}

void    hk_kdegrid::current_row_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::currentrowchanged");
#endif

    p->safeSetInitiator(hk_kdegridprivate::Gridview);

    if (p->initiator == hk_kdegridprivate::Gridview
            && datasource() != NULL && datasource()->is_enabled()) {
        ulong row = p->p_view->currentIndex().row();

        if (row >= datasource()->max_rows())
            datasource()->setmode_insertrow();
        else
            datasource()->goto_row(row);
    }
    
    p->safeClearInitiator( hk_kdegridprivate::Gridview);
}

void    hk_kdegrid::current_column_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::currentcolumnchanged");
#endif
    p->setStatusText();
}

void hk_kdegrid::columns_moved(int /*logicalIndex*/, int /*oldVisualIndex*/, int /*newVisualIndex*/)
{
    p->columnmergeneeded = true;
}

void hk_kdegrid::slot_mouse_clicked(void)
{
   if (p_presentation && p_presentation->mode() == hk_presentation::viewmode)
   {
       action_on_click();
       if (p->p_model->columnCount() > 0 && gridcolumn(p->p_view->currentIndex().column()))
           gridcolumn(p->p_view->currentIndex().column())->action_on_click();
   }
}

void hk_kdegrid::slot_mouse_doubleclicked(void)
{
   if (p_presentation && p_presentation->mode() == hk_presentation::viewmode)
   {
       action_on_doubleclick();
       if (p->p_model->columnCount() > 0 && gridcolumn(p->p_view->currentIndex().column()))
           gridcolumn(p->p_view->currentIndex().column())->action_on_doubleclick();
   }
}

void hk_kdegrid::slot_simplegrid_focusin(void)
{
    if (p_presentation && p_presentation->mode() == hk_presentation::viewmode) {
        action_on_getfocus();
    }
}

void hk_kdegrid::slot_simplegrid_focusout(void)
{
    if (p_presentation && p_presentation->mode() == hk_presentation::viewmode) {
        action_on_loosefocus();
    }
}

void  hk_kdegrid::slot_simplegrid_focuschanged(int prevRow, int prevCol, int currRow, int currCol)
{
    if (((prevRow != currRow) || (prevCol != currCol)) && 
        columnscount() > 0) {
        if (prevCol >=0)
            gridcolumn(prevCol)->action_on_loosefocus();
        if (currCol >=0)
            gridcolumn(currCol)->action_on_getfocus();
    }
}

void hk_kdegrid::widget_specific_insert_mode(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::widget_specific_insert_mode");
#endif
    if (is_readonly()) return;
    if (p->initiator == hk_kdegridprivate::Undef)
        p->initiator = hk_kdegridprivate::Datasource;
    p->p_model->currentRowChanged(p->previousrow);
    if (p->initiator != hk_kdegridprivate::Gridview)
        p->p_view->setCurrentIndex(p->p_model->index(datasource()->max_rows(),
                                   p->p_view->currentIndex().column()));

    if (p->initiator == hk_kdegridprivate::Datasource)
        p->initiator = hk_kdegridprivate::Undef;
}


void hk_kdegrid::reposition(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::reposition");
#endif
    if (p_presentation == NULL || p_setcoordinates == true) return;
    p_setwidgetcoordinates = true;
    unsigned int ppx = p_presentation->sizetype() == hk_presentation::relative ? p_presentation->horizontal2relativ(QFrame::x()) : QFrame::x();
    unsigned int ppy = p_presentation->sizetype() == hk_presentation::relative ? p_presentation->vertical2relativ(QFrame::y()) : QFrame::y();
    unsigned int pph = p_presentation->sizetype() == hk_presentation::relative ? p_presentation->vertical2relativ(QFrame::height()) : QFrame::height();
    unsigned int ppw = p_presentation->sizetype() == hk_presentation::relative ? p_presentation->horizontal2relativ(QFrame::width()) : QFrame::width();
    set_size(ppx, ppy, ppw, pph);
    p_setwidgetcoordinates = false;
}

void hk_kdegrid::widget_specific_columns_created(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::widget_specific_columns_created");
#endif
    p->p_model->grid_reset();
    p->p_view->columns_resize();
    p->setStatusText();
}

void hk_kdegrid::resizeEvent(QResizeEvent *event)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::resizeEvent");
#endif
    QFrame::resizeEvent(event);
    reposition();

}


void  hk_kdegrid::moveEvent(QMoveEvent *event)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::resizeEvent");
#endif
    QFrame::moveEvent(event);
    reposition();
}

void hk_kdegrid::populate_context_menu(QWidget * /*owner*/, KMenu *cmenu)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::populate_context_menu");
#endif
    if (cmenu == NULL || datasource() == NULL) return;
    if (presentation() && presentation()->mode() == hk_presentation::designmode) return;

    if (p->p_definecolumnsaction == NULL) {
        p->p_definecolumnsaction = new QAction(i18n("Define columns"), this);
        connect(p->p_definecolumnsaction, SIGNAL(triggered()), this, SLOT(show_gridcolumndialog()));
        p->p_reloadaction = new QAction(i18n("Reload"), this);
        connect(p->p_reloadaction, SIGNAL(triggered()), this, SLOT(reload()));
        p->p_deleterowaction = new QAction(i18n("Delete row"), this);
        connect(p->p_deleterowaction, SIGNAL(triggered()), this, SLOT(delete_current_row()));
        p->p_findaction = new QAction(i18n("Find"), this);
        connect(p->p_findaction, SIGNAL(triggered()), this, SLOT(find_clicked()));
        p->p_setnullaction = new QAction(i18n("set 'NULL' value"), this);
        p->p_setnullaction->setShortcut(Qt::ControlModifier + Qt::Key_0);
        connect(p->p_setnullaction, SIGNAL(triggered()), this, SLOT(set_column_to_null()));
        p->p_loadaction = new QAction(i18n("Load from file"), this);
        connect(p->p_loadaction, SIGNAL(triggered()), this, SLOT(load_column_from_file()));
        p->p_saveaction = new QAction(i18n("Save to file"), this);
        connect(p->p_saveaction, SIGNAL(triggered()), this, SLOT(save_column_to_file()));
        p->p_copyaction = new QAction(i18n("Copy"), this);
        connect(p->p_copyaction, SIGNAL(triggered()), p->p_view, SLOT(copy()));
        p->p_copyaction->setShortcut(Qt::ControlModifier + Qt::Key_C);
        p->p_pasteaction = new QAction(i18n("Paste"), this);
        connect(p->p_pasteaction, SIGNAL(triggered()), p->p_view, SLOT(paste()));
        p->p_pasteaction->setShortcut(Qt::ControlModifier + Qt::Key_V);
    }
    hk_column *c = gridcolumn(p->p_view->currentIndex().column()) ?
                   gridcolumn(p->p_view->currentIndex().column())->column() : NULL;

    if (!runtime_only())
        cmenu-> addAction(p->p_definecolumnsaction);
    cmenu->addAction(p->p_findaction);
    cmenu->addSeparator();
    cmenu->addAction(p->p_deleterowaction);
    if (c && !c->is_readonly() && !c->is_notnull())
        cmenu->addAction(p->p_setnullaction);
    if (c && !c->is_readonly()
            && (c->columntype() == hk_column::memocolumn || c->columntype() == hk_column::binarycolumn)) {
        cmenu->addAction(p->p_loadaction);
        cmenu->addAction(p->p_saveaction);
    }
    cmenu->addSeparator();
    cmenu->addAction(p->p_copyaction);

    p->p_pasteaction->setEnabled(c && !c->is_readonly() && !c->is_notnull()
                                 && p->p_view->canProcessClipboard());
    cmenu->addAction(p->p_pasteaction);
    cmenu->addSeparator();
    cmenu->addAction(p->p_reloadaction);
}

void    hk_kdegrid::reload(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::reload");
#endif
    if (datasource() != NULL) {
        unsigned long position = datasource()->row_position();

        datasource()->disable();
        datasource()->enable();
        datasource()->goto_row(position);
        setFocus();
    }
}

void    hk_kdegrid::set_column_to_null(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::set_column_to_null");
#endif
    hk_column *c = gridcolumn(p->p_view->currentIndex().column()) ?
                   gridcolumn(p->p_view->currentIndex().column())->column() : NULL;

    if (c != NULL) c->set_asnullvalue();
}

void    hk_kdegrid::load_column_from_file(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::load_column_from_file");
#endif
    hk_column *c = gridcolumn(p->p_view->currentIndex().column()) ?
                   gridcolumn(p->p_view->currentIndex().column())->column() : NULL;
    QString s = KFileDialog::getOpenFileName(KUrl("kfiledialog:///bin"), QString::null, this);

    if (!s.isEmpty())
        c->load_from_file(u2l(s.toUtf8().data()));
}

void    hk_kdegrid::save_column_to_file(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::save_column_to_file");
#endif
    hk_column *c = gridcolumn(p->p_view->currentIndex().column()) ?
                   gridcolumn(p->p_view->currentIndex().column())->column() : NULL;
    QString s = KFileDialog::getSaveFileName(KUrl("kfiledialog:///bin"), QString::null, this);

    if (!s.isEmpty()) c->save_to_file(u2l(s.toUtf8().data()));
}

void    hk_kdegrid::show_gridcolumndialog(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::show_gridcolumndialog");
#endif
    if (p->columnmergeneeded)
        p->mergeColumnOrder();
    
    hk_kdegridcolumndialog *dialog = new hk_kdegridcolumndialog(this, 0, true);

    if (dialog->exec() == QDialog::Accepted)
        has_changed(true, forcesetting);
    widget_specific_enable_disable();
    delete dialog;
}

void    hk_kdegrid::delete_selected_rows(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::delete_selected_rows");
#endif
    if (p->p_view->selectionModel()->hasSelection()) {
        QItemSelection s = p->p_view->selectionModel()->selection();

        p->p_model->beginRemoveRows(s.first().topLeft().parent(), s.first().topLeft().row(),
                                    s.first().bottomRight().row());
        datasource()->delete_rows(s.first().topLeft().row(), s.first().bottomRight().row());
        p->p_model->endRemoveRows();
    }
}

void    hk_kdegrid::delete_current_row(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::delete_current_row");
#endif
    if (datasource() != NULL)
        datasource()->delete_actualrow();
}

void hk_kdegrid::find_clicked(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::find_clicked");
#endif
    if (p->p_finddialog == NULL) {
        p->p_finddialog = new hk_kdefinddialog(this, 0, false);
        connect(p->p_finddialog, SIGNAL(signal_findbutton_clicked()), this, SLOT(find_next()));
        connect(p->p_finddialog, SIGNAL(signal_findargument_changed()), this, SLOT(findargument_changed()));
    }

    p->p_findfirstview = true;
    findargument_changed();
    p->p_finddialog->show();
}


void    hk_kdegrid::find_next(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::find_next");
#endif
    bool search = true;
    if (p->p_finddialog == NULL) return;
    while (search) {
        if (p->p_finddialog->all_columnsbox->isChecked()) {
            cout << "noch nicht eingebaut" << endl;
            search = false;
        }

        else {
            uint from = 0;
            uint to = 0;
            if (p->p_finddialog->find_backwardsbox->isChecked()) {

                from = 0;
                to = ((datasource()->row_position() > 0) && !p->p_findfirstview) ?
                     datasource()->row_position() - 1 :
                     (p->p_findfirstview ? datasource()->row_position() : datasource()->max_rows() - 1);
            } else {
                from = ((datasource()->row_position() < datasource()->max_rows() - 2) && !p->p_findfirstview) ?
                       datasource()->row_position() + 1 :
                       (p->p_findfirstview ? datasource()->row_position() : 0);
                to = datasource()->max_rows() - 1;
            }
            uint r = gridcolumn(p->p_view->currentIndex().column())->find(from,
                     to, u2l(p->p_finddialog->searchfield->currentText().toUtf8().data()),
                     !p->p_finddialog->part_of_columnbox->isChecked(),
                     p->p_finddialog->case_sensitivebox->isChecked(),
                     p->p_finddialog->find_backwardsbox->isChecked());
            if (r < datasource()->max_rows()) {
                uint c = p->p_view->currentIndex().column();
                datasource()->goto_row(r);
                p->p_view->clearSelection();
                p->p_view->setCurrentIndex(p->p_model->index(r, c));
                search = false;
            } else {
                if (!p->p_findfirstview) {
                    show_warningmessage(hk_translate("Not found."));
                    search = false;
                    return;
                } else {
                    datasource()->goto_first();
                }

            }
        }

        p->p_findfirstview = false;
    }

}


void hk_kdegrid::findargument_changed(void)
{
    p->p_findfirstview = true;
}


void  hk_kdegrid::before_row_change(void)
{
    p->previousrow = datasource()->row_position();
    hk_dsgrid::before_row_change();
}


void hk_kdegrid::widget_specific_font_changed(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::widget_specific_font_changed");
#endif
    setFont(QFont(QString::fromUtf8(l2u(hk_dsgrid::font().fontname()).c_str())
                  , hk_dsgrid::font().fontsize(), hk_dsgrid::font().bold() ? QFont::Bold : QFont::Normal,
                  hk_dsgrid::font().italic()));
//    statusbar->setFont(QFont(QString::fromUtf8 (l2u(p_font).c_str()),p_fontsizeint));
}


void hk_kdegrid::print_grid(void)
{
#ifdef HK_DEBUG
    hkdebug("hk_kdegrid::print_grid");
#endif
    p->p_view->print_grid();
}


void    hk_kdegrid::widget_specific_backgroundcolour_changed(const hk_colour &)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::widget_specific_backgroundcolour_changed");
#endif
    if (presentation() == NULL)
        p->p_model->setBackgroundColor(KApplication::palette().color(QPalette::Base));
    else {
        hk_colour c = backgroundcolour();
        p->p_model->setBackgroundColor(QColor(c.red(), c.green(), c.blue()));
        if (p->statbar == NULL) return;
        QPalette pal = p->statbar->palette();
        QColor newcolour(c.red(), c.green(), c.blue());

        pal.setColor(p->statbar->backgroundRole(), newcolour);
        p->statbar->setPalette(pal);
    }
}


void    hk_kdegrid::widget_specific_foregroundcolour_changed(const hk_colour &)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::widget_specific_foregroundcolour_changed");
#endif
    if (presentation() == NULL)
        p->p_model->setForegroundColor(KApplication::palette().color(QPalette::Text));
    else {
        hk_colour c = foregroundcolour();
        p->p_model->setForegroundColor(QColor(c.red(), c.green(), c.blue()));
        if (p->statbar == NULL) return;
        QPalette pal = p->statbar->palette();
        QColor newcolour(c.red(), c.green(), c.blue());

        pal.setColor(p->statbar->foregroundRole(), newcolour);
        p->statbar->setPalette(pal);
    }
}


void hk_kdegrid::widget_specific_rowheight_changes(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::widget_specific_rowheight_changes");
#endif
    if (p->p_view) p->p_view->updateRowHeight();
}

bool hk_kdegrid::save_changed()
{
    if (datasource() != NULL && datasource()->check_store_changed_data()) {
        datasource()->store_changed_data();
        return true;
    } else {
        datasource()->reset_changed_data();
        return false;
    }
}

void  hk_kdegrid::savedata(ostream& s,bool saveall)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::savedata");
#endif    
    if (p->columnmergeneeded)
        p->mergeColumnOrder();    
    hk_dsgrid::savedata(s,saveall);
}

void hk_kdegrid::show_widget(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::show_widget");
#endif
    show();
}

void hk_kdegrid::hide_widget(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::hide_widget");
#endif
    hide();
}

bool hk_kdegrid::close_widget(void)
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::close_widget");
#endif

    return close();
}


bool hk_kdegrid::presentationmode_changed()
{
#ifdef HK_DEBUG
    hkdebug("kdegrid::presentationmode_changed");
#endif
    if (!p_presentation) {
        setEnabled(is_enabled());
        return hk_dsgrid::presentationmode_changed();
    }
    bool result = hk_dsgrid::presentationmode_changed();
    if (p_presentation->mode() == hk_presentation::filtermode) {
        setEnabled(false);
    } else
        setEnabled(is_enabled());
    if (p->p_view) p->p_view->setEnabled(p_presentation->mode() == hk_presentation::viewmode);
    if (p->selector) p->selector->setEnabled(p_presentation->mode() == hk_presentation::viewmode);
    return  result;
}
