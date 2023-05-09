#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#define LOAD_FAILURE 0
#define LOAD_SUCCESS 1

int save_game(int file_num);

int load_game(int file_num);

int print_save_files();

int get_current_save_file();

#endif // LOAD_SAVE_H