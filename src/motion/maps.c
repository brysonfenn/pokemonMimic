#include "maps.h"

#include <ncurses.h>

#include "maps/generic_map.h"
#include "maps/map_set1.h"
#include "maps/map_set2.h"
#include "maps/map_set3.h"
#include "maps/map_set4.h"
#include "maps/map_set5.h"
#include "maps/map_set6.h"
#include "maps/map_set7.h"

#include "map_drawing.h"
#include "motion2d.h"
#include "location.h"
#include "../items/key_items.h"
#include "../player.h"
#include "../print/print_utils.h"
#include "../print/print_defines.h"
#include "../monsters/pokemon.h"

//Map functions
static void (*draw_funcs[MAX_MAP_NUM+2])() = { &draw_generic_map, 
                                    &draw_vir_city, &draw_route2, &draw_route1, &draw_starter_town, &draw_lab,                      //1-5
                                    &draw_vir_forest, &draw_pew_city, &draw_gym1, &draw_route3, &draw_mt_moon_n,                    //6-10
                                    &draw_cer_city, &draw_gym2, &draw_route4, &draw_underground, &draw_vermillion_city,             //11-15
                                    &draw_route5, &draw_ss_anne1, &draw_ss_anne2, &draw_gym3, &draw_mt_moon_s,                      //16-20
                                    &draw_route6, &draw_dig_cave, &draw_route7, &draw_museum, &draw_route8,                         //21-25
                                    &draw_route9, &draw_rock_tunnel_n, &draw_rock_tunnel_s, &draw_lavender_town, &draw_cel_city,    //26-30
                                    &draw_route10, &draw_rocket1, &draw_rocket2, &draw_rocket3, &draw_game_corner,                  //31-35
                                    &draw_gym4, &draw_tower1, &draw_tower2, &draw_tower3, &draw_route11,                            //36-40
                                    &draw_route12, &draw_fuchsia_city, &draw_gym5, &draw_route13, &draw_route14,                    //41-45
                                    &draw_route15, &draw_safari1, &draw_safari2, &draw_safari3, &draw_safari4,                      //46-50
                                    &draw_route16, &draw_saffron_city, &draw_gym6, &draw_cinnabar_island, &draw_route17,            //51-55
                                    &draw_gym7, &draw_gym8
                                    };

char map_file_name[32];
static int * wild_pok_list;
static int * water_pok_list;

