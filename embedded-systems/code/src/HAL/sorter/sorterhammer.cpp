/**
 * @file sorterhammer.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des SortiererAuswerfers.
 *
 * Steuert den Auswerfer-Magneten über GPIO. Beim Aussortieren wird der Magnet
 * aktiviert, kurz gewartet und dann wieder deaktiviert.
 */

#include "sorterhammer.hpp"

SortiererAuswerfer::SortiererAuswerfer() {
    mechanismus = SortTyp::AUSWERFER;
    sortiererStatus = false;
}

bool SortiererAuswerfer::getSortiererStatus() {
    return sortiererStatus;
}

void SortiererAuswerfer::durchlassen() {
    if (getSortiererStatus()) {
        ThreadCtl(_NTO_TCTL_IO, 0);
        uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
        out32((uintptr_t)(gpio_bank_1 + OFFSET_CRL_REG), MECHANISMUS_BETAETIGEN);
        sortiererStatus = false;
    }
}

void SortiererAuswerfer::aussortieren() {
    if (!getSortiererStatus()) {
        ThreadCtl(_NTO_TCTL_IO, 0);
        uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
        out32((uintptr_t)(gpio_bank_1 + OFFSET_SET_REG), MECHANISMUS_BETAETIGEN);
        sortiererStatus = true;
        usleep(1000 * 1000);
        durchlassen();
    }
}