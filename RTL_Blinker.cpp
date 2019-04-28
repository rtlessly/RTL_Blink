/*******************************************************************************
 * Implementation file for LED Blinker
 *
 * created 09 Jul 2016
 * by R. Terry Lessly
 *******************************************************************************/
#define DEBUG 0
 
#include <Arduino.h>
#include <Streaming.h>
#include <RTL_Stdlib.h>
#include "RTL_Blinker.h"


DEFINE_CLASSNAME(Blinker);


//******************************************************************************
// Constructor
//******************************************************************************
Blinker::Blinker(uint8_t ledPin, uint16_t period, uint16_t dutyCycle)
{
    _state.pin = ledPin;
    _state.mode = REPEAT_MODE;
    _state.running = false;
    _state.led = 0;
    
    pinMode(_state.pin, OUTPUT);
    SetBlinkRate(period, dutyCycle);
}


//******************************************************************************
// Public interface
//******************************************************************************

void Blinker::SetBlinkRate(uint16_t period, uint16_t dutyCycle)
{
    dutyCycle = constrain(dutyCycle, 0, 100);
    _onTime = period * dutyCycle / 100;
    _offTime = period - _onTime;
}


void Blinker::Start()
{
    if (_state.running) return; // Ignore if already running

    _state.running = true;
    _startTime = millis();
    On();   // Aways start with LED on

    TRACE(Logger(_classname_, F("Start")) << F(": LED=") << _state.led << F(": Mode=") << _state.mode << endl);
}


void Blinker::Start(uint16_t period, uint16_t dutyCycle)
{
    _state.running = false;      // Turn off first to allow restarting in new mode
    _state.mode = REPEAT_MODE;
    SetBlinkRate(period, dutyCycle);
    Start();
}


void Blinker::OneShot(uint16_t period)
{
    if (period > 0) _onTime = period;

    _state.running = false;      // Turn off first to allow restarting in new mode
    _state.mode = ONE_SHOT_MODE;
    Start();
}


void Blinker::Poll()
{
    TRACE(Logger(_classname_, F("Poll")) << endl);

    if (!_state.running) return;

    auto toggleTime = _state.led ? _onTime : _offTime;
    auto now = millis();

    if ((now - _startTime) >= toggleTime)
    {
        Toggle();
        _startTime = now
        TRACE(Logger(_classname_, F("Poll")) << F(": LED=") << _state.led << endl);

        if (_state.mode == ONE_SHOT_MODE) Stop();
    }
}
