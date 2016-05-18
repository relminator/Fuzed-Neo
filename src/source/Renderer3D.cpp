/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Renderer3D.h"


//Vertices for the cube
static v16 CubeVectors[] =
{
 		floattov16(-0.5), floattov16(-0.5), floattov16(0.5), 
		floattov16(0.5),  floattov16(-0.5), floattov16(0.5),
		floattov16(0.5),  floattov16(-0.5), floattov16(-0.5),
		floattov16(-0.5), floattov16(-0.5), floattov16(-0.5),
		floattov16(-0.5), floattov16(0.5),  floattov16(0.5), 
		floattov16(0.5),  floattov16(0.5),	floattov16(0.5),
		floattov16(0.5),  floattov16(0.5),  floattov16(-0.5),
		floattov16(-0.5), floattov16(0.5),  floattov16(-0.5)
};

//polys
static u8 CubeFaces[] =
{
	3,2,1,0,
	0,1,5,4,
	1,2,6,5,
	2,3,7,6,
	3,0,4,7,
	4,5,6,7
	//5,6,7,4
};


static inline void gxTexCoord2i(t16 u, t16 v)
{
	GFX_TEX_COORD = (v << 20) | ( (u << 4) & 0xFFFF );
}

void glBox3D( s32 x, s32 y, s32 z, int color )
{
	
	v16 wid = floattov16(1);
	v16 hei = floattov16(1);
	
	glColor( color );
	glPushMatrix();
		glTranslatef32(x, y, z);
		glBegin( GL_QUADS );
			glVertex3v16(   0,    0, 0 );
			glVertex3v16( wid,    0, 0 );
			glVertex3v16( wid,  hei, 0 );
			glVertex3v16(   0,  hei, 0 );
		glEnd();
	glPopMatrix(1);
	
	glColor( RGB15(31,31,31) );
	
	
}

void glBox3Dv16( s32 x1, s32 y1, s32 x2, s32 y2, s32 z, int color )
{
	
	x2++;
	y2++;
	
	glColor( color );
	glPushMatrix();
		glTranslatef32(x1, y1, z);
		glBegin( GL_TRIANGLES );
	
			glVertex3v16( x1, y1, 0 );
			glVertex3v16( x2, y1, 0 );
			glVertex3v16( x2, y1, 0 );

			glVertex3v16( x2, y1, 0 );
			glVertex3v16( x2, y2, 0 );
			glVertex3v16( x2, y2, 0 );

			glVertex3v16( ++x2,y2, 0 );  // bug fix for lower-right corner disappearing pixel
			glVertex3v16( x1,y2, 0 );
			glVertex3v16( x1,y2, 0 );

			glVertex3v16( x1,y2, 0 );
			glVertex3v16( x1,y1, 0 );
			glVertex3v16( x1,y1, 0 );
		
		glEnd();
	
	glPopMatrix(1);
	
	glColor( RGB15(31,31,31) );
	
	
}

void glBoxFilled3Dv16( s32 x1, s32 y1, s32 x2, s32 y2, s32 z, int color )
{
	
	v16 wid = (x2 - x1) + 1;
	v16 hei = (y2 - y1) + 1;
	
	glColor( color );
	glPushMatrix();
		glTranslatef32(x1, y1, z);
		glBegin( GL_QUADS );
			glVertex3v16(   0,    0, 0 );
			glVertex3v16( wid,    0, 0 );
			glVertex3v16( wid,  hei, 0 );
			glVertex3v16(   0,  hei, 0 );
		glEnd();
	glPopMatrix(1);
	
	glColor( RGB15(31,31,31) );
	
	
}


void glSprite3D( s32 x, s32 y, s32 z, int flipmode, const glImage *spr )
{
	
	v16 wid = (spr->width * TILE_SIZE_3D) / TILE_SIZE;
	v16 hei = (spr->height * TILE_SIZE_3D) / TILE_SIZE;
	
	int	u1 = spr->u_off + (( flipmode & GL_FLIP_H ) ? spr->width-1  : 0);
 	int	u2 = spr->u_off + (( flipmode & GL_FLIP_H ) ? 0			    : spr->width);
	int v1 = spr->v_off + (( flipmode & GL_FLIP_V ) ? spr->height-1 : 0);
 	int v2 = spr->v_off + (( flipmode & GL_FLIP_V ) ? 0 		    : spr->height);
	
	
	glPushMatrix();
		glTranslatef32(x, y, z);
		glBegin( GL_QUADS );
			gxTexCoord2i( u1, v1 );glVertex3v16(   0,    0, 0 );
			gxTexCoord2i( u2, v1 );glVertex3v16( wid,    0, 0 );
			gxTexCoord2i( u2, v2 );glVertex3v16( wid,  hei, 0 );
			gxTexCoord2i( u1, v2 );glVertex3v16(   0,  hei, 0 );
		glEnd();
	glPopMatrix(1);
	
	
}


