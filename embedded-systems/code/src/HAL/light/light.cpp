/**
 * @file light.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Klasse Licht.
 *
 * Setzt die entsprechenden Bits im GPIO-Register, um Q1 oder Q2 ein- bzw. auszuschalten.
 */

#include "light.hpp"

Licht::Licht(TypLicht given_licht) {
	licht = given_licht;
	lichtStatus = false;
	initLED();
}

bool Licht::getLichtStatus() {
	return lichtStatus;
}

TypLicht Licht::getTypLicht() {
	return licht;
}

void Licht::ledAn() {
	if(!getLichtStatus()) {
		ThreadCtl( _NTO_TCTL_IO, 0);
		uintptr_t gpio_bank_2 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_2);
		out32((uintptr_t) (gpio_bank_2 + OFFSET_SET_REG), getTypLicht());
		lichtStatus = true;
	}
}

void Licht::ledAus() {
	if(getLichtStatus()) {
		ThreadCtl( _NTO_TCTL_IO, 0);
		uintptr_t gpio_bank_2 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_2);
		out32((uintptr_t) (gpio_bank_2 + OFFSET_CRL_REG), getTypLicht());
		lichtStatus = false;
	}
}

void Licht::initLED() {
	ThreadCtl(_NTO_TCTL_IO, 0);
	uintptr_t gpio_bank_2 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_2);
	unsigned int oe = in32((uintptr_t) (gpio_bank_2 + GPIO_OE));
	oe &= ~licht;
	out32((uintptr_t) (gpio_bank_2 + GPIO_OE), oe);
	out32((uintptr_t) (gpio_bank_2 + OFFSET_CRL_REG), licht);
}
