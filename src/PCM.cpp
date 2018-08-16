/**
 * @file PCM.cpp
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

#include <PCM.h>

uint_farptr_t PCM::voice_a = 0;
bool PCM::status_a = false;
uint16_t PCM::index_a = 0;
uint16_t PCM::size_a = 0;

uint_farptr_t PCM::voice_b = 0;
bool PCM::status_b = false;
uint16_t PCM::index_b = 0;
uint16_t PCM::size_b = 0;

uint_farptr_t PCM::voice_c = 0;
bool PCM::status_c = false;
uint16_t PCM::index_c = 0;
uint16_t PCM::size_c = 0;

uint_farptr_t PCM::voice_d = 0;
bool PCM::status_d = false;
uint16_t PCM::index_d = 0;
uint16_t PCM::size_d = 0;

#include <serial.h>

// Sample timer
ISR(TIMER1_COMPA_vect)
{
    // This will be shifted 2 bits to right, to the value of 32768 that is the
    // center of the wave.
    uint32_t out = 131072;

    // Sums up to 4 voices of 16-bit (index*2 bytes);
    if(PCM::status_a && PCM::index_a < PCM::size_a){
        out += (int16_t)pgm_read_word_far(PCM::voice_a + (uint32_t)(PCM::index_a)*2);
        PCM::index_a++;
    }

    if(PCM::status_b && PCM::index_b < PCM::size_b){
        out += (int16_t)pgm_read_word_far(PCM::voice_b + (uint32_t)(PCM::index_b)*2);
        PCM::index_b++;
    }

    if(PCM::status_c && PCM::index_c < PCM::size_c){
        out += (int16_t)pgm_read_word_far(PCM::voice_c + (uint32_t)(PCM::index_c)*2);
        PCM::index_c++;
    }
        
    if(PCM::status_d && PCM::index_d < PCM::size_d){
        out += (int16_t)pgm_read_word_far(PCM::voice_d + (uint32_t)(PCM::index_d)*2);
        PCM::index_d++;
    }

    // I can add the voice_e and certify in usage that 5 voices will never be summed

    // Shift 2 bits to right to maintain up to 16 bit resolution.
    out >>= 2;

    OCR0A = (uint8_t)(out >> 8); // MSB
    OCR0B = (uint8_t)out;        // LSB
}

void PCM::init(uint16_t sample_freq)
{
    // Sampling timer @TMR1
    OCR1A = F_CPU/sample_freq;
    TCCR1B |= (1 << WGM12) | (1 << CS10); // No prescaler CTC    
    TCNT1 = 0;
    TIMSK1 |= (1 << OCIE1A);

    // PWM DAC out @TMR0
#ifdef __AVR_ATmega328P__
    DDRD |= (1 << PD5) | (1 << PD6);
#elif __AVR_ATmega2560__
    DDRB |= (1 << PB7);
    DDRG |= (1 << PG5);
#endif
    
    // Fast PWM non-inverting OC0A and OC0B
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << CS00); // No prescaling
}

void PCM::set_voice(Voice voice, uint_farptr_t buffer, uint16_t size)
{
    switch(voice){
    case Voice::A:
        voice_a = buffer;
        size_a = size;
        index_a = 0;
        break;
    case Voice::B:
        voice_b = buffer;
        size_b = size;
        index_b = 0;
        break;
    case Voice::C:
        voice_c = buffer;
        size_c = size;
        index_c = 0;
        break;
    case Voice::D:
        voice_d = buffer;
        size_d = size;
        index_d = 0;
        break;
    }
}

void PCM::set_active(Voice voice, bool active)
{
    switch(voice){
    case Voice::A:
        ATOMIC_BLOCK(ATOMIC_FORCEON){
            status_a = active;
            index_a = 0;
        }
        break;
    case Voice::B:
        ATOMIC_BLOCK(ATOMIC_FORCEON){
            status_b = active;
            index_b = 0;
        }
        break;
    case Voice::C:
        ATOMIC_BLOCK(ATOMIC_FORCEON){
            status_c = active;
            index_c = 0;
        }
        break;
    case Voice::D:
        ATOMIC_BLOCK(ATOMIC_FORCEON){
            status_d = active;
            index_d = 0;
        }
        break;
    } 
}

