/* FuBaR 1.3.1X */
/* This code uses some hacked functins, mainly the original */
/* diku/merc/envy fight functions, but comes together to    */
/* create some very nice and very  unique code.             */
/* therefore, i give my thanks to the creators of merc and  */
/* envy, but my biggest thanks goes to the creators of diku */
/* without whom mudding would not be as popular as it is    */
/* today. thank you. Big thanks to project co-head Eleven   */
/************************************************************/
#define unix 1
#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void item_damage    args( ( CHAR_DATA *ch, int dam ) );
void make_corpse    args( ( CHAR_DATA *ch ) );
void bomb_explode args(( OBJ_DATA *bomb, ROOM_INDEX_DATA *in_room ));
void armbomb args (( CHAR_DATA *ch, char *argument ));
void damage_from_obj args(( OBJ_DATA *obj, CHAR_DATA *victim, int dam, int dt ));
void obj_kill args(( CHAR_DATA *victim ));
void bomb_room args(( OBJ_DATA *bomb, ROOM_INDEX_DATA *in_room ));

void bomb_explode( OBJ_DATA *bomb, ROOM_INDEX_DATA *in_room )
{
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;
    EXIT_DATA * pexit;
    ROOM_INDEX_DATA * room;
    ROOM_INDEX_DATA * first_room;
    int dir;
    int distance;
    int max_distance;
            AFFECT_DATA af;
   
      max_distance = bomb->value[2];

    for (dir = 0; dir < 6; dir++) /* look in every direction */
    {
       room = bomb->in_room;
        for (distance = 0 ; distance < max_distance; distance++)
        {
            pexit = room->exit[dir]; /* find the door to the next room */
            if ((pexit == NULL) || (pexit->to_room == NULL) ||
                (IS_SET(pexit->exit_info, EX_CLOSED)))
                break; /* exit not there OR points to nothing OR is closed */

			bomb_room( bomb, room );


  if ( bomb->in_room != NULL )
  {
   ROOM_INDEX_DATA *was_in_room;
   char	     *msg;
   int	     door;
	     
   msg = "The sound of a huge explosion shakes the ground you stand on.";

    was_in_room = bomb->in_room;

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = was_in_room->exit[door] )
	    && pexit->to_room
	    && pexit->to_room != was_in_room )
	{
	    bomb->in_room = pexit->to_room;
	    act(AT_BLOOD, msg, NULL, bomb, NULL, TO_ROOM );
	}
    }
    bomb->in_room = was_in_room;
   }
            room = pexit->to_room; /* go to the next room */
        } /* for distance */
    } /* for dir */
 

   extract_obj(bomb);  
   return;
}

