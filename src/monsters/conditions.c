#include "conditions.h"

#include "pokemon.h"
#include "../player.h"
#include "../print_utils.h"
#include "../print_defines.h"

//Inflict condition on pok given, handle accuracy
int inflict_condition(Condition condition, int accuracy, struct pokemon* pok) {
	//Do not inflict a condition if fainted
	if (pok->currentHP <= 0) {
		return 0;
	}

	clear();
	printBattle();

	int random = rand() % 100;
	if (random >= accuracy) return 0;	//Only inflict condition if random is in accuracy bound
	
	text_box_cursors(TEXT_BOX_NEXT_LINE);
	switch (condition) {
		case POISONED:
			if (pok->visible_condition != NO_CONDITION) {
				printw("%s could not be poisoned!", pok->name); refresh(); sleep(2);
				return 1;
			}
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s was badly poisoned!", pok->name); refresh(); sleep(2);
			pok->visible_condition = POISONED;
			break;

		case PARALYZED:
			if (pok->visible_condition != NO_CONDITION) {
				printw("%s could not be paralyzed!", pok->name); refresh(); sleep(2);
				return 1;
			}
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s was paralyzed. It may be unable to move!", pok->name); refresh(); sleep(2);
			pok->visible_condition = PARALYZED;
			break;

		case ASLEEP:
			if (pok->visible_condition != NO_CONDITION) {
				printw("%s could not be put to sleep!", pok->name); refresh(); sleep(2);
				return 1;
			}
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s fell asleep!", pok->name); refresh(); sleep(2);
			pok->visible_condition = ASLEEP;
			pok->sleep_count = (rand() % 4) + 1;
			break;

		case BURNED:
			if (pok->visible_condition != NO_CONDITION) {
				printw("%s could not be burned!", pok->name); refresh(); sleep(2);
				return 1;
			}
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s was badly burned!", pok->name); refresh(); sleep(2);
			pok->visible_condition = BURNED;
			break;

		case SEEDED:
			if (pok->hidden_condition != NO_CONDITION) {
				printw("But it failed!"); refresh(); sleep(2);
				return 1;
			}
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s was seeded!", pok->name); refresh(); sleep(2);
			pok->hidden_condition = SEEDED;
			break;

		case CONFUSED:
			if (pok->hidden_condition != NO_CONDITION) {
				printw("%s could not be confused!", pok->name); refresh(); sleep(2);
				return 1;
			}
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s is now confused!", pok->name); refresh(); sleep(2);
			pok->hidden_condition = CONFUSED;
			pok->confusion_count = (rand() % 4) + 1;
			break;

		default:
			printw("Tried to inflict unrecognized condition"); text_box_cursors(TEXT_BOX_NEXT_LINE);
			printw("with code: %d", condition); refresh(); sleep(2);
			break;
	}

	return 0;
}

//Filler side-effect function that does nothing
int attack_do_nothing(Condition condition, int accuracy, struct pokemon* pok) {
	return 0;
}

//Handle poisoning, leech seed, etc.
int handle_end_conditions() {
	pokemon * player_pok = player.current_pokemon;
	pokemon * enemy_pok = player.enemy_pokemon;
	char player_condition_text[16] = "NOT FOUND";
	char enemy_condition_text[16] = "NOT FOUND";

	//If either pokemon fainted, do not handle conditions
	if (player_pok->currentHP <= 0 || enemy_pok->currentHP <= 0) {
		return 0;
	}

	//Assign text for poison and burn
	if (player_pok->visible_condition == POISONED) 
		sprintf(player_condition_text, "%s", "poison");
	else if (player_pok->visible_condition == BURNED) 
		sprintf(player_condition_text, "%s", "burn");
	if (enemy_pok->visible_condition == POISONED) 
		sprintf(enemy_condition_text, "%s", "poison");
	else if (enemy_pok->visible_condition == BURNED) 
		sprintf(enemy_condition_text, "%s", "burn");

	//Poison and burn
	clear(); printBattle();
	if (player_pok->visible_condition == POISONED || player_pok->visible_condition == BURNED) {
		text_box_cursors(TEXT_BOX_BEGINNING);
		printw("%s was hurt by %s!", player_pok->name, player_condition_text);
		player_pok->currentHP -= ((player_pok->maxHP / 16) + 1);
		if (player_pok->currentHP < 0) player_pok->currentHP = 0;
		refresh(); sleep(2);
	}

	if (enemy_pok->visible_condition == POISONED || enemy_pok->visible_condition == BURNED) {
		text_box_cursors(TEXT_BOX_BEGINNING);
		printw("%s%s was hurt by %s!", ENEMY_TEXT, enemy_pok->name, enemy_condition_text);
		enemy_pok->currentHP -= ((enemy_pok->maxHP / 16) + 1);
		if (enemy_pok->currentHP < 0) enemy_pok->currentHP = 0;
		refresh(); sleep(2);
	}
	if (player_pok->currentHP == 0 || enemy_pok->currentHP == 0) return 2;

	//Leech Seed
	clear(); printBattle();
	int sappedHP;
	if (player_pok->hidden_condition == SEEDED) {
		text_box_cursors(TEXT_BOX_BEGINNING);
		printw("%s's HP was sapped!", player_pok->name);
		sappedHP = ((player_pok->maxHP / 8) + 1);
		//Give HP equal to taken HP
		if (sappedHP > player_pok->currentHP) { enemy_pok->currentHP += player_pok->currentHP; player_pok->currentHP = 0; }
		else {enemy_pok->currentHP += sappedHP; player_pok->currentHP -= sappedHP;}
		if (enemy_pok->currentHP > enemy_pok->maxHP) enemy_pok->currentHP = enemy_pok->maxHP;
		refresh(); sleep(2);
	}
	if (enemy_pok->hidden_condition == SEEDED) {
		text_box_cursors(TEXT_BOX_BEGINNING);
		printw("%s%s's HP was sapped!", ENEMY_TEXT, enemy_pok->name);
		sappedHP = ((enemy_pok->maxHP / 8) + 1);
		//Give HP equal to taken HP
		if (sappedHP > enemy_pok->currentHP) { player_pok->currentHP += enemy_pok->currentHP; enemy_pok->currentHP = 0; }
		else {player_pok->currentHP += sappedHP; enemy_pok->currentHP -= sappedHP;}
		if (player_pok->currentHP > player_pok->maxHP) player_pok->currentHP = player_pok->maxHP;
		refresh(); sleep(2);
	}
	if (player_pok->currentHP == 0 || enemy_pok->currentHP == 0) return 2;

	return 0;
}

//print a pokemon's condition abbreviation
void add_condition(char * str, struct pokemon * pok) {
	if (pok->visible_condition == NO_CONDITION || pok->currentHP == 0)
		sprintf(str, "%s ", str);
	else if (pok->visible_condition == POISONED)
		sprintf(str, "%s PSN", str);
	else if (pok->visible_condition == PARALYZED)
		sprintf(str, "%s PRZ", str);
	else if (pok->visible_condition == ASLEEP)
		sprintf(str, "%s SLP", str);
}