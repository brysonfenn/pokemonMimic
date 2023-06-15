#include "conditions.h"

#include <stdlib.h>

#include "pokemon.h"
#include "../player.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../print/print_battle.h"


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
	
	text_box_cursors(TEXT_BOX_BEGINNING);
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
			printw("%s is now paralyzed.", pok->name); 
			text_box_cursors(TEXT_BOX_NEXT_LINE);
			printw("It may be unable to move!"); refresh(); sleep(2);
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
			add_hidden_condition(pok, SEEDED, 0);
			break;

		case CONFUSED:
			if (has_hidden_condition(pok, CONFUSED)) {
				printw("%s could not be confused!", pok->name); refresh(); sleep(2);
				return 1;
			}
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s is now confused!", pok->name); refresh(); sleep(2);
			add_hidden_condition(pok, CONFUSED, 0);
			pok->confusion_count = (rand() % 4) + 1;
			break;

		case FLINCHED:
			add_hidden_condition(pok, FLINCHED, 0);
			break;
		
		case BIND:
			if (has_hidden_condition(pok, BIND)) return 2;
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s was bound tightly!", pok->name); refresh(); sleep(2);

			//Bind lasts 2-5 turns
			add_hidden_condition(pok, BIND, (rand() % 4) + 2);
			break;
		
		case WRAPPED:
			if (has_hidden_condition(pok, WRAPPED)) return 2;
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s was wrapped tightly!", pok->name); refresh(); sleep(2);

			//Bind lasts 2-5 turns
			add_hidden_condition(pok, WRAPPED, (rand() % 4) + 2);
			break;

		case SAND_TOMBED:
			if (has_hidden_condition(pok, SAND_TOMBED)) return 2;
			if (pok != player.current_pokemon) printw(ENEMY_TEXT);
			printw("%s was trapped in a sand tomb!", pok->name); refresh(); sleep(2);

			//Bind lasts 2-5 turns
			add_hidden_condition(pok, SAND_TOMBED, (rand() % 4) + 2);
			break;

		default:
			printw("Tried to inflict unrecognized condition"); text_box_cursors(TEXT_BOX_NEXT_LINE);
			printw("with code: %d", condition); refresh(); sleep(2);
			break;
	}

	return 0;
}


//Add hidden condition to pokemon, return 1 if condition could not be added
bool add_hidden_condition(struct Pokemon * pok, Condition condition, int8_t value) {
	if (has_hidden_condition(pok, condition)) return false;

	pok->num_hidden_conditions++;
	pok->hidden_conditions = realloc(pok->hidden_conditions, pok->num_hidden_conditions * sizeof(Condition));
	pok->hidden_conditions[pok->num_hidden_conditions - 1] = condition;

	pok->hidden_condition_values = realloc(pok->hidden_condition_values, pok->num_hidden_conditions * sizeof(int8_t));
	pok->hidden_condition_values[pok->num_hidden_conditions - 1] = value;

	return true;
}

//Check if a given pokemon has a specified hidden condition
bool has_hidden_condition(struct Pokemon * pok, Condition condition) {
	for (int i = 0; i < pok->num_hidden_conditions; i++) {
		if (pok->hidden_conditions[i] == condition) return true;
	}
	return false;
}

//Decrement the value associated with a given condition, return false if that value is 0 or if condition not found
bool decrement_hidden_condition_val(struct Pokemon * pok, Condition condition) {
	for (int i = 0; i < pok->num_hidden_conditions; i++) {
		if (pok->hidden_conditions[i] == condition && pok->hidden_condition_values[i] > 0) {
			pok->hidden_condition_values[i]--;
			return true;
		}
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
			pok->hidden_condition_values[i] = 0;
			found_condition = true;
			break;
		}
	}

	//If the condition was not found, stop here
	if (!found_condition) return false;

	//Adjust indexes of the hidden conditions if it was found
	for (; i < pok->num_hidden_conditions-1; i++) {
		pok->hidden_conditions[i] = pok->hidden_conditions[i+1];
		pok->hidden_condition_values[i] = pok->hidden_condition_values[i+1];
	}

	pok->num_hidden_conditions--;

	//Resize the array to num conditions as long as array isn't zero. If it is zero, size should be 1 Condition
	int new_size = pok->num_hidden_conditions * sizeof(Condition);
	if (new_size == 0) new_size = sizeof(Condition);
	pok->hidden_conditions = realloc(pok->hidden_conditions, new_size);

	// Get what size of hidden conditions value array should be and resize
	new_size = ((new_size / sizeof(Condition)) * sizeof(int8_t));
	pok->hidden_condition_values = realloc(pok->hidden_condition_values, new_size);

	return found_condition;
}

//Remove all hidden conditions form a given pokemon
void remove_all_hidden_conditions(struct Pokemon * pok) {
	if (pok->num_hidden_conditions == 0) return;
	pok->hidden_conditions = realloc(pok->hidden_conditions, sizeof(Condition));
	pok->hidden_condition_values = realloc(pok->hidden_condition_values, sizeof(int8_t));
	pok->num_hidden_conditions = 0;
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