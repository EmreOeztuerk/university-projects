/**
 * @file HAL.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der zentralen HAL-Klasse.
 *
 * Erstellt und initialisiert alle Hardware-Komponenten (Motoren, Sensoren, Ampeln, Taster)
 * und verarbeitet im `run()`-Thread eingehende Befehle vom Dispatcher, um die Hardware zu steuern.
 */

#include "HAL.hpp"
#include <unistd.h>

HAL::HAL( int id, IDispatcher* disp) : disID(-1), dispatcher(disp){
	attach = nullptr;
	gpio_ptr = MAP_DEVICE_FAILED;

	/* Aktorik-Objekte */
	motor = new Motor();
//	ampel = new AmpelLED();
	ampel_rot = new AmpelLED();
	ampel_gelb = new AmpelLED();
	ampel_gruen = new AmpelLED();
	weiche = new SortiererWeiche();
	auswerfer = new SortiererAuswerfer();

	q1 = new Licht(TypLicht::LED_Q1);
	q2 = new Licht(TypLicht::LED_Q2);

	/* Sensorik-Objekte */
	taste_start = new Taste(TypTaste::TASTE_START, dispatcher, id);
	taste_stop = new Taste(TypTaste::TASTE_STOP, dispatcher, id);
	taste_reset = new Taste(TypTaste::TASTE_RESET, dispatcher, id);
	taste_estop = new Taste(TypTaste::E_STOP, dispatcher, id);

	einlauf = new Sensor(SensorTyp::LS_EINLAUF, dispatcher, id);
	hoehenmesser = new Sensor(SensorTyp::HOEHENMESSER, dispatcher, id);
	metall = new Sensor(SensorTyp::METALL, dispatcher, id);
	sortierer = new Sensor(SensorTyp::LS_SORTIERER, dispatcher, id);
	rutsche = new Sensor(SensorTyp::LS_RUTSCHE, dispatcher, id);
	auslauf = new Sensor(SensorTyp::LS_AUSLAUF, dispatcher, id);

	this->idd = id; // Speichern der ID in der HAL Instanz
	//idd =id;
}

HAL::~HAL() {
    if (attach) name_detach(attach, 0);
}

