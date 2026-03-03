/**
 * @file Context.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Context-Klasse.
 */

#include "Context.h"
#include "ContextData.h"
#include "rootfsm/pseudostartstate.h"
#include "rootfsm/Ruhezustand.h"
#include "../Dispatcher/signal.hpp"
#include "../Dispatcher/Timer.hpp"
#include "estopfsm/estopbasestate.h"
#include "rootfsm/Ruhezustand.h"
#include <chrono>

Context::Context(Actions *shared_action, bool Festo1, bool Festo2)
    : state(nullptr), action(shared_action), attachHFSM(nullptr), data(Festo1, Festo2, shared_action), hatMetall(false),
      festo1(false), festo2(false) {
    if (shared_action == nullptr) {
        std::cerr << "FEHLER: shared_action ist NULL!" << std::endl;
    }
    state = new PseudoStartState();
    state->initSubStateMachines();

    //	 state = new Ruhezustand();
    //	 state->initSubStateMachines();

    state->setData(&data);
    state->setAction(shared_action);
    std::cout << "[Context]Festo1: " << Festo1 << std::endl;
    std::cout << "[Context]Festo2: " << Festo2 << std::endl;

    if (Festo1) {
        // data.festo1 = Festo1;
        festo1 = Festo1;
    }

    if (Festo2) {
        //data.festo2= Festo2;
        festo2 = Festo2;
    }

    //initFSM();
}

Context::~Context() {
    delete state;
}

/* --------------------------------------------------------
 * SENSOR
 * --------------------------------------------------------*/

void Context::s_lsa1_ub() { state->s_lsa1_ub(); }
void Context::s_lsa2_ub() { state->s_lsa2_ub(); }
void Context::s_lsa_nub() { state->s_lsa_nub(); }

void Context::s_lsh_ub() { state->s_lsh_ub(); }
void Context::s_lsh_nub() { state->s_lsh_nub(); }

void Context::s_lsw_ub() { state->s_lsw_ub(); }
void Context::s_lsw_nub() { state->s_lsw_nub(); }

void Context::s_lsr_ub() { state->s_lsr_ub(); }
void Context::s_lsr_nub() { state->s_lsr_nub(); }

void Context::s_lse1_ub() { state->s_lse1_ub(); }
void Context::s_lse1_nub() { state->s_lse1_nub(); }

void Context::s_lse2_ub() { state->s_lse2_ub(); }
void Context::s_lse2_nub() { state->s_lse2_nub(); }

void Context::s_metal_an() { state->s_metal_an(); }
// void Context::s_metal_aus() { state->s_metal_aus(); }

/* --------------------------------------------------------
 * AKTORIK / TASTER
 * --------------------------------------------------------*/

void Context::s_st_g() { state->s_st_g(); }
void Context::s_st1_g() { state->s_st1_g(); }
void Context::s_st2_g() { state->s_st2_g(); }
void Context::s_st1_lg() { state->s_st1_lg(); }
void Context::s_st2_lg() { state->s_st2_lg(); }

void Context::s_stp_g() { state->s_stp_g(); }

void Context::s_rt1_g() { state->s_rt1_g(); }
void Context::s_rt2_g() { state->s_rt2_g(); }

void Context::s_e1_g() { state->s_e1_g(); }
void Context::s_e1_ng() { state->s_e1_ng(); }

void Context::s_e2_g() { state->s_e2_g(); }
void Context::s_e2_ng() { state->s_e2_ng(); }

/* --------------------------------------------------------
 * VERBINDUNGSÜBERWACHUNG
 * --------------------------------------------------------*/

void Context::s_vbdn() { state->s_vbdn(); }
void Context::s_nvbdn() { state->s_nvbdn(); }

/* --------------------------------------------------------
 * ÜBERGABE / FESTO
 * --------------------------------------------------------*/

void Context::s_uebergabe_ws() { state->s_uebergabe_ws(); }
void Context::s_uebergabe_ws_fertig() { state->s_uebergabe_ws_fertig(); }

void Context::s_festo2_bereit() {
    data.f2_voll = false;
    state->s_festo2_bereit();
}

void Context::s_festo2_nbereit() {
    data.f2_voll = true;
    state->s_festo2_nbereit();
}

void Context::s_festo1_frei() {
    data.festo1Frei = true;
    state->s_festo1_frei();
}

