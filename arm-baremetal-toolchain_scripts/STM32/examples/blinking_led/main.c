

#if 1
#include <stm32f4xx_hal.h>

#define LED_PIN GPIO_PIN_8

TIM_HandleTypeDef tim_handle;

void TIM2_IRQHandler(void) {
  HAL_TIM_IRQHandler(&tim_handle);
  // HAL_TIM_IRQHandler(&(TIM_HandleTypeDef) {.Instance = TIM2});
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	HAL_GPIO_TogglePin(GPIOA, LED_PIN);
}

int main(void) {
	HAL_Init();

	// First setup the io pin
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitTypeDef gpio_init = {
			.Pin = LED_PIN,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_PULLUP,
			.Speed = GPIO_SPEED_FAST,
		};

		HAL_GPIO_Init(GPIOA, &gpio_init);
		HAL_GPIO_WritePin(GPIOA, LED_PIN, GPIO_PIN_RESET);
	}
#if 1
	// Then setup the timer
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM2_IRQn, 4, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);

		tim_handle.Instance = TIM2;
		tim_handle.Init.Period = 10000/16 - 1;
		tim_handle.Init.Prescaler = 0xFFF;
		tim_handle.Init.ClockDivision = 0;
		tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;

		HAL_TIM_Base_Init(&tim_handle);
		HAL_TIM_Base_Start_IT(&tim_handle);
	}

	while(1);
#else
	while(1) {
		HAL_GPIO_TogglePin(GPIOA, LED_PIN);
	}
#endif
}

#else
#include <stm32f4xx.h>

#define LED_PIN	8

volatile uint8_t i = 0;

void TIM2_IRQHandler(void) {
	// flash on update event
	if (i++ == 0)
		if (TIM2->SR & TIM_SR_UIF) GPIOA->ODR ^= (1 << LED_PIN);

	TIM2->SR = 0x0; // reset the status register
}



int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable the clock to GPIOA
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable TIM2 clock

	GPIOA->MODER |= (1 << 2 * LED_PIN); //(1 << 26); // set pin 13 to be general purpose output
#if 1
	NVIC->ISER[0] |= 1 << (TIM2_IRQn); // enable the TIM2 IRQ

	TIM2->PSC  = 0xFFF;//0x0; // no prescaler, timer counts up in sync with the peripheral clock
	TIM2->DIER |= TIM_DIER_UIE; // enable update interrupt
	TIM2->ARR  = 0x01; // count to 1 (autoreload value 1)
	TIM2->CR1  |= TIM_CR1_ARPE | TIM_CR1_CEN; // autoreload on, counter enabled
	TIM2->EGR  = 1; // trigger update event to reload timer registers

	while (1);
#else
	while (1) {
		GPIOA->ODR ^= (1 << LED_PIN);
	}
#endif
}

#endif /* else HAL_EXAMPLE */
