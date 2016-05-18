/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Option.h"

static const s32 Force = floattof32(0.5);    // Arbitrary force constant
static const s32 Friction = floattof32(0.959996);	// Damping for spring
static const int Magnitude = 5;		// Arbirtarry distance kinda work like dt(deltatime) increase = faster springs

// Force should have been F = -K * X or F = -K * ( Length - Restlength)
// But meh, this is faster and works just as well

Option::Option()
{
	Counter = 0;
	
	x = 0;
	y = 0;
	xv = 0;
	yv = 0;
	ox = 0;
	oy = 0;
	Mass = 400;
	PullAccel = Force / Mass;
	
	Counter = 0;
	Flipmode = GL_FLIP_NONE;
	SpriteFrame = 0;
	BaseFrame = 0;
	MaxFrame = 4;
	
}

Option::~Option()
{
	
}

void Option::Update()
{
	Counter++;
	
	s32 Dx = x - ox;
	s32 Dy = y - oy;
	s32 PullForce = PullAccel * Magnitude;		// Change in velocity
    xv -= (Dx * PullForce) >> 12;	// Lessen the distance between the 2 points by subtracting 
    yv -= (Dy * PullForce) >> 12;	// Dist * change in velocity
    
    xv *= Friction;
    yv *= Friction;
  	
  	xv = xv >> 12;
  	yv = yv >> 12;
  	
  	x += xv;
  	y += yv;
	
	if( (Counter & 3) == 0 )
	{
		SpriteFrame = ( SpriteFrame + 1 ) % MaxFrame;
	}
	
	if( (Counter & 1) == 0 )
	{
		for( int i = 0; i < MAX_SHADOWS; i++ )
		{
			if( !Shadows[i].IsActive() )		// insert on a free slot
			{
				Shadows[i].Init( x, y, 15 );
				break;
			
			}
		}
	}
	
  	
	// Shadows
	for( int i = 0; i < MAX_SHADOWS; i++ )
	{
		Shadows[i].Update();
	}
	
}

void Option::Draw( const glImage *spr ) const
{
	glSprite3D( x - (16 * TILE_SIZE_3D)/TILE_SIZE, y - (16  * TILE_SIZE_3D)/TILE_SIZE, 4, Flipmode, &spr[BaseFrame+SpriteFrame] );

	glPolyFmt( POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
	int z = -8;
	int Color = 31;
	for( int i = 0; i< MAX_SHADOWS; i++ )
	{
		if( Shadows[i].IsActive() )		
		{
			glColor( RGB15( 31-Color, Color, Color ) );
			glSprite3D( Shadows[i].GetX() - (16 * TILE_SIZE_3D)/TILE_SIZE, Shadows[i].GetY() - (16  * TILE_SIZE_3D)/TILE_SIZE, z, Flipmode, &spr[BaseFrame+SpriteFrame] );
			z -= 4;
			Color -= 4;
			
		}
	}
	
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	glColor( RGB15( 31, 31, 31 ) );
			
}
	
	