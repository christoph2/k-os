/*
 *   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
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

#include <windows.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "Os_Port_Win32.h"
#include "Os_Port_IPC.h"

#define NODE_ADDRESS    ((NodeType)0x55)

#define DISABLING_MESSAGE   "\nDisabling COM, Tracing and Remote-Coontrol.\n"
#define SERVER_ENDPOINT     "KOS\\SERVER"

#define E_OK            ((int)0x00)
#define E_NOT_OK        ((int)0xff)

typedef unsigned int NodeType;
typedef void * MessageType;


#define EXTENDED_FRAME      ((unsigned __int32)0x80000000UL)
#define MAX_LSDU_LEN        ((unsigned __int8)0x08)

#define MASK_STANDARD_ID    ((unsigned __int32)0x000007ff)
#define MASK_EXTENDED_ID    ((unsigned __int32)0x1fffffff)


typedef struct tagOs_Port_IPC_SimCANFrame {
    unsigned __int32    id;
    unsigned __int8     dlc;
    unsigned __int8     sdu[MAX_LSDU_LEN];
} Os_Port_IPC_SimCANFrame;


static HANDLE slot;
static HANDLE rcThread;

void RemoteControlThread(void);
void OsPort_StartRemoteControlThread(void);

unsigned __int8 OsPort_NamedPipeWrite(HANDLE hPipe, const unsigned __int8 * message, unsigned __int16 len);

static void getCanoicalName(const char * prefix,size_t len, char * const host,char * const path,char * const  name,char * canonicalName);

boolean WIPC_Register(HANDLE hPipe, NodeType Node);
boolean WIPC_Unregister(HANDLE hPipe, NodeType Node);
//int WIPC_Heartbeat(HANDLE hPipe, NodeType Node);
void WIPC_Listener(HANDLE hPipe, MessageType Message);


unsigned OsPort_NamedPipeOpen(FAKE_HANDLE * hServer, char * const name, char * const  path, char * const  host)
{
   char canonicalName[256];
   DWORD error;

   getCanoicalName("pipe", sizeof(canonicalName) - 1, host, path, name, canonicalName);

   *hServer = CreateFileA(&canonicalName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

   if (*hServer == INVALID_HANDLE_VALUE) {
        error = GetLastError();
        if (error == ERROR_FILE_NOT_FOUND) {
            printf("Could not open NamedPipe -- maybe K_OS-server not started?" DISABLING_MESSAGE);
        } else {
            printf("OsPort_NamedPipeOpen failed with: %u.\n", error);
        }
        return FALSE;
   } else {        
        return TRUE;
   }
}


unsigned __int8 OsPort_NamedPipeWrite(HANDLE hPipe, const unsigned __int8 * data, unsigned __int16 len)
{
    BOOL result;
    DWORD numBytesWritten;

    result = WriteFile(hPipe, (LPCVOID)data, (DWORD)len, &numBytesWritten, (LPOVERLAPPED)NULL); 
 
    if (!result)  {
        PrintErrorMsg("OsPort_MailslotCreate");
        return FALSE; 
    }  else {
        return TRUE;
    }
}


void OsPort_StartRemoteControlThread(void)
{
    DWORD ThreadId;

    rcThread = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)RemoteControlThread, (LPVOID)NULL, 0UL, &ThreadId);
}


void RemoteControlThread(void)
{
    while (TRUE) {
        Sleep(250);
//        printf("RC-Thread\n");
    }
}


static void getCanoicalName(const char * prefix, size_t len, char * const host,char * const path, char * const  name,char * canonicalName)
{
    sprintf_s(canonicalName, len, "\\\\%s\\%s\\%s%s",host, prefix, path, name);
}


void PrintErrorMsg(char *lpszFunction)
{ 
    /*
    **
    **  Quick n' dirty implementation - MUST BE SANITIZED!!!
    **
    */
    char * szBuf;
    LPWSTR  lpMsgBuf;
    WORD len, idx;
    DWORD err = GetLastError();

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );
   
    len = lstrlen(lpMsgBuf);
    szBuf = (char *)LocalAlloc(LHND, (len >> 1) + 1);
    for (idx = 0; idx < len * 2; ++idx) {
        szBuf[idx] = lpMsgBuf[idx];
    }
//_tprintf(szBuf, "%s failed with error %d: %s",  lpszFunction, err, szBuf); 
    printf("%s failed with error %d: %s",  lpszFunction, err, szBuf); 
 
    LocalFree(szBuf);
    LocalFree(lpMsgBuf);
}


