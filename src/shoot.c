#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void    death_xp_loss        args( ( CHAR_DATA *victim ) );
void    group_gain           args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    check_dodge          args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    check_killer         args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    dam_message          args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
                                    int dt ) );
void    set_fighting         args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    item_damage             args( ( CHAR_DATA *ch, int dam ) );
void    ranged_damage	args(( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ));

int per_type args((CHAR_DATA *ch, OBJ_DATA *Obj));

void ranged_hit args((CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *arrow,
int dam));

void do_shoot( CHAR_DATA *ch, char *argument )
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  char arg3[MAX_INPUT_LENGTH];
  char buf[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  ROOM_INDEX_DATA *to_room;
  ROOM_INDEX_DATA *in_room;
  OBJ_DATA *Obj;
  EXIT_DATA *pexit;
  int dir = 0;
  int dist = 0;
  int MAX_DIST = 5;
  extern char *dir_noun [];
  OBJ_DATA *bow;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );
  argument = one_argument( argument, arg3 );

  if (!can_use_skpell( ch, gsn_archery ) )
	{ send_to_char(C_DEFAULT, "Maybe you should learn to use a bow.\n\r", ch );
  	return;
	}
  if ( arg1[0] == '\0' )
  {
    send_to_char( C_DEFAULT, "What do you intend on shooting?\n\r", ch);
    return;
  }

  if ( ( Obj = get_obj_carry( ch, arg1 ) ) == NULL )
  {
    send_to_char( C_DEFAULT,
		 "You are not carrying that item.\n\r", ch );
    return;
  }

  if (!(IS_SET(Obj->extra_flags, ITEM_AMMUNITION)) )
  {
    send_to_char( C_DEFAULT,
		 "You cannot shoot that item.\n\r", ch );
    return;
  }

  if (IS_SET( Obj->extra_flags, ITEM_NODROP ) )
  {
    send_to_char( C_DEFAULT, "You can't let go of it!\n\r", ch );
    return;
  }

  if ( ( bow = get_eq_char( ch, WEAR_HOLD ) ) == NULL
	|| bow->item_type != ITEM_RANGED_WEAPON )
    {
	send_to_char(AT_BLUE,"You aren't holding a ranged weapon.\n\r", ch
);
	return;
    }



  in_room = ch->in_room;
  to_room = ch->in_room;

  if ( ( victim = ch->fighting ) == NULL )
  {
    if ( arg2[0] == '\0' )
    {
      send_to_char( C_DEFAULT, "Shoot at whom?\n\r", ch );
      return;
    }

    if ( arg3[0] == '\0' )
    {
      if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
      {
	send_to_char( C_DEFAULT, "They aren't here.\n\r", ch );
	return;
      }
    }
    else
    {
      if ( get_curr_dex( ch ) >= 20 )
      {
        MAX_DIST = 7;
        if ( get_curr_dex( ch ) == 25 )
          MAX_DIST = 8;
      }
      
      for ( dir = 0; dir < 6; dir++ )
	if ( arg2[0] == dir_name[dir][0] && !str_prefix( arg2,
							 dir_name[dir] ) )
	  break;

      if ( dir == 6 )
      {
	send_to_char( C_DEFAULT, "Shoot in which direction?\n\r", ch );
	return;
      }

      if ( ( pexit = to_room->exit[dir] ) == NULL ||
	   ( to_room = pexit->to_room ) == NULL )

      {
	send_to_char( C_DEFAULT, "You cannot shoot in that direction.\n\r",
		     ch );
	return;
      }

      if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
      {
	send_to_char( C_DEFAULT, "You cannot shoot through a door.\n\r",
ch );
	return;
      }

      if ( IS_SET( pexit->exit_info, EX_ISWALL ) )
      {
        send_to_char( C_DEFAULT, "You cannot shoot through a wall.\n\r",
ch );
        return;
      }  

    if ( number_percent() > ch->pcdata->learned[gsn_archery] +
(get_curr_dex( ch ) / 2)) {
        send_to_char(AT_RED,"You fumble your bow!\n\r", ch );
        return;
        }

      for ( dist = 1; dist <= MAX_DIST; dist++ )
      {
	char_from_room( ch );
	char_to_room( ch, to_room );
	if ( ( victim = get_char_room( ch, arg3 ) ) != NULL )
	  break;

	if ( ( pexit = to_room->exit[dir] ) == NULL ||
	     ( to_room = pexit->to_room ) == NULL ||
	       IS_SET( pexit->exit_info, EX_CLOSED ) ||
               IS_SET( pexit->exit_info, EX_ISWALL) )
	{
	  sprintf( buf, "A $p flys in from $T and hits the %s wall.", 
		   dir_name[dir] );
	  act( AT_WHITE, buf, ch, Obj, dir_noun[rev_dir[dir]], TO_ROOM );
	  sprintf( buf, "You shoot your $p %d room%s $T, where it hits a
wall.",
		   dist, dist > 1 ? "s" : "" );
	  act( AT_WHITE, buf, ch, Obj, dir_name[dir], TO_CHAR );
	  char_from_room( ch );
	  char_to_room( ch, in_room );
	  oprog_throw_trigger( Obj, ch );
	  obj_from_char( Obj );
	  obj_to_room( Obj, to_room );
	  return;
	}
      }

      if ( victim == NULL )
      {
	act( AT_WHITE, 
	    "A $p flies in from $T and falls harmlessly to the ground.",
	    ch, Obj, dir_noun[rev_dir[dir]], TO_ROOM );
	sprintf( buf,
		"Your $p falls harmlessly to the ground %d room%s $T of
here.",
		dist, dist > 1 ? "s" : "" );
	act( AT_WHITE, buf, ch, Obj, dir_name[dir], TO_CHAR );
	char_from_room( ch );
	char_to_room( ch, in_room );
	oprog_throw_trigger( Obj, ch );
	obj_from_char( Obj );
	obj_to_room( Obj, to_room );
	return;
      }
    }
    if ( dist > 0 )
    {
      char_from_room( ch );
      char_to_room( ch, in_room );
      act( AT_WHITE, "A $p flys in from $T and hits $n!", victim, Obj,
	  dir_noun[rev_dir[dir]], TO_NOTVICT );
      act( AT_WHITE, "A $p flys in from $T and hits you!", victim, Obj,
	  dir_noun[rev_dir[dir]], TO_CHAR );
      sprintf( buf, "Your $p flew %d rooms %s and hit $N!", dist,
	      dir_name[dir] );
      act( AT_WHITE, buf, ch, Obj, victim, TO_CHAR );
      oprog_throw_trigger( Obj, ch );
      obj_from_char( Obj );
      obj_to_room( Obj, to_room );
      ranged_damage( ch, victim, per_type( ch, Obj ), gsn_archery );

 
      update_skpell( ch, gsn_archery );

      if ( IS_NPC( victim ) )
      {
         if ( victim->level > 3 )
             victim->hunting = ch;
      }  
      return;
    }
  }
  obj_from_char( Obj );
  obj_to_room( Obj, to_room );
  act( AT_WHITE, "$n shoots a $p at $N!", ch, Obj, victim, TO_ROOM );
  act( AT_WHITE, "You shoot your $p at $N.", ch, Obj, victim, TO_CHAR );
  oprog_throw_trigger( Obj, ch );
  ranged_damage( ch, victim, per_type( ch, Obj ), gsn_archery );
  ranged_hit( ch, victim, Obj, TYPE_UNDEFINED );
  /* put ranged_hit statement here */
  return;
}

