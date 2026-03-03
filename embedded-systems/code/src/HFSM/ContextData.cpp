/**
* @file ContextData.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der ContextData-Klasse.
 */

#include <iostream>
#include <string>
#include <chrono>
#include "ContextData.h"
#include <fstream>


ContextData::ContextData(bool f1, bool f2, Actions *shared_action) : festo1(f1),
                                                                     festo2(f2),
                                                                     action(shared_action),
                                                                     // HIER: Das echte shared_action übergeben, KEIN reinterpret_cast mehr!
                                                                     lsa_timer(shared_action),
                                                                     lsh_timer(shared_action),
                                                                     lsw_timer(shared_action),
                                                                     lse_timer(shared_action),
                                                                     lsr_timer(shared_action) {
    sollReihenfolge.clear();
    // Richtige Reihenfolge in vector
    sollReihenfolge.push_back(WerkstueckTyp::HB);
    sollReihenfolge.push_back(WerkstueckTyp::H);
    sollReihenfolge.push_back(WerkstueckTyp::F);
    sollReihenfolge.push_back(WerkstueckTyp::HBM);
    anzahlWerkstuecke = -1;
    order_index = 0;

    if (festo1) {
        //Festo02
        t_lsa_lsh_l = 5562;
        t_lsa_q1_l = 4304;
        t_lsh_lsw_l = 2636;
        t_lsw_lse_l = 5259;
        t_lse1_lsa2_l = 1400; //1350;//1250


        t_lse1_lsa2_s = 660; //635;//435
        t_lsa_lsh_s = 2323;
        t_lsa_q1_s = 516;
        t_lsh_lsw_s = 1165;
        t_lsw_lse_s = 2258;
    } else if (festo2) {
        //Festo10
        t_lse1_lsa2_l = 1250;
        t_lsa_lsh_l = 7742;
        t_lsh_lsw_l = 4221; //4121
        t_lsw_lse_l = 7497;

        t_lse1_lsa2_s = 435;
        t_lsa_lsh_s = 2245;
        t_lsh_lsw_s = 1332; //1232
        t_lsw_lse_s = 2146;
    }
}

ContextData::~ContextData() {
}


// Messung starten
void ContextData::zeitStarten() {
    startZeit = std::chrono::steady_clock::now();
}

// Differenz berechnen und über ContextData speichern
void ContextData::zeitSpeichern(const std::string &name) {
    // 1. Zeitdauer berechnen
    auto now = std::chrono::steady_clock::now();
    double dauer = std::chrono::duration_cast<std::chrono::milliseconds>(now - startZeit).count();

    // 2. Interne Map nutzen
    speichereZeit(name, dauer);


    // 4. ID des aktuellen Werkstücks holen (NEU)
    std::string aktuelleID = "";
    if (!werkstueckListe.empty()) {
        Werkstueck ws = werkstueckListe.front();
        aktuelleID = ws.getID();
    }

    // 5. In Datei schreiben (Immer anhängen!)
    if (name == "Reisezeit_LSW_LSR") {
        // Absoluter Pfad für BeagleBone/Linux
        std::string dateiname = "/tmp/Reisezeit_LSW_LSR.txt";

        // Einfach öffnen und anhängen (std::ios::app)
        std::ofstream datei(dateiname, std::ios::app);

        if (datei.is_open()) {
            datei << "ID: " << aktuelleID << " | Zeit: " << dauer << " ms" << std::endl;
            datei.close();
            std::cout << "[ContextData] DATEI UPDATE: ID " << aktuelleID << "" << dauer << " ms (angehaengt)." <<
                    std::endl;
        } else {
            std::cerr << "[ContextData] FEHLER: Konnte " << dateiname << " nicht öffnen!" << std::endl;
        }
    }
}

