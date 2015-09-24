#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>
#include <stdint.h>

#include "devboard.h"

static SPI_HandleTypeDef spi_handle;

static void led_gpio_clk_enable(enum led l) {
    switch(l) {
        case LED1: LED1_GPIO_CLK_ENABLE(); break;
        case LED2: LED2_GPIO_CLK_ENABLE(); break;
        case LED3: LED3_GPIO_CLK_ENABLE(); break;
        case LED4: LED4_GPIO_CLK_ENABLE(); break;
        case LED5: LED5_GPIO_CLK_ENABLE(); break;
        case LED6: LED6_GPIO_CLK_ENABLE(); break;
        case LED7: LED7_GPIO_CLK_ENABLE(); break;
        case LED8: LED8_GPIO_CLK_ENABLE(); break;
        case LEDn: break;
    }
}

static void led_gpio_clk_disable(enum led l) {
    switch(l) {
        case LED1: LED1_GPIO_CLK_DISABLE(); break;
        case LED2: LED2_GPIO_CLK_DISABLE(); break;
        case LED3: LED3_GPIO_CLK_DISABLE(); break;
        case LED4: LED4_GPIO_CLK_DISABLE(); break;
        case LED5: LED5_GPIO_CLK_DISABLE(); break;
        case LED6: LED6_GPIO_CLK_DISABLE(); break;
        case LED7: LED7_GPIO_CLK_DISABLE(); break;
        case LED8: LED8_GPIO_CLK_DISABLE(); break;
        case LEDn: break;
    }
}


static void button_gpio_clk_enable(enum button b) {
    switch(b) {
        case BUTTON1: BUTTON1_GPIO_CLK_ENABLE(); break;
        case BUTTON2: BUTTON2_GPIO_CLK_ENABLE(); break;
        case BUTTON3: BUTTON3_GPIO_CLK_ENABLE(); break;
        case BUTTON4: BUTTON4_GPIO_CLK_ENABLE(); break;
        case BUTTON5: BUTTON5_GPIO_CLK_ENABLE(); break;
        case BUTTON6: BUTTON6_GPIO_CLK_ENABLE(); break;
        case BUTTON7: BUTTON7_GPIO_CLK_ENABLE(); break;
        case BUTTON8: BUTTON8_GPIO_CLK_ENABLE(); break;
        case BUTTONn: break;
    }
}

static void button_gpio_clk_disable(enum button b) {
    switch(b) {
        case BUTTON1: BUTTON1_GPIO_CLK_DISABLE(); break;
        case BUTTON2: BUTTON2_GPIO_CLK_DISABLE(); break;
        case BUTTON3: BUTTON3_GPIO_CLK_DISABLE(); break;
        case BUTTON4: BUTTON4_GPIO_CLK_DISABLE(); break;
        case BUTTON5: BUTTON5_GPIO_CLK_DISABLE(); break;
        case BUTTON6: BUTTON6_GPIO_CLK_DISABLE(); break;
        case BUTTON7: BUTTON7_GPIO_CLK_DISABLE(); break;
        case BUTTON8: BUTTON8_GPIO_CLK_DISABLE(); break;
        case BUTTONn: break;
    }
}

void BSP_LED_init(enum led l) {
    led_gpio_clk_enable(l);

    HAL_GPIO_Init(GPIO_PORT[l], &(GPIO_InitTypeDef) {
        .Pin = GPIO_PIN[l],
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FAST,
    });
    HAL_GPIO_WritePin(GPIO_PORT[l], GPIO_PIN[l], GPIO_PIN_RESET);
}

void BSP_LED_on(enum led l) {
    HAL_GPIO_WritePin(GPIO_PORT[l], GPIO_PIN[l], GPIO_PIN_SET);
}

void BSP_LED_off(enum led l) {
    HAL_GPIO_WritePin(GPIO_PORT[l], GPIO_PIN[l], GPIO_PIN_RESET);
}

void BSP_LED_toggle(enum led l) {
    HAL_GPIO_TogglePin(GPIO_PORT[l], GPIO_PIN[l]);
}


void BSP_BUTTON_init(enum button b, enum button_mode mode) {
    button_gpio_clk_enable(b);

    switch (mode) {
        case BUTTON_MODE_GPIO:
            HAL_GPIO_Init(BUTTON_PORT[b], &(GPIO_InitTypeDef) {
                .Pin = BUTTON_PIN[b],
                .Mode = GPIO_MODE_INPUT,
                .Pull = GPIO_PULLDOWN,
                .Speed = GPIO_SPEED_FAST,
            });
            break;

        case BUTTON_MODE_EXTI:
            HAL_GPIO_Init(BUTTON_PORT[b], &(GPIO_InitTypeDef) {
                .Pin = BUTTON_PIN[b],
                .Mode = GPIO_MODE_IT_FALLING,
                .Pull = GPIO_NOPULL,
            });
            HAL_NVIC_SetPriority(BUTTON_IRQn[b], 0x0F, 0x00); // Lowest priority
            HAL_NVIC_EnableIRQ(BUTTON_IRQn[b]);
            break;
    }
}

