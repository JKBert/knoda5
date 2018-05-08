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

#ifndef HK_KDESUBFORM_H
#define HK_KDESUBFORM_H
#include <hk_subform.h>
#include <qscrollarea.h>
class hk_kdesimpleform;

class hk_kdesubform: public QScrollArea,public hk_subform
{
Q_OBJECT
public:
  hk_kdesubform(hk_kdesimpleform*);
  virtual ~hk_kdesubform();
  virtual void show_widget(void);
  virtual void hide_widget(void);
  virtual bool close_widget(void);

protected:
  bool widget_specific_coordinates(uint px,uint py,uint pwidth,uint pheight);
  virtual void widget_specific_enabled_changed(void);
  void resizeEvent ( QResizeEvent * event );
  void moveEvent ( QMoveEvent * event);
  virtual bool	presentationmode_changed(void);

private:
  void reposition(void);
};

#endif //HK_KDESUBFORM_H
