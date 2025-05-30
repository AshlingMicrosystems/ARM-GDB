/* Common target dependent code for GDB on MIPS systems running FreeBSD.

   Copyright (C) 2017-2025 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef GDB_MIPS_FBSD_TDEP_H
#define GDB_MIPS_FBSD_TDEP_H

void mips_fbsd_supply_fpregs (struct regcache *, int, const void *, size_t);
void mips_fbsd_supply_gregs (struct regcache *, int, const void *, size_t);
void mips_fbsd_collect_fpregs (const struct regcache *, int, void *, size_t);
void mips_fbsd_collect_gregs (const struct regcache *, int, void *, size_t);

#endif /* GDB_MIPS_FBSD_TDEP_H */
