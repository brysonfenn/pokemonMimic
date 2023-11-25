#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H


void audio_play_file(char * file_name);

void audio_loop_file(char * file_name);

void audio_save_looping_file(int index);

void audio_restore_looping_file(int index);

void init_audio_player();

void audio_end_play();

void audio_end_loop();


#endif // AUDIO_PLAYER_H