#include <Led.h>

Led::Led(volatile uint8_t *_ddr, volatile uint8_t *_port, int _pin_num) :
        port(_port), pin_num(_pin_num), active(false)
{
    *_ddr |= (1 << pin_num); // Output
    *port &= ~(1 << pin_num); // Off
    active = false;
    Millis::init();
}

void Led::blink(uint32_t _delay)
{
    delay = _delay;
    *port |= (1 << pin_num); // On
    start_time = Millis::get();
    active = true;
}

void Led::update()
{
    if(active && Millis::get() - start_time > delay){
        *port &= ~(1 << pin_num);
        active = false;
    }
}