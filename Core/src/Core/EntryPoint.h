#pragma once

#define ENABLE_CUP_ASSERTS
#define ENABLE_CUP_LOGGING

#include "CupEngine.h"

extern Cup::CupEngine* Cup::CreateApplication();

int main()
{
	auto demo = Cup::CreateApplication();
	if (demo->Construct(256, 240, 4, 4))
		demo->Start();

	return 0;
}