void glSpriteRotate3D( s32 x, s32 y, s32 z, s32 angle, int flipmode, const glImage *spr )
{
	
	v16 wid = (spr->width * TILE_SIZE_3D) / TILE_SIZE;
	v16 hei = (spr->height * TILE_SIZE_3D) / TILE_SIZE;
	
	v16 s_half_x = wid/2;
	v16	s_half_y = hei/2;
	
	s32 x1 =  -s_half_x;
	s32 y1 =  -s_half_y;
	
	s32 x2 =  s_half_x;
	s32 y2 =  s_half_y;
	
	
	int	u1 = spr->u_off + (( flipmode & GL_FLIP_H ) ? spr->width-1  : 0);
 	int	u2 = spr->u_off + (( flipmode & GL_FLIP_H ) ? 0		    	: spr->width-1);
	int v1 = spr->v_off + (( flipmode & GL_FLIP_V ) ? spr->height-1 : 0);
 	int v2 = spr->v_off + (( flipmode & GL_FLIP_V ) ? 0		    	: spr->height-1);
 	
    
	glPushMatrix();
	
		glTranslate3f32( x, y, z );
		glRotateZi( angle );
		
		
		glBegin( GL_QUADS );
			
			gxTexCoord2i( u1, v1 ); glVertex3v16( x1, y1,0 );	
			gxTexCoord2i( u1, v2 ); glVertex3v16( x1, y2,0 );
			gxTexCoord2i( u2, v2 ); glVertex3v16( x2, y2,0 );
			gxTexCoord2i( u2, v1 ); glVertex3v16( x2, y1,0 );
			
		glEnd();
	
	glPopMatrix( 1 );
		

}

void glSpriteRotateScale3D( s32 x, s32 y, s32 z, s32 angle, s32 scaleX, s32 scaleY, int flipmode, const glImage *spr )
{
	
	v16 wid = (spr->width * TILE_SIZE_3D) / TILE_SIZE;
	v16 hei = (spr->height * TILE_SIZE_3D) / TILE_SIZE;
	
	v16 s_half_x = wid/2;
	v16	s_half_y = hei/2;
	
	s32 x1 =  -s_half_x;
	s32 y1 =  -s_half_y;
	
	s32 x2 =  s_half_x;
	s32 y2 =  s_half_y;
	
	
	int	u1 = spr->u_off + (( flipmode & GL_FLIP_H ) ? spr->width-1  : 0);
 	int	u2 = spr->u_off + (( flipmode & GL_FLIP_H ) ? 0		    	: spr->width-1);
	int v1 = spr->v_off + (( flipmode & GL_FLIP_V ) ? spr->height-1 : 0);
 	int v2 = spr->v_off + (( flipmode & GL_FLIP_V ) ? 0		    	: spr->height-1);
 	
    
	glPushMatrix();
	
		glTranslate3f32( x, y, z );
		glRotateZi( angle );
		glScalef32( scaleX, scaleY, 1 << 12 );
		
		
		glBegin( GL_QUADS );
			
			gxTexCoord2i( u1, v1 ); glVertex3v16( x1, y1,0 );	
			gxTexCoord2i( u1, v2 ); glVertex3v16( x1, y2,0 );
			gxTexCoord2i( u2, v2 ); glVertex3v16( x2, y2,0 );
			gxTexCoord2i( u2, v1 ); glVertex3v16( x2, y1,0 );
			
		glEnd();
	
	glPopMatrix( 1 );
		

}

void glQuad3D( s32 x, s32 y, s32 z, v16 wid, v16 hei, int flipmode, const glImage *spr )
{
	

	int	u1 = spr->u_off + (( flipmode & GL_FLIP_H ) ? spr->width-1  : 0);
 	int	u2 = spr->u_off + (( flipmode & GL_FLIP_H ) ? 0			    : spr->width);
	int v1 = spr->v_off + (( flipmode & GL_FLIP_V ) ? spr->height-1 : 0);
 	int v2 = spr->v_off + (( flipmode & GL_FLIP_V ) ? 0 		    : spr->height);
	
	
	glPushMatrix();
		glTranslatef32(x, y, z);
		glBegin( GL_QUADS );
			gxTexCoord2i( u1, v1 );glVertex3v16(   0,    0, 0 );
			gxTexCoord2i( u2, v1 );glVertex3v16( wid,    0, 0 );
			gxTexCoord2i( u2, v2 );glVertex3v16( wid,  hei, 0 );
			gxTexCoord2i( u1, v2 );glVertex3v16(   0,  hei, 0 );
		glEnd();
	glPopMatrix(1);
	
	
}



