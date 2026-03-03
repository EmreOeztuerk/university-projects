/**
 * @file dispatcher.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Dispatcher-Klasse.
 *
 * Enthält die Logik für das Nachrichten-Dispatching, Timer-Handling und
 * die Kommunikation zwischen HAL, FSM und anderen Anlagen (via Qnet).
 */

#include "dispatcher.hpp"
#include <unistd.h>
#include <chrono>

typedef struct _pulse header_t;

Dispatcher::Dispatcher(int id, std::string nachbarHost) : verbindung(nullptr), heartbeatTimer(nullptr) {
    attachDIS = nullptr; //Channel des Dispatchers
    meinID = -1; // Dispatcher ID
    halID = -1; // Channel ID der HAL
    rootFSM = -1; // Channel ID Logik
    idd = id;
    verbindung = new Verbindung(this, nachbarHost);
}

Dispatcher::~Dispatcher() {
    if (meinID != -1) {
        ConnectDetach(meinID);
    }
    if (attachDIS) {
        name_detach(attachDIS, 0);
    }


    if (heartbeatTimer != nullptr) {
        delete heartbeatTimer;
    }
    if (verbindung) {
        delete verbindung;
    }
}

void Dispatcher::run() {
    // Set Name des Dispatcher
    if (idd == 1) {
        attachDIS = name_attach(NULL, ATTACH_POINT1, NAME_FLAG_ATTACH_GLOBAL);
    } else {
        attachDIS = name_attach(NULL, ATTACH_POINT2, NAME_FLAG_ATTACH_GLOBAL);
    }


    if (attachDIS == NULL) {
        std::cerr << "[Dispatcher] Fehler: Name wurde nicht attached." << std::endl;
        return;
    }

    // ---------Channel erzeugen--------
    meinID = ConnectAttach(0, 0, attachDIS->chid, _NTO_SIDE_CHANNEL, 0);

    if (meinID == -1) {
        std::cerr << "[Dispatcher] Fehler mit meinID." << std::endl;
    }

    std::cout << "[Dispatcher] Warte auf Nachrichten..." << std::endl;

    //----------- Alle Channel-IDs erhalten-------

    halID = name_open(idd == 1 ? "HAL_1" : "HAL_2", 0);

    if (halID == -1) {
        std::cout << "[Dispatcher] Mit HAL nicht verbunden..." << std::endl;
    } else {
        std::cout << "[Dispatcher] Mit HAL  verbunden..." << std::endl;
    }

    rootFSM = name_open(idd == 1 ? "HFSM_1" : "HFSM_2", 0);

    if (rootFSM == -1) {
        std::cout << "[Dispatcher] Mit HFSM nicht verbunden..." << std::endl;
    } else {
        std::cout << "[Dispatcher] Mit HFSM  verbunden..." << std::endl;
    }

    // Timer
    heartbeatTimer = new Timer(attachDIS->chid, T_PULSE_TIMER);
    heartbeatTimer->starten(AUSZEIT, true);


    struct _pulse pulse;

    while (true) {
        int rcvid = MsgReceive(attachDIS->chid, &pulse, sizeof(pulse), NULL);
        if (rcvid == -1) {
            continue; // Keine Nachricht bekommen
        }

        // --GNS---
        if (rcvid > 0) {
            header_t *hdr = (header_t *) &pulse;
            if (hdr->type >= _IO_BASE && hdr->type <= _IO_MAX) {
                if (hdr->type == _IO_CONNECT) {
                    MsgReply(rcvid, EOK, NULL, 0);
                } else {
                    MsgError(rcvid, ENOSYS);
                }
            } else {
                MsgReply(rcvid, EOK, NULL, 0);
            }
        }


        if (rcvid == 0) {
            if (pulse.code == T_PULSE_TIMER) {
                verbindung->sendKeepAlive();
                verbindung->checkTick();
                continue;
            }

            if (pulse.code == Signal::T_PING_PONG) {
                verbindung->resetHerzschlag();
                continue;
            }

            if (pulse.code == Signal::S_VBDN) {
                this->restartHeartbeatTimer();
                verbindung->resetHerzschlag();
                continue;
            }

            if (pulse.code == Signal::S_E1_G) {
                std::cout << "\n[Dispatcher] E1_STOP GEKOMMEN" << std::endl;
            }
            // Nachricht Inthalt
            int code = pulse.code;
            int wert = pulse.value.sival_int;
            //std::cout << "[DISPATCHER] Pulse erhalten!  -> Code: " << code
            //                     << " | Wert: " << wert << std::endl;

            signalHandlung(code, wert);
        }
    }
}


