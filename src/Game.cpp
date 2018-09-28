/**
 * @file Game.cpp
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

#include <Game.h>

#include <serial.h>
#include <util/delay.h>
#include <stdlib.h>

#define DEBUG 1

Button Game::kick(&DDRF, &PORTF, &PINF, PINF0);
Button Game::crash(&DDRF, &PORTF, &PINF, PINF1);
Button Game::snare(&DDRF, &PORTF, &PINF, PINF2);
Button Game::hihat(&DDRF, &PORTF, &PINF, PINF3);

void Game::init(uint32_t p_kick, uint16_t kick_s, uint32_t p_crash, uint16_t crash_s,
                uint32_t p_snare, uint16_t snare_s, uint32_t p_hihat, uint16_t hihat_s)
{
    SD::init();

    PCM::init(44100);
    PCM::set_voice(PCM::Voice::A, p_kick, kick_s);
    PCM::set_voice(PCM::Voice::B, p_crash, crash_s);
    PCM::set_voice(PCM::Voice::C, p_snare, snare_s);
    PCM::set_voice(PCM::Voice::D, p_hihat, hihat_s);
}

uint8_t Game::update()
{
    kick.update();
    crash.update();
    snare.update();
    hihat.update();

    if(kick.get_state() == Button::State::PRESSED){
        PCM::set_active(PCM::Voice::A);
        switch(Interface::get_state()){
        case Interface::State::RECORDING:
            SD::record(Millis::get(), (uint8_t)Instrument::KICK);
            break;
        case Interface::State::PLAYING:
            return (uint8_t)Instrument::KICK;
        default:
            break;
        }
    }
    if(crash.get_state() == Button::State::PRESSED){
        PCM::set_active(PCM::Voice::B);
        switch(Interface::get_state()){
        case Interface::State::RECORDING:
            SD::record(Millis::get(), (uint8_t)Instrument::CRASH);
            break;
        case Interface::State::PLAYING:
            return (uint8_t)Instrument::CRASH;
        default:
            break;
        }
    }
    if(snare.get_state() == Button::State::PRESSED){
        PCM::set_active(PCM::Voice::C);
        switch(Interface::get_state()){
        case Interface::State::RECORDING:
            SD::record(Millis::get(), (uint8_t)Instrument::SNARE);
            break;
        case Interface::State::PLAYING:
            return (uint8_t)Instrument::SNARE;
        default:
            break;
        }
    }
    if(hihat.get_state() == Button::State::PRESSED){
        PCM::set_active(PCM::Voice::D);
        switch(Interface::get_state()){
        case Interface::State::RECORDING:
            SD::record(Millis::get(), (uint8_t)Instrument::HIHAT);
            break;
        case Interface::State::PLAYING:
            return (uint8_t)Instrument::HIHAT;
        default:
            break;
        }
    }
    return -1;
}

void Game::play_beat(char *fname)
{
    uint32_t timems;
    uint8_t data;
    _delay_ms(500);
    SD::open_play(fname);

    uint32_t start = Millis::get();
    while(SD::read_beat(&timems, &data)){
        while(Millis::get() - start < timems);
        switch((Game::Instrument)data){
        case Instrument::KICK:
            PCM::set_active(PCM::Voice::A);
            break;
        case Instrument::CRASH:
            PCM::set_active(PCM::Voice::B);
            break;
        case Instrument::SNARE:
            PCM::set_active(PCM::Voice::C);
            break;
        case Instrument::HIHAT:
            PCM::set_active(PCM::Voice::D);
            break;
        }
    }

    SD::close_play();
}

bool Game::repeat_beat(char *fname)
{
    uint32_t timems, lastData, lastPlay;
    uint8_t data, played;
    SD::open_play(fname);

    SD::read_beat(&timems, &data);
    do {
        played = update();
    } while(played > 3);
    lastPlay = Millis::get();
    lastData = timems;

    if(played != data){
        SD::close_play();
        return false;
    }
    
    while(SD::read_beat(&timems, &data)){
        uint32_t now;
        do {
            played = update();
            now = Millis::get();
            if(now - lastPlay > timems - lastData + MAX_DELAY_BEAT){
                SD::close_play();
                return false;
            }
        } while(played > 3);

        if(now - lastPlay + MAX_DELAY_BEAT < timems - lastData){
            SD::close_play();
            return false;
        }
        
        if(played != data){
            SD::close_play();
            return false;
        }

        lastPlay = now;
        lastData = timems;
    
    } 

    return true;
}