/**
 * @file RegionWarnungWartenAufWarnung.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Keine Warnung aktiv.
 *
 * Normalzustand der Warnregion. Wartet auf Signale, die eine Warnung auslösen
 * (z.B. Rutsche fast voll, Fehler erkannt).
 */

#ifndef REGION_WARNUNG_WARTENAUFWARNUNG_H
#define REGION_WARNUNG_WARTENAUFWARNUNG_H

#include "RegionWarnungBasestate.h"

/**
 * State "WartenAufWarnung":
 *  - entspricht im Diagramm dem Zustand ohne aktives Warnsignal.
 *  - Transition:
 *      RTSCH_VOLL  ->  WarnsignalAktivieren
 */
class RegionWarnungWartenAufWarnung : public RegionWarnungBaseState {
public:
    void entry() override;

    TriggerProcessingState ec_rtsch_voll() override;

    void showState() override;
};

#endif // REGION_WARNUNG_WARTENAUFWARNUNG_H