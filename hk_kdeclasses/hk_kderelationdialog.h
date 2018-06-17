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
#ifndef HK_KDERELATIONDIALOG_H
#define HK_KDERELATIONDIALOG_H
#include "hk_kderelationdialogbase.h"
#include <hk_column.h>
#include <list>

class hk_kdedatasourceframe;
class hk_kdedbdesigner;
class hk_kdedbrelation;
class QTableWidgetItem;
class QSignalMapper;


class hk_kderelationdialog : public hk_kderelationdialogbase
{
    Q_OBJECT
public:
    hk_kderelationdialog( hk_kdedatasourceframe* master,hk_kdedatasourceframe* slave, 
    hk_kdedbdesigner* parent,hk_kdedbrelation* r=NULL , const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kderelationdialog();
    int masterindex(const QString&);
    int slaveindex(const QString&);
    bool set_relation(void);
    void set_referentialintegrity(void);
public slots:
    virtual void ok_button_clicked();
    virtual void slot_data_changed(QWidget*);
    void add_row(void);
protected:
    void keyPressEvent(QKeyEvent * );
    bool eventFilter(QObject* object,QEvent* event);
private:
    void set_stringlist(QStringList* strlist,list<hk_string>*);
    void set_existing_fields(void);
    void set_existing_refintegrityfields(void);
    int    textposition(QStringList* ,const hk_string&  );
    void delete_rows(void);
    hk_kdedatasourceframe* p_masterdsframe;
    hk_kdedatasourceframe* p_slavedsframe;
    QStringList		p_mastercollist;
    QStringList		p_slavecollist;
    hk_kdedbdesigner*     	p_designer;
    hk_kdedbrelation*	p_relation;
    QSignalMapper*          p_signalmerge;
};

#endif // HK_KDERELATIONDIALOG_H
