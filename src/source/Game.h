/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#ifndef GAME_H
#define GAME_H

#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>
#include <vector>
#include <vector>
#include <fat.h>


#include "gl2d.h"
#include "GlFont.h"
#include "Vector2df32.h"
#include "VectorSpring.h"
#include "UTIL.h"
#include "GlobalManager.h"
#include "Sound.h"
#include "MemoryFeedback.h"
#include "Oamfont.h"

#include "Explosion.h"
#include "Particles.h"

#include "Fx2D.h"
#include "Fx3D.h"

// Cearn's awesome Fixed-point ATAN2
#include "cearn_atan.h"

#include "TextMaps.h"

#include "GlobalConstants.h"
#include "Map2D.h"
#include "Renderer3D.h"
#include "Camera.h"
#include "Player.h"

#include "Spiral.h"
#include "DiagSpiral.h"
#include "GreenTurtle.h"
#include "RedTurtle.h"
#include "Bouncer.h"
#include "Jumper.h"
#include "Magneto.h"
#include "Fire.h"

#include "Block.h"
#include "VanishPlatform.h"

#include "Snipe.h"
#include "Coin.h"

#include "ExitPost.h"
#include "BombUp.h"
#include "MagicUp.h"

#include "title.h"
#include "neo_badge.h"
#include "relminator.h"
#include "level01_tiles.h"
#include "water.h"
#include "gripe.h"
#include "bg.h"
#include "enemies_sprite.h"
#include "bullet_sprites.h"
#include "bomb_sprite.h"
#include "explosions_sprite.h"
#include "flares_sprite.h"
#include "fairy_sprite.h"

#include "font_bubbla.h"
#include "font_krom.h"

#include "glfont_krom.h"
#include "glfont_bubbla.h"


//MAXMOD headers
#include "soundbank_bin.h"
#include "soundbank.h"

#include "uvcoord_explosions_sprite.h"
#include "uvcoord_enemies_sprite.h"

#include "demo_bin.h"


class Game 
{

public:

	// Game state 
	enum 
	{
		PLAY = 0,
		REPLAY,
		PAUSE,
		STAGE_COMPLETE,
		STAGE_START,
		PLAYER_DIE,
		INTERMISSION,
		STAGE_REPORT,
		GAME_OVER,
		OPTIONS,
		EXTRAS,
		CREDITS,
		TITLE,
		SPLASH,
		ENDING,
		END
		
	};
	
	// Menu choices
	enum 
	{
		CHOICE_START_GAME = 0,
		CHOICE_OPTIONS,
		CHOICE_EXTRAS,
		CHOICE_CREDITS,
		CHOICE_EXIT
	};
	
	Game();
	~Game();
	void Update();
	void Draw();
	void DrawDebug();
	void DrawSubScreen();
	void StatePlay();
	void StateRePlay();
	void StateStart();
	void StatePause();
	void StatePlayerDie();
	void StateStageComplete();
	void StateIntermission();
	void StateStageReport();
	void StateGameOver();
	void StateOptions();
	void StateExtras();
	void StateCredits();
	void StateTitle();
	void StateSplash();
	void StateEnding();
	void InitHardware();
	void InitEverything();
	void InitEntities();
	void LoadImages();
	void LoadSounds();
	void LoadMap( const char *TextMap[] );
	void RestartLevel( const char *TextMap[] );
	void SetState( int state );
	
	bool IsStageDone() const
	{
		return StageDone;
	}
	
	unsigned int GetStage() const
	{
		return Stage;
	}
	
	void SetStageDone( bool v )
	{
		StageDone = v;
	}
	
	void SetStage( int v )
	{
		Stage = v;
	}
	
	bool IsReplay() const
	{
		return Replay;
	}
	
private:

	void SpawnEnemies( const char *TextMap[] );
	
	typedef void ( Game::*Action )();
	
	Action StateFunction;
	
	int State;
	
	int KeyH, KeyD;		// for key input
	int CameraMode;
	
	unsigned int Stage;
	bool StageDone;
	
	bool DrawAABB;
	int ShotID;
	
	int Frame;
	int Timer;
	bool Blinker;
	bool Replay;
	
	Vector3Df32 PausedCam;
	Vector3Df32 PausedCamLerp;
	
	MemoryFeedback Memory;
	int SubBG3;
	
	Oamfont KromFont;
	Oamfont BubblaFont;

	
	GLFont KromGLFont;
	GLFont BubblaGLFont;
	
	Camera Cam;
	Player Mario;
	MapContainer Map;
	
	int TitleTextureID;
	int RelminatorTextureID;
	int TilesTextureID;
	int WaterTilesTextureID;
	int BGTilesTextureID;
	int PlayerSpritesTextureID;
	int FairySpritesTextureID;
	int EnemySpritesTextureID;
	int BombSpritesTextureID;
	int BulletSpritesTextureID;
	int ExplosionSpritesTextureID;
	int FlaresSpritesTextureID;
	
	
	std::vector<EntityFactory*> Enemies;
	
	glImage TitleSprites[2];
	glImage RelminatorTiles[1];
	glImage Tiles[16];
	glImage WaterTiles[4];
	glImage BGTiles[1];
	glImage PlayerSprites[32];
	glImage FairySprites[16];
	glImage EnemySprites[ENEMIES_SPRITE_NUM_IMAGES];
	glImage BombSprites[4];
	glImage BulletSprites[512/32];
	glImage ExplosionsImages[EXPLOSIONS_SPRITE_NUM_IMAGES];
	glImage FlaresImages[4];
	glImage KromGLFontImages[1024/16];
	glImage BubblaGLFontImages[512/8];
	
	BlockFactory Blocks;
	VanishPlatformFactory VanishPlatforms;
	SpiralFactory Spirals;
	DiagSpiralFactory DiagSpirals;
	GreenTurtleFactory GreenTurtles;
	RedTurtleFactory RedTurtles;
	BouncerFactory Bouncers;
	JumperFactory Jumpers;
	MagnetoFactory Magnetos;
	FireFactory Fires;
	
	SnipeFactory Snipes;
	CoinFactory Coins;
	
	ExitPostFactory ExitPosts;
	BombUpFactory BombUps;
	MagicUpFactory MagicUps;
	
	
};

#endif // GAME_H
