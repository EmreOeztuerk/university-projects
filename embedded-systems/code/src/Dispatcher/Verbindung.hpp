/**
 * @file Verbindung.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Verbindung-Klasse für die Qnet-Kommunikation.
 *
 * Verwaltet die Verbindung zur benachbarten Anlage (Festo 1 <-> Festo 2),
 * inklusive Verbindungsaufbau, Heartbeat-Überwachung und Nachrichtenversand.
 */

#ifndef VERBINDUNG_HPP_
#define VERBINDUNG_HPP_

#include <string>
#include <thread>
#include <atomic>
#include <cstdbool>
#include <iostream>
#include <dirent.h>
#include <cstring>
#include <fcntl.h>
#include <cerrno>
#include <sys/dispatch.h>
#include "signal.hpp"

class Dispatcher;

#define AUSZEIT 300

class Verbindung {
private:
    enum class Zustand {
        VERBUNDEN,
        HEARTBEAT1_VERPASST,
        HEARTBEAT2_VERPASST,
        WARTE_AUF_VERBINDUNG
    };

    Dispatcher *dis1;
    int nachbarnID;
    int meinID;
    Zustand aktuellerZustand;
    int herzschlagZaehler;
    std::string nachbarHostname;


    void entryAction(Zustand zustand);

    void exitAction(Zustand zustand);

    bool roteLamopeAus = false;

public:
    Verbindung(Dispatcher *dispatcher, std::string nachbarHost);

    virtual ~Verbindung();

    void verbinden();

    /*
		 * @brief Bietet Kommunikation zwischen Dispatchers
		 */
    void sendenAnNachbarn(int code, int wert);

    /*
		* @brief Setter: Setzt nachbarnID
		*/
    void setNachbarnID(int nachbarnID);

    /*
		* @brief Getter: Setzt herzschlagZaehler zurueck
		*/
    void resetHerzschlag();

    /*
		* @brief Getter: Liefert zurueck, ob die Verbindung gueltig ist
		* @return true oder false
		*/
    bool checkTick();

    /*
		* @brief Getter: Liefert zurueck, ob Anlagen verbunden sind
		* @return true oder false
		*/
    bool istVerbunden() const;

    /*
		* @brief Getter HerzschlagZaehler
		*/
    int getHerzschlagZaehler() const;

    /*
		* @brief Sendet "ich bin noch da!"
	    */
    void sendKeepAlive();
};

#endif /* VERBINDUNG_HPP_ */