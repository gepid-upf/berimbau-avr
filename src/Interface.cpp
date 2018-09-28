/**
 * @file Interface.cpp
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

#include <Interface.h>
#include <lcdpcf8574.h>
#include <SD.h>
#include <Game.h>

Interface::State Interface::state = Interface::State::MAIN;

Button Interface::up(&DDRF, &PORTF, &PINF, PINF4);
Button Interface::down(&DDRF, &PORTF, &PINF, PINF5);
Button Interface::select(&DDRF, &PORTF, &PINF, PINF6);
Button Interface::back(&DDRF, &PORTF, &PINF, PINF7);

uint8_t Interface::name_index = 0;
char Interface::name_buffer[21];
char Interface::char_sel = 0x40;

uint8_t Interface::line = 0;
uint8_t Interface::nfiles = 0;
Interface::Roll Interface::rolling = Roll::UP;

uint8_t Interface::tries = 0;
uint8_t Interface::fails = 0;
bool Interface::again = false;
bool Interface::success = false;


void Interface::init()
{
    lcd_init(LCD_DISP_ON_BLINK);
    lcd_home();
    lcd_led(0); //set led
    lcd_puts("Inicializando...");
}

void Interface::show_main()
{
    lcd_clrscr();
    lcd_puts("1. Modo gravar");
    lcd_gotoxy(0, 1);
    lcd_puts("2. Modo desafio");
    lcd_gotoxy(0, 3);
    lcd_puts("  ^    v    X    O  ");
    state = State::MAIN;
    line=0;
    lcd_command(LCD_DISP_ON_BLINK);
}

bool Interface::update()
{
    up.update();
    down.update();
    select.update();
    back.update();

    switch(state){
    case State::MAIN:
        if(select.get_state() == Button::State::PRESSED){
            if(!line)
                name_input();
            else
                challenge();
        }
        if(up.get_state() == Button::State::PRESSED){
            if(line) line--;
            lcd_gotoxy(0, line);
        }
        if(down.get_state() == Button::State::PRESSED){
            if(!line) line++;
            lcd_gotoxy(0, line);
        }
    default:
        break;
    case State::NAME_INPUT:
        if(up.get_state() == Button::State::PRESSED){
            char_sel++;
            if(char_sel == 0x5B) char_sel = 0x40;

            if(char_sel == 0x40)
                lcd_putc(' ');
            else
                lcd_putc(char_sel);

            lcd_gotoxy(name_index, 1);
        
            break;
        }
        if(down.get_state() == Button::State::PRESSED){
            if(name_index != 19 && char_sel != 0x40){
                name_buffer[name_index++] = char_sel;
                char_sel = 0x40;
            } 
            lcd_gotoxy(name_index, 1);
        
            break;
        }
        if(select.get_state() == Button::State::PRESSED){
            if(char_sel != 0x40){
                name_buffer[name_index++] = char_sel;
                name_buffer[name_index] = 0;
                show_recording();
                return true;
            }
            break;
        }
        if(back.get_state() == Button::State::PRESSED){
            if(!name_index){
                if(char_sel != 0x40){
                    lcd_putc(' ');
                    char_sel = 0x40;
                    lcd_gotoxy(name_index, 1);
                } else {
                    show_main();
                    return true;
                }
            } else {
                lcd_putc(' ');
                if(name_index > 0) name_index--;
                lcd_gotoxy(name_index, 1);
                char_sel = name_buffer[name_index];
            }
            break;
        }
        break;
    case State::RECORDING:
        if(back.get_state() == Button::State::PRESSED){
            SD::stop_record();
            show_main();
        }
        break;
    case State::CHALLENGE_INPUT:
        if(up.get_state() == Button::State::PRESSED){
            if(line){
                line--;
                switch(rolling){
                case Roll::UP:
                    char tmpbuf[12];
                    char printbuf[23];
                    SD::rec.ls(tmpbuf, File::LS_FILE, line);
                    remove_dot(tmpbuf);
                    lcd_gotoxy(0, 1);
                    sprintf(printbuf, "%u. %-17s", line+1, tmpbuf);
                    lcd_puts(printbuf);
                    SD::rec.ls(tmpbuf, File::LS_FILE, line+1);
                    remove_dot(tmpbuf);
                    lcd_gotoxy(0, 2);
                    sprintf(printbuf, "%u. %-17s", line+2, tmpbuf);
                    lcd_puts(printbuf);
                    lcd_gotoxy(0, 1);
                    break;                    
                case Roll::DOWN:
                    rolling = Roll::UP;
                    lcd_gotoxy(0, 1);
                    break;
                }
            }
        }
        if(down.get_state() == Button::State::PRESSED){
            if(line < nfiles-1){
                line++;
                switch(rolling){
                case Roll::DOWN:
                    char tmpbuf[12];
                    char printbuf[23];
                    SD::rec.ls(tmpbuf, File::LS_FILE, line);
                    remove_dot(tmpbuf);
                    lcd_gotoxy(0, 2);
                    sprintf(printbuf, "%u. %-17s", line+1, tmpbuf);
                    lcd_puts(printbuf);
                    SD::rec.ls(tmpbuf, File::LS_FILE, line-1);
                    remove_dot(tmpbuf);
                    lcd_gotoxy(0, 1);
                    sprintf(printbuf, "%u. %-17s", line, tmpbuf);
                    lcd_puts(printbuf);
                    lcd_gotoxy(0, 2);
                    break;
                case Roll::UP:
                    rolling = Roll::DOWN;
                    lcd_gotoxy(0, 2);
                    break;
                }
            }
        }
        if(select.get_state() == Button::State::PRESSED){
            SD::rec.ls(name_buffer, File::LS_FILE, line);
            state = State::PLAYING;
            tries = 0;
            fails = 0;
            while(!play_song());
            while(!try_play());
            if(success){
                lcd_clrscr();
                lcd_puts("Voce venceu!");
                char buffer[16];
                lcd_gotoxy(0, 1);
                sprintf(buffer, "Tentativas: %u", fails);
                lcd_puts(buffer);
                lcd_gotoxy(0, 2);
                sprintf(buffer, "Falhas: %u", fails);
                lcd_puts(buffer);
                lcd_gotoxy(0, 3);
                lcd_puts("  CONTINUAR      X  ");
                do {
                    back.update();
                } while(back.get_state() != Button::State::PRESSED);
                show_main();
            } else {
                show_main();
            }
            return true;
        }
        if(back.get_state() == Button::State::PRESSED){
            show_main();
            return true;
        }
        break;
    case State::PLAYING:
        if(select.get_state() == Button::State::PRESSED){
            again = true;
            return true;
        }
        if(back.get_state() == Button::State::PRESSED){
            again = false;
            return true;
        }
        break;
    }
    return false;
}

void Interface::name_input()
{
    lcd_clrscr();
    lcd_puts("Insira o nome:");
    lcd_gotoxy(0, 3);
    lcd_puts("  ^    >    X    O  ");
    lcd_gotoxy(0, 1);
    lcd_command(LCD_DISP_ON_CURSOR);
    state = State::NAME_INPUT;
    name_index = 0;

    while(!update());
}

void Interface::challenge()
{
    char tmpbuf[12];
    char printbuf[15];
    lcd_clrscr();
    lcd_puts("Selecione o ritmo:");
    nfiles = 0;
    line = 0;
    SD::rec.rewind();
    while(SD::rec.ls(tmpbuf, File::LS_FILE)){
        remove_dot(tmpbuf);
        if(++nfiles <= 2){
            sprintf(printbuf, "%u. %s", nfiles, tmpbuf);
            lcd_gotoxy(0, nfiles);
            lcd_puts(printbuf);
        }
    }
    lcd_gotoxy(0, 3);
    lcd_puts("  ^    v    X    O  ");
    lcd_gotoxy(0, 1);
    state = State::CHALLENGE_INPUT;
    rolling = Roll::UP;

    while(!update());
}

void Interface::show_recording()
{
    SD::open_record(name_buffer);
    state = State::RECORDING;
    lcd_clrscr();
    lcd_command(LCD_DISP_ON);
    lcd_puts("Gravando...");
    lcd_gotoxy(0,3);
    lcd_puts("                 O  ");
}

Interface::State Interface::get_state()
{
    return state;
}

void Interface::remove_dot(char *filename)
{
    for(int i = 0; i < 9; i++){
        if(filename[i] == '.'){
            filename[i] = '\0';
            break;
        }
    }
}

bool Interface::play_song()
{
    tries++;
    lcd_command(LCD_DISP_ON);
    lcd_clrscr();
    lcd_puts("Escute atentamente");
    lcd_gotoxy(0, 1);
    char buffer[16];
    sprintf(buffer, "Tentativas: %u", tries);
    Game::play_beat(name_buffer);
    lcd_clrscr();
    lcd_puts("Ouvir novamente?");
    lcd_gotoxy(0, 1);
    sprintf(buffer, "Tentativas: %u", tries);
    lcd_puts(buffer);
    lcd_gotoxy(0, 3);
    lcd_puts("            X    O  ");
    while(!update());

    return !again;
}

bool Interface::try_play()
{
    success = false;
    lcd_clrscr();
    lcd_puts("Repita o ritmo");
    lcd_gotoxy(0, 1);
    char buffer[16];
    sprintf(buffer, "Tentativas: %u", tries);
    lcd_puts(buffer);
    while(!Game::repeat_beat(name_buffer))
    {
        fails++;
        lcd_clrscr();
        lcd_puts("Ritmo incorreto");
        lcd_gotoxy(0,1);
        lcd_puts("Tentar novamente?");
        char buffer[16];
        lcd_gotoxy(0, 2);
        sprintf(buffer, "Tentativas: %u", fails);
        lcd_puts(buffer);
        lcd_gotoxy(0, 3);
        lcd_puts("            X    O  ");

        while(!update());

        return !again;
    }
    success = true;
    return true;
}