#include "conditions.h"

#include "pokemon.h"
#include "../player.h"
#include "../print_utils.h"

int inflict_condition(Condition condition, int accuracy, struct pokemon* pok) {
	int random = rand() % 100;
	
	text_box_cursors(0);

	if (condition == POISONED && random < accuracy) {
		if (pok->visible_condition != NO_CONDITION) {
			printw("%s could not be poisoned!", pok->name); refresh(); sleep(2);
			return 1;
		}
		if (pok != player.current_pokemon) printw("Enemy ");
		printw("%s was badly poisoned!", pok->name); refresh(); sleep(2);
		pok->visible_condition = POISONED;
	}
	else if (condition == ASLEEP && random < accuracy) {
		if (pok->visible_condition != NO_CONDITION) {
			printw("%s could not be put to sleep!", pok->name); refresh(); sleep(2);
			return 1;
		}
		if (pok != player.current_pokemon) printw("Enemy ");
		printw("%s fell asleep!", pok->name); refresh(); sleep(2);
		pok->visible_condition = ASLEEP;
		pok->sleep_count = (rand() % 4) + 1;
	}
	else if (condition == SEEDED && random < accuracy) {
		if (pok->hidden_condition != NO_CONDITION) {
			printw("But it failed!"); refresh(); sleep(2);
			return 1;
		}
		if (pok != player.current_pokemon) printw("Enemy ");
		printw("%s was seeded!", pok->name); refresh(); sleep(2);
		pok->hidden_condition = SEEDED;
	}
	return 0;
}

int attack_do_nothing(Condition condition, int accuracy, struct pokemon* pok) {
	return 0;
}

int handle_end_conditions() {
	pokemon * player_pok = player.current_pokemon;
	pokemon * enemy_pok = player.enemy_pokemon;

	clear(); printBattle();
	if (player_pok->visible_condition == POISONED) {
		text_box_cursors(0);
		printw("%s was hurt by poison!", player_pok->name);
		player_pok->currentHP -= ((player_pok->maxHP / 16) + 1);
		if (player_pok->currentHP < 0) player_pok->currentHP = 0;
		refresh(); sleep(2);
	}

	if (enemy_pok->visible_condition == POISONED) {
		text_box_cursors(1);
		printw("Enemy %s was hurt by poison!", enemy_pok->name);
		enemy_pok->currentHP -= ((enemy_pok->maxHP / 16) + 1);
		if (enemy_pok->currentHP < 0) enemy_pok->currentHP = 0;
		refresh(); sleep(2);
	}
	if (player_pok->currentHP == 0 || enemy_pok->currentHP == 0) return 2;

	clear(); printBattle();
	
	int sappedHP;
	if (player_pok->hidden_condition == SEEDED) {
		text_box_cursors(0);
		printw("%s's HP was sapped!", player_pok->name);
		sappedHP = ((player_pok->maxHP / 8) + 1);
		//Give HP equal to taken HP
		if (sappedHP > player_pok->currentHP) { enemy_pok->currentHP += player_pok->currentHP; player_pok->currentHP = 0; }
		else {enemy_pok->currentHP += sappedHP; player_pok->currentHP -= sappedHP;}
		if (enemy_pok->currentHP > enemy_pok->maxHP) enemy_pok->currentHP = enemy_pok->maxHP;
		refresh(); sleep(2);
	}
	if (enemy_pok->hidden_condition == SEEDED) {
		text_box_cursors(1);
		printw("Enemy %s's HP was sapped!", enemy_pok->name);
		sappedHP = ((enemy_pok->maxHP / 8) + 1);
		//Give HP equal to taken HP
		if (sappedHP > enemy_pok->currentHP) { player_pok->currentHP += enemy_pok->currentHP; enemy_pok->currentHP = 0; }
		else {player_pok->currentHP += sappedHP; enemy_pok->currentHP -= sappedHP;}
		if (player_pok->currentHP > player_pok->maxHP) player_pok->currentHP = player_pok->maxHP;
		refresh(); sleep(2);
	}

	return 0;
}

void print_condition(struct pokemon * pok) {
	if (pok->visible_condition == NO_CONDITION || pok->currentHP == 0)
		printw(" ");
	else if (pok->visible_condition == POISONED)
		printw("PSN");
	else if (pok->visible_condition == ASLEEP)
		printw("SLP");
	refresh(); 
}