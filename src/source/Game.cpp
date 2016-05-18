/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Game.h"

#define MAX_TIMER (10000-1)

	
Game::Game()
{
	State = PLAY;
	CameraMode = 2;
	DrawAABB = false;
	Blinker = false;
	Replay = false;
}

Game::~Game()
{
	Stage = 0;
	StageDone = false;
	
	Frame = 0;
	Timer = 0;
	
}


void Game::Update()
{
	
	(this->*StateFunction)();
	
}

void Game::Draw()
{
	
	static int BombFrame = 0;
	static int MagicFrame = 0;
	
	GlobalManager::Instance()->ResetAll();
		
	glPushMatrix();
		
		glScalef32( 1 << 12, -1<< 12, 1 << 12 );
		
		Cam.Look();
		
		
		glBindTexture(GL_TEXTURE_2D, TilesTextureID);
		Map.Base.Draw( Tiles );
		//Map.Collision.DrawDebug( Tiles );
		
		glBindTexture(GL_TEXTURE_2D, ExplosionSpritesTextureID);
		Explosion::Instance()->Draw( 8 );
		
		glBindTexture(GL_TEXTURE_2D, WaterTilesTextureID);
		Map.Base.DrawTranslucent( Tiles );
		
		glBindTexture(GL_TEXTURE_2D, BGTilesTextureID);
		Map.Base.DrawBG( Mario.GetX() >> 12, Mario.GetY() >> 12, -2<<12, BGTiles );
		
		glBindTexture(GL_TEXTURE_2D, PlayerSpritesTextureID);
		Mario.Draw( PlayerSprites );
		
		glBindTexture(GL_TEXTURE_2D, BulletSpritesTextureID);
		Mario.DrawShots( BulletSprites );
		
		glBindTexture(GL_TEXTURE_2D, BombSpritesTextureID);
		Mario.DrawBombs( BombSprites );
		
		//glBindTexture(GL_TEXTURE_2D, FairySpritesTextureID);
		//Mario.DrawFairy( FairySprites );
		
		
		glBindTexture(GL_TEXTURE_2D, FlaresSpritesTextureID);
		Particle::Instance()->Draw( 256 );

		
		glBindTexture(GL_TEXTURE_2D, EnemySpritesTextureID);
		
		std::vector<EntityFactory*>::iterator iter;
	
		for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
		{
			(*iter)->DrawEntities( -4 );
		}

		
		if( DrawAABB )
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			
			for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
			{
				(*iter)->DrawAABB( 4, RGB15(31,0,31) );
			}
	
			Mario.DrawAABB( 4, RGB15(31,0,31) );
		}
		
		
		switch( State )
		{
			case PLAY:
				if( (Frame & 3) == 0 )
				{
					BombFrame = ( BombFrame + 1 ) % 4;
					MagicFrame = ( MagicFrame + 1 ) % 8;
				}				
				break;
			case PAUSE:
				break;
			case STAGE_START:
				break;
			case PLAYER_DIE:
				break;
			case INTERMISSION:
				break;
			case GAME_OVER:
				break;
			default:
				break;
		}
		
	glPopMatrix(1);


	// End drawing in 3D mode
	
	
	
	// Start 2D mode
	glBegin2D();

		glSprite( 0, -5, GL_FLIP_NONE, &EnemySprites[1+BombFrame] );
		glSprite( -5, 10, GL_FLIP_NONE, &EnemySprites[33+MagicFrame] );
		BubblaGLFont.Print( 8 * 2, 0, "=" );
		BubblaGLFont.Print( 8 * 2, 20, "=" );
		BubblaGLFont.PrintAscii( 8 * 3, 0, 42, Mario.GetBombsLeft() );
		BubblaGLFont.PrintAscii( 8 * 3, 20, 42, Mario.GetShotsLeft() );
		
		/*
		KromGLFont.Print( 164, 0, "SC" );
		BubblaGLFont.PrintScore( 196, 4, Mario.GetScore(), "0", 7 );
		
		KromGLFont.Print( 206, 20, "T" );
		BubblaGLFont.PrintScore( 220, 24, Timer, "0", 4 );
		
		int Lives = Mario.GetLives();
		if( (State == PLAYER_DIE) || (State == GAME_OVER) ) Lives++;
		KromGLFont.Print( 224, 40, "L" );
		BubblaGLFont.PrintScore( 238, 44, Lives, "0", 2 );
		*/
		
		int col = abs(sinLerp( Frame * 512 )) * 31;  
		switch( State )
		{
			case PLAY:
				if( ExitPosts.IsOpen() )
				{
					glColor( RGB15(31,col >> 12,31));
					BubblaGLFont.Print( 180, 0, "WARP OPEN" );
					glColor( RGB15(31,31,31));
				}
				break;
			case REPLAY:
				if( Blinker ) KromGLFont.PrintCentered( 0, 60, "DEMO" );
				glColor( RGB15(31,col >> 12,31));
				BubblaGLFont.PrintCentered( 0, 90, "PRESS <START> TO RETURN." );
				glColor( RGB15(31,31,31));
				break;
			case PAUSE:
				if( Blinker ) KromGLFont.PrintCentered( 0, 60, "PAUSED" );
				BubblaGLFont.PrintCentered( 0, 90, "PRESS <R> TO ZOOM AWAY." );
				BubblaGLFont.PrintCentered( 0, 100, "<ARROWS> TO SURVEY THE STAGE." );
				BubblaGLFont.PrintCentered( 0, 110, "<ARROWS> + <R> WORKS TO0." );
				break;
			case STAGE_START:
				if( Blinker ) KromGLFont.PrintCentered( 0, 60, "READY" );
				break;
			case PLAYER_DIE:
				KromGLFont.PrintCentered( 0, 60, "OUCH!" );
				break;
			case INTERMISSION:
				KromGLFont.Print( 70, 50, "STAGE ", Stage + 1 );
				KromGLFont.Print( 80, 90, "LEFT ", Mario.GetLives() );
				break;
			case GAME_OVER:
				KromGLFont.PrintCentered( 0, 60, "- GAME OVER -" );
				if( Blinker ) BubblaGLFont.PrintCentered( 0, 90, "PRESS START TO RETURN TO TITLE." );
				break;
			default:
				break;
		}
		
	glEnd2D();

	
	
	glFlush( 0 );

	swiWaitForVBlank();

	DrawSubScreen();
	
	if( !DrawAABB )
	{
		
		iprintf( "\x1b[1;1HJump = [A]                 " );
		iprintf( "\x1b[2;1HZoom = Shoulder [R]or[L]   " );
		iprintf( "\x1b[3;1HAttack [B]                 ");
		iprintf( "\x1b[4;1HShoot [X]                  ");
		iprintf( "\x1b[5;1HDrawAABBs [Select] = %i    ", DrawAABB );
		iprintf( "\x1b[7;1HPause[Start]               " );
		iprintf( "\x1b[8;1HReset[L + R]               " );
		
		iprintf( "\x1b[12;1HBombs = %i            ", Mario.GetBombsLeft() );
		iprintf( "\x1b[13;1HMagic = %i            ", Mario.GetShotsLeft() );
		
		iprintf( "\x1b[18;1HRelminator   " );
		iprintf( "\x1b[19;1Hhttp://Rel.Phatcode.Net   " );
	}
	else
	{
		DrawDebug();
	}
	
}

