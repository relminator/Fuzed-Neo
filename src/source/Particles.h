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
#include "vector2df32.h"
#include "Renderer3D.h"
#include "GlobalManager.h"

#ifndef PARTICLE_H
#define PARTICLE_H

class Part
{
public:
	friend class Particle;
	
	enum TYPE   //DEATH ID
	{
		TINY = 0,
		MEDIUM,
		LARGE,
		HUGE
	};
	
	Part()
	{
		Active = false;
		Speed = 3 << 12;		// 
		Life = 60;				// 1 second 
		Red = 31;
		Green = 31;
		Blue = 31;
		ID = TINY;
	}
	
	~Part()
	{
		
	}
	
	void Init( Vector2df32 Pos, Vector2df32 Dir, TYPE Type )
	{
	
		Active = true;
		Speed = floattof32(0.035) + (UTIL::RND() & 255);		
		
		
		Position.x = Pos.x;
		Position.y = Pos.y;
		
		Direction.x = Dir.x;
		Direction.y = Dir.y;
		
		ID = Type;
		Life = 60 + ID * 60;		 
		Angle = atan2Lookup( Direction.x, Direction.y);
		
		switch( ID )
		{
			case TINY:
				Red = 31;
				Green = 31;
				Blue = UTIL::RND() & 31;
				break;
			case MEDIUM:
				Red = 31;
				Green = UTIL::RND() & 31;
				Blue = UTIL::RND() & 31;
				break;
			case LARGE:
				Red = UTIL::RND() & 31;
				Green = 31;
				Blue = UTIL::RND() & 31;
				break;
			case HUGE:
				Red = UTIL::RND() & 31;
				Green = UTIL::RND() & 31;
				Blue = 31;
				break;
			default:
				Red =  31;
				Green = 31;
				Blue = UTIL::RND() & 31;
				
		}
			
	}
	
	void Init( Vector2df32 Pos, s32 Ang, TYPE Type )
	{
	
		Active = true;
		Speed = floattof32(0.035) + (UTIL::RND() & 255);		
		Position.x = Pos.x;
		Position.y = Pos.y;
		
		Angle = Ang;
		
		Direction.x = cosLerp(Angle);
		Direction.y = sinLerp(Angle);
		
		ID = Type;
		Life = 30 + (ID >> 1) * 10;		 
		
		switch( ID )
		{
			case TINY:
				Red = 31;
				Green = 31;
				Blue = UTIL::RND() & 31;
				break;
			case MEDIUM:
				Red = 31;
				Green = UTIL::RND() & 31;
				Blue = UTIL::RND() & 31;
				break;
			case LARGE:
				Red = UTIL::RND() & 31;
				Green = 31;
				Blue = UTIL::RND() & 31;
				break;
			case HUGE:
				Red = UTIL::RND() & 31;
				Green = UTIL::RND() & 31;
				Blue = 31;
				break;
			default:
				Red = UTIL::RND() & 31;
				Green = UTIL::RND() & 31;
				Blue = UTIL::RND() & 31;
				
		}
			
	}
	
	void Init( s32 sx, s32 sy, int life )
	{
	
		Active = true;
		Speed = 0;		
		
		
		Position.x = sx;
		Position.y = sy;
		
		Direction.x = 0;
		Direction.y = 0;
		
		ID = TINY;
		Life = life;		 
		Angle = 0;
	
	}
	
	void Update()
	{
		
		Position.x = Position.x + ((Direction.x * Speed) >> 12);
		Position.y = Position.y + ((Direction.y * Speed) >> 12);
	
		Life--;
		
		if( Life <= 0 )
		{
			Active = false;
		}
			
	}
	
	const s32 &GetX() const 
	{
		return Position.x;
	}
	
	const s32 &GetY() const 
	{
		return Position.y;
	}
	
	const bool &IsActive() const 
	{
		return Active;
	}
	
	
private:
	
	bool	Active;
	s32		Speed;
	s32 	Angle;
	int		Life;
	int		Red;
	int		Green;
	int		Blue;
	
	Vector2df32 Position;
	Vector2df32 Direction;
	
	
	TYPE	ID;
	

};

class Particle 
{

	static Particle* ms_instance;

public:
	static Particle* Instance();
	static void Release();

	void Init( glImage* const Sprites );
	void Update();
	void Draw( s32 z );
	void Spawn( Vector2df32 Pos, Vector2df32 Dir, Part::TYPE ID );
	void Spawn( Vector2df32 Pos, s32 Ang, Part::TYPE ID );
	void Spawn( Vector2df32 Pos, int NumParts );
	void KillAll();
	
private:
	Particle();
	~Particle();

	int NumParticles;
	
	glImage *Images;
	
	std::vector<Part>  Particles;
	
};

#endif // PARTICLE_H