double ContextData::holeZeit(const std::string &name) const {
    auto it = zeiten.find(name);
    if (it != zeiten.end()) {
        return it->second;
    }
    return -1.0; // Fehlerwert
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------


void ContextData::Fehlermeldung(const std::string &typ) {
    // ANSI-Farbcodes für farbige Konsolenausgabe
    //const std::string rot   = "\033[31m";  // rote Schrift (Warnung/Fehler)
    //const std::string fett  = "\033[1m";   // fett (bold)
    //const std::string reset = "\033[0m";   // Formatierung zurücksetzen

    std::string anlage;

    // Anlage abhängig vom Parameter bestimmen
    if (typ == "E1") {
        anlage = "erster Anlage";
    } else if (typ == "E2") {
        anlage = "zweiter Anlage";
    } else if (typ == "E") {
        // Standardfall: E, unklarer Wert => beide Anlagen betroffen
        anlage = "beiden Anlagen";
    }

    // Fehlermeldung farbig ausgeben
    std::cout
            << "⚠️  EStop wurde in der " << anlage << " gedrückt!"
            << std::endl;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void ContextData::set(bool &variable, bool wert) {
    variable = wert;
}


bool ContextData::datenprozessLoeschen() {
    // Hier implementieren, z.B. alle Daten löschen/resetten
    std::cout << "ContextData: Datenprozess gelöscht" << std::endl;

    f2_voll = false;
    f2_bereit = true;
    anzahlWSF1 = 0;
    anzahlWSF2 = 0;
    festo1Frei = false;
    order_index = 0;
    //naechsteTyp = WerkstueckTyp::HB;
    motorSchnell = true;
    enteredHoheMessen = false;
    enteredWartenAufErlaubnisVonF2 = false;
    enteredWartenBisWSEntfernt = false;
    FehlerVerlorenBeiLAS2 = false;
    anzahlWerkstuecke = -1;
    erwarteteTypenQueue.clear();
    werkstueckListe.clear();
    letztesMal = false;
    clearFehler();
    reset_abgeschlossen = true;
    return true;
}

bool ContextData::istReset_abgeschlossen() {
    return reset_abgeschlossen;
}

void ContextData::setSortTyp(SortTyp given_sortTyp) {
    sortTyp = given_sortTyp;
}

void ContextData::addWS_F1() {
    anzahlWSF1 += 1;
    action->s_festo1_nfrei();
    std::cout << "[ContextData] addWS_F1 anzahl Werkstuecke an Anlage 1: " << anzahlWSF1 << std::endl;
}

void ContextData::addWS_F2() {
    anzahlWSF2 += 1;
    action->s_festo2_nfrei();
    if (anzahlWSF2 >= 2) {
        f2_voll = true;
        action->s_festo2_nbereit();
    }
    std::cout << "[ContextData] addWS_F2 anzahl Werkstuecke an Anlage 2: " << anzahlWSF2 << std::endl;
}

void ContextData::loescheWS_F1() {
    anzahlWSF1 -= 1;

    if (anzahlWSF1 == 0) {
        action->s_festo1_frei();
        if (festo2Frei && enteredAussortieren) {
            action->cmd_festos_frei();
        }
    }
    if (anzahlWSF1 < 0) anzahlWSF1 = 0;

    //std::cout << "[ContextData] Werkstück  Festo1 verlassen\n";
    std::cout << "[ContextData] loescheWS_F1 anzahl Werkstuecke an Anlage 1: " << anzahlWSF1 << std::endl;
}

void ContextData::loescheWS_F2() {
    anzahlWSF2 -= 1;
    if (anzahlWSF2 < 2) {
        action->s_festo2_bereit();
        f2_voll = false;
    }


    if ((anzahlWSF2 == 0)) {
        action->s_festo2_frei();

        if (festo1Frei) {
            action->cmd_festos_frei();
        }
    }
    if (anzahlWSF2 < 0) anzahlWSF2 = 0;
    //std::cout << "[ContextData] Werkstück  Festo2 verlassen\n";
    std::cout << "[ContextData] loescheWS_F2 anzahl Werkstuecke an Anlage 2: " << anzahlWSF2 << std::endl;
    std::cout << "[ContextData] festo1Frei: " << festo1Frei << std::endl;
}


// -----------------------
// Fehler-API
// -----------------------
const ContextData::FehlerInfo &ContextData::getLetzterFehler() const {
    return letzterFehler;
}

void ContextData::clearFehler() {
    letzterFehler = {};
}

AnlageId ContextData::aktuelleAnlage() const {
    if (festo1 && festo2) return AnlageId::BEIDE;
    if (festo1) return AnlageId::FESTO1;
    if (festo2) return AnlageId::FESTO2;
    return AnlageId::UNBEKANNT;
}

void ContextData::setFehler(Fehlerart art, FehlerOrt ort) {
    setFehler(art, ort, aktuelleAnlage());
}

void ContextData::addFehler(Fehlerart art, FehlerOrt ort) {
    addFehler(art, ort, aktuelleAnlage());
}

void ContextData::setFehler(Fehlerart art, FehlerOrt ort, AnlageId anlage) {
    letzterFehler.art = art;
    letzterFehler.ort = ort;
    letzterFehler.anlage = anlage;
}

void ContextData::addFehler(Fehlerart art, FehlerOrt ort, AnlageId anlage) {
    ++anzahlFehler;
    // Letzten Fehler merken (für Anzeige/Debug)
    letzterFehler.art = art;
    letzterFehler.ort = ort;
    letzterFehler.anlage = anlage;
}

bool ContextData::keineFehler() const {
    return anzahlFehler == 0;
}

void ContextData::fehlerGeloest() {
    if (anzahlFehler > 0) {
        --anzahlFehler;
    }
    if (anzahlFehler == 0) {
        // wenn alles weg: letzter Fehler „leer“
        letzterFehler = {};
    }
}

void ContextData::resetFehlerZaehler() {
    anzahlFehler = 0;
    letzterFehler = {};
}

// -----------------------
// Guards (Implementierung)
// -----------------------
bool ContextData::gFesto1() const { return festo1; }
bool ContextData::gFesto2() const { return festo2; }
bool ContextData::gF2Bereit() const { return f2_bereit; }
bool ContextData::gF2Voll() const { return f2_voll; }
bool ContextData::getMotorSchnell() const { return motorSchnell; }

bool ContextData::gTypGleichNaechster() const {
    if (werkstueckListe.empty()) {
        std::cerr << "[Guard] WSListe ist leer!" << std::endl;
        return false;
    }

    WerkstueckTyp typ = werkstueckListe.front().getTyp();

    WerkstueckTyp erwarteterTyp = WerkstueckTyp::UNBEKANNT;

    if (festo1) {
        if (order_index < sollReihenfolge.size()) {
            erwarteterTyp = sollReihenfolge[order_index];
        } else {
            erwarteterTyp = sollReihenfolge[0];
        }
    } else if (festo2) {
        if (!erwarteteTypenQueue.empty()) {
            erwarteterTyp = erwarteteTypenQueue.front();
        } else {
            std::cout << "[Guard] Erwartete WS Liste ist leer!" << std::endl;
            return false;
        }
    }
    bool match = (typ == erwarteterTyp);

    std::cout << "[Guard] Check: Typ (" << (int) typ
            << ") == erwarteterTyp (" << (int) erwarteterTyp << ")? -> " << match << std::endl;

    return match;
}


bool ContextData::gTypUngleichNaechster() const {
    return !gTypGleichNaechster();
}

bool ContextData::gGleicheTyp() const { return gTypGleichNaechster(); }
bool ContextData::gNichtGleicheTyp() const { return !gGleicheTyp(); }

bool ContextData::gRutscheVoll() const { return rutsche_voll; }
bool ContextData::gRutsche2Voll() const { return rutsche2_voll; }

bool ContextData::gLSWUBFehlerRutsche2Voll() const {
    if (werkstueckListe.empty()) return false;
    Werkstueck ws = werkstueckListe.front();
    bool zuAussortieren = ws.istAusschuss();
    if (!zuAussortieren) {
        return false;
    }
    return gRutsche2Voll();
}

bool ContextData::gLSWUBAussortieren() const {
    if (festo2) {
        Werkstueck ws = werkstueckListe.front();
        bool zuAussortieren = ws.istAusschuss();
        if (zuAussortieren && !gRutscheVoll()) {
            return true;
        }
        return false;
    }
    bool match = gTypGleichNaechster();
    if (match) return false;
    // festo1 oder festo 2, rutsche nicht voll
    if (!match && !gRutscheVoll()) {
        std::cout << "[Guard] WS -> Aussortieren" << std::endl;
        return true;
    }
    return false;
}

bool ContextData::gLSWUBDurchlauf() const {
    if (werkstueckListe.empty()) return false;
    Werkstueck ws = werkstueckListe.front();

    bool zuAussortieren = ws.istAusschuss(); // entscheidung von ueberpruefeReihenfolge()

    // RICHTIGE WS
    if (!zuAussortieren) {
        const_cast<ContextData *>(this)->wollteAussortieren = false;
        return true;
    }

    // FALSCHE WS ABER RUTSCHE VOLL BEI ANLAGE 1
    if ((festo1 == true) && (zuAussortieren == true) && (gRutscheVoll() == true)) {
        std::cout << "[Guard] WS falsch aber Rutsche ist voll -> DURCHLAUF" << std::endl;

        const_cast<ContextData *>(this)->wollteAussortieren = true;
        return true;
    }
    return false;
}

void ContextData::registerId1() {
    anzahlWerkstuecke++;
    if (anzahlWerkstuecke > 99) {
        // MaximaleAnzahl von Werkstuecke
        anzahlWerkstuecke = 0;
    }
    Werkstueck neuesWS(anzahlWerkstuecke); // Objekt erstellt
    werkstueckListe.push_back(neuesWS); // In der Liste hinzufügen
    std::cout << "[ContextData] register1d: Werkstueck " << neuesWS.getID()
            << " hinzugefuegt." << std::endl;
}


void ContextData::registerId2(Signal signal) {
    if (werkstueckListe.empty()) {
        std::cout << "[ContextData] registerId2: Liste von Werkstueck ist leer!" << std::endl;
        return;
    }
    Werkstueck &ws = werkstueckListe.front();

    switch (signal) {
        case Signal::WERKSTUECK_FLACH:
            ws.setTyp(WerkstueckTyp::F);
            break;
        case Signal::WERKSTUECK_HOCH:
            ws.setTyp(WerkstueckTyp::H);
            break;
        case Signal::WERKSTUECK_MIT_BOHRUNG:
            if (hatMetall || ws.istMetall()) {
                ws.setTyp(WerkstueckTyp::HBM);
            } else {
                ws.setTyp(WerkstueckTyp::HB);
            }

            break;
        default:
            // UNBEKANNT
            ws.setTyp(WerkstueckTyp::UNBEKANNT);
            ws.setAussortieren(true);
            return;
    }
    std::cout << "[ContextData] register2: Typ gesetzt fuer ID " << ws.getID() << std::endl;
}

/*
void ContextData::registerId3(bool wert) {
	if (werkstueckListe.empty()) {
		std::cout << "[ContextData] Liste von Werkstueck ist leer!" << std::endl;
		return;
	}
	Werkstueck &ws = werkstueckListe.front();

	// Metall-Eigenschaft im Werkstück setzen
		ws.setMetall(wert);

	if (ws.istAusschuss()) {
		werkstueckListe.pop_front(); // Entfernung von WS
		return;
	}

	// 2. TYP UPDATE
	// Nur HB -> HBM, wenn Metall erkannt wurde. Alle anderen Typen bleiben wie sie sind.
	if (wert) {
		if (ws.getTyp() == WerkstueckTyp::HB) {
			ws.setTyp(WerkstueckTyp::HBM);
			std::cout << "[ContextData] Typ von HB zu HBM aktualisiert (Metall erkannt)." << std::endl;
		}
	}
}*/

void ContextData::registerId3(bool wert) {
    if (werkstueckListe.empty()) {
        std::cout << "[TRACKING] FEHLER registerId3: Liste ist leer!" << std::endl;
        return;
    }

    Werkstueck &ws = werkstueckListe.front();

    std::cout << "\n[TRACKING] >>> registerId3 fuer WS " << ws.getID() << " (Weiche/Metal Check) <<<" << std::endl;
    if (ws.istAusschuss()) {
        std::cout << "-> WS ist bereits Ausschuss (wird entfernt)." << std::endl;
        // werkstueckListe.pop_front();
        /* if (anzahlWerkstuecke > -1) {
                anzahlWerkstuecke--;
             }*/
        return;
    }
    // LOGIK-KORREKTUR: Additive Metallerkennung

    // Wir setzen Metall auf true, wenn erkannt. Wir überschreiben es NICHT mit false.

    if (wert) {
        ws.setMetall(true);
        if (ws.getTyp() == WerkstueckTyp::HB) {
            ws.setTyp(WerkstueckTyp::HBM);
            std::cout << "    -> Typ-Update: HB -> HBM (Metall erkannt)." << std::endl;
        } else {
            std::cout << "           -> Metall erkannt." << std::endl;
        }
    } else {
        // Falls wert == false, behalten wir den alten Status bei (vielleicht wurde Metall schon vorher erkannt)
        std::cout << "   -> Kein (neues) Metall-Signal." << std::endl;
    }
    // WICHTIG: Die Klassenvariable 'typ' aktualisieren, damit zeitSpeichern() den korrekten Typ kennt!
    this->typ = ws.getTyp();

    std::cout << "           -> ContextData::typ aktualisiert auf: " << (int) this->typ << "\n" << std::endl;
}


Signal ContextData::getMeasuredSignal() const {
    return wsTyp;
}

void ContextData::setMeasuredSignal(const Signal &s) {
    wsTyp = s;
}

//
void ContextData::werkstueckVerlaesstAnlage() {
    if (!werkstueckListe.empty()) {
        Werkstueck ws = werkstueckListe.front();
        std::cout << "********************************************************************" << std::endl;
        std::cout << "[ContextData] Werkstueck ID " << ws.getID() << " verlaesst Anlage." << std::endl;
        std::cout << "********************************************************************" << std::endl;

        werkstueckListe.pop_front();
    }
}

void ContextData::ueberpruefeReihenfolge() {
    if (werkstueckListe.empty()) {
        std::cerr << "[ERROR] Liste ohne Werkstueck!" << std::endl;
        return;
    }

    Werkstueck &ws = werkstueckListe.front();
    bool aus = false;

    // =========================================================
    // ANLAGE 1 (F1)
    // =========================================================
    if (festo1) {
        if (order_index >= sollReihenfolge.size()) {
            order_index = 0; // Wird Reihenfolge erneut
        }

        WerkstueckTyp erwartet = sollReihenfolge[order_index]; // Feste Reihenfolge
        WerkstueckTyp ist = ws.getTyp(); // WS Typ Erkennung von Anlage 1

        if (ist != erwartet) {
            std::cout << "[F1] Falsche Reihenfolge -> AUSSORTIEREN" << std::endl;
            aus = true;
            if (hatMetall) {
                aus = false;
            }
        } else {
            aus = false;
        }
    }

    // =========================================================
    // ANLAGE 2 (F2)
    // =========================================================
    else if (festo2) {
        if (erwarteteTypenQueue.empty()) {
            std::cout << "[F2] WerkstueckListe leer! -> AUSSORTIEREN" << std::endl;
            aus = true;
        } else {
            WerkstueckTyp erwartet = erwarteteTypenQueue.front(); // Was Anlage 1 uns sagt
            WerkstueckTyp ist = ws.getTyp(); // WS Typ von Anlage 2

            // -----------------------------------------------------------------
            // 1. Rutsche voll in Anlage 1
            // -----------------------------------------------------------------
            if (erwartet == WerkstueckTyp::UNBEKANNT) {
                std::cout << "[F2] F1 shickt mir UNBEKANNT." << std::endl;
                std::cout << " -> AUSSORTIEREN " << std::endl;
                //erwarteteTypenQueue.pop_front();
                aus = true;
            }

            // -----------------------------------------------------------------
            // NACHRICHT == TYP ERKANNT
            // -----------------------------------------------------------------
            else if (ist == erwartet) {
                std::cout << "[F2] OK -> WEITER" << std::endl;
                //erwarteteTypenQueue.pop_front();
                aus = false;
            }

            // -----------------------------------------------------------------
            // NACHRICHT != TYP ERKANNT
            // -----------------------------------------------------------------
            else {
                std::cout << "[F2] WSTyp bei Anlage 2 ist anders als bei Anlage 1 -> AUSSORTIEREN" << std::endl;
                //erwarteteTypenQueue.pop_front();
                anders = true;
                aus = true;
            }
        }
    }

    ws.setAussortieren(aus);
}

void ContextData::advanceOrder() {
    if (festo1) {
        order_index++;
        if (order_index >= sollReihenfolge.size()) {
            order_index = 0;
            std::cout << "[Data] Reihenfolge von WS erneut" << std::endl;
        }
        std::cout << "[Data] F1: Order Index ++ -> " << order_index << std::endl;
    } else if (festo2) {
        if (!erwarteteTypenQueue.empty()) {
            erwarteteTypenQueue.pop_front();
            std::cout << "[Data] F2: WS: " << erwarteteTypenQueue.size() << std::endl;
        }
    }
}

//-----------------------------
//ANLAGE 1
//-----------------------------
int ContextData::getTypCodeForSending() {
    if (werkstueckListe.empty()) return -1;
    WerkstueckTyp t = werkstueckListe.front().getTyp();

    switch (t) {
        case WerkstueckTyp::F:
            return (int) Signal::WERKSTUECK_FLACH;
        case WerkstueckTyp::H:
            return (int) Signal::WERKSTUECK_HOCH;
        case WerkstueckTyp::HB:
            return (int) Signal::WERKSTUECK_MIT_BOHRUNG;
        case WerkstueckTyp::HBM:
            return (int) Signal::WERKSTUECK_MIT_BOHRUNG_M;
        case WerkstueckTyp::UNBEKANNT:
            return (int) Signal::WERKSTUECK_UNBEKANNT;
        default:
            return -1; // Unbekannt / Fehler
    }
}

//-----------------------------
//ANLAGE 2
//-----------------------------
void ContextData::empfangeTypVonF1(int typCode) {
    WerkstueckTyp typ = WerkstueckTyp::UNBEKANNT;
    switch (typCode) {
        case (int) Signal::WERKSTUECK_FLACH:
            typ = WerkstueckTyp::F;
            break;
        case (int) Signal::WERKSTUECK_HOCH:
            typ = WerkstueckTyp::H;
            break;
        case (int) Signal::WERKSTUECK_MIT_BOHRUNG:
            typ = WerkstueckTyp::HB;
            break;
        case (int) Signal::WERKSTUECK_MIT_BOHRUNG_M:
            typ = WerkstueckTyp::HBM;
            break;

        default:
            typ = WerkstueckTyp::UNBEKANNT;
            std::cerr << "[ContextData] Festo 1 sagt: Werkstuck ist unbekannt"
                    << typCode << std::endl;
            break;
    }

    erwarteteTypenQueue.push_back(typ);

    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "[ContextData F2] MSG EMPFANGEN" << std::endl;
    std::cout << " -> Raw Code: " << typCode << std::endl;
    std::cout << " -> Mapped Typ: " << (int) typ << std::endl;
    std::cout << " -> Warteschlange Size: " << erwarteteTypenQueue.size() << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
}

//------Hilfsmethoden---------

void ContextData::speichereZeit(const std::string &name, double millis) {
    zeiten[name] = millis;
}