void Context::s_festo1_nfrei() {
    data.festo1Frei = false;
    state->s_festo1_nfrei();
}

void Context::s_festo2_frei() {
    data.festo2Frei = true;
    state->s_festo2_frei();
}

void Context::s_festo2_nfrei() {
    data.festo2Frei = false;
    state->s_festo2_nfrei();
}

void Context::s_ws_an_festo2() { state->s_ws_an_festo2(); }

void Context::s_festo1_stoppen() { state->s_festo1_stoppen(); }
/* --------------------------------------------------------
 * ZUSTAND DER ANLAGE
 * --------------------------------------------------------*/

void Context::cmd_sp_zst_1() { state->cmd_sp_zst_1(); }
void Context::cmd_sp_zst_2() { state->cmd_sp_zst_2(); }

void Context::cmd_wh_zst_1() { state->cmd_wh_zst_1(); }
void Context::cmd_wh_zst_2() { state->cmd_wh_zst_2(); }

/* --------------------------------------------------------
 * FEHLER
 * --------------------------------------------------------*/

void Context::ec_vbdng_ub() {
    // Mapping: Verbindungsabbruch -> Ort oft unbekannt oder global
    //data.setFehler(Fehlerart::verbindungUnterbrochen, FehlerOrt::DEFAULT);
    state->ec_vbdng_ub();
}

void Context::ec_ws_vrln() {
    // Mapping: Werkstück verloren -> Ort Default (da wir nicht wissen wo genau)
    //data.setFehler(Fehlerart::verlorenWS, FehlerOrt::DEFAULT);
    state->ec_ws_vrln();
}

void Context::ec_ws_unb() {
    // Mapping: Unbekanntes WS / Neu -> Passiert meist am Anfang (LSA)
    //data.setFehler(Fehlerart::neuWS, FehlerOrt::DEFAULT);
    state->ec_ws_unb();
}

// void Context::ec_ws_verschw() {
//     data.setFehler(Fehlerart::verlorenWS, FehlerOrt::DEFAULT);
//     state->ec_ws_verschw();
// }
void Context::ec_abstand_ne() {
    // Mapping: Abstand zu klein -> Passiert meist am Einlauf
    //data.setFehler(Fehlerart::abstandZuKlein, FehlerOrt::LSA);
    state->ec_abstand_ne();
}

void Context::ec_rtsch_voll() {
    // Mapping: Rutsche voll -> Ort ist definitiv LSR (Rutsche)
    //data.setFehler(Fehlerart::rutscheVoll, FehlerOrt::LSR);
    data.rutsche_voll = true;
    state->ec_rtsch_voll();
}


/* --------------------------------------------------------
 * WERKSTÜCKE
 * --------------------------------------------------------*/

void Context::cmd_ws_bekannt() { state->cmd_ws_bekannt(); }
void Context::cmd_ws_unbekannt() { state->cmd_ws_unbekannt(); }

/* --------------------------------------------------------
 * FESTO STATUS
 * --------------------------------------------------------*/

void Context::cmd_festos_frei() { state->cmd_festos_frei(); }

void Context::s_partner_fehler_1() { state->s_partner_fehler_1(); }
void Context::s_partner_ok_1() { state->s_partner_ok_1(); }

void Context::s_partner_fehler_2() { state->s_partner_fehler_2(); }
void Context::s_partner_ok_2() { state->s_partner_ok_2(); }

void Context::cmd_rtsch_nv() { state->cmd_rtsch_nv(); }
//void Context::cmd_rtsch2_nv(){state->cmd_rtsch2_nv()}

/* --------------------------------------------------------
 * TIMER
 * --------------------------------------------------------*/

void Context::t_lsa_lsh() { state->t_lsa_lsh(); }
void Context::t_lsh_lsw() { state->t_lsh_lsw(); }
void Context::t_lsw_lse() { state->t_lsw_lse(); }
void Context::t_lse1_lsa2() { state->t_lse1_lsa2(); }
void Context::t_lsw_lsr() { state->t_lsw_lsr(); }
void Context::t_q1_lsh() { state->t_q1_lsh(); }
void Context::t_lsa_q1() { state->t_lsa_q1(); }
void Context::t_toleranzzeit_lsa() { state->t_toleranzzeit_lsa(); }
void Context::t_toleranzzeit_lsh() { state->t_toleranzzeit_lsh(); }
void Context::t_toleranzzeit_lsw() { state->t_toleranzzeit_lsw(); }
void Context::t_toleranzzeit_lse() { state->t_toleranzzeit_lse(); }
void Context::t_ping_pong() { state->t_ping_pong(); }


