#include <stdio.h>
#include <stdlib.h>

#include <alsa/asoundlib.h>
#include <vorbis/vorbisfile.h>

#include "i_sound.h"

#define PCM_DEVICE "default"


//Global Variables
int audiohead;
audio_t audios[MAXAUDIO];

audio_t I_CreateAudio(char *path)
{
	int err; // Troubleshooting
	audio_t ad;

	ad.path = path;
	ad.vorbissource = fopen(path, "r");


	// Initializing Vorbis //
	err = ov_open_callbacks(ad.vorbissource, &ad.vorbisfile, NULL, 0, OV_CALLBACKS_NOCLOSE);

	if(err < 0)
	{
		printf("%s is not a bitstream\n", path);
		return ad;
	}

	vorbis_info *vi = ov_info(&ad.vorbisfile, -1);
	ad.channels = vi->channels;
	ad.rate = vi->rate;

	if( OV_EINVAL == (ad.seconds = ov_time_total(&ad.vorbisfile, -1)) )
	{
		printf("%s is not seekable\n", path);
		return ad;
	}

	// Initializing Alsa //

	// Opening PCM device in playback mode
	if( (err = snd_pcm_open(&ad.handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	{
		printf("Could not create device handle : %s\n", snd_strerror(err));
		return ad;
	}

	//Allocate memory + default values
	snd_pcm_hw_params_alloca(&ad.params);
	snd_pcm_hw_params_any(ad.handle, ad.params);


	// Setting PCM values
	if( (err = snd_pcm_hw_params_set_access(ad.handle, ad.params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	{
		printf("Cannot set interleaved. %s\n", snd_strerror(err));
		return ad;
	}
	else if( (err = snd_pcm_hw_params_set_format(ad.handle, ad.params, SND_PCM_FORMAT_S16_LE)) < 0)
	{
		printf("Cannot set audio format. %s\n", snd_strerror(err));
		return ad;
	}
	else if( (err = snd_pcm_hw_params_set_channels(ad.handle, ad.params, ad.channels)) < 0)
	{
		printf("Cannot set channels. %s\n", snd_strerror(err));
		return ad;
	}
	else if( (err = snd_pcm_hw_params_set_rate_near(ad.handle, ad.params, &ad.rate, 0)) < 0)
	{
		printf("Could not set rate. %s\n", snd_strerror(err));
		return ad;
	}
	else if( (err = snd_pcm_hw_params(ad.handle, ad.params)) < 0)
	{
		printf("Cannot set hardware parameters. %s\n", snd_strerror(err));
		return ad;
	}
	else
	{
		ad.type = audio_playback;

		// Setting the PCM buffer
		snd_pcm_uframes_t frames;
		snd_pcm_hw_params_get_period_size(ad.params, &frames, 0);
		ad.buffersize = frames * ad.channels * 2;
		ad.buffer = (char*) malloc(ad.buffersize);
	}

	return ad;
}

void I_ClearAudio(audio_t *audio)
{
	ov_clear(&audio->vorbisfile);
	snd_pcm_close(audio->handle);
	free(audio->buffer);

	audio->type = audio_empty;
};

void I_PostAudio(audio_t *audio)
{
	audios[audiohead] = *audio;
	audiohead = (++audiohead)&(MAXAUDIO-1);
}

void I_ProcessAudios(void)
{
	int current_section;
	for (int i=0; i < MAXAUDIO; ++i)
	{
		if(audios[i].type == audio_empty)
			continue;

		long ret = ov_read(&audios[i].vorbisfile, audios[i].buffer, audios[i].buffersize, 0, 2, 1, &current_section);

		if( (snd_pcm_avail(audios[i].handle)) < 0)
        {
			I_ClearAudio(&audios[i]);
            printf("Bye!\n");
        }

		snd_pcm_writei(audios[i].handle, audios[i].buffer, ret / 2 / audios[i].channels);
	}
};

