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

#ifndef HK_KDEINTERPRETERDIALOG_H
#define HK_KDEINTERPRETERDIALOG_H

#include <qvariant.h>
#include <kparts/mainwindow.h>
#include "hk_dsvisible.h"
#include "hk_datasource.h"
#include "hk_database.h"

class hk_kdeinterpreterdialogprivate;
/**
 *
 *@short KDE high level widget to define queries and see the result.
 *@version $Revision: 1.10 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *hk_kdeinterpreterdialog is a high level widget to define queries and see the result.
 *In this mode of development it has a field to enter the SQL query and a grid which shows the
 * resulting data if there is any. hk_kdeinterpreterdialog also handles actionqueries correctly.
 */

class uploadcodeiface
{
public:
    virtual void upload_text(const hk_string& code) const = 0;
    virtual const QString& get_action_text(void) const = 0;
protected:
    ~uploadcodeiface() {};
};

class hk_kdeinterpreterdialog : public KParts::MainWindow
{
    Q_OBJECT

public:
    hk_kdeinterpreterdialog(const uploadcodeiface& psh, QWidget* w = 0, const char* n = 0, Qt::WFlags f = 0);
    ~hk_kdeinterpreterdialog();
enum DialogCode { Rejected, Accepted };
/**
 *when the datasource vanishes (i.e. will be destroyed) and autoclose is true, the hk_kdeinterpreterdialog will be
 *closed otherwise it still is on your screen.
 */
    void set_autoclose(bool c){p_autoclose=c;}
    int exec(int rownumber,const hk_string& warningmessage);
    int result(void){return rescode;}
    hk_string code();
    void set_code(const hk_string&, bool has_changed=true);
    void hide();
    bool has_changed(void){return p_has_changed;}
    void set_caption(hk_visible*,const hk_string &action);
    
public slots:
    void print(void);
    virtual void help_clicked();
    void upload_clicked();

protected:
    virtual void closeEvent ( QCloseEvent* e);
    void keyPressEvent ( QKeyEvent * e );
    void setResult( int r )	{ rescode = r; }

protected slots:
    virtual void done(int);
    virtual void accept();
    virtual void reject();
    void slot_has_changed(void);
    void slot_undo_changed(void);

private:
    bool p_autoclose;
    bool p_has_changed;
    bool highlighting_set;
        //bool in_loop;
    int rescode;
    hk_kdeinterpreterdialogprivate* p_private;
};
#endif //HK_KDEINTERPRETERDIALOG_H
