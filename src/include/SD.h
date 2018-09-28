#ifndef _SD_H_
#define _SD_H_

#include <stdint.h>
#include <avr/io.h>
#include <File.h>

class SD {
public:
    static bool init();
    static bool open_record(char* fname);
    static size_t record(uint32_t timestamp, uint8_t data);
    static bool stop_record();

    static bool open_play(char *fname);
    static bool read_beat(uint32_t *timestamp, uint8_t *instrument);
    static bool close_play();

    static File rec;

private:
    static bool mounted;

    static SDCard disk;
    static FAT fs;
    static File root;
    static File file;

    static uint32_t start_time;
    static bool first;
};

#endif /* _SD_H_ */
