#include "pokemon.h"

// pok  varName   = {"name here",  id  hp chp   ba   bd    bs   type1     type2    num_att ca cd cs  acc lvl exp}
pokemon emptyPok = {"MissingNo",    0,100,100,   0,   0,    0, NO_TYPE,  NO_TYPE };
pokemon bulbasaur = {"Bulbasaur",   1,  9,  9,   4,   4,    4, GRASS,    POISON };
pokemon charmander = {"Charmander", 4,  9,  9,   4,   4,    4, FIRE,     NO_TYPE };
pokemon squirtle = {"Squirtle",     7,  9,  9,   4,   4,    4, WATER,    NO_TYPE };
pokemon caterpie = {"Caterpie",    10,  6,  6, 900,   2,    3, BUG,      NO_TYPE };
pokemon weedle   = {"Weedle",      13,  7,  7,   3,   2,    3, BUG,      POISON };
pokemon pidgey   = {"Pidgey",      16,  8,  8,   4,   2,    3, FLYING,   NORMAL };
pokemon rattata  = {"Rattata",     19,  7,  7,   2,   3,    4, NORMAL,   NO_TYPE };
pokemon sandshrew= {"Sandshrew",   27,  7,  7,   3,   4,    2, GROUND,   NO_TYPE };

#define NUM_CREATED_POKEMON 8
#define NUM_STARTERS 3
#define NUM_WILD_POKEMON (NUM_CREATED_POKEMON - NUM_STARTERS)
static pokemon * pokList[10] = { &bulbasaur, &charmander, &squirtle, &caterpie, &weedle, &pidgey, &rattata, &sandshrew };

static pokemon newest_pokemon;

pokemon * get_random_pokemon(int level_min, int level_max) {
  newest_pokemon = *(pokList[rand() %  NUM_CREATED_POKEMON]);
  // newest_pokemon = caterpie;
  pokemon * new_pok = &newest_pokemon;
  pokemon_init(new_pok, RANDOM_LEVEL, level_min, level_max);
  return new_pok;
}

//Always immediately dereference the return value of this function.
pokemon * get_random_wild_pokemon(int level_min, int level_max) {
  newest_pokemon = *(pokList[(rand() % NUM_WILD_POKEMON) + NUM_STARTERS]);
  newest_pokemon = caterpie;
  pokemon * new_pok = &newest_pokemon;
  pokemon_init(new_pok, RANDOM_LEVEL, level_min, level_max);
  return new_pok;
}

