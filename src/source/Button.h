/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BUTTON_H
#define BUTTON_H


class Button 
{

public:
	
	enum
	{
		BUTTON_MAX_FRAME = 16384
	};
	
	enum E_BUTTON_FLAGS
	{
		BUTTON_UP = (1 << 0),
		BUTTON_DOWN = (1 << 1),
		BUTTON_LEFT = (1 << 2),
		BUTTON_RIGHT = (1 << 3),
		BUTTON_JUMP = (1 << 4),
		BUTTON_ATTACK = (1 << 5),
		BUTTON_MAGIC = (1 << 6),
		BUTTON_SUICIDE = (1 << 7)
	};

	enum E_BUTTON_CONTROLS
	{
		CONTROL_UP = 0,
		CONTROL_DOWN,
		CONTROL_LEFT,
		CONTROL_RIGHT,
		CONTROL_JUMP,
		CONTROL_ATTACK,
		CONTROL_MAGIC,
		CONTROL_SUICIDE
	};
	
	Button();
	~Button();

	void Save( const char *FileName );
	void Load( const char *FileName );
	void Load( const u8 *BinFile );
	
	void Clear();

	void Reset()
	{
		Index = 0;
	}
	
	inline void Push( u8 key )
	{
		KeyArray[Index] = key;
		IncFrame();
	}
	
	inline u8 Pop()
	{
		u8 Key =  KeyArray[Index];
		IncFrame();
		return Key;
	}
	
	inline u8 Key()
	{
		u8 Key =  KeyArray[Index];
		IncFrame();
		return Key;
	}
	
	inline u8 Key( int index )
	{
		index = (index + 1) & (BUTTON_MAX_FRAME - 1);
		return KeyArray[index];
	}
	
	int KeyMap( E_BUTTON_CONTROLS KeyVal )
	{
		return KeyReMap[KeyVal];
	}
	
private:


	inline void IncFrame()
	{
		Index = (Index + 1) & (BUTTON_MAX_FRAME - 1);
	}
	
	int Index;
	int KeyReMap[8];

	u8 *KeyArray;
	
};

#endif // BUTTON_H
