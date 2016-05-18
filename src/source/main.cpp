/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/
 
#include "Game.h"

Game Engine;
	
int main( int argc, char *argv[] ) 
{	
	
	Engine.InitHardware();
	Engine.InitEverything();
	Engine.LoadSounds();
	Engine.LoadImages();
	Engine.InitEntities();
	
	TextMap::OptimizeMaps();
	//Engine.SetStage( TextMap::Maps.size() - 1 );
	//Engine.SetStage( 6 );
				
	while(1)
	{
		
		Engine.RestartLevel( TextMap::Maps[Engine.GetStage()] );
		Engine.SetStageDone(false);
		
		while( !Engine.IsStageDone() ) 
		{
			
			Engine.Update();
			Engine.Draw();
		}
	}

	return 0;


}//end main 


