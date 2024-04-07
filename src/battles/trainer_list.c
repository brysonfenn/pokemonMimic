#include "trainer_list.h"

#include <string.h>

#include "trainer.h"
#include "../player.h"
#include "../monsters/pokemon.h"
#include "../print/print_utils.h"

#define NUM_TRAINERS 100

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
static Trainer t029 = {29, "Sailor Geoffrey", "Welcome to the SS Anne!", 2, {POKEMON_POLIWAG, POKEMON_MACHOP}, {15,16} };
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

static Trainer t041 = {41, "Trainer Gerald", "You already have 3 badges? I should be easy then.", 2, {POKEMON_GRIMER, POKEMON_ARBOK}, {19,21} };
static Trainer t042 = {42, "Traveler Johnson", "It's a nice day out for a Pokemon Battle!", 2, {POKEMON_GEODUDE, POKEMON_PRIMEAPE}, {20,21} };
static Trainer t043 = {43, "Traveler Evan", "I'm from Lavender Town. What about you? Let's Battle!", 2, {POKEMON_LICKITUNG, POKEMON_HAUNTER}, {21,21} };
static Trainer t044 = {44, "Miner Jack", "Rock Tunnel is full of strong Pokemon. You might want to get some Repels.", 2, {POKEMON_GRAVELER, POKEMON_CUBONE}, {21,21} };
static Trainer t045 = {45, "Miner Vincent", "You heard Jack. I would also recommend getting some Repels.", 2, {POKEMON_MACHOKE, POKEMON_MAROWAK}, {21,22} };
static Trainer t046 = {46, "Trainer Emmett", "I love the wild Pokemon around here", 3, {POKEMON_FARFETCHD, POKEMON_EKANS, POKEMON_TAUROS}, {21,22,22} };
static Trainer t047 = {47, "Trainer Augustin", "I'm not sure why we are standing in a line", 2, {POKEMON_BELLSPROUT, POKEMON_GROWLITHE}, {22,22} };
static Trainer t048 = {48, "Trainer Justin", "I like this town. But It's kind of Spooky", 2, {POKEMON_NIDORAN_M, POKEMON_NIDORAN_F}, {22,22} };
static Trainer t049 = {49, "Trainer Ricardo", "Have you been to Celadon City?", 2, {POKEMON_WEEPINBELL, POKEMON_ARCANINE}, {23,23} };
static Trainer t050 = {50, "Trainer Julie", "I'm here hanging out with some friends", 2, {POKEMON_NIDORINO, POKEMON_NIDORINA}, {23,23} };
static Trainer t051 = {51, "Trainer Jasper", "If you fight all of us, you get more experience!", 2, {POKEMON_SCYTHER, POKEMON_PINSIR}, {23,24} };
static Trainer t052 = {52, "Trainer Ronda", "If you fight all of us, you get more experience!", 2, {POKEMON_ELECTABUZZ, POKEMON_JYNX}, {23,24} };
static Trainer t053 = {53, "Rocket Grunt", "Nothing to see here! Just a warehouse.", 2, {POKEMON_RATTATA, POKEMON_RATICATE}, {24,24} };
static Trainer t054 = {54, "Rocket Grunt", "Where did you come from?", 2, {POKEMON_KOFFING, POKEMON_EKANS}, {24,24} };
static Trainer t055 = {55, "Rocket Grunt", "You Shouldn't be here!", 2, {POKEMON_GRIMER, POKEMON_BEEDRILL}, {24,24} };
static Trainer t056 = {56, "Rocket Grunt", "How did you get in here?", 2, {POKEMON_GROWLITHE, POKEMON_GRIMER}, {24,25} };
static Trainer t057 = {57, "Rocket Grunt", "I heard there was a kid like you sneaking around!", 2, {POKEMON_EKANS, POKEMON_MUK}, {25,25} };
static Trainer t058 = {58, "Rocket Grunt", "I'll stop you with my fighting Pokemon!", 2, {POKEMON_MACHOP, POKEMON_HITMONLEE}, {25,25} };
static Trainer t059 = {59, "Rocket Grunt", "You've messed with the wrong crime group!", 2, {POKEMON_LICKITUNG, POKEMON_WEEPINBELL}, {25,26} };
static Trainer t060 = {60, "Rocket Grunt", "Stop trying to mess with our plans!", 2, {POKEMON_VOLTORB, POKEMON_RAICHU}, {25,26} };

