/**
* @file ResetTasterWarten2.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand: Warten auf Reset (Schritt 2).
 *
 * Bestätigung/zweiter Schritt der Reset-Prozedur.
 */

#ifndef RESETTASTERWARTEN2_HPP
#define RESETTASTERWARTEN2_HPP

#include "RZBaseState.hpp"
#include "../subcommon/TriggerProcessingState.h"
#include "Betriebsbereit.hpp"

class ResetTasterWarten2 : public RZBaseState {
public:
    void entry() override;

    virtual TriggerProcessingState s_rt2_g() override;

    void showState() override;
};


#endif //RESETTASTERWARTEN2_HPP