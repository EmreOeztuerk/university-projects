/**
 * @file Wartezustand.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition des Wartezustands.
 *
 * Der initiale Zustand im Ruhezustand. Wartet auf Benutzerinteraktion, um
 * die Anlage betriebsbereit zu machen.
 */

#ifndef WARTEZUSTAND_HPP
#define WARTEZUSTAND_HPP

#include "RZBaseState.hpp"


class Wartezustand : public RZBaseState {
public:
    void entry() override;

    virtual TriggerProcessingState s_st1_lg() override;

    virtual TriggerProcessingState s_st2_lg() override;

    virtual TriggerProcessingState s_st1_g() override;

    virtual TriggerProcessingState s_st2_g() override;

    virtual TriggerProcessingState s_rt1_g() override;

    virtual TriggerProcessingState s_rt2_g() override;

    void showState() override;
};

#endif //WARTEZUSTAND_HPP