/**
 * @file Wartezustand.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Wartezustands.
 */

#include "Wartezustand.hpp"
#include "ResetTasterWarten2.hpp"
#include "ResetTasterWarten1.hpp"
#include "StartTasterWarten2.hpp"
#include "StartTasterWarten1.hpp"
#include "StartTasterWarten22.hpp"
#include "StartTasterWarten11.hpp"
#include "../ContextData.h"

void Wartezustand::entry() {
    action->enteredWartezustand();
    std::cout << "  Sub-State von Ruhezustand: Wartezustand" << std::endl;
}

// ---------Ruhezustand nach Service Mode----------------
TriggerProcessingState Wartezustand::s_st1_lg() {
    std::cout << "Wartezustand::ST1_LG called" << std::endl;
    leavingState();

    new(this) StartTasterWarten2;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Wartezustand::s_st2_lg() {
    std::cout << "Wartezustand::ST2_LG called" << std::endl;
    leavingState();

    new(this) StartTasterWarten1;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

// ---------Ruhezustand nach Betriebzustand----------------

TriggerProcessingState Wartezustand::s_st1_g() {
    std::cout << "Wartezustand::ST1_G called" << std::endl;
    leavingState();
    //if(data->kalibrierungFertig == true){
    new(this) StartTasterWarten22;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
    //}
    return TriggerProcessingState::pending;
}

TriggerProcessingState Wartezustand::s_st2_g() {
    std::cout << "Wartezustand::ST1_G called" << std::endl;
    leavingState();
    //if(  data->kalibrierungFertig == true){
    new(this) StartTasterWarten11;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
    // }
    return TriggerProcessingState::pending;
}


TriggerProcessingState Wartezustand::s_rt1_g() {
    std::cout << "Wartezustand::RT1_G called" << std::endl;
    leavingState();

    new(this) ResetTasterWarten2;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState Wartezustand::s_rt2_g() {
    std::cout << "Wartezustand::RT2_G called" << std::endl;
    leavingState();

    new(this) ResetTasterWarten1;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void Wartezustand::showState() {
    std::cout << "  Sub-State von Ruhezustand: Wartezustand" << std::endl;
}