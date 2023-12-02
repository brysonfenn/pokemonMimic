#include "audio_player.h"

#include <stdbool.h>

#include <string.h>
#include "ncurses.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static ma_result result;
static ma_engine engine;
static bool engine_started = false;

static ma_decoder decoder;
static ma_device device;
static bool looping = false;

char file_path[64];
char looping_file_name[64] = "empty";
char saved_looping_file_names[3][64] = { "wild_battle.mp3", "wild_battle.mp3", "wild_battle.mp3" };


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);


void audio_play_file(char * file_name) {
    if (engine_started) {
        ma_engine_uninit(&engine);
        init_audio_player();
    }
    engine_started = true;

    sprintf(file_path, "audio/%s", file_name);
    ma_engine_play_sound(&engine, file_path, NULL);
}


void audio_loop_file(char * file_name) {
    ma_result result;
    ma_device_config deviceConfig;

    if (strcmp(looping_file_name, file_name) == 0 && looping) {
        return;
    }
    sprintf(looping_file_name, "%s", file_name);

    if (looping) {
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
    }

    sprintf(file_path, "audio/%s", file_name);
    result = ma_decoder_init_file(file_path, NULL, &decoder);
    if (result != MA_SUCCESS) {
        return -2;
    }

    /*
    A decoder is a data source which means we just use ma_data_source_set_looping() to set the
    looping state. We will read data using ma_data_source_read_pcm_frames() in the data callback.
    */
    ma_data_source_set_looping(&decoder, MA_TRUE);

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printw("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printw("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }

    looping = true;
}


void audio_save_looping_file(int index) {
    sprintf(saved_looping_file_names[index], "%s", looping_file_name);
}


void audio_restore_looping_file(int index) {
    audio_loop_file(saved_looping_file_names[index]);
}



void init_audio_player() {
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printw("Could not init audio player"); refresh(); sleep(2);
        return;
    }
}


void audio_end_play() {
    if (engine_started) {
        ma_engine_uninit(&engine);
        engine_started = false;
        init_audio_player();
    }
}


void audio_end_loop() {
    if (looping) {
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        looping = false;
    }
}


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    /* Reading PCM frames will loop based on what we specified when called ma_data_source_set_looping(). */
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}