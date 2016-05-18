/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <stdio.h>

#include "gl2d.h"
#include "GlobalConstants.h"

#ifndef RENDERER3D_H
#define RENDERER3D_H

void glBox3D( s32 x, s32 y, s32 z, int color );
void glBox3Dv16( s32 x1, s32 y1, s32 x2, s32 y2, s32 z, int color );
void glBoxFilled3Dv16( s32 x1, s32 y1, s32 x2, s32 y2, s32 z, int color );
void glSprite3D( s32 x, s32 y, s32 z, int flipmode, const glImage *spr );
void glSpriteRotate3D( s32 x, s32 y, s32 z, s32 angle, int flipmode, const glImage *spr );
void glSpriteRotateScale3D( s32 x, s32 y, s32 z, s32 angle, s32 scaleX, s32 scaleY, int flipmode, const glImage *spr );
void glQuad3D( s32 x, s32 y, s32 z, v16 wid, v16 hei, int flipmode, const glImage *spr );


void DrawQuad( int poly, int flipmode, const glImage *spr );
void DrawQuad( int poly, int flipmode, int TxOff, int TyOff , const glImage *spr );
void DrawCube( s32 x, s32 y, s32 z, const glImage *spr );
void DrawFrontTopCube( s32 x, s32 y, s32 z, int TxOff, int TyOff, const glImage *spr  );
void DrawFrontCube( s32 x, s32 y, s32 z, int TxOff, int TyOff, const glImage *spr  );
void DrawFrontCube3x( s32 x, s32 y, s32 z, int TxOff, int TyOff, const glImage *spr  );



#endif // RENDERER3D_H
