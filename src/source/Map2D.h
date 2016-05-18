/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <stdio.h>
#include "GlobalManager.h"
#include "string.h"
#include "Renderer3D.h"
#include "GlobalConstants.h"


#ifndef MAP2D_H
#define MAP2D_H

enum TILETYPE   //Most common
{
	LADDER = -127,
	TOP_LADDER,
	TOP_LADDER_HANDLE,
	WATER,
	TOP_WATER,
	SIGN,
	MOSS_BOTTOM,
	BLOCK_COLLIDE,
	
	NONE = 0,
	
	BRICK,
	TOP_BRICK,
	TOP_BRICK_MOSS_TOP,
	BRICK_MOSS_BOTTOM,
	SOFT_BRICK,
	SPIKE,
	
	INVISIBLE
	
};


inline char Mid( const char *text, int pos )
{
	return text[pos];	
}

inline int GetTextMapHeight( const char *TextMap[] )
{
	int hi = 0;
	char a = 'a';
	while( a != 'Q' )
	{
		a = Mid( TextMap[hi], 0 );
		hi++;		
	}
	return hi-1;
}

class Map2D 
{

public:
	Map2D();
	Map2D( int Wid, int Hei );
	~Map2D();
	
	
	inline s8 &operator() ( int x, int y ) const
	{
		return Elements[x + y * Width];
	}
	
	inline s8 GetElement( int x, int y ) const
	{
		return Elements[x + y * Width];
	}
	
	inline int GetWidth() const
	{
		return Width;
	}
	
	inline int GetHeight() const
	{
		return Height;
	}
	
	inline int GetNumElements() const
	{
		return NumElements;
	}
	
	inline s8 Collide( s32 x, s32 y)
	{
		int TileX = x / TILE_SIZE_3D;
		int TileY = y / TILE_SIZE_3D;
		return GetElement(TileX,TileY);
	}
	
	
	inline void SetElement( int x, int y, u8 v )
	{
		Elements[x + y * Width] = v;
	}
	
	
	void Convert( const char *TextMap[] );
	void Load( const char *Filename );
	
	
	void Clear();
		
	void PrintMap();
	
	void Draw( const glImage *Tiles );
	void DrawTranslucent( const glImage *Tiles );
	void DrawBG( int PlayerX, int PlayerY, s32 z, const glImage *BGTiles );
	void DrawDebug( const glImage *Tiles );
	void ClearBlockCollision();
	
private:
	
	
	int Width;
	int Height;
	int NumElements;
	s8 *Elements;
	
	int Xpadding;
	int Ypadding;
	
	s32 TxOff;
	s32 TyOff;
	
	
};


class MapContainer
{
	
public:

	MapContainer();
	~MapContainer();
	
	void RestoreCollisionTiles();
	
	
	Map2D Base;
	Map2D Collision;
	
};

#endif // MAP2D_H