static Trainer t061 = {61, "Rocket Grunt", "I found the kid!", 2, {POKEMON_CLEFAIRY, POKEMON_EXEGGCUTE}, {24,24} };
static Trainer t062 = {62, "Rocket Grunt", "How did you find this place?", 2, {POKEMON_MAGIKARP, POKEMON_GYARADOS}, {24,24} };
static Trainer t063 = {63, "Giovanni", "So, you're the one that is causing trouble? Well, I'll show you who's boss", 3, 
                                                    {POKEMON_ONIX, POKEMON_RHYHORN, POKEMON_PERSIAN}, {25,25,29} };
static Trainer t064 = {64, "Gym Trainer Daisy", "I like grass type Pokemon!", 2, {POKEMON_ODDISH, POKEMON_GLOOM}, {24,26} };
static Trainer t065 = {65, "Gym Trainer Violet", "Grass Types are beautiful.", 2, {POKEMON_BELLSPROUT, POKEMON_WEEPINBELL}, {24,26} };
static Trainer t066 = {66, "Gym Trainer Lily", "Erika is a tough trainer. Watch out!", 2, {POKEMON_TANGELA, POKEMON_IVYSAUR}, {26,27} };
static Trainer t067 = {67, "Gym Trainer Clover", "You seem like a good trainer. Let's see how you battle!", 3, 
                                                    {POKEMON_TANGELA, POKEMON_BUTTERFREE, POKEMON_BEEDRILL}, {26,26,27} };
static Trainer t068 = {68, "Rocket Grunt", "Stop trying to mess with our plans!", 2, {POKEMON_GRIMER, POKEMON_ZUBAT}, {26,27} };
static Trainer t069 = {69, "Trainer Terrence", "I'm a trainer too!", 2, {POKEMON_HORSEA, POKEMON_SEADRA}, {15,17} };
static Trainer t070 = {70, "Trainer Adam", "I like flying Pokemon!", 2, {POKEMON_PIDGEOTTO, POKEMON_PIDGEOT}, {17,19} };
static Trainer t071 = {71, "Channeler Patricia", "Ghost Pokemon are everywhere around here.", 2, {POKEMON_GASTLY, POKEMON_GASTLY}, {24, 24} };
static Trainer t072 = {72, "Channeler Karina", "Without that Silph Scope, you wouldn't see Ghost Pokemon!", 2, {POKEMON_GASTLY, POKEMON_GASTLY}, {24, 25} };
static Trainer t073 = {73, "Channeler Angelica", "Be gone!", 2, {POKEMON_GASTLY, POKEMON_HAUNTER}, {24, 26} };
static Trainer t074 = {74, "Channeler Jennifer", "Team Rocket is horrible! Make them Leave!", 2, {POKEMON_GASTLY, POKEMON_HAUNTER}, {25, 26} };
static Trainer t075 = {75, "Channeler Hope", "Team Rocket kidnapped Mr. Fuji. Are you with them?", 2, {POKEMON_HAUNTER, POKEMON_HAUNTER}, {24, 26} };
static Trainer t076 = {76, "Rocket Grunt", "You broke into our hideout in Celadon!", 3, {POKEMON_ZUBAT, POKEMON_ZUBAT, POKEMON_GOLBAT}, {25,25,25} };
static Trainer t077 = {77, "Rocket Grunt", "You should join us!", 4, {POKEMON_ZUBAT, POKEMON_ZUBAT, POKEMON_RATTATA, POKEMON_RATICATE}, {23,24,23,25} };
static Trainer t078 = {78, "Rocket Grunt", "Get out of here, kid!", 2, {POKEMON_KOFFING, POKEMON_DROWZEE}, {26,26} };
static Trainer t079 = {79, "Rocket Grunt", "If you beat me, we'll leave...", 2, {POKEMON_EKANS, POKEMON_WEEZING}, {26,27} };
static Trainer t080 = {80, "Fisherman Val", "I love the water.", 2, {POKEMON_SLOWPOKE, POKEMON_STARYU}, {25,26} };

