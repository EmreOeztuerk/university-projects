/**
 * @file BaseState.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Abstrakte Basisklasse für alle Zustände der Anwendung.
 *
 * BaseState definiert:
 *  - Gemeinsame Schnittstellen für Entry-/Exit-Aktionen
 *  - Trigger-Methoden für Sensoren, Taster, Fehler und Timer
 *  - Verwaltung der Sub-State-Machines (EStop, Ruhezustabd, Servicemode,
 *                                  Betriebszustand, Fehlerbehandlung)
 *
 * Jede konkrete State-Klasse erbt von BaseState und überschreibt
 * nur die für sie relevanten Methoden.
 */

#ifndef BASESTATE_H
#define BASESTATE_H

#include "../estopfsm/EstopBaseState.h"
#include "../RZfsm/RZBaseState.hpp"
#include "../servicemode_fsm/servicemode_basestate.h"
#include "../betriebszustandfsm/BetriebszustandBaseState.h"
#include "../fehlerbehandlungfsm/FehlerbehandlungBaseState.h"

#include <iostream>
#include "../Actions.h"
#include "../ContextData.h"
using namespace std;

/**
 * @class BaseState
 * @brief Abstrakte Basisklasse für alle Zustände (FSM).
 *
 * Diese Klasse stellt alle möglichen Trigger und Lebenszyklus-Methoden
 * bereit. Standardimplementierungen sind leer, sodass Subklassen
 * nur relevante Trigger überschreiben müssen.
 */
class BaseState {
protected:
    /// Zugriff auf Kontextdaten (Sensorwerte, Status, Flags, etc.)
    ContextData *data;

    /// Zugriff auf Aktionsschnittstelle (Aktorik, Ausgaben, Meldungen)
    Actions *action;

    /// Sub-State-Machine für EStop
    EStopBaseState *estopstatemachine;

    /// Sub-State-Machine für Ruhezustand
    RZBaseState *rzstatemachine;

    /// Sub-State-Machine für Service-Mode
    Servicemode_BaseState *servicemode_statemachine;

    //	/// Sub-State-Machine für Service-Mode
    BetriebszustandBaseState *bzstatemachine;
    //
    //	/// Sub-State-Machine für Service-Mode
    FehlerbehandlungBaseState *fbstatemachine;

public:
    /**
     * @brief Virtueller Destruktor.
     *
     * Löscht dynamisch erzeugte Sub-State-Machines.
     */
    virtual ~BaseState() {
        delete estopstatemachine;
        //delete servicemode_statemachine;
        delete fbstatemachine;
        delete bzstatemachine;
    };

    /**
     * @brief Initialisierung aller Sub-State-Machines.
     *
     * Wird typischerweise beim Eintritt in einen Composite State aufgerufen.
     */
    virtual void initSubStateMachines();

    /// Setzt den gemeinsamen Kontext
    void setData(ContextData *data);

    /// Setzt die Aktionsschnittstelle
    void setAction(Actions *action);

    /// Kennzeichnet einen Pseudo-Start-Zustand
    virtual bool isPseudoStartState() { return false; };

    /// Kennzeichnet einen Pseudo-End-Zustand
    virtual bool isPseudoEndState() { return false; };

    /**
     * @brief Standard Entry-Point.
     *
     * Ruft per Default die entry()-Methode auf.
     */
    virtual void enterByDefaultEntryPoint() { entry(); };

    /**
     * @brief Deep-History Entry-Point.
     *
     * Wird genutzt, um in den zuletzt aktiven Sub-Zustand zurückzukehren.
     */
    virtual void enterByDeepHistoryEntryPoint() { entry(); };

    /// Spezifischer Entry-Point E1
    virtual void enterByE1EntryPoint() {
    };

    /// Spezifischer Entry-Point E2
    virtual void enterByE2EntryPoint() {
    };

    /**
     * Explizite Ausgaenge aus dem aktuellen Zustand zu E-Stop.
     */
    virtual void exitExplicitToEStop1() {
    };

    virtual void exitExplicitToEStop2() {
    };

    /**
     * Expliziter Ausgang aus dem aktuellen Zustand in den Betriebszustand.
     */
    virtual void exitExplicitToBetriebszustand() {
    };

    /**
     * Expliziter Ausgang aus dem aktuellen Zustand in den Ruhezustand.
     */
    virtual void exitExplicitToRuhezustand() {
    };

    /**
     * @brief Aktionen beim Verlassen des Zustands.
     */
    virtual void leavingState() {
        std::cout << "[Debug Root BaseState] leavingState" << std::endl;
        exit();
    };

    /**
     * @brief Eintritt über einen Pseudo-Start-Zustand.
     */
    virtual void enterViaPseudoStart();

    // no enterViaDeepHistory possible here

    /**
     * @brief Behandlung des Standard-Exits nach Sub-State-Ende.
     */
    virtual void handleDefaultExit(const TriggerProcessingState &handled) {
    };

    // entry/exit-actions
    virtual void entry() {
    };
    virtual void exit() { std::cout << "[Debug Root BaseState] exit" << std::endl; };

    // Trigger
    // --------------------------------------------------------
    // SENSOR-EVENTS
    // --------------------------------------------------------
    virtual void s_lsa_ub() {
    };
    virtual void s_lsa_nub() {
    };
    virtual void s_lsa1_ub() {
    }; // LSA1 unterbrochen
    virtual void s_lsa1_nub() {
    }; // LSA1 nicht unterbrochen

