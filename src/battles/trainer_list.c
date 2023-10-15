#include "trainer_list.h"

#include "trainer.h"

static Trainer t0  = {0, "Trainer X1", "Hi! I train Pokemon too!", 2, {POKEMON_RATTATA, POKEMON_PIDGEY}, {4,4} };
static Trainer t1 = {1, "Trainer John", "Hi! I train Pokemon too!", 2, {POKEMON_RATTATA, POKEMON_PIDGEY}, {4,4} };
static Trainer t2 = {2, "Bugcatcher David", "Are you afraid of Bugs?", 2, {POKEMON_KAKUNA, POKEMON_WEEDLE}, {4,5} };
static Trainer t3 = {3, "Trainer Steven", "I'm a tough trainer, let's battle!", 2, {POKEMON_SANDSHREW, POKEMON_DIGLETT}, {4,5} };
static Trainer t4 = {4, "Rival G", "I'm your rival!", 1, {POKEMON_BULBASAUR}, {5} };
static Trainer t5 = {5, "Bugcatcher Marty", "I'll defeat you with my Bug Pokemon!", 2, {POKEMON_CATERPIE, POKEMON_WEEDLE}, {4,5} };
static Trainer t6 = {6, "Camper Liam", "I like Rock Type Pokemon. Let's Battle!", 2, {POKEMON_GEODUDE, POKEMON_SANDSHREW}, {10,11} };
static Trainer t7 = {7, "Bugcatcher Josh", "I like Bug Pokemon, what about you?", 2, {POKEMON_METAPOD, POKEMON_VENONAT}, {5,6} };
static Trainer t8 = {8, "Trainer Liam", "I'm a trainer, just like you!", 2, {POKEMON_ODDISH, POKEMON_NIDORAN_M}, {5,5} };

static Trainer * trainers[10] = { &t0, &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8 };

struct Trainer * get_trainer(int trainer_id) {
    return trainers[trainer_id];
}