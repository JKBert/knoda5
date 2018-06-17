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

#ifndef HK_KDESPINBOX
#define HK_KDESPINBOX
#include <qspinbox.h>
#include <qvalidator.h>
#include<hk_font.h>

class hk_kdespinboxvalidator;
class hk_kdespinbox : public QSpinBox
{
    Q_OBJECT

        public:
        hk_kdespinbox(QWidget* parent=0, const char* name=0);
        hk_kdespinbox(int minValue, int maxValue, int step=1,QWidget* parent=0,const char* name=0);
        virtual ~hk_kdespinbox();
	enum enum_displaytype{standard,percent,pixels,cm};
	void set_displaytype(enum_displaytype);
	enum_displaytype displaytype(void);

    protected:
        virtual QString textFromValue(int value) const;
        virtual int valueFromText(const QString& s) const;
	virtual QValidator::State validate(QString& inp, int & pos) const;
    private:
        void init(void);
        enum_displaytype p_displaytype;
        hk_kdespinboxvalidator* p_validator;
};


#endif
