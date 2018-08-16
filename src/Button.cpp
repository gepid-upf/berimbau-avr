/**
 * @file Button.cpp
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

#include <Button.h>

Button::Button(volatile uint8_t *ddr, volatile uint8_t *port, volatile uint8_t *pin, int pin_num) :
                                                                pin(pin),
                                                                pin_num(pin_num)
{
    *ddr &= ~(1 << pin_num); // Input
    *port |= (1 << pin_num); // Pull-up
    MCUCR &= ~(1 << PUD); // Global pull-up enable
    pin_state = State::UNPRESSED;
    Millis::init();
    last_press = 0;
    last_unpress = 0;
}

void Button::update()
{
    switch(pin_state){
    case State::UNPRESSED:
        if(!(*pin & (1 << pin_num)) && Millis::get() - last_unpress > DEBOUNCE_MS){
            pin_state = State::PRESSED;
            last_press = Millis::get();
        }        
        break;
    case State::PRESSED:
    case State::UNRELEASED:
        if((*pin & (1 << pin_num)) && Millis::get() - last_press > DEBOUNCE_MS){
            pin_state = State::UNPRESSED;
            last_unpress = Millis::get();
        }
        break;
    }
}

Button::State Button::get_state()
{
    switch(pin_state){
    case State::PRESSED:
        pin_state = State::UNRELEASED;
        return State::PRESSED;
    default:
        return pin_state;
    }
}