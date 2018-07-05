#define linux 1
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

void do_bet( CHAR_DATA* ch, char* argument ) {
	char arg1[MAX_STRING_LENGTH];
	char arg2[MAX_STRING_LENGTH];
	char buf[MAX_STRING_LENGTH];
	bool won = TRUE;
	int bet;
	int c;
	int winnings = 0;
	int chmoney;
	int luck[3];
	char symbols[8] = { ' ', '_', '-', '=', '@', '#', '7', '$'};
	argument = one_argument ( argument, arg1 );
	argument = one_argument ( argument, arg2 );
	if ( ( arg1[0] == '\0' ) || ( !is_number( arg1 ) ) || ( arg2 == '\0' ) ) {
		send_to_char( C_DEFAULT, "Proper syntax: Bet <amount> <type>\n\r", ch );
		return;
	}
	bet = atoi( arg1 );
	if ( bet <= 0 ) {
		send_to_char( C_DEFAULT, "Your bet must be a positive number.\n\r", ch );
		return;
	}

/*	if ( !str_cmp( arg2, "gold" ) )
		chmoney = ch -> money.gold;
	else
	if ( !str_cmp( arg2, "silver" ) )
		chmoney = ch -> money.silver;
	else
	if ( !str_cmp( arg2, "copper" ) )
		chmoney = ch -> money.copper;
	else {
		send_to_char( C_DEFAULT, "That is not a valid type of money\n\r", ch );
		return;
	}*/
	chmoney = ch->gold;
	if ( chmoney < bet  ) {
		send_to_char ( C_DEFAULT, "You don't have that much!\n\r", ch );
		/*sprintf( buf, "You've got %d %s.\n\r", chmoney, arg2 );
		send_to_char ( C_DEFAULT, buf, ch );*/
		return;
	}
	if ( IS_SET ( ch-> in_room -> room_flags, ROOM_SLOTS ) ) {
		for (c = 0; c < 3; c++) {
			luck[c] = number_range( 0, 128 );
			luck[c] = 7 - ( luck[c] / 2 );
		}
		if ( ( luck[0] == 7 ) &&
			( luck[1] == 7 ) &&
			( luck[2] == 7 ) )
				winnings = bet * 200;
		else
		if ( ( luck[0] == 6 ) &&
			( luck[1] == 6 ) &&
			( luck[2] == 6 ) )
				winnings = bet * 100;
		else
		if ( ( luck[0] == 5 ) &&
			( luck[1] == 5 ) &&
			( luck[2] == 5 ) )
				winnings = bet * 50;
		else
		if ( ( luck[0] == 4 ) &&
			( luck[1] == 4 ) &&
			( luck[2] == 4 ) )
				winnings = bet * 25;
		else
		if ( ( luck[0] == 4 ) ||
			( luck[1] == 4 ) ||
			( luck[2] == 4 ) )
				winnings = bet * 2;
		else
		if ( ( luck[0] == 3 ) &&
			( luck[1] == 3 ) &&
			( luck[2] == 3 ) )
				winnings = bet * 20;
		else
		if ( ( luck[0] == 2 ) &&
			( luck[1] == 2 ) &&
			( luck[2] == 2 ) )
				winnings = bet * 10;
		else
		if ( ( luck[0] == 1 ) &&
			( luck[1] == 1 ) &&
			( luck[2] == 1 ) )
				winnings = bet * 5;
		else
		if ( ( luck[0] == 1 ) &&
			( luck[1] == 2 ) &&
			( luck[2] == 3 ) )
				winnings = bet * 30;
		else {
			winnings -= bet;
			won = FALSE;
		}
		chmoney += winnings;
		ch->gold = chmoney;
/*		if ( !str_cmp( arg2, "gold" ) )
			ch->money.gold = chmoney;
		else
		if ( !str_cmp( arg2, "silver" ) )
			ch->money.silver = chmoney;
		else
		if ( !str_cmp( arg2, "copper" ) )
			ch->money.copper = chmoney; */
		sprintf( buf, "\n.---.---.---.\n| %c | %c | %c |\n'---'---'---'\n\r",
			symbols[luck[0]],
			symbols[luck[1]],
			symbols[luck[2]]);
		send_to_char( C_DEFAULT, buf, ch );
		if ( won == TRUE ) {
			sprintf( buf, "Congratulations, you won %d!", winnings );
			send_to_char( AT_CYAN, buf, ch );
		}
		else
			send_to_char( AT_RED, "Sorry, you lost!\n\r", ch );
	}
	else {
		send_to_char( C_DEFAULT, "You can't do that here.\n\r", ch);
		return;
	}
}
