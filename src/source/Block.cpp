/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Block.h"


Block::Block()
{
	ID = SOFT;
	Active = false;	
	
	x = 0;
	y = 0; 
	xv = 0; 
	yv = 1;
	Speed = floattof32(0); 
	
	Width = (31 * TILE_SIZE_3D) / TILE_SIZE;
	Height = (31 * TILE_SIZE_3D) / TILE_SIZE;   
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	Frame= 0;
	BaseFrame = 0;
	MaxFrame = 1;
	
	AutoControl = false;
	Thrown = false;
	AutoControlDistance = TILE_SIZE_3D / 256;
	Direction = 1;
}

Block::~Block()
{
}


void Block::Init( s32 sx, s32 sy, int type, int direction )
{
	ID = type;
	Active = true;
	
	AutoControl = false;
	Thrown = false;
	
	x = sx;  
	y = sy;
	
	xv = 0;
	yv = 0;
	
}


bool Block::CollideFloors( s32 ix, s32 iy, int &TileY, Map2D &Map )
{
	s32 TileXpixels = ix - (ix % TILE_SIZE_3D);
	s32 TestEnd = ix + Width;
	
	TileY = iy / TILE_SIZE_3D;
	
	int TileX = TileXpixels / TILE_SIZE_3D;
	
	while(TileXpixels <= TestEnd)
	{
		if( (Map(TileX,TileY) >= BRICK) || (Map(TileX,TileY) == BLOCK_COLLIDE) ) return true;	
		TileX++;
		TileXpixels += TILE_SIZE_3D;
	}	
	
	return false;
}

bool Block::CollideWalls( s32 ix, s32 iy, int &TileX, Map2D &Map )
{
	s32 TileYpixels = iy - (iy % TILE_SIZE_3D);
	s32 TestEnd = iy + Height;
	
	TileX = ix / TILE_SIZE_3D;
	
	int TileY = TileYpixels / TILE_SIZE_3D;
	
	while(TileYpixels <= TestEnd)
	{
		if( (Map(TileX,TileY) >= BRICK)  || (Map(TileX,TileY) == BLOCK_COLLIDE) ) return true;	
		TileY++;
		TileYpixels += TILE_SIZE_3D;
	}	

	return false;
}

void Block::CollideOnMap( Map2D &Map )
{
	
	int TileX, TileY;
	
	Direction = 0;
	
	if( !Thrown )
	{
		if( yv >= 0 )
		{
			
			if( CollideFloors( x, y + yv + Height, TileY, Map ) )
			{
				if( yv > 512 ) Sound::Instance()->PlaySFX(E_SFX_BLOCK_HIT_FLOOR);
				
				y = ( TileY * TILE_SIZE_3D ) - Height - ONE;
				yv = ONE;
				if( !AutoControl && ( xv == 0 ) )
				{
					int Tx = ( x + Width / 2 ) / TILE_SIZE_3D;
					int Ty = ( y + Height / 2 ) / TILE_SIZE_3D;
					Map( Tx, Ty ) = INVISIBLE;      // set as an invisible tile for easy collisions
				}
				
			}
			else
			{
				int Tx = ( x + Width / 2 ) / TILE_SIZE_3D;
				int Ty = ( y + Height / 2 ) / TILE_SIZE_3D;		
				Map( Tx, Ty ) = NONE;      // set as 0 when free falling
			
				y += yv;
				yv += GRAVITY*2;
				if( yv > FALL_SPEED_THRESHOLD ) yv = FALL_SPEED_THRESHOLD;
			}
		
		}

	}
	
	if( xv > 0 )
	{
		Direction = 1;
		//Flipmode = GL_FLIP_NONE;
	
		if( CollideWalls( x + xv + Width, y, TileX, Map ) )
		{
			Thrown = false;
			x = (TileX * TILE_SIZE_3D) - Width - ONE;
			xv = 0;
		}
		else
		{
			x += xv;
		}
		
	}		
	else if( xv < 0 )
	{
		Direction = -1;
		//Flipmode = GL_FLIP_H;
		
		if( CollideWalls( x + xv, y, TileX, Map ) )
		{
			Thrown = false;
			x = ( (TileX + 1 ) * TILE_SIZE_3D) + ONE;
			xv = 0;
		}
		else
		{
			x += xv;
		}
		
	}
	
		
	//if( xv == 0 ) Direction = 0;
	

}


void Block::Update( Player &Mario, Map2D &Map )
{
	Counter++;
	
	if( !AutoControl )
	{
		CollideOnMap( Map );
	}
	else
	{
		AutoControlDistance--;
		if( AutoControlDistance < 0 )
		{
			AutoControlDistance = TILE_SIZE_3D / 256;
			AutoControl = false;
			xv = 0;
		}
		else
		{
			CollideOnMap( Map );
		}
		
		
	}
	
	cx = x + Width/2;
	cy = y + Height/2;
	
	// aabb
	ResolveAABBS();
	
	
}

