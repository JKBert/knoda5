// ****************************************************************************
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
// Original version of the file for hk_kde4classes library
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

#ifndef HK_KDESIMPLEGRID
#define HK_KDESIMPLEGRID
#include <kde4classesmacros.h>
#include <qtableview.h>

class hk_kdesimpleform;
class hk_kdegrid;
class hk_kdegridpart;
class hk_kde4simplegridprivate;

class KDECLASSES_EXPORT hk_kde4simplegrid : public QTableView
{
    Q_OBJECT
    friend class hk_kdegrid;
    friend class p_private;

public:
    hk_kde4simplegrid(hk_kdegrid *grid, hk_kdesimpleform *form);
    virtual  ~hk_kde4simplegrid(void);
    virtual void 	setModel(QAbstractItemModel *model);
    int currentColumn(void);
    inline hk_kdegrid *kdegrid() const {
        return p_grid;
    };
    bool canProcessClipboard() const;
    bool isParentWidgetOf(QWidget* widget);

    // Preferred: text/tab-separated-values
    // Default: text/plain
    static const QString mimePreferredFormat;
    static const QString mimeDefaultFormat;

signals:
    void signal_infocus();
    void signal_outfocus();
    void focusedCellChanged(int prevRow, int prevCol, int currRow, int currCol);

public slots:
    void columns_resize(void);
    void print_grid(void);
    void copy(void);
    void paste(void);
    void cut(void);
    virtual void reset();
    void modelLayoutChanged();

protected:
    void updateRowHeight(void);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);
    void focusInEvent ( QFocusEvent * );
    void focusOutEvent ( QFocusEvent * );
    bool focusNextPrevChild(bool next);
    bool moveNextPrevCell(QAbstractItemView::CursorAction direction);
    QString marked_text(void);
    void paste_tsv(const QString &src);
    void currentChanged(const QModelIndex & curr, const QModelIndex & prev );

protected slots:
    void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);
    void focusChanged(QWidget* oldWidget, QWidget* newWidget);
    void rowHeightChanged(int Index, int oldheight, int newheight);
    void columnWidthChanged(int Index, int oldwidth, int newwidth);
    void rowHeaderPressed(int Index);
    void show_contextmenu(const QPoint &pos);

private:
    hk_kdegrid *p_grid;
    hk_kde4simplegridprivate *p_private;
};

#endif
