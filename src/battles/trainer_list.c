#include "trainer_list.h"

#include "trainer.h"
#include "../player.h"
#include "../monsters/pokemon.h"
#include "../print/print_utils.h"

#define NUM_TRAINERS 50

static Trainer t000  = {0, "Trainer Empty", "Hi! I train Pokemon too!", 2, {POKEMON_RATTATA, POKEMON_PIDGEY}, {4,4} };
static Trainer t001 = {1, "Trainer John", "Hi! I train Pokemon too!", 2, {POKEMON_RATTATA, POKEMON_PIDGEY}, {4,4} };
static Trainer t002 = {2, "Bugcatcher David", "Are you afraid of Bugs?", 2, {POKEMON_KAKUNA, POKEMON_WEEDLE}, {4,5} };
static Trainer t003 = {3, "Trainer Steven", "I'm a tough trainer, let's battle!", 2, {POKEMON_SANDSHREW, POKEMON_DIGLETT}, {4,5} };
static Trainer t004 = {4, "Rival G", "I'm your rival!", 1, {POKEMON_BULBASAUR}, {5} };
static Trainer t005 = {5, "Bugcatcher Marty", "I'll defeat you with my Bug Pokemon!", 2, {POKEMON_CATERPIE, POKEMON_WEEDLE}, {4,5} };
static Trainer t006 = {6, "Camper Liam", "I like Rock Type Pokemon. Let's have a Battle!", 2, {POKEMON_GEODUDE, POKEMON_SANDSHREW}, {10,11} };
static Trainer t007 = {7, "Bugcatcher Josh", "I like Bug Pokemon, what about you?", 2, {POKEMON_METAPOD, POKEMON_VENONAT}, {5,6} };
static Trainer t008 = {8, "Trainer Liam", "I'm a trainer, just like you!", 2, {POKEMON_ODDISH, POKEMON_NIDORAN_M}, {5,5} };
static Trainer t009 = {9, "Trainer Allison", "I'm not supposed to be here.", 2, {POKEMON_PIDGEY, POKEMON_MANKEY}, {6,7} };
static Trainer t010 = {10, "Trainer Michelle", "I was forced into this.", 2, {POKEMON_MEOWTH, POKEMON_PSYDUCK}, {6,7} };
static Trainer t011 = {11, "Trainer Olivia", "I'm not super into this.", 2, {POKEMON_NIDORAN_F, POKEMON_VULPIX}, {7,7} };
static Trainer t012 = {12, "Trainer Jordan", "Stonks.", 2, {POKEMON_SANDSHREW, POKEMON_DIGLETT}, {7,9} };
static Trainer t013 = {13, "Trainer Heather", "There's no Rowlett in this Generation.", 2, {POKEMON_ODDISH, POKEMON_PIDGEOTTO}, {9,8} };
static Trainer t014 = {14, "Trainer Joey", "I like Starter Pokemon!", 3, {POKEMON_BULBASAUR, POKEMON_CHARMANDER, POKEMON_SQUIRTLE}, {8,8,8} };
static Trainer t015 = {15, "Miner Jason", "I have been mining for hours, I need a break.", 2, {POKEMON_GEODUDE, POKEMON_EKANS}, {9,11} };
static Trainer t016 = {16, "Trainer Andrew", "This cave is really dark!", 2, {POKEMON_ZUBAT, POKEMON_PARAS}, {10,11} };
static Trainer t017 = {17, "Trainer Peter", "I'm looking for cool rocks!", 2, {POKEMON_GEODUDE, POKEMON_MANKEY}, {12,13} };
static Trainer t018 = {18, "Rival G", "It's been a while! Let's Battle!", 3, {POKEMON_RATTATA, POKEMON_GRAVELER, POKEMON_SQUIRTLE}, {10,11,13} };
static Trainer t019 = {19, "Trainer Matthew", "I like poison types!", 2, {POKEMON_ZUBAT, POKEMON_EKANS}, {10,10} };
static Trainer t020 = {20, "Gym Trainer Ariel", "You'll never defeat misty!", 2, {POKEMON_GOLDEEN, POKEMON_SEEL}, {15,17} };