int Dispatcher::bestimmtPrioritaet(int code) {
    switch (code) {
        case Signal::EC_VBDNG_UB:
        case Signal::S_VBDN:
        case Signal::CMD_MTR_RECHTS_L:
        case Signal::S_LSH_UB:
        case Signal::S_LSH_NUB:
            return HOCH_PRIORITAET; // Kristcher Fehler
            break;
        case Signal::S_E1_G:
        case Signal::S_E2_G:

            return EXTREM_HOCH; // Kristcher Fehler
            break;
        case Signal::S_METAL_AN:
            return SEHR_HOCH_PRIORITAET;
            break;
        case Signal::S_UEBERGABE_WS:
        case Signal::WERKSTUECK_ANDERS:
        case Signal::WERKSTUECK_FLACH:
        case Signal::WERKSTUECK_HOCH:
        case Signal::WERKSTUECK_MIT_BOHRUNG:
        case Signal::WERKSTUECK_UNBEKANNT:
        case Signal::WERKSTUECK_MIT_BOHRUNG_M:
            return HOCH_PRIORITAET;
        case Signal::T_PING_PONG:
        case Signal::S_PARTNER_FEHLER_1:
        case Signal::S_PARTNER_OK_1:
        case Signal::S_PARTNER_FEHLER_2:
        case Signal::S_PARTNER_OK_2:
        case Signal::CMD_MTR_STOP:
        //case Signal::CMD_MTR_RECHTS_L:
        case Signal::CMD_MTR_RECHTS_S:
            return MITTELHOCH_PRIORITAET; // Verbindung
            break;
        case Signal::S_ST_G:
        case Signal::S_ST1_G:
        case Signal::S_ST2_G:
        case Signal::S_ST1_LG:
        case Signal::S_ST2_LG:
        case Signal::S_STP_G:
        case Signal::S_RT1_G:
        case Signal::S_RT2_G:
        case Signal::S_LSE1_UB:
        case Signal::S_LSE1_NUB:
        case Signal::S_E1_NG:
        case Signal::S_E2_NG:

            return MITTEL_PRIORITAET;
            break;
        default: return NORMAL_PRIORITAET; // normale Signalen
    }
}

void Dispatcher::weiterleiten(int coid, int code, int value) {
    if (coid == -1) {
        const char *targetName = nullptr;

        if (&coid == &halID) {
            targetName = (idd == 1) ? "HAL_1" : "HAL_2";
        } else if (&coid == &rootFSM) {
            targetName = (idd == 1) ? "HFSM_1" : "HFSM_2";
        }

        if (targetName != nullptr) {
            coid = name_open(targetName, 0);
        }
    }

    if (coid != -1) {
        int priority = bestimmtPrioritaet(static_cast<Signal>(code));

        MsgSendPulse(coid, priority, code, value);
    }
}