static Pokemon_id wild_pok_lists[32][16] = {
//  #Pok, levels, ID'S...
    { 2,   3, 5,  POKEMON_PIDGEY, POKEMON_RATTATA }, //#0 Viridian City
    { 2,   2, 4,  POKEMON_PIDGEY, POKEMON_RATTATA }, //#1 Route 1
    { 3,   3, 5,  POKEMON_CATERPIE, POKEMON_WEEDLE, POKEMON_ODDISH }, //#2 Route 2
    { 6,   3, 6,  POKEMON_CATERPIE, POKEMON_WEEDLE, POKEMON_PIKACHU, POKEMON_METAPOD, POKEMON_KAKUNA, POKEMON_PIDGEOTTO }, //#3 Vir Forest
    { 3,   4, 6,  POKEMON_NIDORAN_F, POKEMON_SANDSHREW, POKEMON_SPEAROW }, //#4 Pewter City
    { 3,   5, 8,  POKEMON_JIGGLYPUFF, POKEMON_MANKEY, POKEMON_NIDORAN_M }, //#5 Route 3
    { 4,   7, 9,  POKEMON_GEODUDE, POKEMON_ZUBAT, POKEMON_CLEFAIRY, POKEMON_PARAS }, //#6 Mt Moon S
    { 4,   7, 9,  POKEMON_GEODUDE, POKEMON_ZUBAT, POKEMON_CLEFAIRY, POKEMON_PARAS }, //#7 Mt Moon N
    { 3,   8, 10, POKEMON_VULPIX, POKEMON_EKANS, POKEMON_MANKEY }, //#8 Cerulean City
    { 4,   8, 10, POKEMON_PONYTA, POKEMON_POLIWAG, POKEMON_VULPIX, POKEMON_MEOWTH }, //#9 Route 4
    { 4,   9, 12, POKEMON_BELLSPROUT, POKEMON_PONYTA, POKEMON_DODUO, POKEMON_DROWZEE }, //#10 Route 5
    { 1,  99, 99, POKEMON_CHARMELEON }, //#11 Default list
    { 4,  11, 14, POKEMON_ABRA, POKEMON_DROWZEE, POKEMON_KOFFING, POKEMON_TANGELA }, //#12 Route 6
    { 4,  14, 16, POKEMON_DIGLETT, POKEMON_DIGLETT, POKEMON_DIGLETT, POKEMON_DUGTRIO }, //#13 Diglett's Cave
    { 4,  13, 16, POKEMON_ABRA, POKEMON_TANGELA, POKEMON_BEEDRILL, POKEMON_BUTTERFREE }, //#14 Route 7
    { 4,  16, 21, POKEMON_TAUROS, POKEMON_PINSIR, POKEMON_DODUO, POKEMON_FARFETCHD },         //#15 Route 8
    { 4,  17, 23, POKEMON_GEODUDE, POKEMON_ZUBAT, POKEMON_MACHOP, POKEMON_ONIX },         //#16 Rock Tunnel
    { 4,  19, 25, POKEMON_GASTLY, POKEMON_GASTLY, POKEMON_GASTLY, POKEMON_HAUNTER },         //#17 Pokemon Tower
    { 4,  23, 29, POKEMON_KRABBY, POKEMON_SHELLDER, POKEMON_RHYHORN, POKEMON_GROWLITHE },        //#18 Route 12
    { 4,  25, 30, POKEMON_VOLTORB, POKEMON_VENONAT, POKEMON_EXEGGCUTE, POKEMON_MAGNEMITE },         //#19 Route 13
    { 4,  28, 33, POKEMON_PSYDUCK, POKEMON_CUBONE, POKEMON_PARASECT, POKEMON_PERSIAN },         //#20 Route 14
    { 4,  25, 30, POKEMON_MAGMAR, POKEMON_SEEL, POKEMON_GRIMER, POKEMON_GLOOM },         //#21 Route 15
    { 10,  25, 31, POKEMON_NIDORINA, POKEMON_NIDORINA, POKEMON_NIDORINA, POKEMON_NIDORINO, POKEMON_NIDORINO, 
                    POKEMON_NIDORINO,POKEMON_SCYTHER, POKEMON_PINSIR, POKEMON_LICKITUNG, POKEMON_CHANSEY },         //#22 Safari 1
    { 10,  25, 31, POKEMON_NIDORINA, POKEMON_NIDORINO, POKEMON_NIDORINO, POKEMON_NIDORINO, POKEMON_SCYTHER,
                    POKEMON_SCYTHER, POKEMON_KINGLER, POKEMON_LICKITUNG, POKEMON_POLIWHIRL, POKEMON_RAPIDASH },         //#23 Safari 2
    { 10,  25, 31, POKEMON_MR_MIME, POKEMON_KANGASKHAN, POKEMON_PINSIR, POKEMON_LICKITUNG, POKEMON_CHANSEY,
                    POKEMON_JYNX, POKEMON_ELECTABUZZ, POKEMON_MAROWAK, POKEMON_SANDSLASH, POKEMON_ARCANINE },         //#24 Safari 3
    { 10,  25, 31, POKEMON_RATICATE, POKEMON_KANGASKHAN, POKEMON_KANGASKHAN, POKEMON_PRIMEAPE, POKEMON_DRATINI, 
                    POKEMON_NIDORINO,POKEMON_SCYTHER, POKEMON_DRATINI, POKEMON_DODRIO, POKEMON_CHANSEY },         //#25 Safari 4
};

