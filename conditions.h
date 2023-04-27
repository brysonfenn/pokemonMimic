#ifndef CONDITIONS_H
#define CONDITIONS_H

struct pokemon;

typedef enum { NO_CONDITION, POISONED, PARALYZED, ASLEEP, BURNED, FROZEN, SEEDED } Condition;

int inflict_condition(Condition condition, int accuracy, struct pokemon * pok);

int attack_do_nothing(Condition condition, int accuracy, struct pokemon * pok);

int handle_end_conditions();

void print_condition(struct pokemon * pok);

#endif // CONDITIONS_H