/**
 * @file motor.hpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Definition der Motor-Klasse zur Steuerung des Förderbands.
 *
 * Verwaltet die Hardware-Ansteuerung des Motors (Start, Stopp, Richtung, Geschwindigkeit)
 * über GPIO-Zugriffe.
 */

#ifndef MOTOR_H
#define MOTOR_H

#include "../../gpio_address.hpp"
#include "direction.hpp"
#include "speed.hpp"

/**
 *
 */
class Motor {
private:
    bool motorStatus;
    Geschw geschwindigkeit;
    Richtung richtung;

    void setGeschw(_Uint32t given_geschw);

    void setRichtung(_Uint32t given_richtung);

public:
    Motor();

    virtual ~Motor() {
    };

    Geschw getGeschw();

    Richtung getRichtung();

    bool getMotorStatus();

    void startMotor(_Uint32t given_richtung, _Uint32t given_geschw);

    void stopMotor();

    void wechselGeschw(_Uint32t given_geschw);

    void wechselRichtung(_Uint32t given_richtung);

    void action(_Uint32t a, _Uint32t b);

    void finish(_Uint32t a, _Uint32t b);
};

#endif