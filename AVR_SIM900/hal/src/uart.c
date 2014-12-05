/**
 * @file:   uart.c
 * @brief:  Controlling UART
 * @date:   12 kwi 2014
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

#include <uart.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @addtogroup USART
 * @{
 */

void    (*rxCallback)(uint8_t);   ///< Callback function for receiving data
uint8_t (*txCallback)(uint8_t*);  ///< Callback function for transmitting data

/**
 * @brief Initialize USART
 * @param baud
 * @param rxCb
 * @param txCb
 */
void UART_Init(uint32_t baud, void(*rxCb)(uint8_t), uint8_t(*txCb)(uint8_t*) ) {

  // assign the callbacks
  rxCallback = rxCb;
  txCallback = txCb;

  // Double operating speed of UART module -
  // better baud accuracy for 16MHz crystal
  UCSRA = (1<<U2X);

  // transform baud rate into register value
  // divide by 8 for U2X=1, divide by 16 for U2X=0
  uint16_t baudVal = (uint16_t)(F_CPU/8/baud - 1);

  // set baud rate
  UBRRH = (uint8_t) (baudVal>>8);
  UBRRL = (uint8_t) (baudVal);

  // 8n1 data format
  UCSRC = (1<<URSEL) | (3<<UCSZ0);

  // Enable USART - transmitter and receiver
  UCSRB = (1<<RXEN) | (1<<TXEN);

  // Enable RXNE interrupt
  UCSRB |= (1<<RXCIE);
  // Disable TXE interrupt - we enable it only when there is
  // data to send
  UCSRB &= ~(1<<UDRIE);

  // Enable global interrupt
  sei();

}
/**
 * @brief Enable transmitter.
 * @details This function has to be called by the higher layer
 * in order to start the transmitter.
 */
void UART_TxEnable(void) {
  UCSRB |= (1<<UDRIE); // enable buffer empty interrupt
}

/**
 * @brief USART data register empty interrupt
 * @param USART_UDRE_vect
 */
ISR(USART_UDRE_vect) {

  uint8_t c;

  if (txCallback) { // if transmit callback not NULL
    // get data from higher layer using callback
    if (txCallback(&c)) { // if there is data
      UDR = c; // Send data
    } else { // if no more data to send disable the transmitter
      UCSRB &= ~(1<<UDRIE);
    }
  }
}

/**
 * @brief USART received data interrupt.
 * @param USART_RXC_vect
 */
ISR(USART_RXC_vect) {

  uint8_t c = UDR; // Get data from UART

   if (rxCallback) { // if not NULL
     rxCallback(c); // send received data to higher layer
   }
}


/**
 * @}
 */
