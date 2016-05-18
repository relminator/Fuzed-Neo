/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Player.h"


#define MAX_COOLDOWN 180
#define MAX_INVCOOLDOWN 60
#define MAX_ATKCOOLDOWN 15
#define SHOT_SPAWN_DELAY 1

Player::Player()
{
	
	x = TILE_SIZE_3D * 10; 
	y = TILE_SIZE_3D * 3;
	speed = floattof32(0.05);
	xv = 0;
	yv = 0;
	
	
	Width = (24 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (25 * TILE_SIZE_3D) / TILE_SIZE;   // so that we don't hit ceiling while walking
	
	Dead = false;
	Lives = 3;
	Score = 0;
	Level = 0;
	Exp = 0;
	NextExp = 1000;
	ShotID = Shot::NORMAL;
	
	HitXv = 0;
	Direction = RIGHT;
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	SpriteFrame = 0;
	BaseFrame = 0;
	MaxFrame = 4;
	StationaryCooldown = MAX_COOLDOWN;
	
	Floored = false;
	CanJump = false;
	Jumping = false;
	Falling = false;
	Climbing = false;
	Jumping = false;
	Attacking = false;
	AttackHits = false;
	CanHit = false;
	SettingBomb = false;
	CanPlantBomb = false;
	NumBombs = 0;
	
	PushCounter = 60;
	
	Invincible = false;
	InvincibleCoolDown = MAX_INVCOOLDOWN;
	AttackCoolDown = 0;
	
	ShotSpawnDelay = 0;
	MaxShotSpawnDelay = SHOT_SPAWN_DELAY;
	
	Looking = 0;
	
	KeyH = 0;
	KeyD = 0;
	
	BombsLeft = 0;
	ShotsLeft = 0;
	InExitPost = false;
	
	KeyPressed = 0;
	Keys.Reset();
	Keys.Clear();
	
}

Player::~Player()
{
}


void Player::Init()
{
	
	x = -100 << 12;
	y = -100 << 12;
	speed = floattof32(0.05);
	xv = 0;
	yv = 0;
	
	
	Lives = 2;
	Dead = false;
	Level = 0;
	Score = 0;
	Exp = 0;
	NextExp = 1000;
	ShotID = Shot::MAGNETO;
	
	HitXv = 0;
	Direction = LEFT;
	
	Counter = 0;
	Flipmode = GL_FLIP_H;
	SpriteFrame = 0;
	BaseFrame = 0;
	MaxFrame = 4;
	StationaryCooldown = MAX_COOLDOWN;
	
	Floored = false;
	CanJump = false;
	Jumping = false;
	Falling = false;
	Climbing = false;
	Jumping = false;
	Attacking = false;
	AttackHits = false;
	CanHit = false;
	SettingBomb = false;
	CanPlantBomb = false;
	NumBombs = 0;
	
	PushCounter = 60;
	
	Invincible = false;
	InvincibleCoolDown = MAX_INVCOOLDOWN;
	AttackCoolDown = 0;
	
	ShotSpawnDelay = 0;
	MaxShotSpawnDelay = SHOT_SPAWN_DELAY;
	
	Looking = 0;
	
	KeyH = 0;
	KeyD = 0;
	
	BombsLeft = 0;
	ShotsLeft = 0;
	InExitPost = false;
	
	
	for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if( Shots[i].IsActive() )
		{
			Shots[i].Kill();
		}
	}
	
	for( int i = 0; i < MAX_PLAYER_BOMBS; i++)
	{
		if( Bombs[i].IsActive() )
		{
			Bombs[i].Kill();
		}
	}
	
	aabb.Init( x, y, Width, Height );
	aabb2.Init( x, y, Width, Height );
	aabb2.Resize( Width/4, Height/4 );
	aabbAttack.Init( x + (Direction * Width), y + Height/4, Width, Height - Height/2);
	
	KeyPressed = 0;
	Keys.Reset();
	
}


void Player::ReSpawn( s32 sx, s32 sy, int dir )
{
	
	x = sx; 
	y = sy;
	speed = floattof32(0.05);
	xv = 0;
	yv = 0;
	
	
	Dead = false;
	ShotID = Shot::MAGNETO;
	
	HitXv = 0;
	Direction = dir;
	Flipmode = GL_FLIP_NONE;
	
	if( Direction < 0 ) Flipmode = GL_FLIP_H;
	
	Counter = 0;
	SpriteFrame = 0;
	
	BaseFrame = 8;
	MaxFrame = 4;
	
	StationaryCooldown = MAX_COOLDOWN;
	
	Floored = false;
	CanJump = false;
	Jumping = false;
	Falling = false;
	Climbing = false;
	Jumping = false;
	Attacking = false;
	AttackHits = false;
	CanHit = false;
	SettingBomb = false;
	CanPlantBomb = false;
	NumBombs = 0;
	
	PushCounter = 60;
	
	Invincible = false;
	InvincibleCoolDown = MAX_INVCOOLDOWN;
	AttackCoolDown = 0;
	
	ShotSpawnDelay = 0;
	MaxShotSpawnDelay = SHOT_SPAWN_DELAY;
	
	Looking = 0;
	
	KeyH = 0;
	KeyD = 0;
	
	BombsLeft = 0;
	ShotsLeft = 0;
	InExitPost = false;
	
	
	for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if( Shots[i].IsActive() )
		{
			Shots[i].Kill();
		}
	}
	
	for( int i = 0; i < MAX_PLAYER_BOMBS; i++)
	{
		if( Bombs[i].IsActive() )
		{
			Bombs[i].Kill();
		}
	}
	
	aabb.Init( x, y, Width, Height );
	aabb2.Init( x, y, Width, Height );
	aabb2.Resize( Width/4, Height/4 );
	aabbAttack.Init( x + (Direction * Width), y + Height/4, Width, Height - Height/2);
	
	KeyPressed = 0;
	Keys.Reset();
	
}

void Player::SnapX( s32 ix )
{
	int offset = (TILE_SIZE_3D - Width)/2;
	ix = ix / TILE_SIZE_3D;
	x = (ix * TILE_SIZE_3D) + offset;
}

int Player::GetTile( s32 ix, s32 iy, Map2D &Map )
{
	int TileX = ix / TILE_SIZE_3D;
	int TileY = iy / TILE_SIZE_3D;
	return Map(TileX,TileY);

}	

bool Player::IsOnLadder( s32 ix, s32 iy, Map2D &Map )
{
	int offset = Width/2;
	int TileX1 = (ix - offset) / TILE_SIZE_3D;
	int TileX2 = (ix + offset) / TILE_SIZE_3D;
	int TileY = iy / TILE_SIZE_3D;
	if( (Map(TileX1,TileY) == LADDER ) && (Map(TileX2,TileY) == LADDER ) ) return true;
	
	return false;
}	

bool Player::IsOnTopLadder( s32 ix, s32 iy, Map2D &Map )
{
	
	int offset = Width/2;
	int TileX1 = (ix - offset) / TILE_SIZE_3D;
	int TileX2 = (ix + offset) / TILE_SIZE_3D;
	int TileY = iy / TILE_SIZE_3D;
	if( (Map(TileX1,TileY) == TOP_LADDER ) && (Map(TileX2,TileY) == TOP_LADDER ) ) return true;
	
	return false;
}	