void bomb_room( OBJ_DATA *bomb, ROOM_INDEX_DATA *in_room )
{
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *vch;
  CHAR_DATA *vch_next;

      for( vch = bomb->in_room->people; vch; vch = vch_next)
      {
       vch_next = vch->next_in_room;

       if ( vch == NULL )
        continue;
      
       if(vch->deleted)
        continue;

 
	if(bomb->value[1] == BOMB_NAPALM)
	{
	  sprintf( buf, 
      "%s explodes violently, sending flames in all directions!\n\r",
bomb->short_descr );
         send_to_char(AT_RED,buf,vch);
	if ( !IS_NPC( vch ) )
         damage_from_obj( bomb, vch,
         number_range ( bomb->value[3], bomb->value[3] * 2 ),
         TYPE_UNDEFINED  );
        else
         damage_from_obj( bomb, vch,
         number_range ( bomb->value[3]*4, bomb->value[3] * 8 ),
         TYPE_UNDEFINED );

                af.type      = gsn_incinerate;
                af.level     = bomb->level;
                af.duration  = bomb->level / 8;
                af.location  = APPLY_NONE;
                af.modifier  = 0;
                af.bitvector = AFF_FLAMING;
                affect_join( vch, &af );
                send_to_char(AT_RED, "Your body bursts into flame!\n\r", vch);

        }

	if(bomb->value[1] == BOMB_EXPLOSIVE)
	{
	  sprintf( buf, "The %s explodes violently!\n\r",
bomb->short_descr );
	send_to_char(AT_BLOOD,buf,vch);
	 if ( !IS_NPC( vch ) )
          damage_from_obj( bomb, vch,
          number_range(bomb->value[3], bomb->value[3] * 2), TYPE_UNDEFINED  );
         else
          damage_from_obj(bomb, vch,
          number_range (bomb->value[3]*4, bomb->value[3] * 8),
          TYPE_UNDEFINED  );

	}

	if(bomb->value[1] == BOMB_CHEMICAL)
	{
	  sprintf( buf, 
	"The %s explodes violently, filling the room with a green gas!",
bomb->short_descr );
	send_to_char(AT_GREEN,buf,vch);
        if ( !IS_NPC( vch ) )
         damage_from_obj( bomb, vch,
         number_range ( bomb->value[3], bomb->value[3] * 4),
         TYPE_UNDEFINED  );
        else
         damage_from_obj( bomb, vch,
         number_range ( bomb->value[3]*4, bomb->value[3] * 16),
         TYPE_UNDEFINED  );

	 send_to_char(AT_GREEN, "You choke and gag.\n\r", vch );
	 act(AT_GREEN, "$n chokes and gags.", vch, NULL, NULL, TO_ROOM );

	    af.type      = gsn_poison;
            af.level	 = bomb->level;
	    af.duration  = bomb->level;
	    af.location  = APPLY_STR;
	    af.modifier  = -2;
	    af.bitvector = AFF_POISON;
	    affect_join( vch, &af );

	}

      act( AT_WHITE, buf, vch, bomb, NULL, TO_ROOM );

return;
}

void do_armbomb( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *Obj;
  OBJ_DATA *timer;
  OBJ_DATA *pilewire;
  ROOM_INDEX_DATA *to_room;
  char buf[MAX_STRING_LENGTH];
  char arg[MAX_INPUT_LENGTH];
  int clock;

  one_argument( argument, arg );
  to_room = ch->in_room;
  clock = atoi(arg);

   if ( arg[0] == '\0' )
   {  send_to_char( C_DEFAULT, "You must specify a time in ticks.\n\r", ch );
   return;
   }

   if  ( !(Obj = get_eq_char( ch, WEAR_HOLD )) )
   {
      send_to_char( C_DEFAULT, "Gotta be holding it to set it.\n\r", ch );
      return;
   }

   if ( Obj->item_type != ITEM_BOMB )
   {
    send_to_char( C_DEFAULT, "You aren't holding an explosive.\n\r", ch );
    return;
   }

    if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE) )
    {
       send_to_char(AT_BLOOD, "You cannot set a bomb in a safe room.\n\r", ch );
       return;
    }


  if  ( clock < 1 )
  {
      send_to_char( C_DEFAULT, "You can't set a bomb for less than 1.\n\r", ch );
      return;
  }

  send_to_char( C_DEFAULT, "You set the timebomb, RUN!\n\r", ch );
	  sprintf( buf, 
      "%s drops a small device, which begins to making ticking noises.", ch->name );
	act( AT_WHITE, buf, ch, Obj, NULL, TO_ROOM );
  unequip_char( ch, Obj );
  Obj->timer = clock;
  obj_from_char( Obj );
  obj_to_room( Obj, to_room );
  Obj->owner = ch;
  return;
}

