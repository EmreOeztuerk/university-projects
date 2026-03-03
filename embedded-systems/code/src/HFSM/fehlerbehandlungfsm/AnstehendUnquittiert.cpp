/**
* @file AnstehendUnquittiert.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung von AnstehendUnquittiert.
 */

#include "AnstehendUnquittiert.h"
#include "AnstehendQuittiert.h"
#include "gegangenUnquittiert.h"

using namespace std;

void AnstehendUnquittiert::entry() {
    //action->cmd_led_amp_rot_an();
    action->cmd_led_amp_rot_bs();
    action->enteredAnstehendUnquittiert();
    ContextData::FehlerInfo info = data->getLetzterFehler();
    data->setFehler(info.art, info.ort); // nicht nötig, weil wir das in bz machen
    data->anzahlFehler++;

    // --- Bedienerfreundliche Ausgabe Start ---
    cout << "\n#######################################################" << endl;
    cout << "ACHTUNG: FEHLER AUFGETRETEN (Anstehend Unquittiert)" << endl;
    cout << "#######################################################" << endl;

    // Fehlerart als Text ausgeben
    cout << "-> Fehlerart: ";
    switch (info.art) {
        case Fehlerart::neuWS: cout << "Neues Werkstück zu früh (neuWS)";
            break;
        case Fehlerart::verlorenWS: cout << "Werkstück verloren (verlorenWS)";
            break;
        case Fehlerart::rutscheVoll: cout << "Rutsche voll";
            break;
        case Fehlerart::abstandZuKlein: cout << "Abstand zu klein";
            break;
        case Fehlerart::verbindungUnterbrochen: cout << "Verbindung unterbrochen";
            break;
        case Fehlerart::externerFehler: cout << "Externer Fehler (Andere Anlage)";
            break;
        case Fehlerart::keinFehler: cout << "Kein Fehler";
            break;
        default: cout << "Unbekannt (" << (int) info.art << ")";
            break;
    }
    cout << endl;

    // Fehlerort als Text ausgeben
    cout << "-> Fehlerort: ";
    switch (info.ort) {
        case FehlerOrt::LSA: cout << "Lichtschranke Anfang (LSA)";
            break;
        case FehlerOrt::LSA1: cout << "Lichtschranke Anfang 1 (LSA1)";
            break;
        case FehlerOrt::LSA2: cout << "Lichtschranke Anfang 2 (LSA2)";
            break;
        case FehlerOrt::LSH: cout << "Lichtschranke Höhe (LSH)";
            break;
        case FehlerOrt::LSW: cout << "Lichtschranke Weiche (LSW)";
            break;
        case FehlerOrt::LSE: cout << "Lichtschranke Ende (LSE)";
            break;
        case FehlerOrt::LSE1: cout << "Lichtschranke Ende 1 (LSE1)";
            break;
        case FehlerOrt::LSE2: cout << "Lichtschranke Ende 2 (LSE2)";
            break;
        case FehlerOrt::LSR: cout << "Lichtschranke Rutsche (LSR)";
            break;
        case FehlerOrt::LSH_BIS_LSW: cout << "Bereich LSH bis LSW";
            break;
        case FehlerOrt::LSW_BIS_LSE: cout << "Bereich LSW bis LSE";
            break;
        case FehlerOrt::ANDEREMACHINE: cout << "Andere Maschine";
            break;
        default: cout << "Unbekannter Ort (" << (int) info.ort << ")";
            break;
    }
    cout << endl;

    cout << "-------------------------------------------------------" << endl;
    cout << "MASSNAHMEN:" << endl;
    cout << "1. Bitte das betroffene Werkstück vom Förderband entnehmen." << endl;
    cout << "2. Bitte Reset-Taste der Anlage 1 drücken." << endl;
    cout << "#######################################################\n" << endl;
    // --- Bedienerfreundliche Ausgabe Ende ---

    cout << "[Fehlerbehandlung] AnstehendUnquittiert::entry() Fehler="
            << data->anzahlFehler << endl;
}


void AnstehendUnquittiert::exit() {
    action->cmd_led_amp_rot_aus();

    cout << "[Fehlerbehandlung] AnstehendUnquittiert::exit()" << endl;
}

TriggerProcessingState AnstehendUnquittiert::s_rt1_g() {
    leavingState();
    new(this) AnstehendQuittiert;
    enterByDefaultEntryPoint();
    return TriggerProcessingState::consumed;
}

TriggerProcessingState AnstehendUnquittiert::s_lsr_nub() {
    cout << "[AnstehendUnquittiert] s_lsr_nub" << endl;
    // Zugriff auf die Fehlerdaten
    auto fehler = data->getLetzterFehler();
    cout << "Debug: Art=" << (int) fehler.art << " Ort=" << (int) fehler.ort << endl;
    // Prüfen: Ist der Fehler "Rutsche Voll" UND ist er an der "Rutsche"?
    if (fehler.art == Fehlerart::rutscheVoll && fehler.ort == FehlerOrt::LSR) {
        cout << "[AnstehendUnquittiert] Guard OK: Rutsche wurde geleert." << endl;

        leavingState();
        new(this) GegangenUnquittiert;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }
    // Guard nicht erfüllt -> Event ignorieren oder an Super-State weiterreichen
    return TriggerProcessingState::pending;
}

TriggerProcessingState AnstehendUnquittiert::s_lsa2_ub() {
    auto fehler = data->getLetzterFehler();
    cout << "Debug: Art=" << (int) fehler.art << " Ort=" << (int) fehler.ort << endl;
    // Prüfen: Fehler am LSA2 (Festo 2 Eingang) UND Werkstück verloren
    if (fehler.ort == FehlerOrt::LSA2 && fehler.art == Fehlerart::verlorenWS) {
        cout << "[AnstehendUnquittiert] Guard OK: LSA2 unterbrochen (WS wieder da)." << endl;
        data->FehlerVerlorenBeiLAS2 = true;
        leavingState();
        new(this) GegangenUnquittiert;
        enterByDefaultEntryPoint();
        return TriggerProcessingState::consumed;
    }
    return TriggerProcessingState::pending;
}

void AnstehendUnquittiert::showState() {
    cout << "    FB: Anstehend_Unquittiert" << endl;
}