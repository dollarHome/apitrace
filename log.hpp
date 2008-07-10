/****************************************************************************
 *
 * Copyright 2008 Tungsten Graphics, Inc.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <windows.h>
#include <tchar.h>


namespace Log {

    void Open(const TCHAR *szName);
    void ReOpen(void);
    void Close(void);
    
    void Text(const char *text);
    void TextF(const char *format, ...);
    
    void BeginCall(const char *function);
    void EndCall(void);
    
    void BeginArg(const char *type, const char *name);
    void EndArg(void);

    void BeginReturn(const char *type);
    void EndReturn(void);

    void BeginElement(const char *type);
    void BeginElement(const char *type, const char *name);
    void EndElement(void);

    void BeginReference(const char *type, const void *addr);
    void EndReference(void);

    void DumpString(const char *str);
    
}

#endif /* _LOG_HPP_ */
