#include "load_save.h"

#include <stdio.h>
#include <time.h>

#include "player.h"
#include "monsters/pokemon.h"
#include "monsters/attacks.h"
#include "items.h"
#include "motion/location.h"
#include "monsters/typings.h"
#include "print/print_defines.h"
#include "print/print_utils.h"

#define LINE_SIZE 1024

static int current_save_file = 0;

void notify_error(char * expected, char * line, int expected_elements, int matched_elements);


//Save game into file named "save_file#.text" where # is file_num
int save_game(int file_num) {
	Pokemon curr_pok;
	attack curr_att;
	Item curr_item;
	int inputNum;
	char print_str[1024];

	FILE *fp;
	char filename[50];
	sprintf(filename, "save_files/save_file%d.txt", file_num);
	fp = fopen(filename, "r");

	//Double check save file
	if (file_num !=  current_save_file && fp != NULL) {
		begin_list();
		sprintf(print_str, "You selected file %d, which contains data that would\n", file_num);
		sprintf(print_str, "%sbe lost if you save here. Are you sure you want to\nsave to file %d?\n", print_str, file_num);
		sprintf(print_str, "%s \n  Yes\n  No", print_str);
		print_to_list(print_str);
		inputNum = get_selection(4, 1, 0);
		if (inputNum == 1 || inputNum == PRESSED_B) return 0;
	}

	// Open the file for writing
	fp = fopen(filename, "w");

	// Check if the file was opened successfully
	if (fp == NULL) {
	    printw("Failed to open file.\n");
	    return 1;
	}

	//Get Time Stamp:
	time_t current_time;
    struct tm* time_info;
    char time_string[80];
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_string, sizeof(time_string), "%m/%d/%Y %H:%M:%S", time_info);

    // Print time string
    fprintf(fp, "Last Saved: %s\n", time_string);

	// Write the message to the file
	fprintf(fp, "Player: \n%d %d %d\n", player.numInParty, player.numInBag, player.money);
	fprintf(fp, "Location: {%d,(%d,%d)}\n", player.loc->map, player.loc->x, player.loc->y);
	fprintf(fp, "Heal Center: {%d,(%d,%d)}\n", player.blackout_center->map, player.blackout_center->x, player.blackout_center->y);
	fprintf(fp, "Pokemon: \n");
	for (int i = 0; i < player.numInParty; i++) {
		curr_pok = player.party[i];
		fprintf(fp, "%s %d %d %d %d %d %d %d %d %d %d %d %s %s %x %d %d\n", curr_pok.name, curr_pok.id_num, curr_pok.maxHP, curr_pok.currentHP,
			curr_pok.numAttacks, curr_pok.baseAttack, curr_pok.baseDefense, curr_pok.baseSpAttack, curr_pok.baseSpDefense,
			curr_pok.baseSpeed, curr_pok.level, curr_pok.exp, get_type_string_by_id(curr_pok.type1), get_type_string_by_id(curr_pok.type2),
			curr_pok.iv, curr_pok.visible_condition, curr_pok.sleep_count);
			
		for (int j = 0; j < curr_pok.numAttacks; j++) {
			curr_att = curr_pok.attacks[j];
			fprintf(fp, "\t%s. %d %d\n", curr_att.name, curr_att.id_num, curr_att.curr_pp);
		}
	}

	
	fprintf(fp, "Bag:\n");
	for (int i = 0; i < player.numInBag; i++) {
		curr_item = player.bag[i];
		fprintf(fp, "%s. %d %d\n", curr_item.name, curr_item.id_num, curr_item.number);
	}

	fprintf(fp, "\nTrainers Battled: %d\n", player.num_trainers_battled);
	fprintf(fp, "  ");
	for (int i = 0; i < player.num_trainers_battled; i++) {
		fprintf(fp, "%d ", player.trainers_battled_id[i]);
	}
	fprintf(fp, "\n");

	// Close the file
	fclose(fp);

	sprintf(print_str, " \nGame File %d Saved Successfully!\n", file_num);
	print_to_list(print_str); sleep(2);
	current_save_file = file_num;

	return 0;
}


