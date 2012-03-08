/*
   k_os (Konnex Operating-System based on the OSEK/VDX-Standard).

   (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
                                        cpu12.gems@googlemail.com>

   All Rights Reserved

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

   s. FLOSS-EXCEPTION.txt
 */
#include "Com_Flt.h"

/*
**  Local Types.
*/
typedef struct tagMsg_Vars {
    uint32  new_value;
    uint32  old_value;
    uint16  occurrence;
} Msg_Vars;

typedef struct tagMsg_Consts {
    uint32  mask;
    uint32  x;
    uint32  min;
    uint32  max;
    uint16  period;
    uint16  offset;
} Msg_Consts;

typedef boolean (*ComFlt_FilterFunction)(const Msg_Vars * vars, const Msg_Consts * consts);

/*
**  Local Function Prototypes.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_Always(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_Never(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_MaskedNewEqualsX(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_MaskedNewDiffersX(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsEqual(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsDifferent(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_MaskedNewEqualsMaskedOld(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_MaskedNewDiffersMaskedOld(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsWithin(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsOutside(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsGreater(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsLessOrEqual(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsLess(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsGreaterOrEqual(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_OneEveryN(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
);
#else
static boolean  ComFlt_Always(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_Never(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_MaskedNewEqualsX(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_MaskedNewDiffersX(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_NewIsEqual(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_NewIsDifferent(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_MaskedNewEqualsMaskedOld(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_MaskedNewDiffersMaskedOld(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_NewIsWithin(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_NewIsOutside(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_NewIsGreater(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_NewIsLessOrEqual(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_NewIsLess(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_NewIsGreaterOrEqual(const Msg_Vars * vars, const Msg_Consts * consts);
static boolean  ComFlt_OneEveryN(const Msg_Vars * vars, const Msg_Consts * consts);
#endif /* KOS_MEMORY_MAPPING */

/*
**  Local Constants.
*/
static const ComFlt_FilterFunction ComFlt_FilterMapping[] = {
    ComFlt_Always,                      /*  COM_FILTER_ALWAYS			    */
    ComFlt_Never,                       /*  COM_FILTER_NEVER			    */
    ComFlt_MaskedNewEqualsX,            /*  COM_FILTER_MASKEDNEWEQUALSX		    */
    ComFlt_MaskedNewDiffersX,           /*  COM_FILTER_MASKEDNEWDIFFERSX	    */
    ComFlt_NewIsEqual,                  /*  COM_FILTER_NEWISEQUAL		    */
    ComFlt_NewIsDifferent,              /*  COM_FILTER_NEWISDIFFERENT		    */
    ComFlt_MaskedNewEqualsMaskedOld,    /*  COM_FILTER_MASKEDNEWEQUALSMASKEDOLD	    */
    ComFlt_MaskedNewDiffersMaskedOld,   /*  COM_FILTER_MASKEDNEWDIFFERSMASKEDOLD    */
    ComFlt_NewIsWithin,                 /*  COM_FILTER_NEWISWITHIN		    */
    ComFlt_NewIsOutside,                /*  COM_FILTER_NEWISOUTSIDE		    */
    ComFlt_NewIsGreater,                /*  COM_FILTER_NEWISGREATER		    */
    ComFlt_NewIsLessOrEqual,            /*  COM_FILTER_NEWISLESSOREQUAL		    */
    ComFlt_NewIsLess,                   /*  COM_FILTER_NEWISLESS		    */
    ComFlt_NewIsGreaterOrEqual,         /*  COM_FILTER_NEWISGREATEROREQUAL	    */
    ComFlt_OneEveryN,                   /*  COM_FILTER_ONEEVERYN		    */
};


#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_COM_START_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */

/*
**  Local Functions.
*/
#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_Always(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_Always(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    UNREFERENCED_PARAMETER(vars);
    UNREFERENCED_PARAMETER(consts);

    return TRUE;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_Never(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_Never(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    UNREFERENCED_PARAMETER(vars);
    UNREFERENCED_PARAMETER(consts);

    return FALSE;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_MaskedNewEqualsX(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_MaskedNewEqualsX(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return (vars->new_value & consts->mask) == consts->x;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_MaskedNewDiffersX(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_MaskedNewDiffersX(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return (vars->new_value & consts->mask) != consts->x;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsEqual(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_NewIsEqual(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return vars->new_value == vars->old_value;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsDifferent(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_NewIsDifferent(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return vars->new_value != vars->old_value;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_MaskedNewEqualsMaskedOld(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_MaskedNewEqualsMaskedOld(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return (vars->new_value & consts->mask) == (vars->old_value & consts->mask);
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_MaskedNewDiffersMaskedOld(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_MaskedNewDiffersMaskedOld(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return (vars->new_value & consts->mask) != (vars->old_value & consts->mask);
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsWithin(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_NewIsWithin(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return (vars->new_value >= consts->min) && (vars->new_value <= consts->max);
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsOutside(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_NewIsOutside(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return (consts->min > vars->new_value) || (vars->new_value > consts->max);
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsGreater(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_NewIsGreater(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return vars->new_value > vars->old_value;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsLessOrEqual(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_NewIsLessOrEqual(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return vars->new_value <= vars->old_value;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsLess(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_NewIsLess(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return vars->new_value <= vars->old_value;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_NewIsGreaterOrEqual(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_NewIsGreaterOrEqual(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return vars->new_value >= vars->old_value;
}

#if KOS_MEMORY_MAPPING == STD_ON
STATIC FUNC(boolean, OSEK_COM_CODE) ComFlt_OneEveryN(
    P2CONST(Msg_Vars, AUTOMATIC, OSEK_COM_APPL_DATA) vars,
    P2CONST(Msg_Consts, AUTOMATIC, OSEK_COM_APPL_DATA) consts
)
#else
static boolean ComFlt_OneEveryN(const Msg_Vars * vars, const Msg_Consts * consts)
#endif /* KOS_MEMORY_MAPPING */
{
    return (vars->occurrence % consts->period) == consts->offset;
}

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_COM_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
