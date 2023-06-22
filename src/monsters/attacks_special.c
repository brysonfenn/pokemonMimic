#include "attacks_special.h"

#include "pokemon.h"
#include <ncurses.h>

#include "../player.h"

#include "conditions.h"
#include "../print/print_defines.h"
#include "../print/print_utils.h"
#include "../print/print_battle.h"
#include "typings.h"
#include "attacks.h"


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
    self->sleep_count = (rand() % 4) + 1;

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("%s went to sleep!", self->name); refresh(); sleep(2);
    clear_battle_box(); printBattle();

    return 0;
}


//Special function for magnitude move
int magnitude_move_func(int nothing1, int nothing2, struct Pokemon * victim, int damage) {
    Pokemon * self;
    int flags;
    bool enemy;
    if (victim == player.current_pokemon) { self = player.enemy_pokemon; enemy = true; }
    else { self = player.current_pokemon; enemy = false; }

    int magnitude_val = 0;
    int power = 0;
    int random = rand() % 100;

    if (random < 5)        { magnitude_val = 4; power = 10; }   //  5%
    else if (random <  15) { magnitude_val = 5; power = 30; }   // 10%
    else if (random <  35) { magnitude_val = 6; power = 50; }   // 20%
    else if (random <  65) { magnitude_val = 7; power = 70; }   // 30%
    else if (random <  85) { magnitude_val = 8; power = 90; }   // 20%
    else if (random <  95) { magnitude_val = 9; power =110; }   // 10%
    else if (random < 100) { magnitude_val =10; power =150; }   //  5%

    text_box_cursors(TEXT_BOX_NEXT_LINE);
    printw("Magnitude %d", magnitude_val); refresh(); sleep(1);

    // Baseline damage
    damage = get_basic_damage(self->level, power, self->baseAttack, victim->baseDefense, 
                                    self->atk_stage, victim->def_stage);

    damage = (damage <= 0) ? 1 : damage;  // Pokemon should always be able to do 1 damage

    //STAB Bonus
    if (self->type1 == GROUND || self->type2 == GROUND) {
        damage = (damage * 1.5);
    }

    //Calculate effectiveness
    damage = get_damage_after_effectiveness(GROUND, victim, damage, true, &flags);

    //Drop HP only if attack has damage power
    if (damage > 0) {
        blinkPokemon(enemy, DAMAGED_COLOR, DAMAGE_BLINK_TIMES);
        victim->currentHP -= damage;
        if (victim->currentHP < 0) victim->currentHP = 0;
        printBattle();
    }

    if (flags & SUPER_EFFECTIVE_FLAG) {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("It's super effective!"); refresh(); sleep(2);
    }
    else if (flags & NOT_VERY_EFFECTIVE_FLAG) {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("It's not very effective..."); refresh(); sleep(2);
    }
    else if (flags & DOES_NOT_AFFECT_FLAG) {
        text_box_cursors(TEXT_BOX_NEXT_LINE);
        printw("It had no effect."); refresh(); sleep(2);
    }
}


//Repeat own move in a given range
int self_repeat_move(int low, int high, struct Pokemon * victim, int damage) {
    Pokemon * self;
    bool enemy;
    if (victim == player.current_pokemon) { self = player.enemy_pokemon; enemy = true; }
    else { self = player.current_pokemon; enemy = false; }

    //Decrement until repeat is done
    if (has_hidden_condition(self, REPEAT_MOVE)) {
        if (!decrement_hidden_condition_val(self, REPEAT_MOVE)) {
            remove_hidden_condition(self, REPEAT_MOVE);
            return 2;
        }
        return 1;
    }

    int repeat_times;
    if (low == high) repeat_times = low;
    else repeat_times = (rand() % (low+1)) + (high - low);

    add_hidden_condition(self, REPEAT_MOVE, repeat_times);
    return 0;
}

