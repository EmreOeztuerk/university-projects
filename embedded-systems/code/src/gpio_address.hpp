/**
 * @file gpio_address.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Zentrale Definition der GPIO-Adressen und Register-Offsets.
 *
 * Enthält alle Hardware-Adressen (Memory Mapped IO) für den Zugriff auf
 * die GPIO-Bänke des BeagleBone Black (AM335x).
 */

#ifndef GPIO_ADDRESS_HPP
#define GPIO_ADDRESS_HPP

#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/procmgr.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/mman.h>
#include <hw/inout.h>

#define N_PINS 32

#define GPIO_BANK_0 0x44E07000	//GPIO_PORT
#define GPIO_BANK_1 0x4804C000	// spruh73l.pdf S.182
#define GPIO_BANK_2 0x481AC000



#define MAP_SIZE 0x1000	//GPIO_REGISTER_LENGHT

#define GPIO_OE      			0x134	//?
#define GPIO_DATAIN  			0x138	//!
//#define GPIO_IRQSTATUS_0		0x2C
//#define GPIO_IRQSTATUS_SET_0	0x34
//#define GPIO_RISINGDETECT		0x148
//#define GPIO_FALLINGDETECT		0x14C

#define OFFSET_SET_REG 0x194	//GPIO_SETDATAOUT
#define OFFSET_CRL_REG 0x190	//GPIO_CLEARDATAOUT

/* Interrupt numbers  (spruh73l.pdf S.465 ff.) */
#define INTR_GPIO_PORT0 97
#define INTR_GPIO_PORT1 99
#define INTR_GPIO_PORT2 33

/* GPIO register offsets (spruh73l.pdf S.4877) */
#define GPIO_LEVELDETECT0 0x140
#define GPIO_LEVELDETECT1 0x144
#define GPIO_RISINGDETECT 0x148
#define GPIO_FALLINGDETECT 0x14C

#define GPIO_IRQSTATUS_0 0x2C
#define GPIO_IRQSTATUS_1 0x30
#define GPIO_IRQSTATUS_SET_0 0x34
#define GPIO_IRQSTATUS_SET_1 0x38

#define GPIO_DATAIN 0x138
#define GPIO_SETDATAOUT 0x194


/* Helper macros */
#define BIT_MASK(x) (0x1 << (x))


/* My pulse codes */
#define PULSE_STOP_THREAD _PULSE_CODE_MINAVAIL + 1
#define PULSE_INTR_ON_PORT0 _PULSE_CODE_MINAVAIL + 2
#define PULSE_INTR_ON_PORT1 _PULSE_CODE_MINAVAIL + 3

/* My pulse codes */
#define PULSE_STOP_THREAD     _PULSE_CODE_MINAVAIL + 1
#define PULSE_INTR_ON_PORT0   _PULSE_CODE_MINAVAIL + 2

// NEU: eigener Pulse-Code für ADC-Werte
#define PULSE_ADC_WERT        _PULSE_CODE_MINAVAIL + 3


#endif
