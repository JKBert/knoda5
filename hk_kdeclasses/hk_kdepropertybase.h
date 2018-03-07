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
/****************************************************************************
** Form interface generated from reading ui file 'propertyeditor.ui'
**
** Created: Sa Jul 30 16:59:14 2005
**      by: The User Interface Compiler ($Id: hk_kdepropertybase.h,v 1.51 2005/07/30 15:07:58 knorr Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef HK_KDEPROPERTYEDITORBASE_H
#define HK_KDEPROPERTYEDITORBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class hk_kdespinbox;
class QTabWidget;
class QLineEdit;
class QLabel;
class QComboBox;
class QToolButton;
class QSpinBox;
class KColorButton;
class QPushButton;
class hk_visible;

class hk_kdepropertyeditorbase : public QWidget
{
    Q_OBJECT

public:
    hk_kdepropertyeditorbase( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~hk_kdepropertyeditorbase();

    QTabWidget* tab;
    QWidget* tab_2;
    QLineEdit* labelfield;
    QLabel* combomodelabel;
    QLabel* labellabel;
    QComboBox* combomodefield;
    QLabel* defaultlabel;
    QLineEdit* defaultfield;
    QToolButton* datasourcebutton;
    QLabel* datasourcelabel;
    QComboBox* datasourcefield;
    QComboBox* columnfield;
    QLabel* tooltiplabel;
    QLineEdit* tooltipfield;
    QLabel* columnlabel;
    QLabel* identifierlabel;
    QLabel* Idlabel;
    QLineEdit* identifierfield;
    QLineEdit* idfield;
    QLabel* subformlabel;
    QComboBox* listcolumnfield;
    QComboBox* listdatasourcefield;
    QLabel* listcolumnlabel;
    QToolButton* subformbutton;
    QLabel* listdatasourcelabel;
    QLabel* viewcolumnlabel;
    QComboBox* viewcolumnfield;
    QComboBox* subformfield;
    QLabel* readonlylabel;
    QComboBox* readonlyfield;
    QComboBox* usetextlistfield;
    QLabel* usetextlistlabel;
    QToolButton* listdatasourcebutton;
    QToolButton* usetextlistbutton;
    QWidget* tab_3;
    QSpinBox* fontsizefield;
    QComboBox* boldfield;
    QComboBox* italicfield;
    QComboBox* separatorfield;
    KColorButton* p_backgroundcolourbutton;
    QComboBox* fontfield;
    QLabel* xlabel;
    QLabel* boldlabel;
    QLabel* p_backgroundcolourlabel;
    QLabel* separatorlabel;
    hk_kdespinbox* yfield;
    hk_kdespinbox* heightfield;
    QLabel* fontsizelabel;
    hk_kdespinbox* xfield;
    QLabel* fontlabel;
    QLabel* heightlabel;
    QLabel* ylabel;
    hk_kdespinbox* widthfield;
    KColorButton* p_foregroundcolourbutton;
    QLabel* p_foregroundcolourlabel;
    QLabel* widthlabel;
    QLabel* italiclabel;
    QLabel* digitlabel;
    QSpinBox* digitfield;
    QLabel* scalelabel;
    QSpinBox* scalefield;
    QLabel* alignmentlabel;
    QComboBox* alignmentfield;
    QLabel* enabledlabel;
    QLabel* localimagelabel;
    QPushButton* localimagebutton;
    QComboBox* enabledfield;
    QLabel* istogglebuttonlabel;
    QComboBox* istogglebuttonfield;
    QLabel* togglepushedlabel;
    QComboBox* togglepushedfield;
    QWidget* frametab;
    hk_kdespinbox* leftlinefield;
    QLabel* toplinelabel;
    QLabel* bottomlinelabel;
    QLabel* diagonallurolabel;
    hk_kdespinbox* rightlinefield;
    QLabel* diagonallorulabel;
    hk_kdespinbox* toplinefield;
    hk_kdespinbox* diagonallurofield;
    hk_kdespinbox* bottomlinefield;
    QLabel* leftlinelabel;
    QLabel* rightlinelabel;
    hk_kdespinbox* diagonallorufield;
    QWidget* tab_4;
    QLabel* actionlabel;
    QLabel* objectlabel;
    QComboBox* objectfield;
    QComboBox* actionfield;
    QComboBox* showmaximizedfield;
    QPushButton* conditionbutton;
    QLabel* showmaximizedlabel;
    QPushButton* onpushactionbutton;
    QLabel* pushactionlabel;
    QLabel* doubleclickactionlabel;
    QPushButton* ondoubleclickactionbutton;
    QLabel* ongetfocuslabel;
    QPushButton* ongetfocusbutton;
    QLabel* beforedeletelabel;
    QPushButton* oncloseactionbutton;
    QPushButton* beforeupdateactionbutton;
    QPushButton* afterupdateactionbutton;
    QLabel* afterupdatelabel;
    QLabel* beforeupdateactionlabel;
    QLabel* afterdeletelabel;
    QLabel* afterrowchangelabel;
    QPushButton* afterrowchangeactionbutton;
    QLabel* beforeinsertlabel;
    QPushButton* beforeinsertactionbutton;
    QPushButton* afterdeleteactionbutton;
    QLabel* closeactionlabel;
    QPushButton* beforedeleteactionbutton;
    QPushButton* onopenactionbutton;
    QPushButton* beforerowchangeactionbutton;
    QLabel* beforerowchangelabel;
    QLabel* openactionlabel;
    QLabel* onloosefocuslabel;
    QPushButton* onloosefocusbutton;
    QPushButton* onkeybutton;
    QLabel* onkeylabel;
    QLabel* onselectlabel;
    QPushButton* onselectactionbutton;
    QLabel* afterinsertlabel;
    QPushButton* afterinsertactionbutton;
    QLabel* onvaluechangedactionlabel;
    QPushButton* onvaluechangedactionbutton;

public slots:
    virtual void pushactionbutton_clicked();
    virtual void actionobject_changes();
    virtual void add_formdatasource();
    virtual void add_listformdatasource();
    virtual void combomode_changed();
    virtual void data_changes();
    virtual void datasource_selected();
    virtual void format_changes();
    virtual void listdatasource_selected();
    virtual void conditionbutton_clicked();
    virtual void set_object( hk_visible * );
    virtual void openactionbutton_clicked();
    virtual void closeactionbutton_clicked();
    virtual void beforeupdatebutton_clicked();
    virtual void afterupdatebutton_clicked();
    virtual void beforedeletebutton_clicked();
    virtual void afterdeletebutton_clicked();
    virtual void beforerowchangebutton_clicked();
    virtual void afterrowchangebutton_clicked();
    virtual void action_changes();
    virtual void beforeinsertbutton_clicked();
    virtual void afterinsertbutton_clicked();
    virtual void identifier_changed();
    virtual void doubleclickactionbutton_clicked();
    virtual void border_changes();
    virtual void geometry_changes();
    virtual void subformbutton_clicked();
    virtual void ongetfocusbutton_clicked();
    virtual void onloosefocusbutton_clicked();
    virtual void tooltipfield_changed();
    virtual void onkeybutton_clicked();
    virtual void use_textlist_changed();
    virtual void use_textlistbutton_clicked();
    virtual void onselectbutton_clicked();
    virtual void localimage_selected();
    virtual void has_changed();
    virtual void onvaluechangedbutton_clicked();
    virtual void buttonformat_changed();

protected:
    QGridLayout* hk_kdepropertyeditorbaseLayout;
    QGridLayout* tabLayout;
    QSpacerItem* spacer9;
    QSpacerItem* Spacer4;
    QSpacerItem* Spacer3;
    QGridLayout* tabLayout_2;
    QSpacerItem* spacer10;
    QSpacerItem* Spacer1;
    QSpacerItem* Spacer2;
    QGridLayout* frametabLayout;
    QSpacerItem* spacer11;
    QGridLayout* layout3;
    QSpacerItem* Spacer2_2;
    QSpacerItem* Spacer1_2;
    QGridLayout* tabLayout_3;
    QSpacerItem* spacer12;
    QSpacerItem* Spacer6;
    QSpacerItem* Spacer7;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

};

#endif // HK_KDEPROPERTYEDITORBASE_H
