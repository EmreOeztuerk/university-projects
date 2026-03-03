/**
* @file StartTasterWarten11.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von StartTasterWarten11.
 */

#include "StartTasterWarten11.hpp"
using namespace std;

void StartTasterWarten11::entry() {
    action->enteredStartTasterWarten11();
    std::cout << "  Sub-State von Ruhezustand: StartTasterWarten11" << std::endl;
}

TriggerProcessingState StartTasterWarten11::s_st1_g() {
    std::cout << "StartTasterWarten11::ST1_G called" << std::endl;
    leavingState();
    return TriggerProcessingState::explicitExitViaB;
}

void StartTasterWarten11::showState() {
    std::cout << "  Sub-State von Ruhezustand: StartTasterWarten11" << std::endl;
}