void ranged_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{

    if ( victim->position == POS_DEAD )
        return;

    /*
     * Stop up any residual loopholes.
     */
    if ( dam > 3500 )
    {
        char buf [ MAX_STRING_LENGTH ];

      if ( dt != 91 && ch->level <= LEVEL_HERO
      && dt != 40 )
      {
        if ( IS_NPC( ch ) && ch->desc && ch->desc->original )
            sprintf( buf,
                    "Damage: %d from %s by %s: > 3500 points with %d dt!",
                    dam, ch->name, ch->desc->original->name, dt );
        else
            sprintf( buf,
                    "Damage: %d from %s: > 3500 points with %d dt!",
                    dam, ch->name, dt );

        bug( buf, 0 );
      }
    }

    if ( victim != ch )
    {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
        if ( is_safe( ch, victim ) )
            return;
        check_killer( ch, victim );


        /*
         * More charm stuff.
         */
        if ( victim->master == ch )
            stop_follower( victim );

        /*
         * Inviso attacks ... not.
        */
        if ( IS_AFFECTED( ch, AFF_INVISIBLE ) )
        {
            affect_strip( ch, gsn_invis      );
            affect_strip( ch, gsn_mass_invis );
            REMOVE_BIT( ch->affected_by, AFF_INVISIBLE );
            act(AT_GREY, "$n fades into existence.", ch, NULL, NULL, TO_ROOM );
        }
        if (IS_AFFECTED2( ch, AFF_PHASED ) )
        {
            affect_strip ( ch, skill_lookup("phase shift") );
            affect_strip ( ch, skill_lookup("mist form") );
            REMOVE_BIT( ch->affected_by2, AFF_PHASED );
            act(AT_GREY, "$n returns from an alternate plane.", ch, NULL, NULL, TO_ROOM );
        }
        /*
         * Damage modifiers.
         */
        if ( ch->race == RACE_OGRE )
            dam -= dam / 20;
/*
    if ( ch->level < L_APP && ch->class == CLASS_VAMPIRE )
     if ( !IS_SET( ch->in_room->room_flags, ROOM_INDOORS ) )
     {
      if ( time_info.hour > 6 && time_info.hour < 18 )
      {
       send_to_char( AT_RED,
       "The sunlight has weakened your attack.\n\r", ch );
       dam -= dam / 2;
      }
    }

    if ( victim->level < L_APP && victim->class == CLASS_VAMPIRE )
    if ( !IS_SET( victim->in_room->room_flags, ROOM_INDOORS ) )
     {
      if ( time_info.hour > 6 && time_info.hour < 18 )
      {
       send_to_char( AT_RED,
       "The sunlight has weakened your defense.\n\r", victim );
       dam += dam / 2;
      }
    }

    if ( ch->class == CLASS_VAMPIRE )
     if ( !IS_SET( ch->in_room->room_flags, ROOM_INDOORS ) )
     {
      if ( time_info.hour > 23 || time_info.hour < 1 )
      {
        dam *= 2;
      }
    }

    if ( victim->class == CLASS_VAMPIRE )
     if ( !IS_SET( victim->in_room->room_flags, ROOM_INDOORS ) )
     {
      if ( time_info.hour > 23 || time_info.hour < 1 )
      {
        dam /= 2;
      }
    }
*/
        /*
         * Check for disarm, trip, parry, and dodge.
         */
        if  (  dt >= TYPE_HIT )
        {

            if ( check_dodge( ch, victim ) && dam > 0 )
                return;       
         }



        if ( IS_AFFECTED( victim, AFF_SANCTUARY ) )
            dam /= 2;
        if ( IS_AFFECTED2( victim, AFF_GOLDEN ) )
            dam /= 4;
        if ( IS_AFFECTED2( victim, AFF_DANCING ) )
            dam += dam/2;
        if ( IS_AFFECTED( victim, AFF_FIRESHIELD )
        && !( dt == gsn_backstab && chance( number_range( 5, 10 ) ) ) )
            dam -= dam / 8;
        if ( IS_AFFECTED( victim, AFF_ICESHIELD )
        && !( dt == gsn_backstab && chance( number_range( 5, 10 ) ) ) )
            dam -= dam / 8;
        if ( IS_AFFECTED( victim, AFF_CHAOS )
        && !( dt == gsn_backstab && chance( number_range( 5, 10 ) ) ) )
            dam -= dam / 4;
        if ( IS_AFFECTED( victim, AFF_SHOCKSHIELD )
        && !( dt == gsn_backstab && chance( number_range( 5, 10 ) ) ) )
            dam -= dam / 4;
        if ( IS_AFFECTED( victim, AFF_VIBRATING )
        && !( dt == gsn_backstab && chance( number_range( 5, 10 ) ) ) )
            dam -= dam / 4;
        if ( IS_AFFECTED2( victim, AFF_INERTIAL ) )
            dam -= dam / 8;
        if ( IS_SET( victim->act, UNDEAD_TYPE( victim ) ) )
            dam -= dam / 8;
        if ( IS_AFFECTED2( victim, AFF_BLADE )
        && !( dt == gsn_backstab && chance( number_range( 5, 10 ) ) ) )
            dam -= dam / 4;
        if ( IS_AFFECTED2( victim, AFF_FIELD )
        && !( dt == gsn_backstab && chance( number_range( 5, 10 ) ) ) )
            dam -= dam / 4;
        if ( IS_AFFECTED( victim, AFF_PROTECT   )
            && IS_EVIL( ch ) )
            dam -= dam / 4;
        if ( IS_AFFECTED2( victim, AFF_PROTECTION_GOOD )
            && IS_EVIL( ch ) )
            dam -= dam / 4;
        if ( dam < 0 )
            dam = 0;

    }

    /* We moved dam_message out of the victim != ch if above
     * so self damage would show.  Other valid type_undefined
     * damage is ok to avoid like mortally wounded damage - Kahn
     */
    if ( ( !IS_NPC(ch) ) && ( !IS_NPC(victim) ) )
       dam -= dam/4;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    if ( !IS_NPC(ch) && prime_class( ch ) == CLASS_WARRIOR )
       dam += dam/2;
    if ( ( !IS_NPC(ch) ) && (ch->race == RACE_OGRE ) )
       dam += dam/10;
    if (!IS_NPC(ch) && !IS_NPC(victim))
      dam /= number_range(2, 4);
    if ( dt != TYPE_UNDEFINED )
        dam_message( ch, victim, dam, dt );

    if ( dam > 25 && number_range( 0, 100 ) <= 15 )
      item_damage(victim, dam);

    victim->hit -= dam;
    if ( ( ( !IS_NPC( victim )                  /* so imms only die by */
           && IS_NPC( ch )                      /* the hands of a PC   */
           && victim->level >= LEVEL_IMMORTAL )
         ||
           ( !IS_NPC( victim )                   /* so imms don,t die  */
           && victim->level >= LEVEL_IMMORTAL    /* by poison type dmg */
           && ch == victim ) )                   /* since an imm == pc */
         && victim->hit < 1 )
            victim->hit = 1;
  if ( victim->position == POS_DEAD  )
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
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE( victim ) )
        stop_fighting( victim, FALSE );
    /*
     * Payoff for killing things.
     */

    if ( victim->position == POS_DEAD )
    {
        if ( !IS_ARENA(ch) )
        {
          group_gain( ch, victim );

          if(((ch->guild != NULL) ? ch->guild->type & GUILD_CHAOS : 0)
           && ch->guild == victim->guild
           && victim->guild_rank > ch->guild_rank)
          {
            int temp;
            temp = ch->guild_rank;
            ch->guild_rank = victim->guild_rank;
            victim->guild_rank = temp;
          }
          if ( ( !IS_NPC(ch) ) && ( !IS_NPC(victim) ) )
          {
            CLAN_DATA  *pClan;
            CLAN_DATA  *Cland;
            if ( ch->clan != victim->clan )
            {
          if ( !(abs(ch->level - victim->level) > 10 ))
          {
              if ( (pClan = get_clan_index(ch->clan)) != NULL )
                pClan->pkills++;
              if ( (Cland = get_clan_index(victim->clan)) != NULL )
                Cland->pdeaths++;
           }            }
/*            REMOVE_BIT(victim->act, PLR_THIEF);*/
          }
          if ( ( !IS_NPC(ch) ) && ( !IS_NPC(victim) ) )
          {
          if ( !(abs(ch->level - victim->level) > 10 ) )
          {
          ch->pkills++;
          victim->pkilled++;
          }
          }
          if ( ( !IS_NPC(ch) ) && ( IS_NPC(victim) ) )
          {
           CLAN_DATA    *pClan;
           if ( (pClan=get_clan_index(ch->clan)) != NULL )
             pClan->mkills++;
          }
          if ( ( IS_NPC(ch) ) && (!IS_NPC(victim)) )
          {
           CLAN_DATA   *pClan;
           if ( (pClan=get_clan_index(victim->clan)) != NULL )
             pClan->mdeaths++;
          }

          if ( !IS_NPC( victim ) )
          {
            /*
             * Dying penalty:
             * 1/2 way back to previous level.
             */
            if ( victim->level < LEVEL_HERO
            || ( victim->level >= LEVEL_HERO && IS_NPC( ch ) ) )
                death_xp_loss( victim );
            sprintf( log_buf, "%s killed by %s at %d.", victim->name,
                ch->name, victim->in_room->vnum );
            log_string( log_buf, CHANNEL_LOG, -1 );
            wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);
            if ( !IS_NPC( ch )
            && IS_SET( victim->act, PLR_THIEF )
            && ch->guild
            && !strcmp( ch->guild->name, "MERCENARY" ) )
                {
                REMOVE_BIT( victim->act, PLR_THIEF );
                info( "%s the puny thief gets destroyed by the &rMERCENARY&C %s!",
                      (int)victim->name, (int)ch->name );
                }
            else if ( !IS_NPC( ch )
            && IS_SET( ch->act, PLR_THIEF )
            && victim->guild
            && !strcmp( victim->guild->name, "MERCENARY" ) )
                info( "%s, the sly thief, has killed the &rMERCENARY&C %s.",
                      (int)ch->name, (int)victim->name );
            else
                info( "%s gets slaughtered by %s!", (int)victim->name,
                  (int)(IS_NPC(ch) ? ch->short_descr : ch->name) );
            save_clans();
          }
        }
        else
        {
          sprintf(log_buf, "&C%s &chas defeated &C%s &cin the arena!",
                  ch->name, victim->name);
          wiznet(log_buf, NULL, NULL, WIZ_DEATHS, 0, 0);
          log_string(log_buf, CHANNEL_LOG, -1);
          challenge(log_buf, 0, 0);
          ch->arenawon++;
          victim->arenalost++;
        }

        raw_kill( ch, victim );

        /* Ok, now we want to remove the deleted flag from the
         * PC victim.
         */
        if ( !IS_NPC( victim ) )
            victim->deleted = FALSE;

        if ( !IS_NPC( ch ) && IS_NPC( victim ) )
        {
            if ( IS_SET( ch->act, PLR_AUTOLOOT ) )
                do_get( ch, "all corpse" );
            else
                do_look( ch, "in corpse" );

            if ( IS_SET( ch->act, PLR_AUTOCOINS ) )
                do_get( ch, "all.coin corpse" );
            if ( IS_SET( ch->act, PLR_AUTOSAC  ) )
                do_sacrifice( ch, "corpse" );
        }

        return;
    }

    if ( victim == ch )
        return;

    /*
     * Take care of link dead people.
     */
    if ( !IS_NPC( victim ) && !victim->desc )
    {
        if ( number_range( 0, victim->wait ) == 0 )
        {
            do_recall( victim, "" );
            return;
        }
    }

    /*
     * Wimp out?
     */
    if ( IS_NPC( victim ) && dam > 0 )
    {
        if ( ( IS_SET( victim->act, ACT_WIMPY ) && number_bits( 1 ) == 0
              && victim->hit < MAX_HIT(victim) / 2 )
            || ( IS_AFFECTED( victim, AFF_CHARM ) && victim->master
                && victim->master->in_room != victim->in_room ) )
            do_flee( victim, "" );
    }

    if ( !IS_NPC( victim )
        && victim->hit   > 0
        && victim->hit  <= victim->wimpy
        && victim->wait == 0 )
        do_flee( victim, "" );

    tail_chain( );
    return;
}

