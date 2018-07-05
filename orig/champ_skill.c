#include <stdio.h>
#include <string.h>
#include <time.h>
#include "merc.h" 

void    affect_to_char3 args(( CHAR_DATA *ch, AFFECT_DATA *paf ));
void recursive_clone args((CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone));

void spell_purify( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA  *obj;
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int        yesno  = FALSE;

	    for ( obj = ch->carrying; obj; obj = obj->next_content )
		{

       if ( IS_ANTI_CLASS( obj, ITEM_ANTI_MAGE ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_MAGE );
	    act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_CLERIC ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_CLERIC );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_THIEF ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_THIEF );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_WARRIOR ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_WARRIOR );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_PSI ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_PSI );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_DRUID ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_DRUID );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_RANGER ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_RANGER );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_PALADIN ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_PALADIN );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_BARD ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_BARD );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_VAMP ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_VAMP );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_NECRO ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_NECRO );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_WWF ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_WWF );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_CLASS( obj, ITEM_ANTI_MONK ) )
        {
            REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_MONK );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
	if ( IS_ANTI_CLASS( obj, ITEM_ANTI_NINJA ) )
	{
	    REMOVE_BIT( obj->anti_class_flags, ITEM_ANTI_NINJA );
	    act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
	}
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_HUMAN ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_HUMAN );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_ELF ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_ELF );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_DWARF ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_DWARF );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_PIXIE ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_PIXIE );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_HALFLING ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_HALFLING );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_DROW ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_DROW );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_ELDER ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_ELDER );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_OGRE ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_OGRE );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_LIZARDMAN ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_LIZARDMAN );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_DEMON ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_DEMON );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_GHOUL ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_GHOUL );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_ILLITHID ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_ILLITHID );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_MINOTAUR ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_MINOTAUR );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_TROLL ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_TROLL );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_SHADOW ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_SHADOW );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
        if ( IS_ANTI_RACE( obj, ITEM_ANTI_TABAXI ) )
        {
            REMOVE_BIT( obj->anti_race_flags, ITEM_ANTI_TABAXI );
            act(AT_WHITE, "$p glows white.", victim, obj, NULL, TO_CHAR );
            yesno = TRUE;
        }
      }
          
    if ( ch != victim && yesno )
        send_to_char(AT_BLUE, "Ok.\n\r", ch );
    return;
}

void do_creation( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *create;
	OBJ_DATA *new_obj;
    char arg[MAX_INPUT_LENGTH];
	char buf[MAX_STRING_LENGTH];
    int value;
    AFFECT_DATA *paf;
    AFFECT_DATA af;

    argument = one_argument(argument,arg);

	if (!can_use_skpell(ch, gsn_creation))
	{
	send_to_char(AT_BLUE, "You don't know how to do that!\n\r", ch );
	return;
	}
value = number_percent();
  if ( value > ch->pcdata->learned[gsn_creation] )
  {
  send_to_char(AT_BLUE, "You failed!\n\r", ch );
  return;
  }
 

	create = create_mobile(get_mob_index(MOB_CREATION));
    if ( arg[0] == '\0' )
	{
		sprintf( buf, "genetic creation" );
    create->name 	= str_dup(buf);
	}
	else
	{
    create->name 	= str_dup(arg);
	}

    if ( arg[0] == '\0' )
	{
    create->short_descr	= str_dup(buf);
	}
	else
	{
    create->short_descr	= str_dup(arg);
	}
	sprintf(buf, "%s's %s stands by his master.\n\r", ch->name,
create->name);
    create->long_descr	= str_dup(buf);
    create->sex		= ch->sex;
    create->class[0]	= ch->class[0];
    create->class[1]	= -1;
    create->race		= ch->race;
    create->level	= (ch->level)-5;
    create->trust	= 0;
    create->timer	= ch->timer;
    create->wait		= ch->wait;
    create->hit		= ch->hit;
    create->mod_hit	= ch->mod_hit;
    create->perm_hit	= ch->perm_hit;
    create->mana		= ch->mana;
    create->perm_mana	= ch->perm_mana;
    create->mod_mana	= ch->mod_mana;
    create->move		= ch->move;
    create->perm_move	= ch->perm_move;
    create->mod_move	= ch->mod_move;
#ifdef NEW_MONEY
    create->money.gold   = 0;
    create->money.silver	= 0;
    create->money.copper = 0;
#else
    create->gold		= 0;
#endif
    create->exp		= ch->exp;
    create->affected_by	= ch->affected_by;
    create->position	= ch->position;
    create->alignment	= ch->alignment;
    create->hitroll	= ch->level;
    create->damroll	= ch->level;
    create->wimpy	= 0;
    create->armor	= ch->armor;    

    /* now add the affects */
    for (paf = ch->affected; paf != NULL; paf = paf->next)
        affect_to_char(create,paf);
	char_to_room(create,ch->in_room);
        act(AT_BLUE,"$n has created $N.",ch,NULL,create,TO_ROOM);
        act(AT_BLUE,"You create $N.",ch,NULL,create,TO_CHAR);
	sprintf(buf,"$N creates %s.",create->short_descr);
	wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
    add_follower( create, ch );
    af.type      = skill_lookup("charm person");
    af.level     = ch->level;
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( create, &af );
    SET_BIT( create->act, ACT_MUTATION );
        return;
}

