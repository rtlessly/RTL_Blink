/*******************************************************************************
 * Header file for LED Blinker
 *
 * created 09 Jul 2016
 * by R. Terry Lessly
 *
 *******************************************************************************/
#ifndef _RTL_BLINKER_H_
#define _RTL_BLINKER_H_

#include <inttypes.h>
#include <IPollable.h>


class Blinker : IPollable
{
    public: static const uint8_t REPEAT_MODE   = 0;
    public: static const uint8_t ONE_SHOT_MODE = 1;

    //**************************************************************************
    // Constructors
    //**************************************************************************
    public: Blinker(uint16_t period);
    public: Blinker(uint16_t period, bool oneShot);
    public: Blinker(uint8_t ledPin=LED_BUILTIN, uint16_t period=500, bool oneShot=false);

    //**************************************************************************
    // Public methods
    //**************************************************************************
    public: void Start(uint16_t period=0);
    public: void Start(uint16_t period, bool oneShot);
    public: void Stop();
    public: void OneShot(uint16_t period=0);

    public: void Poll();

    //**************************************************************************
    // Internal implementation
    //**************************************************************************
    private: inline void On();
    private: inline void Off();
    private: inline void Toggle();

    //**************************************************************************
    // Internal state
    //**************************************************************************
    private: struct BlinkerState
    {
        uint8_t pin     : 5;  // The Arduino digital pin where the LED is connected
        uint8_t running : 1;  // Running state (0=available, 1=in use)
        uint8_t mode    : 1;  // Blinker mode (0=REPEAT_MODE, 1=ONE_SHOT_MODE);
        uint8_t led     : 1;  // the current state of the LED (1=on, 0=off)
    }
    _state;

    uint16_t _period;    // Blink period
    uint32_t _startTime; // Start time of last blink period
};

#endif

