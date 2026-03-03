/**
 * @file Anlage.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Anlage-Klasse.
 *
 * Enthält die Initialisierungslogik und das Thread-Management für das Gesamtsystem.
 */

#include "Anlage.hpp"
#include <unistd.h>


Anlage::Anlage(int id,std::string nachbarHost) : id(id) {

    dispatcher = new Dispatcher(id,nachbarHost);
    hal = new HAL(id, dispatcher);
    actions = new Actions(dispatcher);
    adc = nullptr;
    if(id == 1){
    	 context = new Context(actions,true,false); // True fuer Festo 1
    }

    if(id == 2){
      	 context = new Context(actions,false,true); // True fuer festo 2
      }

    std::cout << "[Anlage]  Festo Anlage " << id << " ist initialisiert!" << std::endl;
}

Anlage::~Anlage() {
	stoppen();
	if(adc) delete adc;
    if(dispatcher) delete dispatcher;
    if(hal) delete hal;
    if(context) delete context;
    if(actions) delete actions;
}

void Anlage::starten(){
	//  HAL Thread
				tHAL = std::thread([this]() {
				        this->hal->run();
				    });
				std::cout << "[Anlage]  HAL  ist an!" << std::endl;

	// Logik Thread
		 tHFSM = std::thread([this]() {
		         this->context->run();
		     });

		 std::cout << "[Anlage]  Logik  ist an!" << std::endl;

	// Dispatcher Thread
	tDispatcher = std::thread([this](){
	    this->dispatcher->run();
	});

	 std::cout << "[Anlage]  Dispatcher  ist an!" << std::endl;

		sleep(2);

	 adc = new ADCHandler(dispatcher); // Man muss adc hier initialisieren

	 std::cout << "[Anlage]  ADCHandler  ist an!" << std::endl;
}

void Anlage::stoppen(){
	warten();
}

void Anlage::warten(){
	 if(tDispatcher.joinable()) tDispatcher.join();
	 if(tHAL.joinable()) tHAL.join();
	 if(tHFSM.joinable()) tHFSM.join();
}


