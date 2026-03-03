/**
 * @file RegionLSWBisLSEWartenAufWSBeiLSE.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von WartenAufWSBeiLSE.
 */

#ifndef ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENAUFWSBEILSE_H
#define ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENAUFWSBEILSE_H

#include "RegionLSWBisLSEBaseState.h"

class RegionLSWBisLSEWartenAufWSBeiLSE : public RegionLSWBisLSEBaseState {
public:
    void entry() override;

    TriggerProcessingState s_lse2_ub() override;

    TriggerProcessingState s_lse1_ub() override;

    TriggerProcessingState t_toleranzzeit_lse() override;

    void showState() override;
};
#endif //ESEP_2025WISE_TEAM_1_3_REGIONLSWBISLSEWARTENAUFWSBEILSE_H