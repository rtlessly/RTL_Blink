/*******************************************************************************
 * Header file for LED Blinker
 *
 * created 09 Jul 2016
 * by R. Terry Lessly
 *******************************************************************************/
#ifndef _RTL_BLINKER_H_
#define _RTL_BLINKER_H_

#include <inttypes.h>
#include <RTL_StdLib.h>
#include <TaskBase.h>


///*****************************************************************************
/// <summary>
/// Blinks an LED attached to an Arduino digital pin.
/// </summary>
/// <remarks>
/// The blinker can be configured to be either a repeating or one-shot blinker. 
/// For a repeating blinker the blink rate is specifed as a total blink period 
/// (on-time + off-time) and a duty cycle, where the duty cycle specifies
/// the percentage of the total period (0-100%) that the LED is on.
/// 
/// This class is implemented as a Task (subclass of ATask), so it is intended to
/// work with TaskSchedular singleton. For a Blinker to work, the TaskSchedular::Dispatch()
/// function must be called very often, such as in every iteration of the loop() 
/// function.
/// </remarks>
///*****************************************************************************
class Blinker : public TaskBase
{
    DECLARE_CLASSNAME;

    public: static const uint8_t REPEAT_MODE   = 0;
    public: static const uint8_t ONE_SHOT_MODE = 1;

    //**************************************************************************
    // Constructors
    //**************************************************************************
    public: Blinker() : Blinker(LED_BUILTIN, 1000, 50) {};
    public: Blinker(uint16_t period) : Blinker(LED_BUILTIN, period, 50) {};
    public: Blinker(uint16_t period, uint16_t dutyCycle) : Blinker(LED_BUILTIN, period, dutyCycle) {};
    public: Blinker(uint8_t ledPin, uint16_t period, uint16_t dutyCycle);

    //**************************************************************************
    // ATask interface
    //**************************************************************************
    public: void Poll() override;
    public: void StateChanging(TaskState newState) override;

    //**************************************************************************
    // Public methods
    //**************************************************************************
    public: void SetBlinkRate(uint16_t period, uint16_t dutyCycle=50);
    public: void Start(uint16_t period, uint16_t dutyCycle = 50);
    public: void Start();
    public: void Stop();
    public: void OneShot(uint16_t period=0);
    public: void On();
    public: void Off();
    public: void Toggle();

    //**************************************************************************
    // Public properties
    //**************************************************************************
    public: uint8_t GetMode() { return _state.mode; };
    public: void SetMode(uint8_t mode) { _state.mode = (mode ? ONE_SHOT_MODE : REPEAT_MODE); };

    //**************************************************************************
    // Internal state
    //**************************************************************************
    private: struct BlinkerState
    {
        uint8_t pin     : 5;  // The Arduino pin where the LED is connected
        uint8_t mode    : 1;  // Blinker mode (0=REPEAT_MODE, 1=ONE_SHOT_MODE);
        uint8_t led     : 1;  // the current state of the LED (1=on, 0=off)
    }
    _state;

    private: uint16_t _onTime;    // LED on time
    private: uint16_t _offTime;   // LED off time
    private: uint32_t _timeout;   // timeout for current blink period
};

//******************************************************************************
// Inline methods
//******************************************************************************

inline void Blinker::On()
{
    digitalWrite(_state.pin, _state.led = HIGH);
}


inline void Blinker::Off()
{
    digitalWrite(_state.pin, _state.led = LOW);
}


inline void Blinker::Toggle()
{
    if (_state.led) Off(); else On();
}


inline void Blinker::Start()
{
    Resume();
}


inline void Blinker::Stop()
{
    Suspend();
}

#endif
