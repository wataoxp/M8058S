/*
 * delay.c
 *
 *  Created on: Jan 6, 2025
 *      Author: wataoxp
 */
#include "delay.h"

CoreClock tDelay::ClockSource = HSICLOCK;
TIM* TimPolicy::pTim = nullptr;

uint32_t tDelay::Init(CoreClock source)
{
	if(!source)
	{
		return 1;
	}
	ClockSource = source;

	InitMillTick();
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

	return 0;
}

void tDelay::Delay(uint32_t nTime)
{
	__IO uint32_t tmp = SysTick->CTRL;
	__IO uint32_t mDelay = nTime;
	((void)tmp);

	while(mDelay)
	{
		if (LL_SYSTICK_IsActiveCounterFlag() != 0)
		{
			mDelay--;
		}
	}
}

void tDelay::uDelay(uint32_t nTime)
{
	// 1usのクロック数*待機us=消費クロック数
    uint32_t Ticks = nTime * ClockSource;
    uint32_t Start = SysTick->VAL;
    uint32_t Elapsed = 0;	// 経過クロック数

    uint32_t Prev = Start;
    uint32_t Current;

    while (Elapsed < Ticks)
    {
        Current = SysTick->VAL;

        // ダウンカウントが真なら普通にカウント
        if (Prev >= Current)
        {
            Elapsed += (Prev - Current);
        }
        else	// アンダーフローによってカウンタがLOAD値に戻った場合はLOAD値から引く
        {
            Elapsed += (Prev + (SysTick->LOAD - Current) + 1);
        }
        Prev = Current;
    }
}