void damage_from_obj( OBJ_DATA *obj, CHAR_DATA *victim, int dam, int dt )
{

    CHAR_DATA *vch;
    CHAR_DATA *ch;
    const  char         *vp;
    char          buf          [ MAX_STRING_LENGTH ];

	 if ( dam <=   0 ) {	vp = "misses";		}
    else if ( dam <=   4 ) {	vp = "scratches";	}
    else if ( dam <=   8 ) {	vp = "grazes";		}
    else if ( dam <=  12 ) {	vp = "hits";		}
    else if ( dam <=  16 ) {	vp = "injures";		}
    else if ( dam <=  20 ) {	vp = "wounds";		}
    else if ( dam <=  24 ) {    vp = "mauls";		}
    else if ( dam <=  28 ) {	vp = "decimates";	}
    else if ( dam <=  32 ) {	vp = "devastates";	}
    else if ( dam <=  36 ) {	vp = "maims";		}
    else if ( dam <=  40 ) {	vp = "MUTILATES";	}
    else if ( dam <=  44 ) {	vp = "DISEMBOWELS";	}
    else if ( dam <=  48 ) {	vp = "EVISCERATES";	}
    else if ( dam <=  52 ) {	vp = "MASSACRES";	}
    else if ( dam <= 100 ) {    vp = "*** DEMOLISHES ***";			}
    else if ( dam <= 150 ) {    vp = "*** DEVASTATES ***";			}
    else if ( dam <= 250 ) {    vp = "*** OBLITERATES ***";		}
    else if ( dam <= 300 ) {    vp = "=== OBLITERATES ===";		}
    else if ( dam <= 500 ) {    vp = "*** ANNIHILATES ***";		}
    else if ( dam <= 750 ) {    vp = ">>> ANNIHILATES <<<";		}
    else if ( dam <= 1000) {    vp = "<<< ERADICATES >>>";			}
    else                   {    vp = "&Xdoes &rUNSPEAKABLE&X things to";
}

    if ( victim->position == POS_DEAD )
	return;


	if ( IS_AFFECTED( victim, AFF_SANCTUARY ) )
	    dam /= 2;
        if ( IS_AFFECTED2( victim, AFF_GOLDEN ) )
	    dam /= 4;
	if ( IS_AFFECTED2( victim, AFF_DANCING ) )
	    dam += dam/2;

	if ( IS_AFFECTED2( victim, AFF_INERTIAL ) )
	    dam -= dam / 8;
        if ( IS_SET( victim->act, UNDEAD_TYPE( victim ) ) )
            dam -= dam / 8;      

		if ( dam < 0 )
			dam = 0;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
	if ( dam > 0 )
    {
        sprintf( buf, "&WThe blast from &c%s %s &Wyou!! (%d)\n\r",
        obj->short_descr, vp, dam );
        send_to_char( C_DEFAULT, buf, victim );
	}

    if ( dam > 25 && number_range( 0, 100 ) <= 15 )
      item_damage(victim, dam);

    victim->hit -= dam;

  if ( victim->position == POS_DEAD )
	return;
       
    update_pos( victim );	

    switch( victim->position )
    {
    case POS_MORTAL:
	send_to_char(AT_RED, 
	    "You are mortally wounded, and will die soon, if not aided.\n\r",
	    victim );
	act(AT_RED, "$n is mortally wounded, and will die soon, if not aided.",
	    victim, NULL, NULL, TO_ROOM );
	break;

    case POS_INCAP:
	send_to_char(AT_RED,
	    "You are incapacitated and will slowly die, if not aided.\n\r",
	    victim );
	act(AT_RED, "$n is incapacitated and will slowly die, if not aided.",
	    victim, NULL, NULL, TO_ROOM );
	break;

    case POS_STUNNED:
	send_to_char(AT_WHITE,"You are stunned, but will probably recover.\n\r",
	    victim );
	act(AT_WHITE, "$n is stunned, but will probably recover.",
	    victim, NULL, NULL, TO_ROOM );
	break;

    case POS_DEAD:
	send_to_char(AT_BLOOD, "You have been KILLED!!\n\r\n\r", victim );
	act(AT_BLOOD, "$n is DEAD!!", victim, NULL, NULL, TO_ROOM );
	break;

    default:
	if ( dam > MAX_HIT(victim) / 4 )
	    send_to_char(AT_RED, "That really did HURT!\n\r", victim );
	if ( victim->hit < MAX_HIT(victim) / 4 )
	    send_to_char(AT_RED, "You sure are BLEEDING!\n\r", victim );
	break;
    }

    /*
     * Payoff for killing things.
     */

  if ( victim->position == POS_DEAD )
  {
    if ( !IS_ARENA(victim) || !IS_SET( victim->act2, PLR_WAR ))
    {                                                      
	  if ( !IS_NPC( victim ) )
	        info( "%s gets blown to bits by %s!", (int)victim->name,
                  (int)obj->short_descr );
	}
	else if (IS_SET( victim->act2, PLR_WAR ))
	{
	  char  buf[MAX_STRING_LENGTH];
	   sprintf(buf, "&C%s &c has become a casualty of war.",
victim->name );
	   info(buf, 0, 0 );
	   war.inwar--;
	    if (war.wartype == 1 && war.inwar == 1)
		{
          for ( vch = char_list; vch; vch = vch->next )
		  {
			if (IS_SET( vch->act2, PLR_WAR ))
				ch = vch;
		  }
		sprintf( buf, "&C%s &Rhas survived iron man war!",
ch->name );
	    info( buf, 0, 0 );
	    war.wartype = 0;
	    war.max_level = 0;
	    war.max_level = 0;
	    war.inwar = 0;
	    war.count = 0;
	    war.timeleft = 0;
        war.iswar = FALSE;
	    char_from_room(ch);
	    char_to_room(ch, get_room_index(ROOM_VNUM_TEMPLE));
	    }	 
	  }
	  else
	  {
      if ( victim == arena.cch )
		  ch = arena.och;
	  else
		  ch = arena.cch;
	  sprintf(log_buf, "&C%s &chas defeated &C%s &cin the arena!",
	          ch->name, victim->name);
	  wiznet(log_buf, NULL, NULL, WIZ_DEATHS, 0, 0);
	  log_string(log_buf, CHANNEL_LOG, -1);
	  challenge(log_buf, 0, 0);
	  ch->arenawon++;
	  victim->arenalost++;
	  }

	obj_kill( victim );

	/* Ok, now we want to remove the deleted flag from the
	 * PC victim.
	 */
	if ( !IS_NPC( victim ) )
	    victim->deleted = FALSE;

	return;
    }

    tail_chain( );
    return;
}

