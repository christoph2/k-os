/*
 * k_os (Konnex Operating-System based on the OSEK/VDX-Standard).
 *
 * (C) 2007-2009 by Christoph Schueler <chris@konnex-tools.de>
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

#include "Com_Flt.h"

/*
    COM_FILTER_ALWAYS,
    COM_FILTER_NEVER,
    COM_FILTER_MASKEDNEWEQUALSX,
    COM_FILTER_MASKEDNEWDIFFERSX,
    COM_FILTER_NEWISEQUAL,
    COM_FILTER_NEWISDIFFERENT,
    COM_FILTER_MASKEDNEWEQUALSMASKEDOLD,
    COM_FILTER_MASKEDNEWDIFFERSMASKEDOLD,
    COM_FILTER_NEWISWITHIN,
    COM_FILTER_NEWISOUTSIDE,
    COM_FILTER_NEWISGREATER,
    COM_FILTER_NEWISLESSOREQUAL,
    COM_FILTER_NEWISLESS,
    COM_FILTER_NEWISGREATEROREQUAL,
    COM_FILTER_ONEEVERYN
*/

typedef struct tagMsg_Vars {
    uint32 new_value;
    uint32 old_value;
    uint16 occurrence;
} Msg_Vars;

typedef struct tagMsg_Consts {
    uint32 mask;
    uint32 x;
    uint32 min;
    uint32 max;
    uint16 period;
    uint16 offset;
} Msg_Consts;

boolean ComFlt_Always(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_Never(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_MaskedNewEqualsX(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_MaskedNewDiffersX(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_NewIsEqual(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_NewIsDifferent(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_MaskedNewEqualsMaskedOld(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_MaskedNewDiffersMaskedOld(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_NewIsWithin(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_NewIsOutside(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_NewIsGreatear(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_NewIsLessOrEqual(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_NewIsLess(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_NewIsGreaterOrEqual(const Msg_Vars *vars,const Msg_Consts *consts);
boolean ComFlt_OneEveryN(const Msg_Vars *vars,const Msg_Consts *consts);


boolean ComFlt_Always(const Msg_Vars *vars,const Msg_Consts *consts)
{
    UNREFERENCED_PARAMETER(vars);
    UNREFERENCED_PARAMETER(consts);

    return TRUE;
}


boolean ComFlt_Never(const Msg_Vars *vars,const Msg_Consts *consts)
{
    UNREFERENCED_PARAMETER(vars);
    UNREFERENCED_PARAMETER(consts);

    return FALSE;
}

boolean ComFlt_MaskedNewEqualsX(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return ((vars->new_value & consts->mask)==consts->x);
}

boolean ComFlt_MaskedNewDiffersX(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return ((vars->new_value & consts->mask)!=consts->x);
}

boolean ComFlt_NewIsEqual(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return (vars->new_value==vars->old_value);
}

boolean ComFlt_NewIsDifferent(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return (vars->new_value!=vars->old_value);
}

boolean ComFlt_MaskedNewEqualsMaskedOld(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return ((vars->new_value & consts->mask)==(vars->old_value & consts->mask));
}

boolean ComFlt_MaskedNewDiffersMaskedOld(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return ((vars->new_value & consts->mask)!=(vars->old_value & consts->mask));
}

boolean ComFlt_NewIsWithin(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return ((vars->new_value>=consts->min) && (vars->new_value<=consts->max));
}

boolean ComFlt_NewIsOutside(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return ((consts->min>vars->new_value) || (vars->new_value>consts->max));
}

boolean ComFlt_NewIsGreatear(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return (vars->new_value>vars->old_value);
}

boolean ComFlt_NewIsLessOrEqual(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return (vars->new_value<=vars->old_value);
}

boolean ComFlt_NewIsLess(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return (vars->new_value<=vars->old_value);
}

boolean ComFlt_NewIsGreaterOrEqual(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return (vars->new_value>=vars->old_value);
}

boolean ComFlt_OneEveryN(const Msg_Vars *vars,const Msg_Consts *consts)
{
    return ((vars->occurrence % consts->period)==consts->offset);
}
