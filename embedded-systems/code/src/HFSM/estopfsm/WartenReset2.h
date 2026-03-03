/**
 * @file WartenReset2.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand der EStop-Sub-State-Machine, in dem
 * auf den Reset-Taster 2 gewartet wird.
 */

#ifndef WARTENRESET2_H
#define WARTENRESET2_H

#include "estopbasestate.h"

/**
 * @class WartenReset2
 * @brief Zustand: Warten auf Reset-Taster 2.
 */
class WartenReset2 : public EStopBaseState {
public:
    void entry() override;

    TriggerProcessingState s_e1_g() override;

    TriggerProcessingState s_e2_g() override;

    TriggerProcessingState s_rt2_g() override;

    void showState() override;
};

#endif /* WARTENRESET2_H */