bool Player::IsOnWater( s32 ix, s32 iy, Map2D &Map )
{
	int TileX = ix / TILE_SIZE_3D;
	int TileY = iy / TILE_SIZE_3D;
	if( (Map(TileX,TileY) == WATER ) || (Map(TileX,TileY) == TOP_WATER ) ) return true;
	
	return false;
}	

bool Player::IsOnSpike( s32 ix, s32 iy, Map2D &Map )
{
	int TileX = ix / TILE_SIZE_3D;
	int TileY = iy / TILE_SIZE_3D;
	if( (Map(TileX,TileY) == SPIKE ) ) return true;
	
	return false;
}	


bool Player::CollideTopLadders( s32 ix, s32 iy, int &TileY, Map2D &Map )
{
	s32 TileXpixels = ix - (ix % TILE_SIZE_3D);
	s32 TestEnd = ix + Width;
	
	TileY = iy / TILE_SIZE_3D;
	
	int TileX = TileXpixels / TILE_SIZE_3D;
	
	while(TileXpixels <= TestEnd)
	{
		if( (Map(TileX,TileY) == TOP_LADDER ) ) return true;	
		TileX++;
		TileXpixels += TILE_SIZE_3D;
	}	
	
	return false;
}

int Player::CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map )
{
	s32 TileXpixels = ix - (ix % TILE_SIZE_3D);
	s32 TestEnd = ix + Width;
	
	TileY = iy / TILE_SIZE_3D;
	
	int TileX = TileXpixels / TILE_SIZE_3D;
	
	while(TileXpixels <= TestEnd)
	{
		int Tile = Map(TileX,TileY);
		if(  Tile >= BRICK  ) return Tile;	
		TileX++;
		TileXpixels += TILE_SIZE_3D;
	}	
	
	return false;
}


int Player::CollideWalls( s32 ix, s32 iy, int &TileX, Map2D &Map )
{
	s32 TileYpixels = iy - (iy % TILE_SIZE_3D);
	s32 TestEnd = iy + Height;
	
	TileX = ix / TILE_SIZE_3D;
	
	int TileY = TileYpixels / TILE_SIZE_3D;
	
	while(TileYpixels <= TestEnd)
	{
		int Tile = Map(TileX,TileY);
		if(  Tile >= BRICK  ) return Tile;	
		TileY++;
		TileYpixels += TILE_SIZE_3D;
	}	

	return false;
}

void Player::CollideOnMap( Map2D &Map )
{
	
	int TileX;
	
	if( xv > 0 )
	{
		if( CollideWalls( x + xv + Width, y, TileX, Map ) )
			x = (TileX * TILE_SIZE_3D) - Width - ONE;
		else
			x += xv;
		
	}		
	else if( xv < 0 )
	{
		
		if( CollideWalls( x + xv, y, TileX, Map ) )
			x = ( (TileX + 1 ) * TILE_SIZE_3D) + ONE;
		else
			x += xv;
	}
	
}

void Player::CollideOnMap( int key, Map2D &Map )
{
	
	int TileX, TileY;
	Floored = false;
	
	
	if( IsOnWater( x + Width/2, y + Height/2, Map ) )
	{
		xv *= 3;
		xv /= 4;
		if( yv > 3 )
		{
			yv *= 3;
			yv /= 4;
		}


}
	
	if( xv > 0 )
	{
		
		if( CollideWalls( x + xv + Width, y, TileX, Map ) )
			x = (TileX * TILE_SIZE_3D) - Width - ONE;
		else
			x += xv;
		
	}		
	else if( xv < 0 )
	{
		
		if( CollideWalls( x + xv, y, TileX, Map ) )
			x = ( (TileX + 1 ) * TILE_SIZE_3D) + ONE;
		else
			x += xv;
	}
	
	
						
		
	if( yv < 0 )
	{
		
		if( CollideFloors( x, y + yv, TileY, Map ) )   // hit the roof
		{
			y = ( (TileY + 1) * TILE_SIZE_3D) + ONE ;
			yv = 0;
		}
		else
		{
			y += yv;
			yv += (GRAVITY);
			
		}
		
	}		
	else
	{
		
		if( CollideFloors( x, y + yv + Height, TileY, Map ) )
		{
			y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
			yv = ONE;
			Jumping = false;
			Falling = false;
			if( Climbing )    // set jumping to false and yv = 0 so that we could move and jump
			{
				Climbing = false;
				yv = 0;
			}
			Floored = true;
			if( !(key & Keys.KeyMap(Button::CONTROL_JUMP)) ) CanJump = true;
			
		}
		else
		{
			y += yv;
			yv += (GRAVITY);
			if( yv > FALL_SPEED_THRESHOLD ) yv = FALL_SPEED_THRESHOLD;
			CanJump = false;
		
		}
		
		if( !Climbing )
		{
			if( !Jumping )
			{
				if( !Falling )
				{
					if( CollideTopLadders( x, y + yv + Height, TileY, Map ) )
					{
						y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
						yv = 0;
						Jumping = false;
						Floored = true;
						if( !(key & Keys.KeyMap(Button::CONTROL_JUMP)) ) CanJump = true;
						
					}
				}
			}
			else  // offset a little down the check when jumping
			{
				// Needs to auto control this guy to fix that bug
				// where it snaps to top of ladder when the foot of
				// the player hits the top ladder (Fixed see code below)
				// Not really an issue with good level design
				if( CollideTopLadders( x, y + yv + Height, TileY, Map ) )
				{
					if( !Jumping )   // if not jumping snap on top
					{
						y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
						yv = 0;
						Jumping = false;
						Falling = false;
						Floored = true;
						if( !(key & Keys.KeyMap(Button::CONTROL_JUMP)) ) CanJump = true;
					}
					else  // do nothing
					{
						//yv = 0;
						//Jumping = false;
						Falling = true;
						Climbing = false;
					}
						
				}
			}
			
			
		}
	
		
	}
	
	
}

