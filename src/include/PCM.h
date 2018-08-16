/**
 * @file PCM.h
 * 
 * @author
 * Angelo Elias Dalzotto (150633@upf.br)
 * GEPID - Grupo de Pesquisa em Cultura Digital (http://gepid.upf.br/)
 * Universidade de Passo Fundo (http://www.upf.br/)
 * 
 * @copyright
 * Copyright (c) 2018 Angelo Elias Dalzotto & Gabriel Boni Vicari
 * 
 * @brief This is a class responsible for audio output in AVR microcontrollers.
 * It uses 2 8-bit PWM outputs to achieve maximum frequency and 16-bit
 * resolution. The output must be filtered with a low-pass fiter with cut-off
 * frequency of 20KHz.
 */

#ifndef _AVR_PCM_H_
#define _AVR_PCM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>

/**
 * TIMER1 interrupt service.
 * It is the sampling frequency timer.
 */
extern "C" void TIMER1_COMPA_vect(void) __attribute__((signal));

/**
 * PCM class
 */
class PCM {
public:

    /**
     * Enum Voice
     * Manages the voice to be bind to a buffer
     */
    enum class Voice {
        A,
        B,
        C,
        D
    };

    /**
     * @brief Timer initializer.
     * 
     * @details This class only use static methods and attributes.
     * This is the "static constructor".
     * 
     * @param sample_freq The frequency of the WAV sampling rate.
     */
    static void init(uint16_t sample_freq);

    /**
     * @brief Binds a buffer to a voice to be mixed.
     * 
     * @param voice Is the Voice enum to be binded.
     * @param buffer Is the address of the array of the samples. This must be
     * the return of the pgm_get_far_address(array) function to AVR
     * microcontrollers with more than 64KB of flash memory.
     * @param size Is the size of the sample array.
     */
    static void set_voice(Voice voice, uint_farptr_t buffer, uint16_t size);

    /**
     * @brief Sets a voice to be output.
     * 
     * @param voice Is the Voice wich will be active.
     * @param active Is the state of the voice. Default to active.
     */
    static void set_active(Voice voice, bool active = true);

    /**
     * @brief Timer ISR declaration.
     * 
     * @details This is done to include the ISR in the class.
     * This is not to be called.
     */
    friend void TIMER1_COMPA_vect(void);

private:
    static uint_farptr_t voice_a; //This is simply a long pointer
    static uint16_t index_a;
    static uint16_t size_a;
    static bool status_a;

    static uint_farptr_t voice_b; //This is simply a long pointer
    static uint16_t index_b;
    static uint16_t size_b;
    static bool status_b;

    static uint_farptr_t voice_c; //This is simply a long pointer
    static uint16_t index_c;
    static uint16_t size_c;
    static bool status_c;

    static uint_farptr_t voice_d; //This is simply a long pointer
    static uint16_t index_d;
    static uint16_t size_d;
    static bool status_d;
};

#endif /* _AVR_PCM_H_ */