void Game::StatePlay()
{
	
	scanKeys();
	KeyH = keysHeld();
	KeyD = keysDown();
	
	
	if( Mario.GetLives() < 0 )
	{
		SetState(GAME_OVER);
		return;
	}
	
	ExitPosts.SetOpen(false);
	if( Snipes.GetNumEntities() == 0 )
	{
		ExitPosts.SetOpen(true);
	}
	
	
	if( Mario.InExit() && ( ExitPosts.IsOpen() ) )
	{
		SetState(STAGE_COMPLETE);
		return;
	}
	
	if( Mario.IsDead() )
	{
		SetState(PLAYER_DIE);
		return;
	}
	
	if( Snipes.IsSnipeKilled() )
	{
		SetState(PLAYER_DIE);
		return;
	}
	
	if( Timer == 4000 )
	{
		Sound::Instance()->PlayBGM( E_BGM_LOW_TIME, MM_PLAY_LOOP );
	}
	
	if( Timer <=0 )
	{
		SetState(PLAYER_DIE);
		return;
	}
	
	
	if(KeyD & KEY_SELECT)
	{
		Mario.Die();
		return;
		
	}
	
	if( (KeyH & KEY_R) && (KeyH & KEY_L) )
	{
		Mario.Init();
		StageDone = true;
		SetState(TITLE);
	}
	
	if(KeyD & KEY_START)
	{
		SetState(PAUSE);
		PausedCam.x = Mario.GetX() + PausedCamLerp.x;
		PausedCam.y = Mario.GetY() + PausedCamLerp.y;
		PausedCam.z = Cam.GetEyeDistanceFromScreen();
		return;
	}
	
	
	
	CameraMode = 2;
	Cam.SetMode( CameraMode );
	Cam.Zoom(floattof32(-0.2));	

	Frame++;
	
	Timer = MAX_TIMER - Frame;
	
	Mario.SetKeys( KeyH, KeyD );
	
	
	Explosion::Instance()->Update();
	
	
	Map.RestoreCollisionTiles();
	
	// Clear invisible tiles set by enemies
	// So that blocks can move
	Map.Collision.ClearBlockCollision();
	
	Blocks.SortAll();
	
	std::vector<EntityFactory*>::iterator iter;
	for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
	{
		(*iter)->UpdateEntities( Mario, Map.Collision );
	}
	 
	for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
	{
		(*iter)->CollideWithPlayer( Mario, Map.Collision );
	}
	
	
	//Collide every enemy with movable blocks
	for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
	{
		(*iter)->CollideWithBlocks( Blocks, Mario );
	}
	
	
	Mario.Update( KeyH, KeyD, Map );
	
	Particle::Instance()->Update();
	
	Cam.Update( Mario.GetX(), Mario.GetY(), UTIL::Sign(Mario.GetXV()), 0, PausedCamLerp.x, PausedCamLerp.y, Map.Collision );
	
	
	
}

void Game::StateRePlay()
{
	
	scanKeys();
	KeyH = keysHeld();
	KeyD = keysDown();
	
	
	if( Mario.GetLives() < 0 )
	{
		SetState(GAME_OVER);
		return;
	}
	
	ExitPosts.SetOpen(false);
	if( Snipes.GetNumEntities() == 0 )
	{
		ExitPosts.SetOpen(true);
	}
	
	
	if( Mario.InExit() && ( ExitPosts.IsOpen() ) )
	{
		SetState(STAGE_COMPLETE);
		return;
	}
	
	if( Mario.IsDead() )
	{
		SetState(PLAYER_DIE);
		return;
	}
	
	if( Snipes.IsSnipeKilled() )
	{
		SetState(PLAYER_DIE);
		return;
	}
	
	if( Timer <=0 )
	{
		SetState(PLAYER_DIE);
		return;
	}
	
	if(KeyD & KEY_START)
	{
		SetState(TITLE);
		StageDone = true;
		return;
	}
	
	/*
	if(KeyD & KEY_SELECT)
	{
		DrawAABB = !DrawAABB;
	}
	
	if( (KeyH & KEY_R) && (KeyH & KEY_L) )
	{
		SetState(PLAYER_DIE);
		return;
	}
	
	if(KeyD & KEY_START)
	{
		SetState(PAUSE);
		PausedCam.x = Mario.GetX() + PausedCamLerp.x;
		PausedCam.y = Mario.GetY() + PausedCamLerp.y;
		PausedCam.z = Cam.GetEyeDistanceFromScreen();
		return;
	}
	*/
	
	
	CameraMode = 2;
	Cam.SetMode( CameraMode );
	Cam.Zoom(floattof32(-0.2));	

	Frame++;
	
	if( (Frame & 31) == 0 ) Blinker = !Blinker;
	
	Timer = MAX_TIMER - Frame;
	
	Mario.SetKeys( KeyH, KeyD );
	
	
	Explosion::Instance()->Update();
	
	
	Map.RestoreCollisionTiles();
	
	// Clear invisible tiles set by enemies
	// So that blocks can move
	Map.Collision.ClearBlockCollision();
	
	Blocks.SortAll();
	
	std::vector<EntityFactory*>::iterator iter;
	for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
	{
		(*iter)->UpdateEntities( Mario, Map.Collision );
	}
	 
	for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
	{
		(*iter)->CollideWithPlayer( Mario, Map.Collision );
	}
	
	
	//Collide every enemy with movable blocks
	for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
	{
		(*iter)->CollideWithBlocks( Blocks, Mario );
	}
	
	
	Mario.UpdateReplay( Map );
	
	Particle::Instance()->Update();
	
	Cam.Update( Mario.GetX(), Mario.GetY(), UTIL::Sign(Mario.GetXV()), 0, PausedCamLerp.x, PausedCamLerp.y, Map.Collision );
	
	
	
}

void Game::StatePause()
{
	
	s32 sx = PausedCam.x;
	s32 sy = PausedCam.y;
	
	s32 Lxv;
	s32 Lyv;
	
	int FrameCounter = 0;
	Blinker = true;
	
	Sound::Instance()->PlaySFX(E_SFX_MENU_OK);
	
	while( State == PAUSE )
	{	
		
		FrameCounter++;
		if( (FrameCounter & 31) == 0 ) Blinker = !Blinker;
	
		scanKeys();
		KeyH = keysHeld();
		KeyD = keysDown();
		
		if(KeyD & KEY_START)
		{
			break;
		}
			
		Lxv = 0;
		Lyv = 0;
		
		if( KeyH & KEY_UP ) 
		{
			Lyv = -floattof32( 0.100);
		}
		
		if( KeyH & KEY_DOWN )
		{
			Lyv = floattof32( 0.100);
		}
		
		if( KeyH & KEY_RIGHT )
		{
			Lxv = floattof32( 0.100);
		}
		
		if( KeyH & KEY_LEFT )
		{
			Lxv = -floattof32( 0.100);
		}
		
		PausedCam.x += Lxv;
		PausedCam.y += Lyv;
	
		
		if(KeyH & KEY_R)
		{
			
			PausedCam.z = floattof32(0.2);
			Cam.Zoom( PausedCam.z );
		}
		else
		{
			//Cam.SetMode(2);
			PausedCam.z = floattof32(-0.2);
			Cam.Zoom( PausedCam.z );
		
		}	
		
		Cam.UpdatePaused( PausedCam.x, PausedCam.y, Mario.GetX(), Mario.GetY(), Map.Collision );
		
		Draw();
		
	}
	
	
	const int Steps = 32;
	s32 Dx = (sx - PausedCam.x) / Steps;
	s32 Dy = (sy - PausedCam.y) / Steps;
	
	for( int i = 0; i < Steps; i++ )
	{
		PausedCam.x += Dx;
		PausedCam.y += Dy;
		
		Cam.UpdatePaused( PausedCam.x, PausedCam.y, Mario.GetX(), Mario.GetY(), Map.Collision );
		Draw();
	}	
	
	Sound::Instance()->PlaySFX(E_SFX_MENU_OK);
	
	SetState(PLAY);
		
}

void Game::StatePlayerDie()
{
	
	int FrameCounter = 60 * 3;
	
	Sound::Instance()->PlayBGM( E_BGM_DEATH, MM_PLAY_ONCE );
	
	while( State == PLAYER_DIE )
	{	
		FrameCounter--;
		if( FrameCounter < 0 )
		{
			SetState(INTERMISSION);
		}
	
		Cam.Update( Mario.GetX(), Mario.GetY(), 0, 0, PausedCamLerp.x, PausedCamLerp.y, Map.Collision );
	
		Draw();
		
	}
		
	StageDone = true;
		
}

