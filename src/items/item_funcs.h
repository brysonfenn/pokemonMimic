#ifndef ITEM_FUNCS_H
#define ITEM_FUNCS_H

//Potion execute function
int execute_potion(int hp_gain, char * name);

//Pokeball execute function (catch_rate is % multiplier)
int attempt_catch(int catch_rate, char * name);

//Heal Condition function (condition is condition code)
int heal_condition(int condition, char * name);

//Revive a pokemon up to percent of their HP
int revive_pokemon(int percent, char * name);

//Get player selection to evolve, and evolve
int use_evolve_stone(int stone_id, char * name);

//Use Repel - Repel pokemon for num_steps
int use_repel(int num_steps, char * name);

//Filler function for empty_item that does nothing
int do_nothing(int input_num);

#endif // ITEM_FUNCS_H