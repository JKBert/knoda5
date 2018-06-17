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

#ifndef HK_KDEXMLEXPORTDIALOG_H
#define HK_KDEXMLEXPORTDIALOG_H
#include "hk_kdexmlexportdialogbase.h"
#include <hk_reportutils.h>
class QProgressDialog;

class hk_kdexmlexportdialog : public hk_kdexmlexportdialogbase ,public hk_reportxml
{
    Q_OBJECT

    public:
        hk_kdexmlexportdialog(const QString& table, QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
        ~hk_kdexmlexportdialog();
        void set_usetablelist(void);
        void set_usequerylist(void);
        void set_useviewlist(void);
        void set_database(hk_database* d);

    public slots:
        void ok_clicked();
    virtual void listtype_changed();
      virtual void help_clicked();

    protected slots:
        void buttons_enabled();
        void filebutton_clicked();
        void printing_cancelled(void);
        virtual void excel_clicked();

    protected:
     static  bool set_progress(long int,long int,const hk_string&);
    virtual void keyPressEvent ( QKeyEvent * e );

    private:
        void set_datasourcelist(void);
        QString      p_file;
        QString p_tablename;
	datasourcetype p_datasourcetype;
    static  QProgressDialog* p_progressdialog;
        static bool p_cancelimport;
};
#endif                                            // HK_KDEXMLEXPORTDIALOG_H