void Dispatcher::signalHandlung(int code, int wert) {
    static int letztesMalGesendet = -1;

    switch (code) {
        // =================================================
        // INPUT
        // HAL -> FSMs
        // =================================================
        case Signal::S_LSE2_UB:
        case Signal::S_LSE2_NUB:
        case Signal::S_LSH_UB:
        case Signal::S_LSH_NUB:
        case Signal::S_LSW_UB:
        case Signal::S_LSW_NUB:
        case Signal::S_LSR1_UB:
        case Signal::S_LSR1_NUB:
        case Signal::S_LSR2_UB:
        case Signal::S_LSR2_NUB:
        //case Signal::S_LSA1_UB:
        case Signal::S_LSA1_NUB:
        case Signal::S_LSA2_UB:
        case Signal::S_LSA2_NUB:
        case Signal::S_METAL_AN:
        case Signal::S_METAL_AUS:
        case Signal::T_LSA_LSH:
        case Signal::T_LSH_LSM:
        case Signal::T_LSW_LSE:
        case Signal::T_LSW_LSR:
        //case Signal::T_LSE1_LSA2:
        case Signal::T_Q1_LSH:
        case Signal::T_LSA_Q1:


        case Signal::T_TOLERANZZEIT_LSH:
        case Signal::T_TOLERANZZEIT_LSW:
        case Signal::T_TOLERANZZEIT_LSE:
        case Signal::S_HS_WERT:
        case Signal::WERKSTUECK_FLACH:
        case Signal::WERKSTUECK_HOCH:
        case Signal::WERKSTUECK_MIT_BOHRUNG:
        case Signal::WERKSTUECK_UNBEKANNT:
        case Signal::WERKSTUECK_ERKANNT:
        case Signal::CMD_RTSCH1_NV:

            // Sendet an alle FSMs
            if (rootFSM != -1) {
                weiterleiten(rootFSM, code, wert); // Logik
            } else {
                std::cerr << " [Dispatcher] Fehler bei Senden: FSM ist ungueltig!" << std::endl;
            }
            break;

        // =================================================
        // OUTPUT
        // FSM -> HAL
        // =================================================

        case Signal::CMD_MTR_STOP:
        case Signal::CMD_MTR_RECHTS_L:
        case Signal::CMD_MTR_RECHTS_S:
        case Signal::CMD_WCH_AN:
        case Signal::CMD_WCH_AUS:
        case Signal::CMD_ASWERF_AN:
        case Signal::CMD_ASWERF_AUS:
        case Signal::CMD_LED_AMP_ROT_AN:
        case Signal::CMD_LED_AMP_ROT_AUS:
        case Signal::CMD_LED_AMP_ROT_BS:
        case Signal::CMD_LED_AMP_ROT_BL:
        case Signal::CMD_LED_AMP_GLB_AN:
        case Signal::CMD_LED_AMP_GLB_AUS:
        case Signal::CMD_LED_AMP_GLB_B:
        case Signal::CMD_LED_AMP_GRN_AN:
        case Signal::CMD_LED_AMP_GRN_AUS:
        case Signal::CMD_LED_ST_AN:
        case Signal::CMD_LED_ST_AUS:
        case Signal::CMD_LED_RT_AN:
        case Signal::CMD_LED_RT_AUS:
        case Signal::CMD_LED_Q1_AN:
        case Signal::CMD_LED_Q1_AUS:
        case Signal::CMD_LED_Q2_AN:
        case Signal::CMD_LED_Q2_AUS:
        case Signal::CMD_DURCHLASSEN:
        case Signal::CMD_AUSSORTIEREN:


            if (halID != -1) {
                weiterleiten(halID, code, wert); // Sendet an HAL
            } else {
                std::cerr << " [Dispatcher] Fehler bei Senden: HAL ungueltig!" << std::endl;
            }
            break;

        // =================================================
        // ANLAGE -> ANLAGE
        // =================================================
        case Signal::CMD_SP_ZST_1:
        case Signal::CMD_SP_ZST_2:
        case Signal::CMD_WH_ZST_1:
        case Signal::CMD_WH_ZST_2:
        case Signal::CMD_WS_BEKANNT:
        case Signal::CMD_WS_UNBEKANNT:
        //case Signal::CMD_RTSCH1_NV:
        //case Signal::CMD_RTSCH2_NV:
        //case Signal::S_UEBERGABE_WS:
        case Signal::S_UEBERGABE_WS_FERTIG:
        //case Signal::CMD_FESTOS_FREI:
        //case Signal::S_FESTO2_BEREIT:
        //case Signal::S_FESTO2_NBEREIT:
        case Signal::EC_VBDNG_UB:


            if (verbindung->istVerbunden()) {
                verbindung->sendenAnNachbarn(code, wert); // Sendet an Festo 2 (Nachbarn)
            } else {
                std::cerr << " [Dispatcher: F1->F2] Fehler bei Senden!" << std::endl;
            }

            break;

        case Signal::S_ST_G:
        case Signal::S_ST1_G:
        case Signal::S_ST2_G:
        case Signal::S_ST1_LG:
        case Signal::S_ST2_LG:
        case Signal::S_STP_G:
        case Signal::S_RT1_G:
        case Signal::S_RT2_G:
        case Signal::S_LSE1_UB:
        case Signal::S_LSE1_NUB:
        case Signal::S_E1_G:
        case Signal::S_E1_NG:
        case Signal::S_E2_G:
        case Signal::S_E2_NG:
        case Signal::S_LSA1_UB:
        case Signal::S_PARTNER_FEHLER_1:
        case Signal::S_PARTNER_OK_1:
        case Signal::S_PARTNER_FEHLER_2:
        case Signal::S_PARTNER_OK_2:
        case Signal::T_LSE1_LSA2:
        case Signal::T_TOLERANZZEIT_LSA:
        case Signal::S_FESTO2_BEREIT:
        case Signal::S_FESTO2_NBEREIT:
        case Signal::EC_RTSCH2_VOLL:
        case Signal::CMD_RTSCH2_NV:
        case Signal::S_FESTO1_FREI:
        case Signal::S_FESTO1_NFREI:
        case Signal::S_FESTO2_FREI:
        case Signal::S_FESTO2_NFREI:
        case Signal::CMD_FESTOS_FREI:
        case Signal::S_FESTO1_STOPPEN:
        case Signal::S_WS_AN_FESTO2:

            // Sendet an alle FSMs
            if (rootFSM != -1) {
                weiterleiten(rootFSM, code, wert); // Logik
            } else {
                std::cerr << " [Dispatcher] Fehler bei Senden: FSM ist ungueltig!" << std::endl;
            }

            if (istMeinSignal(code)) {
                if (verbindung->istVerbunden()) {
                    //if(code != letztesMalGesendet){
                    verbindung->sendenAnNachbarn(code, wert);
                    letztesMalGesendet = code;
                    //}
                }
            }


            break;
        case Signal::S_UEBERGABE_WS:
            if (idd == 2) {
                if (rootFSM != -1) {
                    weiterleiten(rootFSM, code, wert); // Logik
                } else {
                    std::cerr << " [Dispatcher] Fehler bei Senden: FSM ist ungueltig!" << std::endl;
                }
            }
            if (istMeinSignal(code)) {
                if (verbindung->istVerbunden()) {
                    verbindung->sendenAnNachbarn(code, wert);
                    letztesMalGesendet = code;
                }
            }
            break;
        case Signal::WERKSTUECK_ANDERS:
            if (idd == 1) {
                if (rootFSM != -1) {
                    weiterleiten(rootFSM, code, wert); // Logik
                } else {
                    std::cerr << " [Dispatcher] Fehler bei Senden: FSM ist ungueltig!" << std::endl;
                }
            }

            if (istMeinSignal(code)) {
                if (verbindung->istVerbunden()) {
                    verbindung->sendenAnNachbarn(code, wert);
                    letztesMalGesendet = code;
                }
                break;

            case Signal::EC_WS_UNB:
            case Signal::EC_WS_VERSCHW:
            case Signal::EC_WS_VRLN:
            case Signal::EC_RTSCH1_VOLL:
                //case Signal::EC_RTSCH2_VOLL:
            case Signal::EC_ABSTAND_NE:
            case Signal::EC_RTSCH_VOLL:

                if (rootFSM != -1) {
                    weiterleiten(rootFSM, code, wert); // Logik
                } else {
                    std::cerr << " [Dispatcher] Fehler bei Senden: Fehlerbehandlung ungueltig!" << std::endl;
                }
                break;

            default:
                std::cerr << "[Dispatcher] Unbekannte Signal: " << code << std::endl;
                break;
            }
    }
}

