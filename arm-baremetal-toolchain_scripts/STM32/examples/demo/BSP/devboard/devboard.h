#ifndef DEVBOARD_H
#define DEVBOARD_H

#include <stm32f4xx_hal.h>

enum led {
    LED1,
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED7,
    LED8,

    LEDn // Number of LEDS
};

#define LED1_PIN                    GPIO_PIN_8
#define LED1_GPIO_PORT              GPIOA
#define LED1_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()     __GPIOA_CLK_DISABLE()

#define LED2_PIN                    GPIO_PIN_12
#define LED2_GPIO_PORT              GPIOB
#define LED2_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define LED3_PIN                    GPIO_PIN_3
#define LED3_GPIO_PORT              GPIOA
#define LED3_GPIO_CLK_ENABLE()      __GPIOA_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()     __GPIOA_CLK_DISABLE()

#define LED4_PIN                    GPIO_PIN_7
#define LED4_GPIO_PORT              GPIOE
#define LED4_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()

#define LED5_PIN                    GPIO_PIN_1
#define LED5_GPIO_PORT              GPIOB
#define LED5_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define LED6_PIN                    GPIO_PIN_0
#define LED6_GPIO_PORT              GPIOB
#define LED6_GPIO_CLK_ENABLE()      __GPIOB_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()     __GPIOB_CLK_DISABLE()

#define LED7_PIN                    GPIO_PIN_5
#define LED7_GPIO_PORT              GPIOC
#define LED7_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define LED7_GPIO_CLK_DISABLE()     __GPIOC_CLK_DISABLE()

#define LED8_PIN                    GPIO_PIN_4
#define LED8_GPIO_PORT              GPIOC
#define LED8_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()
#define LED8_GPIO_CLK_DISABLE()     __GPIOC_CLK_DISABLE()

#define GPIO_PORT ((GPIO_TypeDef* const []) { \
    LED1_GPIO_PORT, \
    LED2_GPIO_PORT, \
    LED3_GPIO_PORT, \
    LED4_GPIO_PORT, \
    LED5_GPIO_PORT, \
    LED6_GPIO_PORT, \
    LED7_GPIO_PORT, \
    LED8_GPIO_PORT, \
})

#define GPIO_PIN ((const uint16_t const[]) { \
    LED1_PIN, \
    LED2_PIN, \
    LED3_PIN, \
    LED4_PIN, \
    LED5_PIN, \
    LED6_PIN, \
    LED7_PIN, \
    LED8_PIN, \
})


enum button {
    BUTTON1,
    BUTTON2,
    BUTTON3,
    BUTTON4,
    BUTTON5,
    BUTTON6,
    BUTTON7,
    BUTTON8,

    BUTTONn // number of buttons
};

enum button_mode {
    BUTTON_MODE_GPIO,
    BUTTON_MODE_EXTI,
};

#define BUTTON1_PIN                    GPIO_PIN_8
#define BUTTON1_GPIO_PORT              GPIOE
#define BUTTON1_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define BUTTON1_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()
#define BUTTON1_EXTI_IRQn              EXTI9_5_IRQn

#define BUTTON2_PIN                    GPIO_PIN_9
#define BUTTON2_GPIO_PORT              GPIOE
#define BUTTON2_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define BUTTON2_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()
#define BUTTON2_EXTI_IRQn              EXTI9_5_IRQn

#define BUTTON3_PIN                    GPIO_PIN_10
#define BUTTON3_GPIO_PORT              GPIOE
#define BUTTON3_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define BUTTON3_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()
#define BUTTON3_EXTI_IRQn              EXTI15_10_IRQn

#define BUTTON4_PIN                    GPIO_PIN_11
#define BUTTON4_GPIO_PORT              GPIOE
#define BUTTON4_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define BUTTON4_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()
#define BUTTON4_EXTI_IRQn              EXTI15_10_IRQn

#define BUTTON5_PIN                    GPIO_PIN_12
#define BUTTON5_GPIO_PORT              GPIOE
#define BUTTON5_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define BUTTON5_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()
#define BUTTON5_EXTI_IRQn              EXTI15_10_IRQn

#define BUTTON6_PIN                    GPIO_PIN_13
#define BUTTON6_GPIO_PORT              GPIOE
#define BUTTON6_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define BUTTON6_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()
#define BUTTON6_EXTI_IRQn              EXTI15_10_IRQn

#define BUTTON7_PIN                    GPIO_PIN_14
#define BUTTON7_GPIO_PORT              GPIOE
#define BUTTON7_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define BUTTON7_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()
#define BUTTON7_EXTI_IRQn              EXTI15_10_IRQn

