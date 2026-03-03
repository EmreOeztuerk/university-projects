/**
 * @file AnstehendQuittiert.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von AnstehendQuittiert.
 */

#include "AnstehendQuittiert.h"
#include "FehlerbehandlungPseudoEndState.h"
#include "../common/Types.h"
#include <iostream>


using namespace std;


void AnstehendQuittiert::entry() {
    //action->cmd_led_amp_rot_aus(); // rote lampe aus
    action->cmd_led_amp_rot_an();
    action->enteredAnstehendQuittiert();
    data->setFehler(Fehlerart::keinFehler, FehlerOrt::DEFAULT);
    data->anzahlFehler = 0; // loescht alle fehler, wenn mehrere vorhanden sind

    // --- Bedienerfreundliche Ausgabe Start ---
    cout << "\n#######################################################" << endl;
    cout << "STATUS: FEHLER QUITTIERT (Bereit zum Start)" << endl;
    cout << "#######################################################" << endl;
    cout << "-> Fehlerart: Kein Fehler (Fehler wurde zurückgesetzt)" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "MASSNAHMEN:" << endl;
    cout << "-> Bitte Start-Taste der Anlage 1 drücken, um fortzufahren." << endl;
    cout << "#######################################################\n" << endl;
    // --- Bedienerfreundliche Ausgabe Ende ---

    cout << "[Fehlerbehandlung] AnstehendQuittiert::entry()" << endl;
}

void AnstehendQuittiert::exit() {
    action->cmd_led_amp_rot_aus();

    cout << "[Fehlerbehandlung] AnstehendQuittiert::exit()" << endl;
}


TriggerProcessingState AnstehendQuittiert::s_st_g() {
    if (data->anzahlFehler == 0) {
        leavingState();
        action->cmd_partner_ok_melden();
        new(this) FehlerbehandlungPseudoEndState;
        enterByDefaultEntryPoint();
        cout << "[Fehlerbehandlung] AnstehendQuittiert::s_st_g()" << endl;
        return TriggerProcessingState::endstatereached;
    }
    return TriggerProcessingState::pending;
}


void AnstehendQuittiert::showState() {
    cout << "[Fehlerbehandlung]: AnstehendQuittiert" << endl;
}