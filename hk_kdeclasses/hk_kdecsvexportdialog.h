// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2013 Patrik Hanak <hanakp@users.sourceforge.net>
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

#ifndef HK_KDECSVexportDIALOG_H
#define HK_KDECSVexportDIALOG_H
#include "hk_kdecsvexportdialogbase.h"
#include <qvariant.h>
#include <qdialog.h>
#include <hk_dsvisible.h>
#include <hk_datasource.h>
#include <hk_reportutils.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QToolButton;
class QProgressDialog;
/**
 *
 *@short KDE frontend dialog to export data in CSV format
 *@version $Revision: 1.17 $
 *@author Horst Knorr (hk_classes@knoda.org)
 */
class hk_kdecsvexportdialog : public hk_kdecsvexportdialogbase  ,public hk_reportcsv
{
    Q_OBJECT

  public:
    hk_kdecsvexportdialog(const QString& table, QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
    ~hk_kdecsvexportdialog();

    void set_usetablelist(void);
    void set_usequerylist(void);
    void set_useviewlist(void);
    void set_database(hk_database* d);

  public slots:
    virtual void help_clicked();
    virtual void listtype_changed();
    virtual void morebutton_clicked();
    
    protected slots:
        virtual void filebutton_clicked();
        virtual void buttons_enabled();
        virtual void ok_clicked(void);
        void printing_cancelled(void);

  protected:
    virtual void keyPressEvent ( QKeyEvent * e );
    static  bool set_progress(long int,long int,const hk_string&);

  private:

    static bool p_cancelimport;
    static  QProgressDialog* p_progressdialog;
        void set_datasourcelist(void);
        QString      p_file;
        QString   p_tablename;
        QString      p_tab;
	datasourcetype p_datasourcetype;
};
#endif                                            // HK_KDECSVexportDIALOG_H
