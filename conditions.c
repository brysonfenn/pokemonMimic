#include "conditions.h"

#include "pokemon.h"

int inflict_condition(Condition condition, int accuracy, struct pokemon* pok) {
	int random = rand() % 100;
	if (condition == POISONED && random < accuracy) {
		if (pok->condition != NO_CONDITION) {
			return 1;
		}
		printf("%s was badly poisoned!\n", pok->name); sleep(2);
		pok->condition = POISONED;
	}
	return 0;
}

int attack_do_nothing(Condition condition, int accuracy, struct pokemon* pok) {
	return 0;
}

int handle_poison(struct pokemon * pok1, struct pokemon * pok2) {
	if (pok1->condition == POISONED) {
		printf("%s was hurt by poison!\n", pok1->name);
		pok1->currentHP -= ((pok1->maxHP / 16) + 1);
		if (pok1->currentHP < 0) pok1->currentHP = 0;
		sleep(2);
	}
	if (pok2->condition == POISONED) {
		printf("%s was hurt by poison!\n", pok2->name);
		pok2->currentHP -= ((pok2->maxHP / 16) + 1);
		if (pok2->currentHP < 0) pok2->currentHP = 0;
		sleep(2);
	}
}

void print_condition(struct pokemon * pok) {
	if (pok->condition == NO_CONDITION)
		printf(" ");
	else if (pok->condition == POISONED)
		printf("PSN");
}