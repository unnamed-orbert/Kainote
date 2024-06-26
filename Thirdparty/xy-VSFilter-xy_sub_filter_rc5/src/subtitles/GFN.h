/* 
 *	Copyright (C) 2003-2006 Gabest
 *	http://www.gabest.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#pragma once

#include <atlcoll.h>

// MPC-HC: namespace Subtitle::SubType
// note: names in G_EXTTYPESTR
enum exttype {EXTSRT = 0, EXTSUB, EXTSMI, EXTPSB, EXTSSA, EXTASS, EXTIDX, EXTUSF, EXTXSS, EXTTXT,
    EXTSSF, EXTRT, EXTSUP, EXTVTT};

extern TCHAR* G_EXTTYPESTR[];
typedef struct 
{
    CString full_file_name;
    CString extra_name;
    int ext_order;
    int path_order;
    /*exttype ext;*/
} SubFile;

extern void GetSubFileNames(CString fn, CAtlArray<CString>& paths, CString load_ext_list, CAtlArray<SubFile>& ret);