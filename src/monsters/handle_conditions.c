#include "conditions.h"

#include <stdlib.h>

#include "pokemon.h"
#include "../player.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../print/print_battle.h"

int handle_leech_seed(bool enemy, struct Pokemon * pok, struct Pokemon * perp);
int handle_ingrain(bool is_enemy, struct Pokemon * pok);
int handle_hurt_fraction(bool enemy, struct Pokemon * pok, char * pok_condition_text, int fraction);

//Handle poisoning, leech seed, etc.
int handle_end_conditions(struct Pokemon * pok) {
    bool is_enemy;
    Pokemon * perp;
    bool success = false;

    char condition_text[256];

    //Set enemy boolean and perp
    if (pok == player.enemy_pokemon) {
        is_enemy = true;
        perp = player.current_pokemon;
    }
    else {
        is_enemy = false;
        perp = player.enemy_pokemon;
    }

	//If either pokemon fainted, do not handle conditions
	if (pok->currentHP <= 0 || perp->currentHP <= 0) {
		return 0;
	}

	//Remove temporary hidden conditions
	remove_hidden_condition(pok, FLINCHED);
    remove_hidden_condition(pok, PROTECTED);

	//Poison and burn
	if (pok->visible_condition == POISONED) {
		handle_hurt_fraction(is_enemy, pok, "poison", 16);
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
	}
    if (pok->visible_condition == BURNED) {
        handle_hurt_fraction(is_enemy, pok, "burn", 16);
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
    }

    // BEGIN TEMPORARY AFFLICTIONS // BIND, WRAP, ETC.
    if (has_hidden_condition(pok, TIGHT_HOLD)) {
        success = decrement_hidden_condition_val(pok, TIGHT_HOLD);
        if (!success) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s was released from tight hold", pok->nickname); refresh(); sleep(2);
            remove_hidden_condition(pok, TIGHT_HOLD);
            return ATTACK_SUCCESS;
        }

        handle_hurt_fraction(is_enemy, pok, "tight hold", 16);
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
    }
    if (has_hidden_condition(pok, SAND_TOMBED)) {
        success = decrement_hidden_condition_val(pok, SAND_TOMBED);
        if (!success) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s was released from sand tomb", pok->nickname); refresh(); sleep(2);
            remove_hidden_condition(pok, SAND_TOMBED);
            return ATTACK_SUCCESS;
        }

        handle_hurt_fraction(is_enemy, pok, "sand tomb", 16);
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
    }
    if (has_hidden_condition(pok, FIRE_SPINNED)) {
        success = decrement_hidden_condition_val(pok, FIRE_SPINNED);
        if (!success) {
            text_box_cursors(TEXT_BOX_BEGINNING);
            printw("%s was released from fire spin", pok->nickname); refresh(); sleep(2);
            remove_hidden_condition(pok, FIRE_SPINNED);
            return ATTACK_SUCCESS;
        }

        handle_hurt_fraction(is_enemy, pok, "fire spin", 16);
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
    }
    if (has_hidden_condition(pok, CURSED)) {
        handle_hurt_fraction(is_enemy, pok, "curse", 4);
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
	
	//Leech Seed & Ingrain
	if (has_hidden_condition(pok, SEEDED)) {
        handle_leech_seed(is_enemy, pok, perp);
        if (pok->currentHP == 0) return ATTACK_SUCCESS;
	}
    if (has_hidden_condition(pok, INGRAINED)) {
        handle_ingrain(is_enemy, pok);
    }

	return ATTACK_SUCCESS;
}


int handle_leech_seed(bool is_enemy, struct Pokemon * pok, struct Pokemon * perp) {
    clear(); printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    if (is_enemy) printw("%s", ENEMY_TEXT);
    printw("%s's HP was sapped!", pok->nickname); refresh(); sleep(1);
    int sappedHP = ((pok->maxHP / 8) + 1);

    blinkPokemon(!is_enemy, DAMAGED_COLOR, LEECH_SEED_BLINK_TIMES, pok); 
    printBattle();
    blinkPokemon(is_enemy, HEAL_COLOR, LEECH_SEED_BLINK_TIMES, pok);

    //Give HP equal to taken HP and fix any issues
    if (sappedHP > pok->currentHP) { perp->currentHP += pok->currentHP; pok->currentHP = 0; }
    else { perp->currentHP += sappedHP; pok->currentHP -= sappedHP; }
    if (perp->currentHP > perp->maxHP) perp->currentHP = perp->maxHP;

    return 0;
}

int handle_ingrain(bool is_enemy, struct Pokemon * pok) {
    clear(); printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    if (is_enemy) printw("%s", ENEMY_TEXT);
    printw("%s absorbed HP from its roots!", pok->nickname); refresh(); sleep(1);
    int heal_hp = ((pok->maxHP / 16) + 1);

    blinkPokemon(!is_enemy, HEAL_COLOR, LEECH_SEED_BLINK_TIMES, pok);

    //Give HP to pokemon
    pok->currentHP += heal_hp;
    if (pok->currentHP > pok->maxHP) pok->currentHP = pok->maxHP;
    return 0;
}

//Deal damage to a given Pokemon equal to ((1 / fraction) * max_hp). "Hurt by {pok_condition_text}"
int handle_hurt_fraction(bool enemy, struct Pokemon * pok, char * pok_condition_text, int fraction) {
    clear(); printBattle();

    text_box_cursors(TEXT_BOX_BEGINNING);
    if (enemy) printw("%s", ENEMY_TEXT);
    printw("%s was hurt by %s!", pok->nickname, pok_condition_text); refresh(); sleep(1);
    blinkPokemon(!enemy, DAMAGED_COLOR, 3, pok);

    pok->currentHP -= ((pok->maxHP / fraction) + 1);
    if (pok->currentHP < 0) pok->currentHP = 0;
    
}