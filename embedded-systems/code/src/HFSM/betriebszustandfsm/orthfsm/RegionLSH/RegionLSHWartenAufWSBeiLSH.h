/**
 * @file RegionLSHWartenAufWSBeiLSH.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten auf Werkstück bei der Höhenmessung.
 *
 * Die Lichtschranke LSH ist frei, es wird darauf gewartet, dass ein Werkstück eintrifft (Lichtschranke unterbrochen).
 */

#ifndef REGIONLSHWARTENAUFWSBEILSH_H
#define REGIONLSHWARTENAUFWSBEILSH_H

#include "RegionLSHBaseState.h"

class RegionLSHWartenAufWSBeiLSH : public RegionLSHBaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsh_ub() override;

    TriggerProcessingState t_toleranzzeit_lsh() override;

    void showState() override;
};
#endif //REGIONLSHWARTENAUFWSBEILSH_H