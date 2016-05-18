/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <maxmod9.h>

#include <stdio.h>

#include "gl2d.h"
#include "Vector2df32.h"
#include "UTIL.h"
#include "Sound.h"
#include "Button.h"

#include "GlobalConstants.h"
#include "Map2D.h"
#include "Renderer3D.h"
#include "AABB.h"
#include "Explosion.h"
#include "Particles.h"
#include "Shot.h"
#include "Bomb.h"
#include "Option.h"


#ifndef PLAYER_H
#define PLAYER_H

#define  MAX_PLAYER_BULLETS 1
#define  MAX_PLAYER_BOMBS 8
#define  MAX_PLAYER_SHADOWS 8

class Player 
{
public:
	Player();
	~Player();
	
	void Init();
	void ReSpawn( s32 sx, s32 sy, int dir );
	void SnapX( s32 ix );
	int GetTile( s32 ix, s32 iy, Map2D &Map );
	bool IsOnLadder( s32 ix, s32 iy, Map2D &Map );
	bool IsOnTopLadder( s32 ix, s32 iy, Map2D &Map );
	bool IsOnWater( s32 ix, s32 iy, Map2D &Map );
	bool IsOnSpike( s32 ix, s32 iy, Map2D &Map );
	bool CollideTopLadders( s32 ix, s32 iy, int &TileY, Map2D &Map );
	int CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map );
	int CollideWalls( s32 ix, s32 iy, int &TileX, Map2D &Map );
	void CollideOnMap( Map2D &Map );
	void CollideOnMap( int key, Map2D &Map );
	void DoStuff( int key, int keyD, MapContainer &Map );
	void Update( int key, int keyD, MapContainer &Map );
	void CollideOnMapReplay( Map2D &Map );
	void DoStuffReplay( MapContainer &Map );
	void UpdateReplay( MapContainer &Map );
	void HitAnimation();
	void Draw( const glImage *spr );
	void DrawAABB( s32 z, int Color );
	void UpdateShots( Map2D &Map );	
	void DrawShots( const glImage *spr ) const;
	void UpdateBombs( MapContainer &Map );
	void DrawBombs( const glImage *spr ) const;
	void DrawFairy( const glImage *spr ) const;
	int CollideShots( AABB &Aabb );
	int CollideBombs( AABB &Aabb );
	int CollideBombsBlocks( AABB &Aabb, Map2D &Map  );
	bool CollideAttack( AABB &Aabb );
	bool CollideAttackBlocks( AABB &Aabb );
	void Die();
	
	void SaveReplay( const char *FileName )
	{
		Keys.Save( FileName );
	}
	
	void LoadReplay( const char *FileName )
	{
		Keys.Load( FileName );
	}
	
	void LoadReplay( const u8 *BinFile )
	{
		Keys.Load( BinFile );
	}
	
	s32 GetX() const 
	{
		return x;
	}

	s32 GetY() const 
	{
		return y;
	}

	s32 GetCX() const 
	{
		return cx;
	}

	s32 GetCY() const 
	{
		return cy;
	}

	s32 GetXV() const 
	{
		return xv;
	}

	s32 GetYV() const 
	{
		return yv;
	}

	s32 GetWidth() const 
	{
		return Width;
	}

	s32 GetHeight() const 
	{
		return Height;
	}

	bool IsDead() const 
	{
		return Dead;
	}
	
	bool IsInvincible() const 
	{
		return Invincible;
	}
	
	bool IsClimbing() const 
	{
		return Climbing;
	}
	
	bool IsFloored() const
	{
		return Floored;
	}
	
	bool IsAttacking() const
	{
		return Attacking;
	}
	
	bool InExit() const
	{
		return InExitPost;
	}
	
	bool GetAttackHits() const 
	{
		return AttackHits;
	}
	
	bool GetCanHit() const 
	{
		return CanHit;
	}
	
	int GetLooking() const 
	{
		return Looking;
	}
	
	int GetScore() const 
	{
		return Score;
	}
	
	int GetTileX() const 
	{
		return x / TILE_SIZE_3D;
	}

	int GetTileY() const 
	{
		return y / TILE_SIZE_3D;
	}

	const AABB &GetAABB() const 
	{
		return aabb;
	}

	const AABB &GetAABB2() const 
	{
		return aabb2;
	}

	int GetExp() const 
	{
		return Exp;
	}
	
	int GetLevel() const 
	{
		return Level;
	}
	
	int GetNextExp() const 
	{
		return NextExp;
	}
	
	int GetShotID() const 
	{
		return ShotID;
	}
	
	int GetDirection() const 
	{
		return Direction;
	}
	
	int GetPushCounter() const 
	{
		return PushCounter;
	}
	
	
	int GetKeyD() const 
	{
		return KeyD;
	}
	
	int GetKeyH() const 
	{
		return KeyH;
	}
	
	
	int GetBombsLeft() const 
	{
		return BombsLeft;
	}
	
	int GetShotsLeft() const 
	{
		return ShotsLeft;
	}
	
	int GetLives() const 
	{
		return Lives;
	}
	
	void AddLives(int v) 
	{	
		Lives += v;
	}
	
	void SetExp(int Exp) 
	{	
		this->Exp = Exp;
	}
	
	void SetLevel(int Level) 
	{	
		this->Level = Level;
	}
	
	void SetNextExp(int NextExp) 
	{
		this->NextExp = NextExp;
	}
	
	void SetShotID(int ShotID) 
	{
		this->ShotID = ShotID;
	}
	
	void SetX( s32 v ) 
	{
		x = v;
	}

	void SetY( s32 v ) 
	{
		y = v;
	}

	void SetXV( s32 v ) 
	{
		xv = v;
	}

	void SetYV( s32 v ) 
	{
		yv = v;
	}

	void SetWidth( s32 v ) 
	{
		Width = v;
	}

	void SetHeight( s32 v ) 
	{
		Height = v;
	}

	void SetInvincible( bool v ) 
	{
		Invincible = v;
	}

	void SetJumping(bool Jumping) 
	{
		this->Jumping = Jumping;
	}
	
	void SetFalling(bool Falling) 
	{
		this->Falling = Falling;
	}
	
	void SetClimbing(bool Climbing) 
	{
		this->Climbing = Climbing;
	}
	
	void SetCanJump(bool CanJump) 
	{
		this->CanJump = CanJump;
	}
	
	void SetPushCounter(int PushCounter) 
	{
		this->PushCounter = PushCounter;
	}
	
	void DecPushCounter() 
	{
		PushCounter--;
	}
	
	void SetKeys(int kh, int kd ) 
	{
		KeyH = kh;
		KeyD = kd;
	}
	
	void SetBombsLeft( int v ) 
	{
		BombsLeft = v;
	}
	
	void IncBombsLeft() 
	{
		if( BombsLeft < 8 ) BombsLeft++;
	}
	
	void DecBombsLeft() 
	{
		if( BombsLeft > 0 ) BombsLeft--;
	}
	
	void SetShotsLeft( int v ) 
	{
		ShotsLeft = v;
	}
	
	void IncShotsLeft() 
	{
		if( ShotsLeft < 8 ) ShotsLeft++;
	}
	
	void DecShotsLeft() 
	{
		if( ShotsLeft > 0 ) ShotsLeft--;
	}
	
	void SetInExitPost( bool v )
	{
		InExitPost = v;
	}
	
	void AddToScore( int v ) 
	{
		Score += v;
	}
	
