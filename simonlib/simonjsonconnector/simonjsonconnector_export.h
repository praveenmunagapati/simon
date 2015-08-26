/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_SIMONJSONCONNECTOR_EXPORT_H_F1032DFE61FA42E3B31933515AF33099
#define SIMON_SIMONJSONCONNECTOR_EXPORT_H_F1032DFE61FA42E3B31933515AF33099

// needed for KDE_EXPORT and KDE_IMPORT macros
#include <KF5/KDELibs4Support/kdemacros.h>

#ifndef SIMONJSONCONNECTOR_EXPORT
# if defined(MAKE_SIMONJSONCONNECTOR_LIB)
// We are building this library
#  define SIMONJSONCONNECTOR_EXPORT KDE_EXPORT
# else
// We are using this library
#  define SIMONJSONCONNECTOR_EXPORT
# endif
#endif

# ifndef SIMONJSONCONNECTOR_EXPORT_DEPRECATED
#  define SIMONJSONCONNECTOR_EXPORT_DEPRECATED KDE_DEPRECATED SIMONJSONCONNECTOR_EXPORT
# endif
#endif
