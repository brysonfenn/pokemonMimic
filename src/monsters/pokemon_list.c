#include "pokemon.h"

#include <stdio.h>
#include <ncurses.h>

#include "../print_utils.h"
#include "../print_defines.h"

// pok  varName   = {"name here",  id  hp chp   ba   bd   bsa   bsd    bs   type1     type2  }
pokemon emptyPok = {"MissingNo",    0,100,100,   0,   0,    0,    0,    0, NO_TYPE,  NO_TYPE };

#define NUM_CREATED_POKEMON 8
#define NUM_STARTERS 3
#define NUM_WILD_POKEMON (NUM_CREATED_POKEMON - NUM_STARTERS)

static Pokemon_id pok_id_list[10] = { POKEMON_BULBASAUR, POKEMON_CHARMANDER, POKEMON_SQUIRTLE, POKEMON_CATERPIE, POKEMON_WEEDLE,
  POKEMON_PIDGEY, POKEMON_RATTATA, POKEMON_SANDSHREW };

static pokemon newest_pokemon;

//Return a random pokemon of any possible
pokemon * get_random_pokemon(int level_min, int level_max) {
  int pok_position = rand() % NUM_CREATED_POKEMON;
  newest_pokemon = *(get_pokemon_frame(pok_id_list[pok_position]));
  pokemon * new_pok = &newest_pokemon;
  pokemon_init(new_pok, RANDOM_LEVEL, level_min, level_max);
  return new_pok;
}

//Return a random pokemon, excluding starters
//Always immediately dereference the return value of this function.
pokemon * get_random_wild_pokemon(int level_min, int level_max) {
  int pok_position = (rand() % NUM_WILD_POKEMON) + NUM_STARTERS;
  newest_pokemon = *(get_pokemon_frame(pok_id_list[pok_position]));
  // newest_pokemon = *(get_pokemon_frame(POKEMON_CATERPIE));
  pokemon * new_pok = &newest_pokemon;
  pokemon_init(new_pok, RANDOM_LEVEL, level_min, level_max);
  // pokemon_init(new_pok, 0, 15, 20);
  return new_pok;
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