private:

	void GetAnimationFrames();
	void Animate();
	
	void SpawnShot();
	void SpawnBomb();
	
	s32 x;
	s32 y;
	s32 cx;
	s32 cy;
	s32 speed;
	s32 xv;
	s32 yv;
	s32 Width;
	s32 Height;
	
	bool Dead;
	int Lives;
	int Score;
	int Level;
	int Exp;
	int NextExp;
	int ShotID;
	
	s32 HitXv;
	int Direction;
	
	int Counter;
	int Flipmode;
	int SpriteFrame;
	int BaseFrame;
	int MaxFrame;
	int StationaryCooldown;

	bool Floored;
	bool CanJump;
	bool Jumping;
	bool Falling;    // is only true when pressing A while climbing
	bool Climbing;
	bool Attacking;
	bool AttackHits;
	bool CanHit;
	bool SettingBomb;
	int Looking;
	int PushCounter;
	bool Invincible;
	int InvincibleCoolDown;
	int AttackCoolDown;
	int CanPlantBomb;
	int NumBombs;
	
	int BombsLeft;
	int ShotsLeft;
	
	AABB aabb;
	AABB aabb2;
	AABB aabbAttack;
	
	int ShotSpawnDelay;
	int MaxShotSpawnDelay;
	int NumShots;
	
	
	int KeyH;
	int KeyD;
	
	Button Keys;
	u8 KeyPressed;
	int KeyMap;
	
	bool InExitPost;
	
	//Option	Fairy;
	
	Shot Shots[MAX_PLAYER_BULLETS];
	Bomb Bombs[MAX_PLAYER_BOMBS];

	Part Shadows[MAX_SHADOWS];
	
};

#endif // PLAYER_H
