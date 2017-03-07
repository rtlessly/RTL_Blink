/*******************************************************************************
 * Implementation file for LED Blinker
 *
 * created 09 Jul 2016
 * by R. Terry Lessly
 *
 *******************************************************************************/
#define DEBUG 0
 
 #include <Arduino.h>
#include <Debug.h>
#include <EventDispatcher.h>
#include "Blinker.h"


//******************************************************************************
// Constructor
//******************************************************************************

Blinker::Blinker(uint8_t ledPin, unsigned int period, bool oneShot)
{
    _state.pin = ledPin;
    _state.mode = oneShot ? ONE_SHOT_MODE : REPEAT_MODE;
    _state.running = false;
    _state.led = 0;
    _period = period;

    EventDispatcher::Add(*this); 
}


//******************************************************************************
// Public interface
//******************************************************************************

void Blinker::Start(unsigned int period, bool oneShot)
{
    if (_state.running) return; // Ignore if already running

    _state.running = true;
    _state.mode = oneShot ? ONE_SHOT_MODE : REPEAT_MODE;
    pinMode(_state.pin, OUTPUT);

    if (period > 0) _period = period;

    _startTime = millis();

    On();   // Aways start with LED on
}


void Blinker::Stop()
{
    _state.running = false;
    Off();  // Ensure the LED off
}


void Blinker::OneShot(unsigned int period)
{
    Start(period, true);
}


void Blinker::Poll()
{
    if (!_state.running || _period == 0) return;

    unsigned long now = millis();

    if ((now - _startTime) >= _period)
    {
        Toggle();

        if (_state.mode == REPEAT_MODE) _startTime = now; else Stop();
    }
}


//******************************************************************************
// Internal methods
//******************************************************************************

inline void Blinker::On()
{
    digitalWrite(_state.pin, _state.led=HIGH);
}


inline void Blinker::Off()
{
    digitalWrite(_state.pin, _state.led=LOW);
}


inline void Blinker::Toggle()
{
    digitalWrite(_state.pin, _state.led^=HIGH);
}