void Player::DoStuff( int key, int keyD, MapContainer &Map )
{
	
	xv = 0;
	xv = HitXv;
	
	if( !Climbing )
	{	
		if(keyD & Keys.KeyMap(Button::CONTROL_ATTACK))			// attack
		{   
			UTIL::BitSet(KeyPressed, Button::BUTTON_ATTACK );
			if( !(key & Keys.KeyMap(Button::CONTROL_DOWN)) )			// if down button aint pushed attack
			{
				//UTIL::BitSet(KeyPressed, Button::BUTTON_DOWN);
				if( AttackCoolDown < 0 )
				{
					Attacking = true;
					AttackHits = false;
					AttackCoolDown = MAX_ATKCOOLDOWN;
					CanHit = true;
					SpriteFrame = 0;
					BaseFrame = 18;
					MaxFrame = 6;
					PushCounter = 60;
					InvincibleCoolDown = MAX_INVCOOLDOWN;
					StationaryCooldown = MAX_COOLDOWN;
					Explosion::Instance()->Spawn( x + Width/2 + (Direction * Width),
												  y + Height/2,
												  0,
												  0,
												  Explode::MEDIUM_3 );
					Sound::Instance()->PlaySFX(E_SFX_ATTACK);

				}
			}
			else    // plant bomb
			{
				if( Floored && ( BombsLeft > 0 ) )
				{
					SettingBomb = true;
					SpriteFrame = 0;
					BaseFrame = 24;
					MaxFrame = 3;
					AttackCoolDown = MAX_ATKCOOLDOWN;
					PushCounter = 60;
					InvincibleCoolDown = MAX_INVCOOLDOWN;
					StationaryCooldown = MAX_COOLDOWN;
					CanPlantBomb = true;
				}
			}
			
		}
		
		if(key & Keys.KeyMap(Button::CONTROL_LEFT))
		{
			UTIL::BitSet(KeyPressed, Button::BUTTON_LEFT);
			Flipmode = GL_FLIP_H;
			xv = -speed;
		}
		
		if(key & Keys.KeyMap(Button::CONTROL_RIGHT))
		{
			UTIL::BitSet(KeyPressed, Button::BUTTON_RIGHT);
			Flipmode = GL_FLIP_NONE;
			xv = speed;
		}
		
		if( IsOnLadder( x + Width/2, y + Height, Map.Collision ) || IsOnTopLadder( x + Width/2, y + Height + 1, Map.Collision ) )
		{
			if( key & Keys.KeyMap(Button::CONTROL_UP) )
			{
				UTIL::BitSet(KeyPressed, Button::BUTTON_UP);
				// if we are on top then we cannot climb further up
				// So we only set climbing to true if foot is under top ladder tile
				if( IsOnTopLadder( x + Width/2, y + Height, Map.Collision ) )   
				{
					SnapX( x + Width/2 );
					Climbing = true;				
				}
				
				// no need to check on ladder tile when going up
				if( IsOnLadder( x + Width/2, y + Height, Map.Collision ) )
				{
						SnapX( x + Width/2 );
						Climbing = true;
				}
					
			}
			
			if( key & Keys.KeyMap(Button::CONTROL_DOWN) )
			{
				UTIL::BitSet(KeyPressed, Button::BUTTON_DOWN);
				// no need to check on top ladder tile when going down
				if( IsOnTopLadder( x + Width/2, y + Height + 1, Map.Collision ) )   
				{
					SnapX( x + Width/2 );
					Climbing = true;				
				}
				
				// if we are on bottom(floor) then we cannot climb further down
				// So we only set climbing to true if foot is under top ladder tile
				if( IsOnLadder( x + Width/2, y + Height + 1, Map.Collision ) )
				{
					
					SnapX( x + Width/2 );
					Climbing = true;
										
				}
				
			}
		}
		else  // not on laddder
		{
			if( key & Keys.KeyMap(Button::CONTROL_UP) )
			{
				UTIL::BitSet(KeyPressed, Button::BUTTON_UP);
				Looking = -1;
			}
			else if( key & Keys.KeyMap(Button::CONTROL_DOWN) )
			{
				UTIL::BitSet(KeyPressed, Button::BUTTON_DOWN);
				Looking = 1;
			}
			else
			{
				Looking = 0;
			}
			
		}
		
		
		if( (key & Keys.KeyMap(Button::CONTROL_JUMP)) && CanJump )
		{
			UTIL::BitSet(KeyPressed, Button::BUTTON_JUMP);
			CanJump = false;
			Jumping = true;
			yv = -JUMP_HEIGHT;
			Sound::Instance()->PlaySFX(E_SFX_JUMP);
		}
		
		
	}
	else //Climbing
	{
		int TileY = 0;   // So that we can move left or right after we hit the floor going down from the ladder
		if( CollideFloors( x, y + yv + Height, TileY, Map.Collision ) ) Climbing = false;
		Looking = 0;
		Jumping = false;   // fix some bugs
		
		if( keyD & Keys.KeyMap(Button::CONTROL_JUMP))
		{
			UTIL::BitSet(KeyPressed, Button::BUTTON_JUMP);
			Climbing = false;
			Falling = true;
		}
		
		// Climbing doesn't have gravity
		yv = 0;
		if(key & Keys.KeyMap(Button::CONTROL_UP))
		{
			UTIL::BitSet(KeyPressed, Button::BUTTON_UP);
			yv = -speed;
		}
		
		if(key & Keys.KeyMap(Button::CONTROL_DOWN))
		{
			UTIL::BitSet(KeyPressed, Button::BUTTON_DOWN);
			yv = +speed;
		}
		else	// not pressing keydown so snap tiles if foot + tilesize hits top tile ladder
		{	
			if( IsOnTopLadder( x + Width/2, y + Height + TILE_SIZE_3D - 1, Map.Collision ) ) 
			{
				int TileY = (y + Height + TILE_SIZE_3D - 1) / TILE_SIZE_3D;
				y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
				yv = 0;
				Climbing = false;
				Falling = false;
				CanJump = true;  // we snapped on top of the tile so we can jump
			}
		}
		
		if(key & Keys.KeyMap(Button::CONTROL_LEFT))
		{
			UTIL::BitSet(KeyPressed, Button::BUTTON_LEFT);
			Flipmode = GL_FLIP_H;
		}
		
		if(key & Keys.KeyMap(Button::CONTROL_RIGHT))
		{
			UTIL::BitSet(KeyPressed, Button::BUTTON_RIGHT);
			Flipmode = GL_FLIP_NONE;
		}
	
		
	}
	
	// Destroy tiles that can be destroyed
	if( Attacking )
	{
		int Atx = x + Width/2 + (Direction * Width);
		int Aty = y + Height/2;
		if( Map.Collision(Atx / TILE_SIZE_3D, Aty / TILE_SIZE_3D ) == SOFT_BRICK )
		{
			Map.Collision(Atx / TILE_SIZE_3D, Aty / TILE_SIZE_3D ) = NONE;
			Map.Base(Atx / TILE_SIZE_3D, Aty / TILE_SIZE_3D ) = NONE;
			Vector2df32 Pos;
			Pos.x = Atx;
			Pos.y = Aty;
			Particle::Instance()->Spawn( Pos, 24 );
			
			Explosion::Instance()->Spawn( Atx,
										  Aty,
									      0,
									      0,
									      Explode::MEDIUM_4,
									      Explode::SMALL_2 );
			AttackHits = true;   // so that we don't destroy mov blocks falling down
			Sound::Instance()->PlaySFX(E_SFX_ATTACK_HIT);
			
		}
		
	}
	
	if( Flipmode == GL_FLIP_NONE )
		Direction = RIGHT;
	else
		Direction = LEFT;
		
	
}

