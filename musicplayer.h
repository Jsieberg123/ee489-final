/*
 * MusicPlayer.h
 *
 * Created: 4/20/2017 12:15:53 PM
 *  Author: ec9687os
 */ 


#ifndef MUSICPLAYER_H_
#define MUSICPLAYER_H_

void musicPlayer_init();

struct note {
    unsigned int freq;
    unsigned int duration; 
};

typedef struct note note;

struct song {
    note* notes;
    unsigned int length;
    char* name;
};

typedef struct song song;

#endif /* MUSICPLAYER_H_ */