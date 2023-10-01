#include "conditions.h"

#include <stdlib.h>

#include "pokemon.h"
#include "../player.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../print/print_battle.h"

int handle_leech_seed(bool enemy, struct Pokemon * pok, struct Pokemon * perp);
int handle_hurt_1_16(bool enemy, struct Pokemon * pok, char * pok_condition_text);

//Handle poisoning, leech seed, etc.
int handle_end_conditions(struct Pokemon * pok) {
    bool enemy;
    Pokemon * perp;
    bool success = false;

    char condition_text[256];

    //Set enemy boolean and perp
    if (pok == player.enemy_pokemon) {
        enemy = true;
        perp = player.current_pokemon;
    }
    else {
        enemy = false;
        perp = player.enemy_pokemon;
    }

	//If either pokemon fainted, do not handle conditions
	if (pok->currentHP <= 0 || perp->currentHP <= 0) {
		return 0;
	}

	//Remove flinched hidden condition
	remove_hidden_condition(pok, FLINCHED);

	//Poison and burn
	if (pok->visible_condition == POISONED) {
		handle_hurt_1_16(enemy, pok, "poison");
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
	}
    if (pok->visible_condition == BURNED) {
        handle_hurt_1_16(enemy, pok, "burn");
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
    }

    // BEGIN TEMPORARY AFFLICTIONS // BIND, WRAP, ETC.
    if (has_hidden_condition(pok, BIND)) {
        success = decrement_hidden_condition_val(pok, BIND);
        if (!success) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s was released from bind", pok->name); refresh(); sleep(2);
            remove_hidden_condition(pok, BIND);
            return ATTACK_SUCCESS;
        }

        handle_hurt_1_16(enemy, pok, "bind");
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
    }
    if (has_hidden_condition(pok, WRAPPED)) {
        success = decrement_hidden_condition_val(pok, WRAPPED);
        if (!success) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s was released from wrap", pok->name); refresh(); sleep(2);
            remove_hidden_condition(pok, WRAPPED);
            return ATTACK_SUCCESS;
        }

        handle_hurt_1_16(enemy, pok, "bind");
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
    }
    if (has_hidden_condition(pok, SAND_TOMBED)) {
        success = decrement_hidden_condition_val(pok, SAND_TOMBED);
        if (!success) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s was released from sand tomb", pok->name); refresh(); sleep(2);
            remove_hidden_condition(pok, SAND_TOMBED);
            return ATTACK_SUCCESS;
        }

        handle_hurt_1_16(enemy, pok, "sand tomb");
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
    }
    if (has_hidden_condition(pok, FIRE_SPINNED)) {
        success = decrement_hidden_condition_val(pok, FIRE_SPINNED);
        if (!success) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s was released from fire spin", pok->name); refresh(); sleep(2);
            remove_hidden_condition(pok, FIRE_SPINNED);
            return ATTACK_SUCCESS;
        }

        handle_hurt_1_16(enemy, pok, "fire spin");
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
    }
    // END TEMPORARY AFFLICTIONS //

    if (has_hidden_condition(pok, YAWNED)) {
        success = decrement_hidden_condition_val(pok, YAWNED);
        if (!success) {
            inflict_condition(ASLEEP, 100, pok, 0);
            remove_hidden_condition(pok, YAWNED);
            return ATTACK_SUCCESS;
        }
    }
	
	//Leech Seed
	if (has_hidden_condition(pok, SEEDED)) {
        handle_leech_seed(enemy, pok, perp);
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
	}

	return 0;
}


int handle_leech_seed(bool is_enemy, struct Pokemon * pok, struct Pokemon * perp) {
    clear(); printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    if (is_enemy) printw("%s", ENEMY_TEXT);
    printw("%s's HP was sapped!", pok->name); refresh(); sleep(1);
    int sappedHP = ((pok->maxHP / 8) + 1);

    blinkPokemon(!is_enemy, DAMAGED_COLOR, LEECH_SEED_BLINK_TIMES); 
    printBattle();
    blinkPokemon(is_enemy, HEAL_COLOR, LEECH_SEED_BLINK_TIMES);

    //Give HP equal to taken HP and fix any issues
    if (sappedHP > pok->currentHP) { perp->currentHP += pok->currentHP; pok->currentHP = 0; }
    else { perp->currentHP += sappedHP; pok->currentHP -= sappedHP; }
    if (perp->currentHP > perp->maxHP) perp->currentHP = perp->maxHP;

    return 0;
}


int handle_hurt_1_16(bool enemy, struct Pokemon * pok, char * pok_condition_text) {
    clear(); printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    if (enemy) printw("%s", ENEMY_TEXT);
    printw("%s was hurt by %s!", pok->name, pok_condition_text); refresh(); sleep(1);
    blinkPokemon(!enemy, DAMAGED_COLOR, 3);

    pok->currentHP -= ((pok->maxHP / 16) + 1);
    if (pok->currentHP < 0) pok->currentHP = 0;
    
}