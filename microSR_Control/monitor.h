#pragma once
#include "Serial.h"

int monitor(int status)
{
	if (!findTeensy(portName)) return 0;

	if (status)
	{
		comOpen();

	}
	else
	{
		comClose();

	}
	return 1;
}
