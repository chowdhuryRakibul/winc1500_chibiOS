*****************************************************************************
** ChibiOS/RT port for ARM-Cortex-M4 STM32F410Rx.                            **
*****************************************************************************

** TARGET **

The demo runs on an STM32 Nucleo64-F410RB board.

** The Wifi Scan and Connect Demo **

The demo flashes the board LED using a thread, It also connects with ATWinc1500
via SPI2 (SPID2) and scans for WiFi networks. Upon finding the predefined SSID, 
it connects with it. All output on the serial port SD2 
(USART2, mapped on USB virtual COM port).

** PinOut **

MISO	PC2
MOSI	PC3
SCK		PB10
CS		PC1
IRQ		PB15
Enable	PB1
Reset	PB14

** Build Procedure **

The demo has been tested by using the free Codesourcery GCC-based toolchain
and YAGARTO.
Just modify the TRGT line in the makefile in order to use different GCC ports.

** Notes **

Some files used by the demo are not part of ChibiOS/RT but are copyright of
ST Microelectronics and are licensed under a different license.
Also note that not all the files present in the ST library are distributed
with ChibiOS/RT, you can find the whole library on the ST web site:

                             http://www.st.com
