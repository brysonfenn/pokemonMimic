#ifndef TRAINER_H
#define TRAINER_H

#include <stdint.h>
#include <stdbool.h>

#include "../monsters/pokemon_id_list.h"

typedef struct Trainer {
    char name[32];
    int8_t num_in_party;
    Pokemon_id party_id_list[6];
    int8_t levels[6];
    bool already_battled;
} Trainer;

//Battle a trainer with a random name and random # of pokemon
int battle_trainer(Trainer * trainer);

//Get a random name (could be female or male)
char * get_random_name();

#endif // TRAINER_H