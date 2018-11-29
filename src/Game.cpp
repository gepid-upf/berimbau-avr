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

Button Game::caxixi(&DDRF, &PORTF, &PINF, PINF0);
Button Game::moeda(&DDRF, &PORTF, &PINF, PINF1);
Button Game::presa(&DDRF, &PORTF, &PINF, PINF2);
Button Game::solta(&DDRF, &PORTF, &PINF, PINF3);

uint8_t Game::in_time = 0;

void Game::init(uint32_t p_caxixi, uint16_t caxixi_s, uint32_t p_moeda,
                uint16_t moeda_s, uint32_t p_presa, uint16_t presa_s,
                uint32_t p_solta, uint16_t solta_s)
{
    SD::init();

    PCM::init(44100);
    PCM::set_voice(PCM::Voice::A, p_caxixi, caxixi_s);
    PCM::set_voice(PCM::Voice::B, p_moeda, moeda_s);
    PCM::set_voice(PCM::Voice::C, p_presa, presa_s);
    PCM::set_voice(PCM::Voice::D, p_solta, solta_s);
}

uint8_t Game::update()
{
    caxixi.update();
    moeda.update();
    presa.update();
    solta.update();

    if(caxixi.get_state() == Button::State::PRESSED){
        PCM::set_active(PCM::Voice::A);
        switch(Interface::get_state()){
        case Interface::State::RECORDING:
            SD::record(Millis::get(), (uint8_t)Instrument::CAXIXI);
            break;
        case Interface::State::PLAYING:
            return (uint8_t)Instrument::CAXIXI;
        default:
            break;
        }
    }
    if(moeda.get_state() == Button::State::PRESSED){
        PCM::set_active(PCM::Voice::B);
        switch(Interface::get_state()){
        case Interface::State::RECORDING:
            SD::record(Millis::get(), (uint8_t)Instrument::MOEDA);
            break;
        case Interface::State::PLAYING:
            return (uint8_t)Instrument::MOEDA;
        default:
            break;
        }
    }
    if(presa.get_state() == Button::State::PRESSED){
        PCM::set_active(PCM::Voice::C);
        switch(Interface::get_state()){
        case Interface::State::RECORDING:
            SD::record(Millis::get(), (uint8_t)Instrument::PRESA);
            break;
        case Interface::State::PLAYING:
            return (uint8_t)Instrument::PRESA;
        default:
            break;
        }
    }
    if(solta.get_state() == Button::State::PRESSED){
        PCM::set_active(PCM::Voice::D);
        switch(Interface::get_state()){
        case Interface::State::RECORDING:
            SD::record(Millis::get(), (uint8_t)Instrument::SOLTA);
            break;
        case Interface::State::PLAYING:
            return (uint8_t)Instrument::SOLTA;
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
        case Instrument::CAXIXI:
            PCM::set_active(PCM::Voice::A);
            break;
        case Instrument::MOEDA:
            PCM::set_active(PCM::Voice::B);
            break;
        case Instrument::PRESA:
            PCM::set_active(PCM::Voice::C);
            break;
        case Instrument::SOLTA:
            PCM::set_active(PCM::Voice::D);
            break;
        }
    }

    SD::close_play();
}

bool Game::repeat_beat(char *fname)
{
    uint32_t timems, lastData, lastPlay;
    uint32_t deltatotal = 0;
    uint16_t beattotal = 0;
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
            played = update(); // Read pressed
            now = Millis::get(); // Time now
            if(now - lastPlay > timems - lastData + MAX_DELAY_BEAT){ // DELTA > deltaRead
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

        deltatotal += abs((int32_t)now - (int32_t)lastPlay - ((int32_t)timems - (int32_t)lastData));
        beattotal++;

        lastPlay = now;
        lastData = timems;
    
    }

    in_time = deltatotal * -100.0/(beattotal*MAX_DELAY_BEAT) + 100;

    return true;
}

uint8_t Game::get_in_time()
{
    return in_time;
}