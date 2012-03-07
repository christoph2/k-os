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
static boolean ComFlt_Always(const Msg_Vars * vars, const Msg_Consts * consts)
{
    UNREFERENCED_PARAMETER(vars);
    UNREFERENCED_PARAMETER(consts);

    return TRUE;
}


static boolean ComFlt_Never(const Msg_Vars * vars, const Msg_Consts * consts)
{
    UNREFERENCED_PARAMETER(vars);
    UNREFERENCED_PARAMETER(consts);

    return FALSE;
}


static boolean ComFlt_MaskedNewEqualsX(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return (vars->new_value & consts->mask) == consts->x;
}


static boolean ComFlt_MaskedNewDiffersX(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return (vars->new_value & consts->mask) != consts->x;
}


static boolean ComFlt_NewIsEqual(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return vars->new_value == vars->old_value;
}


static boolean ComFlt_NewIsDifferent(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return vars->new_value != vars->old_value;
}


static boolean ComFlt_MaskedNewEqualsMaskedOld(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return (vars->new_value & consts->mask) == (vars->old_value & consts->mask);
}


static boolean ComFlt_MaskedNewDiffersMaskedOld(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return (vars->new_value & consts->mask) != (vars->old_value & consts->mask);
}


static boolean ComFlt_NewIsWithin(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return (vars->new_value >= consts->min) && (vars->new_value <= consts->max);
}


static boolean ComFlt_NewIsOutside(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return (consts->min > vars->new_value) || (vars->new_value > consts->max);
}


static boolean ComFlt_NewIsGreater(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return vars->new_value > vars->old_value;
}


static boolean ComFlt_NewIsLessOrEqual(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return vars->new_value <= vars->old_value;
}


static boolean ComFlt_NewIsLess(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return vars->new_value <= vars->old_value;
}


static boolean ComFlt_NewIsGreaterOrEqual(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return vars->new_value >= vars->old_value;
}


static boolean ComFlt_OneEveryN(const Msg_Vars * vars, const Msg_Consts * consts)
{
    return (vars->occurrence % consts->period) == consts->offset;
}

#if KOS_MEMORY_MAPPING == STD_ON
    #define OSEK_COM_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KOS_MEMORY_MAPPING */
