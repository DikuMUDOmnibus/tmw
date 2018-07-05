/* FuBaR 1.3.1X */
/* This code uses some hacked functins, mainly the original */
/* diku/merc/envy fight functions, but comes together to    */
/* create some very nice and very  unique code.             */
/* therefore, i give my thanks to the creators of merc and  */
/* envy, but my biggest thanks goes to the creators of diku */
/* without whom mudding would not be as popular as it is    */
/* today. thank you. Big thanks to project co-head Eleven  */
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


void    death_xp_loss        args( ( CHAR_DATA *victim ) );
void    group_gain           args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool    check_dodge          args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    check_killer         args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    dam_message          args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
                                    int dt ) );
void    set_shooting         args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void    stop_shooting	args(( CHAR_DATA *ch, bool fBoth ));
void    item_damage             args( ( CHAR_DATA *ch, int dam ) );
void ranged_damage args(( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt ));
void    ranged_multi    args(( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
bool    target_available args(( CHAR_DATA *ch, CHAR_DATA *fch ));
int    per_type args((CHAR_DATA *ch, OBJ_DATA *Obj));
CHAR_DATA * crossfire_check args(( CHAR_DATA *ch, CHAR_DATA *fch )); 
void   ranged_hit args((CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *gun ));
OBJ_DATA * parmor_check args ( ( CHAR_DATA *ch ) );
int   count_inv_obj args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
/*
  *
  * control those freaks fighting long range. :)
  *
 */
void update_ranged( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *victim;
    int stun;

    for ( ch = char_list; ch; ch = ch->next )
    {
        if ( ch->deleted )
            continue;

        for (stun = 0; stun < STUN_MAX; stun++)
        {
          if ( IS_STUNNED( ch, stun ) )
            ch->stunned[stun]--;
        }


        if ( ( victim = ch->engaged ) )
        {
            if ( IS_AWAKE( ch )){
/*		send_to_char( AT_WHITE, "message 1\n\r", ch); */
                ranged_multi( ch, victim, TYPE_UNDEFINED ); }
            else {
/*		send_to_char( C_DEFAULT, "Stop_shooting 1\n\r", ch ); */
                stop_shooting( ch, FALSE ); }
            continue;
        }

        if ( IS_AFFECTED( ch, AFF_BLIND )
            || ( IS_NPC( ch ) && ch->pIndexData->pShop ) )
            continue;
    }             
    return;
} 

void ranged_multi( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{

    OBJ_DATA *gun = NULL;
    int shot;
    int numshot;
    OBJ_DATA *obj;
    bool gunfound = FALSE;
    ch->engaged = victim;
/*	send_to_char( C_DEFAULT, "ranged_multi.\n\r", ch );  booger */

    if ( IS_NPC( ch ) )
    {
      mprog_hitprcnt_trigger( ch, victim );
      mprog_fight_trigger( ch, victim );
    } 


    if ( ch->position <= POS_STUNNED )
    {
     send_to_char(AT_WHITE,
	"You are stunned, but will probably recover.\n\r", ch );
     act(AT_WHITE, "$n is stunned, but will probably recover.",
	    ch, NULL, NULL, TO_ROOM );
     return;
    }

    if ( ( IS_AFFECTED2( ch, AFF_CONFUSED ) )
       && number_percent ( ) < 40 )
    {
      act(AT_YELLOW, "$n looks around confused at what's going on.", ch, NULL, NULL, TO_ROOM );
      send_to_char( AT_YELLOW, "You stand confused.\n\r", ch );
      return;
    }
    
	/* send_to_char( C_DEFAULT, "Stun/confused check.\n\r", ch );  booger */
/* couldn't put ammo here to speed up ranged_hit because it would not
identify ammo each time it would hit. thus when character is out of ammo
it would not having the repetitive click type affect. */
/*   ooooooooooh, new update, you only have to wear a gun to shoot it now,
     and oh man, each gun you have equipped shoots! mounted too */


     for ( obj = ch->carrying; obj; obj = obj->next_content )
        {
        if ( obj->deleted )
            continue;
/*	send_to_char( C_DEFAULT, "Looping through objs.\n\r", ch );  booger */
        if ( obj->item_type == ITEM_RANGED_WEAPON )
            {
            gun = obj;
            }
/*	send_to_char( C_DEFAULT, "Found a gun.\n\r", ch );  booger */
		if ( obj->wear_loc == WEAR_NONE ) 
		{ 
/*	send_to_char( C_DEFAULT, "Not equipped.\n\r", ch );  booger */
			continue;
        }
		else
		{
/*	send_to_char( C_DEFAULT, "Shooting.\n\r", ch );  booger */
			gunfound = TRUE;
            if ( gun && gun->value[4] < 2 )
            ranged_hit( ch, victim, gun );

            numshot = gun->value[4];

           if ( gun && gun->value[4] > 1 )
              for ( shot = 0; shot < numshot; shot++ )
                      ranged_hit( ch, victim, gun );
		}

        }

     for ( obj = ch->carrying; obj; obj = obj->next_content )
        {
        if ( obj->deleted )
            continue;
/*	send_to_char( C_DEFAULT, "Looking for mounted weapons.\n\r", ch );  booger */
 
                if ( IS_SET(obj->extra_flags, ITEM_LOADED ))
				{
/*	send_to_char( C_DEFAULT, "Shooting.\n\r", ch );  booger */
					gunfound = TRUE;
					gun = obj;
			if ( gun && gun->value[4] < 2 )
            ranged_hit( ch, victim, gun ); 

            numshot = gun->value[4];

           if ( gun && gun->value[4] > 1 )
              for ( shot = 0; shot < numshot; shot++ )
                      ranged_hit( ch, victim, gun );
				}
        }

	   if (!gunfound)
       {
        send_to_char(AT_BLUE, "You do not have a ranged weapon equipped.\n\r", ch);
        return;
	   }
/*	send_to_char( C_DEFAULT, "end ranged_multi.\n\r", ch );  booger */
    return;
}

void do_engage( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA 		*victim;
    OBJ_DATA 		*ammo;
    OBJ_DATA 		*gun = NULL;
    OBJ_DATA 		*clip = NULL;
    OBJ_DATA		*obj;
    ROOM_INDEX_DATA 	*to_room;
    ROOM_INDEX_DATA 	*in_room;
    EXIT_DATA 		*pexit;
    char       		arg1[ MAX_INPUT_LENGTH ];
    char       		arg2[ MAX_INPUT_LENGTH ];
    int 		MAX_DIST = 0;
    int  		dir;
    bool 		ammocheck = FALSE;
    bool		gunfound = FALSE;

  argument = one_argument( argument, arg1 );
  argument = one_argument( argument, arg2 );

/*	send_to_char( C_DEFAULT, "Starting engage engine.\n\r", ch ); booger */
  if ( ch->engaged != NULL )
     {
	send_to_char( C_DEFAULT, "You are already shooting at someone.\n\r", ch );
	return;
     }

     for ( obj = ch->carrying; obj; obj = obj->next_content )
        {
	if ( obj->deleted )
	    continue;


	if ( obj->item_type == ITEM_RANGED_WEAPON )
	    {
	    gun = obj;
	    }

    if (  obj->wear_loc == WEAR_NONE )
	    continue;
     else
	    gunfound = TRUE;	 
	 }

    if (!gunfound)
     {
     for ( obj = ch->carrying; obj; obj = obj->next_content )
        {
        if ( obj->deleted )
            continue;

	if ( IS_SET(obj->extra_flags, ITEM_LOADED))
	{
	gun = obj;
	gunfound = TRUE;
	}
	}
     }

        if (!gunfound)
          {
	send_to_char(AT_BLUE, "You do not have a ranged weapon equipped.\n\r", ch );
         return;
		}
/*	send_to_char( C_DEFAULT, "Gun check.\n\r", ch ); booger*/
/*        
if ( gun->value[2] == 0 )
ammocheck = TRUE;

if (gun->value[2] > 0)
{ */
    for ( ammo = gun->contains; ammo; ammo = ammo->next_content )
    {
     if ( !ammo || ammo->deleted )
      continue;

     if ( ammo->item_type != ITEM_BULLET && ammo->item_type != ITEM_CLIP )
      continue;

     if ( ammo->item_type == ITEM_BULLET )
     {
      ammocheck = TRUE;
      break;
     }

     if ( ammo->item_type == ITEM_CLIP )
     {
      clip = ammo;

      if ( !clip->contains )
       continue;

      for ( ammo = clip->contains; ammo; ammo = ammo->next_content )
      {
       if ( !ammo )
        continue;
  
       if ( ammo->item_type == ITEM_BULLET )
       {
        ammocheck = TRUE;
        break;
       }
      }
      if ( ammocheck == TRUE )
       break;
     }
    } /*
} */

     if ( !ammocheck || ammo->item_type == ITEM_CLIP )
     {
      send_to_char( AT_WHITE, "You are out of ammo.\n\r", ch );
      return;
     }
/*	send_to_char( C_DEFAULT, "Ammo check.\n\r", ch );  booger */
    in_room = ch->in_room;
    to_room = ch->in_room;
    MAX_DIST = gun->value[1];

    if ( arg1[0] == '\0' )
    {
      send_to_char( C_DEFAULT, "Shoot at whom?\n\r", ch );
      return;
    }

    if ( arg2[0] == '\0' )
    {
      if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
      {
	send_to_char( C_DEFAULT, "They aren't here.\n\r", ch );
	return;
      }
    }
    else
    {
      for ( dir = 0; dir < 6; dir++ )
       if ( arg2[0] == dir_name[dir][0]
           && !str_prefix( arg2, dir_name[dir] ) )
	  break;

      ch->rangedir = dir;

      if ( dir == 6 )
      {
	send_to_char( C_DEFAULT, "Shoot in which direction?\n\r", ch );
	return;
      }

      if ( ( pexit = to_room->exit[dir] ) == NULL ||
	   ( to_room = pexit->to_room ) == NULL ||
	   IS_SET( pexit->exit_info, EX_ISWALL ))

      {
	send_to_char( C_DEFAULT, "You cannot shoot in that direction.\n\r", ch );
	return;
      }

      if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
      {
	send_to_char( C_DEFAULT, "You cannot shoot through a door.\n\r", ch );
	return;
      }
/* sigh flux changed this code up to be slightly cracked
   this comment is causing the searches for mobs to miss
   occasionally, actually, a lot.... working on something
   more dynamic */
      if ( !( victim = get_char_world( ch, arg1 ) ) ||
		victim->in_vehicle != NULL)
      {
	send_to_char( C_DEFAULT, "They aren't anywhere.\n\r", ch );
	return;
      }

    if ( IS_AFFECTED( ch, AFF_CHARM ) && ch->master == victim )
    {
        act(AT_BLUE, "$N is your beloved master!", ch, NULL, victim, TO_CHAR );
        return;
    }
/*	send_to_char( C_DEFAULT, "Syntax check.\n\r", ch );  booger */
   set_shooting( ch, victim );
   ranged_multi( ch, victim, TYPE_UNDEFINED );
  }
/*	send_to_char( C_DEFAULT, "end engage.\n\r", ch );  booger */
 return;
}

void ranged_hit( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *gun )
{
    int          dir, aim, number;
    OBJ_DATA *ammo;
    OBJ_DATA *shell;
    OBJ_DATA *clip = NULL;
    ROOM_INDEX_DATA *to_room;
    ROOM_INDEX_DATA *in_room;
    EXIT_DATA *pexit;
    int dam = 0;
    int dist = 0;
    int MAX_DIST = 0;
    char buf[MAX_STRING_LENGTH];
    extern char *dir_noun []; 
    bool ammocheck = FALSE;    
    CHAR_DATA *xfirevict;
/*
if (gun->value[2] == 0)
ammocheck = TRUE;
if (gun->value[2] > 0)
{*/
/*		send_to_char( C_DEFAULT, "ranged_hit.\n\r", ch );  booger */
    for ( ammo = gun->contains; ammo; ammo = ammo->next_content )
    {
     if ( !ammo )
      continue;

     if ( ammo->item_type != ITEM_BULLET && ammo->item_type != ITEM_CLIP )
      continue;

     if ( ammo->item_type == ITEM_BULLET )
     {
	ammocheck = TRUE;
      break;
     }

     if ( ammo->item_type == ITEM_CLIP )
     {
      clip = ammo;

      if ( !clip->contains )
       continue;

      for ( ammo = clip->contains; ammo; ammo = ammo->next_content )
      {
       if ( ammo->item_type == ITEM_BULLET )
       {
        ammocheck = TRUE;
        break;
       }
      }
      if ( ammocheck == TRUE )
       break;
     }
    }/*
} */
     if ( !ammocheck || ammo->item_type == ITEM_CLIP )
     {
      act( AT_RED,
       "You move to fire your $p, but find that it's out of ammo!",
        ch, gun, NULL, TO_CHAR );
      act( AT_YELLOW,
       "$n's $p makes a clicking noise.", ch, gun, NULL, TO_ROOM );
      stop_shooting( ch, FALSE );
      return;
     }
/*	send_to_char( C_DEFAULT, "ammo check.\n\r", ch );  booger */
/*
if (gun->value[2] > 0)
*/
     dam = number_range(ammo->value[2], ammo->weight + (ammo->value[2] * 2));
/*
else
     dam = number_range( (200 + gun->level), (1000 + gun->level));
*/
     if ( dam > 1000 && !IS_IMMORTAL(ch) )
     {
      sprintf( buf, "ranged_hit dam range > 1000 from %d to %d",
       ammo->value[2], ammo->weight + (ammo->value[2] * 2) );
      bug( buf, 0 );
      if ( ammo->name )
       bug( ammo->name, 0 );
     }
/*	send_to_char( C_DEFAULT, "Damage calculated.\n\r", ch );  booger */

     in_room = ch->in_room;
     to_room = ch->in_room;
     dir = ch->rangedir;

      if ( ( pexit = to_room->exit[dir] ) == NULL ||
	   ( to_room = pexit->to_room ) == NULL )
      {
	send_to_char( C_DEFAULT, "You cannot shoot in that direction.\n\r", ch );
       stop_shooting( ch, FALSE );
	return;
      }

      if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
      {
	send_to_char( C_DEFAULT, "You cannot shoot through a door.\n\r", ch );
       stop_shooting( ch, FALSE );
	return;
      }
/*	send_to_char( C_DEFAULT, "Shooting in a valid direction.\n\r", ch );  booger */
/*
if (gun->value[2] > 0)
{
*/
      shell = create_object( get_obj_index(OBJ_VNUM_BULLET_SHELL), 0 );
      shell->timer = number_range( 5, 10 );
      obj_to_room( shell, ch->in_room );
/*
}
*/
      MAX_DIST = gun->value[1];
      for ( dist = 1; dist <= MAX_DIST; dist++ )
      {
/*	send_to_char( C_DEFAULT, "Searching rooms.\n\r", ch );  booger */
	char_from_room( ch );
	char_to_room( ch, to_room );


	if ( target_available( ch, victim ) )
	  break;

	if ( ( pexit = to_room->exit[dir] ) == NULL ||
	     ( to_room = pexit->to_room ) == NULL ||
	       IS_SET( pexit->exit_info, EX_CLOSED )||
	       IS_SET( pexit->exit_info, EX_ISWALL ))
	{
/*
if (gun->value[2] > 0)
{
*/
	  sprintf( buf, "A $p flys in from $T and hits the %s wall.", 
		   dir_name[dir] );
	  act( AT_WHITE, buf, ch, ammo, dir_noun[rev_dir[dir]], TO_ROOM );
	  sprintf( buf, "You shoot your $p %d room%s $T, where it hits a wall.",
		   dist, dist > 1 ? "s" : "" );

	  act( AT_WHITE, buf, ch, ammo, dir_name[dir], TO_CHAR );
	  char_from_room( ch );
	  char_to_room( ch, in_room );
          oprog_throw_trigger( ammo, ch );
          obj_from_obj( ammo );
          extract_obj( ammo );
/*
}
else
{
          sprintf( buf, "An enrgy blast flys in from $T and hits the %s wall.",
                   dir_name[dir] );
          act( AT_WHITE, buf, ch, NULL, dir_noun[rev_dir[dir]], TO_ROOM );
          sprintf( buf, "You shoot your $p %d room%s $T, where it hits a wall.",
                   dist, dist > 1 ? "s" : "" );
          act( AT_WHITE, buf, ch, gun, dir_name[dir], TO_CHAR );
          char_from_room( ch );
          char_to_room( ch, in_room );
}
*/
          stop_shooting( ch, FALSE );
	  return;
	}
      }

      if ( !target_available( ch, victim ) )
      {
/*
if (gun->value[2] > 0)
{
*/
	act( AT_WHITE, 
	    "A $p flies in from $T and falls harmlessly to the ground.",
	    ch, ammo, dir_noun[rev_dir[dir]], TO_ROOM );
	sprintf( buf,
	"Your $p falls harmlessly to the ground %d room%s $T of here.",
		dist, dist > 1 ? "s" : "" );
	act( AT_WHITE, buf, ch, ammo, dir_name[dir], TO_CHAR );
	char_from_room( ch );
	char_to_room( ch, in_room );
	oprog_throw_trigger( ammo, ch );
         obj_from_obj( ammo );
          extract_obj( ammo );
/*
}
else
{
        act( AT_WHITE,
            "A energy blast flies in from $T and falls harmlessly to the ground.",
            ch, ammo, dir_noun[rev_dir[dir]], TO_ROOM );
        sprintf( buf,
        "Your blast fizzles out harmlessly%d room%s $T of here.",
                dist, dist > 1 ? "s" : "" );
        act( AT_WHITE, buf, ch, NULL, dir_name[dir], TO_CHAR );
        char_from_room( ch );
        char_to_room( ch, in_room );
}
*/
        stop_shooting( ch, FALSE );
	return;
      }
     if (dist > 0 && (xfirevict = crossfire_check(ch, victim)) != NULL)
     {
/*
if (gun->value[2] > 0)
{
*/
      char_from_room( ch );
      char_to_room( ch, in_room );
      act( AT_WHITE, "A $p flys in from $T and hits $n!", xfirevict, ammo,
          dir_noun[rev_dir[dir]], TO_NOTVICT );
      act( AT_WHITE, "A $p flys in from $T and hits you!", xfirevict, ammo,
          dir_noun[rev_dir[dir]], TO_CHAR );
      sprintf( buf, "Your $p flew %d rooms %s and hit $N!", dist,
              dir_name[dir] );
      act( AT_WHITE, buf, ch, ammo, xfirevict, TO_CHAR );
      oprog_throw_trigger( ammo, ch );
      obj_from_obj( ammo );
      extract_obj( ammo );
      ranged_damage( ch, xfirevict, dam, gsn_bullet_shot );
/*
}
else
{      char_from_room( ch );
      char_to_room( ch, in_room );
      act( AT_WHITE, "An energy blast flys in from $T and hits $n!", xfirevict, NULL,
          dir_noun[rev_dir[dir]], TO_NOTVICT );
      act( AT_WHITE, "An energy blast flys in from $T and hits you!", xfirevict,
NULL,
          dir_noun[rev_dir[dir]], TO_CHAR );
      sprintf( buf, "Your energy blast flew %d rooms %s and hit $N!", dist,
              dir_name[dir] );
      act( AT_WHITE, buf, ch, NULL, xfirevict, TO_CHAR );
      ranged_damage( ch, xfirevict, dam, gsn_energy_shot );
}
*/
      if ( IS_NPC( xfirevict ) )
      {
         if ( xfirevict->level > 3 )
             xfirevict->hunting = ch;
      }
      return;
     }
/*	send_to_char( C_DEFAULT, "crossfire check.\n\r", ch );  booger */
    aim = (get_curr_dex( ch ) * 3);
    number = number_percent();


     if ( dist > 0 )
     {
/*
if (gun->value[2] > 0)
{
*/
/*	send_to_char( C_DEFAULT, "about to hit target...\n\r", ch );  booger */

      char_from_room( ch );
      char_to_room( ch, in_room );
      act( AT_WHITE, "A $p flys in from $T and hits $n!", victim, ammo,
	  dir_noun[rev_dir[dir]], TO_NOTVICT );
      act( AT_WHITE, "A $p flys in from $T and hits you!", victim, ammo,
	  dir_noun[rev_dir[dir]], TO_CHAR );
      sprintf( buf, "Your $p flew %d rooms %s and hit $N!", dist,
	      dir_name[dir] );
      act( AT_WHITE, buf, ch, ammo, victim, TO_CHAR );
      oprog_throw_trigger( ammo, ch );
      obj_from_obj( ammo );
      extract_obj( ammo );
      ranged_damage( ch, victim, dam, gsn_bullet_shot );
/*	send_to_char( C_DEFAULT, "target has been smacked.\n\r", ch );  booger */
/*
}
else
{      char_from_room( ch );
      char_to_room( ch, in_room );
      act( AT_WHITE, "An energy blast flys in from $T and hits $n!",
xfirevict, NULL,
          dir_noun[rev_dir[dir]], TO_NOTVICT );
      act( AT_WHITE, "An energy blast flys in from $T and hits you!",
xfirevict,
NULL,
          dir_noun[rev_dir[dir]], TO_CHAR );
      sprintf( buf, "Your energy blast flew %d rooms %s and hit $N!",
dist,
              dir_name[dir] );
      act( AT_WHITE, buf, ch, NULL, xfirevict, TO_CHAR );
      ranged_damage( ch, xfirevict, dam, gsn_energy_shot );
} 
*/
      if ( IS_NPC( victim ) )
      {
         if ( victim->level > 3 )
             victim->hunting = ch;
      }  
      return;
	}
   
/*
if (gun->value[2] > 0)
{
*/
  obj_from_obj( ammo );
  act( AT_WHITE, "$n shoots a $p at $N!", ch, ammo, victim, TO_ROOM );
  act( AT_WHITE, "You shoot your $p at $N.", ch, ammo, victim, TO_CHAR );
  oprog_throw_trigger( ammo, ch );
  ranged_damage( ch, victim, dam, gsn_bullet_shot );
  extract_obj( ammo );
/*
}
else
{
  act( AT_WHITE, "$n shoots a $p at $N!", ch, gun, victim, TO_ROOM );
  act( AT_WHITE, "You shoot your $p at $N.", ch, gun, victim, TO_CHAR );
  ranged_damage( ch, victim, dam, gsn_energy_shot );
}
*/
/*	send_to_char( C_DEFAULT, "end ranged_hit.\n\r", ch );  booger */
  tail_chain( );
 return;
}

void ranged_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt )
{
    OBJ_DATA *parmor;
/*	send_to_char( C_DEFAULT, "ranged_damage.\n\r", ch );  booger */
    if ( victim->position == POS_DEAD )
        return;

    /*
     * Stop up any residual loopholes.
     */

    if ( victim != ch )
    {
        /*
         * Certain attacks are forbidden.
         * Most other attacks are returned.
         */
/*        if ( is_safe( ch, victim ) )
            return; 
        check_killer( ch, victim ); 
        send_to_char( AT_WHITE, "setting shooting\n\r", ch);
        set_shooting( ch, victim ); */

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
            act(AT_GREY, "$n fades into existence.", ch, NULL, NULL,
TO_ROOM );
        }
        if (IS_AFFECTED2( ch, AFF_PHASED ) )
        {
            affect_strip ( ch, skill_lookup("phase shift") );
            affect_strip ( ch, skill_lookup("mist form") );
            REMOVE_BIT( ch->affected_by2, AFF_PHASED );
            act(AT_GREY, "$n returns from an alternate plane.", ch, NULL,
NULL, TO_ROOM );
        }
 
    if ((parmor = parmor_check(victim)) != NULL
    && parmor->value[1] > 1 )
	dam /= parmor->value[1];

        /*
         * Check for dodge.
         */
/*        if  (  dt >= TYPE_HIT )
        {

            if ( check_dodge( ch, victim ) && dam > 0 )
                return;       
         } */

        if ( IS_AFFECTED( victim, AFF_SANCTUARY ) )
            dam /= 2;
        if ( IS_AFFECTED2( victim, AFF_GOLDEN ) )
            dam /= 4;

       
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
    if ( !IS_AWAKE( victim ) ) {
/*	send_to_char( C_DEFAULT, "stop shooting 3\n\r", ch ); */
        stop_shooting( victim, FALSE ); }
    /*
     * Payoff for killing things.
     */

    if ( victim->position == POS_DEAD )
    {
        if ( !IS_ARENA(ch) )
        {
          group_gain( ch, victim );

            if ( ( !IS_NPC(ch) ) && ( !IS_NPC(victim) ) )
            {
              if ( !(abs(ch->level - victim->level) > 10 ) )
              {
              ch->pkills++;
              victim->pkilled++;
                if ( ch->clan != victim->clan )
                {
                  CLAN_DATA  *pClan;
                  CLAN_DATA  *Cland;
                  if ( (pClan = get_clan_index(ch->clan)) != NULL )
                  pClan->pkills++;
                  if ( (Cland = get_clan_index(victim->clan)) != NULL )
                  Cland->pdeaths++;
                }     
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
        stop_shooting( ch, FALSE );
        /* Ok, now we want to remove the deleted flag from the
         * PC victim.
         */
        if ( !IS_NPC( victim ) )
            victim->deleted = FALSE;

/*        if ( !IS_NPC( ch ) && IS_NPC( victim ) )
        {
            if ( IS_SET( ch->act, PLR_AUTOLOOT ) )
                do_get( ch, "all corpse" );
            else
                do_look( ch, "in corpse" );

            if ( IS_SET( ch->act, PLR_AUTOCOINS ) )
                do_get( ch, "all.coin corpse" );
            if ( IS_SET( ch->act, PLR_AUTOSAC  ) )
                do_sacrifice( ch, "corpse" );
        } */

        return;
    }

    if ( victim == ch )
        return;

    /*
     * Take care of link dead people.
     */
/*    if ( !IS_NPC( victim ) && !victim->desc )
    {
        if ( number_range( 0, victim->wait ) == 0 )
        {
            do_recall( victim, "" );
            return;
        }
    } */

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


void set_shooting( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf [ MAX_STRING_LENGTH ];

    if ( ch->engaged )
    {
        bug( "Set_shooting: already fighting", 0 );
        sprintf( buf, "...%s shooting %s at %d",
                ( IS_NPC( ch )     ? ch->short_descr     : ch->name     ),
                ( IS_NPC( victim ) ? victim->short_descr : victim->name ),
                victim->in_room->vnum );
        bug( buf , 0 );
        return;
    }

    if ( IS_AFFECTED( ch, AFF_SLEEP ) )
        affect_strip( ch, gsn_sleep );

    ch->engaged = victim;

    return;
}

void do_disengage( CHAR_DATA *ch, char *argument )
{
    if ( ch->engaged == NULL )
	{
	send_to_char( C_DEFAULT, "You are not shooting at anyone.\n\r", ch );
	return;
	}

	stop_shooting( ch, FALSE );

return;
}

void stop_shooting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

    for ( fch = char_list; fch; fch = fch->next )
    {
        if ( fch == ch || ( fBoth && fch->engaged == ch ) )
        {
            fch->engaged       = NULL;
            fch->hunting        = NULL;
        }
    }

    return;
}


CHAR_DATA *crossfire_check( CHAR_DATA *ch, CHAR_DATA *fch )
{
    CHAR_DATA *rch;
    int aim, number;   





    aim = (get_curr_dex( ch ) * 3);
    number = number_percent();

    if ( number <= aim )
	return NULL;

    for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
    {
        if ( rch->level >= LEVEL_IMMORTAL )
	continue;

	if ( rch == ch )
	continue;

        if ( fch != rch )
            return rch;
    }

    return NULL;
}


bool target_available( CHAR_DATA *ch, CHAR_DATA *fch )
{
    CHAR_DATA *rch;

    for ( rch = ch->in_room->people; rch; rch = rch->next_in_room )
    {
        if ( !can_see( ch, rch ) || rch->name != fch->name )
            continue;
        if ( fch->name == rch->name )
            return TRUE;
    }

    return FALSE;
}

void do_reload( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    OBJ_DATA *ammo;
    OBJ_DATA *ammox;
    OBJ_DATA *clip = NULL;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    bool ammocheck = FALSE;
    int  clipnum = 0;
    int  amount = 0;
    int  hasamount = 0;

  argument = one_argument( argument, arg );
  argument = one_argument( argument, arg2 );

  if ( arg[0] == '\0' )
  {
   send_to_char( AT_BLUE,
    "What do you plan to load into the gun or clip?\n\r", ch );
   return;
  }

  if ( ( obj = get_eq_char( ch, WEAR_HOLD ) ) == NULL )
  {
   send_to_char(AT_BLUE,"You aren't holding anything to reload.\n\r", ch );
   return;
  }


  if ( !( ammo = get_obj_carry( ch, arg ) ) )
  {
   send_to_char(AT_BLUE, "You are not holding the ammunition you refer to.\n\r", ch );
   return;
  }

  if ( ammo->item_type != ITEM_BULLET && ammo->item_type != ITEM_CLIP )
  {
   send_to_char( C_DEFAULT, "That is neither a clip nor a bullet.\n\r", ch );
   return;
  }

  if ( ammo->value[2] != obj->value[2] )
  {
   send_to_char( C_DEFAULT,
    "The calibur of the ammunition does not equal the calibur of the gun/clip.\n\r", ch );
   return;
  }

  if ( obj->item_type == ITEM_RANGED_WEAPON )
  {
    for ( ammox = obj->contains; ammox; ammox = ammox->next_content )
    {
     if ( !ammox )
      continue;

     if ( ammox->item_type != ITEM_BULLET && ammox->item_type != ITEM_CLIP )
      continue;

     if ( ammox->item_type == ITEM_BULLET )
     {
      ammocheck = TRUE;
      break;
     }

     if ( ammox->item_type == ITEM_CLIP )
     {
      clip = ammox;

      if ( !clip->contains ) 
       continue;

      for ( ammox = clip->contains; ammox; ammox = ammox->next_content )
      {
       if ( ammox->item_type == ITEM_BULLET )
       {
        ammocheck = TRUE;
        break;
       }
      }
      if ( ammocheck == TRUE )
       break;
     }
    }

  if ( ammox != NULL )
  {
   if ( ammox->item_type == ITEM_BULLET && 
        ammo->item_type == ITEM_BULLET && !(clip) )
   {
    send_to_char( C_DEFAULT, "There is already a single bullet in the chamber.\n\r", ch );
    return;
   }

   if ( (clip) && ammo->item_type == ITEM_CLIP )
   {
    send_to_char( C_DEFAULT, "There is a clip loaded already.\n\r", ch );
    return;
   }
  }

   if ( ammo->item_type == ITEM_CLIP && ammo->pIndexData->vnum != obj->value[3] )
   {
    send_to_char( C_DEFAULT, "That clip does not go with this gun.\n\r", ch );
    return;
   }    

   obj_from_char( ammo );
   obj_to_obj( ammo, obj );
   act(AT_GREEN, "You load $p into $P.", ch, ammo, obj, TO_CHAR );
   act(AT_GREEN, "$n loads $p into $P.", ch, ammo, obj, TO_ROOM );
  }
  else if ( obj->item_type == ITEM_CLIP )
  {
    char buf[MAX_STRING_LENGTH];
    if ( ammo->item_type != ITEM_BULLET )
    {
     send_to_char( C_DEFAULT, "That is not a bullet.\n\r", ch );
     return;
    }

    for ( ammox = obj->contains; ammox; ammox = ammox->next_content )
     clipnum += 1;

    if ( clipnum >= obj->value[1] )
    {
     send_to_char( C_DEFAULT, "That clip is full.\n\r", ch );
     return;
    }

   if ( !arg2[0] == '\0' && is_number(arg2) )
    amount = atoi(arg2);
   else
    amount = 1;

    hasamount = count_inv_obj( ch, ammo );
    if ( amount > hasamount )
    {
    send_to_char( AT_BLUE, "You don't have that much ammo.\n\r", ch );
    return;
    }

    if ( amount > ( obj->value[1] - clipnum ) )
    {
     sprintf( buf,
      "You are trying to load too many bullets into the clip,"
      " it has room enough for %d.\n\r", obj->value[1] - clipnum );
    
     send_to_char( C_DEFAULT, buf, ch );
     return;
    }

   while( amount >= 1 )
   {
    if ( (ammo = get_obj_carry( ch, arg )) )
    {
     obj_from_char( ammo );
     obj_to_obj( ammo, obj );
    }
    amount -= 1;
   }
   act(AT_GREEN, "You load $p into $P.", ch, ammo, obj, TO_CHAR );
   act(AT_GREEN, "$n loads $p into $P.", ch, ammo, obj, TO_ROOM );
 }
 return;
}

void do_unload( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *ammo;
    OBJ_DATA *gun = NULL;
    OBJ_DATA *obj;
    bool ammocheck = FALSE;
    bool gunfound = FALSE;

     for ( obj = ch->carrying; obj; obj = obj->next_content )
        {
        if ( obj->deleted )
            continue;

        if ( obj->item_type == ITEM_RANGED_WEAPON )
            {
            gun = obj;
            }
    if ( obj->wear_loc == WEAR_NONE )
            continue;
    	else
         gunfound = TRUE;

        }

        if (!gunfound)
          {
        send_to_char(AT_BLUE, "You do not have a ranged weapon equipped.\n\r", ch);
         return;
		}


    if ( !gun->contains )
    {
     send_to_char(AT_BLUE,"There isn't anything in that weapon.\n\r", ch );
     return;
    }
 

    for ( ammo = gun->contains; ammo; ammo = ammo->next_content )
    {
     if ( !ammo || ammo->deleted )
      continue;

     if ( !gun->contains ) 
      break;

     obj_from_obj( ammo );
     obj_to_char( ammo, ch );
     ammocheck = TRUE;
    }

  if ( ammocheck )
  {
   act(AT_GREEN, "You unload $p.", ch, gun, NULL, TO_CHAR );
   act(AT_GREEN, "$n unloads $p.", ch, gun, NULL, TO_ROOM );
  }
  else
   act(AT_GREEN, "Your $p is empty.", ch, gun, NULL, TO_CHAR );

 return;
}

int count_inv_obj(CHAR_DATA *ch, OBJ_DATA *obj)
{
    OBJ_DATA *inv;
    int count = 0;

    for ( inv = ch->carrying; inv; inv = inv->next_content )
        {
	if (inv->deleted)
	continue;
	if ( inv->pIndexData->vnum == obj->pIndexData->vnum )
	count++;
	}

return count;
}