static Trainer t021 = {21, "Gym Trainer Katie", "I like water pokemon!", 2, {POKEMON_HORSEA, POKEMON_KRABBY}, {16,17} };
static Trainer t022 = {22, "Trainer Jackie", "oo ah oo da ba?", 2, {POKEMON_VULPIX, POKEMON_JIGGLYPUFF}, {9,10} };
static Trainer t023 = {23, "Youngster Russell", "I'm looking for apple pants!", 2, {POKEMON_EKANS, POKEMON_PIDGEOTTO}, {10,10} };
static Trainer t024 = {24, "Trainer Erin", "I'm here looking after Russell.", 2, {POKEMON_SANDSHREW, POKEMON_GROWLITHE}, {10,11} };
static Trainer t025 = {25, "Cooltrainer Lydia", "I like Pokemon!!", 2, {POKEMON_CLEFAIRY, POKEMON_BUTTERFREE}, {11,11} };
static Trainer t026 = {26, "Cooltrainer Hailey", "I like watching Pokemon!!", 3, {POKEMON_MANKEY, POKEMON_GOLBAT, POKEMON_GLOOM}, {11,11,12} };
static Trainer t027 = {27, "Trainer Eric", "I found some cool Pokemon here.", 3, {POKEMON_DODUO, POKEMON_PONYTA, POKEMON_BELLSPROUT}, {11,12,12} };
static Trainer t028 = {28, "Trainer Jill", "Have you met Bill?", 2, {POKEMON_WEEPINBELL, POKEMON_DODRIO}, {12,14} };
static Trainer t029 = {29, "Sailor Geoffrey", "Welcome to the SS Anne!", 2, {POKEMON_POLIWAG, POKEMON_MACHOP}, {14,15} };
static Trainer t030 = {30, "Sailor Kenneth", "I'm feeling a little Seasick!", 2, {POKEMON_PSYDUCK, POKEMON_SLOWBRO}, {16,16} };
static Trainer t031 = {31, "Sailor Sebastian", "I like Tough Pokemon!", 2, {POKEMON_HITMONCHAN, POKEMON_PINSIR}, {17,17} };
static Trainer t032 = {32, "Traveler Jackson", "This boat is huge!", 3, {POKEMON_PIDGEOTTO, POKEMON_KADABRA, POKEMON_RAPIDASH}, {16,17,17} };
static Trainer t033 = {33, "Madame Halpin", "I'm getting too old for battles!", 2, {POKEMON_TANGELA, POKEMON_JYNX}, {18,18} };
static Trainer t034 = {34, "Sailor Garth", "My water type Pokemon love this boat", 3, {POKEMON_SEEL, POKEMON_POLIWHIRL, POKEMON_CLOYSTER}, {18,18,19} };
static Trainer t035 = {35, "Sir Lewis", "I'm starting my retirement here", 2, {POKEMON_PERSIAN, POKEMON_ELECTABUZZ}, {19,20} };
static Trainer t036 = {36, "Trainer Mindy", "I'm scared of bugs!", 2, {POKEMON_VULPIX, POKEMON_PIKACHU}, {6,7} };
static Trainer t037 = {37, "Traveler Veronica", "I'm from Vermillion City. Have you been there?", 2, {POKEMON_DROWZEE, POKEMON_NIDORINO}, {9,9} };
static Trainer t038 = {38, "Gym Trainer Mike", "This is a tough Gym. Good Luck!", 2, {POKEMON_VOLTORB, POKEMON_MAGNEMITE}, {19,20} };
static Trainer t039 = {39, "Gym Trainer Lee", "I love Electric Types!", 2, {POKEMON_PIKACHU, POKEMON_ELECTRODE}, {20,20} };
static Trainer t040 = {40, "Gym Trainer Ignacio", "I have Steel Electric types, Get ready!", 2, {POKEMON_MAGNEMITE, POKEMON_MAGNETON}, {19,21} };

