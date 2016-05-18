/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <maxmod9.h>
#include <vector>

#include "UTIL.h"

#ifndef SOUND_H
#define SOUND_H


//#define NO_SOUND


enum E_BGM
{
    E_BGM_LEVEL01,
	E_BGM_LEVEL_COMPLETE,
	E_BGM_LOW_TIME,
	E_BGM_TITLE,
	E_BGM_DEATH
};

enum E_SFX
{
    E_SFX_JUMP,
	E_SFX_BOUNCE,
    E_SFX_HURT,
	E_SFX_ATTACK,
	E_SFX_ATTACK_HIT,
	E_SFX_MAGIC,
	E_SFX_MAGIC_ACTIVE,
	E_SFX_BLOCK_HIT_FLOOR,
	E_SFX_BLOCK_MOVE,
	E_SFX_EXPLODE,
	E_SFX_POWER_UP,
	E_SFX_SNIPE_UP,
	E_SFX_COIN_UP,
	E_SFX_SNIPE_COMPLETE,
	E_SFX_WATER_SPLASH,
	E_SFX_MENU_OK,
	E_SFX_MENU_SELECT,
	E_SFX_YAHOO
	
};

class Sound 
{

	static Sound* ms_instance;

public:
	inline static Sound* Instance();
	inline static void Release();

	inline void Init( mm_addr soundbank );
	inline void SetMode( mm_mode_enum mode );
	inline void SetMasterVolume( mm_word _volume ); 

	inline void LoadBGM( mm_word module_ID );
	inline void LoadSFX( mm_word sample_ID );
	
	inline void UnloadBGM( mm_word module_ID );
	
	inline void PlayBGM( int index, mm_pmode mode );
	inline void PlaySFX( int index );
	
	
	inline void StopBGM();
	
	inline void FadeBGM( s32 &Interp, s32 Speed );
	

private:
	Sound();
	~Sound();
	
	
	std::vector<mm_word> 	BGM_ID;
	std::vector<mm_word>	SFX_ID;
	
	std::vector<mm_sound_effect> SFX;
	
	
	int 			num_bgm;
	int 			num_sfx;
	
	s32 			volume;
	int 			master_volume;		// max 1024
	

};


//--------------------------------------
//	
//--------------------------------------
inline Sound::Sound()
{
	
	num_bgm = 0;
	num_sfx = 0;
	
	volume = 1024 << 12;
	master_volume = 512;
	
}

inline Sound::~Sound()
{
	
	BGM_ID.clear();
	SFX_ID.clear();
	SFX.clear();
	
}

//--------------------------------------
//	
//--------------------------------------
inline Sound* Sound::Instance()
{
	if(ms_instance == 0){
		ms_instance = new Sound();
	}
	return ms_instance;
}

//--------------------------------------
//	
//--------------------------------------
inline void Sound::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}


#ifndef NO_SOUND

//--------------------------------------
//	
//--------------------------------------
inline void Sound::Init( mm_addr soundbank )
{
	mmInitDefaultMem( (mm_addr)soundbank );
}

//--------------------------------------
//	
//--------------------------------------
inline void Sound::SetMode( mm_mode_enum mode )
{
	mmSelectMode( mode );
}

//--------------------------------------
//	
//--------------------------------------
inline void Sound::SetMasterVolume( mm_word _volume )
{
	volume = _volume << 12;
	master_volume = _volume;
	mmSetModuleVolume( _volume );
}

//--------------------------------------
//	
//--------------------------------------
inline void Sound::PlayBGM( int index, mm_pmode mode )
{
	mmStart( BGM_ID[index], mode );
}

//--------------------------------------
//	
//--------------------------------------
inline void Sound::PlaySFX( int index )
{
	mmEffectEx(&SFX[index]);
}

//--------------------------------------
//	
//--------------------------------------
inline void Sound::StopBGM()
{
	mmStop();
}

//--------------------------------------
//	
//--------------------------------------
inline void Sound::FadeBGM( s32 &Interp, s32 Speed )
{
	
	Interp += Speed;
	Interp = UTIL::Clamp(Interp,0,4096);
	
	volume = UTIL::Lerpf32( 0 << 12, master_volume << 12, Interp );
	
	mmSetModuleVolume( volume >> 12 );
	
}


//--------------------------------------
//	
//--------------------------------------
inline void Sound::LoadBGM( mm_word module_ID )
{
	
	mmLoad( module_ID );
	BGM_ID.push_back(module_ID);
	num_bgm++;

}

//--------------------------------------
//	
//--------------------------------------
inline void Sound::LoadSFX( mm_word sample_ID )
{

	mm_sound_effect sfx;
	
	mmLoadEffect( sample_ID );
	
	SFX_ID.push_back(sample_ID);
	
	sfx.id      = sample_ID; 	// sample ID (make sure it is loaded)
	sfx.rate    = 1024;     	// playback rate, 1024 = original sound
	sfx.handle  = 0;           // 0 = allocate new handle
	sfx.volume  = 255;         // volume level
	sfx.panning = 128;     	   // panning at center
	
	SFX.push_back(sfx);
	
	num_sfx++;

}

//--------------------------------------
//	
//--------------------------------------
inline void Sound::UnloadBGM( mm_word module_ID )
{
	mmUnload( module_ID );
	std::vector<mm_word>::iterator iter;
	for( iter = BGM_ID.begin(); iter != BGM_ID.end(); ++iter )
	{
		if( (*iter) == module_ID )
		{
			BGM_ID.erase(iter);
		}
	}
	
}

#else   // NO_SOUND

inline void Sound::Init( mm_addr soundbank )
{
}
inline void Sound::SetMode( mm_mode_enum mode )
{
}
inline void Sound::SetMasterVolume( mm_word _volume )
{
}
inline void Sound::PlayBGM( int index, mm_pmode mode )
{
}
inline void Sound::PlaySFX( int index )
{
}
inline void Sound::StopBGM()
{
}
inline void Sound::FadeBGM( s32 &Interp, s32 Speed )
{
}
inline void Sound::LoadBGM( mm_word module_ID )
{
}
inline void Sound::LoadSFX( mm_word sample_ID )
{
}
inline void Sound::UnloadBGM( mm_word module_ID )
{
}

#endif  // NO_SOUND


#endif // SOUND_H