void Game::StateStageComplete()
{
	
	int FrameCounter = 60 * 3;
	
	Sound::Instance()->PlayBGM( E_BGM_LEVEL_COMPLETE, MM_PLAY_ONCE );
	Sound::Instance()->PlaySFX(E_SFX_YAHOO);
	
	while( State == STAGE_COMPLETE )
	{	
		FrameCounter--;
		if( FrameCounter < 0 )
		{
			SetState(STAGE_REPORT);
		}
		
		if( (FrameCounter & 31) == 0 ) Blinker = !Blinker;
	
		Cam.Update( Mario.GetX(), Mario.GetY(), 0, 0, PausedCamLerp.x, PausedCamLerp.y, Map.Collision );
		Draw();
		
	}
	
	if( !Replay )
	{
		Mario.AddLives(1);
		if( Stage == (TextMap::Maps.size()-1 ) )
		{
			SetState( ENDING );
		}
		Stage = (Stage + 1) % TextMap::Maps.size();
	}
		
}

void Game::StateStart()
{
	
	
	s32 Zoom = ((115 * TILE_SIZE_3D) / TILE_SIZE) * 2;
	
	int FrameCounter = 60 * 2;
	
	Cam.Zoom( Zoom );
		
	//Sound::Instance()->PlayBGM( E_BGM_LEVEL01, MM_PLAY_LOOP );
	Blinker = false;
	
	while( State == STAGE_START )
	{	
		FrameCounter--;
		if( FrameCounter < 0 )
		{
			if( !Replay )
				SetState(PLAY);
			else
				SetState(REPLAY);
		}
	
		if( (FrameCounter & 15) == 0 ) Blinker = !Blinker;
	
		Cam.Zoom( floattof32(-0.05) );
	
		Cam.Update( Mario.GetX(), Mario.GetY(), 0, 0, PausedCamLerp.x, PausedCamLerp.y, Map.Collision );
	
		Draw();
		
	}
	
	StageDone = false;
		
}


void Game::StateIntermission()
{
	
	int FrameCounter = 60 * 2;
	s32 Zoom = ((115 * TILE_SIZE_3D) / TILE_SIZE) * 2;
	Cam.Zoom( Zoom );
	
	Sound::Instance()->PlayBGM( E_BGM_LEVEL01, MM_PLAY_LOOP );
	
	while( State == INTERMISSION )
	{	
		FrameCounter--;
		if( FrameCounter < 0 )
		{
			SetState(STAGE_START);
		}
	
		Cam.Update( Mario.GetX(), Mario.GetY(), 0, 0, PausedCamLerp.x, PausedCamLerp.y, Map.Collision );
		Draw();
		
	}
	
	
}

void Game::StateStageReport()
{
	int BombFrame = 0;
	int MagicFrame = 0;
	
	int FrameCounter = 60 * 5;
	
	int Bonus = Mario.GetLives() * 1000;
	Bonus += Mario.GetBombsLeft() * 500;
	Bonus += Mario.GetShotsLeft() * 750;
	
	Mario.AddToScore(Bonus);
	
	while( State == STAGE_REPORT )
	{	
		scanKeys();
		KeyH = keysHeld();
		KeyD = keysDown();
		
		if(KeyD & KEY_START)
		{
			if( !Replay )
			{
				SetState(INTERMISSION);
				Mario.SaveReplay("level01.rep");
			}
			else
			{
				Mario.Init();
				SetState(TITLE);
			}
		}
		
		FrameCounter--;
		if( FrameCounter < 0 )
		{
			if( !Replay )
			{
				SetState(INTERMISSION);
				Mario.SaveReplay("level01.rep");
			}
			else
			{
				Mario.Init();
				SetState(TITLE);
			}
		}
	
		if( (FrameCounter & 3) == 0 )
		{
			BombFrame = ( BombFrame + 1 ) % 4;
			MagicFrame = ( MagicFrame + 1 ) % 8;
		}				
				
		GlobalManager::Instance()->ResetAll();
		// Start 2D mode
		glBegin2D();
	
			Fx3D::Instance()->WarpScreen( FrameCounter, &WaterTiles[0]);
			
			KromGLFont.PrintCentered( 0, 10, "STAGE REPORT" );
			glSprite( 30, 40, GL_FLIP_NONE, &PlayerSprites[15] );
			glSprite( 30, 80, GL_FLIP_NONE, &EnemySprites[1 + BombFrame] );
			glSprite( 30, 100, GL_FLIP_NONE, &EnemySprites[33 + MagicFrame] );
			
			KromGLFont.Print( 70, 48, "= ", Mario.GetLives() );
			KromGLFont.Print( 70, 88, "= ", Mario.GetBombsLeft() );
			KromGLFont.Print( 70, 108, "= ", Mario.GetShotsLeft() );
			
			KromGLFont.Print( 140, 48, "X ", 1000 );
			KromGLFont.Print( 140, 88, "X ", 500 );
			KromGLFont.Print( 140, 108, "X ", 750 );
			
			KromGLFont.Print( 30, 132, "BONUS : ", Bonus );
			
			KromGLFont.Print( 30, 172, "SCORE : ", Mario.GetScore() );
			
		glEnd2D();
		
		glFlush( 0 );

		swiWaitForVBlank();
		
	}
	
	StageDone = true;
	
	
}

void Game::StateGameOver()
{
	
	Sound::Instance()->PlayBGM( E_BGM_DEATH, MM_PLAY_ONCE );
	
	int FrameCounter = 0;
	Blinker = true;
	while( State == GAME_OVER )
	{	
		
		FrameCounter++;
		if( (FrameCounter & 31) == 0 ) Blinker = !Blinker;
	
		scanKeys();
		KeyH = keysHeld();
		KeyD = keysDown();
		
		if(KeyD & KEY_START)
		{
			SetState(TITLE);
		}
		
	
		Cam.Update( Mario.GetX(), Mario.GetY(), 0, 0, PausedCamLerp.x, PausedCamLerp.y, Map.Collision );
		Draw();
		
	}
	
	Mario.Init();
	Stage = 0;
	StageDone = true;
		
}

void Game::StateOptions()
{
	int FrameCounter = 0;
	
	const char *Menu[] =
	{ 
		"<A> - JUMP",
		"<B> - NORMAL SHORT RANGE ATTACK",
		"<B + DOWN> - PLACE BOMBS",
		"<X> - TERRAIN FOLLOWING MAGIC",
		"<X + DOWN> - GRAVITY MAGIC",
		"<START> - PAUSE",
		"    PAUSE MODE:",
		"    <ARROWS> - SURVEY THE LEVEL",
		"    <SHOULDER R> - ZOOM OUT",
		"<SELECT> - GIVE UP",
		"<L + R> - RETURN TO TITLE"
	};
	
	s32 interpolator = 0; 
	s32 xPos[11];

	
	for( int i = 0; i < 11; i++ )
	{
			xPos[i] = (256 + i * 50) << 12;
	}
	
	while( State == OPTIONS )
	{	
		scanKeys();
		KeyH = keysHeld();
		KeyD = keysDown();
		
		if( KeyD )
		{
			SetState(TITLE);
		}
		
		interpolator = UTIL::Clamp( interpolator + 8, 0, 1 << 12 );
		
		for( int i = 0; i < 11; i++ )
		{
			xPos[i] = UTIL::LerpSmoothf32( xPos[i], 0, interpolator );
		}
	
		FrameCounter++;
		GlobalManager::Instance()->ResetAll();
		
		// Start 2D mode
		glBegin2D();
	
			Fx3D::Instance()->WarpScreen( FrameCounter, &WaterTiles[0]);
			
			s32 cinterpolator = UTIL::Clamp( interpolator * 8, 0, 1 << 12 );
		
			KromGLFont.PrintCentered( 0, 
									  10 + (UTIL::LerpSmoothf32( 192 << 12, 0, cinterpolator ) >> 12),
									  "CONTROLS" );
			
			for( int i = 0; i < 11; i++ )
			{
				BubblaGLFont.Print( 4 + (xPos[i] >> 12), 50 + i * 10, Menu[i] );
			}
			
		glEnd2D();
		
		glFlush( 0 );

		swiWaitForVBlank();
		
	}
	
	
}