void obj_kill( CHAR_DATA *victim )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    bool is_arena = IS_ARENA(victim);
    
    stop_fighting( victim, TRUE );

  if (!IS_SET(victim->act2, PLR_WAR))
  {
        make_corpse( victim );
   }

    for ( paf = victim->affected; paf; paf = paf_next )
        {

     	 paf_next = paf->next;
         affect_remove( victim, paf );
        }
    for ( paf = victim->affected2; paf; paf = paf_next )
      {
       paf_next = paf->next;
       affect_remove2( victim, paf );
      }
    victim->affected_by	= 0;
    victim->affected_by2 = 0;
    if ( IS_NPC( victim ) )
    {
	victim->pIndexData->killed++;
	kill_table[URANGE( 0, victim->level, MAX_LEVEL-1 )].killed++;
	extract_char( victim, TRUE );
	return;
    }

  if ( IS_SET(victim->act2, PLR_WAR) ){
    /* newbie extraction here */
    char_from_room( victim );
    char_to_room( victim, get_room_index(ROOM_VNUM_MORGUE ));
	}else{
    extract_char( victim, FALSE );    
    }


    if ( !is_arena || !IS_SET( victim->act2, PLR_WAR ))
    {
      victim->armor        = 100;
      victim->hitroll      = 0;
      victim->damroll      = 0;
      victim->saving_throw = 0;
      victim->carry_weight = 0;
      victim->carry_number = 0;
    }
    victim->position     = POS_RESTING;
    victim->hit	         = UMAX( 1, victim->hit  );
    victim->mana         = UMAX( 1, victim->mana );
    victim->bp           = UMAX( 1, victim->bp   );
    victim->move         = UMAX( 1, victim->move );
    if (IS_SET(victim->act2, PLR_WAR))
    REMOVE_BIT(victim->act2, PLR_WAR);
    save_char_obj( victim, FALSE );
    return;
}
