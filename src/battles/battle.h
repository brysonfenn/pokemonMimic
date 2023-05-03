#ifndef BATTLE_H
#define BATTLE_H

#include "../monsters/pokemon.h"

#define BATTLE_WHITE_OUT 1
#define BATTLE_WIN 2

#define BATTLE_SELECT_1_X 1
#define BATTLE_SELECT_2_X 19
#define BATTLE_SELECT_3_X 37
#define SELECT_Y 6

int initiate_battle(pokemon enemyPoke);

#endif