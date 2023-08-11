PROJECT=main

# main program + cpu startup code
OBJS = main.o startup.o syscalls.o

# additional module support
#OBJS += motor.o pwm.o

# USB CDC support
OBJS += USB/usb.o USB/usb_desc.o USB/usb_serial.o

CFLAGS = -std=gnu11 -Wall -ffunction-sections -fdata-sections -O2 -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb -I./USB -I./CMSIS -I.
ASFLAGS = -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb
LDFLAGS  = -Os -Wl,--gc-sections,--relax -lm -specs=nosys.specs -nostartfiles -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb -Timxrt1062.ld

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
STRIP = arm-none-eabi-strip
SIZE = arm-none-eabi-size
OBJCOPY = arm-none-eabi-objcopy

all: $(PROJECT).hex

run: $(PROJECT).hex
	teensy_loader_cli -mmcu=imxrt1062 -w -v $<

$(PROJECT).hex: $(PROJECT).elf
	$(STRIP) $<
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(PROJECT).elf: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(PROJECT).hex $(PROJECT).elf

.c: .o
	$(CC) $(CFLAGS) -c $< -o $@    

.cpp: .o
	$(CC) $(CFLAGS) -c $< -o $@

.s: .o
	$(AS) $(ASFLAGS) -o $@ $<
