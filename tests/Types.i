%{
#include <stdint.h>
%}

%include <stdint.i>

typedef unsigned char boolean;

typedef /*@signed-integral-type@*/ int8_t       sint8;
typedef /*@unsigned-integral-type@*/ uint8_t    uint8;
typedef /*@signed-integral-type@*/ int16_t      sint16;
typedef /*@unsigned-integral-type@*/ uint16_t   uint16;
typedef /*@signed-integral-type@*/ int32_t      sint32;
typedef /*@unsigned-integral-type@*/ uint32_t   uint32;

typedef /*@signed-integral-type@*/ int_least8_t     sint8_least;
typedef /*@unsigned-integral-type@*/ uint_least8_t  uint8_least;
typedef /*@signed-integral-type@*/ int_least16_t    sint16_least;
typedef /*@unsigned-integral-type@*/ uint_least16_t uint16_least;
typedef /*@signed-integral-type@*/ int_least32_t    sint32_least;
typedef /*@unsigned-integral-type@*/ uint_least32_t uint32_least;

typedef double float64;

typedef uint8_t uint8;
typedef uint16_t uint16;

%include "carrays.i"
%array_class(unsigned char, byteArray);
%array_class(unsigned short, codeArray);


typedef enum tagStatusType {
    E_OK,

    E_OS_ACCESS,
    E_OS_CALLEVEL,
    E_OS_ID,
    E_OS_LIMIT,
    E_OS_NOFUNC,
    E_OS_RESOURCE,
    E_OS_STATE,
    E_OS_VALUE,

    E_OS_SYS_ASSERTION_FAILED,

    E_OS_SERVICEID = E_AS,
    E_OS_ILLEGAL_ADDRESS,
    E_OS_MISSINGEND,
    E_OS_DISABLEDINT,
    E_OS_STACKFAULT,
    E_OS_PROTECTION_MEMORY,
    E_OS_PROTECTION_TIME,
    E_OS_PROTECTION_ARRIVAL,
    E_OS_PROTECTION_LOCKED,
    E_OS_PROTECTION_EXCEPTION,
    
    E_COM_ID = E_COM,   
    E_COM_LENGTH, 
    E_COM_LIMIT,
    E_COM_NOMSG,

    E_COM_SYS_UNINIT   = E_COM_SYS,
    E_NOT_OK           = 0xff
} StatusType;

typedef uint16 TickType;
                        
typedef TickType * TickRefType;

typedef struct tagAlarmBaseType {
    TickType    maxallowedvalue; 
    TickType    ticksperbase;    
    TickType    mincycle;        
} AlarmBaseType, * AlarmBaseRefType, CtrInfoType, * CtrInfoRefType;

typedef uint8           CounterType;
typedef CounterType *   CounterRefType;

typedef uint8           EventType;
typedef uint8           EventMaskType;
typedef EventMaskType * EventMaskRefType;

typedef uint8           ResourceType;
typedef ResourceType *  ResourceRefType;

typedef enum tagTaskStateType {
    SUSPENDED,
    READY,
    RUNNING,
    WAITING
} TaskStateType;

typedef TaskStateType * TaskStateRefType;

typedef uint8 TaskType;
typedef TaskType * TaskRefType;

%{

%}


