/**
 * @file Game.h
 * 
 * @author
 * Angelo Elias Dalzotto (150633@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright
 * Copyright (c) 2018 Angelo Elias Dalzotto & Gabriel Boni Vicari
 * 
 * @brief This class controls the recording and the challenge mode for the
 * air drums.
 */

#ifndef _GAME_H_
#define _GAME_H_

#include <PCM.h>
#include <Button.h>
#include <Led.h>
#include <SD.h>
#include <Interface.h>

class Game {
public:
    enum class State {
        FREE,
        RECORD,
        PLAY
    };
    enum class Instrument {
        CAXIXI,
        MOEDA,
        PRESA,
        SOLTA
    };

    static void init(uint32_t p_caxixi, uint16_t caxixi_s, uint32_t p_moeda,
                        uint16_t moeda_s, uint32_t p_presa, uint16_t presa_s,
                        uint32_t p_solta, uint16_t solta_s);
    static uint8_t update();

    static void set_state(State state);
    static State get_state();

    static void open_file(char *file_name);
    static void stop_record();
    static void play_beat(char *fname);
    static bool repeat_beat(char *fname);
    static uint8_t get_in_time();

    static void update_leds();

private:
    static Button caxixi;
    static Button moeda;
    static Button presa;
    static Button solta;
    static Led l_caxixi;
    static Led l_moeda;
    static Led l_presa;
    static Led l_solta;

    static uint8_t in_time;

    static const uint32_t MAX_DELAY_BEAT = 250;
};

#endif /* _GAME_H_ */