#include "pokemon.h"

#include <stdio.h>
#include <ncurses.h>

#include "../print_utils.h"
#include "../print_defines.h"
#include "../player.h"
#include "../motion/location.h"

// pok  varName   = {"name here",  id  hp chp   ba   bd   bsa   bsd    bs   type1     type2  }
pokemon emptyPok = {"MissingNo",    0,100,100,   0,   0,    0,    0,    0, NO_TYPE,  NO_TYPE };

#define NUM_CREATED_POKEMON 8
#define NUM_STARTERS 3
#define NUM_WILD_POKEMON (NUM_CREATED_POKEMON - NUM_STARTERS)

static Pokemon_id pok_id_list[10] = { POKEMON_BULBASAUR, POKEMON_CHARMANDER, POKEMON_SQUIRTLE, POKEMON_CATERPIE, POKEMON_WEEDLE,
  POKEMON_PIDGEY, POKEMON_RATTATA, POKEMON_SANDSHREW };

static Pokemon_id wild_pok_lists[10][10] = {
//Map #, levels, ID'S...
  { 3,   5, 7,  POKEMON_BULBASAUR, POKEMON_CHARMANDER, POKEMON_SQUIRTLE }, //No Map is #0
  { 2,   5, 8,  POKEMON_PIDGEY, POKEMON_RATTATA }, //Map #1
  { 3,   4, 8,  POKEMON_CATERPIE, POKEMON_WEEDLE, POKEMON_SANDSHREW }, //Map #2
  { 2,   2, 5,  POKEMON_PIDGEY, POKEMON_RATTATA } //Map #3
};

static pokemon newest_pokemon;


//Initialize a given pokemon new_pok and get randomized stats
  //level = particular level
  //if level = 0, get random level from range level_min:level_max
void pokemon_init(pokemon * new_pok, int level, int level_min, int level_max) {
  new_pok->exp = 15;
  new_pok->iv = rand();
  new_pok->numAttacks = 0;
  new_pok->visible_condition = NO_CONDITION;
  new_pok->hidden_condition = NO_CONDITION;

  calculate_stats(new_pok, level, level_min, level_max);
  new_pok->currentHP = new_pok->maxHP;
  pokemon_give_moves(new_pok);
}


//Create a new pokemon. **Immediately dereference the returned pokemon**
pokemon * create_new_pokemon(Pokemon_id pok_id, int level, int level_min, int level_max) {
  newest_pokemon = *get_pokemon_frame(pok_id);
  pokemon_init(&newest_pokemon, level, level_min, level_max);
  return (&newest_pokemon);
}


//Return a random pokemon of any possible
pokemon * get_random_pokemon(int level_min, int level_max) {
  int pok_position = rand() % NUM_CREATED_POKEMON;
  pokemon * new_pok = create_new_pokemon(pok_id_list[pok_position], RANDOM_LEVEL, level_min, level_max);
  return new_pok;
}


//Return a random pokemon, excluding starters
//Always immediately dereference the return value of this function.
pokemon * get_random_wild_pokemon(int level_min, int level_max) {
  int pok_position, new_pok_id;
  int min_level = 2;
  int max_level = 6;
  int set_level = RANDOM_LEVEL;

  //Get pokemon according to map, if it is an unknown map, get level 99 ivysaur
  if (player.loc->map < 1 || player.loc->map > 3) {
    new_pok_id = POKEMON_IVYSAUR;
    set_level = 99;
  }
  else {
    //Get the position of a random pokemon based on the the player's current map
    pok_position = (rand() % wild_pok_lists[player.loc->map][0]) + 3;
    new_pok_id = wild_pok_lists[player.loc->map][pok_position];
    min_level = wild_pok_lists[player.loc->map][1];
    max_level = wild_pok_lists[player.loc->map][2];
  }

  // new_pok_id = POKEMON_CATERPIE;
  pokemon * new_pok = create_new_pokemon(new_pok_id, set_level, min_level, max_level);

  return new_pok;
}


//Get a starter pokemon
pokemon * get_starter(int starter) {
  int pok_id;

  if (starter == 0) pok_id = POKEMON_BULBASAUR;
  else if (starter == 1) pok_id = POKEMON_CHARMANDER;
  else pok_id = POKEMON_SQUIRTLE;

  return create_new_pokemon(pok_id, 5, 0, 0);
}


//Get a pokemon with all its base stats
pokemon * get_pokemon_frame(Pokemon_id pok_id) {
  newest_pokemon = emptyPok;
  pokemon * pok = &newest_pokemon;

  // Open the file for reading
  FILE *fp;
  char filename[50];
  sprintf(filename, "pokemon_list.txt");
  char line[LINE_SIZE];
  fp = fopen(filename, "r");

  // Check if the file was opened successfully
  if (fp == NULL) {
      printw("pokemon_list.txt could not be found.\n"); refresh(); sleep(2);
      exit(0);
  }

  //pok_id is the line number in "pokemon_list.txt"
  for (int i = 0; i < pok_id+1; i++) {
    fgets(line, LINE_SIZE, fp);
  }
  
  //Prepare variables
  int id_num, hp, att, def, spd, sp_Attack, sp_Defense;
  char name[LINE_SIZE];
  char type1[LINE_SIZE];
  char type2[LINE_SIZE];

  //Check if there is only one type
  sscanf(line, "%d,%49[^,],%49[^,],%s", &id_num, &name, &type1, &type2);
  if (type2[0] == ',') {
    sscanf(line, "%d,%49[^,],%49[^,],,%d,%d,%d,%d,%d,%d", &id_num, &name, &type1,
            &hp, &att, &def, &sp_Attack, &sp_Defense, &spd);
    sprintf(type2, "NONE");
  }
  else {
    sscanf(line, "%d,%49[^,],%49[^,],%49[^,],%d,%d,%d,%d,%d,%d", &id_num, &name, &type1,
            &type2, &hp, &att, &def, &sp_Attack, &sp_Defense, &spd);
  }

  //Assign types to pokemon
  pok->type1 = get_type_id_by_string(type1);
  pok->type2 = get_type_id_by_string(type2);
  sprintf(pok->name, "%s", name);
  pok->id_num = id_num; pok->maxHP = hp; pok->baseAttack = att; 
  pok->baseDefense = def; pok->baseSpAttack = sp_Attack; 
  pok->baseSpDefense = sp_Defense; pok->baseSpeed = spd;

  fclose(fp);

  return pok;
}