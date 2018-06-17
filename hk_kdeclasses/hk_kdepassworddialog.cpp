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

#include <hk_kdepassworddialog.h>
#include <klocale.h>
#include <hk_kdepassworddialog.moc>
#include <hk_class.h>
#include <hk_connection.h>
#include <QKeyEvent>

class dialogedit:public QLineEdit
{
    public:
        dialogedit(QWidget* wid,const char*t=0): QLineEdit(wid)
        {
            setObjectName(t);
        }
    protected:
        void   keyPressEvent ( QKeyEvent * e );
};

void dialogedit::keyPressEvent( QKeyEvent * e )
{
    if (e->key()==Qt::Key_Return)
    {
        if (e->modifiers()&Qt::ControlModifier)
            focusNextPrevChild(false);
        else
            focusNextPrevChild(true);
     e->accept();

    }
    else QLineEdit::keyPressEvent(e);
}


class combobox:public QComboBox
{
    public:
        combobox(QWidget* wid): QComboBox(wid){}
    protected:
        void   keyPressEvent ( QKeyEvent * e );
};

void combobox::keyPressEvent( QKeyEvent * e )
{
    QComboBox::keyPressEvent(e);
    if (e->key()==Qt::Key_Return)
    {
        if (e->modifiers()&Qt::ControlModifier)
            focusNextPrevChild(false);
        else
            focusNextPrevChild(true);
     e->accept();
    }
}

class dialogbool:public QCheckBox
{
    public:
        dialogbool(QWidget* wid,const char*t=0): QCheckBox(wid)
        {
            setObjectName(QString::fromAscii(t));
        }
    protected:
        void   keyPressEvent ( QKeyEvent * e );
};

void dialogbool::keyPressEvent( QKeyEvent * e )
{
    if (e->key()==Qt::Key_Return)
    {
        if (e->modifiers()&Qt::ControlModifier)
            focusNextPrevChild(false);
        else
            focusNextPrevChild(true);
     e->accept();

    }
    else QCheckBox::keyPressEvent(e);
}

class  button:public QPushButton
{
    public:
        button(QWidget* wid=0,const char*t=0 ): QPushButton(wid){
            setObjectName(QString::fromAscii(t));
        }
    protected:
        void   keyPressEvent ( QKeyEvent * e );

};

void button::keyPressEvent( QKeyEvent * e )
{
    if (e->key()==Qt::Key_Return)
    {
        emit (clicked());
     e->accept();

    }
    else QPushButton::keyPressEvent(e);
}


