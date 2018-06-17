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

#ifndef HK_KDEREPORTPROPERTYBASE_H
#define HK_KDEREPORTPROPERTYBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class hk_kdespinbox;
class QTabWidget;
class QLabel;
class QLineEdit;
class QToolButton;
class QComboBox;
class QPushButton;
class QSpinBox;
class hk_visible;

class hk_kdereportpropertybase : public QWidget
{
    Q_OBJECT

public:
    hk_kdereportpropertybase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~hk_kdereportpropertybase();

    QTabWidget* tab;
    QWidget* datatab;
    QLabel* subreportlabel;
    QLineEdit* identifierfield;
    QToolButton* datasourcebutton;
    QComboBox* runningcountfield;
    QLabel* columnlabel;
    QComboBox* sortingfield;
    QPushButton* subreportbutton;
    QLineEdit* valuefield;
    QLabel* identifierlabel;
    QComboBox* datasourcefield;
    QLabel* runningcountlabel;
    QLabel* valuelabel;
    QLabel* sortinglabel;
    QComboBox* columnfield;
    QLabel* datasourcelabel;
    QLabel* Idlabel;
    QLineEdit* idfield;
    QWidget* formattab;
    QLabel* widthlabel;
    QLabel* fontlabel;
    QComboBox* fontfield;
    QSpinBox* fontsizefield;
    QLabel* ylabel;
    QLabel* fontsizelabel;
    QLabel* reporttypelabel;
    QComboBox* reporttypefield;
    QLabel* alignmentlabel;
    QComboBox* alignmentfield;
    QLabel* boldlabel;
    QLabel* italiclabel;
    QComboBox* boldfield;
    QComboBox* italicfield;
    QLabel* xlabel;
    hk_kdespinbox* xfield;
    QLabel* heightlabel;
    hk_kdespinbox* heightfield;
    hk_kdespinbox* widthfield;
    hk_kdespinbox* yfield;
    QSpinBox* digitfield;
    QComboBox* separatorfield;
    QLabel* digitlabel;
    QLabel* separatorlabel;
    QLabel* wordbreaklabel;
    QComboBox* wordbreakfield;
    QLabel* dynamicheightlabel;
    QComboBox* dynamicheightfield;
    QWidget* frametab;
    QComboBox* toplinefield;
    QComboBox* bottomlinefield;
    QComboBox* leftlinefield;
    QComboBox* rightlinefield;
    QComboBox* diagonallurofield;
    QComboBox* diagonallorufield;
    QLabel* toplinelabel;
    QLabel* bottomlinelabel;
    QLabel* leftlinelabel;
    QLabel* rightlinelabel;
    QLabel* diagonallurolabel;
    QLabel* diagonallorulabel;
    hk_kdespinbox* bordertopfield;
    hk_kdespinbox* borderbottomfield;
    hk_kdespinbox* borderleftfield;
    hk_kdespinbox* borderrightfield;
    QLabel* borderrightlabel;
    QLabel* borderleftlabel;
    QLabel* borderbottomlabel;
    QLabel* bordertoplabel;
    QWidget* actiontab;
    QLabel* onprintlabel;
    QPushButton* onprintaction;
    QLabel* afterrowchangelabel_3;
    QLabel* beforerowchangelabel_3;
    QPushButton* afterrowchangeaction;
    QLabel* openactionlabel;
    QLabel* closeactionlabel;
    QLabel* onnewpagelabel;
    QPushButton* onnewpageaction;
    QPushButton* onopenactionbutton;
    QPushButton* oncloseactionbutton;
    QPushButton* beforerowchangeaction;

public slots:
    virtual void identifier_changed();
    virtual void actionobject_changes();
    virtual void add_propertydatasource();
    virtual void data_changes();
    virtual void datasource_selected();
    virtual void datasourcebutton_clicked();
    virtual void format_changes();
    virtual void subreportbutton_clicked();
    virtual void set_object( hk_visible * );
    virtual void onprint_action_clicked();
    virtual void beforerowchange_action_clicked();
    virtual void afterrowchange_action_clicked();
    virtual void onprintnewpage_action_clicked();
    virtual void closeactionbutton_clicked();
    virtual void openactionbutton_clicked();
    virtual void font_changes();
    virtual void position_changes();
    virtual void size_changes();

protected:
    QGridLayout* hk_kdereportpropertybaseLayout;
    QGridLayout* datatabLayout;
    QSpacerItem* Spacer3;
    QSpacerItem* Spacer4;
    QGridLayout* formattabLayout;
    QGridLayout* frametabLayout;
    QSpacerItem* Spacer1_2;
    QSpacerItem* Spacer2_2;
    QGridLayout* actiontabLayout;
    QSpacerItem* spacer23;
    QSpacerItem* spacer24;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // HK_KDEREPORTPROPERTYBASE_H