#define BUTTON8_PIN                    GPIO_PIN_15
#define BUTTON8_GPIO_PORT              GPIOE
#define BUTTON8_GPIO_CLK_ENABLE()      __GPIOE_CLK_ENABLE()
#define BUTTON8_GPIO_CLK_DISABLE()     __GPIOE_CLK_DISABLE()
#define BUTTON8_EXTI_IRQn              EXTI15_10_IRQn

#define BUTTON_PORT ((GPIO_TypeDef* const []) { \
    BUTTON1_GPIO_PORT, \
    BUTTON2_GPIO_PORT, \
    BUTTON3_GPIO_PORT, \
    BUTTON4_GPIO_PORT, \
    BUTTON5_GPIO_PORT, \
    BUTTON6_GPIO_PORT, \
    BUTTON7_GPIO_PORT, \
    BUTTON8_GPIO_PORT, \
})

#define BUTTON_PIN ((const uint16_t const []) { \
    BUTTON1_PIN, \
    BUTTON2_PIN, \
    BUTTON3_PIN, \
    BUTTON4_PIN, \
    BUTTON5_PIN, \
    BUTTON6_PIN, \
    BUTTON7_PIN, \
    BUTTON8_PIN, \
})

#define BUTTON_IRQn ((const uint8_t const []) { \
    BUTTON1_EXTI_IRQn, \
    BUTTON2_EXTI_IRQn, \
    BUTTON3_EXTI_IRQn, \
    BUTTON4_EXTI_IRQn, \
    BUTTON5_EXTI_IRQn, \
    BUTTON6_EXTI_IRQn, \
    BUTTON7_EXTI_IRQn, \
    BUTTON8_EXTI_IRQn, \
})


#define DEVBOARD_SPIx                               SPI1
#define DEVBOARD_SPIx_CLK_ENABLE()                  __SPI1_CLK_ENABLE()

#define DEVBOARD_SPIx_SCK_AF                        GPIO_AF5_SPI1
#define DEVBOARD_SPIx_SCK_GPIO_PORT                 GPIOA
#define DEVBOARD_SPIx_SCK_PIN                       GPIO_PIN_5
#define DEVBOARD_SPIx_SCK_GPIO_CLK_ENABLE()         __GPIOA_CLK_ENABLE()
#define DEVBOARD_SPIx_SCK_GPIO_CLK_DISABLE()        __GPIOA_CLK_DISABLE()

#define DEVBOARD_SPIx_MISO_MOSI_AF                  GPIO_AF5_SPI1
#define DEVBOARD_SPIx_MISO_MOSI_GPIO_PORT           GPIOA
#define DEVBOARD_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()   __GPIOA_CLK_ENABLE()
#define DEVBOARD_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()  __GPIOA_CLK_DISABLE()

#define DEVBOARD_SPIx_MISO_PIN                      GPIO_PIN_6
#define DEVBOARD_SPIx_MOSI_PIN                      GPIO_PIN_7


#define SEVEN_SEG_LATCH_PIN                         GPIO_PIN_9
#define SEVEN_SEG_LATCH_GPIO_PORT                   GPIOA
#define SEVEN_SEG_LATCH_GPIO_CLK_ENABLE()           __GPIOA_CLK_ENABLE()
#define SEVEN_SEG_LATCH_GPIO_CLK_DISABLE()          __GPIOA_CLK_DISABLE()

#define SEVEN_SEG_LATCH_OFF()    HAL_GPIO_WritePin(SEVEN_SEG_LATCH_GPIO_PORT, SEVEN_SEG_LATCH_PIN, GPIO_PIN_SET);
#define SEVEN_SEG_LATCH_ON()     HAL_GPIO_WritePin(SEVEN_SEG_LATCH_GPIO_PORT, SEVEN_SEG_LATCH_PIN, GPIO_PIN_RESET);

void BSP_LED_init   (enum led l);
void BSP_LED_on     (enum led l);
void BSP_LED_off    (enum led l);
void BSP_LED_toggle (enum led l);

void            BSP_BUTTON_init     (enum button b, enum button_mode mode);
GPIO_PinState   BSP_BUTTON_get_state(enum button b);

void    seven_seg_init      (void);
void    seven_seg_write     (uint8_t disp1, uint8_t disp2);
void    seven_seg_disp_num  (uint8_t num1, uint8_t num2);

#endif /* DEVBOARD_H */
