// ****************************************************************************
// Copyright (c) 2010-2014 Patrik Hanak <hanakp@users.sourceforge.net>
// This file is part of the hk_kde4classes library.
//
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef SIMPLEGRIDIMPL
#define SIMPLEGRIDIMPL

#include <QAbstractTableModel>
#include <qstyleditemdelegate.h>

class QModelIndex;
class QWidget;
class QVariant;
class hk_kdegrid;
class hk_datasource;

class hk_kdegridmodel: public QAbstractTableModel
{
    Q_OBJECT
    friend class hk_kdegrid;
public:

    enum columnedittype { columnedit, columnbool, columncombo, columnmemo };

    hk_kdegridmodel(hk_kdegrid *parent = 0);
    void set_datasource(hk_datasource *ds);
    void grid_reset();
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QMap<int, QVariant> itemData(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orient,  int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual void sort(int col, Qt::SortOrder order = Qt::AscendingOrder);
    void inline setBackgroundColor(const QColor &col) {
        cellbgcolor = col;
    }
    void inline setForegroundColor(const QColor &col) {
        cellfgcolor = col;
    }
    void currentRowChanged(unsigned long prevrow);

    static int const ColumnTypeRole;
    static int const PasteRole;

protected:

    QPixmap arrow_image;
    QPixmap no_image;
    
    hk_kdegrid *p_grid;
    hk_datasource *p_datasource;
    bool p_whilesorting;
    QString columnlabelcache;
    QVariant headerbgcolor;
    QVariant headerfgcolor;
    QVariant cellbgcolor;
    QVariant cellfgcolor;
    
    QVariant stringDataPart(int row, int col) const;
    QVariant boolDataPart(int row, int col) const;
    QString columnlabel(int col) const;    
    
};

class SimpleGridDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SimpleGridDelegate(QObject *parent = 0, hk_kdegrid *parentgrid = 0);

protected:
    hk_kdegrid *grid;
};

class LineEditDelegate : public SimpleGridDelegate
{
    Q_OBJECT
public:
    LineEditDelegate(QObject *parent = 0, hk_kdegrid *parentgrid = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class ComboBoxDelegate : public SimpleGridDelegate
{
    Q_OBJECT
public:
    ComboBoxDelegate(QObject *parent = 0, hk_kdegrid *parentgrid = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class CheckBoxDelegate : public SimpleGridDelegate
{
    Q_OBJECT
public:
    CheckBoxDelegate(QObject *parent = 0, hk_kdegrid *parentgrid = 0);
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

class MemoDelegate : public SimpleGridDelegate
{
    Q_OBJECT
public:
    MemoDelegate(QObject *parent = 0, hk_kdegrid *parentgrid = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif
