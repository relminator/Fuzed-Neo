/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include "Vector2df32.h"
#include "UTIL.h"


#ifndef GLOBALCONSTANTS_H
#define GLOBALCONSTANTS_H

#define TILE_SIZE 32       // 2D tilesize not used for 3d collisions only map drawing and other stuff
#define TILE_SIZE_3D (1 << 12)  // Size of each tile in 3D

#define ONE 1

const s32 GRAVITY = floattof32( 0.00512 );
const s32 JUMP_HEIGHT = floattof32( 0.098 );
const s32 FALL_SPEED_THRESHOLD = floattof32( 0.9 );
const s32 ACCEL = floattof32(0.0072);
const s32 X_FRICTION = floattof32(0.016);
const s32 Y_FRICTION = floattof32(0.018);

enum 
{
	LEFT = -1,
	RIGHT = 1
};

#endif // GLOBALCONSTANTS_H
