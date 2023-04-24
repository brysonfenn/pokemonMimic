#ifndef TYPINGS_H
#define TYPINGS_H

struct pokemon;

typedef enum {NO_TYPE, NORMAL, FIRE, WATER, ELECTRIC, GRASS, ICE, FIGHTING, POISON, GROUND,
                FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON } Type;


int handle_effectiveness(Type moveType, struct pokemon * pok, int damage);

#endif // TYPINGS_H