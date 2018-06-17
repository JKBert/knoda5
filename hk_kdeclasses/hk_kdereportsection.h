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

#ifndef HK_KDEREPORTSECTION_H
#define HK_KDEREPORTSECTION_H
#include <hk_reportsection.h>
#include <qwidget.h>
class hk_kdesimplereport;
class QPainter;
class hk_kdeformfocus;
class QLabel;

class hk_kdereportsection:public QWidget, public hk_reportsection
{

    Q_OBJECT

        public:
        hk_kdereportsection(hk_kdesimplereport* r, const char* name = 0, Qt::WFlags fl = 0 );
        virtual    ~hk_kdereportsection();
        hk_kdeformfocus* new_focus(void);
        hk_kdesimplereport* kdereport(void){return p_kdereport;}
        unsigned int max_displaywidth(void);
        unsigned int max_displayheight(void);
        void set_displaystring(const QString&);
        QString displaystring(void);
        QWidget* header(void);
        void set_activesection(bool active);
        void resize_section(void);
        void adjust_sectionsize(void);

        signals:
        void field_created(void);
    protected:

        virtual hk_reportdata* widget_specific_new_data(void);
        virtual void widget_specific_after_loaddata(void);
        virtual void paintEvent(QPaintEvent* e);
        virtual void mousePressEvent(QMouseEvent*);
        virtual void widget_specific_offset_changed(void);

    private:

        void create_field(const QPoint& point);
        hk_kdesimplereport* p_kdereport;
        unsigned int p_maxdisplayheight;
        unsigned int p_maxdisplaywidth;
        QString p_displaystring;
        QLabel* p_header;
        bool p_is_active_section;
        double pix_je_cm;

};
#endif
