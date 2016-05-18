/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Fx3D.h"


static const int F3D_TILE_SIZE = 16;
static const int XRES = SCREEN_WIDTH/F3D_TILE_SIZE;
static const int YRES = SCREEN_HEIGHT/F3D_TILE_SIZE;

int Xcoord[XRES+4][YRES+4];
int Ycoord[XRES+4][YRES+4];

VectorSpring Coords[XRES+4][YRES+4];


Fx3D* Fx3D::ms_instance = 0;


//--------------------------------------
//	
//--------------------------------------
Fx3D::Fx3D()
{

	for( int y = 0; y < (YRES + 4); y++)
	{
		for( int x = 0; x < (XRES + 4); x++)
		{
			Coords[x][y].SetParameters( 400, floattof32(1.5), floattof32(0.9), 15 );
			Coords[x][y].SetOx((x * F3D_TILE_SIZE) << 12);
			Coords[x][y].SetOy((y * F3D_TILE_SIZE) << 12);
		}
	}
	
	for( int y = 0; y < (YRES + 4); y++)
	{
		for( int x = 0; x < (XRES + 4); x++)
		{
			Coords[x][y].Update();
		}
	}
	
}

Fx3D::~Fx3D()
{
}

//--------------------------------------
//	
//--------------------------------------
Fx3D* Fx3D::Instance()
{
	if(ms_instance == 0){
		ms_instance = new Fx3D();
	}
	return ms_instance;
}

//--------------------------------------
//	
//--------------------------------------
void Fx3D::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}




//--------------------------------------
//	 
//--------------------------------------
void Fx3D::Fade( int Color1,    // Color to fade to 
			     int Color2,
				 int Color3,
				 int Color4,
				 s32 &Interp,
			     s32 Speed )
{

	Interp += Speed;
	Interp = UTIL::Clamp( Interp, 0, 4096 );
	int Trans = UTIL::Lerpf32( 1 << 12 , 31 << 12, Interp ) >> 12;  
	Trans = UTIL::Clamp( Trans, 1, 31 );
	
	glPolyFmt( POLY_ALPHA(Trans) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );

	glBoxFilledGradient( 0, 0, 256, 192,
						 Color1,
						 Color2,
						 Color3,
						 Color4
					   );
	
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
	
}



//--------------------------------------
//	 
//--------------------------------------
void Fx3D::WarpScreen( int Frame, const glImage *sprite )
              
{
	

	// Calculate warp
	for( int y = -1; y <= (YRES+1); y++)
	{
		for( int x = -1; x <= (XRES+1); x++)
		{
			s32 tx = (x * F3D_TILE_SIZE) << 12;
			s32 ty = (y * F3D_TILE_SIZE) << 12;
			s32 stx = sinLerp(((x+y)*1230 + Frame*130)) +
					  sinLerp((y*2750 + Frame*600));
			s32 sty = sinLerp((y*4630 + Frame*400)) +
					  cosLerp(((y+x)*6400 + Frame*290));
			
			stx *= 5;
			sty *= 5;
			
			Xcoord[x+1][y+1] = (tx+stx)>>12;
			Ycoord[x+1][y+1] = (ty+sty)>>12;
		}
	}
	
	
	// Draw warp
	//glColor( RGB15(31,31,31) );

	for( int y = -1; y <= YRES; y++)
	{
		for( int x = -1; x <= XRES; x++)
		{
			int xx = x+1;
			int yy = y+1;
			
			int x1 = Xcoord[xx][yy];
			int y1 = Ycoord[xx][yy];
			int x2 = Xcoord[xx+1][yy];
			int y2 = Ycoord[xx+1][yy];
			int x3 = Xcoord[xx+1][yy+1];
			int y3 = Ycoord[xx+1][yy+1];
			int x4 = Xcoord[xx][yy+1];
			int y4 = Ycoord[xx][yy+1];
			
			glSpriteOnQuad( x1, y1,
							x2, y2,
							x3, y3,
							x4, y4,
							Frame & 31,(Frame*2) & 31,
							GL_FLIP_NONE, 
							&sprite[0] );
		}
	}
	
			
	
}


