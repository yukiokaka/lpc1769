#  Project Name
PROJECT=prototype

#  List of the objects files to be compiled/assemble
OBJECTS=startup_LPC17xx.o core_cm3.o system_LPC17xx.o $(PROJECT).o
LSCRIPT=LPC17xx.ld
OPTIMIZATION = s -fno-schedule-insns2 -fsection-anchors -fpromote-loop-indices -ffunction-sections -fdata-sections
#DEBUG = -g

#  Compiler Options
GCFLAGS = -Wall -mcpu=cortex-m3 -mfloat-abi=softfp -mthumb -mfix-cortex-m3-ldrd -O$(OPTIMIZATION) $(DEBUG)
GCFLAGS += -D__RAM_MODE__=0 -Icmsis
LDFLAGS = -mcpu=cortex-m3 -mfloat-abi=softfp -mthumb -mfix-cortex-m3-ldrd -O$(OPTIMIZATION) -Wl,-Map=$(PROJECT).map -T$(LSCRIPT)
ASFLAGS = -mcpu=cortex-m3 --defsym RAM_MODE=0

#  Compiler/Assembler/Linker Paths
GCC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
REMOVE = rm -f
SIZE = arm-none-eabi-size


LPCXPRESSODIR=/usr/local/lpcxpresso_7.1.1_125/lpcxpresso
#########################################################################

all:: $(PROJECT).hex $(PROJECT).bin $(PROJECT).lss

$(PROJECT).lss: $(PROJECT).elf
	$(OBJDUMP) -h -S $(PROJECT).elf > $(PROJECT).lss

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary -j .text -j .data $(PROJECT).elf $(PROJECT).bin


$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -R .stack -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(OBJECTS)
	$(GCC) $(LDFLAGS) $(OBJECTS) -o $(PROJECT).elf

stats: $(PROJECT).elf
	$(SIZE) $(PROJECT).elf

clean:
	$(REMOVE) $(OBJECTS)
	$(REMOVE) $(PROJECT).bin
	$(REMOVE) $(PROJECT).elf
	$(REMOVE) $(PROJECT).hex
	$(REMOVE) $(PROJECT).lss
	$(REMOVE) $(PROJECT).map

#########################################################################
#  Default rules to compile .c and .cpp file to .o
#  and assemble .s files to .o

.c.o :
	$(GCC) $(GCFLAGS) -c $<

.cpp.o :
	$(GCC) $(GCFLAGS) -c $<

.S.o :
	$(AS) $(ASFLAGS) -o $@ $<

.PHONY: boot
boot:
	$(LPCXPRESSODIR)/bin/dfu-util -d 0x471:0xdf55 -c 0 -t 2048 -R -D $(LPCXPRESSODIR)/bin/LPCXpressoWIN.enc

.PHONY: write
write: $(PROJECT).bin
	$(LPCXPRESSODIR)/bin/crt_emu_cm3_nxp -wire=winusb -pLPC1769 -vendor=NXP -2 -flash-load-exec=$(PROJECT).bin
#########################################################################