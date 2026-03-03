/**
 * @file Betriebsbereit.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Zustands Betriebsbereit.
 */

#include "Betriebsbereit.hpp"
#include "StartTasterWarten11.hpp"
#include "StartTasterWarten22.hpp"

void Betriebsbereit::entry() {
    action->enteredBetriebsbereit();
    data->datenprozessLoeschen();
    cout << "  Sub-State von Ruhezustand: Betriebsbereit" << endl;
}

TriggerProcessingState Betriebsbereit::s_st1_g() {
    std::cout << "Betriebsbereit::ST1_G called" << std::endl;
    leavingState();

    new(this) StartTasterWarten22;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Betriebsbereit::s_st2_g() {
    std::cout << "Betriebsbereit::ST2_G called" << std::endl;
    leavingState();

    new(this) StartTasterWarten11;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void Betriebsbereit::showState() {
    cout << "  Sub-State von Ruhezustand: Betriebsbereit" << endl;
}