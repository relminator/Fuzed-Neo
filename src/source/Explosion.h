/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <gl2d.h>
#include <vector>

#include "UTIL.h"
#include "Sound.h"
#include "cearn_atan.h"
#include "Renderer3D.h"

#ifndef EXPLOSION_H
#define EXPLOSION_H


class Explode
{
public:
	friend class Explosion;

	enum TYPE   //DEATH ID
	{
		TINY = 0,
		SMALL_1,
		SMALL_2,
		SMALL_3,
		SMALL_4,
		MEDIUM_1,
		MEDIUM_2,
		MEDIUM_3,
		MEDIUM_4,
		MEDIUM_5
	};


	Explode()
	{
		Frame = 0;
		BaseFrame = 0;
		NumFrames = 1;
		Counter = 0;
		Active = false;
		x = -1024 << 12;
		y = -1024 << 12;
		Dx = 0;
		Dy = 0;
	}
	
	~Explode()
	{
		
	}
	
	void Init( s32 _x, s32 _y, s32 _dx, s32 _dy, int StartFrame, int Numframe )
	{
		
		x = _x;
		y = _y;
		Dx = _dx;
		Dy = _dy;
		
		Frame = 0;
		BaseFrame = StartFrame;
		NumFrames = Numframe;
		
		Counter = 0;
		Active = true;
		
	}
	
	void Update()
	{
		
		
		x += Dx;
		y += Dy;
		
		if( (Counter & 3) == 0 )
		{
			Frame++;
			if( Frame >= NumFrames ) Active = false;
		}
		
		Counter++;
		
	}
	
	
private:
	s32		x;
	s32		y;
	s32		Dx;
	s32		Dy;
	int		Counter;
	int		Frame;
	int		NumFrames;
	int		BaseFrame;
	bool	Active;
};


//------------------------------------------------------------------------------
//	Explosion Container
//------------------------------------------------------------------------------

class Explosion 
{

	static Explosion* ms_instance;

public:
	
	static Explosion* Instance();
	static void Release();

	void Init( int Palette, glImage* const Sprites );
	void Update();
	void Draw( s32 z );
	void Spawn( s32 _x, s32 _y, s32 _dx, s32 _dy, Explode::TYPE ID );
	void Spawn( s32 _x, s32 _y, s32 _dx, s32 _dy, Explode::TYPE ID_1, Explode::TYPE ID_2 );
	void MegaSpawn( s32 cx, s32 cy, s32 radius, Explode::TYPE min, Explode::TYPE max, int cooldowncounter ); 
	void MegaExplode(); 
	
	void KillEntities();
	int NumExplosions() 
	{
		return numExplosions;
	}
private:
	Explosion();
	~Explosion();

	int numExplosions;
	
	glImage *Images;
	int ColorTable;
	
	// mega explosion
	int Counter;
	int CoolDownCounter;
	s32 Radius;
	int MinID;
	int MaxID;
	s32 CenterX;
	s32 CenterY;
	
	std::vector<Explode>  Explosions;
};




#endif // EXPLOSION_H