static Trainer * trainers[NUM_TRAINERS] = { &t000, 
    &t001, &t002, &t003, &t004, &t005, &t006, &t007, &t008, &t009, &t010, &t011, &t012, &t013, &t014, &t015, &t016, &t017, &t018, &t019, &t020,
    &t021, &t022, &t023, &t024, &t025, &t026, &t027, &t028, &t029, &t030, &t031, &t032, &t033, &t034, &t035, &t036, &t037, &t038, &t039, &t040
};

static Trainer leader201 = {201, "Leader Brock", "I am the First Gym Leader. Let's Battle!", 2, {POKEMON_GEODUDE, POKEMON_ONIX}, {12,14} };
static Trainer leader202 = {202, "Leader Misty", "I am Misty, the Second Gym Leader. Let's Battle!", 2, {POKEMON_STARYU, POKEMON_STARMIE}, {18,21} };
static Trainer leader203 = {203, "Lieutenant Surge", "I am Lt. Surge, Gym Leader of Vermillion. Get ready to be shocked!", 
                                3, {POKEMON_VOLTORB, POKEMON_PIKACHU, POKEMON_RAICHU}, {21,18,24} };

static Trainer * leaders[10] = { &t000,
    &leader201, &leader202, &leader203
};

static Trainer rival001 = {251, "Rival G", "I'm your rival, Let's Battle!", 1, {POKEMON_BULBASAUR}, {5} };
static Trainer rival002 = {252, "Rival G", "It's been a while, are you any better?", 3, {POKEMON_GEODUDE, POKEMON_RATTATA, POKEMON_BULBASAUR}, {10,11,13} };
static Trainer rival003 = {253, "Rival G", "You'll have to beat me to see the Captain", 4, {POKEMON_RATICATE, POKEMON_GRAVELER, POKEMON_KADABRA, POKEMON_IVYSAUR}, {19,20,20,22} };

static Trainer * rivals[10] = { &t000,
    &rival001, &rival002, &rival003
};

void update_rival(int id);

struct Trainer * get_trainer(int trainer_id) {
    //Rival IDs are more than 250
    if (trainer_id > 250) {
        update_rival(trainer_id - 250);
        return rivals[trainer_id - 250];
    }
    //Leader IDs are more than 200
    else if (trainer_id > 200) {
        return leaders[trainer_id - 200];
    }
    else {
        return trainers[trainer_id];
    }
}

void update_rival(int id) {
    Trainer * curr_rival = rivals[id];
    int rival_starter = POKEMON_BULBASAUR;
    int rival_starter_index = 0;
    int player_starter = POKEMON_MISSING_NO;

    //Find where BULBASAUR evolution is
    for (int i = 0; i < curr_rival->num_in_party; i++) {
        if (curr_rival->party_id_list[i] <= POKEMON_VENUSAUR) {
            rival_starter_index = i;
            rival_starter = curr_rival->party_id_list[i];
        }
        else if (curr_rival->party_id_list[i] <= POKEMON_BLASTOISE) {
            return;
        }
    }

    //Find which starter the player has
    for (int i = 0; i < player.numInParty; i++) {
        //If starter found
        if (player.party[i].id_num <= POKEMON_BLASTOISE) {
            player_starter = player.party[i].id_num;
        }
    }
    for (int i = 0; i < player.numInPCStorage; i++) {
        //If starter found
        if (player.pc_storage[i].id_num <= POKEMON_BLASTOISE) {
            player_starter = player.pc_storage[i].id_num;
        }
    }

    //Change Rival based on player's starter
    if (player_starter == POKEMON_MISSING_NO) {
        curr_rival->party_id_list[rival_starter_index] = POKEMON_EEVEE;
    }
    else if (player_starter <= POKEMON_VENUSAUR) {
        curr_rival->party_id_list[rival_starter_index] = POKEMON_CHARMANDER + rival_starter - 1;
    }
    else if (player_starter <= POKEMON_CHARIZARD) {
        curr_rival->party_id_list[rival_starter_index] = POKEMON_SQUIRTLE + rival_starter - 1;
    }
    else if (player_starter <= POKEMON_BLASTOISE) {
        curr_rival->party_id_list[rival_starter_index] = POKEMON_BULBASAUR + rival_starter - 1;
    }
    else {
        curr_rival->party_id_list[rival_starter_index] = POKEMON_PIKACHU;
    }

}