// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for knoda -Knorr's Datenbank.
// copyright (c) 2010-2014 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for knoda4 -Knorr's Datenbank.
//
// This file is part of the knoda4 -Knorr's Datenbank.
// This file may be distributed and/or modified under the terms of the
// GNU General Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_KDEDRIVERSELECT_H
#define HK_KDEDRIVERSELECT_H
#include "hk_kdedriverselectbase.h"
#include <kfiledialog.h>

class internalfiledialog: public KFileDialog
{
Q_OBJECT
 public:
     internalfiledialog(const QUrl& startDir, const QString& filter,
		QWidget *parent);
protected:
virtual void accept();
virtual void reject();
virtual void keyPressEvent(QKeyEvent*);
};

class hk_kdedriverselect : public hk_kdedriverselectbase
{
    Q_OBJECT

public:
    hk_kdedriverselect( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdedriverselect();
     bool driver_selected(){return p_driverselected;}
     QString selected_file(){return p_selectedfile;}
     QString currentDriver();
public slots:
    virtual void list_clicked();
    virtual void connection_clicked();
    virtual void tabwidget_changed();
    virtual void file_selected(const QUrl&);
    virtual void help_clicked(void);
    virtual void option_clicked();

protected:
void accept(void);
bool eventFilter(QObject*,QEvent*);
virtual void keyPressEvent ( QKeyEvent * e );
    QVBoxLayout* tab2Layout;
internalfiledialog * p_filedialog;
bool p_driverselected;
QString p_selectedfile;
};

#endif // HK_KDEDRIVERSELECT_H
