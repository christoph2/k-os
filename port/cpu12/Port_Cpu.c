
/*
**
**  todo: Anpassen!!!
**
*/

#include "InstallIsr/ISR.h"

#if defined(__CSMC__)               /* Cosmic               */
    #define II_INC_COMP_C             "./cosmic/ISR_CSMC.c"
#elif defined(__GNUC__)             /* GNU GCC              */
    #define II_INC_COMP_C             "./gcc/ISR_GCC.c"
#elif defined(__HIWARE__)           /* Metrowerks/Freescale */
    #define II_INC_COMP_C             "./metrowerks/ISR_MW.c"
#elif defined(__IAR_SYSTEMS_ICC__)  /* IAR Systems          */
    #define II_INC_COMP_C             "./iar/ISR_IAR.c"
#elif defined(__IMAGECRAFT__)       /* Imagecraft           */
    #define II_INC_COMP_C             "./imagecraft/ISR_ICC.c"
#else                               /* todo: Add Support    */
    #error Unsupported Compiler.
#endif

#include II_INC_COMP_C
