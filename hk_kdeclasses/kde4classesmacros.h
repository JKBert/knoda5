// ****************************************************************************
// copyright (c) 2014 Patrik Hanak <hanakp@users.sourceforge.net>
// This file is part of the hk_kde4classes library.
//
// This file may be distributed and/or modified under the terms of the
// GNU Library Public License version 2 as published by the Free Software
// Foundation and appearing in the file LGPL included in the
// packaging of this file.
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#ifndef KDECLASSES_MACROS_H    
#define KDECLASSES_MACROS_H

#include <kdemacros.h>

#ifndef KDECLASSES_EXPORT

# if defined(MAKE_KDECLASSES_LIB)
   /* Building this library */ 
#  define KDECLASSES_EXPORT KDE_EXPORT
# else
   /* Using this library */ 
#  define KDECLASSES_EXPORT KDE_IMPORT
# endif
#endif

#endif
