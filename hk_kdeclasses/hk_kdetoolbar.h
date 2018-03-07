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
//Version $Revision: 1.16 $

#ifndef HK_KDETOOLBAR_H
#define HK_KDETOOLBAR_H
//#include <hk_kdetoolbarbase.h>
#include <hk_dsvisible.h>
#include <qstring.h>
#include <qobject.h>

class QToolButton;
class QVBoxLayout;
class QHBoxLayout;
class KAction;
class KToggleAction;

class hk_kdetoolbar : public QObject ,public hk_dsvisible
{
    Q_OBJECT
        public:
        hk_kdetoolbar(QWidget* w=NULL,const char* s=NULL);
        ~hk_kdetoolbar(void);
        void set_designmode(void);
        void set_viewmode(void);
        void set_nodesignmode(bool);
        bool is_filter_activated(void);
        void set_filteractions(KToggleAction* filterdefinition,KToggleAction* filterexec);
    public slots:
        void deactivate_filter(void);

        void set_filter(const QString& s);
        void set_filter(bool);

    protected:

    protected slots:
        void designbutton_clicked(void);
        void viewbutton_clicked(void);
//void savebutton_clicked();
//void exitbutton_clicked();

        void filterdefinebutton_clicked();
        void filterexecbutton_clicked();
        void activate_filter();
    private:
        bool p_showqbebutton;
        bool p_showsavebutton;
        bool p_showfilterbutton;
        bool p_nodesignmode;
        bool p_showsearchbutton;
        bool p_is_activated;
        bool p_filter_should_be_activated;
        bool p_use_filter;
        QString p_filter;
        hk_string p_originalfilter;
        bool p_viewmode;
        QWidget* p_parent;
        KToggleAction*      p_filterdefinitionaction;
        KToggleAction*    p_filterexecaction;
};
#endif
