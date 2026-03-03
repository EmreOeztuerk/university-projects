/**
 * @file RegionLSANaechsteWSErlaubt.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Nächstes Werkstück erlaubt.
 *
 * Die Anlage ist bereit, ein weiteres Werkstück am Einlauf zu akzeptieren.
 * Wechselt wieder zurück in den Wartezustand bei LSA.
 */

#ifndef REGIONLSANAECHSTEWSERLAUBT_H
#define REGIONLSANAECHSTEWSERLAUBT_H

#include "RegionLSABaseState.h"

class RegionLSANaechsteWSErlaubt : public RegionLSABaseState {
public:
    void entry() override;

    TriggerProcessingState s_lsa1_ub() override;

    void showState() override;
};

#endif //REGIONLSANAECHSTEWSERLAUBT_H