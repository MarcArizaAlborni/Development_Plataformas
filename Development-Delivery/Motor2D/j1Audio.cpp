#include "p2Defs.h"
#include "p2Log.h"
#include "p2List.h"
#include "j1Audio.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )
#include "Brofiler/Brofiler.h"
#include "j1App.h"
#include "j1SceneUI.h"

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.create("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	uint j = 0;

	Music_Files = config.child("Music").attribute("name").as_string();
	Fx_Files = config.child("Fx").child_value("MovementSounds");

	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	
	// load support for the OGG and Audio formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	TempVolumeMusicValue = 100;
	VolumeMusicValue = 100;

	

	return ret;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	BROFILER_CATEGORY("Audio CleanUp();", Profiler::Color::DarkGoldenRod)
	if (!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;
	for (item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!active)
		return false;

	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	//Area1MusicLvL1 = Mix_LoadMUS("audio/Music/MusicLevel1.ogg");

	//music = Area1MusicLvL1;
	music = Mix_LoadMUS("audio/Music/MenuMusicAudio.ogg");
	if (Area1Level1 == true && App->scene_ui->OnMainMenu == false) {

		music = Mix_LoadMUS("audio/Music/L1_A1.ogg");
	}

	else if (EnemyMusic == true) {
		music = Mix_LoadMUS("audio/Music/EnemyMusic.ogg");
	}

	else if (Area2Level1 == true) {
		music = Mix_LoadMUS("audio/Music/L1_A2.ogg");
	}
	

	
	//music= Mix_LoadMUS("audio/Music/L1_A3.ogg");
	Jump_Sound = Mix_LoadWAV("audio/Music/Jump_1.wav");


	//Jump_Sound = Mix_LoadWAV("audio/FX/Jump_2");


	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
			LOG("Successfully playing %s", path);
		}
	}


	return ret;
}


// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{

	unsigned int ret = 0;

	p2SString tmp("%s%s", Fx_Files.GetString(), path);

	if (!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	Mix_VolumeChunk(chunk, 100);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;

	if (!active)
		return false;

	if (id > 0 && id <= fx.count())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

void j1Audio::VolumeMusic()
{
	Mix_VolumeMusic(VolumeMusicValue);
}

void j1Audio::VolumeFX()
{
	//Mix_VolumeChunk(,VolumeFXValue);
}


