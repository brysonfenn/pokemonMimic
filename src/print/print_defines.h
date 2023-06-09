#ifndef PRINT_DEFINES_H
#define PRINT_DEFINES_H

#define BATTLE_BOX_X 3
#define BATTLE_BOX_Y 2
#define BATTLE_BOX_WIDTH 50
#define BATTLE_BOX_HEIGHT 7

#define BATTLE_BOX_ENEMY_X (BATTLE_BOX_X + 25)
#define BATTLE_BOX_ENEMY_Y (BATTLE_BOX_Y+1)
#define BATTLE_BOX_PLAYER_X (BATTLE_BOX_X + 3)
#define BATTLE_BOX_PLAYER_Y (BATTLE_BOX_Y+4)

#define TEXT_BOX_X BATTLE_BOX_X
#define TEXT_BOX_Y (BATTLE_BOX_Y + BATTLE_BOX_HEIGHT)
#define TEXT_BOX_WIDTH 50
#define TEXT_BOX_HEIGHT 10

#define LIST_BOX_X 3
#define LIST_BOX_Y 1
#define LIST_BOX_WIDTH 58
#define LIST_BOX_HEIGHT 22

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

#define SELECT_CHAR 'z'
#define CANCEL_CHAR 'x'
#define MENU_CHAR 'm'

#endif // PRINT_DEFINES_H