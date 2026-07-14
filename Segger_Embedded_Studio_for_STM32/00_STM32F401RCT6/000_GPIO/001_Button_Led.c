/**-------------------------------------------------------------------
 \date  14.07.2026
 *
 *   STM32F401RCT6   
 *   ------------ 
 *  |            |
 *  |            |
 *  |            |
 *  |       PC.13| ---->  LED
 *  |            | 
 *  |        PA.0| <----  Button
 *  |            |
 *  |      +3.3V |
 *  |        GND |
 *
 *\ authors        ScuratovaAnna
 *\ сode debugging ScuratovaAnna 
 */
#include "stm32f4xx.h"
#include "stdbool.h"

volatile bool condition = true;

#define GPIO_Pin_0  ((uint16_t)0x0001)//Button
#define GPIO_Pin_1  ((uint16_t)0x0002)
#define GPIO_Pin_2  ((uint16_t)0x0004)
#define GPIO_Pin_3  ((uint16_t)0x0008)
#define GPIO_Pin_4  ((uint16_t)0x0010)
#define GPIO_Pin_5  ((uint16_t)0x0020)
#define GPIO_Pin_6  ((uint16_t)0x0040)
#define GPIO_Pin_7  ((uint16_t)0x0080)
#define GPIO_Pin_8  ((uint16_t)0x0100)
#define GPIO_Pin_9  ((uint16_t)0x0200)
#define GPIO_Pin_10 ((uint16_t)0x0400)
#define GPIO_Pin_11 ((uint16_t)0x0800)
#define GPIO_Pin_12 ((uint16_t)0x1000)
#define GPIO_Pin_13 ((uint16_t)0x2000)//Led
#define GPIO_Pin_14 ((uint16_t)0x4000)
#define GPIO_Pin_15 ((uint16_t)0x8000)

#define Input_mode                  (0x0UL)
#define General_purpose_output_mode (0x1UL)
#define Alternate_function_mode     (0x2UL)
#define Analog_mode                 (0x3UL)

#define Output_push_pull            (0x0UL)
#define Output_open_drain           (0x1UL)

#define Low_speed                   (0x0UL)
#define Medium_speed                (0x1UL)
#define High_speed                  (0x2UL)
#define Very_high_speed             (0x3UL)

#define No_pull_up_No_pull_down     (0x0UL)
#define Pull_up                     (0x1UL)
#define Pull_down                   (0x2UL)

__STATIC_INLINE void RCC_EnableSyscfgClock(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    (void)RCC->APB2ENR; // Холостое чтение для задержки шины
}

__STATIC_INLINE void RCC_EnablePwrClock(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    (void)RCC->APB1ENR; // Холостое чтение для задержки шины
}

void delay_ms(uint32_t ms) {
  for (uint32_t i = 0; i < ms * 1000; i++) {
    __NOP(); // No operation instruction
  }
}

void PORTC_13_INIT_Led(void) {
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);                                                        // Запуск тактирования порта C
  MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE13, General_purpose_output_mode << GPIO_MODER_MODE13_Pos); // Настройка GPIOC пин 13 на выход (output mode)
  MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT13, Output_push_pull << GPIO_OTYPER_OT13_Pos);             // Настройка GPIOC пин 13 в режим Push-Pull
  MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDR_OSPEED13, High_speed << GPIO_OSPEEDR_OSPEED13_Pos);        // Настройка GPIOC пин 13 в режим High_speed
  MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPD13, No_pull_up_No_pull_down << GPIO_PUPDR_PUPD13_Pos);     // Настройка GPIOC пин 13 в режим No_pull_up_No_pull_down
}

void PORTA_0_INIT_Button(void) {
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);                                                        // Запуск тактирования порта A
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE0, Input_mode << GPIO_MODER_MODE0_Pos);                    // Настройка GPIOA пин 0 на выход (Input_mode)
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD0, Pull_up << GPIO_PUPDR_PUPD0_Pos);                       // Настройка GPIOA пин 0 в режим Pull_up
}

int main(void) {
  RCC_EnableSyscfgClock();
  RCC_EnablePwrClock();
  PORTC_13_INIT_Led();
  PORTA_0_INIT_Button();
  while (1) {
  condition = READ_BIT(GPIOA->IDR, GPIO_IDR_ID0);
    //(condition == false) ? (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS13)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR13));
    //(condition == true) ? (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS13)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR13));

    //--------------------------------------------------------------------------------------------------------------------
    //Если в проекте был настроен на вход вывод PA.0, то
    //((GPIOA->IDR & 0x1Ul)==0x1UL)?(SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS13)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR13));
    //--------------------------------------------------------------------
    //Если в проекте был настроен на вход вывод PA.1, то
    //((GPIOA->IDR & 0x2Ul)==0x1UL)?(SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS13)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR13));
    //--------------------------------------------------------------------
    //Если в проекте был настроен на вход вывод PA.2, то
    //((GPIOA->IDR & 0x4Ul)==0x1UL)?(SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS13)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR13));
    //--------------------------------------------------------------------
    //Если в проекте был настроен на вход вывод PA.3, то
    //((GPIOA->IDR & 0x8Ul)==0x1UL)?(SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS13)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR13));
    //--------------------------------------------------------------------
    //#define GPIO_Pin_0  ((uint16_t)0x0001)//Px.0(PA.0; PB.0; PC.0 ... Px.0)
    //#define GPIO_Pin_1  ((uint16_t)0x0002)//Px.1
    //#define GPIO_Pin_2  ((uint16_t)0x0004)//Px.2
    //#define GPIO_Pin_3  ((uint16_t)0x0008)//Px.3 и т.д.
    //#define GPIO_Pin_4  ((uint16_t)0x0010)
    //#define GPIO_Pin_5  ((uint16_t)0x0020)
    //#define GPIO_Pin_6  ((uint16_t)0x0040)
    //#define GPIO_Pin_7  ((uint16_t)0x0080)
    //#define GPIO_Pin_8  ((uint16_t)0x0100)
    //#define GPIO_Pin_9  ((uint16_t)0x0200)
    //#define GPIO_Pin_10 ((uint16_t)0x0400)
    //#define GPIO_Pin_11 ((uint16_t)0x0800)
    //#define GPIO_Pin_12 ((uint16_t)0x1000)
    //#define GPIO_Pin_13 ((uint16_t)0x2000)
    //#define GPIO_Pin_14 ((uint16_t)0x4000)
    //#define GPIO_Pin_15 ((uint16_t)0x8000)//Px.15
  ((GPIOA->IDR & GPIO_Pin_0)==true)?(SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS13)) : (SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR13));

  }
}
