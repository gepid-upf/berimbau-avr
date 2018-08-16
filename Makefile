BIN    = air_drums
MCU    = atmega2560
F_CPU  = 16000000
BAUD   = 115200

PORT = /dev/ttyACM0
SPEED = 115200
PROGRAMMER = wiring

CC      = avr-g++
OBJCOPY = avr-objcopy
RM      = rm
MK      = mkdir

SRCDIR   = src
BUILDDIR = build
BINDIR   = bin
INCDIR   = $(SRCDIR)/include
LIBDIR = lib

TARGET = $(BINDIR)/$(BIN)

SRCEXT   = c
CPP_SOURCES  = $(wildcard $(SRCDIR)/*.cpp)
C_SOURCES = $(wildcard $(SRCDIR)/*.c) $(wildcard $(LIBDIR)/*.c)
CPP_OBJECTS  = $(addprefix $(BUILDDIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
C_OBJECTS = $(addprefix $(BUILDDIR)/,$(notdir $(C_SOURCES:.c=.o)))
CFLAGS   = -g -Wall -mmcu=$(MCU) -Os -Wl,-section-start=.crash1=0x10000
INCLUDES = -I$(INCDIR) -I$(LIBDIR)
LDFLAGS  = -g -mmcu=$(MCU) -Wl,-section-start=.crash1=0xF0 -Wl,-section-start=.crash2=0x80EF
DEFINES  = -DF_CPU=$(F_CPU) -DBAUD=$(BAUD)


all: $(TARGET).hex size

size:
	@if [ -f $(TARGET).elf ]; then avr-size -C --mcu=$(MCU) $(TARGET).elf; fi

clean:
	@echo "Cleaning..."
	@$(RM) -rf $(BUILDDIR) $(BINDIR)

$(TARGET).hex: $(TARGET).elf
	@echo "Generating hex..."
	@$(RM) -f $(TARGET).hex
	@$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(TARGET).elf: $(CPP_OBJECTS) $(C_OBJECTS)
	@echo "Linking..."
	@$(MK) -p $(BINDIR)
	@$(CC) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<"
	@$(MK) -p $(BUILDDIR)
	@$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS) $(DEFINES)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling $<"
	@$(MK) -p $(BUILDDIR)
	@$(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS) $(DEFINES)

flash:
	@avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(SPEED) -U flash:w:$(TARGET).hex

monitor:
	@screen $(PORT) $(BAUD)