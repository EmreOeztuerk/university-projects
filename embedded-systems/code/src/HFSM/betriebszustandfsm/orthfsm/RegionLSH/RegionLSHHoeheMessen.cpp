/**
* @file RegionLSHHoeheMessen.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des Zustands HoeheMessen.
 */

#include "RegionLSHHoeheMessen.h"
#include "RegionLSHWartenAufLSH.h"
#include <iostream>
using namespace std;

void RegionLSHHoeheMessen::entry() {
    cout << "[LSH] State: HoeheMessen::entry()" << endl;
    data->enteredHoheMessen = true;
    action->enteredRegionLSHHoeheMessen();
    //action->cmd_mtr_rechts_l();
    data->set(data->motorSchnell, false);
}

TriggerProcessingState RegionLSHHoeheMessen::s_lsh_nub() {
    if (!data->enteredWartenAufErlaubnisVonF2) action->cmd_mtr_rechts_s();

    data->set(data->motorSchnell, true);
    data->enteredHoheMessen = false;
    leavingState();
    new(this) RegionLSHWartenAufLSH;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

void RegionLSHHoeheMessen::showState() {
    cout << "    Region LSH: HoeheMessen" << endl;
}