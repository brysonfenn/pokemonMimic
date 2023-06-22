#ifndef ATTACKS_SPECIAL_H
#define ATTACKS_SPECIAL_H

#include <stdbool.h>

struct Pokemon;

//Special function for rest move
int rest_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Special function for magnitude move
int magnitude_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage);

//Repeat own move in a given range
int self_repeat_move(int low, int high, struct Pokemon * victim, int damage);


#endif // ATTACKS_SPECIAL_H