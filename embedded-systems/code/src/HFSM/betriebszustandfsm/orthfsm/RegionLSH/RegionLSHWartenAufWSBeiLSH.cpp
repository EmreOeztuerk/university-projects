/**
* @file RegionLSHWartenAufWSBeiLSH.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Zustands WartenAufWSBeiLSH.
 */

#include "RegionLSHWartenAufWSBeiLSH.h"
#include "RegionLSHHoeheMessen.h"
#include <iostream>
using namespace std;

void RegionLSHWartenAufWSBeiLSH::entry() {
    cout << "[LSH] State: WartenAufWSBeiLSH::entry()" << endl;
    action->enteredRegionLSHWartenAufWSBeiLSH();
}

TriggerProcessingState RegionLSHWartenAufWSBeiLSH::s_lsh_ub() {
    cout << "[LSH] WartenAufWSBeiLSH::s_lsh_ub() -> HoeheMessen" << endl;
    action->cmd_mtr_rechts_l();
    data->lsw_timer.starte(
        data->t_lsh_lsw_s,
        data->t_lsh_lsw_l,
        [this]() {
            action->t_lsh_lsw();
            //std::cout << "-> TIMER: 100% Weg LSH_LSM erreicht." << std::endl;
        },
        [this]() {
            action->t_toleranzzeit_lsw();
            std::cout << "-> FEHLER LSH_LSM: WS verloren!" << std::endl;
        }
    );
    leavingState();
    new(this) RegionLSHHoeheMessen;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState RegionLSHWartenAufWSBeiLSH::t_toleranzzeit_lsh() {
    cout << "[LSH] WartenAufWSBeiLSH::t_toleranzzeit_lsh() -> Fehlerbehandlung verlorenWS" << endl;
    data->setFehler(Fehlerart::verlorenWS, FehlerOrt::LSH);
    leavingState();
    return TriggerProcessingState::expliciteexitfehlerbehandlung;
}

void RegionLSHWartenAufWSBeiLSH::showState() {
    cout << "    Region LSH: WartenAufWSBeiLSH" << endl;
}