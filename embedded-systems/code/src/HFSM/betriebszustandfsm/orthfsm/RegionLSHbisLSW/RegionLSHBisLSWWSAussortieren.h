/**
 * @file RegionLSHBisLSWWSAussortieren.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Werkstück aussortieren.
 *
 * Das Werkstück soll aussortiert werden. Die Weiche wird geöffnet oder der Pusher aktiviert,
 * um das Werkstück in die Rutsche zu befördern.
 */

#ifndef REGIONLSHBISLSWWSAUSSORTIEREN_H
#define REGIONLSHBISLSWWSAUSSORTIEREN_H

#include "RegionLSHBisLSWBaseState.h"

class RegionLSHBisLSWWSAussortieren : public RegionLSHBisLSWBaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsr_ub() override;

    TriggerProcessingState t_lsw_lsr() override;

    void showState() override;
};
#endif //REGIONLSHBISLSWWSAUSSORTIEREN_H