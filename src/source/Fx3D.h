/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <math.h>
#include <gl2d.h>

#include "GlobalManager.h"
#include "UTIL.h"
#include "VectorSpring.h"

#include "cearn_atan.h"

#ifndef FX3D_H
#define FX3D_H


class Fx3D {

	static Fx3D* ms_instance;

public:
	static Fx3D* Instance();
	static void Release();

	void Fade( int Color1,    // Color to fade to 
		       int Color2,
			   int Color3,
			   int Color4,
			   s32 &Interp,
		       s32 Speed );
	
	void WarpScreen( int Frame, const glImage *sprite );
	void WarpScreenPolar( int Frame, const glImage *sprite );
	void RubberScreen( int Frame, const glImage *sprite );
	
private:
	Fx3D();
	~Fx3D();
	
	
};

#endif // FX3D_H
