#pragma once

#include "CupEngine.h"

extern Cup::CupEngine* Cup::CreateApplication();

int main()
{
	auto demo = Cup::CreateApplication();
	if (demo->Construct(256, 240, 4, 4))
		demo->Start();

	return 0;
}