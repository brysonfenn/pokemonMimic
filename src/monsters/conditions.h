#ifndef CONDITIONS_H
#define CONDITIONS_H

struct pokemon;

//All condition types
typedef enum { NO_CONDITION, SPEED_STAT, ATTACK_STAT, DEFENSE_STAT, SP_ATTACK_STAT, 
                SP_DEFENSE_STAT, ACCURACY_STAT, EVASIVENESS_STAT,
                POISONED, PARALYZED, ASLEEP, BURNED, FROZEN, SEEDED } Condition;

//Inflict condition on pok given, handle accuracy
int inflict_condition(Condition condition, int accuracy, struct pokemon * pok);

//Filler side-effect function that does nothing
int attack_do_nothing(Condition condition, int accuracy, struct pokemon * pok);

//Handle poisoning, leech seed, etc.
int handle_end_conditions();

//print a pokemon's condition abbreviation
void add_condition(char * str, struct pokemon * pok);

#endif // CONDITIONS_H