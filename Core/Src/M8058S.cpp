/*
 * M8058S.cpp
 *
 *  Created on: Apr 4, 2026
 *      Author: wataoxp
 */

#include "M8058S.h"
#include <stdlib.h>

using namespace MelodyCommands;

M8058S::M8058S(GPIO_TypeDef* GPIO,uint32_t pin,pDelay udelay) : GPIOx(GPIO),Pin(pin),microdelay(udelay)
{
	;
}

void M8058S::SendCommand(uint8_t data)
{
	GPIO_CLEAR(GPIOx,Pin);
	GPIO_WRITE(GPIOx,Pin);
	microdelay(6000);

	uint8_t bit;

	// Bit=1-> Low500:High1500
	// Bit=0-> Low1500:High500
	for(uint8_t mask = 1U;mask;mask <<= 1)
	{
		bit = data&mask;

		GPIO_CLEAR(GPIOx,Pin);
		microdelay((bit)? 500:1500);
		GPIO_WRITE(GPIOx,Pin);
		microdelay((bit)? 1500:500);

	}
	GPIO_CLEAR(GPIOx,Pin);
	microdelay(200);
}

void M8058S::Init(void)
{
	GPIO_CLEAR(GPIOx,Pin);

	SendCommand(MaxVolume() + VolumeLevelBase);
	SendCommand(MelodyON_LedOFF);
}

uint8_t M8058S::SetMelody(uint8_t num)
{
	if(num > MaxMelodyNum())
	{
		return OverFlow;
	}

	SendCommand(MelodyNumBase + num);

	return Success;
}

uint8_t M8058S::SelVolume(uint8_t vol)
{
	if(vol > MaxVolume())
	{
		return OverFlow;
	}

	SendCommand(VolumeLevelBase + vol);

	return Success;
}

// 16進数は10進数に直してから入力
void M8058S::SerialControl(UART& uart)
{
	char string[10];
	uint8_t command = 0;
	uint8_t num = 0;

	while(command != '\r')
	{
		command = uart.ReceiveData();
		string[num] = command;
		num++;
	}
	string[num] = '\n';

	uart.TransmitData((uint8_t*)string, num);

	command = atoi(string);

	if(command > MaxMelodyNum())
	{
		SendCommand(command);
	}
	else
	{
		SetMelody(command);
	}
}


