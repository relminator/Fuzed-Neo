/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Camera.h"

static const int EYE_DISTANCE = (115 * TILE_SIZE_3D) / TILE_SIZE;

Camera::Camera()
{
	
	//EyeDistanceFromScreen = 14542;   // Distance of player's eye from screen
	EyeDistanceFromScreen = EYE_DISTANCE;
	Position.x = 0 << 12;
	Position.y = 0 << 12;
	Position.z = EyeDistanceFromScreen;
	
	Target.x = 0 << 12;
	Target.y = 0 << 12;
	Target.z = 0 << 12;
	
	Up.x = 0 << 12;
	Up.y = 1 << 12;
	Up.z = 0 << 12;

	Mode = 0;
	
	
}

Camera::~Camera()
{
}

void Camera::Update( s32 x, s32 y, int signx, int signy, Map2D &Map )
{
	switch( Mode )
	{
		case 0:
			Follow( x, y );
			break;
		case 1:
			Follow( x, y, Map );
			break;
		case 2:
			Follow( x, y, signx, signy, Map );
			break;
		case 3:
			x = (Map.GetWidth() * TILE_SIZE_3D) / 2;
			y = (Map.GetHeight() * TILE_SIZE_3D) / 2;
			Follow( x, y, signx, signy, TILE_SIZE_3D * 10, Map );
			break;
		case 4:
			FollowLimit( x, y, Map );
			break;
		default:
			Follow( x, y, signx, signy, Map );
			
	}
	
	//iprintf( "\x1b[8;1HEyeDistance = %i         ", EyeDistanceFromScreen );
	
		
}

void Camera::Update( s32 x, s32 y, int signx, int signy, s32 &offx, s32 &offy, Map2D &Map )
{
	switch( Mode )
	{
		case 0:
			Follow( x, y );
			break;
		case 1:
			Follow( x, y, Map );
			break;
		case 2:
			Follow( x, y, signx, signy, offx, offy, Map );
			break;
		case 3:
			x = (Map.GetWidth() * TILE_SIZE_3D) / 2;
			y = (Map.GetHeight() * TILE_SIZE_3D) / 2;
			Follow( x, y, signx, signy, TILE_SIZE_3D * 10, Map );
			break;
		case 4:
			FollowLimit( x, y, Map );
			break;
		default:
			Follow( x, y, signx, signy, Map );
			
	}
	
	//iprintf( "\x1b[8;1HEyeDistance = %i         ", EyeDistanceFromScreen );
	
		
}

void Camera::UpdatePaused( s32 &x, s32 &y, s32 PlayerX, s32 PlayerY, Map2D &Map )
{
	FollowLimit( x, y, Map );
}

void Camera::Follow( s32 x, s32 y )
{
	
	Position.x = x;
	Position.y = y - floattof32(0.2);    // offset a little above so we look a little down
	Position.z = EyeDistanceFromScreen;       // move zpos 15 units from screen to eye
	
	Target.x = x;
	Target.y = y;
	Target.z = 0 << 12;
	
	Up.x = 0 << 12;
	Up.y = 1 << 12;
	Up.z = 0 << 12;
	
}



