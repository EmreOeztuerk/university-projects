/**
* @file EstopBaseState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Abstrakte Basisklasse für alle Zustände der EStop-Sub-State-Machine.
 *
 * Diese Klasse definiert:
 *  - Gemeinsame Entry- und Exit-Schnittstellen
 *  - Entry-Points (Default, Deep-History, E1, E2)
 *  - Trigger für EStop- und Reset-Events
 *  - Übergabemechanismus über TriggerProcessingState
 *
 * Konkrete EStop-Zustände (z. B. EStop1Aktiv, EStop2Aktiv)
 * erben von dieser Klasse und implementieren die jeweilige Logik.
 */

#ifndef ESTOPBASESTATE_H
#define ESTOPBASESTATE_H

#include "../contextdata.h"
#include "../actions.h"
#include "../subcommon/TriggerProcessingState.h"

#include <iostream>

using namespace std;

/**
 * @class EStopBaseState
 * @brief Abstrakte Basisklasse für EStop-Zustände.
 *
 * Stellt die gemeinsame Schnittstelle für alle Zustände
 * der EStop-FSM bereit.
 */
class EStopBaseState {
protected:
    /// Zugriff auf Kontextdaten (Status der E-Stop-Taster, Flags, etc.)
    ContextData *data;
    /// Zugriff auf Aktionsschnittstelle
    Actions *action;
    // EStopBaseState *estopstatemachine;
public:
    /**
     * @brief Virtueller Destruktor.
     */
    virtual ~EStopBaseState() {
    };

    /**
     * @brief Initialisierung der EStop-Sub-State-Machine.
     */
    virtual void initEStopStateMachines() {
    };

    /// Setzt die gemeinsamen Kontextdaten
    void setData(ContextData *data) { this->data = data; };

    /// Setzt die Aktionsschnittstelle
    void setAction(Actions *action) { this->action = action; };

    /// Kennzeichnet einen Pseudo-Start-Zustand
    virtual bool isPseudoStartState() { return false; };

    /// Kennzeichnet einen Pseudo-End-Zustand
    virtual bool isPseudoEndState() { return false; };

    //virtual void enterByDefaultEntryPoint(){entry();};

    /**
     * @brief Einstieg über den Deep-History-Entry-Point.
     *
     * Setzt die FSM in den zuletzt aktiven EStop-Sub-Zustand.
     */
    virtual void enterByDeepHistoryEntryPoint() { entry(); };

    /// Einstieg über Entry-Point E1
    virtual void enterByE1EntryPoint() {
    };

    /// Einstieg über Entry-Point E2
    virtual void enterByE2EntryPoint() {
    };

    /**
     * @brief Expliziter Einstieg in den Zustand EStop1Aktiv.
     */
    virtual void enterExplicitToEStop1Aktiv();

    /**
     * @brief Expliziter Einstieg in den Zustand EStop2Aktiv.
     */
    virtual void enterExplicitToEStop2Aktiv();

    /**
     * @brief Default-Entry-Point.
     *
     * Existiert nur, um Kompilierfehler zu vermeiden.
     */
    virtual void enterByDefaultEntryPoint() { entry(); };

    /**
     * @brief Aktionen beim Verlassen des Zustands.
     */
    virtual void leavingState() { exit(); };

    /**
     * @brief Eintritt über einen Pseudo-Start-Zustand.
     */
    virtual void enterViaPseudoStart();

    /**
     * @brief Eintritt über Deep-History.
     */
    virtual void enterViaDeepHistory();

    // --------------------------------------------------------
    // ENTRY / EXIT
    // --------------------------------------------------------
    virtual void entry() {
    };

    virtual void exit() {
    };

    // Trigger
    virtual TriggerProcessingState s_e1_ng() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_e2_ng() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_e1_g() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_e2_g() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_rt1_g() { return TriggerProcessingState::pending; };
    virtual TriggerProcessingState s_rt2_g() { return TriggerProcessingState::pending; };

    virtual void showState() = 0; // Just for illustration!
};

#endif /* ESTOPBASESTATE_H */