void Context::handleEvent(Signal sig, int wert) {
    switch (sig) {
        /* -----------------------------
         *  SENSOREN
         * ----------------------------- */
        case Signal::S_LSA1_UB:
            if (festo1) {
                data.registerId1();
            } //data.zeitStarten();
            //std::cout << "[Debug Context] S_LSA1_NUB, NUR FÜR ZEITMESSUNG" << std::endl;

            s_lsa1_ub();
            std::cout << "[Debug Context] S_LSA2_UB" << std::endl;
            break;

        case Signal::S_LSA2_UB:
            //case Signal::S_LSA_UB:      // falls du so einen Eintrag hast
            if (festo2) {
                data.registerId1();
            }
            std::cout << "[Debug Context] S_LSA2_UB" << std::endl;
            s_lsa2_ub();

            //data.zeitSpeichern("t_lse1_lsa2_s" );
            //data.zeitStarten();
            //data.registerId1();
            break;

        case Signal::S_LSA1_NUB:
        case Signal::S_LSA2_NUB:
            //case Signal::S_LSA_NUB
            std::cout << "[Test] LSA2 erreicht - zeitStarten()" << std::endl;

            // data.zeitSpeichern("t_lse1_lsa2_s" );
            //data.zeitStarten();
            //
            std::cout << "[Debug Context] S_LSA2/1_NUB" << std::endl;
            s_lsa_nub();
            break;

        case Signal::S_LSH_UB:
            std::cout << "[Debug Context] S_LSH_UB" << std::endl;
            s_lsh_ub();

            //data.zeitSpeichern("t_lsa_lsh_s" );
            //data.zeitStarten();
            break;

        case Signal::S_LSH_NUB:
            std::cout << "[Debug Context] S_LSH_NUB" << std::endl;
            s_lsh_nub();
            break;

        case Signal::S_LSW_UB:
            std::cout << "[Debug Context] S_LSW_UB" << std::endl;
            s_lsw_ub();
            //data.registerId3(hatMetall);
            hatMetall = false; // default
            break;

        case Signal::S_LSW_NUB:
            std::cout << "[Debug Context] S_LSW_NUB" << std::endl;
            s_lsw_nub();
            break;

        case Signal::S_LSR1_UB:
        case Signal::S_LSR2_UB:
            //case Signal::S_LSR_UB:
            std::cout << "[Debug Context] S_LSR_UB" << std::endl;
            s_lsr_ub();
            break;

        case Signal::S_LSR1_NUB:
            std::cout << "[Debug Context] S_LSR1_NUB" << std::endl;
            //data.rutsche_voll = false;
            s_lsr_nub();
            break;

        case Signal::S_LSR2_NUB:
            //case Signal::S_LSR_NUB:
            std::cout << "[Debug Context] S_LSR2_NUB" << std::endl;
            //data.rutsche2_voll = false;
            s_lsr_nub();
            break;

        case Signal::S_LSE1_UB:
            std::cout << "[Debug Context] S_LSE1_UB" << std::endl;
            s_lse1_ub();

            //data.zeitSpeichern("t_lsm_lse_s" );
            //data.zeitStarten();
            break;

        case Signal::S_LSE1_NUB:
            std::cout << "[Debug Context] S_LSE1_NUB" << std::endl;
            s_lse1_nub();
            break;

        case Signal::S_LSE2_UB:
            std::cout << "[Debug Context] S_LSE2_UB" << std::endl;
            s_lse2_ub();

            //data.zeitSpeichern("t_lsm_lse_s" );
            //data.zeitStarten();
            break;

        case Signal::S_LSE2_NUB:
            std::cout << "[Debug Context] S_LSE2_NUB" << std::endl;
            s_lse2_nub();
            break;

        case Signal::S_METAL_AN:
            s_metal_an();
            hatMetall = true;

            //if(erstes){
            data.registerId3(hatMetall);
            // erstes = false;
            //}
            std::cout << "[Debug Context] S_METAL_AN" << std::endl;

            //data.zeitSpeichern("t_lsa_lsh_s" );
            //data.zeitStarten();
            break;
        //case Signal::S_METAL_AUS:
        //     s_metal_aus();
        //     break;

        /* -----------------------------
         *  TASTER / AKTORIK
         * ----------------------------- */
        case Signal::S_ST_G:
            std::cout << "[Debug Context] S_ST_G" << std::endl;
            s_st_g();
            break;
        case Signal::S_ST1_G:
            std::cout << "[Debug Context] S_ST1_G" << std::endl;
            s_st1_g();
            s_st_g();
            break;
        case Signal::S_ST2_G:
            std::cout << "[Debug Context] S_ST2_G" << std::endl;
            s_st2_g();

            break;
        case Signal::S_ST1_LG:
            std::cout << "[Debug Context] S_ST1_LG" << std::endl;
            s_st1_lg();
            break;
        case Signal::S_ST2_LG:
            std::cout << "[Debug Context] S_ST2_LG" << std::endl;
            s_st2_lg();
            break;

        case Signal::S_STP_G:
            std::cout << "[Debug Context] S_STP_G" << std::endl;
            s_stp_g();
            break;

        case Signal::S_RT1_G:
            std::cout << "[Debug Context] S_RT1_G" << std::endl;
            s_rt1_g();
            break;
        case Signal::S_RT2_G:
            std::cout << "[Debug Context] S_RT2_G" << std::endl;
            s_rt2_g();
            break;

        case Signal::S_E1_G:
            std::cout << "[Debug Context] S_E1_G" << std::endl;
            s_e1_g();
            break;
        case Signal::S_E1_NG:
            std::cout << "[Debug Context] S_E1_NG" << std::endl;
            s_e1_ng();
            break;

        case Signal::S_E2_G:
            std::cout << "[Debug Context] S_E2_G" << std::endl;
            s_e2_g();
            break;
        case Signal::S_E2_NG:
            std::cout << "[Debug Context] S_E2_NG" << std::endl;
            s_e2_ng();
            break;

        /* -----------------------------
         *  VERBINDUNG
         * ----------------------------- */
        case Signal::S_VBDN:
            s_vbdn();
            break;
        case Signal::S_NVBDN:
            s_nvbdn();
            break;

        /* -----------------------------
         *  ÜBERGABE / FESTO
         * ----------------------------- */
        case Signal::S_UEBERGABE_WS:
            std::cout << "[Debug Context] S_UEBERGABE_WS" << std::endl;
            if (festo2) {
                std::cout << "[Context] Nachricht von F1. Typ: " << wert << std::endl;
                data.empfangeTypVonF1(wert);
            }
            s_uebergabe_ws();
            break;
        case Signal::S_UEBERGABE_WS_FERTIG:
            std::cout << "[Debug Context] S_UEBERGABE_WS_FERTIG" << std::endl;
            s_uebergabe_ws_fertig();
            break;

        case Signal::S_FESTO2_BEREIT:
            std::cout << "[Debug Context] S_FESTO2_BEREIT" << std::endl;
            s_festo2_bereit();
            data.f2_bereit = true;
            break;
        case Signal::S_FESTO2_NBEREIT:
            std::cout << "[Debug Context] S_FESTO2_NBEREIT" << std::endl;
            s_festo2_nbereit();
            data.f2_bereit = false;
            break;

        case Signal::S_FESTO1_FREI:
            std::cout << "[Debug Context] S_FESTO1_FREI" << std::endl;
            s_festo1_frei();
            break;
        case Signal::S_FESTO1_STOPPEN:
            std::cout << "[Debug Context] S_FESTO1_STOPPEN" << std::endl;
            s_festo1_stoppen();
            break;

        case Signal::S_FESTO1_NFREI:
            std::cout << "[Debug Context] S_FESTO1_NFREI" << std::endl;
            s_festo1_nfrei();
            break;

        case Signal::S_FESTO2_FREI:
            std::cout << "[Debug Context] S_FESTO2_FREI" << std::endl;
            s_festo2_frei();
            break;

        case Signal::S_FESTO2_NFREI:
            std::cout << "[Debug Context] S_FESTO2_NFREI" << std::endl;
            s_festo2_nfrei();
            break;

        case Signal::CMD_FESTOS_FREI:
            std::cout << "[Debug Context] S_FESTOS_FREI" << std::endl;
            cmd_festos_frei();

            break;
        case Signal::S_WS_AN_FESTO2:
            std::cout << "[Debug Context] S_WS_AN_FESTO2" << std::endl;
            s_ws_an_festo2();

            break;
        case Signal::CMD_RTSCH1_NV:
            std::cout << "[Debug Context] CMD_RTSCH1_NV" << std::endl;
            data.rutsche_voll = false;
            cmd_rtsch_nv();
            break;
        case Signal::CMD_RTSCH2_NV:
            std::cout << "[Debug Context] CMD_RTSCH2_NV" << std::endl;
            data.rutsche2_voll = false;
            if (festo2) {
                data.rutsche_voll = false;
                cmd_rtsch_nv();
            }

            break;

        /* -----------------------------
         *  FEHLER
         * ----------------------------- */
        case Signal::EC_VBDNG_UB:
            ec_vbdng_ub();
            break;
        case Signal::EC_WS_VRLN:
            ec_ws_vrln();
            break;
        case Signal::EC_WS_UNB:
            ec_ws_unb();
            break;
        case Signal::EC_ABSTAND_NE:
            ec_abstand_ne();
            break;
        case Signal::EC_RTSCH_VOLL:
            std::cout << "[Debug Context] EC_RTSCH_VOLL" << std::endl;
            data.rutsche_voll = true;
            ec_rtsch_voll();
            break;
        case Signal::EC_RTSCH1_VOLL:
            std::cout << "[Debug Context] EC_RTSCH1_VOLL" << std::endl;
            data.rutsche_voll = true;
            ec_rtsch_voll();
            break;
        case Signal::EC_RTSCH2_VOLL:
            std::cout << "[Debug Context] EC_RTSCH2_VOLL" << std::endl;
            data.rutsche2_voll = true;
            if (festo2) {
                data.rutsche_voll = true;
                ec_rtsch_voll();
            }
            break;
        case Signal::S_PARTNER_FEHLER_1:
            std::cout << "[Debug Context] S_PARTNER_FEHLER_1" << std::endl;
            s_partner_fehler_1();
            break;
        case Signal::S_PARTNER_OK_1:
            std::cout << "[Debug Context] S_PARTNER_OK_1" << std::endl;
            s_partner_ok_1();
            break;
        case Signal::S_PARTNER_FEHLER_2:
            std::cout << "[Debug Context] S_PARTNER_FEHLER_2" << std::endl;
            s_partner_fehler_2();
            break;
        case Signal::S_PARTNER_OK_2:
            std::cout << "[Debug Context] S_PARTNER_OK_2" << std::endl;
            s_partner_ok_2();
            break;


        /* -----------------------------
         *  TIMER (wert ggf. egal)
         * ----------------------------- */
        case Signal::T_LSA_LSH:
            std::cout << "[Debug Context] T_LSA_LSH" << std::endl;
            t_lsa_lsh();
            break;
        case Signal::T_LSH_LSM:
            std::cout << "[Debug Context] T_LSH_LSM" << std::endl;
            t_lsh_lsw();
            break;
        case Signal::T_LSW_LSE:
            std::cout << "[Debug Context] T_LSW_LSE" << std::endl;
            t_lsw_lse();
            break;
        case Signal::T_LSW_LSR:
            std::cout << "[Debug Context] T_LSW_LSR" << std::endl;
            t_lsw_lsr();
            break;
        case Signal::T_LSE1_LSA2:
            std::cout << "[Debug Context] T_LSE1_LSA2" << std::endl;
            t_lse1_lsa2();
            break;
        case Signal::T_Q1_LSH:
            std::cout << "[Debug Context] T_Q1_LSH" << std::endl;
            t_q1_lsh();
            break;
        case Signal::T_LSA_Q1:
            std::cout << "[Debug Context] T_LSA_Q1" << std::endl;
            t_lsa_q1();
            break;

        case Signal::T_TOLERANZZEIT_LSA:
            std::cout << "[Debug Context] T_TOLERANZZEIT_LSA" << std::endl;
            t_toleranzzeit_lsa();
            break;
        case Signal::T_TOLERANZZEIT_LSH:
            std::cout << "[Debug Context] T_TOLERANZZEIT_LSH" << std::endl;
            t_toleranzzeit_lsh();
            break;
        case Signal::T_TOLERANZZEIT_LSW:
            std::cout << "[Debug Context] T_TOLERANZZEIT_LSW" << std::endl;
            t_toleranzzeit_lsw();
            break;
        case Signal::T_TOLERANZZEIT_LSE:
            std::cout << "[Debug Context] T_TOLERANZZEIT_LSE" << std::endl;
            t_toleranzzeit_lse();
            break;
        case Signal::T_PING_PONG:
            t_ping_pong();
            break;

        /* -----------------------------
         *  SPEZIAL: Wert benutzen (z.B. Höhen-Sensor) WERKSTUECKTYP
         * ----------------------------- */
        case Signal::WERKSTUECK_MIT_BOHRUNG:
            data.wsTyp = sig;
            std::cout << "[Debug Context] WERKSTUECK_MIT_BOHRUNG" << std::endl;
            data.registerId2(sig);

            break;
        case Signal::WERKSTUECK_FLACH:
            data.wsTyp = sig;
            std::cout << "[Debug Context] WERKSTUECK_FLACH" << std::endl;
            data.registerId2(sig);

            break;
        case Signal::WERKSTUECK_HOCH:
            data.wsTyp = sig;
            std::cout << "[Debug Context] WERKSTUECK_HOCH" << std::endl;
            data.registerId2(sig);

            break;
        case Signal::WERKSTUECK_UNBEKANNT:
            data.wsTyp = sig;
            std::cout << "[Debug Context] WERKSTUECK_UNBEKANNT" << std::endl;
            data.registerId2(sig);
            break;
        case Signal::WERKSTUECK_ANDERS:
            std::cout << "[Debug Context] WERKSTUECK_ANDERS" << std::endl;
            if (data.order_index > 0) {
                data.order_index--;
            } else {
                if (!data.sollReihenfolge.empty()) {
                    data.order_index = data.sollReihenfolge.size() - 1;
                }
            }
            break;
        default:
            std::cout << "[Context] Unbekanntes Signal: " << (int) sig << std::endl;
            break;
    }
}

