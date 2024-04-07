#ifndef PRINT_DEFINES_H
#define PRINT_DEFINES_H

#define BATTLE_BOX_X 3
#define BATTLE_BOX_Y 2
#define BATTLE_BOX_WIDTH 58
#define BATTLE_BOX_HEIGHT 7

#define BATTLE_BOX_ENEMY_X (BATTLE_BOX_X + 30)
#define BATTLE_BOX_ENEMY_Y (BATTLE_BOX_Y+1)
#define BATTLE_BOX_PLAYER_X (BATTLE_BOX_X + 3)
#define BATTLE_BOX_PLAYER_Y (BATTLE_BOX_Y+4)

#define TEXT_BOX_X BATTLE_BOX_X
#define TEXT_BOX_Y (BATTLE_BOX_Y + BATTLE_BOX_HEIGHT)
#define TEXT_BOX_WIDTH BATTLE_BOX_WIDTH
#define TEXT_BOX_HEIGHT 10

#define LIST_BOX_X 3
#define LIST_BOX_Y 1
#define LIST_BOX_WIDTH 58
#define LIST_BOX_HEIGHT 23

#define BTN_INSTRUCTIONS_X (TEXT_BOX_X+TEXT_BOX_WIDTH+1)
#define BTN_INSTRUCTIONS_Y (TEXT_BOX_Y)

#define AWAIT_USER_X BATTLE_BOX_X+BATTLE_BOX_WIDTH+1
#define AWAIT_USER_Y TEXT_BOX_Y+7

#define MOVE_SELECT_SPACING 18

#define BATTLE_SELECT_1_X (TEXT_BOX_X + 1)
#define BATTLE_SELECT_2_X (BATTLE_SELECT_1_X + MOVE_SELECT_SPACING)
#define BATTLE_SELECT_3_X (BATTLE_SELECT_1_X + MOVE_SELECT_SPACING * 2)
#define SELECT_Y TEXT_BOX_Y + 1

#define POKE_SUMMARY_ATKS_BEGIN (LIST_BOX_Y + 18)
#define POKE_SUMMARY_SEL_BEGIN (POKE_SUMMARY_ATKS_BEGIN + 3)
#define POKE_SUMMARY_ATKS_X (LIST_BOX_X+8)

#define SELECTION_CHAR '*'
#define ENEMY_TEXT "Foe "
#define LINE_SIZE 1024

#define PRESSED_B -1

#define PLAYER_MOVING_DOWN 'v'
#define PLAYER_MOVING_UP '^'
#define PLAYER_MOVING_LEFT '<'
#define PLAYER_MOVING_RIGHT '>'

//Colors
#define DEFAULT_COLOR 1
#define PLAYER_COLOR 2
#define GRASS_COLOR 3
#define DAMAGED_COLOR 4
#define TRAINER_COLOR 5
#define HEAL_COLOR 6
#define TREE_COLOR 7
#define WATER_COLOR 8
#define NPC_COLOR 9
#define INVERSE_COLOR 10

#define SELECT_CHAR 'a'
#define SELECT_CHAR_2 'z'
#define CANCEL_CHAR 'b'
#define CANCEL_CHAR_2 'x'
#define MENU_CHAR 'y'
#define MENU_CHAR_2 'm'


//NAME INPUT
#define ALPHABET_BEGIN_X MAP_X + 2
#define ALPHABET_BEGIN_Y MAP_Y + 6
#define INPUT_NAME_DONE_CHAR ('z' + 1)


//TEST-DEBUG VARIABLES
#define DISABLE_WILD_ENCOUNTERS false

#endif // PRINT_DEFINES_H