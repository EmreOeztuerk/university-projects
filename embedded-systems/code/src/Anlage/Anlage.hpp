/**
 * @file Anlage.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Klasse Anlage (System-Container).
 *
 * Diese Klasse repräsentiert die gesamte Maschine (Festo 1 oder Festo 2).
 * Sie initialisiert alle Subsysteme (Dispatcher, HAL, FSM, ADC), verwaltet
 * die Threads und sorgt für ein geordnetes Hoch- und Herunterfahren.
 */

#ifndef ANLAGE_ANLAGE_HPP_
#define ANLAGE_ANLAGE_HPP_

#include <thread>
#include <iostream>


#include "../Dispatcher/dispatcher.hpp"
#include "../HAL.hpp"
#include "../HAL/sensor/ADCHandler.hpp"
#include "../HFSM/Context.h"
#include "../HFSM/Actions.h"

class Anlage{
private:
	Dispatcher* dispatcher;
	HAL* hal;
	ADCHandler* adc;
	Actions* actions; // Logik
	Context* context;

	std::thread tDispatcher;
	std::thread tHAL;
	std::thread tHFSM;

	int id;


public:
	Anlage(int id,std::string nachbarHost);
	virtual ~Anlage();

	 // Alle threads starten
	void starten();

	// Alle threads terminieren
	void stoppen();

	// Auf alle threads warten bis sie terminieren
	void warten();

};



#endif /* ANLAGE_ANLAGE_HPP_ */
