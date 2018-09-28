#include <SD.h>
#include <stdio.h>

SDCard SD::disk(&PORTB, &DDRB, PB0);
FAT SD::fs(&disk);
File SD::root(&fs);
File SD::rec(&fs);
File SD::file(&fs);

bool SD::mounted = false;
uint32_t SD::start_time = 0;
bool SD::first = true;

bool SD::init()
{
    // Initialize disk and mount filesystem
	return  disk.init() && 
            fs.mount() && 
            root.open_root() && 
            rec.open(root, "record", File::O_READ);
}

bool SD::open_record(char* fname)
{
    first = true;
    char strbuf[25];
    sprintf(strbuf, "%s.dat", fname);

    return  file.open(rec, strbuf, File::O_CREAT | File::O_WRITE) &&
            file.rm() &&
            file.open(rec, strbuf, File::O_CREAT | File::O_WRITE);
}

size_t SD::record(uint32_t timestamp, uint8_t data)
{
    if(first){
        first = false;
        start_time = timestamp;
    }
    timestamp -= start_time;

    return file.write((uint8_t*)&timestamp, 4) && file.write(&data, 1);
}

bool SD::stop_record()
{
    return file.close();
}

bool SD::open_play(char *fname)
{
    return file.open(rec, fname, File::O_READ);
}

bool SD::read_beat(uint32_t *timestamp, uint8_t *instrument)
{
    if(file.available() >= 5)
        return file.read((uint8_t*)timestamp, 4) && file.read(instrument, 1);
    
    return false;
}

bool SD::close_play()
{
    return file.close();
}