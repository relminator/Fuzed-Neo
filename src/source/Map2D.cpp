/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Map2D.h"


Map2D::Map2D()
{
	
	Elements = 0;
	Width = 0;
	Height = 0;
	NumElements = Width * Height;
	
	Clear();
	
	Xpadding = 5;
	Ypadding = 5;
	
	TxOff = 0;
	TyOff = 0;
	
}

Map2D::Map2D( int Wid, int Hei )
{
	Elements = new s8[Wid * Hei];
	Width = Wid;
	Height = Hei;
	NumElements = Width * Height;
	
	Clear();
	
	Xpadding = 5;
	Ypadding = 5;
	
	TxOff = 0;
	TyOff = 0;
	
}

Map2D::~Map2D()
{

	if( Elements ) delete[] Elements;
	Elements = 0;

}

void Map2D::Clear()
{
	memset(  Elements, 0, sizeof(u8) * NumElements );
}

void Map2D::Convert( const char *TextMap[] )
{
	if( Elements ) delete[] Elements;
	Elements = 0;
	
	Width = strlen( TextMap[0] );
	Height = GetTextMapHeight( TextMap );
	NumElements = Width * Height;
	
	Elements = new s8[NumElements];
	Clear();
	
	
	for( int y = 0; y < Height; y++ )
	{
		for( int x = 0; x < Width; x++ )
		{
			char a = Mid( TextMap[y], x );
			switch( a )
			{
				case '+':
					SetElement( x, y, BRICK );
					break;
				case '#':
					SetElement( x, y, TOP_BRICK );
					break;
				case '|':
					SetElement( x, y, TOP_BRICK_MOSS_TOP );
					break;
				case '!':
					SetElement( x, y, BRICK_MOSS_BOTTOM );
					break;
				case '*':
					SetElement( x, y, SOFT_BRICK );
					break;
				case '^':
					SetElement( x, y, SPIKE );
					break;
				case '$':
					SetElement( x, y, LADDER );
					break;
				case 'T':
					SetElement( x, y, TOP_LADDER );
					break;
				case 't':
					SetElement( x, y, TOP_LADDER_HANDLE );
					break;
				case '~':
					SetElement( x, y, TOP_WATER );
					break;
				case 'w':
					SetElement( x, y, WATER );
					break;
				case '?':
					SetElement( x, y, SIGN );
					break;
				case '/':
					SetElement( x, y, MOSS_BOTTOM );
					break;
				
			}
			
		}
	}
	
}
/*
void Map2D::ReadEnemies( const char *TextMap[], Entity EnemyArray[]  )
{
	if( Elements ) delete[] Elements;
	Elements = 0;
	
	Width = strlen( TextMap[0] );
	Height = GetTextMapHeight( TextMap );
	NumElements = Width * Height;
	
	Elements = new s8[NumElements];
	Clear();
	
	//H = Horizontal Spiral
	//V = Vertical Spiral
	//R = Red Turtle
	//G = Green Turtle
	//B = Bouncer
	
	for( int y = 0; y < Height; y++ )
	{
		for( int x = 0; x < Width; x++ )
		{
			char a = Mid( TextMap[y], x );
			switch( a )
			{
				case '#':
					SetElement( x, y, BRICK );
					break;
				case '$':
					SetElement( x, y, LADDER );
					break;
				case 'T':
					SetElement( x, y, TOP_LADDER );
					break;
			}
			
		}
	}
	
}

*/

void Map2D::Load( const char *Filename )
{
	if( Elements ) delete[] Elements;
	Elements = 0;
	
	char LineInput[1024];
	int LineCounter = 0;
	int MaxLine = 0;
	
	// Read dimensions
	FILE *File = fopen ( Filename, "r" );
	if ( File != NULL )
	{
		while ( fgets ( LineInput, sizeof(LineInput), File ) != NULL ) 
		{
			LineCounter++;
			if( LineCounter == 1 ) Width = strlen(LineInput)  - 1;
			if( Mid( LineInput, 0 ) == 'Q' )  MaxLine = LineCounter;
		}
		fclose ( File );
	}
	else
	{
		perror ( Filename ); 
	}

	
	Height = MaxLine - 1;
	NumElements = Width * Height;
	
	Elements = new s8[NumElements];
	Clear();
	
	// Load
	LineCounter = 0;
	File = fopen ( Filename, "r" );
	if ( File != NULL )
	{
		while ( fgets ( LineInput, sizeof(LineInput), File ) != NULL ) 
		{
			if( LineCounter < Height )
			{
				for( int x = 0; x < Width; x++ )
				{
					char a = Mid( LineInput, x );
					switch( a )
					{
						case '#':
							SetElement( x, LineCounter, BRICK );
						break;
					}
					
				}
			}
			LineCounter++;
			
		}
		fclose ( File );
	}
	else
	{
		perror ( Filename ); 
	}


}