int Dispatcher::getKanal() {
    if (attachDIS) {
        return attachDIS->chid;
    }
    return FEHLER;
}

void Dispatcher::sendenNachricht(Signal signal, int wert) {
    if (meinID != -1) {
        int prioritaet = bestimmtPrioritaet(signal);
        int res = MsgSendPulse(meinID, prioritaet, signal, wert);
        if (res == -1) {
            std::cout << "Dispatcher: Nachricht wurde nicht gesendet" << std::endl;
        }
    }
}

bool Dispatcher::nachrichtEmpfaengen(int chid, Signal &signal, int &wert) {
    struct _pulse pulse;

    int rcvid = MsgReceive(chid, &pulse, sizeof(pulse), NULL);
    //std::cout << "CODE: " << pulse.code << std::endl;
    // Fehler im System
    if (rcvid == -1) {
        return false;
    }

    // Es ist nicht ein Pulse

    if (rcvid > 0) {
        if (rcvid > 0) {
            header_t *hdr = (header_t *) &pulse;
            if (hdr->type == _IO_CONNECT) {
                MsgReply(rcvid, EOK, NULL, 0);
            } else {
                MsgError(rcvid, ENOSYS);
            }
            return false;
        }
    }

    // Es ist ein Pulse
    if (rcvid == 0) {
        if (pulse.code == _PULSE_CODE_DISCONNECT) {
            ConnectDetach(pulse.scoid);
            return false;
        }


        signal = static_cast<Signal>(pulse.code);
        wert = pulse.value.sival_int;

        return true;
    }

    return false;
}