void ranged_hit( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *arrow, int dt
)
{
    char      buf [ MAX_STRING_LENGTH ];
    int       victim_ac;
    int       thac0;
    int       thac0_00;
    int       thac0_97;
    int       dam;
    int       diceroll;

    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
	/* if this causes crash  remove below kjodo */
    if ( victim->position == POS_DEAD )
    {
        if ( !IS_ARENA(ch) )
        {
	  group_gain( ch, victim );

          if(((ch->guild != NULL) ? ch->guild->type & GUILD_CHAOS : 0)
           && ch->guild == victim->guild
           && victim->guild_rank > ch->guild_rank)
          {
            int temp;
            temp = ch->guild_rank;
            ch->guild_rank = victim->guild_rank;
            victim->guild_rank = temp;
          }
          if ( ( !IS_NPC(ch) ) && ( !IS_NPC(victim) ) )
          {
            CLAN_DATA  *pClan;
            CLAN_DATA  *Cland;
            if ( ch->clan != victim->clan )
            {
              if ( (pClan = get_clan_index(ch->clan)) != NULL )
                pClan->pkills++;
              if ( (Cland = get_clan_index(victim->clan)) != NULL )
                Cland->pdeaths++;
            }
/*            REMOVE_BIT(victim->act, PLR_THIEF);*/
          }
          if ( ( !IS_NPC(ch) ) && ( IS_NPC(victim) ) )
          {
           CLAN_DATA    *pClan;
           if ( (pClan=get_clan_index(ch->clan)) != NULL )
             pClan->mkills++;
          }
          if ( ( IS_NPC(ch) ) && (!IS_NPC(victim)) )
          {
           CLAN_DATA   *pClan;
           if ( (pClan=get_clan_index(victim->clan)) != NULL )
             pClan->mdeaths++;
          }
        
	  if ( !IS_NPC( victim ) )
	  {
	    /*
	     * Dying penalty:
	     * 1/2 way back to previous level.
	     */
	    if ( victim->level < LEVEL_HERO 
	    || ( victim->level >= LEVEL_HERO && IS_NPC( ch ) ) )
		death_xp_loss( victim );
	    sprintf( log_buf, "%s killed by %s at %d.", victim->name,
	        ch->name, victim->in_room->vnum );
	    log_string( log_buf, CHANNEL_LOG, -1 );
            wiznet(log_buf,NULL,NULL,WIZ_DEATHS,0,0);
	    if ( !IS_NPC( ch )
	    && IS_SET( victim->act, PLR_THIEF )
	    && ch->guild
	    && !strcmp( ch->guild->name, "MERCENARY" ) )
		{
		REMOVE_BIT( victim->act, PLR_THIEF );
		info( "%s the puny thief gets destroyed by the
&rMERCENARY&C %s!",
		      (int)victim->name, (int)ch->name );
		}
	    else if ( !IS_NPC( ch )
	    && IS_SET( ch->act, PLR_THIEF )
	    && victim->guild
	    && !strcmp( victim->guild->name, "MERCENARY" ) )
		info( "%s, the sly thief, has killed the &rMERCENARY&C
%s.",
		      (int)ch->name, (int)victim->name );
	    else
	        info( "%s gets slaughtered by %s!", (int)victim->name,
                  (int)(IS_NPC(ch) ? ch->short_descr : ch->name) );
	    save_clans();
	  }
	}
	else
	{
	  sprintf(log_buf, "&C%s &chas defeated &C%s &cin the arena!",
	          ch->name, victim->name);
	  wiznet(log_buf, NULL, NULL, WIZ_DEATHS, 0, 0);
	  log_string(log_buf, CHANNEL_LOG, -1);
	  challenge(log_buf, 0, 0);
	}

	raw_kill( ch, victim );

	/* Ok, now we want to remove the deleted flag from the
	 * PC victim.
	 */
	if ( !IS_NPC( victim ) )
	    victim->deleted = FALSE;
      }

	/* if crash remove above */

    if ( victim->position == POS_DEAD )
        return;

    if ( IS_STUNNED( ch, STUN_NON_MAGIC ) ||
         IS_STUNNED( ch, STUN_TOTAL ) )
      return;

    /*
     * Figure out the type of damage message.
     */
    
    if ( dt == TYPE_UNDEFINED )
    {
        dt = TYPE_HIT;
        if ( arrow && arrow->item_type == ITEM_WEAPON )
            dt += arrow->value[3];
    }

    /*
     * Calculate to-hit-armor-class-0 versus armor.
     */
    if ( IS_NPC( ch ) )
    {
        thac0_00 =  18;
        thac0_97 = -24;
    }
    else
    {
        thac0_00 = class_table[prime_class(ch)].thac0_00;
        thac0_97 = class_table[prime_class(ch)].thac0_97;
    }

    if (!IS_NPC(ch))
        thac0     = interpolate( ch->level, thac0_00, thac0_97 )
                  - GET_HITROLL( ch );
    else
        thac0     = interpolate(ch->level, thac0_00, thac0_97 )
                  - (ch->level + ch->level/2);

    if ( ( !IS_NPC( ch ) ) && ( ch->pcdata->learned[gsn_enhanced_hit] > 0
) )
    {
       thac0 -= ch->pcdata->learned[gsn_enhanced_hit] / 5;
       update_skpell( ch, gsn_enhanced_hit );
    }

    victim_ac = UMAX( -15, GET_AC( victim ) / 10 );
/*
    if ( victim->level < L_APP && victim->class == CLASS_VAMPIRE )
     if ( !IS_SET( victim->in_room->room_flags, ROOM_INDOORS ) )
     {
      if ( time_info.hour > 6 && time_info.hour < 18 )
      {
        victim_ac += victim->level * 2;
      }
    }
*/
    if ( !can_see( ch, victim ) )
        victim_ac -= 4;

    /*
     * The moment of excitement!
     */
    dam = 0;

    while ( ( diceroll = number_bits( 5 ) ) >= 20 )
        ;

    if (     diceroll == 0
        || ( diceroll != 19 && diceroll < thac0 - victim_ac ) )
    {
        /* Miss. */
        ranged_damage( ch, victim, 0, dt );
        tail_chain( );
        return;
    }

    /*
     * Hit.
     * Calc damage.
     */

  if ( !victim || victim->position == POS_DEAD)
        return;
    if ( IS_NPC( ch ) )
    {
        dam = number_range( ch->level / 3, ch->level * 3 / 2 );
        if ( arrow )
            dam += dam / 3;
    }
    else
    {
        if ( arrow )
            dam = number_range( arrow->value[1], arrow->value[2] );
        
        if ( arrow && dam > 1000 && !IS_IMMORTAL(ch) )
        {
            sprintf( buf, "One_hit dam range > 1000 from %d to %d",
                    arrow->value[1], arrow->value[2] );
            bug( buf, 0 );
            if ( arrow->name )
              bug( arrow->name, 0 );
        }
    }

    /*
     * Bonuses.
     */
    dam += GET_DAMROLL( ch );
    if ( arrow && IS_SET( arrow->extra_flags, ITEM_POISONED ) )
        dam += dam / 8;
    if (arrow && IS_SET( arrow->extra_flags, ITEM_FLAME ) )
        dam += dam / 8;
     if (arrow && IS_SET( arrow->extra_flags, ITEM_CHAOS ) )
        dam += dam / 4;
    if (arrow && IS_SET( arrow->extra_flags, ITEM_ICY   ) )
        dam += dam / 8;
    if ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_enhanced_damage] > 0 )
    {
        dam += dam * ch->pcdata->learned[gsn_enhanced_damage] / 150;
        update_skpell( ch, gsn_enhanced_damage );
    }
    if ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_enhanced_two] > 0 )
    {
        dam += dam / 4 * ch->pcdata->learned[gsn_enhanced_two] / 150;
        update_skpell( ch, gsn_enhanced_two );
    }
    if ( !IS_NPC( ch ) && ch->pcdata->learned[gsn_enhanced_three] > 0 )
    {
        dam += dam / 4 * ch->pcdata->learned[gsn_enhanced_three] / 150;
        update_skpell( ch, gsn_enhanced_three );
    }
    if ( !IS_AWAKE( victim ) )
        dam *= 2;
    if ( dam <= 0 )
        dam = 1;
    ranged_damage( ch, victim, dam, dt );
    tail_chain( );
    return;
}

