/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  EotS copyright (C) 1999 by Sledge, Kjodo, Torann			   *
 ***************************************************************************/


#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"





void do_skin( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *item;
    int ii;
    bool skinned;

    if ( (obj = get_obj_here( ch, argument ) ) == NULL )
    {
        send_to_char(AT_YELLOW, "Skin what?\n\r", ch );
        return;
    }

    if ( obj->item_type != ITEM_CORPSE_NPC )
    {
        send_to_char(AT_YELLOW, "How do you propose skinning that?\n\r", ch );
        return;
    }

    /* send_to_char("Need test for skinning object here.\n\r", ch ); */

    while ( obj->contains != NULL )
    {
        item = obj->contains;

        obj_from_obj( item );
        obj_to_room( item, ch->in_room );
    }

    skinned = FALSE;
    for ( ii = 0; ii < 4; ii++ )
    {

        if ( (pObjIndex = get_obj_index( obj->value[ii])) == NULL )
            continue;

        item = create_object( pObjIndex, ch->level);

        obj_to_room( item, ch->in_room );
        skinned = TRUE;
    }

    if ( skinned == TRUE )
    {
        act(AT_YELLOW, "$n skins $p, removing all useful parts.", ch, NULL, obj, TO_NOTVICT );
        send_to_char(AT_YELLOW, "You successfully skin the corpse.\n\r", ch );
    }
    else
    {
        act(AT_YELLOW, "$n attempts to skin $p, but produces nothing of value.", ch, NULL, obj, TO_NOTVICT );
        send_to_char(AT_YELLOW, "You hack the corpse into useless pieces.\n\r", ch );
    }

    extract_obj( obj );

    return;
}


