/**
 * @file main.cpp
 * 
 * @author
 * Angelo Elias Dalzotto (150633@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright
 * Copyright (c) 2018 Angelo Elias Dalzotto & Gabriel Boni Vicari
 * 
 * @brief This file is the main application to the air-drums project.
 * It just polls the inputs, set the sounds and instanciates the objects.
 */

#include <serial.h> // Only for debug

#include <Button.h>
#include <PCM.h>
#include <avr/pgmspace.h>

#include <samples/hh_closed.h>
#include <samples/kick.h>
#include <samples/snare.h>
#include <samples/crash.h>

Button kick(&DDRF, &PORTF, &PINF, PINF0);
Button crash(&DDRF, &PORTF, &PINF, PINF1);
Button snare(&DDRF, &PORTF, &PINF, PINF2);
Button hihat(&DDRF, &PORTF, &PINF, PINF3);

void update_all()
{
    kick.update();
    crash.update();
    snare.update();
    hihat.update();
}

int main()
{   
    uart_init(); // Only for debug

    PCM::init(44100);
    PCM::set_voice(PCM::Voice::A, pgm_get_far_address(kick_data), kick_length);
    PCM::set_voice(PCM::Voice::B, pgm_get_far_address(crash_data_a), crash_length);
    PCM::set_voice(PCM::Voice::C, pgm_get_far_address(snare_data), snare_length);
    PCM::set_voice(PCM::Voice::D, pgm_get_far_address(hh_closed_data), hh_closed_length);

    while(true){
        update_all();
        if(kick.get_state() == Button::State::PRESSED){
            PCM::set_active(PCM::Voice::A);
        }
        if(crash.get_state() == Button::State::PRESSED){
            PCM::set_active(PCM::Voice::B);
        }
        if(snare.get_state() == Button::State::PRESSED){
            PCM::set_active(PCM::Voice::C);
        }
        if(hihat.get_state() == Button::State::PRESSED){
            PCM::set_active(PCM::Voice::D);
        }
    }

    return 0;
}