static Trainer t081 = {81, "Fisherman Sal", "I like fishing. What about you?", 2, {POKEMON_MAGIKARP, POKEMON_GOLDEEN}, {26,26} };
static Trainer t082 = {82, "Fisherman Dal", "Have you been to Fuchsia City?", 2, {POKEMON_SEAKING, POKEMON_SEADRA}, {26,27} };
static Trainer t083 = {83, "Birdcatcher Robyn", "I love catching bird Pokemon!", 2, {POKEMON_FARFETCHD, POKEMON_FEAROW}, {26,26} };
static Trainer t084 = {84, "Trainer Jess", "This is a great place to train", 3, {POKEMON_GROWLITHE, POKEMON_RHYHORN, POKEMON_MACHOKE}, {25,26,27} };


static Trainer * trainers[NUM_TRAINERS] = { &t000, 
    &t001, &t002, &t003, &t004, &t005, &t006, &t007, &t008, &t009, &t010, &t011, &t012, &t013, &t014, &t015, &t016, &t017, &t018, &t019, &t020,
    &t021, &t022, &t023, &t024, &t025, &t026, &t027, &t028, &t029, &t030, &t031, &t032, &t033, &t034, &t035, &t036, &t037, &t038, &t039, &t040,
    &t041, &t042, &t043, &t044, &t045, &t046, &t047, &t048, &t049, &t050, &t051, &t052, &t053, &t054, &t055, &t056, &t057, &t058, &t059, &t060,
    &t061, &t062, &t063, &t064, &t065, &t066, &t067, &t068, &t069, &t070, &t071, &t072, &t073, &t074, &t075, &t076, &t077, &t078, &t079, &t080,
    &t081, &t082, &t083, &t084
};

static Trainer leader201 = {201, "Leader Brock", "I am Brock, Gym Leader of Pewter. Let's Battle!", 2, {POKEMON_GEODUDE, POKEMON_ONIX}, {12,14} };
static Trainer leader202 = {202, "Leader Misty", "I am Misty, my water Type Pokemon are tough, Let's battle!", 2, {POKEMON_STARYU, POKEMON_STARMIE}, {18,21} };
static Trainer leader203 = {203, "Lieutenant Surge", "I am Lt. Surge, Gym Leader of Vermillion. Get ready to be shocked!", 
                                3, {POKEMON_VOLTORB, POKEMON_PIKACHU, POKEMON_RAICHU}, {21,18,24} };
static Trainer leader204 = {204, "Leader Erika", "Welcome to the Celadon Gym. Grass Type Pokemon are stronger than they look. Get ready!", 
                                3, {POKEMON_VICTREEBEL, POKEMON_TANGELA, POKEMON_VILEPLUME}, {29,24,29} };

static Trainer * leaders[10] = { &t000,
    &leader201, &leader202, &leader203, &leader204
};

static Trainer rival001 = {251, "Rival", "I'm your rival, Let's Battle!", 1, {POKEMON_BULBASAUR}, {5} };
static Trainer rival002 = {252, "Rival", "It's been a while, are you any better?", 3, {POKEMON_GEODUDE, POKEMON_RATTATA, POKEMON_BULBASAUR}, {10,11,13} };
static Trainer rival003 = {253, "Rival", "You'll have to beat me to see the Captain", 4, {POKEMON_RATICATE, POKEMON_GRAVELER, POKEMON_KADABRA, POKEMON_IVYSAUR}, {19,20,20,22} };

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
    
    if (strcmp(curr_rival->name, "Rival") != 0) return;
    sprintf(curr_rival->name, "Rival %s", player.rival_name);

    //Find where BULBASAUR evolution is
    for (int i = 0; i < curr_rival->num_in_party; i++) {
        if (curr_rival->party_id_list[i] <= POKEMON_VENUSAUR) {
            rival_starter_index = i;
            rival_starter = curr_rival->party_id_list[i];
        }
    }

    switch (player.original_starter) {
        case POKEMON_BULBASAUR:
            curr_rival->party_id_list[rival_starter_index] = POKEMON_CHARMANDER + rival_starter - 1;
            break;
        case POKEMON_CHARMANDER:
            curr_rival->party_id_list[rival_starter_index] = POKEMON_SQUIRTLE + rival_starter - 1;
            break;
        case POKEMON_SQUIRTLE:
            curr_rival->party_id_list[rival_starter_index] = POKEMON_BULBASAUR + rival_starter - 1;
            break;
        default:
            curr_rival->party_id_list[rival_starter_index] = POKEMON_EEVEE;
            break;
    }

}