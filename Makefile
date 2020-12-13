
GCC	=	/usr/bin/avr-gcc
GCC_COMP_FLAGS	=	-DF_CPU=16000000L -I./drivers -I./FreeRTOS/src -I./FreeRTOS/FreeRTOSTraceDriver -I/usr/avr/include
GCC_LINKER_FLAGS	=	 -L./libs -L./drivers -lIotLibrary -lfreertos
SRCS	=	$(wildcard *.c)
BINS	=	$(SRCS:%.c=bin/%.o)

PARTNO	=	atmega2560
PROGRAMMER	=	atmelice
PORT	=	usb
AVRDUDE_FLAGS	=	-e -p $(PARTNO) -c $(PROGRAMMER) -P $(PORT) -b115200 -D

MAIN	=	main.hex
ELF_TARGET	=	$(MAIN:%.hex=%.elf)

all: build

build: $(MAIN)

info:
	@echo GCC:	$(GCC)
	@echo GCC COMPILE FLAGS:	$(GCC_COMP_FLAGS)
	@echo GCC LINKER FLAGS:	$(GCC_LINKER_FLAGS)
	@echo AVRDUDE FLAGS:	$(AVRDUDE_FLAGS)
	@echo MAIN HEX:	$(MAIN)

bin/%.o: %.c
	mkdir -p bin
	$(GCC) -mmcu=$(PARTNO) $(GCC_COMP_FLAGS) -c $< -o $@

$(MAIN): $(BINS)
	$(GCC) -mmcu=$(PARTNO) -o $(ELF_TARGET) $(BINS) $(GCC_LINKER_FLAGS)
	avr-objcopy -O ihex $(ELF_TARGET) $(MAIN)
	rm $(ELF_TARGET)

flash: build
	/usr/bin/avrdude $(AVRDUDE_FLAGS) -U flash:w:$(MAIN):i

clean:
	rm -rf bin $(MAIN) $(ELF_TARGET)
