/**
 * @file Werkstueck.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse Werkstueck.
 *
 * Repräsentiert ein physisches Werkstück im System.
 * Speichert alle relevanten Eigenschaften, die während des Transports ermittelt werden,
 * wie ID, gemessene Höhenwerte, Typ (flach/hoch/Bohrung), Metall-Status und Position.
 */

#ifndef HFSM_WERKSTUECK_HPP_
#define HFSM_WERKSTUECK_HPP_

#include "common/Types.h"
#include <sstream>
#include <iostream>
#include <iomanip>

class Werkstueck {
private:
    int anzahl;
    WerkstueckTyp typ;
    bool hatMetall;
    bool aussortieren;

public:
    Werkstueck(int nr) : anzahl(nr), typ(WerkstueckTyp::UNBEKANNT), hatMetall(false), aussortieren(false) {
    }

    virtual ~Werkstueck() {
    }

    //Setter
    void setTyp(WerkstueckTyp t) { typ = t; }
    void setMetall(bool wert) { hatMetall = wert; }
    void setAussortieren(bool wert) { aussortieren = wert; }

    // Getter
    WerkstueckTyp getTyp() const { return typ; }
    bool istMetall() const { return hatMetall; }
    bool istAusschuss() { return aussortieren; }

    std::string getID() {
        std::stringstream ss;

        ss << std::setw(2) << std::setfill('0') << anzahl;

        switch (typ) {
            case WerkstueckTyp::F:
                ss << "F00";
                break;

            case WerkstueckTyp::H:
                ss << "H00";
                break;

            case WerkstueckTyp::HB:

                if (hatMetall) {
                    ss << "HBM";
                } else {
                    ss << "HB0";
                }
                break;

            case WerkstueckTyp::HBM:
                ss << "HBM";
                break;

            default:
                ss << "000";
                break;
        }

        return ss.str();
    }
};

#endif /* HFSM_WERKSTUECK_HPP_ */