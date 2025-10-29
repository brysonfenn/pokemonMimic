#ifndef ATTACKS_SPECIAL_H
#define ATTACKS_SPECIAL_H

#include <stdbool.h>

struct Pokemon;

#define ADDED_REPEATS -1
#define REMOVED_REPEATS -2

//Special function for rest move
int rest_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for magnitude move
int magnitude_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Repeat own move in a given range
int handle_repeats(int repeat_times, struct Pokemon * victim);

//Special function for rollout move
int rollout_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for uproar move
int uproar_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for thrash move
int thrash_move_func(int nothing1, int move_type, struct Pokemon * victim, int damage);

//Special function for swagger move
int swagger_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for Seismic Toss move
int seismic_toss_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special Function for Teleport
int teleport_move_func(int nothing1, int chance, struct Pokemon * victim, int damage);

//Special Function for Sheer Cold
int k_o_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special Function for Curse
int curse_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special Function for Night Shade
int night_shade_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special Function for Refresh
int refresh_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for Flail move
int flail_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for Splash move
int splash_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for Solar Beam
int solar_beam_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for Skull Bash
int skull_bash_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for Transform
int transform_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

#endif // ATTACKS_SPECIAL_H