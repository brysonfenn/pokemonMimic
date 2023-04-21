#include "conditions.h"

#include "pokemon.h"

int inflict_condition(Condition condition, int accuracy, struct pokemon* pok) {
	int random = rand() % 100;
	if (condition == POISONED && random < accuracy) {
		printf("%s was badly poisoned!\n", pok->name); sleep(2);
	}
	return 0;
}

int attack_do_nothing(Condition condition, int accuracy, struct pokemon* pok) {
	return 0;
}