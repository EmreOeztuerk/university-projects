/**
 * @file RegionLSWBisLSEWartenAufLSE.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten, bis LSE wieder frei ist (Durchlauf oder Entnahme).
 *
 * Das Werkstück unterbricht gerade die Lichtschranke am Ende.
 * Bei Anlage 1: Übergabe an Anlage 2 (Werkstück läuft durch).
 * Bei Anlage 2: Werkstück stoppt am Bandende und muss entnommen werden.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENAUFLSE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENAUFLSE_H

#include "RegionLSWBisLSEBaseState.h"

class RegionLSWBisLSEWartenAufLSE : public RegionLSWBisLSEBaseState {
public:
    void entry() override;

    TriggerProcessingState t_lsw_lse() override;

    TriggerProcessingState s_lse1_ub() override;

    TriggerProcessingState s_lse2_ub() override;

    void showState() override;
};
#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENAUFLSE_H