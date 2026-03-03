/**
 * @file ADCHandler.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung des ADCHandlers.
 *
 * Verarbeitet Pulse vom ADC-Treiber, konvertiert Rohwerte in Höhendaten
 * und sendet entsprechende Signale (z.B. Werkstück-Typ) an den Dispatcher.
 */

#include "ADCHandler.hpp"
#include "../../Dispatcher/Signal.hpp"


ADCHandler::ADCHandler(Dispatcher* dispatcher) : werk(20){
	_dispatcher = dispatcher;

	int server_chid = dispatcher->getKanal();

	if (server_chid <= 0) {
	        std::cerr << "[ADC] Fehler: Dispatcher ist ungueltig!" << std::endl;
	        return;
	    }

     conIDDisp = ConnectAttach(0,0,server_chid, _NTO_SIDE_CHANNEL,0);
     if(conIDDisp == -1){
    	 	 std::cout<<" [ADC Handler]: ConnectAttach failed!!"<<std::endl;
    	 return;
     }

     std::cout<<" [ADC Handler]: Mit HAL verbunden!"<<std::endl;


     chanID = ChannelCreate(0);
     if(chanID < 0){
         	 	 std::cout<<" [ADC Handler]: ChannelCreate failed!!"<<std::endl;
         	 return;
          }

     conID = ConnectAttach(0,0,chanID, _NTO_SIDE_CHANNEL,0);

     if(conID < 0){
            	 	 std::cout<<"[ADC Handler]: ConnectAttach von conID failed!!"<<std::endl;
            	 return;
             }

     static TSCADC tsc;
	 adc = new ADC(tsc);
	 adc->registerADCISR(conID, 2);

	 receivingThread = std::thread(&ADCHandler::receivingRoutine, this, chanID,adc);
	 adc->sample();
}

ADCHandler::~ADCHandler(){
	    ConnectDetach(conIDDisp);
	    ConnectDetach(conID);
	    ChannelDestroy(chanID);
	    delete adc;
}


void ADCHandler::receivingRoutine(int channelID,ADC* adc){

	ThreadCtl(_NTO_TCTL_IO,0);

	_pulse msg;
	bool isMeasuring = false; // Flag, um LSH_UB nur einmal beim Start zu senden

	while(true){
		 int recvid = MsgReceivePulse(channelID, &msg, sizeof(_pulse), nullptr);
		        if (recvid < 0) {
		            perror("MsgReceivePulse failed!");
		            exit(EXIT_FAILURE);
		        }
		        if (recvid == 0) {

		        	if(msg.code == HOESENSOR_SIGNAL){
		        		adc->sample();

		        		uint16_t raw = static_cast<uint16_t>(msg.value.sival_int & 0x0FFF);
		        		float height_mm = (raw / 4095.0f) * 25.0f; // Umwandlung von raw in mm

		        		//int value_for_detection = static_cast<int>(raw);
		        		werk.kalibrieren(height_mm); // Hoehe kalibrieren

		        		// 1. LSH_UB Signal senden (Beim Start der Unterbrechung)
		        		if (height_mm < werk.getThreshold() && !isMeasuring) {
		        			isMeasuring = true;
		        			// Sende S_LSH_UB an den Dispatcher
		        			_dispatcher->sendenNachricht(Signal::S_LSH_UB,0);
		        		}

		        		// Typ Erkennung
		        		std::string erkannt = werk.processValue(height_mm);

		        		if (!erkannt.empty()) {

		        			Signal typSignal;

		        			// Hier die Zuordnung des erkannten Typs zum Signal-Code
		        			if (erkannt.find("Flach ohne Bohrung") != std::string::npos) {
		        				typSignal = Signal::WERKSTUECK_FLACH;
		        			} else if (erkannt.find("Hoch ohne Bohrung") != std::string::npos) {
		        				typSignal = Signal::WERKSTUECK_HOCH;
		        			}else if (erkannt.find("Hoch mit Bohrung") != std::string::npos) {
		        				typSignal = Signal::WERKSTUECK_MIT_BOHRUNG;
		        			}else if (erkannt.find("Unbekannt / Sonderfall") != std::string::npos) {
		        				typSignal = Signal::WERKSTUECK_UNBEKANNT;
		        			}
//		        			s

		        			// Sende den erkannten Typ als Event an den Dispatcher
		        			_dispatcher->sendenNachricht(typSignal,0);

		        			// Sende LSH_NB, da die Messung beendet ist.
		        			if (isMeasuring) {
		        				isMeasuring = false; // Zurücksetzen für das nächste Werkstück
		        				if (MsgSendPulse(conIDDisp, _NTO_SIDE_CHANNEL, Signal::S_LSH_NUB, 0) == -1) {
		        					std::cerr << "[ADC Handler]: Fehler beim Senden von LSH_NB an Dispatcher!" << std::endl;
		        				} else {
		        					//std::cout << "[ADC Handler]: --> LSH_NB (" << (int)Signal::S_LSH_NUB << ") an Dispatcher gesendet.\n";
		        				}
		        			}

		        		}

		        	}

		        }

	}
}