void Camera::Follow( s32 x, s32 y, Map2D &Map )
{
	
	int MAP_WID = Map.GetWidth();
	int MAP_HEI = Map.GetHeight();
	
	// Calculate limits
	s32 MinX = ( TILE_SIZE_3D /2 ) + (( SCREEN_WIDTH * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MinY = ( TILE_SIZE_3D /2 ) + (( SCREEN_HEIGHT * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MaxX = (MAP_WID * TILE_SIZE_3D) - MinX;
	s32 MaxY = (MAP_HEI * TILE_SIZE_3D) - MinY;;
	
	// limit
	if( x < MinX ) x = MinX;
	if( y < MinY ) y = MinY;
	if( x > MaxX ) x = MaxX;
	if( y > MaxY ) y = MaxY;
	
	Position.x = x;
	Position.y = y;
	Position.z = EyeDistanceFromScreen;       // move zpos 15 units from screen to eye
	
	Target.x = x;
	Target.y = y;
	Target.z = 0 << 12;
	
	Up.x = 0 << 12;
	Up.y = 1 << 12;
	Up.z = 0 << 12;
	
}


void Camera::Follow( s32 x, s32 y, int signx, int signy, Map2D &Map )
{
	
	const int MAX_X = floattof32(2.0);  // Max rotation
	const int MAX_Y = floattof32(4.0);  // Max rotation
	static s32 xoff = 0;
	static s32 yoff = 0;
	
	int MAP_WID = Map.GetWidth();
	int MAP_HEI = Map.GetHeight();


	if( signx > 0 )
	{
		if( xoff < MAX_X ) xoff += floattof32(0.04);  
	}
	else if( signx < 0 )
	{
		if( xoff > -MAX_X ) xoff -= floattof32(0.04);
	}
	else  // 0
	{
		if( xoff > 0 ) xoff -= floattof32(0.02);
		if( xoff < 0 ) xoff += floattof32(0.02);
	}
	
	if( signy > 0 )
	{
		if( yoff < MAX_Y ) yoff += floattof32(0.04);  
	}
	else if( signy < 0 )
	{
		if( yoff > -MAX_Y ) yoff -= floattof32(0.04);
	}
	else  // 0
	{
		if( yoff > 0 ) yoff -= floattof32(0.08);
		if( yoff < 0 ) yoff += floattof32(0.08);
	}
	

	// Calculate limits
	s32 MinX = ( TILE_SIZE_3D /2 ) + (( SCREEN_WIDTH * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MinY = ( TILE_SIZE_3D /2 ) + (( SCREEN_HEIGHT * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MaxX = (MAP_WID * TILE_SIZE_3D) - MinX;
	s32 MaxY = (MAP_HEI * TILE_SIZE_3D) - MinY;;
	
	x += xoff;
	y += yoff;
	// limit
	if( x < MinX ) x = MinX;
	if( y < MinY ) y = MinY;
	if( x > MaxX ) x = MaxX;
	if( y > MaxY ) y = MaxY;
	
	
	Position.x = x;
	Position.y = y;
	Position.z = EyeDistanceFromScreen;       // move zpos 15 units from screen to eye
	
	Target.x = x;
	Target.y = y;
	Target.z = 0 << 12;
	
	Up.x = 0 << 12;
	Up.y = 1 << 12;
	Up.z = 0 << 12;
	
}

void Camera::Follow( s32 x, s32 y, int signx, int signy, int limit, Map2D &Map )
{
	
	int MAP_WID = Map.GetWidth();
	int MAP_HEI = Map.GetHeight();
	
	int MAX_X = MAP_WID/2 * TILE_SIZE_3D;  // Max rotation
	int MAX_Y =  MAP_HEI/2 * TILE_SIZE_3D;  // Max rotation
	
	static s32 xoff = 0;
	static s32 yoff = 0;
	
	
	if( EyeDistanceFromScreen > limit )  EyeDistanceFromScreen = limit;
	
	if( signx > 0 )
	{
		if( xoff < MAX_X ) xoff += floattof32(0.04);  
	}
	else if( signx < 0 )
	{
		if( xoff > -MAX_X ) xoff -= floattof32(0.04);
	}
	else  // 0
	{
		if( xoff > 0 ) xoff -= floattof32(0.02);
		if( xoff < 0 ) xoff += floattof32(0.02);
	}
	
	if( signy > 0 )
	{
		if( yoff < MAX_Y ) yoff += floattof32(0.04);  
	}
	else if( signy < 0 )
	{
		if( yoff > -MAX_Y ) yoff -= floattof32(0.04);
	}
	else  // 0
	{
		if( yoff > 0 ) yoff -= floattof32(0.08);
		if( yoff < 0 ) yoff += floattof32(0.08);
	}
	

	// Calculate limits
	s32 MinX = ( TILE_SIZE_3D /2 ) + (( SCREEN_WIDTH * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MinY = ( TILE_SIZE_3D /2 ) + (( SCREEN_HEIGHT * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MaxX = (MAP_WID * TILE_SIZE_3D) - MinX;
	s32 MaxY = (MAP_HEI * TILE_SIZE_3D) - MinY;;
	
	x += xoff;
	y += yoff;
	// limit
	if( x < MinX ) x = MinX;
	if( y < MinY ) y = MinY;
	if( x > MaxX ) x = MaxX;
	if( y > MaxY ) y = MaxY;
	
	
	Position.x = x;
	Position.y = y;
	Position.z = EyeDistanceFromScreen;       // move zpos 15 units from screen to eye
	
	Target.x = x;
	Target.y = y;
	Target.z = 0 << 12;
	
	Up.x = 0 << 12;
	Up.y = 1 << 12;
	Up.z = 0 << 12;
	
}


void Camera::FollowLimit( s32 &x, s32 &y, Map2D &Map )
{
	
	int MAP_WID = Map.GetWidth();
	int MAP_HEI = Map.GetHeight();
	
	// Calculate limits
	s32 MinX = ( TILE_SIZE_3D /2 ) + (( SCREEN_WIDTH * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MinY = ( TILE_SIZE_3D /2 ) + (( SCREEN_HEIGHT * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MaxX = (MAP_WID * TILE_SIZE_3D) - MinX;
	s32 MaxY = (MAP_HEI * TILE_SIZE_3D) - MinY;;
	
	// limit
	if( x < MinX ) x = MinX;
	if( y < MinY ) y = MinY;
	if( x > MaxX ) x = MaxX;
	if( y > MaxY ) y = MaxY;
	
	Position.x = x;
	Position.y = y;
	Position.z = EyeDistanceFromScreen;       // move zpos 15 units from screen to eye
	
	Target.x = x;
	Target.y = y;
	Target.z = 0 << 12;
	
	Up.x = 0 << 12;
	Up.y = 1 << 12;
	Up.z = 0 << 12;
	
	
}

void Camera::Follow( s32 x, s32 y, int signx, int signy, s32 &offx, s32 &offy, Map2D &Map )
{
	
	const int MAX_X = floattof32(2.0);  // Max rotation
	const int MAX_Y = floattof32(4.0);  // Max rotation
	static s32 xoff = 0;
	static s32 yoff = 0;
	
	int MAP_WID = Map.GetWidth();
	int MAP_HEI = Map.GetHeight();


	if( signx > 0 )
	{
		if( xoff < MAX_X ) xoff += floattof32(0.04);  
	}
	else if( signx < 0 )
	{
		if( xoff > -MAX_X ) xoff -= floattof32(0.04);
	}
	else  // 0
	{
		if( xoff > 0 ) xoff -= floattof32(0.02);
		if( xoff < 0 ) xoff += floattof32(0.02);
	}
	
	if( signy > 0 )
	{
		if( yoff < MAX_Y ) yoff += floattof32(0.04);  
	}
	else if( signy < 0 )
	{
		if( yoff > -MAX_Y ) yoff -= floattof32(0.04);
	}
	else  // 0
	{
		if( yoff > 0 ) yoff -= floattof32(0.08);
		if( yoff < 0 ) yoff += floattof32(0.08);
	}
	

	// Calculate limits
	s32 MinX = ( TILE_SIZE_3D /2 ) + (( SCREEN_WIDTH * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MinY = ( TILE_SIZE_3D /2 ) + (( SCREEN_HEIGHT * TILE_SIZE_3D) / TILE_SIZE) / 2;
	s32 MaxX = (MAP_WID * TILE_SIZE_3D) - MinX;
	s32 MaxY = (MAP_HEI * TILE_SIZE_3D) - MinY;;
	
	x += xoff;
	y += yoff;
	// limit
	if( x < MinX ) x = MinX;
	if( y < MinY ) y = MinY;
	if( x > MaxX ) x = MaxX;
	if( y > MaxY ) y = MaxY;
	
	
	Position.x = x;
	Position.y = y;
	Position.z = EyeDistanceFromScreen;       // move zpos 15 units from screen to eye
	
	Target.x = x;
	Target.y = y;
	Target.z = 0 << 12;
	
	Up.x = 0 << 12;
	Up.y = 1 << 12;
	Up.z = 0 << 12;
	
	offx = xoff;
	offy = yoff;
	
}

void Camera::Look()
{
	
	gluLookAtf32( Position.x, Position.y, Position.z,    // camera pos
				  Target.x, Target.y, Target.z,     // camera target
				  Up.x, Up.y, Up.z);

}

void Camera::Zoom( s32 value )
{
	EyeDistanceFromScreen +=value;
	if( EyeDistanceFromScreen < EYE_DISTANCE )  EyeDistanceFromScreen = EYE_DISTANCE;
	if( EyeDistanceFromScreen > EYE_DISTANCE * 2 )  EyeDistanceFromScreen = EYE_DISTANCE * 2;
	
	
}
