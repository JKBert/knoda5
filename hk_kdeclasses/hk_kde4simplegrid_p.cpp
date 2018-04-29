// ****************************************************************************
// Copyright (c) 2010-2016 Patrik Hanak <hanakp@users.sourceforge.net>
// This file is part of the hk_kde4classes library.
//
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#include <qpainter.h>
#include <QKeyEvent>
#include <QStandardPaths>
#include <qheaderview.h>
#include <kapplication.h>
#include <kiconloader.h>
#include <KIconEngine>
#include <hk_datasource.h>

#include "hk_kde4simplegrid.h"
#include "hk_kde4simplegrid_p.h"
#include "hk_kdelineedit.h"
#include "hk_kdeboolean.h"
#include "hk_kdememo.h"
#include "hk_kdegrid.h"
#include "hk_kdecombobox.h"
#include "hk_database.h"

int const hk_kdegridmodel::ColumnTypeRole = Qt::UserRole;
int const hk_kdegridmodel::PasteRole = Qt::UserRole + 1;

hk_kdegridmodel::hk_kdegridmodel(hk_kdegrid *parent): QAbstractTableModel(parent),
    p_grid(parent), p_datasource(NULL), p_whilesorting(false),
    headerbgcolor(QBrush(KApplication::palette().color(QPalette::Button))),
    headerfgcolor(QBrush(KApplication::palette().color(QPalette::ButtonText))),
    cellbgcolor(KApplication::palette().color(QPalette::Base)),
    cellfgcolor(KApplication::palette().color(QPalette::Text))
{
    Q_ASSERT(p_grid);
    columnlabelcache = p_grid->gridcolumn(0) != NULL ? columnlabel(0) : " ";
    KIconLoader loader ("hk_kde5classes");
    
    arrow_image.load(loader.iconPath("gridcursor", KIconLoader::User));
    no_image = QPixmap(arrow_image.width(), arrow_image.height());
    no_image.fill(Qt::transparent);
}

void hk_kdegridmodel::set_datasource(hk_datasource *ds)
{
    beginResetModel();
    p_datasource = ds;
    endResetModel();
}

void hk_kdegridmodel::grid_reset(void)
{
    Q_ASSERT(p_grid);
    if (p_whilesorting)
        return;
    if (p_grid->gridcolumn(0) != NULL)
        columnlabelcache = columnlabel(0);
    beginResetModel();
    endResetModel();
}

int hk_kdegridmodel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : (p_datasource != NULL && p_datasource->is_enabled()
                                   && (p_grid->presentation() == NULL
                                       || p_grid->presentation()->mode() != hk_presentation::designmode) ?
                                   p_datasource->max_rows() + (p_datasource->is_readonly() || p_grid->is_readonly() ? 0 : 1) : 1);

}

int hk_kdegridmodel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : (!p_grid->is_enabled()
                                   || p_datasource == NULL || !p_datasource->is_enabled()
                                   || (p_grid->presentation() != NULL && p_grid->presentation()->mode() == hk_presentation::designmode) ?
                                   1 : p_grid->columnscount());
}

QVariant hk_kdegridmodel::stringDataPart(int row, int col) const
{
    if (p_datasource != NULL && p_datasource->is_enabled() && p_grid->gridcolumn(col)->column() != NULL
            && (p_grid->presentation() == NULL || p_grid->presentation()->mode() != hk_presentation::designmode)) {
        bool isAtInsertRow = (unsigned int)row == p_datasource->max_rows();
        bool isAtDatasourceRow = (unsigned int)row == p_datasource->row_position();
        bool isCurrentRow =  p_datasource->mode() == hk_datasource::mode_insertrow ? isAtInsertRow
                             : isAtDatasourceRow;
        bool is_null = false;
        QString txt;
 
        if (p_grid->gridcolumn(col)->column()->has_changed() &&  isCurrentRow) {
            txt = QString::fromUtf8(l2u(
                                        p_grid->gridcolumn(col)->columntype() == hk_dsgridcolumn::columncombo ?
                                        p_grid->gridcolumn(col)->value_at(row)
                                        : p_grid->gridcolumn(col)->column()->changed_data_asstring(true)).c_str());
            is_null = p_grid->gridcolumn(col)->column()->changed_data_is_nullvalue();
        } else {
            if (isAtInsertRow) {

                if (p_grid->gridcolumn(col)->use_defaultvalue()) {
                    txt = p_grid->gridcolumn(col)->columntype() == hk_dsgridcolumn::columncombo ? ""
                          : QString::fromUtf8(l2u(p_grid->gridcolumn(col)->defaultvalue()).c_str());
                    is_null = txt.isEmpty();
                }

            } else {
                is_null = p_grid->gridcolumn(col)->column()->is_nullvalue_at(row);
                if (!is_null)
                    txt = QString::fromUtf8(l2u(p_grid->gridcolumn(col)->columntype() == hk_dsgridcolumn::columncombo 
                                                ? p_grid->combo_value_at(col, row)
                                                : p_grid->gridcolumn(col)->column()->asstring_at(row, true)).c_str());
            }
        }
        if (p_grid->gridcolumn(col)->columntype() != hk_dsgridcolumn::columncombo)
            if ((!is_null && is_numerictype(p_grid->gridcolumn(col)->column())
                    && p_grid->gridcolumn(col)->column()->columntype() != hk_column::auto_inccolumn && txt.length() > 0)
                    || (p_grid->gridcolumn(col)->column()->columntype() == hk_column::auto_inccolumn
                        && !isAtInsertRow))
                txt = QString::fromUtf8(format_number(u2l(txt.toUtf8().data()), true, p_grid->gridcolumn(col)->use_numberseparator(),
                                    p_grid->gridcolumn(col)->commadigits(), hk_class::locale()).c_str());
       return QVariant(txt);
    }
    return QVariant(QVariant::Invalid);
}

