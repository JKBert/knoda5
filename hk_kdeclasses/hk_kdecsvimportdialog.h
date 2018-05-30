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

#ifndef HK_KDECSVIMPORTDIALOG_H
#define HK_KDECSVIMPORTDIALOG_H

#include <qvariant.h>
#include <hk_kdecsvimportdialogbase.h>
#include <hk_dsvisible.h>
#include <hk_datasource.h>
#include <hk_importcsv.h>

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
 *@short KDE Widget frontend to import CSV data
 *@version $Revision: 1.15 $
 *@author Horst Knorr (hk_classes@knoda.org)
 */

class hk_kdecsvimportdialog : public hk_kdecsvimportdialogbase  ,public hk_importcsv
{
    Q_OBJECT

  public:
        hk_kdecsvimportdialog( QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0 );
        ~hk_kdecsvimportdialog();
        virtual void        set_datasource(hk_datasource* d);

  public slots:
      virtual void help_clicked();
      virtual void morebutton_clicked();

    protected slots:
        virtual void filebutton_clicked();
        virtual void buttons_enabled();
        virtual void ok_clicked(void);
        void printing_cancelled(void);

    protected:
        virtual bool widget_specific_after_new_row(void);
        void set_tablepreview(void);
virtual void keyPressEvent ( QKeyEvent * e );
 static  bool set_progress(long int,long int,const hk_string&);
    private:
        void before_execute(void);
        void after_execute(void);
        QString      p_file;
        QString      p_tab;
        static bool p_cancelimport;
    static        QProgressDialog* p_progressdialog;
};
#endif                                            // HK_KDECSVIMPORTDIALOG_H
