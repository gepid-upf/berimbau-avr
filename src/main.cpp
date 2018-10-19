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
#include <samples/caxixi.h>
#include <samples/moeda.h>
#include <samples/presa.h>
#include <samples/solta.h>

int main()
{   
    uart_init(); // Only for debug
    Interface::init();
    Game::init(pgm_get_far_address(caxixi_data), caxixi_length,
                pgm_get_far_address(moeda_data), moeda_length,
                pgm_get_far_address(presa_data_a), presa_length,
                pgm_get_far_address(solta_data_a), solta_length);
    

    Interface::show_main();

    while(true){
        Game::update();
        Interface::update();
    }

    return 0;
}