# Alun Jones - Changed to using teensy_loader_cli for loading code
# Matt Williams - Set c99 standard as well.

PROJECT=main

# main program + cpu startup code
OBJS = $(PROJECT).o startup_mimxrt1062.o system_MIMXRT1062.o

# additional module support
OBJS += motor.o pwm.o

# USB CDC support
#OBJS += USB/usb_desc.o USB/usb_dev.o USB/usb_mem.o USB/usb_serial.o

CFLAGS = -std=c99 -Wall -fno-common -O3 -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -specs=nosys.specs -I./USB -I./CMSIS -I.
ASFLAGS = -mcpu=cortex-m7
LDFLAGS  = -lm -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -specs=nosys.specs -nostartfiles -Timxrt1062.ld

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
STRIP = arm-none-eabi-strip
OBJCOPY = arm-none-eabi-objcopy

all:: $(PROJECT).hex

run: $(PROJECT).hex
	teensy_loader_cli -mmcu=imxrt1062 -v $(PROJECT).hex

$(PROJECT).hex: $(PROJECT).elf
	$(STRIP) $(PROJECT).elf
	$(OBJCOPY) -R .stack -O ihex $(PROJECT).elf $(PROJECT).hex

$(PROJECT).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(PROJECT).elf

clean:
	rm -f $(OBJS) $(PROJECT).hex $(PROJECT).elf

.c.o :
	$(CC) $(CFLAGS) -c $< -o $@    

.cpp.o :
	$(CC) $(CFLAGS) -c $< -o $@

.s.o :
	$(AS) $(ASFLAGS) -o $@ $<