void Block::Draw( const glImage *spr, s32 z )
{
	//glSprite3D( x, y , z, Flipmode, &spr[BaseFrame+Frame] );
	if( ID == SOFT)
	{
		DrawCube( x + floattof32(0.5), y + floattof32(0.5), 0, &spr[BaseFrame+Frame] );		
	}
	else
	{
		glColor( RGB15(31,0,0));
		DrawCube( x + floattof32(0.5), y + floattof32(0.5), 0, &spr[BaseFrame+Frame] );			
		glColor( RGB15(31,31,31));
	}	
					
}
	

void Block::DrawAABB( s32 z, int Color )
{
	z += TILE_SIZE_3D/2;
	glColor( Color );
	aabb.Draw( z, Color );
	aabb2.Draw( z + 5, RGB15(0,31,0) );
}

bool Block::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
				
	if( !AutoControl )
	{
		// Only push when gripe is standing on the floor
		if( Mario.IsFloored() )
		{
			// Check to see if player hits the block
			if( Mario.GetTileY() == GetTileY() )
			{
				// Get tile being pushed
				int Tx = Mario.GetX() + Mario.GetWidth()/2;
				Tx += ( (Mario.GetWidth() / 2) + 2 ) * Mario.GetDirection();
				Tx /= TILE_SIZE_3D ;
				
				// Check if there is a collidable tile on either the left or right of the block
				if( (Map( Tx + Mario.GetDirection(), Mario.GetTileY() ) < BRICK ) &&
					(Map( Tx, Mario.GetTileY() - 1 ) != INVISIBLE ) && // check if there is a block above the block being pushed
					(Map( Tx + Mario.GetDirection(), Mario.GetTileY() ) != BLOCK_COLLIDE ) )  // if there is an enemy beside the block
				{
					int BTx = ( x + Width / 2 ) / TILE_SIZE_3D;
							
					if( abs( BTx - Tx ) < 1 )
					{
						
						if( abs(Mario.GetXV()) > 0 )
						{
							Mario.DecPushCounter();
						}
						else
						{
							Mario.SetPushCounter( 60 );
						}
						
						
						if( Mario.GetPushCounter() < 0 )
						{
							AutoControl = true;
							AutoControlDistance = TILE_SIZE_3D / 256;
							
							int BTy = ( y + Height / 2 ) / TILE_SIZE_3D;
							Map( BTx, BTy ) = NONE;
							
							Mario.SetPushCounter( 60 );
							xv = 256 * Mario.GetDirection();
							Sound::Instance()->PlaySFX(E_SFX_BLOCK_MOVE);
						}
						
					}
				
				}
				else
				{
					// Set push counter to 1 when there is an enemy on the side of the block
					// so we don't get pushed back
					if( Map( Tx + Mario.GetDirection(), Mario.GetTileY() ) == BLOCK_COLLIDE )
					{
						Mario.SetPushCounter( 1 );
					}
				}
			}
					
			
		}  // isfloored
		else   // reset push timer so that w don't push quite faster
		{
			Mario.SetPushCounter( 60 );
		}


		if( Mario.CollideBombsBlocks( aabb, Map ) )
		{
			if( ID == SOFT )
			{
				int Tx = ( x + Width / 2 ) / TILE_SIZE_3D;
				int Ty = ( y + Height / 2 ) / TILE_SIZE_3D;		
				Map( Tx, Ty ) = NONE;      // set as 0 when free falling
				Explosion::Instance()->Spawn( x + Width/2,
											  y + Height/2,
											  0,
											  0,
											  Explode::MEDIUM_4,
											  Explode::SMALL_2 );
				Sound::Instance()->PlaySFX(E_SFX_EXPLODE);
				Kill();
			}
		}
	
		
		// Only collide with normal attacks
		if( Mario.CollideAttackBlocks( aabb ) && Mario.GetCanHit() )
		{
			Sound::Instance()->PlaySFX(E_SFX_ATTACK_HIT);
			if( ID == SOFT )
			{
				int Tx = ( x + Width / 2 ) / TILE_SIZE_3D;
				int Ty = ( y + Height / 2 ) / TILE_SIZE_3D;		
				Map( Tx, Ty ) = NONE;      // set as 0 when free falling
				
				Vector2df32 Pos;
				Pos.x = x + Width/2;
				Pos.y = y + Height/2;
				Particle::Instance()->Spawn( Pos, 24 );
				
				Explosion::Instance()->Spawn( Pos.x,
											  Pos.y,
											  0,
											  0,
											  Explode::MEDIUM_4,
											  Explode::SMALL_2 );
			
					
				Sound::Instance()->PlaySFX(E_SFX_EXPLODE);
				Kill();
			}
			else
			{
				int Tx = ( x + Width / 2 ) / TILE_SIZE_3D;
				int Ty = ( y + Height / 2 ) / TILE_SIZE_3D;
				// Check if there is a collidable tile on either the left or right of the block
				//if( (Map( Tx + Mario.GetDirection(), Mario.GetTileY() ) < BRICK ) &&
				//	(Map( Tx + Mario.GetDirection(), Mario.GetTileY() ) != BLOCK_COLLIDE ) )  // if there is an enemy beside the block
				//{
					Thrown = true;
					Map( Tx, Ty ) = NONE;      // set as an none so that we dont collide				
					xv = Mario.GetDirection() * 512;
				//}
			}
		}	
		
	}   // AutoControl

	return false;
}