void Map2D::PrintMap()
{
	
	for( int y = 0; y < Height; y++ )
	{
		for( int x = 0; x < Width; x++ )
		{
			printf( "%i", GetElement(x,y) );
		}
		printf( "\n");
	}

}


void Map2D::Draw( const glImage *Tiles )
{
	
	
	glPushMatrix();
	
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_FRONT | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
	for( int y = 0; y < Height; y++ )
	{
		for( int x = 0; x < Width ; x++ )
		{
			
			int Tile = GetElement(x,y);
			switch( Tile )
			{
				case BRICK:
					// offset by 0.5 so that (0,0) is top left of tile like standard 2D
					DrawCube( x * TILE_SIZE_3D + floattof32(0.5), y * TILE_SIZE_3D + floattof32(0.5), 0, &Tiles[8] );
					break;
				case TOP_BRICK:
					DrawCube( x * TILE_SIZE_3D + floattof32(0.5), y * TILE_SIZE_3D + floattof32(0.5), 0, &Tiles[0] );
					break;
				case TOP_BRICK_MOSS_TOP:
					DrawCube( x * TILE_SIZE_3D + floattof32(0.5), y * TILE_SIZE_3D + floattof32(0.5), 0, &Tiles[0] );
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,  10 + TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[11] );
					break;
				case BRICK_MOSS_BOTTOM:
					DrawCube( x * TILE_SIZE_3D + floattof32(0.5), y * TILE_SIZE_3D + floattof32(0.5), 0, &Tiles[8] );
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,  10 + TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[3] );
					break;
				case SOFT_BRICK:
					// offset by 0.5 so that (0,0) is top left of tile like standard 2D
					DrawCube( x * TILE_SIZE_3D + floattof32(0.5), y * TILE_SIZE_3D + floattof32(0.5), 0, &Tiles[13] );
					break;
				case SPIKE:
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[2] );
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,               0, GL_FLIP_NONE, &Tiles[2] );
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,  TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[2] );
					break;
				case INVISIBLE:
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[2] );
					break;
				case LADDER:
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -64, GL_FLIP_NONE, &Tiles[1] );
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[1] );
					break;
				case TOP_LADDER:
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -64, GL_FLIP_NONE, &Tiles[1] );
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[1] );
					break;
				case TOP_LADDER_HANDLE:
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -64, GL_FLIP_NONE, &Tiles[9] );
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[9] );
					break;
				case SIGN:
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[10] );
					break;
				case MOSS_BOTTOM:
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, 10 + TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[3] );
					break;
					
			}
			
		}	
	}
	
	glPopMatrix(1);
	
}