QVariant hk_kdegridmodel::boolDataPart(int row, int col) const
{
    if (p_datasource != NULL && p_datasource->is_enabled() && p_grid->gridcolumn(col)->column() != NULL
            && (p_grid->presentation() == NULL || p_grid->presentation()->mode() != hk_presentation::designmode)
            && p_grid->gridcolumn(col) != NULL && p_grid->gridcolumn(col)->columntype() == hk_dsgridcolumn::columnbool) {
        bool checked = false;
        bool is_null = false;

        if (p_grid->gridcolumn(col)->column()->has_changed()
                && (((unsigned int)row == p_datasource->row_position()
                     && p_datasource->mode() != hk_datasource::mode_insertrow)
                    || ((unsigned int)row == p_datasource->max_rows()
                        && p_datasource->mode() == hk_datasource::mode_insertrow))) {
            //changed data, current row
            checked = p_grid->gridcolumn(col)->column()->changed_data_asbool();
            is_null = p_grid->gridcolumn(col)->column()->changed_data_is_nullvalue();
        } else if ((unsigned)row == p_datasource->max_rows()) {
            // unchanged data, insert row
            if (p_grid->gridcolumn(col)->use_defaultvalue()) {
                hk_string txt = p_grid->gridcolumn(col)->defaultvalue();

                is_null = txt.empty();
                checked = !is_null && txt != "FALSE";
            }
        } else {
            // unchanged data, standard row
            checked = p_grid->gridcolumn(col)->column()->asbool_at(row);
            is_null = p_grid->gridcolumn(col)->column()->is_nullvalue_at(row);
        }
        return QVariant(is_null ? Qt::PartiallyChecked :
                        checked ? Qt::Checked : Qt::Unchecked);
    }
    return QVariant(QVariant::Invalid);
}

QVariant hk_kdegridmodel::data(const QModelIndex &index, int role) const
{
    int col = index.column();
    int row = index.row();

    Q_ASSERT(p_grid);
    switch (role) {
    case Qt::BackgroundRole: {
        if (row % 2 == 0)  return cellbgcolor;
        break;
    }
    case Qt::ForegroundRole: {
        return cellfgcolor;
    }
    case Qt::TextAlignmentRole: {
        Qt::AlignmentFlag f = Qt::AlignLeft;
        if (p_grid != NULL && p_grid->gridcolumn(col) != NULL)
            switch (p_grid->gridcolumn(col)->alignment()) {
            case hk_dsdatavisible::alignright:
                f = Qt::AlignRight;
                break;
            case hk_dsdatavisible::aligncenter:
                f = Qt::AlignHCenter;
                break;
            default:
                f = Qt::AlignLeft;
            }
        return QVariant(f);
    }
    case Qt::CheckStateRole:
        return boolDataPart(row, col);
    case Qt::DisplayRole:
    case Qt::EditRole:
        return stringDataPart(row, col);
    case hk_kdegridmodel::ColumnTypeRole: {
        if (p_grid->gridcolumn(col) == NULL)
            return QVariant(QVariant::Invalid);
        switch (p_grid->gridcolumn(col)->columntype()) {
        case hk_dsgridcolumn::columnbool :
            return QVariant(columnbool);
        case hk_dsgridcolumn::columncombo:
            return QVariant(columncombo);
        default:
            if (p_grid->gridcolumn(col)->column() != NULL &&
                    p_grid->gridcolumn(col)->column()->columntype() == hk_column::memocolumn)
                return QVariant(columnmemo);
            else
                return QVariant(columnedit);
        }

    }
    }
    return  QVariant(QVariant::Invalid);
}

