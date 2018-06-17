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

#ifndef HK_KDEDRIVERDIALOG_H
#define HK_KDEDRIVERDIALOG_H

#include <qwidget.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlabel.h>

#include <qdialog.h>
#include <hk_string.h>


class QTreeView;
class QListWidget;
/**
 *@short The database-driver select dialog
 *@author Horst Knorr <hk_classes@knoda.org>
 *@version $Revision: 1.11 $
 */
class hk_kdedriverdialog : public QDialog
{
    Q_OBJECT
        public:
        hk_kdedriverdialog(QWidget *parent=0, const char *name=0);
        ~hk_kdedriverdialog();
        hk_string drivername(void){return p_drivername;}
    protected slots:
        void exit_clicked(void);
        void ok_clicked(void);
        void directory_changed(void);
	void scroll_to_directory(void); 
    protected:
        QListWidget *driverlist;
        QPushButton *ok_button;
        QPushButton *exit_button;
        QLabel*    label;
        QTreeView* directorylist;
        hk_string  p_drivername;
        hk_string  p_originaldriverpath;
    virtual void showEvent(QShowEvent* );

};
#endif
