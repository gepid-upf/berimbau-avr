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
        KICK,
        CRASH,
        SNARE,
        HIHAT
    };

    static void init(uint32_t p_kick, uint16_t kick_s, uint32_t p_crash,
                        uint16_t crash_s, uint32_t p_snare, uint16_t snare_s,
                        uint32_t p_hihat, uint16_t hihat_s);
    static uint8_t update();

    static void set_state(State state);
    static State get_state();

    static void open_file(char *file_name);
    static void stop_record();
    static void play_beat(char *fname);
    static bool repeat_beat(char *fname);

private:
    static Button kick;
    static Button crash;
    static Button snare;
    static Button hihat;

    static const uint32_t MAX_DELAY_BEAT = 250;
};

#endif /* _GAME_H_ */