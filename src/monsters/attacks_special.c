#include "attacks_special.h"

#include "pokemon.h"
#include <ncurses.h>

#include "../player.h"

#include "conditions.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../print/print_battle.h"


//Speical function for rest move
int rest_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    bool enemy;

    if (victim == player.current_pokemon) {
        self = player.enemy_pokemon;
        enemy = true;
    }
    else {
        self = player.current_pokemon;
        enemy = false;
    }

    self->currentHP = self->maxHP;
    self->visible_condition = ASLEEP;

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("%s went to sleep!", self->name); refresh(); sleep(2);
    clear_battle_box(); printBattle();

    return 0;
}