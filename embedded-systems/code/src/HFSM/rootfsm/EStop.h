/**
 * @file EStop.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der EStop-Finite-State-Machine (FSM).
 * Diese Klasse repräsentiert den obersten Zustand (Composite State)
 * für den  Emergency Stop.
 *
 * Die Klasse erbt von BaseState und überschreibt alle relevanten
 * Entry-, Exit- und Trigger-Methoden für die EStop-Logik.
 */


#ifndef ESTOP_H
#define ESTOP_H

#include "BaseState.h"

/**
 * @class EStop
 * @brief Oberster Zustand der EStop-Zustandsmaschine.
 *
 * Dieser Zustand kapselt alle Sub-Zustände des Emergency-Stop-Systems
 * und behandelt:
 *  - Entry über verschiedene Entry-Points
 *  - Verlassen des Zustands
 *  - Trigger-basierte Übergänge
 *  - Anzeige des aktuellen Zustands
 */
class EStop : public BaseState {
private:
    //EStopBaseState *estopfsm;
public:
    /**
	 * @brief Einstieg über den Deep-History-Entry-Point.
	 *
	 * Der zuletzt aktive Sub-Zustand der EStop-FSM wird wiederhergestellt.
	 */
    void enterByDeepHistoryEntryPoint() override;

    /**
	 * @brief Einstieg über Entry-Point E1.
	 *
	 * Wird verwendet, wenn der EStop-Zustand explizit
	 * über den E1-Einstieg aktiviert wird.
	 */
    void enterByE1EntryPoint() override;

    /**
	 * @brief Einstieg über Entry-Point E2.
	 *
	 * Alternativer Einstiegspunkt in die EStop-FSM.
	 */
    void enterByE2EntryPoint() override;

    /**
	 * @brief Aktionen beim Verlassen des EStop-Zustands.
	 *
	 * Wird aufgerufen, bevor der Zustand vollständig verlassen wird.
	 */
    void leavingState() override;

    /**
	 * @brief Standard-Exit nach Beendigung eines Sub-Zustands.
	 *
	 * @param handled Referenz auf den verarbeiteten Trigger-Zustand
	 */
    void handleDefaultExit(const TriggerProcessingState &handled) override; // Nach Sub-State-Ende
    /**
	 * @brief Entry-Aktion des EStop-Zustands.
	 *
	 * Wird beim Eintritt in den EStop-Zustand ausgeführt,
	 * unabhängig vom verwendeten Entry-Point.
	 */
    void entry() override;


    //Trigger
    void s_e1_g() override;

    void s_e2_g() override;

    void s_e1_ng() override;

    void s_e2_ng() override;

    void s_rt1_g() override;

    void s_rt2_g() override;


    /**
	 * @brief Ausgabe des aktuellen Zustands (Debug / Logging).
	 */
    void showState() override;
};

#endif /* ESTOP_H */