void Context::run() {
    initFSM();

    // HFSM-Channel erzeugen
    if (festo1) {
        attachHFSM = name_attach(nullptr, ATTACH_POINT_HFSM1, 0);
        if (!attachHFSM) {
            std::cerr << "[HFSM] name_attach HFSM1 fehlgeschlagen" << std::endl;
        }
    }

    if (festo2) {
        attachHFSM = name_attach(nullptr, ATTACH_POINT_HFSM2, 0);
        if (!attachHFSM) {
            std::cerr << "[HFSM] name_attach HFSM2 fehlgeschlagen" << std::endl;
        }
    }


    // DEIN TIMER-OBJEKT NUTZEN:
    // Wir erstellen einen Timer, der an unseren Channel (chid)
    // den Code PULSE_TICK sendet.
    Timer tickTimer(attachHFSM->chid, T_PULSE_TICK, 0);

    // Starte den Timer periodisch alle 20ms
    tickTimer.starten(20, true);

    struct _pulse pulse;
    uint64_t last_time = get_now_ms(); // Pseudocode für aktuelle Zeit


    while (true) {
        int rcvid = MsgReceive(attachHFSM->chid, &pulse, sizeof(pulse), NULL);

        if (rcvid == -1) {
            std::cerr << "[Context] Fehler in MsgReceive!" << std::endl;
            break;
        }


        if (rcvid == 0) {
            //}
            if (pulse.code == T_PULSE_TICK) {
                // DER KERNEL HAT DIESEN PULS GESENDET!
                uint64_t now = get_now_ms();
                double delta_ms = (double) (now - last_time);
                last_time = now;

                // Jetzt alle RegionTimer in den Daten "füttern"
                data.updateTimers(delta_ms, data.getMotorSchnell());
            } else if (pulse.code == _PULSE_CODE_DISCONNECT) {
                ConnectDetach(pulse.scoid);
                continue;
            } else {
                Signal signal = (Signal) pulse.code;
                int value = pulse.value.sival_int;


                //std::cout << "[Context] Signal erhalten: " << (int)signal << std::endl;


                handleEvent(signal, value);
            }
        }
    }

    name_detach(attachHFSM, 0);
}

void Context::initFSM() {
    //sleep(5);
    state->enterViaPseudoStart();
}