void Dispatcher::restartHeartbeatTimer() {
    if (heartbeatTimer != nullptr) {
        heartbeatTimer->stoppen();

        heartbeatTimer->starten(AUSZEIT, true);
        std::cout << "[Dispatcher] Timer noch mal gestartet" << std::endl;
    }
}

bool Dispatcher::istMeinSignal(int code) {
    if (idd == 1) {
        //Anlage 1

        return (code == Signal::S_ST1_G || code == Signal::S_RT1_G || code == Signal::S_E1_G ||
                code == Signal::S_E1_NG || code == Signal::S_LSA1_UB || code == Signal::S_LSE1_UB
                || code == Signal::S_PARTNER_OK_1 || code == Signal::S_PARTNER_FEHLER_1 || code == Signal::T_LSE1_LSA2
                || code == Signal::S_FESTO1_FREI || code == Signal::S_FESTO1_NFREI
                || code == Signal::T_TOLERANZZEIT_LSA || code == Signal::S_UEBERGABE_WS || code ==
                Signal::S_WS_AN_FESTO2);
    } else {
        // Anlage 2
        return (code == Signal::S_ST2_G || code == Signal::S_RT2_G || code == Signal::S_E2_G ||
                code == Signal::S_E2_NG || code == Signal::S_LSE2_UB
                || code == Signal::S_PARTNER_OK_2 || code == Signal::S_PARTNER_FEHLER_2
                || code == Signal::S_FESTO2_BEREIT || code == Signal::S_LSR2_NUB || code == Signal::EC_RTSCH2_VOLL
                || code == Signal::S_FESTO2_NBEREIT || code == Signal::CMD_FESTOS_FREI || code == Signal::CMD_RTSCH2_NV
                || code == Signal::WERKSTUECK_ANDERS || code == Signal::S_FESTO2_FREI || code == Signal::S_FESTO2_NFREI
                || code == Signal::S_FESTO1_STOPPEN);
    }
}