void HAL::run(){

	 if(idd == 1){
		   attach = name_attach(NULL, POINT_ATTACH_HAL1, 0);
		   std::cerr << "[HAL] Kanal erzeugt 1!" << std::endl;
	   }else if(idd == 2){
		   attach = name_attach(NULL, POINT_ATTACH_HAL2, 0);
		   std::cerr << "[HAL] Kanal erzeugt 2!" << std::endl;
	   }

	std::thread thread_tstart(&Taste::taste_isr, taste_start);
	std::thread thread_tstop(&Taste::taste_isr, taste_stop);
	std::thread thread_treset(&Taste::taste_isr, taste_reset);
	std::thread thread_testop(&Taste::taste_isr, taste_estop);

	std::thread thread_einlauf(&Sensor::sensor_isr, einlauf);
	std::thread thread_hoehenmesser(&Sensor::sensor_isr, hoehenmesser);
	std::thread thread_metall(&Sensor::sensor_isr, metall);
	std::thread thread_sortierer(&Sensor::sensor_isr, sortierer);
	std::thread thread_rutsche(&Sensor::sensor_isr, rutsche);
	std::thread thread_auslauf(&Sensor::sensor_isr, auslauf);

	std::cout << "[Debug HAL::run] Sensor-Threads" << std::endl;



	if (attach == NULL) {
		std::cerr << "[HAL] Fehler: Kanal konnte nicht erzeugt werden! '"   << std::endl;
		return;
	}

	//std::cout << "[HAL] Kanal-Name: " << POINT_ATTACH_HAL << std::endl;

	Signal signal;
	int wert;

	while (true) {
		bool erfolgreich = dispatcher->nachrichtEmpfaengen(attach->chid, signal, wert);

		if(erfolgreich){
			int code = signal;

			switch (code) {
				/* Motor */
				case Signal::CMD_MTR_STOP:
					motor->stopMotor();
					std::cout << "[HAL] Motor stoppt." << std::endl;
					break;

				case Signal::CMD_MTR_RECHTS_S:
					motor->startMotor(Richtung::MOTOR_RECHTS, Geschw::MOTOR_SCHNELL);
					std::cout << "[HAL] Motor laeuft." << std::endl;
					break;

				case Signal::CMD_MTR_RECHTS_L:
					motor->startMotor(Richtung::MOTOR_RECHTS, Geschw::MOTOR_LANGSAM);
					std::cout << "[HAL] Motor laeuft langsam." << std::endl;
					break;

				/* Ampel */
				case Signal::CMD_LED_AMP_ROT_AN:
//					ampel->lichtAn(Farbe::LED_ROT);
					ampel_rot->lichtAn(Farbe::LED_ROT);
					std::cout << "[HAL] Ampellicht Rot an." << std::endl;
					break;

				case Signal::CMD_LED_AMP_ROT_AUS:
//					ampel->lichtAus(Farbe::LED_ROT);
					ampel_rot->lichtAus(Farbe::LED_ROT);
					//std::cout << "[HAL] Ampellicht Rot aus." << std::endl;
					break;

				case Signal::CMD_LED_AMP_ROT_BS:
//					ampel->blinkenAn(Farbe::LED_ROT, Modus::BLINK_SCHNELL);
					ampel_rot->blinkenAn(Farbe::LED_ROT, Modus::BLINK_SCHNELL);
					std::cout << "[HAL] Ampellicht Rot blinkt schnell." << std::endl;
					break;

				case Signal::CMD_LED_AMP_ROT_BL:
//					ampel->blinkenAn(Farbe::LED_ROT, Modus::BLINK_LANGSAM);
					ampel_rot->blinkenAn(Farbe::LED_ROT, Modus::BLINK_LANGSAM);
					std::cout << "[HAL] Ampellicht Rot blinkt langsam." << std::endl;
					break;

				case Signal::CMD_LED_AMP_GRN_AN:
//					ampel->lichtAn(Farbe::LED_GRUEN);
					ampel_gruen->lichtAn(Farbe::LED_GRUEN);
					std::cout << "[HAL] Ampellicht Gruen an." << std::endl;
					break;

				case Signal::CMD_LED_AMP_GRN_AUS:
//					ampel->lichtAus(Farbe::LED_GRUEN);
					ampel_gruen->lichtAus(Farbe::LED_GRUEN);
					std::cout << "[HAL] Ampellicht Gruen aus." << std::endl;
					break;

				case Signal::CMD_LED_AMP_GLB_AN:
					std::cout << "[HAL] Signal Ampellicht Gelb an." << std::endl;
//					ampel->lichtAn(Farbe::LED_GELB);
					ampel_gelb->lichtAn(Farbe::LED_GELB);
					break;

				case Signal::CMD_LED_AMP_GLB_AUS:
//					ampel->lichtAus(Farbe::LED_GELB);
					ampel_gelb->lichtAus(Farbe::LED_GELB);
					std::cout << "[HAL] Ampellicht Gelb aus." << std::endl;
					break;

				case Signal::CMD_LED_AMP_GLB_B:
//					ampel->blinkenAn(Farbe::LED_GELB, Modus::BLINK_LANGSAM);
					ampel_gelb->blinkenAn(Farbe::LED_GELB, Modus::BLINK_LANGSAM);
					std::cout << "[HAL] Ampellicht Gelb blinkt langsam." << std::endl;
					break;

				/* Weiche */
				case Signal::CMD_WCH_AN:
					weiche->durchlassen();
					std::cout << "[HAL] Weiche laesst durch." << std::endl;
					break;

				case Signal::CMD_WCH_AUS:
					weiche->aussortieren();
					std::cout << "[HAL] Weiche schliesst." << std::endl;
					break;

				/* Auswerfer */
				case Signal::CMD_ASWERF_AN:
					auswerfer->aussortieren();
					std::cout << "[HAL] Auswerfer wirft raus." << std::endl;
					break;

				case Signal::CMD_ASWERF_AUS:

					auswerfer->durchlassen();
					std::cout << "[HAL] Auswerfer laesst durch." << std::endl;
					break;

				case Signal::CMD_DURCHLASSEN:
					if(idd == 1) weiche->durchlassen();
					if(idd == 2)auswerfer->durchlassen();
					std::cout << "[HAL] Durchlassen." << std::endl;
					break;

				case Signal::CMD_AUSSORTIEREN:
					if(idd == 1) weiche->aussortieren();
					if(idd == 2) auswerfer->aussortieren();
					std::cout << "[HAL] Aussortieren." << std::endl;
					break;

				/* LEDs von Taster */
				case Signal::CMD_LED_ST_AN:
					taste_start->ledAn();
					std::cout << "[HAL] Leuchte der Taste \"Start\" an." << std::endl;
					break;

				case Signal::CMD_LED_ST_AUS:
					taste_start->ledAus();
					std::cout << "[HAL] Leuchte der Taste \"Start\" aus." << std::endl;
					break;

//				case Signal::CMD_LED_RT_AN:
//					taste_reset->ledAn();
//					std::cout << "[HAL] Leuchte der Taste \"Reset\" an." << std::endl;
//					break;
//
//				case Signal::CMD_LED_RT_AUS:
//					taste_reset->ledAus();
//					std::cout << "[HAL] Leuchte der Taste \"Reset\" aus." << std::endl;
//					break;

				/* LEDs Q1 und Q2 */
				case Signal::CMD_LED_Q1_AN:
					q1->ledAn();
					std::cout << "[HAL] Leuchte Q1 an." << std::endl;
					break;

				case Signal::CMD_LED_Q1_AUS:
					q1->ledAus();
					std::cout << "[HAL] Leuchte Q1 aus." << std::endl;
					break;

				case Signal::CMD_LED_Q2_AN:
					q2->ledAn();
					//std::cout << "[HAL] Leuchte Q2 an." << std::endl;
					break;

				case Signal::CMD_LED_Q2_AUS:
					q2->ledAus();
					std::cout << "[HAL] Leuchte Q2 aus." << std::endl;
					break;


				default:
					break;
			}
		}
	}
	if(thread_tstart.joinable()) thread_tstart.join();
	if(thread_tstop.joinable()) thread_tstop.join();
	if(thread_treset.joinable()) thread_treset.join();
	if(thread_testop.joinable()) thread_testop.join();

	if(thread_einlauf.joinable()) thread_einlauf.join();
	if(thread_hoehenmesser.joinable()) thread_hoehenmesser.join();
	if(thread_metall.joinable()) thread_metall.join();
	if(thread_sortierer.joinable()) thread_sortierer.join();
	if(thread_rutsche.joinable()) thread_rutsche.join();
	if(thread_auslauf.joinable()) thread_auslauf.join();
}