void Game::StateEnding()
{
	int FrameCounter = 0;
	
	const char *Menu[] =
	{ 
		"GRIPE WAS ABLE TO RESCUE",
		"ALL OF HIS FRIENDS FROM",
		"THE PARALLEL DIMENSION.",
		"",
		"HOPE TO ADD MORE LEVELS",
		"WHEN I UPDATE NEXT TIME.",
		"",
		"SEE YA..."
		
	};
	
	s32 interpolator = 0; 
	s32 yPosStart[8];
	s32 yPosEnd[8];
	
	
	for( int i = 0; i < 8; i++ )
	{
		yPosStart[i] = (192 + i * 50) << 12;
		yPosEnd[i] = (30 + i * 20) << 12;
 	}
	
	while( State == ENDING )
	{	
		scanKeys();
		KeyH = keysHeld();
		KeyD = keysDown();
		
		if( KeyD )
		{
			SetState(STAGE_REPORT);
		}
		
		interpolator = UTIL::Clamp( interpolator + 8, 0, 1 << 12 );
		
		for( int i = 0; i < 8; i++ )
		{
			yPosStart[i] = UTIL::LerpSmoothf32( yPosStart[i], yPosEnd[i], interpolator );
		}
	
		FrameCounter++;
		GlobalManager::Instance()->ResetAll();
		
		// Start 2D mode
		glBegin2D();
	
			Fx3D::Instance()->WarpScreen( FrameCounter, &WaterTiles[0]);
			
			s32 cinterpolator = UTIL::Clamp( interpolator * 8, 0, 1 << 12 );
		
			KromGLFont.PrintCentered( 0, 
									  10 + (UTIL::LerpSmoothf32( -192 << 12, 0, cinterpolator ) >> 12),
									  "CONGRATULATIONS" );
			
			for( int i = 0; i < 8; i++ )
			{
				BubblaGLFont.PrintCentered( 0, (yPosStart[i] >> 12), Menu[i] );
			}
			
		glEnd2D();
		
		glFlush( 0 );

		swiWaitForVBlank();
		
	}
	
	
}

void Game::StateExtras()
{
	/*
	int FrameCounter = 0;
	
	
	while( State == EXTRAS )
	{	
		scanKeys();
		KeyH = keysHeld();
		KeyD = keysDown();
		
		if( KeyD )
		{
			//SetState(TITLE);
			SetState(INTERMISSION);
		}
		
		FrameCounter++;
		GlobalManager::Instance()->ResetAll();
		
		// Start 2D mode
		glBegin2D();
	
			Fx3D::Instance()->WarpScreen( FrameCounter, &WaterTiles[0]);
			
			KromGLFont.PrintCentered( 0, 10, "EXTRAS" );
			
			KromGLFont.PrintCentered( 0, 60, "SORRY" );
			KromGLFont.PrintCentered( 0, 90, "NOT YET DONE" );
			KromGLFont.PrintCentered( 0, 110, "PRESS ANY KEY" );
			
		glEnd2D();
		
		glFlush( 0 );

		swiWaitForVBlank();
		
	}
	*/
	
	SetState(INTERMISSION);
	Replay = true;
	Mario.LoadReplay( demo_bin );
	Stage = 0;
	
}

void Game::StateCredits()
{
	
	#define MAX_TEXT 15
	
	struct TextPos
	{
		VectorSpring Pos;
		s32 x;
		s32 y;
		s32 x1;
		s32 x2;
		s32 Interp;
		s32 Speed;
		int Spacing;
	};
	
	
	const char *TextMenu1[MAX_TEXT] =
	{ 
		"-CODE::PRODUCTION-",
		"ANYA THERESE B. LOPE",
		"RELMINATOR/RICHARD ERIC M. LOPE",
		" ",
		"-GFX-",
		"MARC RUSSEL",
		"ADIGUN A. POLACK",
		"VARIOUS NET SOURCES",
		" ",	
		"-SFX/BGM-",
		"VGMUSIC",
		"VARIOUS NET SOURCES",
		" ",	
		"HTTP://REL.PHATCODE.NET",
		"HTTP://WWW.SPICYPIXEL.NET",
			
	};
	
	const char *TextMenu2[MAX_TEXT] =
	{ 
		"-LIBRARIES-",
		"EASY GL2D/3D",
		"LIBNDS",
		"MAXMOD",
		" ",
		"-GREETS-",
		"WWW.DEVKITPRO.ORG",
		"WWW.GBADEV.ORG",
		"WWW.FREEBASIC.NET",	
		"GBATEMP.COM",
		"SYMBIANIZE.COM",
		"PINOYDEN.COM",
		"NEOFLASH.COM",	
		"DBFINTERACTIVE.COM",
		"JAVA-GAMING.ORG",
			
	};
	
	const char *TextMenu3[MAX_TEXT] =
	{ 
		"-MOAR GREETZ-",
		"ROSE LOPE ",
		"DR.D",
		"JOFERS",
		"PLASMA357",
		"HELLFIRE",
		"WINTERMUTE",
		"VATOLOCO",
		"JURASSIC PLAYER",	
		"ZEROMUS",
		"DISCOSTEW",
		"ANOTHER WORLD",
		"SVERX",	
		"ELHOBBS",
		"SHOCKWAVE",
			
	};
	
	const char *TextMenu4[MAX_TEXT] =
	{ 
		"-MOAR GREETZ-",
		"DR.NEO",
		"0XBADC0DE",
		"MOTORHERP",
		"ZAMASTER",
		"COSTELLO",
		"FLASH",
		"FEROFAX",
		"MARIE CRISTINA ABEJUELA",	
		"CRISTINA MARIE SENOSA",
		"RICHARD ABEJUELA",
		"STANLEY SENOSA",
		"CJ SENOSA",	
		"DHEART",
		"GILAS PILIPINAS",
			
	};
	
	std::vector<const char**> Texts;
	Texts.push_back( TextMenu1 );
	Texts.push_back( TextMenu2 );
	Texts.push_back( TextMenu3 );
	Texts.push_back( TextMenu4 );
	
			
	TextPos TPos[MAX_TEXT];

	int TextIndex = 0;
	for( int i = 0; i < MAX_TEXT; i++ )
	{
		int Len = strlen(Texts[TextIndex][i]) * 8;
		TPos[i].x1 = 256 << 12;
		TPos[i].x2 = ( (SCREEN_WIDTH - Len) / 2 ) << 12;
		TPos[i].y = (40 + i * 10) << 12;
		TPos[i].Interp = 0;
		int Steps = 60 + (i * 15);
		TPos[i].Speed = 4096/Steps;
		TPos[i].Pos.SetOx(TPos[i].x);
		TPos[i].Pos.SetOy(TPos[i].y);
		TPos[i].Pos.SetParameters( 400, floattof32(3.5), floattof32(0.9), 5 );
	
	}

	const int START_EXIT_1 = 60 * 10;
	
	s32 interpolator = 0;
	
	int FrameCounter = 0;
	
	Sound::Instance()->PlayBGM( E_BGM_LOW_TIME, MM_PLAY_LOOP );
	
	while( State == CREDITS )
	{	
		scanKeys();
		KeyH = keysHeld();
		KeyD = keysDown();
		
		if( KeyD )
		{
			SetState(TITLE);
		}
		
		interpolator = UTIL::Clamp( interpolator + 64, 0, 1 << 12 );
		FrameCounter++;
		GlobalManager::Instance()->ResetAll();
		if( FrameCounter < START_EXIT_1 )
		{
				
			for( int i = 0; i < MAX_TEXT; i++ )
			{	
				TPos[i].Interp += TPos[i].Speed;
				if( TPos[i].Interp > 4096 )  TPos[i].Interp = 4096;
				TPos[i].x = UTIL::Lerpf32( TPos[i].x1, 
										   TPos[i].x2, 
										   TPos[i].Interp );
				TPos[i].Spacing = UTIL::Lerpf32( 7 << 12, 0 << 12, TPos[i].Interp );
				TPos[i].Pos.SetOx(TPos[i].x);
				TPos[i].Pos.SetOy(TPos[i].y);
				TPos[i].Pos.Update();
			}
	
		}
		else if( FrameCounter == START_EXIT_1 )
		{
			for( int i = 0; i < MAX_TEXT; i++ )
			{
				int Len = strlen(Texts[TextIndex][i]) * 8;
				TPos[i].x = TPos[i].x2;
				TPos[i].x1 = TPos[i].x2;
				TPos[i].x2 = ( -Len  ) << 12;
				TPos[i].Interp = 0;
				int Steps =  60 + (i * 15);
				TPos[i].Speed = 4096/Steps;
				TPos[i].Spacing = 0;
				TPos[i].Pos.SetOx(TPos[i].x);
				TPos[i].Pos.SetOy(TPos[i].y);
				
			}
	
		}
		else
		{
			for( int i = 0; i < MAX_TEXT; i++ )
			{	
				TPos[i].Interp += TPos[i].Speed;
				if( TPos[i].Interp > 4096 )  TPos[i].Interp = 4096;
				TPos[i].x = UTIL::Lerpf32( TPos[i].x1, 
										   TPos[i].x2, 
										   TPos[i].Interp );
				TPos[i].Pos.SetOx(TPos[i].x);
				TPos[i].Pos.SetOy(TPos[i].y);
				TPos[i].Pos.Update();
			}
		}
		
		if( FrameCounter == (START_EXIT_1 + 60 + (MAX_TEXT * 15) ) )
		{
			FrameCounter = 0;
			TextIndex = (TextIndex + 1) & 3;
			for( int i = 0; i < MAX_TEXT; i++ )
			{
				int Len = strlen(Texts[TextIndex][i]) * 8;
				TPos[i].x1 = 256 << 12;
				TPos[i].x2 = ( (SCREEN_WIDTH - Len) / 2 ) << 12;
				TPos[i].y = (40 + i * 10) << 12;
				TPos[i].Interp = 0;
				int Steps = 60 + (i * 15);
				TPos[i].Speed = 4096/Steps;
				TPos[i].Pos.SetOx(TPos[i].x);
				TPos[i].Pos.SetOy(TPos[i].y);
				TPos[i].Pos.SetParameters( 400, floattof32(3.5), floattof32(0.9), 5 );
			
			}
			
		}
		
		// Start 2D mode
		glBegin2D();
	
			Fx3D::Instance()->RubberScreen( FrameCounter, &WaterTiles[0]);
			
			KromGLFont.PrintCentered( 0, 
									  10 + (UTIL::LerpSmoothf32( -40 << 12, 10, interpolator ) >> 12),
									  "CREDITS" );
			
			
			for( int i = 0; i < MAX_TEXT; i++ )
			{
				BubblaGLFont.PrintSpaced( TPos[i].Pos.GetX() >> 12,
										  TPos[i].Pos.GetY() >> 12, 
										  Texts[TextIndex][i], 
										  0,
										  TPos[i].Spacing * 2,
										  0 );
			}
			
		glEnd2D();
		
		glFlush( 0 );

		swiWaitForVBlank();
		
	}
	
	
}

