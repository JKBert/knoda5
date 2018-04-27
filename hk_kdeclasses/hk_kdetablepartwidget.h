// ****************************************************************************
// copyright (c) 2000-2005 Horst Knorr <hk_classes@knoda.org>
// Original version of the file for hk_kdeclasses library
// copyright (c) 2010-2015 Patrik Hanak <hanakp@users.sourceforge.net>
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

#ifndef HK_KDETABLEPARTWIDGET_H
#define HK_KDETABLEPARTWIDGET_H

#include <kde4classesmacros.h>
#include <qvariant.h>
#include <qwidget.h>
#include <qstackedwidget.h>
#include <kaction.h>
#include <hk_dstable.h>
#include <KParts/ReadWritePart>

class hk_kdetabledesign;
class hk_kdegrid;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QButtonGroup;
class QTable;
class QProgressDialog;
class hk_kdetoolbar;
class hk_kdetablepart;
class KToggleAction;

/**
 *
 *@short  KDE high level widget to display and alter tables.
 *@version $Revision: 1.10 $
 *@author Horst Knorr (hk_classes@knoda.org)

 *hk_kdetablepartwidget is a high level widget to display the content of tables and to define and alter the
 *table structure.
 *
 */

class KDECLASSES_EXPORT hk_kdetablepartwidget : public QStackedWidget ,public hk_dstable
{
    friend class hk_kdetablepart;
    Q_OBJECT

  public:
    virtual    ~hk_kdetablepartwidget();
    virtual bool set_mode(enum_mode s);

/**
 * true if the design window is shown
 */
    virtual void set_datasource(hk_datasource*);
    hk_datasource* datasource(void);
/**
 *when the datasource vanishes (i.e. will be destroyed) and autoclose is true, the hk_kdetablepartwidget will be
 *closed otherwise it still is on your screen.
 */

    void set_autoclose(bool c){p_autoclose=c;}
	hk_dsgrid* grid(void)const ;
	hk_kdegrid* kdegrid(void) const;
	hk_kdetoolbar* toolbar(void)const;
	KParts::ReadWritePart* gridpart(void) const;
	bool in_designmode(void);

    virtual void show_widget(void);
    virtual void hide_widget(void);
    virtual bool close_widget(void);

	void alter_table(void);
  signals:
    void signal_closed(QWidget*);
	void signal_captionChanged(QWidget*, const QString&);

  public slots:
    void close_table(void);
	void save_table(void);
	void reload_table(void);
	void slot_has_changed(void);
    void designbutton_clicked(void);
    void tablebutton_clicked(void);
	void set_caption(void);

  protected:
    void internal_designbutton_clicked(void);
    void internal_tablebutton_clicked(void);
    hk_kdetablepartwidget(QWidget* w,const char* n=0,Qt::WFlags f=0);
    virtual void before_source_vanishes(void);
    void closeEvent ( QCloseEvent* e);
    hk_kdetoolbar* p_toolbar;
    hk_kdegrid* p_grid;
    hk_kdetabledesign* p_design;
	KToggleAction* p_designaction;
    KToggleAction* p_viewaction;
    KToggleAction* p_filterdefinitionaction;
    KToggleAction* p_filterexecaction;
    KAction*  p_printaction;
    KAction* p_columndialogaction;
    KAction* p_findaction;
    KAction* p_copyaction;
    KAction* p_pasteaction;
	KAction* p_saveaction;
	KAction* p_reloadaction;
  static  bool set_progress(long int,long int,const hk_string&);

  protected slots:
    void exec_cancelled(void);

  private:
    bool p_autoclose;
    KParts::ReadWritePart* p_gridpart;
    static  QProgressDialog* p_progressdialog;
    static bool p_cancel;
    void before_execute(void);
	void setupActions(KActionCollection* ac);
    void after_execute(void);
};
#endif                                            // HK_KDETABLEPARTWIDGET_H
