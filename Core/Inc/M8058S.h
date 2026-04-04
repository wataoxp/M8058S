/*
 * M8058S.h
 *
 *  Created on: Apr 4, 2026
 *      Author: wataoxp
 */

#ifndef INC_M8058S_H_
#define INC_M8058S_H_

#include "periph.h"
#include "uart.h"

typedef void (*pDelay)(uint32_t);

namespace MelodyCommands{
	constexpr uint8_t MinVolume(){return 0;}
	constexpr uint8_t MaxVolume(){return 7;}
	constexpr uint8_t MinMelodyNum(){return 0;}
	constexpr uint8_t MaxMelodyNum(){return 0x3A;}

	typedef enum :uint8_t{
		MelodyNumBase = 0x00,
		VolumeLevelBase = 0xF0,

		MelodyON_LedON = 0xE8,
		MelodyON_LedOFF = 0xE9,
		MelodyOFF_LedOn = 0xEA,
		MelodyOFF_LedOFF = 0xEB,

		Replay = 0xFB,
		NextMelody = 0xFC,
		PrevMelody = 0xFD,
		SetVolume = 0xFE,
		Stop = 0xFF,
	}Commands;

	typedef enum{
		Success,
		UndefFlow,
		OverFlow,
	}ReturnCodes;

}

class M8058S{
private:
	GPIO_TypeDef* GPIOx;
	uint32_t Pin;
	pDelay microdelay;
	void SendCommand(uint8_t data);
public:
	M8058S(GPIO_TypeDef* GPIO,uint32_t pin,pDelay udelay);
	void Init(void);
	uint8_t SetMelody(uint8_t num);
	uint8_t SelVolume(uint8_t vol);
	void RePlay(void);
	void Stop(void);
	void SerialControl(UART& uart);
};

inline void M8058S::RePlay(void)
{
	SendCommand(MelodyCommands::Replay);
}
inline void M8058S::Stop(void)
{
	SendCommand(MelodyCommands::Stop);
}


#endif /* INC_M8058S_H_ */
