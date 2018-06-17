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

#include "hk_kdesubreportdialogbase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qframe.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>
#include <qtreewidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

static const unsigned char image0_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x02,
    0x97, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9c, 0xb5, 0x95, 0x4f, 0x4b, 0x1b,
    0x41, 0x18, 0x87, 0x9f, 0x94, 0x3d, 0xcc, 0x40, 0x0a, 0xbb, 0x60, 0x21,
    0x81, 0x16, 0x1c, 0xa8, 0x87, 0x88, 0x97, 0x80, 0x42, 0x4d, 0x3e, 0x41,
    0x93, 0xf6, 0x22, 0xb4, 0x50, 0x3c, 0x55, 0x8f, 0x1e, 0x3d, 0xf5, 0x2e,
    0xf4, 0x0b, 0x98, 0x53, 0xbd, 0xda, 0x5e, 0xd4, 0x8f, 0xe0, 0x2d, 0xb1,
    0x20, 0xe8, 0xa5, 0x24, 0x42, 0x4b, 0x37, 0xa0, 0xb8, 0x7b, 0x08, 0xec,
    0x82, 0x81, 0x79, 0x0f, 0x0b, 0xe9, 0x61, 0x9b, 0x6d, 0x34, 0x46, 0x63,
    0x4b, 0xdf, 0xcb, 0x32, 0xff, 0x9e, 0xf9, 0xcd, 0x6f, 0xe7, 0x7d, 0x27,
    0xd7, 0x6c, 0x35, 0xf9, 0x1f, 0xe1, 0xdc, 0x37, 0xa1, 0x5a, 0xa9, 0x0e,
    0x46, 0xdb, 0xcd, 0x56, 0x33, 0xf7, 0xd7, 0xe0, 0x51, 0x58, 0xed, 0x55,
    0x0d, 0xe5, 0x28, 0x82, 0x30, 0xa0, 0xf6, 0xb2, 0x46, 0xbd, 0x56, 0x1f,
    0x14, 0x0b, 0x45, 0x94, 0xa3, 0x88, 0xfa, 0x11, 0xbb, 0x9f, 0x77, 0x6f,
    0xdd, 0xe8, 0x1a, 0x78, 0x08, 0xbc, 0xfc, 0x79, 0x49, 0xb1, 0x50, 0x24,
    0xe8, 0x05, 0x6c, 0x7d, 0xdc, 0xe2, 0xf4, 0xf4, 0x94, 0xf2, 0x52, 0x99,
    0x38, 0x8e, 0x29, 0x2d, 0x94, 0x58, 0x7f, 0xbf, 0x4e, 0xe1, 0x49, 0x81,
    0x30, 0x08, 0xb3, 0x35, 0x37, 0x4f, 0x92, 0x81, 0xeb, 0xb5, 0xfa, 0x60,
    0xbb, 0xb1, 0xcd, 0xda, 0xbb, 0x35, 0xb4, 0xd2, 0xa0, 0x41, 0x44, 0x58,
    0x7d, 0xb3, 0x4a, 0xed, 0x65, 0x0d, 0x2f, 0xef, 0xd1, 0xf8, 0xd4, 0xc0,
    0xff, 0xe1, 0x13, 0xc5, 0x11, 0xa5, 0xb9, 0x12, 0x38, 0xb0, 0xd3, 0xd8,
    0x21, 0x08, 0x03, 0xaa, 0x95, 0xea, 0x60, 0x14, 0x9e, 0x81, 0x5d, 0xd7,
    0x85, 0x04, 0x0e, 0xbf, 0x1e, 0xa2, 0x1d, 0x8d, 0x99, 0x35, 0xb8, 0x79,
    0x97, 0xc2, 0x62, 0x01, 0x11, 0x41, 0x44, 0x30, 0xcf, 0x0d, 0x1b, 0x0b,
    0x1b, 0x68, 0x47, 0xd3, 0x3a, 0x6e, 0x11, 0xf7, 0x62, 0xfc, 0xae, 0x8f,
    0x5c, 0xc9, 0x64, 0x2b, 0x44, 0x84, 0x20, 0x0c, 0x58, 0x79, 0xbd, 0x82,
    0x24, 0x42, 0xfb, 0xac, 0x9d, 0xf5, 0xc7, 0x57, 0x31, 0x0a, 0x85, 0x88,
    0x80, 0x80, 0xdf, 0xf3, 0x51, 0x2a, 0x6d, 0x8b, 0x15, 0x22, 0x89, 0x26,
    0x83, 0xf7, 0x0f, 0xf6, 0x73, 0xd5, 0x4a, 0x75, 0x10, 0xf5, 0x23, 0xcc,
    0xac, 0xa1, 0xb2, 0x54, 0xc1, 0xcd, 0xbb, 0xe8, 0xbc, 0xa6, 0x75, 0xd4,
    0x02, 0xa0, 0xbc, 0x50, 0x4e, 0x37, 0xeb, 0x0b, 0x27, 0x67, 0x27, 0xe9,
    0xf7, 0xdb, 0x09, 0xe1, 0x45, 0x38, 0xd9, 0xe3, 0x61, 0x54, 0x16, 0x2b,
    0x98, 0x39, 0xc3, 0xc1, 0x97, 0x03, 0x2c, 0x96, 0xf9, 0xd9, 0x79, 0x2c,
    0x16, 0x8d, 0xa6, 0xdd, 0x6d, 0xa3, 0xd1, 0x00, 0x78, 0x79, 0x8f, 0xce,
    0x79, 0x87, 0xe2, 0x4c, 0x91, 0x38, 0x8e, 0x27, 0x2b, 0x1e, 0x46, 0x10,
    0x06, 0x18, 0x63, 0xd8, 0xfc, 0xb0, 0x49, 0x14, 0x46, 0xf8, 0xa1, 0x8f,
    0xa7, 0x3c, 0x3a, 0xdf, 0x3b, 0x63, 0x9b, 0x90, 0x40, 0xdc, 0x4f, 0x6d,
    0xba, 0x19, 0x8f, 0x6e, 0x76, 0x68, 0x47, 0x43, 0x02, 0xae, 0x72, 0x39,
    0x3a, 0x3e, 0x02, 0x01, 0x54, 0x7a, 0x7c, 0x1c, 0xf0, 0x94, 0x87, 0xdf,
    0xf5, 0xd1, 0x8e, 0x46, 0x90, 0xd4, 0x7b, 0xc6, 0x7f, 0xde, 0x18, 0xd8,
    0x26, 0x16, 0x1c, 0x90, 0x24, 0xbd, 0x09, 0xb1, 0x8d, 0xd3, 0x04, 0xe9,
    0x05, 0xd8, 0xc4, 0x12, 0x49, 0x84, 0xef, 0xfb, 0x90, 0x30, 0x11, 0x7a,
    0xa7, 0x62, 0x1c, 0xb2, 0xc5, 0x24, 0xbf, 0x07, 0x13, 0xd0, 0xa4, 0x4a,
    0x71, 0xc8, 0x14, 0x4f, 0x05, 0x1e, 0x2a, 0x1e, 0xc2, 0xaf, 0x29, 0x72,
    0xc0, 0x62, 0x51, 0x28, 0x6c, 0x62, 0x33, 0xc5, 0x0f, 0xf2, 0x58, 0x39,
    0xea, 0x8f, 0xe2, 0x61, 0x8c, 0x28, 0xfe, 0x27, 0x8f, 0x33, 0xc5, 0xc3,
    0xbb, 0x33, 0x41, 0xf1, 0x54, 0x56, 0x4c, 0xeb, 0xf1, 0xa8, 0xe2, 0xa9,
    0xc0, 0x96, 0x07, 0x78, 0xec, 0x28, 0x70, 0x98, 0xce, 0x63, 0xf3, 0xcc,
    0xa0, 0xf2, 0x6a, 0x2a, 0x8f, 0xdd, 0xc7, 0x69, 0xe1, 0x92, 0xe4, 0x8e,
    0x22, 0x34, 0x0c, 0x6f, 0xc6, 0xc3, 0x3c, 0x35, 0xe8, 0xbc, 0xa6, 0x54,
    0x2e, 0x65, 0xb3, 0x5c, 0xd7, 0xc5, 0x9b, 0xf1, 0xd2, 0x92, 0x69, 0x4a,
    0xd8, 0xc4, 0x22, 0x89, 0x10, 0xf5, 0x23, 0xe2, 0xfe, 0x3d, 0x29, 0xdd,
    0x6c, 0x35, 0x73, 0xab, 0x6f, 0x57, 0x07, 0xdd, 0x8b, 0x2e, 0x7b, 0xbb,
    0x7b, 0x98, 0x92, 0x49, 0x33, 0xed, 0xc2, 0xa7, 0x58, 0x28, 0xa6, 0x93,
    0x24, 0x4d, 0x7b, 0xff, 0xdc, 0x47, 0x44, 0xe8, 0x76, 0xbb, 0xb7, 0x3e,
    0x57, 0xb9, 0x49, 0x8f, 0x69, 0xb5, 0x52, 0x1d, 0x2c, 0xbf, 0x58, 0x4e,
    0x59, 0x89, 0x64, 0x9e, 0x47, 0x71, 0x84, 0xf4, 0x85, 0x30, 0x1c, 0xaf,
    0x68, 0xa3, 0xf1, 0x0b, 0x86, 0x17, 0x6a, 0xbb, 0x39, 0x01, 0xed, 0x1e,
    0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

