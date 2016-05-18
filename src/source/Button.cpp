/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "Button.h"

Button::Button()
{
	 
	KeyReMap[CONTROL_UP] = KEY_UP;
	KeyReMap[CONTROL_DOWN] = KEY_DOWN;
	KeyReMap[CONTROL_LEFT] = KEY_LEFT;
	KeyReMap[CONTROL_RIGHT] = KEY_RIGHT;
	KeyReMap[CONTROL_JUMP] = KEY_A;
	KeyReMap[CONTROL_ATTACK] = KEY_B;
	KeyReMap[CONTROL_MAGIC] = KEY_X;
	KeyReMap[CONTROL_SUICIDE] = KEY_SELECT;
	
	KeyArray = new u8[BUTTON_MAX_FRAME];
	
	Clear();
	
}

Button::~Button()
{
	
	delete[] KeyArray;
}


void Button::Clear()
{
	Index = 0;
	for( int i = 0; i < BUTTON_MAX_FRAME; i++)
	{
		KeyArray[i] = 0;
	}
}

void Button::Save( const char *FileName )
{
	FILE *ReplayFile;
	ReplayFile = fopen( FileName, "wb" );
	
	if( ReplayFile )
	{
		fwrite( KeyArray, sizeof(u8) * BUTTON_MAX_FRAME, 1, ReplayFile );
		fclose( ReplayFile );
	} 
	
}

void Button::Load( const char *FileName )
{
	Clear();
	FILE *ReplayFile;
	ReplayFile = fopen( FileName, "rb" );
	
	if( ReplayFile )
	{
		fread( KeyArray, sizeof(u8) * BUTTON_MAX_FRAME, 1, ReplayFile );
		fclose( ReplayFile );
	} 
	
	Reset();
	
}

void Button::Load( const u8 *BinFile )
{
	memcpy( KeyArray, BinFile, sizeof(u8) * BUTTON_MAX_FRAME  );
}
