#ifndef __I_SOUND_H__
#define __I_SOUND_H__

#include <alsa/asoundlib.h>
#include <vorbis/vorbisfile.h>

#define MAXAUDIO 8
typedef enum
{
	audio_empty,
	audio_playback,
} audiotype;

typedef struct
{
	char* path;
	audiotype type;

	int seconds;
	unsigned int rate;
	unsigned int channels;

	// Vorbis data
	FILE* vorbissource;
	OggVorbis_File vorbisfile;

	//PCM Elements
	char* buffer;
	int buffersize;

	snd_pcm_t* handle;
	snd_pcm_hw_params_t *params;
} audio_t;

//Global Variables
extern int audiohead;
extern audio_t audios[MAXAUDIO];

// Audio functions
audio_t I_CreateAudio(char *path);

void I_PostAudio(audio_t *audio);
void I_ProcessAudios(void);


#endif
