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

#ifndef HK_KDEREPORTDATA_H
#define HK_KDEREPORTDATA_H
#include <hk_reportdata.h>
#include <qwidget.h>
class hk_kdereportsection;
class hk_kdesimplereport;
class hk_kdereportdata:public QWidget, public hk_reportdata
{

    Q_OBJECT

        public:
        hk_kdereportdata( hk_kdereportsection* s, const char* name = 0, Qt::WFlags fl = 0 );
        virtual    ~hk_kdereportdata();

        hk_kdereportsection* section(void);

    protected:
        void paintEvent(QPaintEvent* e);
        virtual void resizeEvent (QResizeEvent *);
        virtual void  moveEvent ( QMoveEvent * ) ;
        virtual bool widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight);
        virtual void widget_specific_backgroundcolour_changed(const hk_colour&);
        virtual void widget_specific_foregroundcolour_changed(const hk_colour&);

    private:
        void    reposition(void);
        hk_kdereportsection* p_kdesection;
        double pix_je_cm ;
};
#endif
