#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <stdbool.h>

struct Pokemon;

//All condition types
typedef enum { NO_CONDITION, SPEED_STAT, ATTACK_STAT, DEFENSE_STAT, SP_ATTACK_STAT, 
                SP_DEFENSE_STAT, ACCURACY_STAT, EVASIVENESS_STAT,
                POISONED, PARALYZED, ASLEEP, BURNED, FROZEN, 
                CONFUSED, SEEDED, FIRE_SPINNED, FLINCHED } Condition;

//Inflict condition on pok given, handle accuracy
int inflict_condition(Condition condition, int accuracy, struct Pokemon * pok);

//Add hidden condition to pokemon, return false if condition could not be added
bool add_hidden_condition(struct Pokemon * pok, Condition condition);

//Check if a given pokemon has a specified hidden condition
bool has_hidden_condition(struct Pokemon * pok, Condition condition);

//Remove a hidden condition if it exists, return 1 if it is not there
bool remove_hidden_condition(struct Pokemon * pok, Condition condition);

//Remove all hidden conditions form a given pokemon
void remove_all_hidden_conditions(struct Pokemon * pok);

//Handle poisoning, leech seed, etc.
int handle_end_conditions();

//print a pokemon's condition abbreviation
void add_condition_string(char * str, struct Pokemon * pok);

#endif // CONDITIONS_H