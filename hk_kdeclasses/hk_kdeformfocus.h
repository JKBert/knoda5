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

#ifndef HK_KDEFORMFOCUS
#define HK_KDEFORMFOCUS
#include <hk_class.h>
#include <qwidget.h>
#include <qobject.h>
#include <list>
class hk_kdeformfocus;
class hk_kdesimpleform;
class hk_kdesimplereport;
class hk_kdereportsection;
class hk_report;

class hk_marker :public QWidget,hk_class
{
    Q_OBJECT
        friend class hk_kdeformfocus;
    public:
        enum enum_markertype {lo,mo,ro,lm,rm,lu,mu,ru};
        hk_marker(hk_kdesimpleform*,hk_kdeformfocus*,hk_marker::enum_markertype);
//  hk_marker(hk_kdesimplereport*,hk_kdeformfocus*,hk_marker::enum_markertype);
        hk_marker(hk_kdereportsection*,hk_kdeformfocus*,hk_marker::enum_markertype);
        ~hk_marker(void);
        void  set_widget(QWidget*);
        enum_markertype type(void);
        void set_cursors(void);

    protected:
        virtual   void paintEvent( QPaintEvent* );
        virtual   void mousePressEvent( QMouseEvent*  );
        virtual   void mouseMoveEvent( QMouseEvent*  );
    private:
        enum_markertype p_type;
        QWidget* p_widget;
        QPoint   p_originalposition;
        hk_kdeformfocus* p_focus;
        hk_kdesimpleform* p_form;
        hk_kdesimplereport* p_report;
        hk_kdereportsection* p_section;
};

class hk_kdeformfocus :public QObject,public hk_class
{
    Q_OBJECT
        friend class hk_marker;
    signals:
    void size_changed();

    public:
        hk_kdeformfocus(hk_kdesimpleform*);
// hk_kdeformfocus(hk_kdesimplereport*);
        hk_kdeformfocus(hk_kdereportsection*);
        ~hk_kdeformfocus(void);
        void set_widget(QWidget*);
        QWidget* widget(void);
        void show(void);
        void hide(void);
        void set_positions(void);
        void repaint(void);
    protected:
        bool eventFilter(QObject* object,QEvent* event);

    private:
        QWidget* p_widget;
        bool p_markermove;
        double pix_je_cm;
        list<hk_marker*> p_markers;
        void marker_moved(void);
// void clearmultiplefocus(void);
        hk_kdereportsection* p_section;
};
#endif
