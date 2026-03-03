/**
 * @file RegionWarnungWarnsignalAktivieren.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warnsignal aktiv.
 *
 * Eine Warnung liegt vor. Die Ampeln blinken (gelb) oder leuchten entsprechend,
 * um den Bediener zu informieren.
 */

#ifndef REGIONWARNUNGWARNSIGNALAKTIVIEREN_H
#define REGIONWARNUNGWARNSIGNALAKTIVIEREN_H

#include "RegionWarnungBasestate.h"

class RegionWarnungWarnsignalAktivieren : public RegionWarnungBaseState {
public:
    void entry() override;

    void exit() override;

    // Für RTSCH_NV / "Rutsche nicht mehr voll"
    TriggerProcessingState cmd_rtsch_nv() override;

    void showState() override;
};

#endif //REGIONWARNUNGWARNSIGNALAKTIVIEREN_H