bool hk_kdegridmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole) {
        p_grid->gridcolumn(index.column())->column()->set_asbool(static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked);
        emit dataChanged(index, index);
        return true;
    }
    if (role == hk_kdegridmodel::PasteRole) {
        int col = index.column();

        // we ignore row id, we always update current row
        if (col >= 0 && ((uint) col) < p_grid->columnscount()
                && p_grid->gridcolumn(col)->column() != NULL) {
            hk_string textvalue = u2l(value.toString().toUtf8().data());

            if (textvalue.empty())
                p_grid->gridcolumn(col)->column()->set_asnullvalue();
            else {
                // we need to find correct listcolumn value based on viewcolumn value
                if (data(index, ColumnTypeRole).toInt() == columncombo) {
                    hk_dscombobox *cx = p_grid->gridcolumn(col)->combobox();
                    uint pos = cx->viewcolumn()->find(textvalue, true, true);

                    textvalue = cx->listdatasource()->column_by_name(cx->listcolumnname())->asstring_at(pos);
                }
                p_grid->gridcolumn(col)->column()->set_asstring(textvalue);
            }
            emit dataChanged(index, index);
            return true;
        }
        return false;
    }
    return QAbstractTableModel::setData(index, value, role);
}

QMap<int, QVariant>  hk_kdegridmodel::itemData(const QModelIndex &index) const
{
    QMap<int, QVariant> m = QAbstractTableModel::itemData(index);

    m[hk_kdegridmodel::ColumnTypeRole] = data(index, hk_kdegridmodel::ColumnTypeRole);
    m[hk_kdegridmodel::PasteRole] = data(index, hk_kdegridmodel::PasteRole);
    return m;
}

QVariant hk_kdegridmodel::headerData(int section, Qt::Orientation orient, int role) const
{
    switch (role) {
    case Qt::BackgroundRole:
        return headerbgcolor;
    case Qt::ForegroundRole:
        return headerfgcolor;
    }
    if (orient == Qt::Horizontal && role == Qt::DisplayRole && p_grid->is_enabled()) {
        return QVariant(p_grid->gridcolumn(section) != NULL ? columnlabel(section) : columnlabelcache);
    }
    if (orient == Qt::Vertical && role == Qt::DisplayRole &&
            (p_datasource == NULL || !p_datasource-> is_enabled() || (((uint)section + 1) > p_datasource->max_rows())
             || (p_grid->presentation() != NULL && p_grid->presentation()->mode() == hk_presentation::designmode)))
        return QVariant("*");
    if (orient == Qt::Vertical && role == Qt::DecorationRole) {
        int currow = (p_datasource != NULL && p_datasource->is_enabled() ?
                      (p_datasource->mode() == hk_datasource::mode_insertrow ? p_datasource->max_rows() : p_datasource->row_position())
                      : 0);
        return QVariant(section == currow ? arrow_image : no_image);
    }
    return QAbstractItemModel::headerData(section, orient, role);
}

Qt::ItemFlags hk_kdegridmodel::flags(const QModelIndex &index) const
{
    unsigned int col = index.column();
    bool gridcol_available = p_grid->gridcolumn(col) != NULL;

    return QAbstractTableModel::flags(index) | (p_datasource == NULL || p_datasource->is_readonly()
            || !gridcol_available || p_grid->is_readonly() || p_grid->gridcolumn(col)->is_readonly()
            || p_grid->gridcolumn(col)->column() == NULL
            || p_grid->gridcolumn(col)->column()->is_readonly() ? Qt::NoItemFlags : Qt::ItemIsEditable | Qt::ItemIsDropEnabled)
           | (gridcol_available && p_grid->gridcolumn(col)->columntype() == hk_dsgridcolumn::columnbool ?
              Qt::ItemIsUserCheckable | Qt::ItemIsTristate : Qt::NoItemFlags);
}