int StartCOMExtension(void)
{
    int Status = E_NOT_OK;
    HANDLE pipeHandle;

    if (OsPort_NamedPipeOpen(&pipeHandle, SERVER_ENDPOINT, "", ".")) {
        if (!WIPC_Register(pipeHandle, NODE_ADDRESS)) {
                // TODO: ErrorHandling!!!
        } else {
            Status = E_OK;
        }
    } else {

    }
    return Status;
}


BOOL parseCommandLine(wchar_t * line,__int32 * NodeAddress)
{
    /* TODO: FIX-IT !!! (This crappy function is a good example why to use Python ...) */
    wchar_t * token;

    token = wcstok(line, L" ");  /* consume prog.-name. */
    token = wcstok(NULL, L" ");

    if (token != NULL) {
        *NodeAddress = _wtoi(token);
        if ((errno == ERANGE) || (errno == EINVAL)) {
            // "invalid numeric format. using default node-address"
            return FALSE;
        } else if (errno == (int)0) {
            // todo: "using node-address from command-line"
            return TRUE;
        } else {
            // "unknown error. using default node-address"
            return FALSE;
        }
    } else {
        return FALSE;
    }
    return FALSE;
}


#define UNUSED          ((unsigned __int8)0x00)
#define RESPONSE        ((unsigned __int8)0x80)
#define DIR             ((unsigned __int8)0x40)

/* 
**  Client --> Server Requests.
*/
#define REGISTER        ((unsigned __int8)0x00)
#define UNREGISTER      ((unsigned __int8)0x01)

#define BUS_REQ         ((unsigned __int8)0x10)

#define TRACE_RECORD                    ((unsigned __int8)0x20)
#define TRACE_RECORD_ENTER_SERVICE      ((unsigned __int8)0x21)
#define TRACE_RECORD_LEAVE_SERVICE      ((unsigned __int8)0x22)
#define TRACE_RECORD_CTX_SWITCH         ((unsigned __int8)0x23)

/* 
**  Server --> Client Requests.
*/
#define QUERY_INFO                      ((unsigned __int8)0x00) | DIR
#define QUERY_FILTERS                   ((unsigned __int8)0x01) | DIR

#define BUS_IND                         ((unsigned __int8)0x10) | DIR

#define MEMORY_READ                     ((unsigned __int8)0x20) | DIR
#define MEMORY_WRITE                    ((unsigned __int8)0x20) | DIR



#pragma pack(push)
#pragma pack(2)
typedef struct tagRegisterInformation {
    unsigned __int8 Command;       
    unsigned __int8 SubCommand;
    unsigned __int32 NodeAddress;
} RegisterInformation;

typedef struct tagQueryInformation {
    SYSTEMTIME Timestamp;
} QueryInformation;


typedef struct tagTraceRecord {
    SYSTEMTIME Timestamp;
} TraceRecord;

typedef struct tagTraceRecordEnterService {
    SYSTEMTIME Timestamp;
    unsigned __int8 ServiceID;
    void * Param1;
    void * Param2;
    void * Param3;
} TraceRecordEnterService;

typedef struct tagTraceRecordLeaveService {
    SYSTEMTIME Timestamp;
    unsigned __int8 ServiceID;
} TraceRecordLeaveService;

typedef struct tagTraceRecordCtxSwitch {
    SYSTEMTIME Timestamp;
    unsigned __int8 TaskID;
} TraceRecordCtxSwitch;

#pragma pack(pop)


boolean WIPC_Register(HANDLE hPipe, NodeType Node)
{
    __int32 NodeAddress;
    WCHAR * cmdLine = GetCommandLine();
    RegisterInformation ri;

    SYSTEMTIME tim;

    GetSystemTime(&tim);


    if (!parseCommandLine(cmdLine, &NodeAddress)) {
        printf("Uups, could not parse NodeAddress!!!\n");
    } else {
        printf("Node-Address: %u\n", NodeAddress);
    }

    ri.Command = REGISTER;
    ri.SubCommand = UNUSED;
    ri.NodeAddress = NodeAddress;

    if (!OsPort_NamedPipeWrite(hPipe, (unsigned __int8 *)&ri, sizeof(RegisterInformation))) {
        return FALSE;
    } else {
        return TRUE;
    }
}
