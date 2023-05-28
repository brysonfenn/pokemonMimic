#include "conditions.h"

#include <stdlib.h>

#include "pokemon.h"
#include "../player.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"


//Inflict condition on pok given, handle accuracy
int inflict_condition(Condition condition, int accuracy, struct Pokemon* pok) {
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
			if (has_hidden_condition(pok, SEEDED)) {
				printw("But it failed!"); refresh(); sleep(2);
				return 1;
			}
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s was seeded!", pok->name); refresh(); sleep(2);
			add_hidden_condition(pok, SEEDED);
			break;

		case CONFUSED:
			if (has_hidden_condition(pok, CONFUSED)) {
				printw("%s could not be confused!", pok->name); refresh(); sleep(2);
				return 1;
			}
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s is now confused!", pok->name); refresh(); sleep(2);
			add_hidden_condition(pok, CONFUSED);
			pok->confusion_count = (rand() % 4) + 1;
			break;

		case FLINCHED:
			add_hidden_condition(pok, FLINCHED);
			break;

		default:
			printw("Tried to inflict unrecognized condition"); text_box_cursors(TEXT_BOX_NEXT_LINE);
			printw("with code: %d", condition); refresh(); sleep(2);
			break;
	}

	return 0;
}


//Add hidden condition to pokemon, return 1 if condition could not be added
bool add_hidden_condition(struct Pokemon * pok, Condition condition) {

	if (has_hidden_condition(pok, condition)) return false;

	pok->num_hidden_conditions++;
	pok->hidden_conditions = realloc(pok->hidden_conditions, pok->num_hidden_conditions * sizeof(Condition));

	pok->hidden_conditions[pok->num_hidden_conditions - 1] = condition;
	
	return true;
}

//Check if a given pokemon has a specified hidden condition
bool has_hidden_condition(struct Pokemon * pok, Condition condition) {
	for (int i = 0; i < pok->num_hidden_conditions; i++) {
		if (pok->hidden_conditions[i] == condition) return true;
	}
	return false;
}

//Remove a hidden condition if it exists, return false if it is not there
bool remove_hidden_condition(struct Pokemon * pok, Condition condition) {
	bool found_condition = false;
	int i = 0;

	//Locate position of the condition
	for (i = 0; i < pok->num_hidden_conditions; i++) {
		if (pok->hidden_conditions[i] == condition) {
			pok->hidden_conditions[i] = NO_CONDITION;
			found_condition = true;
			break;
		}
	}
	if (!found_condition) return false;

	//Adjust indexes of the hidden conditions if it was found
	for (; i < pok->num_hidden_conditions-1; i++) {
		pok->hidden_conditions[i] = pok->hidden_conditions[i+1];
	}

	pok->num_hidden_conditions--;
	pok->hidden_conditions = realloc(pok->hidden_conditions, pok->num_hidden_conditions * sizeof(Condition));
	return found_condition;
}

//Remove all hidden conditions form a given pokemon
void remove_all_hidden_conditions(struct Pokemon * pok) {
	if (pok->num_hidden_conditions == 0) return;
	pok->hidden_conditions = realloc(pok->hidden_conditions, sizeof(Condition));
	pok->num_hidden_conditions = 0;
}


//Handle poisoning, leech seed, etc.
int handle_end_conditions() {
	Pokemon * player_pok = player.current_pokemon;
	Pokemon * enemy_pok = player.enemy_pokemon;
	char player_condition_text[16] = "NOT FOUND";
	char enemy_condition_text[16] = "NOT FOUND";

	//If either pokemon fainted, do not handle conditions
	if (player_pok->currentHP <= 0 || enemy_pok->currentHP <= 0) {
		return 0;
	}

	//Remove flinched hidden condition
	remove_hidden_condition(player_pok, FLINCHED);
	remove_hidden_condition(enemy_pok, FLINCHED);

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
	if (has_hidden_condition(player_pok, SEEDED)) {
		text_box_cursors(TEXT_BOX_BEGINNING);
		printw("%s's HP was sapped!", player_pok->name);
		sappedHP = ((player_pok->maxHP / 8) + 1);
		//Give HP equal to taken HP
		if (sappedHP > player_pok->currentHP) { enemy_pok->currentHP += player_pok->currentHP; player_pok->currentHP = 0; }
		else {enemy_pok->currentHP += sappedHP; player_pok->currentHP -= sappedHP;}
		if (enemy_pok->currentHP > enemy_pok->maxHP) enemy_pok->currentHP = enemy_pok->maxHP;
		refresh(); sleep(2);
	}
	if (has_hidden_condition(enemy_pok, SEEDED)) {
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
void add_condition_string(char * str, struct Pokemon * pok) {
	if (pok->visible_condition == NO_CONDITION || pok->currentHP == 0)
		sprintf(str, "%s ", str);
	else if (pok->visible_condition == POISONED)
		sprintf(str, "%s PSN", str);
	else if (pok->visible_condition == BURNED)
		sprintf(str, "%s BRN", str);
	else if (pok->visible_condition == PARALYZED)
		sprintf(str, "%s PRZ", str);
	else if (pok->visible_condition == ASLEEP)
		sprintf(str, "%s SLP", str);
}