#ifndef BATTLE_H
#define BATTLE_H

#define BATTLE_WHITE_OUT 1
#define BATTLE_WIN 2

struct Pokemon;

//Begin a Battle with a given pokemon
int initiate_battle(struct Pokemon * enemyPoke);

#endif