void Game::StateTitle()
{
	
	#define MAX_MENU 5
	
	struct MenuPos
	{
		s32 x;
		s32 y;
		s32 y1;
		s32 y2;
		s32 Interp;
		s32 Speed;
		int Spacing;
	};
	
	const char *Menu[] =
	{ 
		"START GAME",
		"CONTROLS",
		"RUN DEMO",
		"CREDITS",
		"EXIT GAME"
	};
	
	MenuPos MenPos[MAX_MENU];

	for( int i = 0; i < MAX_MENU; i++ )
	{
		MenPos[i].x = (84 - (10 + i * 10)) << 12;
		MenPos[i].y1 = -420 << 12;
		MenPos[i].y2 = (60 + i * 25) << 12;
		MenPos[i].Interp = 0;
		int Steps = 60 + (i * 10);
		MenPos[i].Speed = 4096/Steps;
	}
	
	
	VectorSpring Choice;
	Choice.SetParameters( 400, floattof32(1.5), floattof32(0.9), 15 );
	Choice.SetOx( 0 );
	Choice.SetOy( 0 );
		
	int FrameCounter = 0;
	int Active = 0;
	const int START_MENU = 60 + (MAX_MENU * 10);
	const int START_DEMO = 60 * 10;
	
	int BombFrame = 0;
	int Counter = 0;
	
	Replay = false;

	BubblaFont.Clear(); // deactivate every oam per frame
	KromFont.Clear(); // deactivate every oam per frame
	
	// Draw
	oamUpdate(&oamSub);
	
	DC_FlushRange( neo_badgeBitmap, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( neo_badgeBitmap, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( 0, 255, neo_badgePal );
	
	s32 BGInterp = 1 << 12;
	Fx2D::Instance()->Fade( BG_PALETTE_SUB,
							Palettes::Instance()->GetBlack(),
							Fx2D::Instance()->GetPlasmaPal(),
							BGInterp,
							16,
							512 );
	
	Sound::Instance()->PlayBGM( E_BGM_TITLE, MM_PLAY_LOOP );
	
	while( State == TITLE )
	{	
		scanKeys();
		KeyH = keysHeld();
		KeyD = keysDown();
		
		if( FrameCounter >= START_MENU ) 
		{
			
			if( FrameCounter == START_MENU ) Sound::Instance()->PlaySFX(E_SFX_MENU_SELECT);
				
			if( (KeyD & KEY_START) || (KeyD & KEY_A) )
			{
				Sound::Instance()->PlaySFX(E_SFX_MENU_OK);
				break;
			}
			
			if( (KeyD & KEY_UP) )
			{
				Active--;
				if( Active < 0 ) Active = (MAX_MENU - 1);
				Sound::Instance()->PlaySFX(E_SFX_MENU_SELECT);
				Counter = 0;
			}
			
			if( (KeyD & KEY_DOWN) )
			{
				Active++;
				if( Active > (MAX_MENU - 1) ) Active = 0;
				Sound::Instance()->PlaySFX(E_SFX_MENU_SELECT);
				Counter = 0;
			}
				
		}
		
		Choice.SetOx( MenPos[Active].x );
		Choice.SetOy( MenPos[Active].y2 );
		Choice.Update();
		
		FrameCounter++;
		Counter++;
		
		if( Counter > START_DEMO )
		{
			//State = INTERMISSION;
			Active = CHOICE_EXTRAS;
			break;
		}
	
		if( (FrameCounter & 31) == 0 ) Blinker = !Blinker;
		
		if( (FrameCounter & 3) == 0 )
		{
			BombFrame = ( BombFrame + 1 ) % 4;
		}				
		
		for( int i = 0; i < MAX_MENU; i++ )
		{	
			MenPos[i].Interp += MenPos[i].Speed;
			if( MenPos[i].Interp > 4096 )  MenPos[i].Interp = 4096;
			MenPos[i].y = UTIL::LerpSmoothf32( MenPos[i].y1, 
										 MenPos[i].y2, 
										 MenPos[i].Interp );
			MenPos[i].Spacing = UTIL::LerpSmoothf32( 7 << 12, 0 << 12, MenPos[i].Interp );
		}

		GlobalManager::Instance()->ResetAll();
		// Start 2D mode
		glBegin2D();
	
			glSprite( 0, 0, GL_FLIP_NONE, &TitleSprites[0] );
			
			for( int i = 0; i < 12; i++ )
			{
				s32 angle = UTIL::RND();
				s32 rx = cosLerp( angle ) * (UTIL::RND() & 7); 
				s32 ry = cosLerp( angle ) * (UTIL::RND() & 7);
				int xx = 56;
				int yy = 60;
				glLine( xx, yy, xx + (rx >> 12),  yy + (ry >> 12), 
						RGB15(  UTIL::RND() & 31,UTIL::RND() & 31,UTIL::RND() & 31 ) );
			}
			
			int col = abs(sinLerp( FrameCounter * 512 )) * 31;  
			
			glColor( RGB15(31,col >> 12,31));
			BubblaGLFont.Print( 168, 182, "V 1.0B 2013" );
			glColor( RGB15(31,31,31));
						
			for( int i = 0; i < MAX_MENU; i++ )
			{
				if( FrameCounter < START_MENU ) 
				{
				
					KromGLFont.PrintSpaced( MenPos[i].x >> 12,
											MenPos[i].y >> 12, 
											Menu[i], 
											0,
											MenPos[i].Spacing,
											MenPos[i].Spacing*2 );
				
				}
				else
				{
					if( i != Active )
					{
						KromGLFont.PrintSpaced( MenPos[i].x >> 12,
												MenPos[i].y >> 12, 
												Menu[i], 
												0,
												MenPos[i].Spacing,
												MenPos[i].Spacing*2 );
					}
					else
					{
						glColor( RGB15(31,col >> 12,31));
						KromGLFont.PrintSine( (MenPos[i].x + Choice.GetXv() * 2) >> 12,
											  (MenPos[i].y + Choice.GetYv()) >> 12, 
											   Menu[i], 
											   0,   // width offset
											   4,  // height
											   2,	// cycles
											   FrameCounter * 1024 ); // start_angle
						glColor( RGB15(31,31,31));
					}
				}
			}
			
			if( FrameCounter > START_MENU )
			{
				glSprite( (Choice.GetX() >> 12) - 20, Choice.GetY() >> 12, GL_FLIP_NONE, &BombSprites[BombFrame] );
			}
			
			
		glEnd2D();
		
		glFlush( 0 );

		swiWaitForVBlank();
		
	}
	
	
	//Sound::Instance()->StopBGM();
	
	switch( Active )
	{
		case CHOICE_START_GAME:
			SetState(INTERMISSION);
			break;
		case CHOICE_OPTIONS:
			SetState(OPTIONS);
			break;
		case CHOICE_EXTRAS:
			SetState(EXTRAS);
			break;
		case CHOICE_CREDITS:
			SetState(CREDITS);
			break;
		case CHOICE_EXIT:
			exit(0);
			break;
		default:
			SetState(INTERMISSION);
			break;
		
	}
	
	
	DC_FlushRange( titleBitmap, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( titleBitmap, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( 0, 255, titlePal );
	
	BGInterp = 1 << 12;
	Fx2D::Instance()->Fade( BG_PALETTE_SUB,
							Palettes::Instance()->GetBlack(),
							Fx2D::Instance()->GetPlasmaPal(),
							BGInterp,
							16,
							512 );
	
	
}

void Game::StateSplash()
{
	
	const int START_EXIT = 60 * 6;
	
	s32 BGInterp = 0;
	s32 RotationInterp = 0;
	s32 scale;
	s32 rotation;
	
	int FrameCounter = 0;
	
	while( State == SPLASH )
	{	
		if( FrameCounter >= START_EXIT )
		{
			SetState(TITLE);
		}
		
		
		BGInterp = UTIL::Clamp( BGInterp + 16, 0, 1 << 12 );;
		RotationInterp = UTIL::Clamp( RotationInterp + 32, 0, 1 << 12 );
		scale = UTIL::LerpSmoothf32( 0, 1 << 12, RotationInterp );
		rotation = UTIL::LerpSmoothf32( 50 << 12, 0, RotationInterp );
		s32 fadeAlpha = UTIL::LerpSmoothf32( 31 << 12, 0, BGInterp );
		
		FrameCounter++;
		GlobalManager::Instance()->ResetAll();
		
		// Start 2D mode
		glBegin2D();
			
			glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
		
			glBoxFilledGradient( 0, 0, SCREEN_WIDTH+1, SCREEN_HEIGHT/2,
								 RGB15(31,31,31), RGB15(0,31,0), RGB15(0,31,0), RGB15(31,31,31) );
			glBoxFilledGradient( 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT,
								 RGB15(0,31,0), RGB15(31,31,31), RGB15(31,31,31), RGB15(0,31,0) );
			
			glSpriteRotateScaleXY( SCREEN_WIDTH/2, (SCREEN_HEIGHT/2),
								   rotation, scale, scale,
								   GL_FLIP_NONE, &RelminatorTiles[0] );
			
			glSpriteRotateScaleXY( SCREEN_WIDTH/2, (SCREEN_HEIGHT/2),
								   -rotation, scale, scale,
								   GL_FLIP_NONE, &RelminatorTiles[0] );
			
			BubblaGLFont.PrintCentered( 0, 
									    (UTIL::LerpSmoothf32( 0, (192 - 10) << 12, BGInterp ) >> 12),
									    "HTTP://REL.PHATCODE.NET" );
			
			glPolyFmt( POLY_ALPHA( (fadeAlpha>>12) + 1) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
			
			glBoxFilled( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB15(0,0,0) );
			
		glEnd2D();
		
		glFlush( 0 );

		swiWaitForVBlank();
		
		Fx2D::Instance()->Fade( BG_PALETTE_SUB,
							Palettes::Instance()->GetBlack(),
							Fx2D::Instance()->GetPlasmaPal(),
							BGInterp,
							16,
							512 );
	
	
	}
	
}

void Game::SetState( int state )
{
	State = state;
	
	switch( State )
	{
		case PLAY:
			StateFunction = &Game::StatePlay;
			break;
		case REPLAY:
			StateFunction = &Game::StateRePlay;
			break;
		case PAUSE:
			StateFunction = &Game::StatePause;
			break;
		case STAGE_COMPLETE:
			StateFunction = &Game::StateStageComplete;
			break;
		case STAGE_START:
			StateFunction = &Game::StateStart;
			break;
		case PLAYER_DIE:
			StateFunction = &Game::StatePlayerDie;
			break;
		case INTERMISSION:
			StateFunction = &Game::StateIntermission;
			break;
		case STAGE_REPORT:
			StateFunction = &Game::StateStageReport;
			break;
		case GAME_OVER:
			StateFunction = &Game::StateGameOver;
			break;
		case OPTIONS:
			StateFunction = &Game::StateOptions;
			break;
		case EXTRAS:
			StateFunction = &Game::StateExtras;
			break;
		case CREDITS:
			StateFunction = &Game::StateCredits;
			break;
		case TITLE:
			StateFunction = &Game::StateTitle;
			break;
		case SPLASH:
			StateFunction = &Game::StateSplash;
			break;
		case ENDING:
			StateFunction = &Game::StateEnding;
			break;
		default:
			StateFunction = &Game::StatePlay;
			break;
	}
	
	
}


//--------------------------------------
//	
//--------------------------------------
void Game::DrawSubScreen()
{
	
	BubblaFont.Clear(); // deactivate every oam per frame
	KromFont.Clear(); // deactivate every oam per frame
	
	// Print stuff via OAM
	KromFont.Print( 20, 70, "SCORE:" );
	KromFont.PrintScore( 120, 70, Mario.GetScore(), "0", 7 );
	
	KromFont.Print( 20, 110, " TIME:" );
	KromFont.PrintScore( 120, 110, Timer, "0", 4 );
	
	int Lives = Mario.GetLives();
	if( (State == PLAYER_DIE) || (State == GAME_OVER) ) Lives++;
	KromFont.Print( 20, 150, " LEFT:" );
	KromFont.PrintScore( 120, 150, Lives, "0", 2 );
	
	BubblaFont.PrintCentered( 20, 182, "WWW.NEOFLASH.COM" );
	
	// Draw
	oamUpdate(&oamSub);
	
	
	
}

void Game::DrawDebug()
{
	
	/*
	iprintf( "\x1b[1;1HBlocks            = %i        ",Blocks.GetNumEntities() );
	iprintf( "\x1b[2;1HSpirals           = %i        ",Spirals.GetNumEntities() );
	iprintf( "\x1b[3;1HDiagSpirals       = %i        ",DiagSpirals.GetNumEntities() );
	iprintf( "\x1b[4;1HGreenTurtles      = %i        ",GreenTurtles.GetNumEntities() );
	iprintf( "\x1b[5;1HRedTurtles        = %i        ",RedTurtles.GetNumEntities() );
	iprintf( "\x1b[6;1HBouncers          = %i        ",Bouncers.GetNumEntities() );
	iprintf( "\x1b[7;1HJumpers           = %i        ",Jumpers.GetNumEntities() );
	iprintf( "\x1b[8;1HMagnetos          = %i        ",Magnetos.GetNumEntities() );
	
	iprintf( "\x1b[10;1HShotsLeft        = %i        ",Mario.GetShotsLeft() );
	iprintf( "\x1b[11;1HBombsLeft        = %i        ",Mario.GetBombsLeft() );
	
	iprintf( "\x1b[12;1HNumMaps          = %i        ",TextMap::Maps.size() );
	iprintf( "\x1b[13;1HExitPost         = %i        ",ExitPosts.IsOpen() );

	iprintf("\x1b[14;1HMemUsed = %i         ", Memory.GetMemUsed()/1024);
	iprintf("\x1b[15;1HMemFree = %i         ", Memory.GetMemFree()/1024);
	
	*/
}

void Game::RestartLevel( const char *TextMap[] )
{

	UTIL::ResetRND();
		
	std::vector<EntityFactory*>::iterator iter;
	for( iter = Enemies.begin(); iter != Enemies.end(); ++iter )
	{
		(*iter)->KillAllEntities();
	}

	Particle::Instance()->KillAll();
	Explosion::Instance()->KillEntities();
	
	LoadMap( TextMap );
	ExitPosts.SetOpen(false);
	
	Snipes.SetSnipeKilled(false);
	Snipes.SetAllRescued(false);
	Frame = 0;
	Timer = MAX_TIMER;
	
}
	
void Game::InitHardware()
{
	
	videoSetMode( MODE_5_3D );
	videoSetModeSub( MODE_5_2D );
	
	
	// initialize gl2d
	glScreen2D();
	
	
	vramSetBankA( VRAM_A_TEXTURE );     
	vramSetBankB( VRAM_B_TEXTURE );     
	vramSetBankC( VRAM_C_TEXTURE );     
	vramSetBankD( VRAM_D_SUB_SPRITE );	// oam fonts et al
	
	vramSetBankE( VRAM_E_TEX_PALETTE );		// 64kb
	
	vramSetBankH( VRAM_H_SUB_BG );		// Our sub BG  
	vramSetBankI( VRAM_I_SUB_BG_0x06208000 ); // H + I makes 256*192
	
	// File
	//fatInitDefault();					// buggzooors here! why oh why?!  Sounds don't work when I enable fat
	
	
	// Sound
	Sound::Instance()->Init( (mm_addr)soundbank_bin );
	Sound::Instance()->SetMode( MM_MODE_A );
	Sound::Instance()->SetMasterVolume( 512 );
	
	// consoleDemoInit();
	// Bg
	SubBG3 = bgInitSub( 3, BgType_Bmp8, BgSize_B8_256x256, 0, 0 );
	
	// Oam
	oamInit(&oamSub, SpriteMapping_1D_128, false);
	
	
}

void Game::InitEverything()
{
	
	SetState(SPLASH);
	
	UTIL::InitRND( 42 );
	
	GlobalManager::Instance()->Init();
	
	CameraMode = 2;
	Cam.SetMode( CameraMode );
	Mario.Init();
	
	glEnable( GL_TEXTURE_2D );

	Blinker = false;
	Replay = false;
	
}


void Game::InitEntities()
{
	
	Blocks.Init( EnemySprites );
	VanishPlatforms.Init( EnemySprites );
	Spirals.Init( EnemySprites );
	DiagSpirals.Init( EnemySprites );
	GreenTurtles.Init( EnemySprites );
	RedTurtles.Init( EnemySprites );
	Bouncers.Init( EnemySprites );
	Jumpers.Init( EnemySprites );
	Magnetos.Init( EnemySprites );
	Fires.Init( EnemySprites );
	
	Snipes.Init( EnemySprites );
	Coins.Init( EnemySprites );
	
	ExitPosts.Init( EnemySprites );
	BombUps.Init( EnemySprites );
	MagicUps.Init( EnemySprites );
	
	Enemies.push_back( &Blocks );
	Enemies.push_back( &VanishPlatforms );
	Enemies.push_back( &Spirals );
	Enemies.push_back( &DiagSpirals );
	Enemies.push_back( &GreenTurtles );
	Enemies.push_back( &RedTurtles );
	Enemies.push_back( &Bouncers );
	Enemies.push_back( &Jumpers );
	Enemies.push_back( &Magnetos );
	Enemies.push_back( &Fires );
	
	Enemies.push_back( &Snipes );
	Enemies.push_back( &Coins );
	
	Enemies.push_back( &ExitPosts );
	Enemies.push_back( &BombUps );
	Enemies.push_back( &MagicUps );

	Snipes.SetSnipeKilled(false);

}
	
void Game::LoadImages()
{
	
	// Oam Fonts
	KromFont.Init(&oamSub, (u8*)font_kromTiles, 16, 16, SpriteSize_16x16, 64, 64);
	BubblaFont.Init(&oamSub, (u8*)font_bubblaTiles, 8, 8, SpriteSize_8x8, 64, 0);
	BubblaFont.LoadPalette( font_bubblaPal, SPRITE_PALETTE_SUB, 512 );
	
	TitleTextureID = 
	glLoadTileSet( TitleSprites,				// pointer to glImage array
				   256,					// sprite width
				   192,					// sprite height
				   256,					// bitmap width
				   256,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_256,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_256,		// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF, // param for glTexImage2D() in videoGL.h
				   titlePalLen/2,		// Length of the palette to use (256 colors)
				   (u16*)titlePal,		// Load our 256 color tiles palette
				   (u8*)titleBitmap);	// image data generated by GRIT
	
	RelminatorTextureID = 
	glLoadTileSet( RelminatorTiles,				// pointer to glImage array
				   256,					// sprite width
				   32,					// sprite height
				   256,					// bitmap width
				   32,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_256,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_32,		// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   relminatorPalLen/2,		// Length of the palette to use (256 colors)
				   (u16*)relminatorPal,		// Load our 256 color tiles palette
				   (u8*)relminatorBitmap);	// image data generated by GRIT
	
	TilesTextureID = 
	glLoadTileSet( Tiles,				// pointer to glImage array
				   32,					// sprite width
				   32,					// sprite height
				   256,					// bitmap width
				   64,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_256,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_64,		// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   level01_tilesPalLen/2,		// Length of the palette to use (256 colors)
				   (u16*)level01_tilesPal,		// Load our 256 color tiles palette
				   (u8*)level01_tilesBitmap);	// image data generated by GRIT
				 
	WaterTilesTextureID = 
	glLoadTileSet( WaterTiles,				// pointer to glImage array
				   32,					// sprite width
				   32,					// sprite height
				   64,					// bitmap width
				   64,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_64,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_64,		// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF, // param for glTexImage2D() in videoGL.h
				   waterPalLen/2,		// Length of the palette to use (256 colors)
				   (u16*)waterPal,		// Load our 256 color tiles palette
				   (u8*)waterBitmap);	// image data generated by GRIT
				 
	
	BGTilesTextureID = 
	glLoadTileSet( BGTiles,				// pointer to glImage array
				   256,					// sprite width
				   256,					// sprite height
				   256,					// bitmap width
				   256,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_256,		// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_256,		// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF, // param for glTexImage2D() in videoGL.h
				   bgPalLen/2,		// Length of the palette to use (256 colors)
				   (u16*)bgPal,		// Load our 256 color tiles palette
				   (u8*)bgBitmap);	// image data generated by GRIT
	
	PlayerSpritesTextureID = 
	glLoadTileSet( PlayerSprites,		// pointer to glImage array
				   32,					// sprite width
				   32,					// sprite height
				   256,					// bitmap width
				   128,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_256,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   gripePalLen/2,			// Length of the palette to use (256 colors)
				   (u16*)gripePal,		// Load our 256 color tiles palette
				   (u8*)gripeBitmap);	// image data generated by GRIT
	
	FairySpritesTextureID = 
	glLoadTileSet( FairySprites,		// pointer to glImage array
				   32,					// sprite width
				   32,					// sprite height
				   64,					// bitmap width
				   64,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_64,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_64,		// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   fairy_spritePalLen/2,			// Length of the palette to use (256 colors)
				   (u16*)fairy_spritePal,		// Load our 256 color tiles palette
				   (u8*)fairy_spriteBitmap);	// image data generated by GRIT
	
	EnemySpritesTextureID = 
	glLoadSpriteSet( EnemySprites,				// pointer to glImage array
					 ENEMIES_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 enemies_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_256,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_256,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,					// Length of the palette to use (256 colors)
					 (u16*)enemies_spritePal,		// Load our 256 color enemies palette
					 (u8*)enemies_spriteBitmap	 	// image data generated by GRIT
				   );
	
	
	BombSpritesTextureID = 
	glLoadTileSet( BombSprites,		// pointer to glImage array
				   16,					// sprite width
				   16,					// sprite height
				   64,					// bitmap width
				   16,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_64,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_16,		// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   bomb_spritePalLen/2,			// Length of the palette to use (256 colors)
				   (u16*)bomb_spritePal,		// Load our 256 color tiles palette
				   (u8*)bomb_spriteBitmap);	// image data generated by GRIT
	
	
	BulletSpritesTextureID = 
	glLoadTileSet( BulletSprites,		// pointer to glImage array
				   32,					// sprite width
				   32,					// sprite height
				   512,					// bitmap width
				   32,					// bitmap height
				   GL_RGB256,			// texture type for glTexImage2D() in videoGL.h 
				   TEXTURE_SIZE_512,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_32,		// sizeY for glTexImage2D() in videoGL.h
				   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
				   bullet_spritesPalLen/2,			// Length of the palette to use (256 colors)
				   (u16*)bullet_spritesPal,		// Load our 256 color tiles palette
				   (u8*)bullet_spritesBitmap);	// image data generated by GRIT
	
	
	ExplosionSpritesTextureID = 
	glLoadSpriteSet( ExplosionsImages,				// pointer to glImage array
					 EXPLOSIONS_SPRITE_NUM_IMAGES, 	// Texture packer auto-generated #define
					 explosions_sprite_texcoords,		// Texture packer auto-generated array
					 GL_RGB256,				// texture type for glTexImage2D() in videoGL.h 
					 TEXTURE_SIZE_256,		// sizeX for glTexImage2D() in videoGL.h
					 TEXTURE_SIZE_128,		// sizeY for glTexImage2D() in videoGL.h
					 TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
					 256,
					 waterPal,
					 (u8*)explosions_spriteBitmap	 	// image data generated by GRIT
				   );
	
	FlaresSpritesTextureID = 
	glLoadTileSet( FlaresImages,		// pointer to glImage array
				   32,				// sprite width
				   32,				// sprite height
				   32,				// bitmap image width
				   128,				// bitmap image height
				   GL_RGB256,		// texture type for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_32,	// sizeX for glTexImage2D() in videoGL.h
				   TEXTURE_SIZE_128,	// sizeY for glTexImage2D() in videoGL.h
				   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
				   256,
				   (u16*)flares_spritePal,
				   (u8*)flares_spriteBitmap // image data generated by GRIT
				 );
	
	//Fonts
	KromGLFont.Load( KromGLFontImages,
			   16, 
			   16,
			   16,
			   1024,
			   GL_RGB16,
			   TEXTURE_SIZE_16,
			   TEXTURE_SIZE_1024,
			   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
			   (u8*)glfont_kromBitmap,
			   16,
			   glfont_kromPal );
	
	BubblaGLFont.Load( BubblaGLFontImages,
			   8, 
			   8,
			   8,
			   512,
			   GL_RGB16,
			   TEXTURE_SIZE_8,
			   TEXTURE_SIZE_512,
			   TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
			   (u8*)glfont_bubblaBitmap,
			   16,
			   glfont_bubblaPal );
	
	
	
	Particle::Instance()->Init( FlaresImages );
	
	Explosion::Instance()->Init(0, ExplosionsImages );
	
	
	// BG
	DC_FlushRange( neo_badgeBitmap, SCREEN_WIDTH * SCREEN_HEIGHT );
	dmaCopy( neo_badgeBitmap, bgGetGfxPtr(SubBG3), SCREEN_WIDTH * SCREEN_HEIGHT );
	Fx2D::Instance()->LoadPal( 0, 255, neo_badgePal );
	
	s32 BGInterp = 1 << 12;
	Fx2D::Instance()->Fade( BG_PALETTE_SUB,
							Palettes::Instance()->GetBlack(),
							Fx2D::Instance()->GetPlasmaPal(),
							BGInterp,
							16,
							512 );
	
}

void Game::LoadSounds()
{
	
	// BGMs
	Sound::Instance()->LoadBGM( MOD_LEVEL01 );
	Sound::Instance()->LoadBGM( MOD_LEVEL_COMPLETE );
	Sound::Instance()->LoadBGM( MOD_LOW_TIME );
	Sound::Instance()->LoadBGM( MOD_TITLE );
	Sound::Instance()->LoadBGM( MOD_DEATH );
	
	// SFXs
	Sound::Instance()->LoadSFX( SFX_JUMP );
	Sound::Instance()->LoadSFX( SFX_BOUNCE );
	Sound::Instance()->LoadSFX( SFX_HURT );
	Sound::Instance()->LoadSFX( SFX_ATTACK );
	Sound::Instance()->LoadSFX( SFX_ATTACK_HIT );
	Sound::Instance()->LoadSFX( SFX_MAGIC );
	Sound::Instance()->LoadSFX( SFX_MAGIC_ACTIVE );
	Sound::Instance()->LoadSFX( SFX_BLOCK_HIT_FLOOR );
	Sound::Instance()->LoadSFX( SFX_BLOCK_MOVE );
	Sound::Instance()->LoadSFX( SFX_EXPLODE );
	Sound::Instance()->LoadSFX( SFX_POWER_UP );
	Sound::Instance()->LoadSFX( SFX_SNIPE_UP );
	Sound::Instance()->LoadSFX( SFX_COIN_UP );
	Sound::Instance()->LoadSFX( SFX_SNIPE_COMPLETE );
	Sound::Instance()->LoadSFX( SFX_WATER_SPLASH );
	Sound::Instance()->LoadSFX( SFX_MENU_OK );
	Sound::Instance()->LoadSFX( SFX_MENU_SELECT );
	Sound::Instance()->LoadSFX( SFX_YAHOO );
	
}

void Game::LoadMap( const char *TextMap[] )
{
		
	Map.Base.Clear();
	Map.Collision.Clear();
	
	Map.Base.Convert( TextMap );
	Map.Collision.Convert( TextMap );
	SpawnEnemies( TextMap );
	
}
	
void Game::SpawnEnemies( const char *TextMap[] )
{

	int Width = strlen( TextMap[0] );
	int Height = GetTextMapHeight( TextMap );
	
	//H = Horizontal Spiral
	//V = Vertical Spiral
	//R = Red Turtle
	//G = Green Turtle
	//B = Bouncer
	
	for( int y = 0; y < Height; y++ )
	{
		for( int x = 0; x < Width; x++ )
		{
			
			char a = Mid( TextMap[y], x );
			switch( a )
			{
				case '>':
					Mario.ReSpawn( x * TILE_SIZE_3D, y * TILE_SIZE_3D, RIGHT );
					break;
				case '<':
					Mario.ReSpawn( x * TILE_SIZE_3D, y * TILE_SIZE_3D, LEFT );
					break;
				case 'X':
					ExitPosts.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D );
					break;
				case '@':
					BombUps.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D );
					break;
				case '&':
					MagicUps.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D );
					break;
				case 'H':
					Spirals.Spawn( x* TILE_SIZE_3D,  y * TILE_SIZE_3D, 1 );
					break;
				case 'V':
					Spirals.Spawn( x* TILE_SIZE_3D,  y * TILE_SIZE_3D,  0 );
					break;
				case 'D':
					DiagSpirals.Spawn( x* TILE_SIZE_3D,  y * TILE_SIZE_3D, UTIL::RND() % 4 );
					break;
				case 'R':
					RedTurtles.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D, 1 );
					break;
				case 'G':
					GreenTurtles.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D, 1 );
					break;
				case 'B':
					Bouncers.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D, -1 );
					break;
				case 'J':
					Jumpers.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D, -1 );
					break;
				case 'M':
					Magnetos.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D,  1 );
					break;
				case 'm':
					Magnetos.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D,  -1 );
					break;
				case 'F':
					Fires.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D,  -1 );
					break;
				case 'S':
					Snipes.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D );
					break;
				case 'C':
					Coins.Spawn( x * TILE_SIZE_3D + TILE_SIZE_3D/4 ,  y * TILE_SIZE_3D + TILE_SIZE_3D/4);
					break;
				case '0':
					Blocks.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D, Block::SOFT, 1 );
					break;
				case '9':
					Blocks.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D, Block::HARD, 1 );
					break;
				case '7':
					VanishPlatforms.Spawn( x * TILE_SIZE_3D,  y * TILE_SIZE_3D );
					break;
			}
			
		}



}
	



}
	