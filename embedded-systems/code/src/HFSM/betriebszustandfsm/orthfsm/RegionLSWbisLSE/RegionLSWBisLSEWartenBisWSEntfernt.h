/**
 * @file RegionLSWBisLSEWartenBisWSEntfernt.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten auf Entnahme (nur Anlage 2).
 *
 * Das Band wurde gestoppt, da ein Werkstück am Ende liegt.
 * Die Anlage wartet, bis der Benutzer das Werkstück entfernt hat (LSE wieder frei).
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENBISWSENTFERNT_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENBISWSENTFERNT_H

#include "RegionLSWBisLSEBaseState.h"

class RegionLSWBisLSEWartenBisWSEntfernt : public RegionLSWBisLSEBaseState {
public:
    void entry() override;

    void exit() override;

    TriggerProcessingState s_lse2_nub() override; // -> zurück zu WartenAufLSE

    void showState() override;
};
#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENBISWSENTFERNT_H