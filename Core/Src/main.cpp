#include "mylib.h"
#include "M8058S.h"

int main(void)
{
	constexpr CoreClock ClockSource = HSICLOCK;

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

	RCC_Config(ClockSource, rccStatus::InternalClock);

	using Delay = DelayMode::Standard;
	Delay::Init(ClockSource);

	UART uart(USART1);
	UART_Config(uart, ClockSource, GPIOA, Pin9, LL_GPIO_AF_1, GPIOA, Pin10, LL_GPIO_AF_1);

	GPIO_Config(GPIOA, Pin2, LL_GPIO_MODE_OUTPUT);
	M8058S melody(GPIOA,Pin2,Delay::uDelay);
	melody.Init();

	while(1)
	{
		melody.SerialControl(uart);
	}

}
