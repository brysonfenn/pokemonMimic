#include "typings.h"

#include "pokemon.h"
#include "../player.h"
#include "../print_utils.h"

float get_single_multiplier(Type moveType, Type victimType);

//Returns damage after applying effectiveness
//Also prints message about effectiveness, if applicable
int get_damage_after_effectiveness(Type moveType, struct pokemon * pok, int damage) {

    float multiplier = 1.0;

    //Many pokemon have two types
    multiplier *= get_single_multiplier(moveType, pok->type1);
    multiplier *= get_single_multiplier(moveType, pok->type2);

    damage *= multiplier;

    if (multiplier != 1.0) text_box_cursors(TEXT_BOX_NEXT_LINE);

    if (multiplier < 1.0) {
        printw("It's not very effective..."); refresh();  sleep(2);
        damage = (damage < 1) ? 1 : damage;
    }
    else if (multiplier > 1.0) {
        printw("It's super effective!"); refresh(); sleep(2);
    }
    else if (multiplier == 0) {
        printw("It didn't effect ");
        if (pok != player.current_pokemon) printw("Enemy ");
        printw("%s", pok->name); refresh(); sleep(2);
    }

    return damage;
}

//Return an effectiveness multiplier depending on move type and one victimType
float get_single_multiplier(Type moveType, Type victimType) {
    float multiplier = 1.0;

    if (victimType == NO_TYPE) return multiplier;

    switch (moveType) {
        case NO_TYPE:
            break;
        case NORMAL:
            if (victimType == ROCK) multiplier = 0.5;
            else if (victimType == GHOST) multiplier = 0;
            break;
        case FIRE:
            if (victimType == GRASS || victimType == ICE || victimType == BUG)
                multiplier = 2.0;
            else if (victimType == FIRE || victimType == WATER || victimType == ROCK || victimType == DRAGON)
                multiplier = 0.5;
            break;
        case WATER:
            if (victimType == FIRE || victimType == GROUND || victimType == ROCK)
                multiplier = 2.0;
            else if (victimType == WATER || victimType == GRASS || victimType == DRAGON)
                multiplier = 0.5;
            break;
        case ELECTRIC:
            if (victimType == WATER || victimType == FLYING)
                multiplier = 2.0;
            else if (victimType == ELECTRIC || victimType == GRASS || victimType == DRAGON)
                multiplier = 0.5;
            else if (victimType == GROUND)
                multiplier = 0;
            break;
        case GRASS:
            if (victimType == WATER || victimType == GROUND || victimType == ROCK)
                multiplier = 2.0;
            else if (victimType == FIRE || victimType == GRASS || victimType == POISON || victimType == FLYING 
                        || victimType == BUG || victimType == DRAGON)
                multiplier = 0.5;
            break;
        case ICE:
            if (victimType == GRASS || victimType == GROUND || victimType == FLYING || victimType == DRAGON)
                multiplier = 2.0;
            else if (victimType == FIRE || victimType == WATER || victimType == ICE)
                multiplier = 0.5;
            break;
        case FIGHTING:
            if (victimType == NORMAL || victimType == ICE || victimType == ROCK)
                multiplier = 2.0;
            else if (victimType == POISON || victimType == FLYING || victimType == PSYCHIC || victimType == BUG)
                multiplier = 0.5;
            else if (victimType == GHOST)
                multiplier = 0;
            break;
        case POISON:
            if (victimType == GRASS)
                multiplier = 2.0;
            else if (victimType == POISON || victimType == GROUND || victimType == ROCK || victimType == GHOST)
                multiplier = 0.5;
            break;
        case GROUND:
            if (victimType == FIRE || victimType == ELECTRIC || victimType == POISON || victimType == ROCK)
                multiplier = 2.0;
            else if (victimType == GRASS || victimType == BUG)
                multiplier = 0.5;
            else if (victimType == FLYING)
                multiplier = 0;
            break;
        case FLYING:
            if (victimType == GRASS || victimType == FIGHTING || victimType == BUG)
                multiplier = 2.0;
            else if (victimType == ELECTRIC || victimType == ROCK)
                multiplier = 0.5;
            break;
        case PSYCHIC:
            if (victimType == FIGHTING || victimType == POISON)
                multiplier = 2.0;
            else if (victimType == PSYCHIC)
                multiplier = 0.5;
            break;
        case BUG:
            if (victimType == GRASS || victimType == PSYCHIC)
                multiplier = 2.0;
            else if (victimType == FIRE || victimType == FIGHTING || victimType == POISON || victimType == FLYING
                        || victimType == GHOST)
                multiplier = 0.5;
            break;
        case ROCK:
            if (victimType == FIRE || victimType == ICE || victimType == FLYING || victimType == BUG)
                multiplier = 2.0;
            else if (victimType == FIGHTING || victimType == GROUND)
                multiplier = 0.5;
            break;
        case GHOST:
            if (victimType == PSYCHIC || victimType == GHOST)
                multiplier = 2.0;
            else if (victimType == NORMAL)
                multiplier = 0;
            break;
        case DRAGON:
            if (victimType == DRAGON)
                multiplier = 2.0;
            break;
        default:
            break;
    }

    return multiplier;
}