void do_duplicate(CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *rest;
    CHAR_DATA *mob;
    OBJ_DATA  *obj;
	char buf[MAX_STRING_LENGTH];
        CHAR_DATA *clone;
        OBJ_DATA *new_obj;
   CHAR_DATA *parent;
    AFFECT_DATA *paf;
    AFFECT_DATA af;

    rest = one_argument(argument,arg);
    argument = one_argument(argument, arg2);

    if (arg[0] == '\0')
    {
	send_to_char(AT_WHITE,"Duplicate what?\n\r",ch);
	return;
    }


	mob = get_char_room(ch,arg);

	if (mob == NULL)
	{
	    send_to_char(AT_WHITE,"You don't see that here.\n\r",ch);
	    return;
    }

    if (!can_use_skpell(ch, gsn_duplicate))
	{
	send_to_char(AT_BLUE, "You can't do that!\n\r", ch );
	return;
	}

  if (number_percent() > ch->pcdata->learned[gsn_duplicate])
  {
  send_to_char(AT_BLUE, "You have FAILED!\n\r", ch );
  sprintf(buf, "%s explodes in a sloppy mess!\n\r", mob->name );
  send_to_char(AT_WHITE, buf, ch );
  extract_char( mob, TRUE );
   return;
  }


    if (mob != NULL)
    {
     parent = mob;

	if (!IS_NPC(mob) ||
		mob->master != ch )
	{
	    send_to_char(AT_WHITE,"You can only duplicate creatures under your control.\n\r",ch);
	    return;
	}

	clone = create_mobile(mob->pIndexData);
    if ( arg2[0] == '\0' )
	{
		sprintf( buf, "genetic creation" );
    clone->name 	= str_dup(buf);
	}
	else
	{
    clone->name 	= str_dup(arg2);
	}

    if ( arg2[0] == '\0' )
	{
    clone->short_descr	= str_dup(buf);
	}
	else
	{
    clone->short_descr	= str_dup(arg2);
	}
	sprintf(buf, "%s's %s stands by his master.\n\r", ch->name,
clone->name);
    clone->long_descr	= str_dup(buf);
    clone->description	= str_dup(parent->description);
    clone->sex		= parent->sex;
    clone->class[0]	= parent->class[0];
    clone->class[1]	= -1;
    clone->race		= parent->race;
    clone->level	= parent->level;
    clone->trust	= 0;
    clone->timer	= parent->timer;
    clone->wait		= parent->wait;
    clone->hit		= parent->perm_hit;
    clone->mod_hit	= 0;
    clone->perm_hit	= parent->perm_hit;
    clone->mana		= parent->mana;
    clone->perm_mana	= parent->perm_mana;
    clone->mod_mana	= parent->mod_mana;
    clone->move		= parent->move;
    clone->perm_move	= parent->perm_move;
    clone->mod_move	= parent->mod_move;
#ifdef NEW_MONEY
    clone->money.gold   = 0;
    clone->money.silver	= 0;
    clone->money.copper = 0;
#else
    clone->gold		= 0;
#endif
    clone->exp		= parent->exp;
    clone->act		= parent->act;
    clone->affected_by	= parent->affected_by;
    clone->position	= parent->position;
    clone->practice	= parent->practice;
    clone->alignment	= parent->alignment;
    clone->hitroll	= parent->hitroll;
    clone->damroll	= parent->damroll;
    clone->wimpy	= parent->wimpy;
    clone->spec_fun	= parent->spec_fun;
    clone->armor	= parent->armor;    

    /* now add the affects */
    for (paf = parent->affected; paf != NULL; paf = paf->next)
        affect_to_char(clone,paf); 
/*	
	for (obj = mob->carrying; obj != NULL; obj = obj->next_content)
	{
		new_obj = create_object(obj->pIndexData,0);
		clone_object(obj,new_obj);
		recursive_clone(ch,obj,new_obj);
		obj_to_char(new_obj,clone);
		new_obj->wear_loc = obj->wear_loc;
	}
*/
	char_to_room(clone,ch->in_room);
        act(AT_WHITE,"$n has duplicated $N.",ch,NULL,clone,TO_ROOM);
        act(AT_WHITE,"You duplicate $N.",ch,NULL,clone,TO_CHAR);
	sprintf(buf,"$N duplicates %s.",clone->short_descr);
	wiznet(buf,ch,NULL,WIZ_LOAD,WIZ_SECURE,get_trust(ch));
    add_follower( clone, ch );
    af.type      = skill_lookup("charm person");
    af.level     = ch->level;
    af.duration  = -1;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_CHARM;
    affect_to_char( clone, &af );
        return;
}
return;
}

