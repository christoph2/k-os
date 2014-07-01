/*
 *   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2013 by Christoph Schueler <github.com/Christoph2,
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
#include <malloc.h>
#include "Os_Port_Win32.h"
#include "Os_Port_IPC.h"

#if 0
    Protocol Structure:
    ===================
    Offset  Field       Len
    0       Length      2
    2       Command     1
    3       SubCommand  1
    4       Data bytes  Length - 2
    ===================
    Commands:
    ===================
    REGISTER
    UNREGISTER
#endif

#define NODE_ADDRESS    ((NodeType)0x742)

#define DISABLING_MESSAGE   "\nDisabling COM, Tracing and Remote-Control.\n"
#define SERVER_ENDPOINT     "KOS\\SERVER"

#define E_OK            ((int)0x00)
#define E_NOT_OK        ((int)0xff)

#define MSCAN_8_8_FILTERS(a, b, c, d, e, f, g, h)   {(unsigned __int8)(a), (unsigned __int8)(b), (unsigned __int8)(c), (unsigned __int8)(d), \
                                                     (unsigned __int8)(e), (unsigned __int8)(f), (unsigned __int8)(g), (unsigned __int8)(h)}

#define MSCAN_4_16_FILTERS(a, b, c, d)              {HIBYTE(a), LOBYTE(a), HIBYTE(b), LOBYTE(b), HIBYTE(c), \
                                                     LOBYTE(c), HIBYTE(d), LOBYTE(d)}

#define MSCAN_2_32_FILTERS(a, b)                    {HIBYTE(HIWORD(a)), LOBYTE(HIWORD(a)), HIBYTE(LOWORD(a)),                    \
                                                     LOBYTE(LOWORD(a)), HIBYTE(HIWORD(b)), LOBYTE(HIWORD(b)), HIBYTE(LOWORD(b)), \
                                                     LOBYTE(LOWORD(b))

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

HANDLE OsPort_CreateCRThread(void(__cdecl *  fun)(void *), void * params);
void OsPort_ExitCRThread(unsigned retval);

void __cdecl RemoteControlThread(void * param);

void OsPort_StartRemoteControlThread(void);

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

   *hServer = CreateFileA(&canonicalName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);

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


unsigned __int8 OsPort_NamedPipeWrite(FAKE_HANDLE hPipe, const unsigned __int8 * message, unsigned __int16 len)
{
    BOOL result;
    DWORD numBytesWritten;

    result = WriteFile(hPipe, (LPCVOID)message, (DWORD)len, &numBytesWritten, (LPOVERLAPPED)NULL); 
 
    if (!result)  {
        PrintErrorMsg("OsPort_NamedPipeWrite");
        return FALSE; 
    }  else {
        return TRUE;
    }
}

unsigned __int8 OsPort_NamedPipeRead(FAKE_HANDLE hPipe, const unsigned __int8 * message, unsigned __int16 len, DWORD * numBytesRead)
{
    BOOL result;

    result = ReadFile(hPipe, (LPCVOID)message, len, numBytesRead, (OVERLAPPED *)NULL);

    if (!result)  {
        PrintErrorMsg("OsPort_NamedPipeRead");
        return FALSE; 
    }  else {
        return TRUE;
    }
}

unsigned __int8 OsPort_TransactNamedPipe(FAKE_HANDLE hPipe, unsigned __int8 const * InBuffer, BYTE InBufferSize, unsigned __int8 const * OutBuffer, unsigned __int8 OutBufferSize, DWORD * BytesRead)
{
    BOOL success;
#if 0
    DWORD State, CurInstances, MaxCollectionCount, CollectDataTimeout;
    wchar_t UserName[16];

    State = PIPE_READMODE_MESSAGE | PIPE_WAIT;

    success = GetNamedPipeHandleState(hPipe, &State, &CurInstances, &MaxCollectionCount, &CollectDataTimeout, (LPWSTR)&UserName, 15);
    success = SetNamedPipeHandleState(hPipe, &State,  NULL, NULL);
    if (!success) {
        PrintErrorMsg("SetNamedPipeHandleState");
    }

    success = TransactNamedPipe(hPipe, (void*)InBuffer, (DWORD) InBufferSize, (void*)OutBuffer, (DWORD)OutBufferSize, BytesRead, (OVERLAPPED*)NULL);
#endif

    success = OsPort_NamedPipeWrite(hPipe, (const unsigned __int8 * )InBuffer, InBufferSize);
    if (!success) {
        PrintErrorMsg("OsPort_TransactNamedPipe [write]");
        return FALSE;
    }
    success = OsPort_NamedPipeRead(hPipe, (const unsigned __int8 * )OutBuffer, OutBufferSize, BytesRead);
    if (!success) {
        PrintErrorMsg("OsPort_TransactNamedPipe [read]");
        return FALSE;
    } else {
        return TRUE;
    }
}

HANDLE OsPort_GetCurrentThreadHandle(void)
{
    HANDLE resultHandle;  
 
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &resultHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);
    return resultHandle;
}

void OsPort_StartRemoteControlThread(void)
{
    DWORD ThreadId;

    rcThread = OsPort_CreateCRThread(RemoteControlThread, NULL);
}


void __cdecl RemoteControlThread(void * param)
{
    while (TRUE) {
        Sleep(250);
//        printf("RC-Thread\n");
    }
}


static void getCanoicalName(const char * prefix, size_t len, char * const host, char * const path, char * const  name, char * canonicalName)
{
    sprintf_s(canonicalName, len, "\\\\%s\\%s\\%s%s", host, prefix, path, name);
}


void PrintErrorMsg(char *lpszFunction)
{ 
    char * szBuf;
    LPWSTR  lpMsgBuf;
    WORD len, idx;
    DWORD err = GetLastError();

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );
   
    len = lstrlen(lpMsgBuf) + 1;
    szBuf = (char *)_alloca((len * sizeof(lpMsgBuf[0])));
    for (idx = 0; idx < len; ++idx) {
        szBuf[idx] = lpMsgBuf[idx];
    }
//_tprintf(szBuf, "%s failed with error %d: %s",  lpszFunction, err, szBuf); 
    printf("%s failed with error %d: %s",  lpszFunction, err, szBuf); 

    _freea(szBuf);
    LocalFree(lpMsgBuf);
}


int StartCOMExtension(void)
{
    int Status = E_NOT_OK;

    return E_OK;

#if 0
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
#endif
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
    unsigned char BusName[16];
} RegisterInformation;

typedef struct tagFilterInformation {
    unsigned __int8 Command;       
    unsigned __int8 SubCommand;
    
    unsigned __int8 FltMsk[8 * 8];
    unsigned __int8 FltAcc[8 * 8];
} FilterInformation;

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
    ri.NodeAddress = NODE_ADDRESS;  // TODO: Fix parsing logic.
    strncpy(ri.BusName, "CAN", 16);

    if (!OsPort_NamedPipeWrite(hPipe, (unsigned __int8 *)&ri, sizeof(RegisterInformation))) {
        return FALSE;
    } else {
        return TRUE;
    }
}
