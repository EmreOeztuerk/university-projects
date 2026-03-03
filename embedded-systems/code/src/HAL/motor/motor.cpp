/**
 * @file motor.cpp
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Implementierung der Motor-Klasse.
 *
 * Enthält die Logik zur Ansteuerung des Förderbandmotors über GPIO-Registerzugriffe
 * (Starten, Stoppen, Richtungswechsel, Geschwindigkeitswahl).
 */

#include "motor.hpp"

/**
 * Bei Erstellung eines Motor-Objektes ist der Motor stillstehend.
 */
Motor::Motor() {
    motorStatus = false;
    geschwindigkeit = Geschw::MOTOR_STOP;
    richtung = Richtung::MOTOR_STEHEN;
}

/**
 * @return: Geschwindigkeit des Motors.
 */
Geschw Motor::getGeschw() {
    return geschwindigkeit;
}

/**
 * @return: Richtung des Motors.
 */
Richtung Motor::getRichtung() {
    return richtung;
}

/**
 * @return: Angabe ob der Motor laeuft oder steht.
 */
bool Motor::getMotorStatus() {
    return motorStatus;
}

/**
 * Setzt die aktuelle Geschwindigkeit des Motors.
 *
 * @param given_geschw: zu setzende Geschwindigkeit des Motors.
 */
void Motor::setGeschw(_Uint32t given_geschw) {
    if (given_geschw == Geschw::MOTOR_SCHNELL) {
        geschwindigkeit = MOTOR_SCHNELL;
    } else if (given_geschw == Geschw::MOTOR_LANGSAM) {
        geschwindigkeit = MOTOR_LANGSAM;
    } else if (given_geschw == Geschw::MOTOR_STOP) {
        geschwindigkeit = MOTOR_STOP;
    }
}

/**
 * Setzt die aktuelle Richtung des Motors.
 *
 * @param given_richtung: zu setzende Richtung des Motors.
 */
void Motor::setRichtung(_Uint32t given_richtung) {
    if (given_richtung == Richtung::MOTOR_STEHEN) {
        richtung = Richtung::MOTOR_STEHEN;
    } else if (given_richtung == Richtung::MOTOR_RECHTS) {
        richtung = MOTOR_RECHTS;
    } else if (given_richtung == Richtung::MOTOR_LINKS) {
        richtung = MOTOR_LINKS;
    }
}

/**
 * Startet den Motor in angegebene Richtung mit angegebenen Geschwindigkeit.
 *
 * @param given_direction: angegebene Laufrichtung.
 * @param given_geschw: angegebene Motorgeschwindigkeit.
 */
void Motor::startMotor(_Uint32t given_richtung, _Uint32t given_geschw) {
    if (!getMotorStatus()) {
        ThreadCtl(_NTO_TCTL_IO, 0);
        uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
        out32((uintptr_t)(gpio_bank_1 + OFFSET_SET_REG), given_richtung);
        out32((uintptr_t)(gpio_bank_1 + OFFSET_SET_REG), given_geschw);
        motorStatus = true;
        setRichtung(given_richtung);
        setGeschw(given_geschw);
    } else {
        Motor::wechselRichtung(given_richtung);
        Motor::wechselGeschw(given_geschw);
    }
}

/**
 * Haltet den Motor an.
 */
void Motor::stopMotor() {
    if (getMotorStatus()) {
        ThreadCtl(_NTO_TCTL_IO, 0);
        uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
        out32((uintptr_t)(gpio_bank_1 + OFFSET_CRL_REG), getRichtung());
        out32((uintptr_t)(gpio_bank_1 + OFFSET_CRL_REG), getGeschw());
        motorStatus = false;
        setRichtung(Richtung::MOTOR_STEHEN);
        setGeschw(Geschw::MOTOR_STOP);
    }
}

/**
 * Wechselt die aktuelle Geschwindigkeit des Motors auf die angegebene.
 *
 * @param given_geschw: zu wechselnde Geschwindigkeit.
 */
void Motor::wechselGeschw(_Uint32t given_geschw) {
    if (given_geschw != getGeschw()) {
        ThreadCtl(_NTO_TCTL_IO, 0);
        uintptr_t gpio_bank_1 = mmap_device_io(MAP_SIZE, (uint64_t) GPIO_BANK_1);
        out32((uintptr_t)(gpio_bank_1 + OFFSET_CRL_REG), getGeschw());
        out32((uintptr_t)(gpio_bank_1 + OFFSET_SET_REG), given_geschw);
        setGeschw(given_geschw);
    }
}

/**
 * Wechselt die aktuelle Richtung des Motors auf die angegebene.
 *
 * @param given_richtung: zu wechselnde Richtung.
 */
void Motor::wechselRichtung(_Uint32t given_richtung) {
    if (given_richtung != getRichtung()) {
        Geschw akt_geschw = getGeschw();
        stopMotor();
        startMotor(given_richtung, akt_geschw);
    }
}