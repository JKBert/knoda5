// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2013 Patrik Hanak <hanakp@users.sourceforge.net>
// KDE 4 port of the file for hk_kde4classes library
//
// This file is part of the hk_kde4classes library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file COPYING included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************
#ifndef HK_KDEQBE
#define HK_KDEQBE

#include <kde4classesmacros.h>
#include <kparts/mainwindow.h>
#include <hk_qbe.h>

class hk_kdeqbeprivate;

class KDECLASSES_EXPORT hk_kdeqbe:public KParts::MainWindow,public hk_qbe
{
Q_OBJECT
public:
        hk_kdeqbe(QWidget* w=0,const char* n=0,Qt::WFlags f=0);
	~hk_kdeqbe();
        virtual void set_datasource(hk_datasource*);
	void set_database(hk_database*);
	virtual void loaddata(xmlNodePtr definition);

public slots:
void add_datasource(void);
void value_changed(int row, int col);
void datasourcedefiniton_changed(void);
void set_columnvalues(void);
void slot_has_changed(void);
void add_column(int pos, int table, const hk_string& columnname);
void add_column(int table, const hk_string& columnname);
void distinct_changed(void);
signals:
void signal_qbe_has_changed(void);
void signal_qbetype_has_changed(void);
void signal_distinct_has_changed(void);
protected:
virtual void set_has_changed(void);
virtual void widget_specific_querytype_has_changed(enum_querytype oldtype,enum_querytype newtype);

private:
hk_kdeqbeprivate* p_private;
void set_tablenames(void);
void set_tablenamelist(void);
void set_columnnames(int col);
void init_column(int col);
void init_tablenames(int col);
};






#endif //HK_KDEQBE
