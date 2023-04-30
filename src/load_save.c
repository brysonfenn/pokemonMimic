#include "load_save.h"

#include <stdio.h>
#include <time.h>

#include "player.h"
#include "monsters/pokemon.h"
#include "monsters/attacks.h"
#include "items.h"

#define LINE_SIZE 1024

static int current_save_file = 0;

int save_game(int file_num) {
	pokemon curr_pok;
	attack curr_att;
	item curr_item;
	int inputNum;

	FILE *fp;
	char filename[50];
	sprintf(filename, "save_files/save_file%d.txt", file_num);
	fp = fopen(filename, "r");

	//Double check save file
	if (file_num !=  current_save_file && fp != NULL) {
		printf("You selected file %d, which contains data that would be lost if you save ", file_num);
		printf("here.\nAre you sure you want to save to file %d?\n", file_num);
		printf("1: Yes\n0: No\n\n");
		inputNum = getValidInput(0, 1, "Select an option: ");
		if (!inputNum) return 0;
	}

	// Open the file for writing
	fp = fopen(filename, "w");

	// Check if the file was opened successfully
	if (fp == NULL) {
	    printf("Failed to open file.\n");
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
	fprintf(fp, "Player: \n%d %d %d %d\n", player.numInParty, player.numAlive, player.numInBag, player.money);
	fprintf(fp, "Pokemon: \n");
	for (int i = 0; i < player.numInParty; i++) {
		curr_pok = player.party[i];
		fprintf(fp, "%s %d %d %d %d %d %d %d %d %d\n", curr_pok.name, curr_pok.id_num, curr_pok.maxHP, curr_pok.currentHP,
			curr_pok.numAttacks, curr_pok.baseAttack, curr_pok.baseDefense, curr_pok.baseSpeed, curr_pok.level, curr_pok.exp);
		for (int j = 0; j < curr_pok.numAttacks; j++) {
			curr_att = curr_pok.attacks[j];
			fprintf(fp, "\t%s. %d\n", curr_att.name, curr_att.id_num);
		}
	}
	fprintf(fp, "Bag:\n");
	for (int i = 0; i < player.numInBag; i++) {
		curr_item = player.bag[i];
		fprintf(fp, "%s. %d %d\n", curr_item.name, curr_item.id_num, curr_item.number);
	}

	// Close the file
	fclose(fp);

	printf("Game File %d Saved Successfully!\n", file_num); sleep(2);
	current_save_file = file_num;
	return 0;
}

int load_game(int file_num) {
	pokemon * curr_pok;
	attack * curr_att;
	item * curr_item;
	int i, j;

	char temp_name[50];
	int numOfItem, temp_id_num;

	FILE *fp;
    char filename[50];
    sprintf(filename, "save_files/save_file%d.txt", file_num);
    char line[LINE_SIZE];

    // Open the file for reading
    fp = fopen(filename, "r");

    // Check if the file was opened successfully
    if (fp == NULL) {
        printf("That Load File does not exist.\n"); sleep(2);
        return 1;
    }

    // Read lines from the file and put them into game values
    fgets(line, LINE_SIZE, fp);	// Time Stamp
    fgets(line, LINE_SIZE, fp);	// Player: 
    fgets(line, LINE_SIZE, fp);
    sscanf(line, "%d %d %d %d", &(player.numInParty), &(player.numAlive), &(player.numInBag), &(player.money));

	int matched_elements;

    fgets(line, LINE_SIZE, fp);	// Pokemon: 
    for (i = 0; i < player.numInParty; i++) {
		player.party[i] = emptyPok;
		curr_pok = &(player.party[i]);
		fgets(line, LINE_SIZE, fp);
		matched_elements = sscanf(line, "%s %d %d %d %d %d %d %d %d %d", &(curr_pok->name), &(curr_pok->id_num), 
			&(curr_pok->maxHP), &(curr_pok->currentHP), &(curr_pok->numAttacks), &(curr_pok->baseAttack), 
			&(curr_pok->baseDefense), &(curr_pok->baseSpeed), &(curr_pok->level), &(curr_pok->exp));

		//Check if the line matched correctly
		if (matched_elements != 10) {
			printf("ERROR with load file. Expected a POKEMON line with 10 elements.\n");
			printf("Only matched %d elements.\n", matched_elements); sleep(4);
			printf("Offending line is: %s\n", line); sleep(3);
			printf("Reloading...\n"); sleep(1);
			player_init(current_save_file);
			return 2;
		}

		reset_base_stats(curr_pok);

		for (j = 0; j < curr_pok->numAttacks; j++) {
			fgets(line, LINE_SIZE, fp);
			matched_elements = sscanf(line, "\t%[^.]. %d", &temp_name, &temp_id_num);

			//Check if the line matched correctly
			if (matched_elements != 2) {
				printf("ERROR with load file. Expected a MOVE line with 2 elements.\n");
				printf("Only matched %d elements.\n", matched_elements); sleep(4); 
				printf("Offending line is: %s\n", line); sleep(3);
				printf("Reloading...\n"); sleep(1);
				player_init(current_save_file);	// reinitialize player
				return 2;
			}
			curr_pok->attacks[j] = *(get_attack_by_id(temp_id_num));
		}
		for (; j < 4; j++) {
			curr_pok->attacks[j] = empty_attack;
		}
	}

	fgets(line, LINE_SIZE, fp);	// Bag:
	for (i = 0; i < player.numInBag; i++) {

		//Check if there were not enough items
		if (fgets(line, LINE_SIZE, fp) == NULL) {
			printf("ERROR with load file. Too many bag items listed\n");
			printf("Expected: %d, But got %d.\n", player.numInBag, i-1); sleep(4);
			printf("Reloading...\n"); sleep(1);
			player_init(current_save_file); // reinitialize player
			return 2;
		}

		matched_elements = sscanf(line, "%[^.]. %d %d", &temp_name, &temp_id_num, &numOfItem);

		//Check if the line matched correctly
		if (matched_elements != 3) {
			printf("ERROR with load file. Expected an ITEM line with 3 elements.\n");
			printf("Only matched %d elements.\n", matched_elements); sleep(4);
			printf("Offending line is: %s\n", line); sleep(3);
			printf("Reloading...\n"); sleep(1);
			player_init(current_save_file);	// reinitialize player
			return 2;
		}
		player.bag[i] = *(get_item_by_id(temp_id_num));
		player.bag[i].number = numOfItem;
	}


    // Close the file
    fclose(fp);

    printf("Game File %d Loaded Successfully!\n", file_num); sleep(2);
    current_save_file = file_num;
    return 0;
}

void print_save_files() {
	FILE *fp;
    char filename[50];
    char line[LINE_SIZE];

    for (int i = 1; i <= 10; i++) {
    	sprintf(filename, "save_files/save_file%d.txt", i);
	    char line[LINE_SIZE];
	    fp = fopen(filename, "r");

	    // Check if the file was opened successfully
	    if (fp == NULL) { continue; }
	    fgets(line, LINE_SIZE, fp);
	    printf("%d: %s", i, line);
    }
	printf("0: Cancel\n\n");

    if (!current_save_file) {
    	printf("Current Save File: None\n\n");
    }
    else {
    	printf("Current Save File: %d\n\n", current_save_file);
    }
}