void Player::Update( int key, int keyD, MapContainer &Map )
{

	
	
	Counter++;
	KeyPressed = 0;
	
	UpdateShots( Map.Collision );
	UpdateBombs( Map );
	
	ShotSpawnDelay--;
	if( keyD & Keys.KeyMap(Button::CONTROL_MAGIC) )
	{
		UTIL::BitSet(KeyPressed, Button::BUTTON_MAGIC);			
		if( ShotSpawnDelay < 0 )
		{
			StationaryCooldown = MAX_COOLDOWN;
			ShotSpawnDelay = MaxShotSpawnDelay;
			if( key & Keys.KeyMap(Button::CONTROL_DOWN) )
			{
				UTIL::BitSet(KeyPressed, Button::BUTTON_DOWN);			
				ShotID = Shot::WATER;
			}
	
			SpawnShot();
		}
		
	}
	
	if( CanPlantBomb )
	{
		SpawnBomb();
		CanPlantBomb = false;
	}
	
	
	if( Invincible )
	{
		xv = 0;
		InvincibleCoolDown--;
		if( InvincibleCoolDown < 0 )
		{
			Die();
			return;
		}
	
	}
	else   // Check special tiles
	{
		// Only check if player foot is not on floor
		if( (GetTile(  x, y + Height + 1, Map.Collision ) != BRICK ) && (GetTile(  x + Width, y + Height + 1, Map.Collision ) != BRICK ) )
		{
			// Check for spikes
			if( IsOnSpike( x, y + Height + 1, Map.Collision ) || IsOnSpike( x + Width, y + Height + 1, Map.Collision ) )
			{
				Invincible = true;
				HitAnimation();
			}
		}
	}
	// Fix the bug where there is no ladder on the ground
	/// if top is on ladder but foot is not then we fall
	if( IsOnLadder( x + Width/2, y , Map.Collision ) && !IsOnLadder( x + Width/2, y + Height - 1, Map.Collision ) )
	{
		Climbing = false;
	}
	
	
	
	// move if not attacking
	if( Invincible )
	{
		MaxFrame = 4;
		BaseFrame = 28;
		SpriteFrame = SpriteFrame % MaxFrame;
		if( (Counter & 3) == 0 )
		{
			SpriteFrame = ( SpriteFrame + 1 ) % MaxFrame;
		}
		
		CollideOnMap( key, Map.Collision );
		
	}
	else
	{
		if( !Attacking && !SettingBomb)
		{
			AttackCoolDown--;
			DoStuff( key, keyD, Map );
		}
		else   // Don't move while attacking
		{
			PushCounter = 60;
			xv = 0;
			yv = 0;
		}
		
		CollideOnMap( key, Map.Collision );
		
		// Set pushcounter to max 
		if( Jumping || Climbing || Falling ) SetPushCounter( 60 );
						
		
		//Fairy.SetOx( (x + Width/2) + Direction * floattof32(1) );
		//Fairy.SetOy( y - floattof32(1)  );
		//Fairy.SetFlipmode( Flipmode );
		//Fairy.Update();
		
		// Animate
		// Override everything when attacking
		if( Attacking )
		{
			if( (Counter & 3) == 0 )
			{
				SpriteFrame = ( SpriteFrame + 1 );
				if( SpriteFrame >= 3 ) CanHit = false;
				if( SpriteFrame > MaxFrame ) Attacking = false;
			}
		}
		else if( SettingBomb )
		{
			if( (Counter & 3) == 0 )
			{
				SpriteFrame = ( SpriteFrame + 1 );
				if( SpriteFrame > MaxFrame )
				{
					SpriteFrame = MaxFrame;
					SettingBomb = false;
				}
			}
		}
		else
		{
			// Animate
			if( xv != 0 )
			{
					if( Jumping || Falling )
					{
						SpriteFrame = 0;
						MaxFrame = 1;
						BaseFrame = 17;
						SpriteFrame = SpriteFrame % MaxFrame;
					}
					else
					{
						if( yv < GRAVITY*2 )
						{
							MaxFrame = 8;
							BaseFrame = 0;
						}
						else
						{
							MaxFrame = 1;
							BaseFrame = 17;
						}
						SpriteFrame = SpriteFrame % MaxFrame;
						StationaryCooldown = MAX_COOLDOWN;				
					}
				
			}
			else
			{
				if( (!Jumping) && (!Falling) )
				{
					if( Climbing )
					{
						StationaryCooldown = MAX_COOLDOWN;
						MaxFrame = 4;
						BaseFrame = 28;
						SpriteFrame = SpriteFrame % MaxFrame;
					}
					else
					{
						StationaryCooldown--;
						if( StationaryCooldown > 0 )
						{
							MaxFrame = 1;
							BaseFrame = 16;
							SpriteFrame = SpriteFrame % MaxFrame;
						}
						else
						{
							MaxFrame = 7;
							BaseFrame = 9;
							SpriteFrame = SpriteFrame % MaxFrame;
						}
								
					}
				}
				else
				{
					if( Climbing )
					{
						StationaryCooldown = MAX_COOLDOWN;
						MaxFrame = 4;
						BaseFrame = 28;
						SpriteFrame = SpriteFrame % MaxFrame;
					}
					else
					{
						SpriteFrame = 0;
						MaxFrame = 1;
						BaseFrame = 17;
						SpriteFrame = SpriteFrame % MaxFrame;
						StationaryCooldown = MAX_COOLDOWN;
					}
				}
			}
			
			if( StationaryCooldown > 0 )
			{
				if( (Counter & 3) == 0 )
				{
					SpriteFrame = ( SpriteFrame + 1 ) % MaxFrame;
				}
			}
			else
			{	
				if( !Climbing )
				{
					
					if( (Counter & 7) == 0 )
					{
						SpriteFrame = ( SpriteFrame + 1 ) % MaxFrame;
					}
					
				}
				
			}
			
		} // Attacking
		
	}  // invincible
	
	
	cx = x + Width / 2;
	cy = y + Height / 2;
	
	// aabb
	aabb.Init( x, y, Width, Height );
	aabb2.Init( x, y, Width, Height );
	aabb2.Resize( Width/4, Height/4 );
	aabbAttack.Init( x + (Direction * Width), y + Height/4, Width, Height - Height/2);
											  
	
	if( (Counter & 1) == 0 )
	{
		for( int i = 0; i < MAX_PLAYER_SHADOWS; i++ )
		{
			if( !Shadows[i].IsActive() )		// insert on a free slot
			{
				Shadows[i].Init( x, y, 15 );
				break;
			
			}
		}
	}
	
	// Shadows
	for( int i = 0; i < MAX_PLAYER_SHADOWS; i++ )
	{
		Shadows[i].Update();
	}
	
	Keys.Push( KeyPressed );
		
}


void Player::HitAnimation()
{
	
	Sound::Instance()->PlaySFX(E_SFX_HURT);
	if( !Climbing )
	{
		if( !Jumping ) 
		{
			yv = -JUMP_HEIGHT;
		}
		else
		{
			if( yv > (GRAVITY * 5) ) yv = -JUMP_HEIGHT;
		}
		
		if( Flipmode == GL_FLIP_NONE )  // right
		{
			HitXv = -speed * 2;
		}
		else
		{
			HitXv = speed * 2;
		}
	}
	else
	{
		Climbing = false;
		yv = 1;
		if( Flipmode == GL_FLIP_NONE )  // right
		{
			HitXv = -speed * 2;
		}
		else
		{
			HitXv = speed * 2;
		}
	}
	
	
	
}