static const unsigned char image1_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x01,
    0xb4, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9c, 0xd5, 0xd5, 0x3f, 0x6a, 0xe3,
    0x40, 0x1c, 0xc5, 0xf1, 0xaf, 0x83, 0x8a, 0x09, 0xa4, 0x71, 0x67, 0xc3,
    0x2e, 0xd8, 0xc5, 0x16, 0xbe, 0x42, 0x7c, 0xa2, 0xfc, 0x61, 0x9b, 0xe4,
    0x02, 0x81, 0x34, 0x01, 0x97, 0xb9, 0x42, 0x52, 0xca, 0x45, 0x0e, 0xa0,
    0x85, 0x44, 0x52, 0x11, 0x70, 0x8a, 0x2d, 0xc6, 0x9d, 0x9a, 0xc0, 0xa4,
    0xf3, 0xc0, 0x1a, 0xe6, 0x57, 0x0c, 0x78, 0x0b, 0x6d, 0x26, 0xb1, 0x2d,
    0xc9, 0x72, 0x91, 0x62, 0xd5, 0x09, 0x3e, 0x7a, 0x7a, 0x3c, 0x06, 0xa9,
    0x93, 0x66, 0x29, 0x5f, 0x71, 0x1d, 0x7c, 0x49, 0xea, 0x7b, 0xf0, 0xf9,
    0xe9, 0xf9, 0xea, 0xf2, 0xe7, 0xe5, 0x6a, 0x17, 0x6e, 0xeb, 0x42, 0x30,
    0x11, 0x0c, 0x47, 0x43, 0xc6, 0xc7, 0xe3, 0xe6, 0x87, 0xda, 0xba, 0x92,
    0x02, 0x1e, 0x2e, 0x4e, 0x2e, 0x00, 0x18, 0x1f, 0x8f, 0x57, 0x69, 0x96,
    0x76, 0x2a, 0xf5, 0x3f, 0xa7, 0x50, 0xcd, 0x2e, 0x04, 0x03, 0x82, 0x70,
    0x7b, 0x73, 0xbb, 0x33, 0x5c, 0x10, 0x26, 0x37, 0x13, 0x04, 0x69, 0x74,
    0x11, 0x80, 0x78, 0x41, 0x96, 0x82, 0x3a, 0x52, 0x4c, 0xae, 0x26, 0xf5,
    0xe1, 0x9e, 0x35, 0xd7, 0xd4, 0x3c, 0x4c, 0xe1, 0x96, 0x0e, 0x6b, 0x2d,
    0xb3, 0xf9, 0xac, 0x36, 0x5c, 0xbc, 0xb0, 0x58, 0x2e, 0xb0, 0xd6, 0xa2,
    0xe7, 0x9a, 0xeb, 0xab, 0xeb, 0xda, 0xe6, 0x11, 0x80, 0xf3, 0x0e, 0x22,
    0x30, 0x6f, 0x06, 0x6b, 0x2c, 0xb1, 0x8e, 0x2b, 0xc3, 0x9d, 0x77, 0xa8,
    0x48, 0x61, 0xde, 0x0c, 0xc6, 0x18, 0xb4, 0xd6, 0xb5, 0xcd, 0x43, 0x63,
    0xfb, 0xc7, 0xd2, 0xef, 0xf5, 0x11, 0x2f, 0x38, 0xef, 0xb8, 0x9b, 0xde,
    0x6d, 0x87, 0xd7, 0xb8, 0xaa, 0xe6, 0x61, 0x63, 0x04, 0xcc, 0xab, 0x61,
    0x61, 0x17, 0x88, 0x17, 0x14, 0x8a, 0xf8, 0x21, 0xe6, 0xec, 0xe4, 0x2c,
    0x84, 0xf7, 0xbe, 0xf5, 0x40, 0xa0, 0x78, 0x2d, 0xb6, 0xdc, 0xe6, 0x69,
    0xf9, 0x68, 0xbc, 0xb4, 0x08, 0x82, 0x88, 0x94, 0xaf, 0x8b, 0x60, 0xf0,
    0x7d, 0x40, 0x3c, 0x8d, 0x29, 0x8a, 0x82, 0xcf, 0x4e, 0xa1, 0x10, 0x11,
    0x54, 0xa4, 0x82, 0xbb, 0x9f, 0xde, 0xa3, 0x0b, 0x1d, 0x36, 0x3e, 0x00,
    0x10, 0x11, 0x9c, 0x38, 0xf0, 0xa0, 0x22, 0x45, 0x57, 0x75, 0x19, 0xfd,
    0x18, 0x91, 0xe4, 0x09, 0xb3, 0x7c, 0x46, 0xfe, 0x94, 0x93, 0x66, 0x69,
    0x67, 0xd3, 0x1d, 0xaa, 0xc3, 0x4a, 0xb7, 0xbe, 0xb1, 0xb5, 0xe5, 0x9d,
    0x82, 0xc1, 0x70, 0x40, 0xf2, 0x98, 0x90, 0xe5, 0xd9, 0x1a, 0x6e, 0xed,
    0x3e, 0x6f, 0xac, 0x22, 0x45, 0xf7, 0xa8, 0x4b, 0xaf, 0xdf, 0x23, 0x79,
    0xdc, 0x6e, 0xb0, 0x8f, 0x0b, 0x53, 0xe0, 0xc1, 0x4a, 0xd9, 0xe4, 0xbd,
    0x41, 0xf2, 0x94, 0x6c, 0xe1, 0xd6, 0x2e, 0x6c, 0xec, 0x05, 0x3c, 0xe8,
    0xb9, 0xae, 0x6d, 0xb0, 0x8f, 0xfb, 0xd8, 0x98, 0xf2, 0xf0, 0xbf, 0x3c,
    0xbf, 0x34, 0xe2, 0x7d, 0x5c, 0xf8, 0xd0, 0xeb, 0xdf, 0x7a, 0x27, 0xde,
    0xc7, 0x85, 0xe0, 0xe4, 0x57, 0xf5, 0x56, 0x9b, 0x57, 0x5b, 0xd7, 0xf9,
    0xef, 0xfe, 0x79, 0x7f, 0x01, 0x08, 0x6c, 0x9e, 0x4f, 0xae, 0xe8, 0xf1,
    0xef, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60,
    0x82
};

