/**
 * @file WartenReset1.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Description:
 * Zustand der EStop-Sub-State-Machine, in dem
 * auf den Reset-Taster 1 gewartet wird.
 */

#ifndef WARTENRESET1_H
#define WARTENRESET1_H

#include "EStopBaseState.h"

/**
 * @class WartenReset1
 * @brief Zustand: Warten auf Reset-Taster 1.
 */
class WartenReset1 : public EStopBaseState {
public:
    void entry() override;

    TriggerProcessingState s_e1_g() override;

    TriggerProcessingState s_e2_g() override;

    TriggerProcessingState s_rt1_g() override;

    void showState() override;
};

#endif /* WARTENRESET1_H */