void Player::Draw( const glImage *spr )
{
	
	if( Invincible )
	{
		glSprite3D( x - (4 * TILE_SIZE_3D)/TILE_SIZE, y - (7  * TILE_SIZE_3D)/TILE_SIZE, 0, Flipmode, &spr[BaseFrame+SpriteFrame] );		
	}
	else
	{
		glSprite3D( x - (4 * TILE_SIZE_3D)/TILE_SIZE, y - (7  * TILE_SIZE_3D)/TILE_SIZE, 0, Flipmode, &spr[BaseFrame+SpriteFrame] );		
	}
	
	
	glPolyFmt( POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
	int z = -8;
	int Color = 31;
	for( int i = 0; i< MAX_PLAYER_SHADOWS; i++ )
	{
		if( Shadows[i].IsActive() )		
		{
			glColor( RGB15( Color, 31-Color, 31-Color ) );
			glSprite3D( Shadows[i].GetX() - (4 * TILE_SIZE_3D)/TILE_SIZE, Shadows[i].GetY() - (7  * TILE_SIZE_3D)/TILE_SIZE, z, Flipmode, &spr[BaseFrame+SpriteFrame] );
			z -= 4;
			Color -= 4;
			
		}
	}
	
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	glColor( RGB15( 31, 31, 31 ) );
	
	//iprintf( "\x1b[9;1HIsOnLadder = %i         ", IsOnLadder( x + Width/2, y + Height ) );
	//iprintf( "\x1b[10;1HCanJump = %i         ", CanJump );
	//iprintf( "\x1b[11;1HJumping = %i         ", Jumping );
	//iprintf( "\x1b[12;1HClimbing = %i         ", Climbing );
	//iprintf( "\x1b[13;1HFalling = %i         ", Falling );
	//iprintf( "\x1b[14;1HInvincibleCoolDown = %i         ", InvincibleCoolDown );
	//iprintf( "\x1b[15;1HInvincible = %i         ", Invincible );
	//iprintf( "\x1b[16;1HLooking = %i         ", Looking );

	
		
}

void Player::DrawAABB( s32 z, int Color )
{
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
	aabbAttack.Draw( z + 10, RGB15(31,31,31) );
	
	// Bullets
	for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if( Shots[i].IsActive() )
		{
			AABB aabb3 = Shots[i].GetAABB();
			
			aabb3.Draw( z + 5, RGB15(0,31,0) );
	
		}
	}
	
	// Bombs
	for( int i = 0; i < MAX_PLAYER_BOMBS; i++)
	{
		if( Bombs[i].IsActive() )
		{
			AABB aabb = Bombs[i].GetAabb2();			
			aabb.Draw( z + 5, RGB15(0,31,0) );
			aabb = Bombs[i].GetAabb();			
			aabb.Draw( z + 10, RGB15(31,31,31) );
			aabb = Bombs[i].GetAabb3();			
			aabb.Draw( z + 20, RGB15(31,0,0) );
			
		}
	}
	
	
}
	


