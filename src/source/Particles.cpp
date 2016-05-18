/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Particles.h"

Particle* Particle::ms_instance = 0;

Particle::Particle()
{
}

Particle::~Particle()
{
}

Particle* Particle::Instance()
{
	if(ms_instance == 0){
		ms_instance = new Particle();
	}
	return ms_instance;
}

void Particle::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}

//--------------------------------------
//	
//--------------------------------------
void Particle::Init( glImage* const Sprites )
{
	
	Images = Sprites;
	Particles.resize( 128 );
	
}

//--------------------------------------
//	
//--------------------------------------
void Particle::Update()
{
	
	NumParticles = 0;
	std::vector<Part>::iterator iter;
	for( iter = Particles.begin(); iter != Particles.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->Update();
			NumParticles++;
		}
	}
	
	//iprintf( "\x1b[16;1HNumparts = %i         ", NumParticles );
	
	
}

//--------------------------------------
//	
//--------------------------------------
void Particle::Draw( s32 z)
{
	
	glPolyFmt( POLY_ALPHA(15) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
	std::vector<Part>::iterator iter;
	for( iter = Particles.begin(); iter != Particles.end(); ++iter )
	{
		if( iter->Active )
		{
			s32 x = iter->Position.x;
			s32 y = iter->Position.y;
			glColor( RGB15( iter-> Red,iter->Green, iter->Blue ) );
			glSpriteRotateScale3D( x, y, z, iter->Angle, 1900, 550, GL_FLIP_NONE, &Images[1] );
			z += 4;
		}
	}
	
	glColor( RGB15(31,31,31) );
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
		
}

//--------------------------------------
//	
//--------------------------------------
void Particle::Spawn( Vector2df32 Pos, Vector2df32 Dir, Part::TYPE Type )
{
	
	
	std::vector<Part>::iterator iter;
	for( iter = Particles.begin(); iter != Particles.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Init( Pos , Dir, Type );
			break;
		
		}
	}
	
}

//--------------------------------------
//	
//--------------------------------------
void Particle::Spawn( Vector2df32 Pos, s32 Ang, Part::TYPE Type )
{
	
	
	std::vector<Part>::iterator iter;
	for( iter = Particles.begin(); iter != Particles.end(); ++iter )
	{
		if( !iter->Active )		// insert a bullet on a free slot
		{

			iter->Init( Pos , Ang, Type );
			break;
		
		}
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void Particle::Spawn( Vector2df32 Pos, int NumParts )
{
	
	
	for( int i = 0; i < NumParts; i++ )
	{
	
		std::vector<Part>::iterator iter;
		for( iter = Particles.begin(); iter != Particles.end(); ++iter )
		{
			if( !iter->Active )		// insert a bullet on a free slot
			{
				s32 angle = UTIL::RND();
			
				iter->Init( Pos , angle, (Part::TYPE) (UTIL::RND() & 3) );
				break;
			
			}
		}
	
	}
	
}


//--------------------------------------
//	
//--------------------------------------
void Particle::KillAll()
{
	
	std::vector<Part>::iterator iter;
	for( iter = Particles.begin(); iter != Particles.end(); ++iter )
	{
		if( iter->Active )
		{
			iter->Active = false;
		}
	}
	
	
}
