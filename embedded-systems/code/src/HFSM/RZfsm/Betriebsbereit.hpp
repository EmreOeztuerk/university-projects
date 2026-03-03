/**
 * @file Betriebsbereit.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Zustand Betriebsbereit.
 *
 * Die Anlage ist vollständig initialisiert und bereit, in den Betriebszustand zu wechseln.
 */

#ifndef BETRIEBSBEREIT_HPP
#define BETRIEBSBEREIT_HPP

#include "RZBaseState.hpp"
#include "../subcommon/TriggerProcessingState.h"


class Betriebsbereit : public RZBaseState {
public:
    void entry() override;

    virtual TriggerProcessingState s_st1_g() override;

    virtual TriggerProcessingState s_st2_g() override;

    void showState() override;
};


#endif //BETRIEBSBEREIT_HPP