hk_kdepassworddialog::hk_kdepassworddialog(hk_connection* connection,QWidget* p,const char* name,struct_connectionfields*fields)
:QDialog(p)
{
    if (name !=NULL)
        setObjectName(name);
    setModal(true);
    setMinimumSize( QSize( 300, 250 ) );
    QString c;
    if (connection) c=QString::fromUtf8(l2u(connection->drivername()).c_str())+" - ";
    c+=i18n("Password Dialog");
    setWindowTitle(c);
    p_connection=connection;
    bool focus_set=false;
    p_fields=fields;
    header = new QLabel(this);
    header->setObjectName("headerlabel");
    header->setText(i18n("Mandatory login details:"));
    gesamtlayout = new QVBoxLayout(this);
    headerlayout = new QHBoxLayout;
    textlayout = new QVBoxLayout;
    fieldlayout = new QVBoxLayout;
    datalayout = new QHBoxLayout;
    buttonlayout = new QHBoxLayout;
    gesamtlayout->setSpacing( 10 );
    gesamtlayout->setMargin( 20 );
    headerlayout->setSpacing( 6 );
    headerlayout->setMargin( 10);
    textlayout->setSpacing( 15 );
    textlayout->setMargin( 10 );
    fieldlayout->setSpacing( 15 );
    fieldlayout->setMargin( 10 );
    datalayout->setSpacing( 6 );
    datalayout->setMargin( 0 );
    buttonlayout->setSpacing( 6 );
    buttonlayout->setMargin( 0 );
    QSpacerItem* hspacer1=new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* vspacer1=new QSpacerItem(5, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QSpacerItem* vspacer2=new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    headerlayout->addWidget(header);
    gesamtlayout->addItem(vspacer1);
    gesamtlayout->addItem(headerlayout);
    gesamtlayout->addItem(datalayout);
    gesamtlayout->addItem(vspacer2);
    gesamtlayout->addItem(buttonlayout);

    datalayout->addItem(textlayout);
    datalayout->addItem(hspacer1);
    datalayout->addItem(fieldlayout);

    if (p_connection->server_needs(hk_connection::NEEDS_HOST))
    {
        hostlabel = new QLabel(this);
        hostlabel->setObjectName("hostlabel");
        hostlabel->setText(i18n("&Host:"));
        textlayout->addWidget(hostlabel);
        hostedit= new dialogedit(this,"hostedit");
        hostlabel->setBuddy(hostedit);
        fieldlayout->addWidget(hostedit);
        if (!focus_set)
        {
            focus_set=true;
            hostedit->setFocus();
        }
        if (fields!=0)hostedit->setText(QString::fromUtf8 (l2u(fields->host).c_str()));
    }
    else
    {
        hostlabel=NULL;
        hostedit=NULL;
    }

        if (p_connection->server_needs(hk_connection::NEEDS_DATABASENAME))
    {
        databasenamelabel = new QLabel(this);
        databasenamelabel->setObjectName("databasenamelabel");
        databasenamelabel->setText(i18n("Database &name:"));
        textlayout->addWidget(databasenamelabel);
        databasenameedit= new combobox(this);
        databasenameedit->setObjectName(QString::fromAscii("databasenameedit"));
	databasenameedit->setFocusPolicy(Qt::ClickFocus);
	databasenameedit->setEnabled(fields->allowdatabaseselect);
	vector<hk_string>* dblist = p_connection->dblist();
	vector<hk_string>::iterator it=dblist->begin();
	int i=0;
	while (it!=dblist->end())
	{
	  databasenameedit->addItem(QString::fromUtf8(l2u((*it)).c_str()));
	  if ((*it)==fields->database) databasenameedit->setCurrentIndex(i);
	  ++it;++i;
	}
	if (databasenameedit && databasenameedit->count()==0 && fields->database.size()>0)
	{
		databasenameedit->addItem(QString::fromUtf8(l2u(fields->database).c_str()));
		databasenameedit->setCurrentIndex(0);
	}
//	if (databasenameedit->count()==0)
	{
	databasenameedit->setEditable(true);
	databasenameedit->setAutoCompletion(true);
        }
	databasenamelabel->setBuddy(databasenameedit);
        fieldlayout->addWidget(databasenameedit);

    }
    else
    {
        databasenamelabel=NULL;
        databasenameedit=NULL;
    }



    if (p_connection->server_needs(hk_connection::NEEDS_USERNAME))
    {
        userlabel = new QLabel(this);
        userlabel->setObjectName("userlabel");
        userlabel->setText(i18n("&User:"));
        textlayout->addWidget(userlabel);
        useredit= new dialogedit(this,"useredit");
        userlabel->setBuddy(useredit);
        fieldlayout->addWidget(useredit);

        if (fields!=0)useredit->setText(QString::fromUtf8 (l2u(fields->user).c_str()));
        if (!focus_set)
        {
            focus_set=true;
            useredit->setFocus();
        }

    }
    else
    {
        userlabel=NULL;
        useredit=NULL;
    }









    if (p_connection->server_needs(hk_connection::NEEDS_PASSWORD))
    {
        passwordlabel = new QLabel(this);
        passwordlabel->setObjectName("passwordlabel");
        passwordlabel->setText(i18n("&Password:"));
        textlayout->addWidget(passwordlabel);
        passwordedit= new dialogedit(this,"passwordedit");
        passwordlabel->setBuddy(passwordedit);
        passwordedit->setEchoMode(dialogedit::Password);
	passwordedit->setText(QString::fromUtf8 (l2u(fields->password).c_str()));
        fieldlayout->addWidget(passwordedit);
        if (!focus_set)
        {
            focus_set=true;
            passwordedit->setFocus();
        }
    }
    else
    {
        passwordlabel=NULL;
        passwordedit=NULL;
    }

    if (p_connection->server_needs(hk_connection::NEEDS_PORT))
    {
        portlabel = new QLabel(this);
        portlabel->setObjectName("portlabel");
        portlabel->setText(i18n("&TCP Port:"));
        textlayout->addWidget(portlabel);
        portedit= new dialogedit(this,"portedit");
	portedit->setFocusPolicy(Qt::ClickFocus);
        portlabel->setBuddy(portedit);
        fieldlayout->addWidget(portedit);
        if (fields!=0)portedit->setText(QString::fromUtf8 (l2u(fields->port).c_str()));
        if (!focus_set)
        {
            focus_set=true;
            portedit->setFocus();
        }
    }
    else
    {
        portlabel=NULL;
        portedit=NULL;
    }

    if (p_connection->server_needs(hk_connection::NEEDS_SQLDELIMITER))
    {
        sqldelimiterlabel = new QLabel(this);
        sqldelimiterlabel->setObjectName("sqldelimiterlabel");
        sqldelimiterlabel->setText(i18n("S&QL Delimiter:"));
        textlayout->addWidget(sqldelimiterlabel);
        sqldelimiteredit= new dialogedit(this,"sqldelimiteredit");
	sqldelimiteredit->setFocusPolicy(Qt::ClickFocus);
        fieldlayout->addWidget(sqldelimiteredit);
        sqldelimiterlabel->setBuddy(sqldelimiteredit);
        if (fields!=0)sqldelimiteredit->setText(QString::fromUtf8 (l2u(fields->sql_delimiter).c_str()));
        if (!focus_set)
        {
            focus_set=true;
            portedit->setFocus();
        }

    }
    else
    {
        sqldelimiterlabel=NULL;
        sqldelimiteredit=NULL;
    }

    if (p_connection->server_needs(hk_connection::NEEDS_BOOLEANEMULATION))
    {
      booleanemulationlabel = new QLabel(this);
      booleanemulationlabel->setObjectName("booleanemulationlabel");
      textlayout->addWidget(booleanemulationlabel);
      booleanemulationlabel->setText(i18n("&Emulate Boolean:"));
      booleanemulationedit= new QComboBox(this);
      booleanemulationedit->setObjectName(QString::fromAscii("booleanemulationedit"));
      booleanemulationedit->addItem(i18n("yes"));
      booleanemulationedit->addItem(i18n("no"));
      booleanemulationedit->setFocusPolicy(Qt::ClickFocus);
      fieldlayout->addWidget(booleanemulationedit);
      booleanemulationlabel->setBuddy(booleanemulationedit);

      if (fields!=0) booleanemulationedit->setCurrentIndex(fields->emulateboolean?0:1);
        if (!focus_set)
        {
            focus_set=true;
            booleanemulationedit->setFocus();
        }
    }
    else
    {
      booleanemulationedit=NULL;
      booleanemulationlabel=NULL;
    }


    savesettingslabel = new QLabel(this);
    savesettingslabel->setObjectName("savesettingslabel");
//      savesettingslabel->setText(i18n("Save settings").c_str());
    textlayout->addWidget(savesettingslabel);
    savesettings=new dialogbool(this,"savesettings");
    savesettings->setText(i18n("&Save settings"));
    savesettings->setFocusPolicy(Qt::ClickFocus);
    fieldlayout->addWidget(savesettings);

    ok_button= new button(this,"OKButton");
    ok_button->setText(i18n("&OK"));
    buttonlayout->addWidget(ok_button);
    QSpacerItem* hspacer2=new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    buttonlayout->addItem(hspacer2);
    cancel_button= new button(this,"CancelButton");
//  cancel_button->setGeometry(leftedit,top+offset+2*v_space,buttonsize,buttonheight);
    cancel_button->setMinimumSize(0,0);
    cancel_button->setText(i18n("&Cancel"));
    buttonlayout->addWidget(cancel_button);
    ;
    if (p_fields) p_fields->cancel=true;

    connect( ok_button, SIGNAL(clicked()), SLOT(ok_buttonclicked()) );
    connect( cancel_button, SIGNAL(clicked()), SLOT(cancel_buttonclicked()) );
}


hk_kdepassworddialog::~hk_kdepassworddialog()
{
    delete ok_button;
    delete cancel_button;
    delete hostlabel;
    delete hostedit;
    delete userlabel;
    delete useredit;
    delete passwordlabel;
    delete passwordedit;
    delete sqldelimiterlabel;
    delete sqldelimiteredit;

}


void hk_kdepassworddialog::ok_buttonclicked()
{
    if (p_fields!=0)
    {
        if (p_connection->server_needs(hk_connection::NEEDS_HOST)) p_fields->host=u2l(hostedit->text().toUtf8().data());
        if (p_connection->server_needs(hk_connection::NEEDS_USERNAME)) p_fields->user=u2l(useredit->text().toUtf8().data());
        if (p_connection->server_needs(hk_connection::NEEDS_DATABASENAME)) p_fields->database=u2l(databasenameedit->currentText().toUtf8().data());
        if (p_connection->server_needs(hk_connection::NEEDS_PASSWORD)) p_fields->password=u2l(passwordedit->text().toUtf8().data());
        if (p_connection->server_needs(hk_connection::NEEDS_SQLDELIMITER)) p_fields->sql_delimiter=u2l(sqldelimiteredit->text().toUtf8().data());
        if (p_connection->server_needs(hk_connection::NEEDS_PORT)) p_fields->port=u2l(portedit->text().toUtf8().data());
        if (booleanemulationedit)p_fields->emulateboolean=(booleanemulationedit->currentIndex()==0);
	p_fields->cancel=false;
        p_fields->save=savesettings->isChecked();
    }
    accept();

}


void hk_kdepassworddialog::cancel_buttonclicked()
{
    if (p_fields!=0) p_fields->cancel=true;
    reject();
}
