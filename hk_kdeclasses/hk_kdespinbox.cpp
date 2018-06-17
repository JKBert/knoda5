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

#include "hk_kdespinbox.h"
#include <hk_string.h>
#include <hk_class.h>
#include <qvalidator.h>

class hk_kdespinboxvalidator :public QValidator
{
    public:
        hk_kdespinbox* p_spinbox;

        hk_kdespinboxvalidator(hk_kdespinbox* s):QValidator(s)
        {
            p_spinbox=s;
            p_ivalidator =new QIntValidator(this);

        }
        ~hk_kdespinboxvalidator()
        {
            delete p_ivalidator;
        }

        virtual QValidator::State validate(QString& val,int& ) const
        {
            QString v=val;
            int p = 0;
            
            p_ivalidator->setRange(p_spinbox->minimum(), p_spinbox->maximum());
            // process suffix
            if (p_spinbox->displaytype()!=hk_kdespinbox::standard)
            {
                int f=v.indexOf(p_spinbox->suffix());

                if (f>-1)
                    v.remove(f,p_spinbox->suffix().length());
            }
            
            // process decimal point
            if (isDecimalUnit()) {
                int f=v.indexOf(p_spinbox->locale().decimalPoint());
                
                if (f>-1)
                    v.remove(f,1);
                else
                    v += "00";
            }
            return p_ivalidator->validate(v, p);
        }
        
        bool inline isDecimalUnit() const { 
            return p_spinbox != NULL && 
                (p_spinbox->displaytype()==hk_kdespinbox::percent|| p_spinbox->displaytype()==hk_kdespinbox::cm);
        }

    protected:

    private:
        QIntValidator*    p_ivalidator;

};

hk_kdespinbox::hk_kdespinbox(QWidget* parent, const char* name): QSpinBox(parent),
p_validator(new hk_kdespinboxvalidator(this))
{
    setObjectName(QString::fromAscii(name));
    init();
}


hk_kdespinbox::hk_kdespinbox(int minValue, int maxValue, int step,QWidget* parent,const char* name)
: QSpinBox(parent),p_validator(new hk_kdespinboxvalidator(this))
{
    if (name)
        setObjectName(QString::fromAscii(name));
    init();
    setMinimum(minValue);
    setMaximum(maxValue);
    setSingleStep(step);
}


hk_kdespinbox::~hk_kdespinbox()
{

    delete p_validator;
}


void hk_kdespinbox::init(void)
{
    set_displaytype(percent);
    setMaximum(10000);
    setMinimum(0);
    setSingleStep(1);
    setLocale(QLocale(hk_class::locale().data()));
}

QValidator::State hk_kdespinbox::validate(QString& inp, int & pos) const
{
  return p_validator == NULL ? QValidator::Invalid:p_validator->validate(inp,pos);
}

QString hk_kdespinbox::textFromValue(int value) const
{   
    if (p_validator->isDecimalUnit()) 
    {
      QString n;
      n.setNum(value % 100);
      if (n.length()==1) n="0"+n;
      return QString("%1%2%3").arg(value/100).arg(locale().decimalPoint()).arg(n);
    } 
    return QSpinBox::textFromValue(value);

}


int hk_kdespinbox::valueFromText(const QString& s) const
{
    if (!p_validator->isDecimalUnit()) 
         return QSpinBox::valueFromText(s);

    hk_string txt=u2l(cleanText().toUtf8().data());
    double n=standardstring2double(txt, hk_class::locale());
    return (int) (100 * n+0.5);
}


void hk_kdespinbox::set_displaytype(enum_displaytype d)
{
    p_displaytype=d;
    QString s;
    switch (p_displaytype)
    {
     case percent : s=" %";break;
     case pixels  : s=" px";break;
     case cm  : hk_class::measuresystem()==hk_class::cm?s=" cm":s=" inch";break;
     default:;
    }
    setSuffix(s);
}


hk_kdespinbox::enum_displaytype hk_kdespinbox::displaytype(void)
{
    return p_displaytype;
}
