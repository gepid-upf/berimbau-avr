/**
 * @file Button.h
 * 
 * @author
 * Angelo Elias Dalzotto (150633@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright
 * Copyright (c) 2018 Angelo Elias Dalzotto & Gabriel Boni Vicari
 * 
 * @brief This is a class to manage buttons with a state machine. It supports
 * reading a single click and waiting a unclick without blocking the code.
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>
#include <avr/io.h>
#include <Millis.h>

/**
 * Change this to adjust the debouncing.
 * More debouncing means slower pressings.
 * Less debouncing can include clicks that are actually noise.
 */
#define DEBOUNCE_TIME_MILLISECONDS 25

/**
 * Button class
 */
class Button {
public:

    /**
     * @brief Button class constructor.
     * 
     * @details It sets the buttons with internal pull-ups. The parameters
     * are the address of the registers that controls the ports the buttons are
     * connected to.
     * 
     * @param *ddr The DDR_ port of the button.
     * @param *port The PORT_ of the button.
     * @param *pin The PIN_ of the button.
     * @param pin_num The PIN_X/DDR_X/PORT_X button number.
     */
    Button(volatile uint8_t *ddr, volatile uint8_t *port, volatile uint8_t *pin, int pin_num);
    
    /**
     * Enum State
     * Controls the state of the button.
     */
    enum class State {
        UNPRESSED,
        PRESSED,
        UNRELEASED
    };

    /**
     * @brief Button state getter.
     * 
     * @returns The current State of the button.
     */
    State get_state();
    
    /**
     * @brief Updates the state of the button based on the digital reading.
     */
    void update();

private:
    static const uint32_t DEBOUNCE_MS = DEBOUNCE_TIME_MILLISECONDS;
    volatile uint8_t *pin;
    int pin_num;
    State pin_state;
    uint32_t last_press;
    uint32_t last_unpress;    
};

#endif /* _BUTTON_H_ */