//--------------------------------------
//	 
//--------------------------------------
void Fx3D::WarpScreenPolar( int Frame, const glImage *sprite )              
{
	

	const int XMID = (XRES+2) / 2;
	const int YMID = (YRES+2) / 2;
	
	// Calculate warp
	for( int y = -1; y <= (YRES+1); y++)
	{
		for( int x = -1; x <= (XRES+1); x++)
		{
			s32 tx = (x * F3D_TILE_SIZE) << 12;
			s32 ty = (y * F3D_TILE_SIZE) << 12;
			s32 dx = ( XMID - x );
			s32 dy = ( YMID - y );
			s32 dist = sqrt32(dx * dx + dy * dy);
			s32 stx = sinLerp(((dist << 12) + atan2Lerp(dy,dx)*8) + Frame*500);
			
			stx *= 5;
			
			Xcoord[x+1][y+1] = (tx+stx)>>12;
			Ycoord[x+1][y+1] = (ty+stx)>>12;
		}
	}
	
	
	// Draw warp
	//glColor( RGB15(31,31,31) );

	for( int y = -1; y <= YRES; y++)
	{
		for( int x = -1; x <= XRES; x++)
		{
			int xx = x+1;
			int yy = y+1;
			
			int x1 = Xcoord[xx][yy];
			int y1 = Ycoord[xx][yy];
			int x2 = Xcoord[xx+1][yy];
			int y2 = Ycoord[xx+1][yy];
			int x3 = Xcoord[xx+1][yy+1];
			int y3 = Ycoord[xx+1][yy+1];
			int x4 = Xcoord[xx][yy+1];
			int y4 = Ycoord[xx][yy+1];
			
			glSpriteOnQuad( x1, y1,
							x2, y2,
							x3, y3,
							x4, y4,
							(-Frame*2) & 31, 0,
							GL_FLIP_NONE, 
							&sprite[0] );
		}
	}
	
			
	
}



//--------------------------------------
//	 
//--------------------------------------
void Fx3D::RubberScreen( int Frame, const glImage *sprite )
              
{
	

	// Calculate warp
	for( int y = -1; y <= (YRES+1); y++)
	{
		for( int x = -1; x <= (XRES+1); x++)
		{
			s32 tx = (x * F3D_TILE_SIZE) << 12;
			s32 ty = (y * F3D_TILE_SIZE) << 12;
			s32 stx = sinLerp(((x+y)*1230 + Frame*130)) +
					  cosLerp((y*1750 + Frame*200));
			s32 sty = cosLerp((y*1430 + Frame*400)) +
					  sinLerp(((y+x)*1400 + Frame*290));
			stx *= 10;
			sty *= 10;
			
			
			Coords[x+1][y+1].SetOx(tx + stx);
			Coords[x+1][y+1].SetOy(ty + sty);
		}
	}
	
	
	for( int y = 0; y < (YRES + 4); y++)
	{
		for( int x = 0; x < (XRES + 4); x++)
		{
			Coords[x][y].Update();
		}
	}
	
	Coords[0][0].SetX((0 * F3D_TILE_SIZE) << 12);
	Coords[0][0].SetY((0 * F3D_TILE_SIZE) << 12);
	Coords[0][0].SetOx((0 * F3D_TILE_SIZE) << 12);
	Coords[0][0].SetOy((0 * F3D_TILE_SIZE) << 12);
	Coords[0][0].Update();
	// Draw warp
	//glColor( RGB15(31,31,31) );

	for( int y = -1; y <= YRES + 1; y++)
	{
		for( int x = -1; x <= XRES + 1; x++)
		{
			int xx = x+1;
			int yy = y+1;
			
			int x1 = Coords[xx][yy].GetX() >> 12;
			int y1 = Coords[xx][yy].GetY() >> 12;
			int x2 = Coords[xx+1][yy].GetX() >> 12;
			int y2 = Coords[xx+1][yy].GetY() >> 12;
			int x3 = Coords[xx+1][yy+1].GetX() >> 12;
			int y3 = Coords[xx+1][yy+1].GetY() >> 12;
			int x4 = Coords[xx][yy+1].GetX() >> 12;
			int y4 = Coords[xx][yy+1].GetY() >> 12;
			
			
			glSpriteOnQuad( x1, y1,
							x2, y2,
							x3, y3,
							x4, y4,
							(-Frame*2) & 31, 
							(-Frame*3) & 31,
							GL_FLIP_NONE, 
							&sprite[0] );
		}
	}
	
			
	
}
