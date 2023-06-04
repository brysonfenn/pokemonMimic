#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#define LOAD_FAILURE 0
#define LOAD_SUCCESS 1

//Save game into file named "save_file#.text" where # is file_num
int save_game(int file_num);

//Load game from file named "save_file#.text" where # is file_num
int load_game(int file_num);

//Print all save files with time stamp of when they were last saved
int print_save_files();

//Return the current save file if there is one. If not, return zero
int get_current_save_file();

#endif // LOAD_SAVE_H