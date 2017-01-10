/**
 * \file
 *
 * \brief Board configuration.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

#define CONF_BOARD_UART_CONSOLE

/** SPI MACRO definition */
#define CONF_BOARD_SPI

/** SPI slave select MACRO definition */
#define CONF_BOARD_SPI_NPCS0

/** Board SPI chip select0 pin definition */
#define BOARD_SPI_NPCS0_GPIO         SPI_NPCS0_GPIO
#define BOARD_SPI_NPCS0_GPIO_FLAGS   SPI_NPCS0_GPIO_FLAGS

/** Board SPI chip select1 pin definition. As there are several pins which 
 *  can be used as SPI chip select1, this needs to be modified according 
 *  to actual board.
 */
//#define BOARD_SPI_NPCS1_GPIO         SPI_NPCS1_GPIO_PA0
//#define BOARD_SPI_NPCS1_GPIO_FLAGS   SPI_NPCS1_GPIO_PA0_FLAGS

/** Board SPI chip select2 pin definition. As there are several pins which 
 *  can be used as SPI chip select2, this needs to be modified according 
 *  to actual board.
 */
//#define BOARD_SPI_NPCS2_GPIO         SPI_NPCS2_GPIO_PA1
//#define BOARD_SPI_NPCS2_GPIO_FLAGS   SPI_NPCS2_GPIO_PA1_FLAGS

/** Board SPI chip select3 pin definition. As there are several pins which 
 *  can be used as SPI chip select3, this needs to be modified according 
 *  to actual board.
 */
//#define BOARD_SPI_NPCS3_GPIO         SPI_NPCS3_GPIO_PA19
//#define BOARD_SPI_NPCS3_GPIO_FLAGS   SPI_NPCS3_GPIO_PA19_FLAGS

#endif /* CONF_BOARD_H_INCLUDED */