void inline initSizePolicy( QWidget* w, QSizePolicy::Policy hp, QSizePolicy::Policy vp)
{
    QSizePolicy policy(hp,vp);
    
    policy.setHeightForWidth(w->sizePolicy().hasHeightForWidth());
    w->setSizePolicy(policy);
}

/*
 *  Constructs a hk_kdesubreportdialogbase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
hk_kdesubreportdialogbase::hk_kdesubreportdialogbase( QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    QImage img;
    img.loadFromData( image0_data, sizeof( image0_data ), "PNG" );
    image0 = QPixmap::fromImage(img);
    img.loadFromData( image1_data, sizeof( image1_data ), "PNG" );
    image1 = QPixmap::fromImage(img);
    setObjectName(QString::fromAscii(name == NULL?"hk_kdesubreportdialogbase":name));
    setModal(modal);
    setSizeGripEnabled(true);
    hk_kdesubreportdialogbaseLayout = new QGridLayout(this);
    hk_kdesubreportdialogbaseLayout->setMargin(11);
    hk_kdesubreportdialogbaseLayout->setSpacing(6);
    hk_kdesubreportdialogbaseLayout->setObjectName(QString::fromAscii("hk_kdesubreportdialogbaseLayout")); 

    Layout8 = new QVBoxLayout(static_cast<QWidget*>(NULL));
    Layout8->setMargin(0);
    Layout8->setSpacing(6);
    Layout8->setObjectName("Layout8");

    setbutton = new QPushButton(this);
    setbutton->setObjectName("setbutton");
    setbutton->setEnabled(false);
    setbutton->setAutoDefault(true);
    setbutton->setDefault(true);
    Layout8->addWidget( setbutton );

    deletebutton = new QPushButton(this);
    deletebutton->setObjectName("deletebutton");
    deletebutton->setEnabled(false);
    Layout8->addWidget( deletebutton );

    buttonCancel = new QPushButton(this);
    buttonCancel->setObjectName("buttonCancel");
    buttonCancel->setAutoDefault(true);
    Layout8->addWidget( buttonCancel );
    Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout8->addItem( Spacer1 );

    hk_kdesubreportdialogbaseLayout->addLayout( Layout8, 0, 1, 2, 1 );

    Frame15 = new QFrame(this);
    Frame15->setObjectName(QString::fromAscii("Frame15"));
    Frame15->setFrameShape( QFrame::Box );
    Frame15->setFrameShadow( QFrame::Sunken );
    Frame15Layout = new QHBoxLayout(Frame15);
    Frame15Layout->setMargin(11);
    Frame15Layout->setSpacing(6);
    Frame15Layout->setObjectName("Frame15Layout");

    subreportlabel = new QLabel(Frame15);
    subreportlabel->setObjectName("subreportlabel");
    Frame15Layout->addWidget( subreportlabel );

    subreportfield = new QComboBox(Frame15);
    subreportfield->setObjectName(QString::fromAscii("subreportfield"));
    Frame15Layout->addWidget( subreportfield );

    hk_kdesubreportdialogbaseLayout->addWidget( Frame15, 0, 0 );

    Frame3 = new QFrame(this);
    Frame3->setObjectName(QString::fromAscii("Frame3"));
    Frame3->setFrameShape( QFrame::Box );
    Frame3->setFrameShadow( QFrame::Sunken );
    Frame3Layout = new QHBoxLayout(Frame3);
    Frame3Layout->setMargin(11);
    Frame3Layout->setSpacing(6);
    Frame3Layout->setObjectName("Frame3Layout");

    Layout3 = new QGridLayout();
    Layout3->setMargin(0);
    Layout3->setSpacing(6);
    Layout3->setObjectName(QString::fromAscii("Layout3")); 

    deletefieldbutton = new QToolButton(Frame3);
    deletefieldbutton->setObjectName(QString::fromAscii("deletefieldbutton"));
    deletefieldbutton->setEnabled( false );
    initSizePolicy(deletefieldbutton, QSizePolicy::Fixed, QSizePolicy::Fixed);
    deletefieldbutton->setIcon( QIcon( image0 ) );

    Layout3->addWidget( deletefieldbutton, 1, 3 );
    Spacer2_2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout3->addItem( Spacer2_2, 3, 2 );

    mastertext = new QLabel(Frame3);
    mastertext->setObjectName("mastertext");

    Layout3->addWidget( mastertext, 0, 0 );

    subreporttext = new QLabel(Frame3);
    subreporttext->setObjectName("subreporttext");

    Layout3->addWidget( subreporttext, 1, 0 );

    thisfield = new QComboBox(Frame3);
    thisfield->setObjectName(QString::fromAscii("thisfield"));
    initSizePolicy(thisfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    thisfield->setMinimumSize( QSize( 90, 0 ) );
    thisfield->setEditable(true);
    thisfield->setInsertPolicy( QComboBox::NoInsert );
    thisfield->setAutoCompletion(true);

    Layout3->addWidget(thisfield, 1, 1, 1, 2 );

    addfieldbutton = new QToolButton(Frame3);
    addfieldbutton->setObjectName(QString::fromAscii("addfieldbutton"));
    addfieldbutton->setEnabled(false);
    initSizePolicy(addfieldbutton, QSizePolicy::Fixed, QSizePolicy::Fixed);
    addfieldbutton->setIcon( QIcon( image1 ) );

    Layout3->addWidget( addfieldbutton, 0, 3 );
    Spacer3 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout3->addItem( Spacer3, 3, 0 );
    Spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout3->addItem(Spacer2, 2, 3, 2, 1);

    printbeforefield = new QCheckBox(Frame3);
    printbeforefield->setObjectName(QString::fromAscii("printbeforefield"));

    Layout3->addWidget( printbeforefield, 2, 0, 1, 2 );

    masterfield = new QComboBox(Frame3);
    masterfield->setObjectName(QString::fromAscii("masterfield"));
    initSizePolicy(masterfield, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    masterfield->setMinimumSize( QSize( 130, 0 ) );
    masterfield->setEditable(true);
    masterfield->setInsertPolicy( QComboBox::NoInsert );
    masterfield->setAutoCompletion( true );

    Layout3->addWidget( masterfield, 0, 1, 1, 2 );
    Frame3Layout->addLayout( Layout3 );

    dependingfieldlist = new QTreeWidget( Frame3 );
    dependingfieldlist -> setObjectName( "dependingfieldlist" );
    initSizePolicy(dependingfieldlist, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    dependingfieldlist->setMinimumSize( QSize( 200, 0 ) );
    dependingfieldlist -> setSelectionMode(QAbstractItemView::SingleSelection);
    dependingfieldlist->setAllColumnsShowFocus( true );
    dependingfieldlist -> setRootIsDecorated(false);
    Frame3Layout->addWidget( dependingfieldlist );

    hk_kdesubreportdialogbaseLayout->addWidget( Frame3, 1, 0 );
    languageChange();
    resize( QSize(588, 270).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( addfieldbutton, SIGNAL( clicked() ), this, SLOT( add_dependingfields() ) );
    connect( deletefieldbutton, SIGNAL( clicked() ), this, SLOT( delete_dependingfields() ) );
    connect( dependingfieldlist, SIGNAL( itemSelectionChanged()) , this, SLOT( check_buttons() ) );
    connect( masterfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( check_buttons() ) );
    connect( masterfield, SIGNAL( activated(int) ), this, SLOT( check_buttons() ) );
    connect( thisfield, SIGNAL( editTextChanged(const QString&) ), this, SLOT( check_buttons() ) );
    connect( thisfield, SIGNAL( highlighted(int) ), this, SLOT( check_buttons() ) );
    connect( subreportfield, SIGNAL( activated(int) ), this, SLOT( subreport_selected() ) );
    connect( setbutton, SIGNAL( clicked() ), this, SLOT( set_subreport() ) );
    connect( deletebutton, SIGNAL( clicked() ), this, SLOT( delete_subreport() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
hk_kdesubreportdialogbase::~hk_kdesubreportdialogbase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void hk_kdesubreportdialogbase::languageChange()
{
    setWindowTitle( tr( "subreport dialog" ) );
    setbutton->setText( tr( "&Set" ) );
    deletebutton->setText( tr( "&Delete" ) );
    buttonCancel->setText( tr( "&Cancel" ) );
    subreportlabel->setText( tr( "subreport:" ) );
    deletefieldbutton->setText( QString::null );
    mastertext->setText( tr( "masterfield" ) );
    subreporttext->setText( tr( "subreportfield" ) );
    addfieldbutton->setText( QString::null );
    printbeforefield->setText( tr( "CheckBox1" ) );
}

void hk_kdesubreportdialogbase::add_dependingfields()
{
    qWarning( "hk_kdesubreportdialogbase::add_dependingfields(): Not implemented yet" );
}

void hk_kdesubreportdialogbase::check_buttons()
{
    qWarning( "hk_kdesubreportdialogbase::check_buttons(): Not implemented yet" );
}

void hk_kdesubreportdialogbase::delete_dependingfields()
{
    qWarning( "hk_kdesubreportdialogbase::delete_dependingfields(): Not implemented yet" );
}

void hk_kdesubreportdialogbase::delete_subreport()
{
    qWarning( "hk_kdesubreportdialogbase::delete_subreport(): Not implemented yet" );
}

void hk_kdesubreportdialogbase::set_subreportlist()
{
    qWarning( "hk_kdesubreportdialogbase::set_subreportlist(): Not implemented yet" );
}

void hk_kdesubreportdialogbase::set_subreport()
{
    qWarning( "hk_kdesubreportdialogbase::set_subreport(): Not implemented yet" );
}

void hk_kdesubreportdialogbase::subreport_selected()
{
    qWarning( "hk_kdesubreportdialogbase::subreport_selected(): Not implemented yet" );
}

