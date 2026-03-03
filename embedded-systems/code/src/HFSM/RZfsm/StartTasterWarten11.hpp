/**
* @file StartTasterWarten11.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zwischenzustand beim Warten auf Start (Anlage 1).
 *
 * Dient als Zwischenschritt in der Startsequenz.
 */

#ifndef STARTTASTERWARTEN11_HPP
#define STARTTASTERWARTEN11_HPP

#include "RZBaseState.hpp"
#include "../subcommon/TriggerProcessingState.h"

class StartTasterWarten11 : public RZBaseState {
public:
    void entry() override;

    virtual TriggerProcessingState s_st1_g() override;

    virtual void showState() override;
};


#endif //STARTTASTERWARTEN11_HPP