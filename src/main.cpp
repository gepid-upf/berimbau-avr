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

#include <Interface.h>
#include <Game.h>
#include <avr/pgmspace.h>

// Include here to ensure the order in memory to allow overflow
#include <samples/hh_closed.h>
#include <samples/kick.h>
#include <samples/snare.h>
#include <samples/crash.h>

int main()
{   
    uart_init(); // Only for debug
    Interface::init();
    Game::init(pgm_get_far_address(kick_data), kick_length,
                pgm_get_far_address(crash_data_a), crash_length,
                pgm_get_far_address(snare_data), snare_length,
                pgm_get_far_address(hh_closed_data), hh_closed_length);
    

    Interface::show_main();

    while(true){
        Game::update();
        Interface::update();
    }

    return 0;
}