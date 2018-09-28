/**
 * @file Interface.h
 * 
 * @author
 * Angelo Elias Dalzotto (150633@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright
 * Copyright (c) 2018 Angelo Elias Dalzotto & Gabriel Boni Vicari
 * 
 * @brief This class is the interface of the program. It is responsible to
 * print the layout for the 20x4 LCD.
 */

#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <Button.h>

class Interface {
public:
    enum class Click {
        UP,
        DOWN,
        BACK,
        SELECT
    };
    enum class State {
        MAIN,
        NAME_INPUT,
        RECORDING,
        CHALLENGE_INPUT,
        PLAYING
    };
    enum class Roll {
        UP,
        DOWN
    };

    static void init();
    static void show_main();
    static bool update();
    static State get_state();    

private:
    static State state;

    static Button up;
    static Button down;
    static Button select;
    static Button back;

    static uint8_t name_index;
    static char name_buffer[21];
    static char char_sel;

    static uint8_t line;
    static uint8_t nfiles;
    static Roll rolling;
    
    static uint8_t tries;
    static bool again;
    static uint8_t fails;
    static bool success;

    static void name_input();
    static void challenge();
    static void show_recording();
    static void remove_dot(char *filename);
    static bool play_song();
    static bool try_play();
};


#endif /* _INTERFACE_H_ */