void DrawQuad( int poly, int flipmode, const glImage *spr )
{	
	
	u32 f1 = CubeFaces[poly * 4] ;
	u32 f2 = CubeFaces[poly * 4 + 1] ;
	u32 f3 = CubeFaces[poly * 4 + 2] ;
	u32 f4 = CubeFaces[poly * 4 + 3] ;

 
	int	u1 = spr->u_off + (( flipmode & GL_FLIP_H ) ? spr->width-1  : 0);
 	int	u2 = spr->u_off + (( flipmode & GL_FLIP_H ) ? 0			    : spr->width);
	int v1 = spr->v_off + (( flipmode & GL_FLIP_V ) ? spr->height-1 : 0);
 	int v2 = spr->v_off + (( flipmode & GL_FLIP_V ) ? 0 		    : spr->height);
	
	
		
	gxTexCoord2i( u1, v1 );
	glVertex3v16(CubeVectors[f1*3], CubeVectors[f1*3 + 1], CubeVectors[f1*3 +  2] );
	
	gxTexCoord2i( u2, v1 );
	glVertex3v16(CubeVectors[f2*3], CubeVectors[f2*3 + 1], CubeVectors[f2*3 + 2] );
	
	gxTexCoord2i( u2, v2 );
	glVertex3v16(CubeVectors[f3*3], CubeVectors[f3*3 + 1], CubeVectors[f3*3 + 2] );

	gxTexCoord2i( u1, v2 );
	glVertex3v16(CubeVectors[f4*3], CubeVectors[f4*3 + 1], CubeVectors[f4*3 + 2] );

	
}


void DrawQuad( int poly, int flipmode, int TxOff, int TyOff, const glImage *spr )
{	
	
	u32 f1 = CubeFaces[poly * 4] ;
	u32 f2 = CubeFaces[poly * 4 + 1] ;
	u32 f3 = CubeFaces[poly * 4 + 2] ;
	u32 f4 = CubeFaces[poly * 4 + 3] ;

 
	int	u1 = spr->u_off + (( flipmode & GL_FLIP_H ) ? spr->width-1  : 0);
 	int	u2 = spr->u_off + (( flipmode & GL_FLIP_H ) ? 0			    : spr->width);
	int v1 = spr->v_off + (( flipmode & GL_FLIP_V ) ? spr->height-1 : 0);
 	int v2 = spr->v_off + (( flipmode & GL_FLIP_V ) ? 0 		    : spr->height);
	
	u1 +=  TxOff;
	v1 +=  TyOff;
	u2 +=  TxOff;
	v2 +=  TyOff;
	
		
	gxTexCoord2i( u1, v1 );
	glVertex3v16(CubeVectors[f1*3], CubeVectors[f1*3 + 1], CubeVectors[f1*3 +  2] );
	
	gxTexCoord2i( u2, v1 );
	glVertex3v16(CubeVectors[f2*3], CubeVectors[f2*3 + 1], CubeVectors[f2*3 + 2] );
	
	gxTexCoord2i( u2, v2 );
	glVertex3v16(CubeVectors[f3*3], CubeVectors[f3*3 + 1], CubeVectors[f3*3 + 2] );

	gxTexCoord2i( u1, v2 );
	glVertex3v16(CubeVectors[f4*3], CubeVectors[f4*3 + 1], CubeVectors[f4*3 + 2] );

	
}


void DrawCube( s32 x, s32 y, s32 z, const glImage *spr  )
{
	glPushMatrix();
		glTranslatef32( x, y, z);
	
		glBegin( GL_QUADS );
		
			DrawQuad( 0, GL_FLIP_V, spr );   // top
			DrawQuad( 1, GL_FLIP_NONE, spr );   // front
			DrawQuad( 2, GL_FLIP_NONE, spr );   // right
			//DrawQuad( 3, GL_FLIP_NONE, spr );   // back
			DrawQuad( 4, GL_FLIP_NONE, spr );   // left
			DrawQuad( 5, GL_FLIP_V, spr );   // bottom
			
		glEnd();
		
	glPopMatrix(1);
	
}


void DrawFrontTopCube( s32 x, s32 y, s32 z, int TxOff, int TyOff, const glImage *spr  )
{

	glPushMatrix();
		glTranslatef32( x, y, z);
	
		glBegin( GL_QUADS );
		
			DrawQuad( 0, GL_FLIP_NONE, TxOff, TyOff, spr );   // top
			DrawQuad( 1, GL_FLIP_NONE, TxOff, TyOff, spr );   // front

		glEnd();
		
	glPopMatrix(1);
	
}

void DrawFrontCube( s32 x, s32 y, s32 z, int TxOff, int TyOff, const glImage *spr  )
{
	glPushMatrix();
		glTranslatef32( x, y, z);
	
		glBegin( GL_QUADS );
		
			DrawQuad( 1, GL_FLIP_NONE, TxOff, TyOff, spr );   // front

		glEnd();
		
	glPopMatrix(1);
	
}

void DrawFrontCube3x( s32 x, s32 y, s32 z, int TxOff, int TyOff, const glImage *spr  )
{
	glPushMatrix();

		glBegin( GL_QUADS );
			glTranslatef32( x, y, z);
			DrawQuad( 1, GL_FLIP_NONE, TxOff, TyOff, spr );   // front
			glTranslatef32( x, y, z - floattof32(0.5) );
			DrawQuad( 1, GL_FLIP_NONE, TxOff, TyOff, spr );   // front
			glTranslatef32( x, y, z + floattof32(0.5) );
			DrawQuad( 1, GL_FLIP_NONE, TxOff, TyOff, spr );   // front
			
		glEnd();
		
	glPopMatrix(1);
	
}
