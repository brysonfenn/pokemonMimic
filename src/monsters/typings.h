#ifndef TYPINGS_H
#define TYPINGS_H

#include <stdbool.h>

struct pokemon;

//Typings
typedef enum {NO_TYPE, NORMAL, FIRE, WATER, ELECTRIC, GRASS, ICE, 
                FIGHTING, POISON, GROUND, FLYING, PSYCHIC, BUG, 
                ROCK, GHOST, DRAGON, DARK, STEEL } Type;

#define SUPER_EFFECTIVE_FLAG 0x02
#define NOT_VERY_EFFECTIVE_FLAG 0x04
#define DOES_NOT_AFFECT_FLAG 0x08

//Returns damage after applying effectiveness
//Also prints message about effectiveness, if applicable
int get_damage_after_effectiveness(Type moveType, struct pokemon * pok, int damage, bool print_statements, int *flags);

char * get_typing_by_id(Type type);

Type get_type_id_by_string(char * str);

#endif // TYPINGS_H