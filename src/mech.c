/* Mech code by Eleven */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void do_mdesc( CHAR_DATA *ch, char *argument )
{
 char buf[MAX_STRING_LENGTH];

 if(IS_NPC(ch))
 {
  bug("Mdesc: NPC!! Someone...help...", 0);
  return;
 }

 if( argument[0] == '\0' )
 {
  send_to_char(C_DEFAULT, "Change Mech description to what?\n\r",ch);
  return;
 }

 buf[0] = '\0';

 if( !str_cmp("show", argument) )
 {
  if(ch->pcdata->mdesc == NULL)
  {
   send_to_char(C_DEFAULT,"You do not have a Mech description.\n\r",ch);
   return;
  }
  else
  {
   sprintf(buf,"&wMech description is (%s&w).\n\r", ch->pcdata->mdesc);
   send_to_char(C_DEFAULT,buf,ch);
   return;
  }
 }

 if( !str_cmp("none", argument) )
 {
  free_string(ch->pcdata->mdesc);
  ch->pcdata->mdesc = NULL;
  send_to_char(C_DEFAULT,"Mech description cleared.\n\r",ch);
  return;
 }

 strcpy(buf, argument);
 free_string(ch->pcdata->mdesc);
 ch->pcdata->mdesc = str_dup(buf);
 buf[0] = '\0';
 sprintf(buf,"&wMech description set to (%s&w).\n\r",argument);
 send_to_char(C_DEFAULT,buf,ch);
 return;
}

void do_exitmech(CHAR_DATA *ch, char *argument)
{
 OBJ_DATA *obj;

 if(IS_NPC(ch))
  return;

 if(ch->pcdata->in_mech == NULL)
 {
  send_to_char(C_DEFAULT,"You are not in a Mech.\n\r",ch);
  return;
 }
 else
 {
  free_string(ch->pcdata->in_mech);
  ch->pcdata->in_mech = NULL;
  obj_from_char(obj);
  obj_to_room(obj, ch->in_room);
  act(C_DEFAULT,"You get out of $p.",ch,obj,NULL,TO_CHAR);
  act(C_DEFAULT,"$n gets out of $p.",ch,obj,NULL,TO_ROOM);

  return;
 }
}
/*
void do_fire( CHAR_DATA *ch, char *argument )
{
 char arg1[MAX_INPUT_LENGTH];
 char arg2[MAX_INPUT_LENGTH];
 char buf[MAX_STRING_LENGTH];
 CHAR_DATA *victim;
 ROOM_INDEX_DATA *to_room;
 ROOM_INDEX_DATA *in_room;
 OBJ_DATA *obj;
 EXIT_DATA *pexit;
 int dir = 0;
 int dist = 0;
 int MAX_DIST = 10;
 extern char *dir_noun[];

 argument = one_argument(argument, arg1);
 argument = one_argument(argument, arg2);

 in_room = ch->in_room;
 to_room = ch->in_room;

 if( arg1[0] == '\0' )
 {
  send_to_char(C_DEFAULT,"You rest your finger on the trigger.\n\r",ch);
  return;
 }
 if( arg2[0] == '\0' )
 {
  send_to_char(C_DEFAULT,"You must specify a target or direction.\n\r",ch);
  return;
 }

 for( dir = 0; dir < 6; dir++ )
  if( arg2[0] == dir_name[dir][0]       &&
      !str_prefix(arg2, dir_name[dir]) )
  break;

  if( dir = 6 )
  {
   send_to_char(C_DEFAULT,"Fire where?\n\r",ch);
   return;
  }
  if( arg2 == '\0' || arg3 == '\0' )
  {
   send_to_char(C_DEFAULT,"Syntax:  fire <type> <where/who>\n\r",ch);
   return;
  }
  if( !str_cmp(arg2, "missile") || !str_cmp(arg2, "mis") )
  {
   send_to_char(C_DEFAULT,"You pull the trigger...\n\r",ch);
   if( ch->mechdata->cur_missile == 0 )
   {
    send_to_char(C_DEFAULT,"&w[&R***&w] &RMissile salvo empty!\n\r",ch);
    return;
   }

   send_to_char(C_DEFAULT,"&w[&G***&w] &GMissile away!\n\r",ch);
   ch->mechdata->cur_missile--;


*/

void do_radar( CHAR_DATA *ch, const ROOM_INDEX_DATA *room,char *argument )
{
 CHAR_DATA *victim;
 DESCRIPTOR_DATA *d;
 EXIT_DATA *pexit;
 CHAR_DATA *target = room->people;
 char  buf[MAX_STRING_LENGTH];
 bool  found;
 bool  in_range;
 bool  proxim;
 int   dir;
 int   distance;
 room = ch->in_room;

 send_to_char(AT_GREEN,"&gYou glance at your &Gradar&g, and see:\n\r",ch);
 found = FALSE;
 in_range = FALSE;
 proxim = FALSE;

 for( d = descriptor_list; d; d = d->next )
 {
  if( d->connected == CON_PLAYING 
      && (victim = d->character)
      && !IS_NPC(victim)
      && victim->in_room 
      && ((victim->in_room->area == ch->in_room->area)
          || (ch->level >= LEVEL_IMMORTAL )))
  {
   found = TRUE;
   for( dir = 0; dir < 6; dir++ )
   {
    room = ch->in_room;
    for( distance = 1; distance < 4; distance++ )
    {
     pexit = room->exit[dir];
     if((pexit == NULL) || (pexit->to_room == NULL ||
     (IS_SET(pexit->exit_info, EX_CLOSED))))
     break;

     in_range = TRUE;
     room = pexit->to_room;
    }
   }
/*   while(target != NULL)  proxim = TRUE; */
   sprintf(buf, "&g*&GBLIP&g*             &z%-10s %s\n\r",
		victim->name,
	        in_range == TRUE ? "&W(&GIN RANGE&W)" : "");
   send_to_char(C_DEFAULT,buf,ch);
  }
 }
 if( !found )
  send_to_char(AT_GREEN,"No targets found.\n\r",ch);
 return;
}

