/**
* @file ResetTasterWarten2.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von ResetTasterWarten2.
 */

#include "ResetTasterWarten2.hpp"

void ResetTasterWarten2::entry() {
    action->enteredResetTasterWarten2();
    std::cout << "  Sub-State von Ruhezustand: ResetTasterWarten2" << std::endl;
}


TriggerProcessingState ResetTasterWarten2::s_rt2_g() {
    std::cout << "StarttasterWarten2::RT2_G called" << std::endl;
    leavingState();

    new(this) Betriebsbereit;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void ResetTasterWarten2::showState() {
    std::cout << "  Sub-State von Ruhezustand: ResetTasterWarten2" << std::endl;
}