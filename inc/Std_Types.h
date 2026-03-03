/*
 * Self-contained Std_Types for k_os.
 */
#ifndef __STD_TYPES_H
#define __STD_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "Platform_Types.h"

#define STD_HIGH   0x01u
#define STD_LOW    0x00u

#define STD_ACTIVE 0x01u
#define STD_IDLE   0x00u

#define STD_ON  0x01u
#define STD_OFF 0x00u

typedef uint8_t Std_ReturnType;

typedef struct tagStd_VersionInfoType {
    uint16_t vendorID;
    uint16_t moduleID;
    uint8_t  instanceID;
    uint8_t  sw_major_version;
    uint8_t  sw_minor_version;
    uint8_t  sw_patch_version;
} Std_VersionInfoType;

typedef enum tagStd_LevelType { Std_Low, Std_High } Std_LevelType;

#endif /* __STD_TYPES_H */
