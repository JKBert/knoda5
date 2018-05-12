// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>  
// This file is part of the hk_kdeclasses library.
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef HK_KDEPASSWORD
#define HK_KDEPASSWORD
#include <qwidget.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <hk_definitions.h>
class dialogedit;
class button;
class combobox;
class hk_connection;
/**
 *
 *@version $Revision: 1.10 $
 *@author Horst Knorr (hk_classes@knoda.org)
 *@internal

 */

class hk_kdepassworddialog:public QDialog
{
    Q_OBJECT
        public:
        hk_kdepassworddialog(hk_connection*,QWidget* p=0,const char* name=0,struct_connectionfields*fields=0);
        ~hk_kdepassworddialog();

    protected:
        QLabel *hostlabel;
        dialogedit *hostedit;
        QLabel *userlabel;
        dialogedit *useredit;
        QLabel *passwordlabel;
        dialogedit *passwordedit;
        QLabel *portlabel;
        dialogedit *portedit;
        QLabel *sqldelimiterlabel;
        dialogedit *sqldelimiteredit;
        button *ok_button;
        button *cancel_button;
        QLabel  *header;
        QLabel   *booleanemulationlabel;
        QComboBox* booleanemulationedit;
        QLabel*   databasenamelabel;
        combobox* databasenameedit;
        QCheckBox* savesettings;
        QLabel *savesettingslabel;

    private:
        struct_connectionfields* p_fields;
        hk_connection* p_connection;
        QVBoxLayout* gesamtlayout;
        QVBoxLayout* textlayout;
        QVBoxLayout* fieldlayout;
        QHBoxLayout* datalayout;
        QHBoxLayout* buttonlayout;
        QHBoxLayout* headerlayout;
    private slots:
        void ok_buttonclicked(void);
        void cancel_buttonclicked(void);
};
#endif
