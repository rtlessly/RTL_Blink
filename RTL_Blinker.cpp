/*******************************************************************************
 * Implementation file for LED Blinker
 *
 * created 09 Jul 2016
 * by R. Terry Lessly
 *******************************************************************************/
#define DEBUG 0
 
#include <Arduino.h>
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
    _state.led = 0;
    
    pinMode(_state.pin, OUTPUT);
    SetBlinkRate(period, dutyCycle);
}


void Blinker::StateChanging(TaskState newState)
{
    switch (newState)
    {
        case TaskState::Resuming:
            On();   // Aways start with LED on
            _timeout = millis() + _onTime;
            TRACE(Logger(_classname_, F("Resuming")) << F(": LED=") << _state.led << F(": Mode=") << _state.mode << endl);
            break;

        case Suspending:
            Off();  // Always turn the LED off
            break;

        default:
            break;
    }
}


void Blinker::Poll()
{
    TRACE(Logger(_classname_, F("Poll")) << endl);

    auto now = millis();

    if (now >= _timeout)
    {
        if (_state.mode == ONE_SHOT_MODE)
        {
            Suspend();
        }
        else
        {
            Toggle();
            _timeout = now + (_state.led ? _onTime : _offTime);
        }

        TRACE(Logger(_classname_, F("Poll")) << F(": LED=") << _state.led << endl);
    }
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


void Blinker::Start(uint16_t period, uint16_t dutyCycle)
{
    _state.mode = REPEAT_MODE;
    SetBlinkRate(period, dutyCycle);
    Start();
}


void Blinker::OneShot(uint16_t period)
{
    if (period > 0) _onTime = period;

    Stop();
    _state.mode = ONE_SHOT_MODE;
    Start();
}
