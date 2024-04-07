#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <stdbool.h>
#include <stdint.h>

struct Pokemon;

//All condition types
typedef enum { NO_CONDITION, SPEED_STAT, ATTACK_STAT, DEFENSE_STAT, SP_ATTACK_STAT, 
                SP_DEFENSE_STAT, ACCURACY_STAT, EVASIVENESS_STAT,
                POISONED, PARALYZED, ASLEEP, BURNED, FROZEN, 
                CONFUSED, SEEDED, FIRE_SPINNED, TIGHT_HOLD, SAND_TOMBED, INGRAINED, 
                REPEAT_MOVE, DISABLED, YAWNED, PROTECTED, CURSED, FLINCHED } Condition;

//Inflict condition on pok given, handle accuracy
int inflict_condition(Condition condition, int accuracy, struct Pokemon * pok, int damage);

//Inflict a condition on self
int self_inflict_condition(Condition condition, int accuracy, struct Pokemon * pok, int damage);

//Add hidden condition to pokemon, return false if condition could not be added
bool add_hidden_condition(struct Pokemon * pok, Condition condition, int8_t value);

//Check if a given pokemon has a specified hidden condition
bool has_hidden_condition(struct Pokemon * pok, Condition condition);

//Decrement the value associated with a given condition, return value, or return 0 if condition not found
int decrement_hidden_condition_val(struct Pokemon * pok, Condition condition);

//Returns the value associated with a given condition, return value, or return -1 if condition not found
int get_hidden_condition_val(struct Pokemon * pok, Condition condition);

//Remove a hidden condition if it exists, return 1 if it is not there
bool remove_hidden_condition(struct Pokemon * pok, Condition condition);

//Remove all hidden conditions form a given pokemon
void remove_all_hidden_conditions(struct Pokemon * pok);

//Handle poisoning, leech seed, etc.
int handle_end_conditions(struct Pokemon * pok);

//print a pokemon's condition abbreviation
void add_condition_string(char * str, struct Pokemon * pok);

//Return true if Pokemon has a condition that stops escape or switching
bool conditions_pok_is_stuck(struct Pokemon * pok);

#endif // CONDITIONS_H