GPIO_PinState BSP_BUTTON_get_state(enum button b) {
    return HAL_GPIO_ReadPin(BUTTON_PORT[b], BUTTON_PIN[b]);
}


static void SPIx_MspInit(SPI_HandleTypeDef *hspi) {
    (void)hspi; // unused parameter

    // Configure the GPIO used by SPI

    // Enable GPIO clocks
    DEVBOARD_SPIx_SCK_GPIO_CLK_ENABLE();
    DEVBOARD_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();


    // Configure SPI SCK
    GPIO_InitTypeDef gpio = {
        .Pin       = DEVBOARD_SPIx_SCK_PIN,
        .Mode      = GPIO_MODE_AF_PP,
        .Pull      = GPIO_PULLUP,
        .Speed     = GPIO_SPEED_FAST,
        .Alternate = DEVBOARD_SPIx_SCK_AF,
    };
    HAL_GPIO_Init(DEVBOARD_SPIx_SCK_GPIO_PORT, &gpio);

    // Configure SPI MISO og MOSI
    gpio.Pin       = DEVBOARD_SPIx_MOSI_PIN;
    gpio.Alternate = DEVBOARD_SPIx_MISO_MOSI_AF;
    gpio.Pull      = GPIO_PULLDOWN;
    HAL_GPIO_Init(DEVBOARD_SPIx_MISO_MOSI_GPIO_PORT, &gpio);

    gpio.Pin = DEVBOARD_SPIx_MISO_PIN;
    HAL_GPIO_Init(DEVBOARD_SPIx_MISO_MOSI_GPIO_PORT, &gpio);

    // Enable the SPI clock
    DEVBOARD_SPIx_CLK_ENABLE();
}

static void SPIx_init(void) {
    if (HAL_SPI_GetState(&spi_handle) == HAL_SPI_STATE_RESET) {
        spi_handle.Instance                = DEVBOARD_SPIx;

        spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
        spi_handle.Init.Direction         = SPI_DIRECTION_2LINES;
        spi_handle.Init.CLKPhase          = SPI_PHASE_2EDGE;
        spi_handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
        spi_handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLED;
        spi_handle.Init.CRCPolynomial     = 7;
        spi_handle.Init.DataSize          = SPI_DATASIZE_8BIT;
        spi_handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
        spi_handle.Init.NSS               = SPI_NSS_SOFT;
        spi_handle.Init.TIMode            = SPI_TIMODE_DISABLED;
        spi_handle.Init.Mode              = SPI_MODE_MASTER;

        SPIx_MspInit(&spi_handle);
        HAL_SPI_Init(&spi_handle);
    }
}

static HAL_StatusTypeDef SPIx_transmit(void *tx, uint32_t size) {
    return HAL_SPI_Transmit(&spi_handle, tx, size, 1000);
}

void seven_seg_init(void) {
    HAL_GPIO_Init(SEVEN_SEG_LATCH_GPIO_PORT, &(GPIO_InitTypeDef) {
        .Pin = SEVEN_SEG_LATCH_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FAST,
    });
    SEVEN_SEG_LATCH_OFF();

    SPIx_init();
    seven_seg_write(0, 0);
}


void seven_seg_write(uint8_t disp1, uint8_t disp2) {
    const uint16_t d = disp2 | (disp1 << 8);
    SPIx_transmit((void*)&d, sizeof(d));
    SEVEN_SEG_LATCH_ON();
    SEVEN_SEG_LATCH_OFF();
}

#define ARR_LEN(arr)    (sizeof(arr) / sizeof(arr[0]))
void seven_seg_disp_num(uint8_t num1, uint8_t num2) {
    const uint8_t lookup[] = {
        [0] = 0x3F, [1] = 0x06, [2] = 0x5B, [3] = 0x4F, [4] = 0x66,
        [5] = 0x6D, [6] = 0x7D, [7] = 0x07, [8] = 0x7F, [9] = 0x6F,
        [0xA] = 0x77, [0xb] = 0x7C, [0xC] = 0x39,
        [0xd] = 0x5E, [0xE] = 0x79, [0xF] = 0x71,
    };

    const uint8_t disp1 = (num1 < ARR_LEN(lookup)) ? lookup[num1] : 0;
    const uint8_t disp2 = (num2 < ARR_LEN(lookup)) ? lookup[num2] : 0;

    seven_seg_write(disp1, disp2);
}
