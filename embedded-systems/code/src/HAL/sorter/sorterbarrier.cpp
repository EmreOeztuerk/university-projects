/**
* @file sorterbarrier.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der SortiererWeiche.
 *
 * Nutzt einen separaten Thread für das Öffnen der Weiche, um den Hauptthread
 * während der Wartezeit (`usleep`) nicht zu blockieren.
 */

#include "sorterbarrier.hpp"

#include <thread> // WICHTIG: Für std::thread

SortiererWeiche::SortiererWeiche() {
    mechanismus = SortTyp::WEICHE;
    sortiererStatus = false;
}

bool SortiererWeiche::getSortiererStatus() {
    return sortiererStatus;
}

useconds_t SortiererWeiche::getMaxZeitOffen() {
    return MAX_ZEIT_OFFEN_MS;
}

void SortiererWeiche::durchlassen() {
    if (!getSortiererStatus()) {
        // Wir starten einen neuen Thread (Lambda-Funktion), der die Blockade übernimmt.
        // Der HAL-Thread läuft sofort weiter.
        std::thread weichenThread([this]() {
            this->weicheAuf();
            // Hinweis: weicheAuf() enthält laut vorherigen Dateien den usleep() und weicheZu() Aufruf.
            // Der Thread wird hier schlafen, nicht das Hauptprogramm.
        });
        // detach() sorgt dafür, dass der Thread im Hintergrund läuft,
        // auch wenn die Funktion durchlassen() beendet ist.
        weichenThread.detach();
    }
}

void SortiererWeiche::aussortieren() {
    if (getSortiererStatus()) {
        weicheZu();
    }
}

// Diese Methode bleibt unverändert, wird aber nun im separaten Thread ausgeführt
void SortiererWeiche::weicheAuf() {
    if (!getSortiererStatus()) {
        ThreadCtl(_NTO_TCTL_IO, 0); // Zugriff auf Hardware im Thread anfordern
        uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
        // GPIO Setzen (Weiche auf)
        out32((uintptr_t)(gpio_bank_1 + OFFSET_SET_REG), MECHANISMUS_BETAETIGEN);
        sortiererStatus = true;
        // Dieser Sleep blockiert nun NUR den "weichenThread", nicht den HAL!
        usleep(MAX_ZEIT_OFFEN_MS);
        weicheZu();
    }
}

void SortiererWeiche::weicheZu() {
    // Hinweis: Prüfen Sie hier ggf. Thread-Sicherheit,
    // aber GPIO SET/CLEAR Register sind in der Regel atomar.
    if (getSortiererStatus()) {
        ThreadCtl(_NTO_TCTL_IO, 0);
        uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
        out32((uintptr_t)(gpio_bank_1 + OFFSET_CRL_REG), MECHANISMUS_BETAETIGEN);
        sortiererStatus = false;
    }
}