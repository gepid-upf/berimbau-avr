/**
 * @file Led.h
 * 
 * @author
 * Angelo Elias Dalzotto (150633@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright
 * Copyright (c) 2018 Angelo Elias Dalzotto & Gabriel Boni Vicari
 * 
 * @brief This is a class to manage leds.
 */

#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include <avr/io.h>
#include <Millis.h>

/**
 * Button class
 */
class Led {
public:

    /**
     * @brief Led class constructor.
     * 
     * @details The parameters are the addresses of the registers that controls
     * the ports where the leds are connected to.
     * 
     * @param *_ddr The DDR_ port of the LED.
     * @param *_port The PORT_ of the LED.
     * @param _pin_num The PIN_X/DDR_X/PORT_X button number.
     */
    Led(volatile uint8_t *_ddr, volatile uint8_t *_port, int _pin_num);

    /**
     * @brief Lights up the led and turns off after the time delay has passed.
     * 
     * @details Requires the update() method to be called.
     * 
     * @param _delay The time to wait to turn off the led.
     */
    void blink(uint32_t _delay);
    
    /**
     * @brief Updates the state of the led based on time.
     */
    void update();


private:
    volatile uint8_t *port;
    int pin_num;
    bool active;
    
    uint32_t start_time;
    uint32_t delay;

};

#endif /* _LED_H_ */