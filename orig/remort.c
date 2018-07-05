#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void do_rebirt( CHAR_DATA *ch, char *argument )
{
  send_to_char( C_DEFAULT,
      "If you want to rebirth, spell it out!\n\r", ch );
  return;
}

void do_rebirth( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA   *obj, *obj_next;
    RACE_DATA  *pRace;
    char       buf[MAX_STRING_LENGTH];
    int        iWear;

  if ( str_cmp(ch->desc->incomm, "rebirth yes") )
  {
    send_to_char( C_DEFAULT,
        "If you want to remort, type 'rebirth yes'\n\r", ch );
    return;
  }
    if ( ch->level < LEVEL_HERO )
    {
        sprintf( buf,
            "You must be level %d to remort.\n\r",
            LEVEL_HERO );
        send_to_char( AT_WHITE, buf, ch );
        return;
    }

    if (ch->level > LEVEL_HERO)
	{ send_to_char(C_DEFAULT, "Why the hell would you want to do that?\n\r", ch );
	return; }

    if ( ch->incarnations >= 2 )
    {
	send_to_char(C_DEFAULT, "You have been reborn max times.\n\r", ch );
	return;
    }

    do_astrip( ch, ch->name );

    for ( iWear = 0; iWear < MAX_WEAR; iWear ++ )  
        {

        if ( !( obj = get_eq_char( ch, iWear ) ) )
            continue;

              unequip_char( ch, obj );

		  extract_obj( obj );
        }
    
    for ( obj = ch->carrying; obj; obj = obj->next_content )
        { 
        obj_next = obj->next_content;
		extract_obj( obj );
        }

    /* reset misc */
    ch->pcdata->condition[COND_THIRST]  =  0;
    ch->pcdata->condition[COND_FULL]    =  0;
    ch->pcdata->condition[COND_DRUNK]    = 0;
    ch->saving_throw = 0;

    /* level one stats */
    ch->perm_hit  = 20;
    ch->mod_hit  = 0;
    ch->perm_mana = 100;
    ch->mod_mana = 0;
    ch->perm_bp   = 20;
    ch->mod_bp = 0;
    ch->perm_move = 100;
    ch->mod_move = 0;
    ch->level = 1;   
    ch->hitroll = 0;
    ch->damroll = 0;
    ch->armor = 100;
#ifdef NEW_MONEY
    ch->money.gold = ch->money.silver = ch->money.copper = 0;
#else
	ch->gold = 0;
#endif
    ch->exp = xp_tolvl( ch )/2;
    ch->raisepts = 0;

    /* reset stats to level 1*/
    pRace = get_race_data(ch->race);
    ch->pcdata->mod_str = pRace->mstr;
    ch->pcdata->mod_int = pRace->mint;
    ch->pcdata->mod_wis = pRace->mwis;
    ch->pcdata->mod_dex = pRace->mdex;
    ch->pcdata->mod_con = pRace->mcon;

    ch->pcdata->perm_str = 13; 
    ch->pcdata->perm_int = 13; 
    ch->pcdata->perm_wis = 13; 
    ch->pcdata->perm_dex = 13; 
    ch->pcdata->perm_con = 13; 

    switch ( class_table[prime_class(ch)].attr_prime )
    {
      case APPLY_STR: ch->pcdata->perm_str = 16; break;
      case APPLY_INT: ch->pcdata->perm_int = 16; break;
      case APPLY_WIS: ch->pcdata->perm_wis = 16; break;
      case APPLY_DEX: ch->pcdata->perm_dex = 16; break;
      case APPLY_CON: ch->pcdata->perm_con = 16; break;
    } 

   /* restore */
   ch->hit      = MAX_HIT(ch);
   ch->mana     = MAX_MANA(ch);
   ch->bp       = MAX_BP(ch);
   ch->move     = MAX_MOVE(ch);
   SET_BIT    ( ch->act2, PLR_REMORT ); 
   ch->incarnations = ++ch->incarnations;
   /* save the character */
   save_char_obj( ch, FALSE );

   /* completed message */
   send_to_char( AT_WHITE, "You have been reborn. You are level one again.\n\r", ch );
}