bool Block::CollideWithBlock( Block &B )
{
	
	
	if( aabb.Intersects( B.GetAabb() ) )
	{
		
		
		s32 cx = B.GetX() + B.GetWidth() / 2;
		s32 cy = B.GetY() + B.GetHeight() / 2;
		
		if( (y + Height) < cy )   // only step on top if player foot is above enemy center
		{
			y = (B.GetY() - Height) + 1;	// Snap to head offset a little down since our sprite is smaller than aabb height
			yv = 0;						// Don't bounce
		}
		else
		{
			if( (x + Width) < cx )   // Collide to the right of block
			{
				x = B.GetX() - Width;
			}
			else if( x > cx )   // Collide to the left of block
			{
				x =  B.GetX() + B.GetWidth();
			}
		}
		
		return true;
	}
		
	return false;
}


//------------------------------------------------------------------------------
//	BlockFactory
//------------------------------------------------------------------------------
//--------------------------------------
//	Ctor and Dtor
//--------------------------------------
BlockFactory::BlockFactory()
{
}
	
BlockFactory::~BlockFactory()
{
	Blocks.clear();				// clear 
}

//--------------------------------------
//	
//--------------------------------------
void BlockFactory::Init( glImage* const Sprites )
{
	Images = Sprites;
	Blocks.resize( MAX_BLOCKS );			// speed of iteration)
}


//--------------------------------------
//	
//--------------------------------------
const Block BlockFactory::GetBlock( int i ) const 
{
	Block B;
	B.SetActive(false);
	if( i < 0 ) return B;
	if( i >= MAX_BLOCKS ) return B;
	if( Blocks[i].IsActive() ) 
		return Blocks[i];
	
	return B;
}
	

//--------------------------------------
//	Virtual Functions
//--------------------------------------
void BlockFactory::UpdateEntities( Player &Mario, Map2D &Map  )
{
	
	NumEntities = 0;
	std::vector<Block>::iterator iter;
	for( iter = Blocks.begin(); iter != Blocks.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->Update( Mario, Map );
			NumEntities++;
		}
	}
	
	//CollideWithBlocks();
	
}

//--------------------------------------
//	
//--------------------------------------
void BlockFactory::DrawEntities( s32 z )
{
	
	std::vector<Block>::iterator iter;
	for( iter = Blocks.begin(); iter != Blocks.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->Draw( Images, z );
			z -= 4;
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BlockFactory::DrawAABB( s32 z, int color )
{
	
	std::vector<Block>::iterator iter;
	for( iter = Blocks.begin(); iter != Blocks.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->DrawAABB( z, color );
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BlockFactory::CollideWithPlayer( Player &Mario, Map2D &Map )
{
	
	std::vector<Block>::iterator iter;
	for( iter = Blocks.begin(); iter != Blocks.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->CollideWithPlayer( Mario, Map );
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BlockFactory::CollideWithBlocks()
{
	
	std::vector<Block>::iterator iter2;
	std::vector<Block>::iterator iter;
	for( iter = Blocks.begin(); iter != Blocks.end(); ++iter )
	{
		if( iter->Active )
		{
			for( iter2 = Blocks.begin(); iter2 != Blocks.end(); ++iter2 )
			{
				if( iter2->Active )
				{
					iter2->CollideWithBlock( *iter );
				}
			}
	
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void BlockFactory::Spawn( s32 x, s32 y, int type, int direction  )
{
	
	std::vector<Block>::iterator iter;
	for( iter = Blocks.begin(); iter != Blocks.end(); ++iter )
	{
	
		if( !iter->Active )		// insert on a free slot
		{
			iter->Init( x, y, type, direction );
			break;
		}
	
	}
	
}	

//--------------------------------------
//	
//--------------------------------------
void BlockFactory::CollideWithBlocks( BlockFactory &Blocks, Player &Mario )
{
	
	
}

//--------------------------------------
//	
//--------------------------------------
void BlockFactory::KillAllEntities()
{
	std::vector<Block>::iterator iter;
	for( iter = Blocks.begin(); iter != Blocks.end(); ++iter )
	{
	
		if( iter->Active )		// insert on a free slot
		{
			iter->Kill();
		}
	
	}
	
}



//--------------------------------------
// Sort the block tiles so that blocks < index are below
// used for collision in water
//--------------------------------------
void BlockFactory::SortAll()
{
	
	for( int i = 0; i < MAX_BLOCKS; i++ )
	{
		if( Blocks[i].Active )		// insert on a free slot
		{
			for( int j = 0; j < (MAX_BLOCKS-1); j++ )
			{
				if( Blocks[j].Active )		// insert on a free slot
				{
					if( Blocks[i].GetTileY() > Blocks[j].GetTileY() )
					{
						Block B = Blocks[i];
						Blocks[i] = Blocks[j];
						Blocks[j] = B;
					}
				}
			}
	
		}	
	}
	
}
	