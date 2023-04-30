#include "conditions.h"

#include "pokemon.h"
#include "../player.h"
#include "../print_utils.h"

int inflict_condition(Condition condition, int accuracy, struct pokemon* pok) {
	int random = rand() % 100;
	if (condition == POISONED && random < accuracy) {
		if (pok->visible_condition != NO_CONDITION) {
			printf("%s could not be poisoned!\n", pok->name); sleep(2);
			return 1;
		}
		if (pok != player.current_pokemon) printf("Enemy ");
		printf("%s was badly poisoned!\n", pok->name); sleep(2);
		pok->visible_condition = POISONED;
	}
	else if (condition == SEEDED && random < accuracy) {
		if (pok->hidden_condition != NO_CONDITION) {
			printf("But it failed!\n"); sleep(2);
			return 1;
		}
		if (pok != player.current_pokemon) printf("Enemy ");
		printf("%s was seeded!\n", pok->name); sleep(2);
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

	clearTerminal(); printBattle(); printf("\n\n\n");
	if (player_pok->visible_condition == POISONED) {
		printf("%s was hurt by poison!\n", player_pok->name);
		player_pok->currentHP -= ((player_pok->maxHP / 16) + 1);
		if (player_pok->currentHP < 0) player_pok->currentHP = 0;
		sleep(2);
	}
	if (enemy_pok->visible_condition == POISONED) {
		printf("Enemy %s was hurt by poison!\n", enemy_pok->name);
		enemy_pok->currentHP -= ((enemy_pok->maxHP / 16) + 1);
		if (enemy_pok->currentHP < 0) enemy_pok->currentHP = 0;
		sleep(2);
	}
	if (player_pok->currentHP == 0 || enemy_pok->currentHP == 0) return 2;

	clearTerminal(); printBattle(); printf("\n\n\n");
	
	int sappedHP;
	if (player_pok->hidden_condition == SEEDED) {
		printf("%s's HP was sapped'!\n", player_pok->name);
		sappedHP = ((player_pok->maxHP / 8) + 1);
		//Give HP equal to taken HP
		if (sappedHP > player_pok->currentHP) { enemy_pok->currentHP += player_pok->currentHP; player_pok->currentHP = 0; }
		else {enemy_pok->currentHP += sappedHP; player_pok->currentHP -= sappedHP;}
		if (enemy_pok->currentHP > enemy_pok->maxHP) enemy_pok->currentHP = enemy_pok->maxHP;
		sleep(2);
	}
	if (enemy_pok->hidden_condition == SEEDED) {
		printf("Enemy %s's HP was sapped!\n", enemy_pok->name);
		sappedHP = ((enemy_pok->maxHP / 8) + 1);
		//Give HP equal to taken HP
		if (sappedHP > enemy_pok->currentHP) { player_pok->currentHP += enemy_pok->currentHP; enemy_pok->currentHP = 0; }
		else {player_pok->currentHP += sappedHP; enemy_pok->currentHP -= sappedHP;}
		if (player_pok->currentHP > player_pok->maxHP) player_pok->currentHP = player_pok->maxHP;
		sleep(2);
	}

	return 0;
}

void print_condition(struct pokemon * pok) {
	if (pok->visible_condition == NO_CONDITION || pok->currentHP == 0)
		printf(" ");
	else if (pok->visible_condition == POISONED)
		printf("PSN");
}