void do_combination( CHAR_DATA *ch, char *argument )
{
	CHAR_DATA *mob;
	CHAR_DATA *pmob;
    char      arg[ MAX_INPUT_LENGTH ];
    char      arg1[ MAX_STRING_LENGTH ];
	char buf[MAX_STRING_LENGTH];
   

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg1 );

    if (arg[0] == '\0')
    {
     send_to_char(AT_WHITE, "syntax: combine <mob 1> <mob 2>\n\r", ch );
	 return;
	}

    if (arg1[0] == '\0')
    {
     send_to_char(AT_WHITE, "syntax: combine <mob 1> <mob 2>\n\r", ch );
	 return;
	}

        mob = get_char_room(ch,arg);

        if (mob == NULL)
        {
            sprintf( buf, "You don't see a %s here.\n\r", arg );
            send_to_char(AT_WHITE, buf,ch);
            return;
		}

		pmob = get_char_room(ch,arg1);

		if (pmob == NULL)
		{
            sprintf( buf, "You don't see a %s here.\n\r", arg1 );
            send_to_char(AT_WHITE, buf,ch);
            return;
		}

    if (!can_use_skpell(ch, gsn_combine))
        {
        send_to_char(AT_BLUE, "You can't do that!\n\r", ch );
        return;
        }

  if (number_percent() > ch->pcdata->learned[gsn_combine])
  {
  send_to_char(AT_BLUE, "You have FAILED!\n\r", ch );
  sprintf(buf, "%s and %s explode in a sloppy mess!\n\r", mob->name,
pmob->name );
  send_to_char( AT_WHITE, buf, ch);
  extract_char( mob, TRUE );
  extract_char( pmob, TRUE );
  return;
  }

  if ( mob != NULL && pmob != NULL )
  {
        if (!IS_NPC(mob) ||
                mob->master != ch ||
				!IS_NPC(pmob) ||
				pmob->master != ch )
        {
            send_to_char(AT_WHITE,"You can only combine creatures under your control.\n\r", ch);
            return;
        }

		if ( (mob->hit + pmob->hit) > 30000 )
		{
			mob->hit = 30000;
		}
		else
		{
			mob->hit = mob->hit + pmob->hit;
		}

				if ( (mob->mod_hit + pmob->mod_hit) > 3000 )
		{
			mob->mod_hit = 3000;
		}
		else
		{
			mob->mod_hit = mob->mod_hit + pmob->mod_hit;
		}

		if ( (mob->perm_hit + pmob->perm_hit) > 30000 )
		{
			mob->perm_hit = 30000;
		}
		else
		{
			mob->perm_hit = mob->perm_hit + pmob->perm_hit;
		}

        if ( (mob->hitroll + pmob->hitroll) > 1000 )
		{
			mob->hitroll = 1000;
		}
		else
		{
			mob->hitroll = mob->hitroll + pmob->hitroll;
		}

        if ( (mob->damroll + pmob->damroll) > 500 )
		{
			mob->damroll = 500;
		}
		else
		{
			mob->damroll = mob->damroll + pmob->damroll;
		}

        if ( (mob->armor + pmob->armor) < -2000 )
		{
			mob->armor = -2000;
		}
		else
		{
			mob->armor = mob->armor + pmob->armor;
		}
    sprintf( buf, "You combine %s into %s.\n\r", pmob->name, mob->name
);
	send_to_char( AT_WHITE, buf, ch );
	extract_char( pmob, TRUE );
    return;
  }
return;
}

/*
 * Illuminati bestow command, for deity only.
 */