static Pokemon_id water_pok_lists[32][16] = {
//  #Pok, levels, ID'S...
    { 2,   15, 20, POKEMON_MAGIKARP, POKEMON_TENTACOOL }, //#0 Default Water
    { 6,   25, 31,  POKEMON_GOLDEEN, POKEMON_GOLDEEN, POKEMON_SEAKING, POKEMON_STARYU, POKEMON_SLOWPOKE, POKEMON_HORSEA },          //#1 Safari
    { 6,   25, 31,  POKEMON_TENTACOOL, POKEMON_TENTACOOL, POKEMON_TENTACOOL, POKEMON_HORSEA, POKEMON_TENTACRUEL, POKEMON_HORSEA },  //#2
    { 6,   27, 34,  POKEMON_TENTACOOL, POKEMON_TENTACOOL, POKEMON_SEADRA, POKEMON_HORSEA, POKEMON_STARMIE, POKEMON_SLOWPOKE }       //#3
};

//     POKEMON_BEEDRILL, POKEMON_PIDGEOT, POKEMON_FEAROW, 
//     POKEMON_ARBOK, POKEMON_RAICHU, POKEMON_NIDOQUEEN, POKEMON_NIDOKING, 
//     POKEMON_CLEFABLE, POKEMON_NINETALES, POKEMON_WIGGLYTUFF, POKEMON_GOLBAT, POKEMON_VILEPLUME,
//     POKEMON_GOLDUCK, 
//     POKEMON_POLIWRATH, POKEMON_KADABRA, POKEMON_ALAKAZAM, POKEMON_MACHOKE, 
//     POKEMON_MACHAMP, POKEMON_WEEPINBELL, POKEMON_VICTREEBEL,
//     POKEMON_GRAVELER, POKEMON_GOLEM, POKEMON_SLOWBRO, 
//     POKEMON_MAGNETON, POKEMON_DEWGONG, POKEMON_MUK, POKEMON_CLOYSTER, 
//     POKEMON_GENGAR, POKEMON_HYPNO, POKEMON_KINGLER, POKEMON_ELECTRODE, 
//     POKEMON_EXEGGUTOR, POKEMON_HITMONLEE, POKEMON_HITMONCHAN
//     POKEMON_WEEZING, POKEMON_RHYDON, POKEMON_GYARADOS, 
//     POKEMON_LAPRAS, POKEMON_DITTO, POKEMON_EEVEE, POKEMON_VAPOREON, POKEMON_JOLTEON, POKEMON_FLAREON, POKEMON_PORYGON, 
//     POKEMON_OMANYTE, POKEMON_OMASTAR, POKEMON_KABUTO, POKEMON_KABUTOPS, POKEMON_AERODACTYL, POKEMON_ARTICUNO, 
//     POKEMON_ZAPDOS, POKEMON_MOLTRES, POKEMON_DRAGONAIR, POKEMON_DRAGONITE, POKEMON_MEWTWO, POKEMON_MEW


