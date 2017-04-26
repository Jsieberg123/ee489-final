/*
 * musicplayer.c
 *
 * Created: 4/20/2017 12:19:13 PM
 *  Author: ec9687os
 */

#include <asf.h>
#include <conf_demo.h>
#include "musicplayer.h"

#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH 128
#define FONT_HEIGHT 7
#define FONT_WIDTH 6

static OLED1_CREATE_INSTANCE(oled1, OLED1_EXT_HEADER);

song songs[2];
unsigned int currentSongNum = 0;
volatile note currentNote;
volatile song currentSong;
volatile int songPos;
bool play;

void onPrev()
{
	currentSongNum = (currentSongNum - 1)%2;
	currentSong = songs[currentSong;]
	songPos = 0;
	gfx_mono_draw_string(currentSong.bane, 10, 10, &sysfont);	
}

void onPausePlay()
{
	play = !play;
}

void onNext()
{
	currentSongNum = (currentSongNum + 1)%2;
	currentSong = songs[currentSong;]
	songPos = 0;
	gfx_mono_draw_string(currentSong.bane, 10, 10, &sysfont);		
}

static void StatusTask(void *params)
{
    gfx_mono_draw_string("<<", 15, 25, &sysfont);
    gfx_mono_draw_string("||>", 58, 25, &sysfont);
    gfx_mono_draw_string(">>", 100, 25, &sysfont);

    bool buttonOneState = false;
    bool buttonTwoState = false;
    bool buttonThreeState = false;
    while (true)
    {
		if (oled1_get_button_state(&oled1, OLED1_BUTTON1_ID) && !buttonOneState)
		{
			buttonOneState = true;
	    	onPrev();
		}
		if (!oled1_get_button_state(&oled1, OLED1_BUTTON1_ID) && buttonOneState)
		{
	    	buttonOneState = false;
		}

		if (oled1_get_button_state(&oled1, OLED1_BUTTON2_ID) && !buttonTwoState)
		{
		    buttonTwoState = true;
		    onPausePlay();
		}
		if (!oled1_get_button_state(&oled1, OLED1_BUTTON2_ID) && buttonTwoState)
		{
		    buttonTwoState = false;
		}	

		if (oled1_get_button_state(&oled1, OLED1_BUTTON3_ID) && !buttonThreeState)
		{
		    buttonThreeState = true;
		    onNext();
		}
		if (!oled1_get_button_state(&oled1, OLED1_BUTTON3_ID) && buttonThreeState)
		{
		    buttonThreeState = false;
		}
    }
}


static void playNoteTask(void *params)
{
	while(true)
	{
		while(currentNote.freq == 0);
		while(!play);

		int delay = 10000/currentNote.freq;
		port_pin_toggle_output_level(PIN_PB30);
		vTaskDelay(delay);
	}
}

static void playSongTask(void *params)
{
	while(true)
	{
		while(!play);

		if(songPos++ >= currentSong.length)
		{
			onNext();
		}
		currentNote = *(currentSong.notes + songPos);
		vTaskDelay(currentNote.duration);
	}
}

void musicPlayer_init()
{
    oled1_init(&oled1);

	play_mutex =  xSemaphoreCreateMutex();

    xTaskCreate(StatusTask, (const char *)"status", 150, NULL, 3, NULL);
    xTaskCreate(playNoteTask, (const char *)"note", 150, NULL, 3, NULL);
    xTaskCreate(playSongTask, (const char *)"song", 150, NULL, 3, NULL);
}