/*  void do_bestow( CHAR_DATA *ch, char *argument )
{
  char arg[MAX_INPUT_LENGTH];
  CHAR_DATA *victim;

  if ( ch->clan != 1 || ch->clev < 5 )
  {
    send_to_char(C_DEFAULT, "Huh?\n\r", ch );
    return;
  }

  argument = one_argument( argument, arg );

  if ( !( victim = get_char_room( ch, arg ) ) || IS_NPC(victim) )
  {
    send_to_char(C_DEFAULT, "They aren't here.\n\r", ch );
    return;
  }

  if ( victim->clan != ch->clan )
  {
    send_to_char(C_DEFAULT, "They aren't in your clan!\n\r", ch );
    return;
  }

  if ( !IS_SET( victim->act, PLR_CSKILL ) )
  {
    SET_BIT( victim->act, PLR_CSKILL );
    act(AT_PINK, "$N bestowed with the Transmute skill.", ch, NULL, victim,
	TO_CHAR );
    send_to_char( AT_PINK, "You have been given the Transmute skill.\n\r",
		  victim );
  }
  else
  {
    REMOVE_BIT( victim->act, PLR_CSKILL );
    act(AT_PINK, "Transmute denied from $N.", ch, NULL, victim, TO_CHAR );
    send_to_char( AT_PINK, "You have been denied the Transmute skill.\n\r",
		  victim );
  }
}
*/

/*
 * Illuminati transmute skill, must be given to a member by the deity
 */
void do_transmute( CHAR_DATA *ch, char *argument )
{
  OBJ_DATA *obj;

/*  if ( ch->clan != 1 || !IS_SET(ch->act, PLR_CSKILL)
      || IS_NPC(ch) ) */
  if ( !can_use_skpell( ch, gsn_transmute ))
  {
    send_to_char(C_DEFAULT, "You cannot do that.\n\r", ch );
    return;
  }

  if ( !( obj = get_obj_carry( ch, argument ) ) )
  {
    send_to_char(C_DEFAULT, "You do not have that item.\n\r", ch );
    return;
  }
/*
  if ( obj->level > 0 )
    chance = (ch->level * 75) / obj->level;
  else 
    chance = 99;
*/
  if ( number_percent( ) < ch->pcdata->learned[gsn_transmute] )
  {
    obj->extra_flags |= ITEM_NO_DAMAGE;
    act(AT_PINK, "$p transmuted.", ch, obj, NULL, TO_CHAR );
    return;
  }
  else
  {
  act(AT_PINK, "You failed!  $p exploded in your face!", ch, obj, NULL,
      TO_CHAR);
  extract_obj(obj);
  }
  return;
}


/*
 * ??? Doomshield skill
 */
void spell_doomshield( int sn, int level, CHAR_DATA *ch, void *vo )
{
  AFFECT_DATA af;

  if ( is_affected( ch, gsn_doomshield ) )
    return;

  af.type = sn;
  af.level = ch->level;
  af.duration = 5;
  af.location = APPLY_NONE;
  af.modifier = 0;
  af.bitvector = 0;
  affect_to_char(ch, &af);
  act( AT_PURPLE, "$n makes a pact with the lowest of darknesses.", ch,
NULL,
       NULL, TO_ROOM );
  send_to_char(AT_PURPLE, "You sell your soul.\n\r", ch );
  return;
}

void spell_unholystrength( int sn, int level, CHAR_DATA *ch, void *vo )
{
  AFFECT_DATA af;

  if ( ch->fighting ||  is_affected( ch, gsn_unholystrength ) )
  {
    send_to_char(C_DEFAULT, "You failed.\n\r",ch);
    return;
  }

  af.type = sn;
  af.duration = ch->level;
  af.level = ch->level;
  af.location = APPLY_NONE;
  af.modifier = 0;
  af.bitvector = 0;
  affect_to_char( ch, &af );
  
  af.location = APPLY_STR;
  af.modifier = 5;
  affect_to_char(ch, &af);

  af.location = APPLY_DAMROLL;
  af.modifier = 50;
  affect_to_char(ch, &af);
/*  ch->ctimer = 17;*/

  send_to_char(AT_DGREEN, "You feel the strength of the Unholy Plague run through you.\n\r",ch);
  return;
}

void spell_image( int sn, int level, CHAR_DATA *ch, void *vo )
{
  AFFECT_DATA af;

  if ( ch->combat_timer )
  {
    send_to_char(AT_BLUE, "You can't right now.\n\r",ch);
    return;
  }

  if (is_affected(ch, gsn_image) )
    return;

  af.location = APPLY_NONE;
  af.modifier = 0;
  af.duration = ch->level;
  af.bitvector = 0;
  af.type = sn;
  affect_to_char( ch, &af );
  send_to_char(AT_CYAN, "You are surrounded by images of the Talisman.\n\r", ch);
  act(AT_CYAN, "$n suddenly splits into many images.",ch,NULL,NULL,TO_ROOM);
  return;
}
/* END */