void hk_kdegridmodel::sort(int col, Qt::SortOrder order)
{
    if (p_grid->gridcolumn(col)) {
        hk_string colname = defaultidentifierdelimiter + p_grid->gridcolumn(col)->columnname() + defaultidentifierdelimiter;
        if (order == Qt::DescendingOrder) colname += " DESC ";
        p_datasource->set_temporarysorting(colname);
    }
    emit layoutAboutToBeChanged();
    p_whilesorting = true;
    p_datasource->disable();
    p_datasource->set_use_temporarysorting(true);
    p_datasource->enable();
    p_whilesorting = false;
    emit layoutChanged();
}

QString hk_kdegridmodel::columnlabel(int col) const
{
    return p_grid->gridcolumn(col)->displayname().length() == 0 ?
           QString::fromUtf8(p_grid->gridcolumn(col)->displayname().size() == 0 ?
                             l2u(p_grid->gridcolumn(col)->columnname()).c_str() :
                             l2u(p_grid->gridcolumn(col)->displayname()).c_str())
               : QString::fromUtf8(l2u(p_grid->gridcolumn(col)->displayname()).c_str());
}

void hk_kdegridmodel::currentRowChanged(unsigned long prevrow)
{
    ulong currow = (p_datasource != NULL && p_datasource->is_enabled()) ?
                   (p_datasource->mode() == hk_datasource::mode_insertrow ? rowCount() - 1 : p_datasource->row_position())
                   : 0;

    emit headerDataChanged(Qt::Vertical, prevrow, prevrow);
    emit headerDataChanged(Qt::Vertical, currow, currow);
}


SimpleGridDelegate::SimpleGridDelegate(QObject *parent, hk_kdegrid *parentgrid): QStyledItemDelegate(parent), grid(parentgrid)
{
    Q_ASSERT(parentgrid != NULL);
}

LineEditDelegate::LineEditDelegate(QObject *parent, hk_kdegrid *parentgrid): SimpleGridDelegate(parent, parentgrid)
{
}

QWidget *LineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    int col = index.column();
    hk_kdelineedit *l = new hk_kdelineedit(parent);
    const QAbstractItemModel *model = index.model();

    l->setFrame(false);
    if (grid->gridcolumn(col)) {
        l->set_alignment(grid->gridcolumn(col)->alignment());
        l->set_numberformat(grid->gridcolumn(col)->use_numberseparator(), grid->gridcolumn(col)->commadigits());
        l->set_datasource(grid->datasource());
        l->set_presentationdatasource(grid->presentationdatasource());
        l->set_defaultvalue(grid->gridcolumn(col)->defaultvalue());
        l->set_columnname(grid->gridcolumn(col)->columnname());
        l->set_font(grid->hk_dsgrid::font());
        l->set_readonly((model->flags(index) & Qt::ItemIsEditable) == 0
                        || grid->gridcolumn(col)->is_readonly());
        l->selectAll();
        l->set_gridcolumn(grid->gridcolumn(col));
    }
    return l;
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}


ComboBoxDelegate::ComboBoxDelegate(QObject *parent, hk_kdegrid *parentgrid): SimpleGridDelegate(parent, parentgrid)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &index) const
{
    int col = index.column();
    hk_kdecombobox *combo = new hk_kdecombobox(parent, dynamic_cast<hk_form *>(grid->presentation()));
    hk_dsgridcolumn *c = grid->gridcolumn(col);

    if (c) {
        combo->set_columnname(c->columnname());
        combo->set_viewcolumnname(c->viewcolumnname());
        combo->set_listcolumnname(c->listcolumnname());
        combo->set_alignment(grid->gridcolumn(col)->alignment());
        combo->set_numberformat(grid->gridcolumn(col)->use_numberseparator(), grid->gridcolumn(col)->commadigits());
        combo->set_defaultvalue(grid->gridcolumn(col)->defaultvalue());
        combo->set_columnname(grid->gridcolumn(col)->columnname());
        combo->set_readonly(((index.flags() & Qt::ItemIsEditable) == 0) || grid->gridcolumn(col)->is_readonly()
                            || grid->is_readonly());
        combo->set_font(grid->hk_dsgrid::font());
        combo->set_gridcolumn(grid->gridcolumn(col));
    }

    if (grid->presentation())
        combo->set_listdatasource(grid->presentation()->get_datasource(c->listdatasourcename()));
    else {
        if (grid->datasource()) {
            hk_database *db = grid->datasource()->database();
            hk_datasource *ds = db->load_datasource(c->listdatasourcename(), c->listdatasourcetype());
            combo->set_listdatasource(ds);
            if (ds) ds->enable();
        }
    }
    if (grid->presentation())
        combo->set_presentationdatasource(grid->presentationdatasource());
    else
        combo->set_datasource(grid->datasource());
    if (combo->lineEdit()) combo->lineEdit()->selectAll();

    return combo;
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, 
                                            const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}



