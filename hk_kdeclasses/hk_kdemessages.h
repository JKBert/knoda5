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
//$Revision: 1.17 $

#ifndef HK_KDEMESSAGES
#define HK_KDEMESSAGES

#include <kde4classesmacros.h>
#include <hk_definitions.h>
#include <hk_connection.h>
#include <qdialog.h>

/**
 * displays the text w.
 */
void hk_kdewarning(const hk_string& w);
/**
 * dialog with a "yes" and a "no" button.
 */
bool hk_kdeyesno_dialog(const hk_string& w,bool default_return);
/**
 * dialog which asks for host,user,password and sqlserver if necessary
 */
void hk_kdepassworddlg(hk_connection*,struct_connectionfields* c);

hk_string hk_kdetranslate(const hk_string& t);

hk_string hk_kdedriverselectdialog(void);

hk_string hk_kdestringvaluedialog(const hk_string&);

hk_string hk_kdedirectorydialog(const hk_string&);

hk_string hk_kdefiledialog(const hk_string&,enum_operationmode);

void hk_kdedatasourceenablefunction(void);

void KDECLASSES_EXPORT set_kdestandarddialogs(void);

hk_string KDECLASSES_EXPORT mimetype2driver(const hk_string& mimetype);

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

class hk_kdetextdialog : public QDialog
{
    Q_OBJECT

public:
    hk_kdetextdialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
    ~hk_kdetextdialog();

    QLabel* textlabel;
    QLineEdit* textfield;
    QPushButton* buttonOk;
    QPushButton* buttonCancel;

protected:
    QVBoxLayout* hk_kdetextdialogLayout;
    QHBoxLayout* Layout1;

protected slots:
    virtual void languageChange();
    virtual void text_changed(const QString&);
};

#endif
