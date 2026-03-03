/**
 * @file Util.h
 * @author Alaa Al-Sowmali, Marta Muñoz Merino, Emre Öztürk, Oleksandr Maksymov
 * @date 20.01.2026
 * @version 1.0
 * @brief Hilfsmakros für Logging und Thread-Paussierung.
 */

#ifndef UTIL_H
#define UTIL_H

// for Error
#include <cstdio>

// for wait
#include <thread>
#include <chrono>


#define DBG_ERROR(err) std::perror(err);

#define WAIT(x) (std::this_thread::sleep_for(std::chrono::milliseconds(x)))

#define _BV(x) (1<<(x))

#endif
