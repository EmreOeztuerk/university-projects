/**
 * @file Verbindung.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Verbindung-Klasse.
 *
 * Enthält die Logik für den Verbindungsaufbau via Qnet (`name_open`),
 * das Senden von Keep-Alive-Signalen (Ping-Pong) und die Erkennung von Verbindungsabbrüchen.
 */

#include "Verbindung.hpp"
#include "dispatcher.hpp"

Verbindung::Verbindung(Dispatcher *disp, std::string nachbarHost)
    : dis1(disp),
      nachbarnID(-1),
      meinID(disp->idd),
      aktuellerZustand(Zustand::WARTE_AUF_VERBINDUNG),
      herzschlagZaehler(0),
      nachbarHostname(nachbarHost) {
}

Verbindung::~Verbindung() {
    if (nachbarnID != -1) {
        ConnectDetach(nachbarnID);
    }
}

void Verbindung::verbinden() {
    int targetID = (meinID == 1) ? 2 : 1;
    std::string targetName;

    if (nachbarHostname.empty()) {
        targetName = "Dispatcher_" + std::to_string(targetID);
    } else {
        targetName = "/net/" + nachbarHostname + "/dev/name/global/Dispatcher_" + std::to_string(targetID);
    }


    int newCoid = name_open(targetName.c_str(), NAME_FLAG_ATTACH_GLOBAL);

    if (newCoid != -1) {
        this->nachbarnID = newCoid;
        this->aktuellerZustand = Zustand::VERBUNDEN;
        this->herzschlagZaehler = 0;
        //dis1->sendenNachricht(Signal::S_VBDN, 0);
        dis1->restartHeartbeatTimer();
        sendenAnNachbarn(Signal::S_VBDN, 0); // schickt Signal an Dispatcher von der anderer Anlage
        sendKeepAlive();
        std::cout << "[Verbindung] VERBUNDEN!" << std::endl;
    } else {
        std::cerr << "[Verbindung] newCoid  Code: " << errno
                << " -> " << strerror(errno) << std::endl;
        dis1->sendenNachricht(Signal::EC_VBDNG_UB, 0);
        dis1->sendenNachricht(Signal::CMD_MTR_STOP, 0);
        dis1->sendenNachricht(Signal::CMD_LED_AMP_ROT_AN, 0);
        dis1->sendenNachricht(Signal::CMD_LED_Q2_AUS, 0);
        roteLamopeAus = false;
    }
}

void Verbindung::setNachbarnID(int nachbarnID) {
    this->nachbarnID = nachbarnID;
    resetHerzschlag();
    std::cout << "[Verbindung] NachbarnID wurde gesetzt:" << nachbarnID << std::endl;
}

void Verbindung::sendenAnNachbarn(int code, int wert) {
    if (aktuellerZustand != Zustand::WARTE_AUF_VERBINDUNG && nachbarnID != -1) {
        int res = MsgSendPulse(nachbarnID, SIGEV_PULSE_PRIO_INHERIT, code, wert);

        if (res == -1) {
            std::cerr << "[Verbindung] Die Nachricht wurde nicht gesendet!" << std::endl;
            ConnectDetach(nachbarnID);
            nachbarnID = -1;
            aktuellerZustand = Zustand::WARTE_AUF_VERBINDUNG;
        } else {
            //std::cerr << "[Verbindung] Die Nachricht wurde gesendet!" << std::endl;
        }
    } else {
        //std::cout << "[Verbindung] ZUSTAND:  " << (int)aktuellerZustand << std::endl;
        //std::cout << "[Verbindung] nachbarID:  " << nachbarnID << std::endl;
    }
}

void Verbindung::resetHerzschlag() {
    if (nachbarnID == -1) verbinden(); // Versucht zu Verbinden

    if (aktuellerZustand != Zustand::VERBUNDEN) {
        //std::cout << "[Verbindung] Anlagen sind verbunden!" << std::endl;
        if (!roteLamopeAus) {
            std::cout << "[Verbindung] nachbarID:" << nachbarnID << std::endl;
            std::cout << "[Verbindung] meinID:" << meinID << std::endl;
            dis1->sendenNachricht(Signal::CMD_LED_AMP_ROT_AUS, 0);
            dis1->sendenNachricht(Signal::CMD_LED_Q2_AN, 0);
            roteLamopeAus = true;
        }
    }

    aktuellerZustand = Zustand::VERBUNDEN;
    herzschlagZaehler = 0;
}

bool Verbindung::checkTick() {
    if (aktuellerZustand == Zustand::WARTE_AUF_VERBINDUNG) {
        static int retryCounter = 0;
        retryCounter++;

        if (retryCounter >= 20) {
            std::cout << "[Verbindung] SUCHEN..." << std::endl;
            verbinden();
            retryCounter = 0;
        }

        return false;
    }

    herzschlagZaehler++;

    if (herzschlagZaehler == 1) {
        aktuellerZustand = Zustand::HEARTBEAT1_VERPASST;
    } else if (herzschlagZaehler == 2) {
        aktuellerZustand = Zustand::HEARTBEAT2_VERPASST;
    } else if (herzschlagZaehler >= 3) {
        aktuellerZustand = Zustand::WARTE_AUF_VERBINDUNG;
        nachbarnID = -1;

        std::cerr << "[Verbindung] TIMEOUT: NICHT VERBUNDEN!" << std::endl;

        dis1->sendenNachricht(Signal::EC_VBDNG_UB, 0);
        dis1->sendenNachricht(Signal::CMD_MTR_STOP, 0);
        dis1->sendenNachricht(Signal::CMD_LED_AMP_ROT_AN, 0);

        return true;
    }

    sendKeepAlive();

    return false;
}


bool Verbindung::istVerbunden() const {
    return aktuellerZustand != Zustand::WARTE_AUF_VERBINDUNG;
}

int Verbindung::getHerzschlagZaehler() const {
    return herzschlagZaehler;
}

void Verbindung::sendKeepAlive() {
    sendenAnNachbarn(Signal::T_PING_PONG, 0);
}