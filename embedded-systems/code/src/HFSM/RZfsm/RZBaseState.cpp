/**
 * @file RZBaseState.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der RZBaseState-Klasse.
 */

#include "RZBaseState.hpp"
#include "../RZfsm/Wartezustand.hpp"

void RZBaseState::setData(ContextData *data) {
    this->data = data;
}

void RZBaseState::setAction(Actions *action) {
    this->action = action;
}

void RZBaseState::enterViaPseudoStart() {
    // entry to state machine
    std::cout << "RZfsm faengt mit Wartezustand an." << std::endl;
    new(this) Wartezustand;
    enterByDefaultEntryPoint(); // Entry of initial state
}