    virtual void s_lsa2_ub() {
    }; // LSA2 unterbrochen
    virtual void s_lsa2_nub() {
    }; // LSA2 nicht unterbrochen

    virtual void s_lsh_ub() {
    }; // LSH unterbrochen
    virtual void s_lsh_nub() {
    };

    virtual void s_lsw_ub() {
    }; // LSW unterbrochen
    virtual void s_lsw_nub() {
    };

    virtual void s_lsr_ub() {
    };

    virtual void s_lsr1_ub() {
    }; // LSR1 unterbrochen
    virtual void s_lsr1_nub() {
    };

    virtual void s_lsr2_ub() {
    }; // LSR2 unterbrochen
    virtual void s_lsr_nub() {
    };

    virtual void s_lse1_ub() {
    }; // LSE1 unterbrochen
    virtual void s_lse1_nub() {
    };

    virtual void s_lse2_ub() {
    }; // LSE2 unterbrochen
    virtual void s_lse2_nub() {
    };

    virtual void s_metal_an() {
    }; // Metallsensor
    virtual void s_hs_wert() {
    }; // Hoehensensor-Wert

    // --------------------------------------------------------
    // AKTORIK / TASTER-EVENTS
    // --------------------------------------------------------
    virtual void s_st_g() {
    }; // Start-Taster kurz gedrückt
    virtual void s_st1_g() {
    }; // Start-Taster kurz gedrückt
    virtual void s_st2_g() {
    }; // Start-Taster kurz gedrückt
    virtual void s_st1_lg() {
    }; // Start-Taster 1 lang gedrückt
    virtual void s_st2_lg() {
    };

    virtual void s_stp_g() {
    }; // Stop-Taster

    virtual void s_rt1_g() {
    }; // Reset-Taster Festo 1
    virtual void s_rt2_g() {
    }; // Reset-Taster Festo 2

    virtual void s_e1_g() {
    }; // E-Stop 1 gedrückt
    virtual void s_e1_ng() {
    }; // E-Stop 1 losgelassen

    virtual void s_e2_g() {
    }; // E-Stop 2 gedrückt
    virtual void s_e2_ng() {
    };

    // --------------------------------------------------------
    // VERBINDUNGSÜBERWACHUNG
    // --------------------------------------------------------
    virtual void s_vbdn() {
    }; // Verbindung vorhanden
    virtual void s_nvbdn() {
    }; // Nicht verbunden

    // --------------------------------------------------------
    // ÜBERGABE FESTO 1 → FESTO 2
    // --------------------------------------------------------
    virtual void s_uebergabe_ws() {
    };

    virtual void s_uebergabe_ws_fertig() {
    };

    virtual void s_festo2_bereit() {
    };

    virtual void s_festo2_nbereit() {
    };

    virtual void s_festo1_frei() {
    };

    virtual void s_festo1_nfrei() {
    };

    virtual void s_festo2_frei() {
    };

    virtual void s_festo1_stoppen() {
    };

    virtual void s_ws_an_festo2() {
    };

    virtual void s_festo2_nfrei() {
    };

    virtual void cmd_rtsch_nv() {
    };
    //virtual void cmd_rtsch2_nv();

    virtual void cmd_sp_zst_1() {
    };

    virtual void cmd_sp_zst_2() {
    };

    virtual void cmd_wh_zst_1() {
    };

    virtual void cmd_wh_zst_2() {
    };

    // --------------------------------------------------------
    // FEHLER-EVENTS
    // --------------------------------------------------------
    virtual void ec_vbdng_ub() {
    }; // Verbindungsfehler
    virtual void ec_ws_vrln() {
    }; // Werkstück verloren
    virtual void ec_ws_unb() {
    }; // Unbekanntes Werkstück
    virtual void ec_abstand_ne() {
    }; // Abstand nicht eingehalten
    virtual void ec_rtsch_voll() {
    }; // Rutsche voll

    virtual void cmd_ws_bekannt() {
    };

    virtual void cmd_ws_unbekannt() {
    };

    virtual void cmd_festos_frei() {
    };

    virtual void s_partner_fehler_1() {
    };

    virtual void s_partner_ok_1() {
    };

    virtual void s_partner_fehler_2() {
    };

    virtual void s_partner_ok_2() {
    };

    // --------------------------------------------------------
    // TIMER-EVENTS
    // --------------------------------------------------------
    virtual void t_lsa_lsh() {
    }; // Zeit zwischen LSA und LSH abgelaufen
    virtual void t_lsh_lsw() {
    }; // Zeit zwischen LSH und LSW abgelaufen
    virtual void t_lsw_lse() {
    }; // Zeit zwischen LSW und LSE abgelaufen
    virtual void t_lsw_lsr() {
    };

    virtual void t_lse1_lsa2() {
    }; // Zeit zwischen LSE1 und LSA2
    virtual void t_q1_lsh() {
    }; // Zeit Q1 → LSH
    virtual void t_lsa_q1() {
    }; // Zeit LSA → Q1
    virtual void t_toleranzzeit_lsa() {
    }; // Toleranzzeit bei Region LSA
    virtual void t_toleranzzeit_lsh() {
    }; // Toleranzzeit bei Region LSH
    virtual void t_toleranzzeit_lsw() {
    }; // Toleranzzeit bei Region LSW
    virtual void t_toleranzzeit_lse() {
    }; // Toleranzzeit bei Region LSE
    virtual void t_ping_pong() {
    }; // Ping-Pong Timer


    // Just for illustration!
    virtual void showState() {
    };
};

#endif /* BASESTATE_H */