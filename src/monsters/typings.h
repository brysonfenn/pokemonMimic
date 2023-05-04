#ifndef TYPINGS_H
#define TYPINGS_H

struct pokemon;

//Typings
typedef enum {NO_TYPE, NORMAL, FIRE, WATER, ELECTRIC, GRASS, ICE, FIGHTING, POISON, GROUND,
                FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON } Type;

//Returns damage after applying effectiveness
//Also prints message about effectiveness, if applicable
int get_damage_after_effectiveness(Type moveType, struct pokemon * pok, int damage);

#endif // TYPINGS_H