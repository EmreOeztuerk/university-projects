/**
* @file ResetTasterWarten1.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von ResetTasterWarten1.
 */

#include "ResetTasterWarten1.hpp"

void ResetTasterWarten1::entry() {
    std::cout << "  Sub-State von Ruhezustand: ResetTasterWarten1" << std::endl;
    action->enteredResetTasterWarten1();
}


TriggerProcessingState ResetTasterWarten1::s_rt1_g() {
    std::cout << "ResetTasterWarten1::RT1_G called" << std::endl;
    leavingState();

    new(this) Betriebsbereit;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void ResetTasterWarten1::showState() {
    std::cout << "  Sub-State von Ruhezustand: ResetTasterWarten1" << std::endl;
}