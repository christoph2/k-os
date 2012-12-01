/*
 * k_dk - Driver Kit for k_os (Konnex Operating-System based on the
 * OSEK/VDX-Standard).
 *
 * (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
 *                                      cpu12.gems@googlemail.com>
 *
 * All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#if !defined(__OS_PORT_IPC_H)
#define __OS_PORT_IPC_H

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


/*
**  NamedPipes.
*/
typedef void * FAKE_HANDLE;

unsigned OsPort_NamedPipeOpen(FAKE_HANDLE * hServer, char * const name, char * const  path, char * const  host);
unsigned __int8 OsPort_NamedPipeWrite(FAKE_HANDLE hPipe, const unsigned __int8 * message, unsigned __int16 len);

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __OS_PORT_IPC_H */