//Load game from file named "save_file#.text" where # is file_num
int load_game(int file_num) {

	Pokemon * curr_pok;
	attack * curr_att;
	Item * curr_item;
	int i, j;
	char print_str[1024];

	char temp_name[50];
	int numOfItem, temp_id_num, temp_pp;

	FILE *fp;
    char filename[50];
    sprintf(filename, "save_files/save_file%d.txt", file_num);
    char line[LINE_SIZE];

    // Open the file for reading
    fp = fopen(filename, "r");

    // Check if the file was opened successfully
    if (fp == NULL) {
		sprintf(print_str, " \n  Load File %d does not exist.\n", file_num);
        print_to_list(print_str); sleep(2);
        return LOAD_FAILURE;
    }

    // Read lines from the file and put them into game values
    fgets(line, LINE_SIZE, fp);	// Time Stamp
    fgets(line, LINE_SIZE, fp);	// Player: 
    fgets(line, LINE_SIZE, fp);
    sscanf(line, "%d %d %d", &(player.numInParty), &(player.numInBag), &(player.money));

	int map, new_x, new_y;
	fgets(line, LINE_SIZE, fp);
	sscanf(line, "Location: {%d,(%d,%d)}", &map, &(new_x), &(new_y));

	player.loc->map = map;
	player.loc->x = new_x;
	player.loc->y = new_y;

	fgets(line, LINE_SIZE, fp);
	sscanf(line, "Heal Center: {%d,(%d,%d)}", &map, &(new_x), &(new_y));

	player.blackout_center->map = map;
	player.blackout_center->x = new_x;
	player.blackout_center->y = new_y;

	int matched_elements;

    fgets(line, LINE_SIZE, fp);	// Pokemon: 
    for (i = 0; i < player.numInParty; i++) {
		player.party[i] = emptyPok;
		curr_pok = &(player.party[i]);
		fgets(line, LINE_SIZE, fp);

		char type1[20];
		char type2[20];
		int exp, iv;

		matched_elements = sscanf(line, "%s %d %d %d %d %d %d %d %d %d %d %d %s %s %x %d %d", &(curr_pok->name), &(curr_pok->id_num), 
			&(curr_pok->maxHP), &(curr_pok->currentHP), &(curr_pok->numAttacks), &(curr_pok->baseAttack), 
			&(curr_pok->baseDefense), &(curr_pok->baseSpAttack), &(curr_pok->baseSpDefense), &(curr_pok->baseSpeed), 
			&(curr_pok->level), &exp, &type1, &type2, &iv, &(curr_pok->visible_condition), &(curr_pok->sleep_count));

		curr_pok->exp = exp;
		curr_pok->iv = iv;
		curr_pok->type1 = get_type_id_by_string(type1);
		curr_pok->type2 = get_type_id_by_string(type2);

		//Check if the line matched correctly
		if (matched_elements != 17) {
			notify_error("POKEMON", line, 17, matched_elements);
			return LOAD_FAILURE;
		}

		reset_stat_stages(curr_pok);

		//Attacks
		for (j = 0; j < curr_pok->numAttacks; j++) {
			fgets(line, LINE_SIZE, fp);
			matched_elements = sscanf(line, "\t%[^.]. %d %d", &temp_name, &temp_id_num, &temp_pp);

			//Check if the line matched correctly
			if (matched_elements != 3) {
				notify_error("MOVE", line, 3, matched_elements);
				return LOAD_FAILURE;
			}
			curr_pok->attacks[j] = *(get_attack_by_id(temp_id_num));
			curr_pok->attacks[j].curr_pp = temp_pp;
		}
		for (; j < 4; j++) {
			curr_pok->attacks[j] = empty_attack;
		}
	}

	fgets(line, LINE_SIZE, fp);	// Bag:
	for (i = 0; i < player.numInBag; i++) {
		fgets(line, LINE_SIZE, fp);

		matched_elements = sscanf(line, "%[^.]. %d %d", &temp_name, &temp_id_num, &numOfItem);

		//Check if the line matched correctly
		if (matched_elements != 3) {
			notify_error("ITEM", line, 3, matched_elements);
			return LOAD_FAILURE;
		}
		player.bag[i] = *(get_item_by_id(temp_id_num));
		player.bag[i].number = numOfItem;
	}

	int id;

	fgets(line, LINE_SIZE, fp); //Empty line
	fgets(line, LINE_SIZE, fp); //Trainers Battled
	// sscanf(line, "Trainers Battled: %d\n", &(player.num_trainers_battled));

	fgets(line, LINE_SIZE, fp);
	char * trainer_id_str = line;
	trainer_id_str += 2;	//Move up two spaces
	player.num_trainers_battled = 0; //Reset num trainers battled
	

	// Scan the string for integers
    while (sscanf(trainer_id_str, "%d", &id) == 1) {
        player.trainers_battled_id[player.num_trainers_battled] = id;
		player.num_trainers_battled++;

        // Move to the next number in the string
        trainer_id_str = strchr(trainer_id_str, ' ');
        if (trainer_id_str == NULL) {
            break;
        }
        trainer_id_str++;	//Move up a space
    }

    // Close the file
    fclose(fp);

	sprintf(print_str, " \nGame File %d Loaded Successfully!\n", file_num);
    print_to_list(print_str); sleep(2);
    current_save_file = file_num;
    return LOAD_SUCCESS;
}


//Print all save files with time stamp of when they were last saved
int print_save_files() {
	FILE *fp;
    char filename[50];
    char line[LINE_SIZE];
	char print_str[4096] = "";

	int num_save_files = 0;

    for (int i = 1; i <= 9; i++) {
    	sprintf(filename, "save_files/save_file%d.txt", i);
	    char line[LINE_SIZE];
	    fp = fopen(filename, "r");

	    // Check if the file was opened successfully
	    if (fp == NULL) { 
			sprintf(print_str, "%s  %d: Empty\n", print_str, i);
			continue; 
		}
	    fgets(line, LINE_SIZE, fp);
	    sprintf(print_str, "%s  %d: %s", print_str, i, line);

		num_save_files++;
    }
	sprintf(print_str, "%s  Cancel\n\n", print_str);

    if (!current_save_file) {
    	sprintf(print_str, "%s \n  Current Save File: None\n", print_str);
    }
    else {
    	sprintf(print_str, "%s \n  Current Save File: %d\n", print_str, current_save_file);
    }

	print_to_list(print_str);

	return num_save_files;
}


//Return the current save file if there is one. If not, return zero
int get_current_save_file() {
	return current_save_file;
}


//Notify if there was an error with the load file
void notify_error(char * expected, char * line, int expected_elements, int matched_elements) {
	char print_str[256];

	begin_list();
	sprintf(print_str, "ERROR with load file. Expected a %s line with \n%d elements. ", expected, expected_elements);
	sprintf(print_str, "%sOnly matched %d elements.\n", print_str, matched_elements);
	print_to_list(print_str); sleep(4);
	sprintf(print_str, "Offending line is: \n  %s\n", line);
	print_to_list(print_str); sleep(3);
	print_to_list("Reloading...\n"); sleep(1);
	player_init(current_save_file);
}