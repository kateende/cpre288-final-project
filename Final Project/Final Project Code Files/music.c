/*
 * music.c
 *
 *  Created on: Apr 18, 2024
 *      Author: kateende
 */

#include <Final Project Code Files/open_interface.h>
#include <Final Project Code Files/Timer.h>
#include <Final Project Code Files/music.h>


static uint8_t song0 = 26;
static uint8_t song0Notes[26] = {64,62,60,62,64,64,64,62,62,62,64,64,64,64,62,60,62,64,64,64,64,62,62,64,62,60} ;
static uint8_t song0Durations[26] = {8,8,8,8,8,8,16,8,8,4,16,8,8,16,8,8,8,8,8,8,16,8,8,8,8,8,32};

static uint8_t song1 = 11;
static uint8_t song1Notes[11] = {31, 43, 55, 67, 79, 91, 103, 104, 105, 106, 107} ;
static uint8_t song1Durations[11] = {32, 32, 32, 32, 32, 8, 8, 8, 8, 8, 8};

static uint8_t song2 = 6;
static uint8_t song2Notes[6] = {67, 72, 76, 79, 76, 79} ;
static uint8_t song2Durations[6] = {12, 12, 12, 27, 9, 72};

static uint8_t song3 = 8;
static uint8_t song3Notes[8] = {55, 55, 55, 52, 53, 53, 53, 50} ;
static uint8_t song3Durations[8] = {16, 16, 16, 80, 16, 16, 16, 128};


void load_songs(uint8_t songs_title){

    switch(songs_title){

    //Play this song when we reached the object
    case 0:
        oi_loadSong(0, song0, song0Notes, song0Durations);
        oi_play_song(0);
        break;
        //When we about to hit some shit
    case 1:
        oi_loadSong(1, song1, song1Notes, song1Durations);
        oi_play_song(1);
        break;
        //charge!
    case 2:
        oi_loadSong(2, song2, song2Notes, song2Durations);
        oi_play_song(2);
        break;
    case 3:
        oi_loadSong(3, song3, song3Notes, song3Durations);
        oi_play_song(3);
        break;





    default:

        break;

    }
}
