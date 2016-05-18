/******************************************************************************
*******************************************************************************

	Fuzed DS
	relminator
	http://rel.phatcode.net


*******************************************************************************
******************************************************************************/

#include "GlobalManager.h"

GlobalManager* GlobalManager::ms_instance = 0;

GlobalManager::GlobalManager()
{
}

GlobalManager::~GlobalManager()
{
}

GlobalManager* GlobalManager::Instance()
{
	if(ms_instance == 0){
		ms_instance = new GlobalManager();
	}
	return ms_instance;
}

void GlobalManager::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}

