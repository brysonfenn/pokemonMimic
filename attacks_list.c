#include "attacks.h"

#define NUM_ATTACKS 50

attack empty_attack=  {"------------",  0,  0, 100, 0, 0, 0, 0 };
//				 	           "Name        "  id  pp  pwr  acc   type    ad dd sd acd  pmove
attack tackle       = {"Tackle      ",  1, 35,  40,  95, NORMAL,   0, 0, 0, 0.0, false, &attack_do_nothing, 0, 0 };
attack scratch      = {"Scratch     ",  2, 35,  40, 100, NORMAL,   0, 0, 0, 0.0, false, &attack_do_nothing, 0, 0 };
attack growl        = {"Growl       ",  3, 40,   0, 100, NO_TYPE,  1, 0, 0, 0.0, false, &attack_do_nothing, 0, 0 };
attack tail_whip    = {"Tail Whip   ",  4, 30,   0, 100, NO_TYPE,  0, 1, 0, 0.0, false, &attack_do_nothing, 0, 0 };
attack string_shot  = {"String Shot ",  5, 40,   0,  95, NO_TYPE,  0, 0, 1, 0.0, false, &attack_do_nothing, 0, 0 };
attack poison_sting = {"Poison Sting",  6, 35,  15, 100, POISON,   0, 0, 0, 0.0, false, &inflict_condition, POISONED, 30 };
attack sand_attack  = {"Sand Attack ",  7, 15,   0, 100, NO_TYPE,  0, 0, 0, 0.1, false, &attack_do_nothing, 0, 0 };
attack quick_attack = {"Quick Attack",  8, 30,  40, 100, NORMAL,   0, 0, 0, 0.0,  true, &attack_do_nothing, 0, 0 };
attack defense_curl = {"Defense Curl",  9, 40,   0, 100, NO_TYPE,  0,-1, 0, 0.0, false, &attack_do_nothing, 0, 0 };
attack vine_whip    = {"Vine whip   ", 10, 25,  45, 100, GRASS,    0, 0, 0, 0.0, false, &attack_do_nothing, 0, 0 };

static attack * local_array[NUM_ATTACKS] = { &empty_attack, 
    &tackle, &scratch, &growl, &tail_whip, &string_shot, &poison_sting, &sand_attack, &quick_attack, &defense_curl, &vine_whip }; // #1-10


attack * get_attack_by_id(id_num) {
	return local_array[id_num];
}