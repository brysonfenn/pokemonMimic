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


#endif // ATTACKS_SPECIAL_H