//Change map drawing function according to map_num
void change_map_funcs(int map_num, void (**draw_map)()) {
    //If map is outside the range, choose generic map functions
    if (map_num < 1 || map_num > MAX_MAP_NUM) {
        *draw_map = &draw_generic_map;
        return;
    }
    *draw_map = draw_funcs[map_num];

    water_pok_list = &(water_pok_lists[0]);

    //Update Static elements map
    char map_name[32] = "empty_map";
    switch (player.loc->map) {
        case MAP_STARTER_TOWN:
            sprintf(map_name, "starter_town");
            break;
        case MAP_R1:
            sprintf(map_name, "route1");
            wild_pok_list = &(wild_pok_lists[1]);
            break;
        case MAP_VIR_CITY:
            sprintf(map_name, "vir_city");
            wild_pok_list = &(wild_pok_lists[0]);
            break;
        case MAP_R2:
            sprintf(map_name, "route2");
            wild_pok_list = &(wild_pok_lists[2]);
            break;
        case MAP_VIR_FOREST:
            sprintf(map_name, "vir_forest");
            wild_pok_list = &(wild_pok_lists[3]);
            break;
        case MAP_PEW_CITY:
            sprintf(map_name, "pew_city");
            wild_pok_list = &(wild_pok_lists[4]);
            break;
        case MAP_R3:
            sprintf(map_name, "route3");
            wild_pok_list = &(wild_pok_lists[5]);
            break;
        case MAP_MT_MOON_S:
            sprintf(map_name, "mt_moon_s");
            wild_pok_list = &(wild_pok_lists[6]);
            break;
        case MAP_MT_MOON_N:
            sprintf(map_name, "mt_moon_n");
            wild_pok_list = &(wild_pok_lists[7]);
            break;
        case MAP_CER_CITY:
            sprintf(map_name, "cer_city");
            wild_pok_list = &(wild_pok_lists[8]);
            break;
        case MAP_GYM2:
            sprintf(map_name, "gym2");
            break;
        case MAP_R4:
            sprintf(map_name, "route4");
            wild_pok_list = &(wild_pok_lists[9]);
            break;
        case MAP_UNDERGROUND:
            sprintf(map_name, "underground");
            break;
        case MAP_R5:
            sprintf(map_name, "route5");
            wild_pok_list = &(wild_pok_lists[10]);
            break;
        case MAP_SS_ANNE1:
            sprintf(map_name, "ss_anne1");
            break;
        case MAP_SS_ANNE2:
            sprintf(map_name, "ss_anne2");
            break;
        case MAP_VERM_CITY:
            sprintf(map_name, "verm_city");
            break;
        case MAP_GYM3:
            sprintf(map_name, "gym3");
            break;
        case MAP_R6:
            sprintf(map_name, "route6");
            wild_pok_list = &(wild_pok_lists[12]);
            break;
        case MAP_DIG_CAVE:
            sprintf(map_name, "dig_cave");
            wild_pok_list = &(wild_pok_lists[13]);
            break;
        case MAP_R7:
            sprintf(map_name, "route7");
            wild_pok_list = &(wild_pok_lists[14]);
            break;
        case MAP_MUSEUM:
            sprintf(map_name, "museum");
            break;
        case MAP_R8:
            sprintf(map_name, "route8");
            wild_pok_list = &(wild_pok_lists[15]);
            break;
        case MAP_R9:
            sprintf(map_name, "route9");
            wild_pok_list = &(wild_pok_lists[15]);
            break;
        case MAP_ROCK_TUNNEL_N:
            sprintf(map_name, "rock_tunnel_n");
            wild_pok_list = &(wild_pok_lists[16]);
            break;
        case MAP_ROCK_TUNNEL_S:
            sprintf(map_name, "rock_tunnel_s");
            wild_pok_list = &(wild_pok_lists[16]);
            break;
        case MAP_LAV_TOWN:
            sprintf(map_name, "lav_town");
            break;
        case MAP_R10:
            sprintf(map_name, "route10");
            break;
        case MAP_CEL_CITY:
            sprintf(map_name, "cel_city");
            break;
        case MAP_ROCKET1:
            sprintf(map_name, "rocket1");
            break;
        case MAP_ROCKET2:
            sprintf(map_name, "rocket2");
            break;
        case MAP_ROCKET3:
            sprintf(map_name, "rocket3");
            break;
        case MAP_TOWER2:
            sprintf(map_name, "tower2");
            wild_pok_list = &(wild_pok_lists[17]);
            break;
        case MAP_TOWER3:
            sprintf(map_name, "tower3");
            wild_pok_list = &(wild_pok_lists[17]);
            break;
        case MAP_GYM4:
            sprintf(map_name, "gym4");
            break;
        case MAP_R11:
            sprintf(map_name, "route11");
            break;
        case MAP_R12:
            sprintf(map_name, "route12");
            wild_pok_list = &(wild_pok_lists[18]);
            break;
        case MAP_FU_CITY:
            sprintf(map_name, "fu_city");
            water_pok_list = &(water_pok_lists[2]);
            break;
        case MAP_GYM5:
            sprintf(map_name, "gym5");
            break;
        case MAP_R13:
            sprintf(map_name, "route13");
            wild_pok_list = &(wild_pok_lists[19]);
            break;
        case MAP_R14:
            sprintf(map_name, "route14");
            wild_pok_list = &(wild_pok_lists[20]);
            break;
        case MAP_R15:
            sprintf(map_name, "route15");
            wild_pok_list = &(wild_pok_lists[21]);
            break;
        case MAP_SAFARI1:
            sprintf(map_name, "safari1");
            wild_pok_list = &(wild_pok_lists[22]);
            water_pok_list = &(water_pok_lists[1]);
            break;
        case MAP_SAFARI2:
            sprintf(map_name, "safari2");
            wild_pok_list = &(wild_pok_lists[23]);
            water_pok_list = &(water_pok_lists[1]);
            break;
        case MAP_SAFARI3:
            sprintf(map_name, "safari3");
            wild_pok_list = &(wild_pok_lists[24]);
            break;
        case MAP_SAFARI4:
            sprintf(map_name, "safari4");
            wild_pok_list = &(wild_pok_lists[25]);
            break;
        case MAP_R16:
            sprintf(map_name, "route16");
            water_pok_list = &(water_pok_lists[2]);
            break;
        case MAP_SAFF_CITY:
            sprintf(map_name, "saff_city");
            break;
        case MAP_GYM6:
            sprintf(map_name, "gym6");
            break;
        case MAP_CIN_ISLAND:
            sprintf(map_name, "cin_island");
            break;
        case MAP_R17:
            sprintf(map_name, "route17");
            water_pok_list = &(water_pok_lists[3]);
            break;
        case MAP_GYM7:
            sprintf(map_name, "gym7");
            break;
        case MAP_GYM8:
            sprintf(map_name, "gym8");
            break;
        default:
            sprintf(map_name, "empty_map");
            wild_pok_list = &(wild_pok_lists[11]);
            break;
    }

    sprintf(map_file_name, "maps/%s.txt", map_name);
}


