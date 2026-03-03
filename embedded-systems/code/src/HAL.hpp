/**
* @file HAL.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk
 * @date 20.01.2026
 * @version 1.0
 * @brief Hardware Abstraction Layer (HAL) Schnittstelle.
 *
 * Definiert die Klasse HAL, die als Schnittstelle zwischen der Logik (FSM/Dispatcher)
 * und der physischen Hardware (Sensoren, Aktoren, GPIOs) dient.
 */

#ifndef SRC_HAL_HPP_
#define SRC_HAL_HPP_

#include <sys/dispatch.h>

#include "HAL/motor/motor.hpp"
#include "HAL/trafficlight/trafficlight.hpp"
#include "HAL/sorter/sorterbarrier.hpp"
#include "HAL/sorter/sorterhammer.hpp"
#include "HAL/buttonlight/button.hpp"
#include "HAL/light/light.hpp"
#include "HAL/sensor/sensor.hpp"
#include "HAL/sensor/ADCHandler.hpp"
#include "Dispatcher/IDispatcher.hpp"


#define MAP_SIZE 0x1000
#define GPIO_BANK_1 0x4804C000


#define GPIO_OE      			0x134
#define GPIO_DATAIN  			0x138
#define GPIO_IRQSTATUS_0		0x2C
#define GPIO_IRQSTATUS_SET_0	0x34
#define GPIO_RISINGDETECT		0x148
#define GPIO_FALLINGDETECT		0x14C

#define OFFSET_IN_REG   0x138     // GPIO_DATAIN
#define OFFSET_SET_REG  0x194     // GPIO_SETDATAOUT
#define OFFSET_CRL_REG  0x190     // GPIO_CLEARDATAOUT


#define POINT_ATTACH_HAL1 "HAL_1"
#define POINT_ATTACH_HAL2 "HAL_2"

class HAL {
private:
    int disID;
    IDispatcher *dispatcher;
    name_attach_t *attach;
    uintptr_t gpio_ptr;


    /* Aktorik-Objekte */
    Motor *motor;
    AmpelLED *ampel_rot;
    AmpelLED *ampel_gelb;
    AmpelLED *ampel_gruen;
    SortiererWeiche *weiche;
    SortiererAuswerfer *auswerfer;

    Licht *q1;
    Licht *q2;

    /* Sensorik-Objekte */
    Taste *taste_start;
    Taste *taste_stop;
    Taste *taste_reset;
    Taste *taste_estop;

    Sensor *einlauf;
    Sensor *hoehenmesser;
    Sensor *metall;
    Sensor *sortierer;
    Sensor *rutsche;
    Sensor *auslauf;

public:
    HAL(int id, IDispatcher *dispatcher);

    virtual ~HAL();

    int idd;

    void run();
};

#endif /* SRC_HAL_HPP_ */