//--------------------------------------
//	
//--------------------------------------
void Player::UpdateShots( Map2D &Map )
{
	
	// Bullets
	for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if( Shots[i].IsActive() )
		{
			Shots[i].Update( Map );
			
			/*  Rem this so that we dont collide
			if( Shots[i].GetID() > Shot::WAVE )
			{
				int Tx = Shots[i].GetX() / TILE_SIZE_3D;
				int Ty = Shots[i].GetY() / TILE_SIZE_3D;
				if( Map(Tx,Ty ) > NONE )
				{
					Explosion::Instance()->Spawn( Shots[i].GetX(),
												  Shots[i].GetY(),
												  0,
												  0,
												  Explode::SMALL_1 );
		
					Shots[i].Kill();
				}
			}
			*/ 
		
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Player::UpdateBombs( MapContainer &Map )
{
	
	// Bombs
	NumBombs = 0;
	for( int i = 0; i < MAX_PLAYER_BOMBS; i++)
	{
		if( Bombs[i].IsActive() )
		{
			Bombs[i].Update( Map );
			NumBombs++;
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void Player::DrawShots( const glImage *spr ) const
{
	
	// Bullets
	for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if( Shots[i].IsActive() )
		{
			
			int df = Shots[i].GetDrawFrame();
			glSpriteRotate3D( Shots[i].GetX(), Shots[i].GetY(), 4, 0, GL_FLIP_NONE, &spr[df] );
			
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Player::DrawBombs( const glImage *spr ) const
{
	
	for( int i = 0; i < MAX_PLAYER_BOMBS; i++)
	{
		if( Bombs[i].IsActive() )
		{
			Bombs[i].Draw( spr );
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Player::DrawFairy( const glImage *spr ) const
{
	
	//Fairy.Draw( spr );
	
}


//--------------------------------------
//	
//--------------------------------------
void Player::SpawnShot()
{
	if( ShotsLeft > 0 )
	{
		
		switch( ShotID )
		{
			case Shot::WAVE:
				for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
				{
					if( !Shots[i].IsActive() )
					{
						ShotsLeft--;
						Shots[i].InitWave( (x + Width/2) + ( Direction * (Width/4) ), y + Height/4, 0, 0, 0, Direction, 20 );						
						break;
					}
										
				} // i
				break;

			case Shot::NORMAL:
				for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
				{
					if( !Shots[i].IsActive() )
					{
						ShotsLeft--;
						Shots[i].InitNormal( (x + Width/2) + ( Direction * (Width/4) ), y + Height/4, 0, 0, Direction, 20 );
						break;
					}
				}		
				break;
			
			case Shot::BOUNCE:
				for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
				{
					if( !Shots[i].IsActive() )
					{
						ShotsLeft--;
						Shots[i].InitBounce( (x + Width/2) + ( Direction * (Width/4) ), y + Height/4, 0, 0, 0, Direction, 20 );
						break;
					}
				}		
				break;
			
			case Shot::THROW:
				for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
				{
					if( !Shots[i].IsActive() )
					{
						ShotsLeft--;
						Shots[i].InitThrow( (x + Width/2) + ( Direction * (Width/4) ), y + Height/4, 0, 0, Direction, 20 );
						break;
					}
				}		
				break;
			
			case Shot::WATER:
				for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
				{
					if( !Shots[i].IsActive() )
					{
						ShotsLeft--;
						Shots[i].InitWater( (x + Width/2) + ( Direction * (Width) ), y + Height/4, 0, 0, Direction, 20 );
						Sound::Instance()->PlaySFX(E_SFX_MAGIC);
						break;
					}
				}		
				break;
			
			case Shot::MAGNETO:
				for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
				{
					if( !Shots[i].IsActive() )
					{
						ShotsLeft--;
						Shots[i].InitMagneto( (x + Width/2) + ( Direction * (Width) ), y + Height/4, 0, 0, Direction, 20 );
						Sound::Instance()->PlaySFX(E_SFX_MAGIC);
						break;
					}
				}		
				break;
			
		}
	
	}
		
	// Reset shot to normal
	ShotID = Shot::MAGNETO; 
	
}


//--------------------------------------
//	
//--------------------------------------
void Player::SpawnBomb()
{

	if( BombsLeft > 0 )
	{
	
		for( int i = 0; i < MAX_PLAYER_BOMBS; i++)
		{
			if( !Bombs[i].IsActive() )
			{
				BombsLeft--;
				Bombs[i].Init( cx + Direction *(Width/16), y + Height/2, Direction );
				break;
			}
		}
		
	}
	
}

//--------------------------------------
//	
//--------------------------------------
int Player::CollideShots( AABB &Aabb )
{
	int Collisions = 0;
	for( int i = 0; i < MAX_PLAYER_BULLETS; i++)
	{
		if( ( Shots[i].IsActive() ) )
		{
			AABB Box = Shots[i].GetAABB();
			if( Box.Intersects(Aabb) )
			{
				//Explosion::Instance()->Spawn( Shots[i].GetX(),
				//							  Shots[i].GetY(),
				//							  0,
				//							  0,
				//							  Explode::SMALL_2 );
				//
				//Shots[i].Kill();
				Collisions++;
				break;
			}			
		}
	}  // next

	return Collisions;

}


//--------------------------------------
//	
//--------------------------------------
int Player::CollideBombs( AABB &Aabb )
{
	int Collisions = 0;
	for( int i = 0; i < MAX_PLAYER_BOMBS; i++)
	{
		if( Bombs[i].IsKaBoom() )
		{
			AABB Box = Bombs[i].GetAabb2();
			if( Box.Intersects(Aabb) )
			{
				Collisions++;
				break;
			}
		}
	}
	
	
	return Collisions;

}

//--------------------------------------
//	
//--------------------------------------
int Player::CollideBombsBlocks( AABB &Aabb, Map2D &Map )
{
	int Collisions = 0;
	for( int i = 0; i < MAX_PLAYER_BOMBS; i++)
	{
		if( Bombs[i].IsKaBoom() )
		{
			AABB Box = Bombs[i].GetAabb3();
			if( Box.Intersects(Aabb) )
			{
				if( (Box.y1 < Aabb.y1) )			// Only if bomb is above block
				{
					Collisions++;
					break;
				}
				
			}
			// No collision so check forward
			Box = Bombs[i].GetAabb4();
			if( Box.Intersects(Aabb) )
			{
				if( (Box.y1 < Aabb.y1) )			// Only if bomb is above block
				{
					Collisions++;
					//Bombs[i].Explode();		// so that we don't destroy more
					Bombs[i].Kill();		// than 1 block
					break;
				}
				
			}
			
			
		}
	}
	
	
	return Collisions;

}

//--------------------------------------
//	
//--------------------------------------
bool Player::CollideAttack( AABB &Aabb )
{
	if( Attacking )   // only hit one block
	{
		if( aabbAttack.Intersects(Aabb) )
		{
			AttackHits = true;
			return true;
		}			
	}
	
	return false;
}

//--------------------------------------
//	
//--------------------------------------
bool Player::CollideAttackBlocks( AABB &Aabb )
{
	if( (Attacking) && (!AttackHits) )   // only hit one block
	{
		if( aabbAttack.Intersects(Aabb) )
		{
			AttackHits = true;			
			return true;
		}			
	}

	return false;
}


//--------------------------------------
//	
//--------------------------------------
void Player::Die()
{
	
	InvincibleCoolDown = MAX_INVCOOLDOWN;
	Invincible = false;
	HitXv = 0;
	Dead = true;
	Lives--;
	
	//x = -1000 << 12;
	//y = -1000 << 12;
	cx = x + Width / 2;
	cy = y + Height / 2;
	
	// aabb
	aabb.Init( x, y, Width, Height );
	aabb2.Init( x, y, Width, Height );
	aabb2.Resize( Width/4, Height/4 );
	aabbAttack.Init( x + (Direction * Width), y + Height/4, Width, Height - Height/2);
	
}

//--------------------------------------
//	
//--------------------------------------
void Player::GetAnimationFrames()
{
	
	if( Attacking )
	{
		MaxFrame = 4;
		BaseFrame = 28;
		SpriteFrame = SpriteFrame % MaxFrame;
	}
	else if( Climbing )
	{
		MaxFrame = 4;
		BaseFrame = 28;
		SpriteFrame = SpriteFrame % MaxFrame;
	}
	else if( Jumping )
	{
		MaxFrame = 4;
		BaseFrame = 28;
		SpriteFrame = SpriteFrame % MaxFrame;
	}
	else if( SettingBomb )
	{
		MaxFrame = 4;
		BaseFrame = 28;
		SpriteFrame = SpriteFrame % MaxFrame;
	}
	else
	{
		MaxFrame = 8;
		BaseFrame = 0;
		SpriteFrame = SpriteFrame % MaxFrame;
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Player::Animate()
{
	
}




void Player::CollideOnMapReplay( Map2D &Map )
{
	
	int TileX, TileY;
	Floored = false;
	
	
	if( IsOnWater( x + Width/2, y + Height/2, Map ) )
	{
		xv *= 3;
		xv /= 4;
		if( yv > 3 )
		{
			yv *= 3;
			yv /= 4;
		}


}
	
	if( xv > 0 )
	{
		
		if( CollideWalls( x + xv + Width, y, TileX, Map ) )
			x = (TileX * TILE_SIZE_3D) - Width - ONE;
		else
			x += xv;
		
	}		
	else if( xv < 0 )
	{
		
		if( CollideWalls( x + xv, y, TileX, Map ) )
			x = ( (TileX + 1 ) * TILE_SIZE_3D) + ONE;
		else
			x += xv;
	}
	
	
						
		
	if( yv < 0 )
	{
		
		if( CollideFloors( x, y + yv, TileY, Map ) )   // hit the roof
		{
			y = ( (TileY + 1) * TILE_SIZE_3D) + ONE ;
			yv = 0;
		}
		else
		{
			y += yv;
			yv += (GRAVITY);
			
		}
		
	}		
	else
	{
		
		if( CollideFloors( x, y + yv + Height, TileY, Map ) )
		{
			y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
			yv = ONE;
			Jumping = false;
			Falling = false;
			if( Climbing )    // set jumping to false and yv = 0 so that we could move and jump
			{
				Climbing = false;
				yv = 0;
			}
			Floored = true;
			if( !(UTIL::BitIsSet( KeyMap, Button::BUTTON_JUMP)) ) CanJump = true;
			
		}
		else
		{
			y += yv;
			yv += (GRAVITY);
			if( yv > FALL_SPEED_THRESHOLD ) yv = FALL_SPEED_THRESHOLD;
			CanJump = false;
		
		}
		
		if( !Climbing )
		{
			if( !Jumping )
			{
				if( !Falling )
				{
					if( CollideTopLadders( x, y + yv + Height, TileY, Map ) )
					{
						y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
						yv = 0;
						Jumping = false;
						Floored = true;
						if( !(UTIL::BitIsSet( KeyMap, Button::BUTTON_JUMP)) ) CanJump = true;
						
					}
				}
			}
			else  // offset a little down the check when jumping
			{
				// Needs to auto control this guy to fix that bug
				// where it snaps to top of ladder when the foot of
				// the player hits the top ladder (Fixed see code below)
				// Not really an issue with good level design
				if( CollideTopLadders( x, y + yv + Height, TileY, Map ) )
				{
					if( !Jumping )   // if not jumping snap on top
					{
						y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
						yv = 0;
						Jumping = false;
						Falling = false;
						Floored = true;
						if( !(UTIL::BitIsSet( KeyMap, Button::BUTTON_JUMP)) ) CanJump = true;
					}
					else  // do nothing
					{
						//yv = 0;
						//Jumping = false;
						Falling = true;
						Climbing = false;
					}
						
				}
			}
			
			
		}
	
		
	}
	
	
}

void Player::DoStuffReplay( MapContainer &Map )
{
	
	xv = 0;
	xv = HitXv;
	
	if( !Climbing )
	{	
		if(UTIL::BitIsSet( KeyMap, Button::BUTTON_ATTACK))			// attack
		{   
			if( !(UTIL::BitIsSet( KeyMap, Button::BUTTON_DOWN)) )			// if down button aint pushed attack
			{
				if( AttackCoolDown < 0 )
				{
					Attacking = true;
					AttackHits = false;
					AttackCoolDown = MAX_ATKCOOLDOWN;
					CanHit = true;
					SpriteFrame = 0;
					BaseFrame = 18;
					MaxFrame = 6;
					PushCounter = 60;
					InvincibleCoolDown = MAX_INVCOOLDOWN;
					StationaryCooldown = MAX_COOLDOWN;
					Explosion::Instance()->Spawn( x + Width/2 + (Direction * Width),
												  y + Height/2,
												  0,
												  0,
												  Explode::MEDIUM_3 );
					Sound::Instance()->PlaySFX(E_SFX_ATTACK);

				}
			}
			else    // plant bomb
			{
				if( Floored && ( BombsLeft > 0 ) )
				{
					SettingBomb = true;
					SpriteFrame = 0;
					BaseFrame = 24;
					MaxFrame = 3;
					AttackCoolDown = MAX_ATKCOOLDOWN;
					PushCounter = 60;
					InvincibleCoolDown = MAX_INVCOOLDOWN;
					StationaryCooldown = MAX_COOLDOWN;
					CanPlantBomb = true;
				}
			}
			
		}
		
		if(UTIL::BitIsSet( KeyMap, Button::BUTTON_LEFT))
		{
			Flipmode = GL_FLIP_H;
			xv = -speed;
		}
		
		if(UTIL::BitIsSet( KeyMap, Button::BUTTON_RIGHT))
		{
			Flipmode = GL_FLIP_NONE;
			xv = speed;
		}
		
		if( IsOnLadder( x + Width/2, y + Height, Map.Collision ) || IsOnTopLadder( x + Width/2, y + Height + 1, Map.Collision ) )
		{
			if( UTIL::BitIsSet( KeyMap, Button::BUTTON_UP) )
			{
				// if we are on top then we cannot climb further up
				// So we only set climbing to true if foot is under top ladder tile
				if( IsOnTopLadder( x + Width/2, y + Height, Map.Collision ) )   
				{
					SnapX( x + Width/2 );
					Climbing = true;				
				}
				
				// no need to check on ladder tile when going up
				if( IsOnLadder( x + Width/2, y + Height, Map.Collision ) )
				{
						SnapX( x + Width/2 );
						Climbing = true;
				}
					
			}
			
			if( UTIL::BitIsSet( KeyMap, Button::BUTTON_DOWN) )
			{
				// no need to check on top ladder tile when going down
				if( IsOnTopLadder( x + Width/2, y + Height + 1, Map.Collision ) )   
				{
					SnapX( x + Width/2 );
					Climbing = true;				
				}
				
				// if we are on bottom(floor) then we cannot climb further down
				// So we only set climbing to true if foot is under top ladder tile
				if( IsOnLadder( x + Width/2, y + Height + 1, Map.Collision ) )
				{
					
					SnapX( x + Width/2 );
					Climbing = true;
										
				}
				
			}
		}
		else  // not on laddder
		{
			if( UTIL::BitIsSet( KeyMap, Button::BUTTON_UP) )
			{
				Looking = -1;
			}
			else if( UTIL::BitIsSet( KeyMap, Button::BUTTON_DOWN) )
			{
				Looking = 1;
			}
			else
			{
				Looking = 0;
			}
			
		}
		
		
		if( (UTIL::BitIsSet( KeyMap, Button::BUTTON_JUMP)) && CanJump )
		{
			CanJump = false;
			Jumping = true;
			yv = -JUMP_HEIGHT;
			Sound::Instance()->PlaySFX(E_SFX_JUMP);
		}
		
		
	}
	else //Climbing
	{
		int TileY = 0;   // So that we can move left or right after we hit the floor going down from the ladder
		if( CollideFloors( x, y + yv + Height, TileY, Map.Collision ) ) Climbing = false;
		Looking = 0;
		Jumping = false;   // fix some bugs
		
		if( UTIL::BitIsSet( KeyMap, Button::BUTTON_JUMP))
		{
			Climbing = false;
			Falling = true;
		}
		
		// Climbing doesn't have gravity
		yv = 0;
		if(UTIL::BitIsSet( KeyMap, Button::BUTTON_UP))
		{
			yv = -speed;
		}
		
		if(UTIL::BitIsSet( KeyMap, Button::BUTTON_DOWN))
		{
			yv = +speed;
		}
		else	// not pressing keydown so snap tiles if foot + tilesize hits top tile ladder
		{	
			if( IsOnTopLadder( x + Width/2, y + Height + TILE_SIZE_3D - 1, Map.Collision ) ) 
			{
				int TileY = (y + Height + TILE_SIZE_3D - 1) / TILE_SIZE_3D;
				y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
				yv = 0;
				Climbing = false;
				Falling = false;
				CanJump = true;  // we snapped on top of the tile so we can jump
			}
		}
		
		if(UTIL::BitIsSet( KeyMap, Button::BUTTON_LEFT))
		{
			Flipmode = GL_FLIP_H;
		}
		
		if(UTIL::BitIsSet( KeyMap, Button::BUTTON_RIGHT))
		{
			Flipmode = GL_FLIP_NONE;
		}
	
		
	}
	
	// Destroy tiles that can be destroyed
	if( Attacking )
	{
		int Atx = x + Width/2 + (Direction * Width);
		int Aty = y + Height/2;
		if( Map.Collision(Atx / TILE_SIZE_3D, Aty / TILE_SIZE_3D ) == SOFT_BRICK )
		{
			Map.Collision(Atx / TILE_SIZE_3D, Aty / TILE_SIZE_3D ) = NONE;
			Map.Base(Atx / TILE_SIZE_3D, Aty / TILE_SIZE_3D ) = NONE;
			Vector2df32 Pos;
			Pos.x = Atx;
			Pos.y = Aty;
			Particle::Instance()->Spawn( Pos, 24 );
			
			Explosion::Instance()->Spawn( Atx,
										  Aty,
									      0,
									      0,
									      Explode::MEDIUM_4,
									      Explode::SMALL_2 );
			AttackHits = true;   // so that we don't destroy mov blocks falling down
			Sound::Instance()->PlaySFX(E_SFX_ATTACK_HIT);
			
		}
		
	}
	
	if( Flipmode == GL_FLIP_NONE )
		Direction = RIGHT;
	else
		Direction = LEFT;
		
	
}

void Player::UpdateReplay( MapContainer &Map )
{

	
	
	Counter++;
	
	
	KeyPressed = 0;
	
	KeyMap = Keys.Key();
		
	UpdateShots( Map.Collision );
	UpdateBombs( Map );
	
	ShotSpawnDelay--;
	if( UTIL::BitIsSet( KeyMap, Button::BUTTON_MAGIC) )
	{
		if( ShotSpawnDelay < 0 )
		{
			StationaryCooldown = MAX_COOLDOWN;
			ShotSpawnDelay = MaxShotSpawnDelay;
			if( UTIL::BitIsSet( KeyMap, Button::BUTTON_DOWN) )
			{
				ShotID = Shot::WATER;
			}
	
			SpawnShot();
		}
		
	}
	
	if( CanPlantBomb )
	{
		SpawnBomb();
		CanPlantBomb = false;
	}
	
	
	if( Invincible )
	{
		xv = 0;
		InvincibleCoolDown--;
		if( InvincibleCoolDown < 0 )
		{
			Die();
			return;
		}
	
	}
	else   // Check special tiles
	{
		// Only check if player foot is not on floor
		if( (GetTile(  x, y + Height + 1, Map.Collision ) != BRICK ) && (GetTile(  x + Width, y + Height + 1, Map.Collision ) != BRICK ) )
		{
			// Check for spikes
			if( IsOnSpike( x, y + Height + 1, Map.Collision ) || IsOnSpike( x + Width, y + Height + 1, Map.Collision ) )
			{
				Invincible = true;
				HitAnimation();
			}
		}
	}
	// Fix the bug where there is no ladder on the ground
	/// if top is on ladder but foot is not then we fall
	if( IsOnLadder( x + Width/2, y , Map.Collision ) && !IsOnLadder( x + Width/2, y + Height - 1, Map.Collision ) )
	{
		Climbing = false;
	}
	
	
	
	// move if not attacking
	if( Invincible )
	{
		MaxFrame = 4;
		BaseFrame = 28;
		SpriteFrame = SpriteFrame % MaxFrame;
		if( (Counter & 3) == 0 )
		{
			SpriteFrame = ( SpriteFrame + 1 ) % MaxFrame;
		}
		
		CollideOnMapReplay( Map.Collision );
		
	}
	else
	{
		if( !Attacking && !SettingBomb)
		{
			AttackCoolDown--;
			DoStuffReplay( Map );
		}
		else   // Don't move while attacking
		{
			PushCounter = 60;
			xv = 0;
			yv = 0;
		}
		
		CollideOnMapReplay( Map.Collision );
		
		// Set pushcounter to max 
		if( Jumping || Climbing || Falling ) SetPushCounter( 60 );
						
		
		//Fairy.SetOx( (x + Width/2) + Direction * floattof32(1) );
		//Fairy.SetOy( y - floattof32(1)  );
		//Fairy.SetFlipmode( Flipmode );
		//Fairy.Update();
		
		// Animate
		// Override everything when attacking
		if( Attacking )
		{
			if( (Counter & 3) == 0 )
			{
				SpriteFrame = ( SpriteFrame + 1 );
				if( SpriteFrame >= 3 ) CanHit = false;
				if( SpriteFrame > MaxFrame ) Attacking = false;
			}
		}
		else if( SettingBomb )
		{
			if( (Counter & 3) == 0 )
			{
				SpriteFrame = ( SpriteFrame + 1 );
				if( SpriteFrame > MaxFrame )
				{
					SpriteFrame = MaxFrame;
					SettingBomb = false;
				}
			}
		}
		else
		{
			// Animate
			if( xv != 0 )
			{
					if( Jumping || Falling )
					{
						SpriteFrame = 0;
						MaxFrame = 1;
						BaseFrame = 17;
						SpriteFrame = SpriteFrame % MaxFrame;
					}
					else
					{
						if( yv < GRAVITY*2 )
						{
							MaxFrame = 8;
							BaseFrame = 0;
						}
						else
						{
							MaxFrame = 1;
							BaseFrame = 17;
						}
						SpriteFrame = SpriteFrame % MaxFrame;
						StationaryCooldown = MAX_COOLDOWN;				
					}
				
			}
			else
			{
				if( (!Jumping) && (!Falling) )
				{
					if( Climbing )
					{
						StationaryCooldown = MAX_COOLDOWN;
						MaxFrame = 4;
						BaseFrame = 28;
						SpriteFrame = SpriteFrame % MaxFrame;
					}
					else
					{
						StationaryCooldown--;
						if( StationaryCooldown > 0 )
						{
							MaxFrame = 1;
							BaseFrame = 16;
							SpriteFrame = SpriteFrame % MaxFrame;
						}
						else
						{
							MaxFrame = 7;
							BaseFrame = 9;
							SpriteFrame = SpriteFrame % MaxFrame;
						}
								
					}
				}
				else
				{
					if( Climbing )
					{
						StationaryCooldown = MAX_COOLDOWN;
						MaxFrame = 4;
						BaseFrame = 28;
						SpriteFrame = SpriteFrame % MaxFrame;
					}
					else
					{
						SpriteFrame = 0;
						MaxFrame = 1;
						BaseFrame = 17;
						SpriteFrame = SpriteFrame % MaxFrame;
						StationaryCooldown = MAX_COOLDOWN;
					}
				}
			}
			
			if( StationaryCooldown > 0 )
			{
				if( (Counter & 3) == 0 )
				{
					SpriteFrame = ( SpriteFrame + 1 ) % MaxFrame;
				}
			}
			else
			{	
				if( !Climbing )
				{
					
					if( (Counter & 7) == 0 )
					{
						SpriteFrame = ( SpriteFrame + 1 ) % MaxFrame;
					}
					
				}
				
			}
			
		} // Attacking
		
	}  // invincible
	
	
	cx = x + Width / 2;
	cy = y + Height / 2;
	
	// aabb
	aabb.Init( x, y, Width, Height );
	aabb2.Init( x, y, Width, Height );
	aabb2.Resize( Width/4, Height/4 );
	aabbAttack.Init( x + (Direction * Width), y + Height/4, Width, Height - Height/2);
											  
	
	if( (Counter & 1) == 0 )
	{
		for( int i = 0; i < MAX_PLAYER_SHADOWS; i++ )
		{
			if( !Shadows[i].IsActive() )		// insert on a free slot
			{
				Shadows[i].Init( x, y, 15 );
				break;
			
			}
		}
	}
	
	// Shadows
	for( int i = 0; i < MAX_PLAYER_SHADOWS; i++ )
	{
		Shadows[i].Update();
	}
	
		
}
