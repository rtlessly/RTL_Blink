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
#include <Streaming.h>
#include "RTL_Blinker.h"


static DebugHelper Debug("Blinker");


static char* BlinkerID = "Blinker";


//******************************************************************************
// Constructor
//******************************************************************************
Blinker::Blinker(uint8_t ledPin, uint16_t period, bool oneShot)
{
    _id = BlinkerID;
    _state.pin = ledPin;
    _state.mode = oneShot ? ONE_SHOT_MODE : REPEAT_MODE;
    _state.running = false;
    _state.led = 0;
    _period = period;
}


Blinker::Blinker(uint16_t period)
{
    _id = BlinkerID;
    _state.pin = LED_BUILTIN;
    _state.mode = REPEAT_MODE;
    _state.running = false;
    _state.led = 0;
    _period = period;
}


Blinker::Blinker(uint16_t period, bool oneShot)
{
    _id = BlinkerID;
    _state.pin = LED_BUILTIN;
    _state.mode = oneShot ? ONE_SHOT_MODE : REPEAT_MODE;
    _state.running = false;
    _state.led = 0;
    _period = period;
}


//******************************************************************************
// Public interface
//******************************************************************************

void Blinker::Start(uint16_t period)
{
    if (_state.running) return; // Ignore if already running

    _state.running = true;
    _startTime = millis();
    On();   // Aways start with LED on

    if (period > 0) _period = period;
    
    Debug.Log(this) << __func__ << F(": LED=") << _state.led << endl;
}


void Blinker::Start(uint16_t period, bool oneShot)
{
    if (_state.running) return; // Ignore if already running

    _state.mode = oneShot ? ONE_SHOT_MODE : REPEAT_MODE;
    Start(period);
}


void Blinker::Stop()
{
    _state.running = false;
    Off();  // Ensure the LED off
}


void Blinker::OneShot(uint16_t period)
{
    Start(period, true);
}


void Blinker::Poll()
{
    Debug.Log(this) << __func__; 

    if (!_state.running || _period == 0) return;

    uint32_t now = millis();

    if ((now - _startTime) >= _period)
    {
        Toggle();
        Debug << " LED=" << _state.led;

        if (_state.mode == REPEAT_MODE) _startTime = now; else Stop();
    }

    Debug << endl;
}


//******************************************************************************
// Internal methods
//******************************************************************************

inline void Blinker::On()
{
    _state.led = HIGH;
    pinMode(_state.pin, OUTPUT);
    digitalWrite(_state.pin, HIGH);
}


inline void Blinker::Off()
{
    _state.led = LOW;
    pinMode(_state.pin, OUTPUT);
    digitalWrite(_state.pin, _state.led);
}


inline void Blinker::Toggle()
{
    if (_state.led) Off(); else On();
}
