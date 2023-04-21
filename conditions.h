#ifndef CONDITIONS_H
#define CONDITIONS_H

typedef enum { NO_CONDITION, POISONED, PARALYZED, ASLEEP, BURNED, FROZEN } Condition;

int inflict_condition(Condition condition, int accuracy);

int attack_do_nothing(Condition condition, int accuracy);

#endif // CONDITIONS_H