CheckBoxDelegate::CheckBoxDelegate(QObject *parent, hk_kdegrid *parentgrid): SimpleGridDelegate(parent, parentgrid)
{
}

bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, 
                                   const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_ASSERT(event);
    Q_ASSERT(model);
    Qt::ItemFlags flags = model->flags(index);
    QVariant value = index.data(Qt::CheckStateRole);

    if (!(flags & Qt::ItemIsEditable) || !(option.state & QStyle::State_Enabled)
            || !(flags & Qt::ItemIsEnabled))
        return false;

    if (!value.isValid())
        return false;

    const QWidget *w = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option) ?
                       qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option)->widget : NULL;
    QStyle *style = w ? w->style() : QApplication::style();

    if ((event->type() == QEvent::MouseButtonRelease)
            || (event->type() == QEvent::MouseButtonDblClick)
            || (event->type() == QEvent::MouseButtonPress)) {
        QStyleOptionViewItemV4 itemOption(option);
        initStyleOption(&itemOption, index);
        QRect checkRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &itemOption, w);
        QMouseEvent *ev = static_cast<QMouseEvent *>(event);

        if (event->type() == QEvent::MouseButtonDblClick)
            return true;

        checkRect.moveLeft(checkRect.left() + (option.rect.width() - checkRect.width()) / 2);

        if (ev->button() != Qt::LeftButton || !checkRect.contains(ev->pos())
                || (event->type() == QEvent::MouseButtonPress))
            return false;


    } else if (event->type() == QEvent::KeyPress) {
        if (static_cast<QKeyEvent *>(event)->key() != Qt::Key_Space
                && static_cast<QKeyEvent *>(event)->key() != Qt::Key_Select)
            return true;
    } else {
        return false;
    }

    Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked
                            ? Qt::Unchecked : Qt::Checked);
    return model->setData(index, state, Qt::CheckStateRole);
}

void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, 
                                            const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

void CheckBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 itemOption = option;
    QStyleOptionButton buttonOption;
    const QWidget *w = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option) ?
                       qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option)->widget : NULL;
    QStyle *style = w ? w->style() : QApplication::style();
    QRect checkRect = style->subElementRect(QStyle::SE_CheckBoxIndicator, &option, w);

    initStyleOption(&itemOption, index);
    // center the checkbox in the cell
    checkRect.moveLeft(checkRect.left() + (option.rect.width() - checkRect.width()) / 2);
    buttonOption.rect = checkRect;
    buttonOption.state = (itemOption.checkState == Qt::Checked ? QStyle::State_On :
                          itemOption.checkState == Qt::Unchecked ? QStyle::State_Off : QStyle::State_NoChange);
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &itemOption, painter);
    style->drawPrimitive(QStyle::PE_IndicatorCheckBox, &buttonOption, painter);
}


MemoDelegate::MemoDelegate(QObject *parent, hk_kdegrid *parentgrid): SimpleGridDelegate(parent, parentgrid)
{
}

QWidget *MemoDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, 
                                    const QModelIndex &index) const
{
    int col = index.column();
    hk_kdememo *l = new hk_kdememo(parent);
    const QAbstractItemModel *model = index.model();

    if (grid->gridcolumn(col)) {
        l->set_alignment(grid->gridcolumn(col)->alignment(), false);
        l->set_numberformat(grid->gridcolumn(col)->use_numberseparator(), 
                            grid->gridcolumn(col)->commadigits(), false);
        l->set_datasource(grid->datasource());
        l->set_presentationdatasource(grid->presentationdatasource(), false);
        l->set_defaultvalue(grid->gridcolumn(col)->defaultvalue(), false);
        l->set_columnname(grid->gridcolumn(col)->columnname(), false);
        l->set_font(grid->hk_dsgrid::font(), false);
        l->set_readonly((model->flags(index) & Qt::ItemIsEditable) == 0
                        || grid->gridcolumn(col)->is_readonly());
        l->selectAll();
        l->set_gridcolumn(grid->gridcolumn(col));
    }
    return l;
}

void MemoDelegate::setEditorData(QWidget * /*editor*/, const QModelIndex & /*index*/) const
{
    // empty body, cell data loaded directly from datasource to hk_kdememo
}

void MemoDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                        const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}
