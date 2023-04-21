#ifndef CONDITIONS_H
#define CONDITIONS_H

struct pokemon;

typedef enum { NO_CONDITION, POISONED, PARALYZED, ASLEEP, BURNED, FROZEN } Condition;

int inflict_condition(Condition condition, int accuracy, struct pokemon * pok);

int attack_do_nothing(Condition condition, int accuracy, struct pokemon * pok);

#endif // CONDITIONS_H