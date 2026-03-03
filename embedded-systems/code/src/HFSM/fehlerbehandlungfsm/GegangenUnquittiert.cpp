/**
* @file GegangenUnquittiert.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von GegangenUnquittiert.
 */

#include "GegangenUnquittiert.h"
#include "FehlerbehandlungPseudoEndState.h"
#include <iostream>

using namespace std;

void GegangenUnquittiert::entry() {
    action->cmd_led_amp_rot_bl();
    action->enteredGegangenUnquittiert();

    // --- Bedienerfreundliche Ausgabe Start ---
    cout << "\n#######################################################" << endl;
    cout << "STATUS: FEHLER GEGANGEN (Warten auf Quittierung)" << endl;
    cout << "#######################################################" << endl;
    cout << "-> Der Fehlergrund liegt nicht mehr vor." << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << "MASSNAHMEN:" << endl;
    cout << "-> Bitte Reset-Taste der Anlage 1 drücken." << endl;
    cout << "#######################################################\n" << endl;
    // --- Bedienerfreundliche Ausgabe Ende ---

    cout << "[Fehlerbehandlung] GegangenUnquittiert::entry()" << endl;
}

void GegangenUnquittiert::exit() {
    action->cmd_led_amp_rot_aus();
    cout << "[Fehlerbehandlung] GegangenUnquittiert::exit()" << endl;
}

TriggerProcessingState GegangenUnquittiert::s_rt1_g() {
    // Reset Fehler verlorenWS
    data->anzahlFehler = 0;
    data->setFehler(Fehlerart::keinFehler, FehlerOrt::DEFAULT);
    if (data->keineFehler()) {
        action->cmd_partner_ok_melden();
        leavingState();
        new(this) FehlerbehandlungPseudoEndState;
        enterByDefaultEntryPoint();
        cout << "[Fehlerbehandlung] GegangenUnquittiert::s_rt1_g()" << endl;
        return TriggerProcessingState::endstatereached;
    }
    return TriggerProcessingState::pending;
}

void GegangenUnquittiert::showState() {
    cout << "[Fehlerbehandlung]: GegangenUnquittiert" << endl;
}