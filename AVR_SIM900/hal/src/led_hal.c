/**
 * @file: 	led_hal.c
 * @brief:	HAL for using LEDs
 * @date: 	25 sie 2014
 * @author: Michal Ksiezopolski
 * 
 * @verbatim
 * Copyright (c) 2014 Michal Ksiezopolski.
 * All rights reserved. This program and the 
 * accompanying materials are made available 
 * under the terms of the GNU Public License 
 * v3.0 which accompanies this distribution, 
 * and is available at 
 * http://www.gnu.org/licenses/gpl.html
 * @endverbatim
 */

#include <led_hal.h>
#include <avr/io.h>

/**
 * @addtogroup LED_HAL
 * @{
 */

#ifdef __AVR_ATmega32__
/**
 * @brief LED GPIO ports
 */
static volatile uint8_t* ledPort[MAX_LEDS] = {
    &PORTA,
    &PORTA,
    &PORTA,
    &PORTA
};

/**
 * @brief LED GPIO data direction
 */
static volatile uint8_t* ledDir[MAX_LEDS] = {
    &DDRA,
    &DDRA,
    &DDRA,
    &DDRA
};

/**
 * @brief LED pin numbers
 */
static uint8_t ledPin[MAX_LEDS] = {
    1<<0,
    1<<1,
    1<<2,
    1<<3,
};

#elif defined __AVR_ATmega8__
/**
 * @brief LED GPIO ports
 */
static volatile uint8_t* ledPort[MAX_LEDS] = {
    &PORTB,
    &PORTD
};

/**
 * @brief LED GPIO data direction
 */
static volatile uint8_t* ledDir[MAX_LEDS] = {
    &DDRB,
    &DDRD
};

/**
 * @brief LED pin numbers
 */
static uint8_t ledPin[MAX_LEDS] = {
    1<<0,
    1<<7
};
#else
  #error "Unsupported MCU"
#endif

/**
 * @brief Add an LED.
 * @param led LED number.
 */
void LED_HAL_Init(uint8_t led) {

    *ledDir[led] |= ledPin[led];   // set pin as output
    *ledPort[led] &= ~ledPin[led]; // reset pin

}

/**
 * @brief Toggle an LED.
 * @param led LED number.
 */
void LED_HAL_Toggle(uint8_t led) {

  *ledPort[led] ^= ledPin[led]; // toggle bit
}

/**
 * @brief Change the state of an LED.
 * @param led LED number.
 * @param state New state.
 */
void LED_HAL_ChangeState(uint8_t led, uint8_t state) {

  if (state == 1) {
    *ledPort[led] |= ledPin[led]; // set bit
  } else {
    *ledPort[led] &= ~ledPin[led]; // reset bit
  }

}

/**
 * @}
 */
