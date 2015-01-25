#######################################################################################

# environment variable of the current user to locate the AVR8 toolchain
AVRPATH = $(AVR8TOOLCHAINBINDIR)

# the type of avr microcontroller
DEVICE = atmega8

# the frequency the microcontroller is clocked with
F_CPU = 16000000

#tinyUSBboard Rev3 and newer
DEFINES += -DTINYUSBBOARD_REV3
DEFINES += -DVUSB_CFG_IOPORTNAME=D -DVUSB_CFG_DMINUS_BIT=7 -DVUSB_CFG_DPLUS_BIT=2
DEFINES += -DVUSB_CFG_HASNO_PULLUP_IOPORTNAME -DVUSB_CFG_HASNO_PULLUP_BIT
DEFINES += -DBUTTON_PROG=D,6

# extra defines
# DEFINES += -DSOMETHING

# where the firmware should be located within the flashmemory (in case you trampoline)
FLASHADDRESS = 0x0000

# (not important for compiling) - the device transporting firmware into the controller
PROGRAMMER = -c usbasp

#######################################################################################



# Tools:
ECHO=@echo
GCC=gcc
MAKE=@make
RM=@rm -f

CC=$(AVRPATH)avr-gcc
OBC=@$(AVRPATH)avr-objcopy
OBD=@$(AVRPATH)avr-objdump
SIZ=@$(AVRPATH)avr-size

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)


MYCFLAGS = -Wall -Os -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions -ffunction-sections -fdata-sections -I. -I../libs/API/ -I../libs/v-usb/usbdrv -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) $(CFLAGS)   $(DEFINES)
MYLDFLAGS = -Wl,--relax,--gc-sections $(LDFLAGS)


FLASHPREAMBLEDEFINE = 
ifneq ($(FLASHADDRESS), 0)
ifneq ($(FLASHADDRESS), 00)
ifneq ($(FLASHADDRESS), 000)
ifneq ($(FLASHADDRESS), 0000)
ifneq ($(FLASHADDRESS), 00000)
ifneq ($(FLASHADDRESS), 0x0)
ifneq ($(FLASHADDRESS), 0x00)
ifneq ($(FLASHADDRESS), 0x000)
ifneq ($(FLASHADDRESS), 0x0000)
ifneq ($(FLASHADDRESS), 0x00000)
FLASHPREAMBLE = 0x0000
FLASHPREAMBLEDEFINE = -DFLASHPREAMBLE=$(FLASHPREAMBLE)
MYLDFLAGS += -Wl,--section-start=.text=$(FLASHADDRESS)
endif
endif
endif
endif
endif
endif
endif
endif
endif
endif




STDDEP	 = *.h ../libs/API/*.h ../libs/v-usb/usbdrv/*.h 
EXTRADEP = Makefile


all:  main.hex main.raw main.asm

usbdrvasm.o: ../libs/v-usb/usbdrv/usbdrvasm.S $(STDDEP) $(EXTRADEP)
	$(CC) -x assembler-with-cpp -c ../libs/v-usb/usbdrv/usbdrvasm.S -o usbdrvasm.o $(MYCFLAGS)

oddebug.o: ../libs/v-usb/usbdrv/oddebug.c $(STDDEP) $(EXTRADEP)
	$(CC) ../libs/v-usb/usbdrv/oddebug.c -c -o oddebug.o $(MYCFLAGS)

usbdrv.o: ../libs/v-usb/usbdrv/usbdrv.c $(STDDEP) $(EXTRADEP)
	$(CC) ../libs/v-usb/usbdrv/usbdrv.c -c -o usbdrv.o $(MYCFLAGS)

apipage.o: ../libs/API/apipage.c $(STDDEP) $(EXTRADEP)
	$(CC) ../libs/API/apipage.c -c -o apipage.o $(MYCFLAGS)

main.o: platform.h platform.c main.c $(STDDEP) $(EXTRADEP)
	$(CC) main.c -c -o main.o $(MYCFLAGS)

main.elf: main.o apipage.o usbdrv.o oddebug.o usbdrvasm.o $(STDDEP) $(EXTRADEP)
	$(CC) main.o apipage.o usbdrv.o oddebug.o usbdrvasm.o -o main.elf $(MYCFLAGS) -Wl,-Map,main.map $(MYLDFLAGS)
	$(ECHO) "."
	$(SIZ) main.elf
	$(ECHO) "."

main.hex: main.elf $(STDDEP) $(EXTRADEP)
	$(OBC) -R .eeprom -R .fuse -R .lock -R .signature -O ihex main.elf main.hex

main.raw: main.elf $(STDDEP) $(EXTRADEP)
	$(OBC) -R .eeprom -R .fuse -R .lock -R .signature -O binary main.elf main.raw

main.asm: main.elf $(STDDEP) $(EXTRADEP)
	$(OBD) -d main.elf > main.asm

disasm: main.elf $(STDDEP) $(EXTRADEP)
	$(OBD) -d main.elf

flash: all
	$(ECHO) "."
	$(AVRDUDE) -U flash:w:main.hex:i
	$(ECHO) "."

deepclean: clean
	$(RM) *~

clean:
	$(RM) *.o
	$(RM) main.hex
	$(RM) main.raw
	$(RM) main.asm
	$(RM) main.map
	$(RM) main.elf
