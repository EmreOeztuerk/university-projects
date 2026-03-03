/**
* @file AufWSWarten.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Zustands AufWSWarten.
 */

#include "AufWSWarten.h"
#include "Sortierung.h"

#include <iostream>
using namespace std;

void AufWSWarten::entry() {
    action->enteredAufWSWarten();
    action->cmd_mtr_stop(); // Motor stoppen
    action->cmd_led_q1_an(); // Q1 LED EIN
    cout << "[Betriebszustand] AufWSWarten::entry()" << endl;
}

void AufWSWarten::exit() {
    data->reset_abgeschlossen = false;
    cout << "[Betriebszustand] AufWSWarten::exit()" << endl;
}

void AufWSWarten::enterByDefaultEntryPoint() {
    entry();
}

TriggerProcessingState AufWSWarten::s_lsa1_ub() {
    if (data->gFesto1()) {
        cout << "[Betriebszustand] AufWSWarten::s_lsa1_ub() -> Sortierung" << endl;

        data->lsh_timer.starte(
            data->t_lsa_lsh_s,
            data->t_lsa_lsh_l,
            [this]() {
                action->t_lsa_lsh();
                //std::cout << "-> TIMER: 100% Weg LSA_LSH erreicht." << std::endl;
            },
            [this]() {
                action->t_toleranzzeit_lsh();
                //std::cout << "-> FEHLER LSA_LSH: WS verloren BEI LSH!" << std::endl;
            }
        );

        leavingState();
        new(this) Sortierung;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }

    return TriggerProcessingState::pending;
}

TriggerProcessingState AufWSWarten::s_ws_an_festo2() {
    if (data->gFesto2()) {
        cout << "[Betriebszustand] AufWSWarten::s_ws_an_festo2() -> Sortierung" << endl;
        leavingState();
        new(this) Sortierung;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}

void AufWSWarten::showState() {
    cout << "[Betriebszustand] AufWSWarten" << endl;
}