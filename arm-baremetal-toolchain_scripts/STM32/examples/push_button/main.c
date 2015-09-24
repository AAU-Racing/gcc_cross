/**
 * See http://stm32f4-discovery.com/2014/08/stm32f4-external-interrupts-tutorial/
 * for some more info on external interrupts. Though it is based on the older
 * std periphial driver instead of the HAL
 */

#include <stm32f4xx_hal.h>

void EXTI15_10_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(0xFC00); // 0xFC00 is pin 15 to 10
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin & GPIO_PIN_15) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);
	}
}

void SysTick_Handler(void) {
  HAL_IncTick();
}

#if 1
#define IS_ODD(n)	((n) & 1 == 1)
void set_system_clock_168mhz(void) {
	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	const uint32_t HSE_mhz = HSE_VALUE/10e5;

	RCC_OscInitTypeDef RCC_OscInit = {
		.OscillatorType = RCC_OSCILLATORTYPE_HSE,
		.HSEState       = RCC_HSE_ON,
		.PLL.PLLState   = RCC_PLL_ON,
		.PLL.PLLSource  = RCC_PLLSOURCE_HSE,
		.PLL.PLLM       = IS_ODD(HSE_mhz) ? HSE_mhz : (HSE_mhz / 2), //12,//HSE_VALUE / 10e6, //25, // should match HSE freq in mhz so HSE/PLLM == 1
		.PLL.PLLN       = IS_ODD(HSE_mhz) ? (168*2) : 168,//168,//336,//336, // 336 / 2 == 168 == max core clk
		.PLL.PLLP       = RCC_PLLP_DIV2, // ((HSE/pllm) * plln) / pllp == 168 (max core clk)
		.PLL.PLLQ       = 7, // ((HSE/pllm) * plln) / pllq == 48 (USB needs 48 and sdio needs 48 or lower)
	};
	if (HAL_RCC_OscConfig(&RCC_OscInit) != HAL_OK) {
		while (1);
	}

	RCC_ClkInitTypeDef RCC_ClkInit = {
		.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2),
		.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK,
		.AHBCLKDivider  = RCC_SYSCLK_DIV1,
		.APB1CLKDivider = RCC_HCLK_DIV4,
		.APB2CLKDivider = RCC_HCLK_DIV2,
	};
	// We set FLASH_LATENCY_5 as we are in vcc range 2.7-3.6 at 168mhz
	// see datasheet table 10 at page 80.
	if (HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_5) != HAL_OK) {
		while (1);
	}

	  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
	if (HAL_GetREVID() == 0x1001) {
		 // Enable the Flash prefetch
		__HAL_FLASH_PREFETCH_BUFFER_ENABLE();
	}
}
#endif


int main(void) {
	HAL_Init();

	set_system_clock_168mhz();

	// First setup the output io pin
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();

		GPIO_InitTypeDef led_init = {
			.Pin   = GPIO_PIN_4,
			.Mode  = GPIO_MODE_OUTPUT_PP,
			.Pull  = GPIO_PULLUP,
			.Speed = GPIO_SPEED_FAST,
		};

		HAL_GPIO_Init(GPIOC, &led_init);
	}

	// Then the output io pin
	{
		__HAL_RCC_GPIOE_CLK_ENABLE();

		GPIO_InitTypeDef button_init = {
			.Pin  = GPIO_PIN_15,
			.Mode = GPIO_MODE_IT_FALLING,
			.Pull = GPIO_NOPULL,
		};
		HAL_GPIO_Init(GPIOE, &button_init);
		HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	}

	// Output PLL/2 clock on PA8
	{
		GPIO_InitTypeDef led_init = {
			.Pin       = GPIO_PIN_8,
			.Mode      = GPIO_MODE_AF_PP,
			.Pull      = GPIO_PULLUP,
			.Speed     = GPIO_SPEED_HIGH,
			.Alternate = GPIO_AF0_MCO,
		};
		HAL_GPIO_Init(GPIOA, &led_init);
		HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_2);
	}

	while(1);
}
