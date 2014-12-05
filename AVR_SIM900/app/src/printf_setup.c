/**
 * @file: 	printf_setup.c
 * @brief:	
 * @date: 	2 gru 2014
 * @author: Michal Ksiezopolski
 *
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

#include <comm.h>
#include <stdio.h>

static int printfWrite(char c, FILE *stream);

/**
 * @brief This setups the stream for printf with a function
 * which sends data on the USART.
 */
static FILE mystdout = FDEV_SETUP_STREAM(printfWrite, NULL, _FDEV_SETUP_WRITE);

/**
 * @brief This function sends data to the USART.
 * @param c Character to send
 * @param stream Stream
 * @return
 * TODO Check what we can do with stream param
 */
static int printfWrite(char c, FILE *stream) {
  COMM_Putc(c);
  return 0;
}
/**
 * @brief This function sets the stdout stream to
 * point to our USART stream.
 */
void printfSetup(void) {
  stdout = &mystdout;
}