void Map2D::DrawTranslucent( const glImage *Tiles )
{
	
	TxOff = (TxOff + 1) & 31;
	glPushMatrix();
	glPolyFmt( POLY_ALPHA(15) | POLY_CULL_FRONT | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
				
	for( int y = 0; y < Height; y++ )
	{
		for( int x = 0; x < Width ; x++ )
		{
			
			int Tile = GetElement(x,y);
			switch( Tile )
			{
				
				case WATER:
					DrawFrontCube( (x * TILE_SIZE_3D) + floattof32(0.5), (y * TILE_SIZE_3D) + floattof32(0.5), 0, 0, -TxOff, &Tiles[0] );
					break;
				case TOP_WATER:
					DrawFrontTopCube( (x * TILE_SIZE_3D) + floattof32(0.5), (y * TILE_SIZE_3D) + floattof32(0.5), 0, 0, -TxOff, &Tiles[0] );
					break;
					
			}
			
		}	
	}
	
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
	glPopMatrix(1);
	
}


void Map2D::DrawBG( int PlayerX, int PlayerY, s32 z, const glImage *BGTiles )
{
	
	glPushMatrix();
	glScalef32( 12 << 12, 12 << 12, 5 << 12);
	
	for( int y = -2; y < 3; y++ )
	{
		for( int x = -2; x < 4; x++ )
		{
			glQuad3D( x * TILE_SIZE_3D, y * TILE_SIZE_3D, z, floattov16(1), floattov16(1), GL_FLIP_NONE, &BGTiles[0] );
		}	
	}
	
	glPopMatrix(1);
	
}





void Map2D::DrawDebug( const glImage *Tiles )
{
	
	
	glPushMatrix();
	
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_FRONT | POLY_ID( GlobalManager::Instance()->GetFreePolyID() ) );
	
	for( int y = 0; y < Height; y++ )
	{
		for( int x = 0; x < Width ; x++ )
		{
			
			int Tile = GetElement(x,y);
			switch( Tile )
			{
				case BRICK:
					// offset by 0.5 so that (0,0) is top left of tile like standard 2D
					//DrawCube( x * TILE_SIZE_3D + floattof32(0.5), y * TILE_SIZE_3D + floattof32(0.5), 0, &Tiles[4] );
					break;
				case TOP_BRICK:
					//DrawCube( x * TILE_SIZE_3D + floattof32(0.5), y * TILE_SIZE_3D + floattof32(0.5), 0, &Tiles[0] );
					break;
				case TOP_BRICK_MOSS_TOP:
					//DrawCube( x * TILE_SIZE_3D + floattof32(0.5), y * TILE_SIZE_3D + floattof32(0.5), 0, &Tiles[0] );
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,  10 + TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[7] );
					break;
				case BRICK_MOSS_BOTTOM:
					//DrawCube( x * TILE_SIZE_3D + floattof32(0.5), y * TILE_SIZE_3D + floattof32(0.5), 0, &Tiles[4] );
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,  10 + TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[6] );
					break;
				case SPIKE:
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[2] );
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,               0, GL_FLIP_NONE, &Tiles[2] );
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,  TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[2] );
					break;
				case INVISIBLE:
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, 8+ TILE_SIZE_3D, GL_FLIP_NONE, &Tiles[2] );
					break;
				case LADDER:
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -64, GL_FLIP_NONE, &Tiles[1] );
					break;
				case TOP_LADDER:
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -64, GL_FLIP_NONE, &Tiles[1] );
					break;
				case TOP_LADDER_HANDLE:
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -64, GL_FLIP_NONE, &Tiles[5] );
					break;
				case SIGN:
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,  TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[3] );
					break;
				case MOSS_BOTTOM:
					//glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D,  10 + TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[6] );
					break;
				case BLOCK_COLLIDE:
					glSprite3D( x * TILE_SIZE_3D, y  * TILE_SIZE_3D, -TILE_SIZE_3D/2, GL_FLIP_NONE, &Tiles[2] );
					break;
					
			}
			
		}	
	}
	
	glPopMatrix(1);
	
}


// Clear invisible tiles set by enemies
// So that blocks can move
void Map2D::ClearBlockCollision()
{
	
	for( int y = 0; y < Height; y++ )
	{
		for( int x = 0; x < Width ; x++ )
		{
			int Tile = GetElement(x,y);
			if( Tile == BLOCK_COLLIDE ) SetElement(x,y, NONE);
		}
	}
	
	
}
	
MapContainer::MapContainer()
{
	
	
}
	
	
MapContainer::~MapContainer()
{
	
}

// Restore non collision tiles like water, ladder, etc
void MapContainer::RestoreCollisionTiles()
{
	
	for( int y = 0; y < Base.GetHeight(); y++ )
	{
		for( int x = 0; x < Base.GetWidth() ; x++ )
		{
			int Tile = Base(x,y);
			if( Tile < BLOCK_COLLIDE ) Collision(x,y) = Tile;
		}
	}
	
}
	