int * get_wild_pok_list() {
    if (has_key_item(K_ITEM_SURF_FLAG) != -1)
        return water_pok_list;
    else
        return wild_pok_list;
}


//Draw elements (like grass, trees, etc) according to map text file
void draw_static_elements() {
    char file_name[64] = "maps/empty_map.txt";
    sprintf(file_name, "%s", map_file_name);
    FILE *file = fopen(file_name, "r");  // Open the file for reading

    if (file == NULL) {
        printw("Failed to map file.");
        return;
    }

    int ch, y, x;
    char ch8;
    int col_num = 0;
    int line_num = 0;

    //First two line has nothing of value
    while ((ch = fgetc(file)) != '\n') { }
    while ((ch = fgetc(file)) != '\n') { }

    move(line_num+MAP_Y+1, MAP_X);

    while ((ch = fgetc(file)) != EOF) {
        ch8 = ch & A_CHARTEXT;

        //If this is an endline char, move to the next line
        if (ch8 == '\n') {
            line_num++;
            move(line_num+MAP_Y+1, MAP_X);
            col_num = 0;
            continue;
        }

        //Skip first three digits -- used for easier visuals in the text file
        if (col_num < 3) {
            //Last line will start with an f, so break if we see one
            if (ch == 'f') break;
            col_num++;
            continue;
        }

        if (ch == 'W') attrset(COLOR_PAIR(GRASS_COLOR));
        else if (ch == 'Y') attrset(COLOR_PAIR(TREE_COLOR));
        else if (ch == '~') attrset(COLOR_PAIR(WATER_COLOR));

         //If this is a vertical line or a space, move cursor forward rather than printing
         // Vertical line characters in a text file have the value 130
        if (ch == ' ' || ch == 130) {
            getyx(stdscr, y, x);  // Get the current cursor position
            move(y, x+1);   //Move cursor forward one space rather than printing
        }
        else {
            printw("%c", ch);
        }
        
        attrset(COLOR_PAIR(DEFAULT_COLOR));
    }
    refresh();

    fclose(file);  // Close the file
}


//Notify player of moving back to pokecenter, then move them
void move_player_to_poke_center() {
    char print_str[256];

    audio_play_file("back_button.mp3");
    begin_list();
    sprintf(print_str, "%s scurried back to the Pokemon Center...", player.name);
    print_to_list(print_str); sleep(2);
    heal_party();
    // change_map(player.blackout_center->map, POKE_CENTER_X+4, POKE_CENTER_Y+3);
    change_map(player.blackout_center->map, player.blackout_center->x, player.blackout_center->y);

    sleep(1);
}