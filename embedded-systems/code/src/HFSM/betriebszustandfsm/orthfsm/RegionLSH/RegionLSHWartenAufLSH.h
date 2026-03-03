/**
 * @file RegionLSHWartenAufLSH.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten auf Freiwerden der LSH.
 *
 * Das Werkstück verlässt gerade den Bereich des Höhenmessers.
 * Wenn die Lichtschranke wieder frei ist, gilt die Messung als abgeschlossen.
 */

#ifndef REGIONLSHWARTENAUFLSH_H
#define REGIONLSHWARTENAUFLSH_H

#include "RegionLSHBaseState.h"

class RegionLSHWartenAufLSH : public RegionLSHBaseState {
public:
    void entry() override;

    TriggerProcessingState t_lsa_lsh() override;

    TriggerProcessingState s_lsh_ub() override;

    void showState() override;
};
#endif //REGIONLSHWARTENAUFLSH_H