# makefile configuration
COMMON_OBJECTS	=
CPU	      	= msp430g2553
CFLAGS		= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include

# switch the compiler (for internal make rules)
CC		= msp430-elf-gcc
AS		= msp430-elf-as
AR 		= msp430-elf-ar

all: lab.elf

# additional rules for files
lab.elf: ${COMMON_OBJECTS} main.o stateMachines.o switches.o led.o buzzer.o p2_interrupt_handler.o wdInterruptHandler.o ../lib/libTimer.a
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lLcd -lp2sw -lTimer

load: lab.elf
	msp430loader.sh lab.elf $^
clean:
	rm -f *.a *.o *.elf

main.o: main.c

stateMachines.o: stateMachines.c stateMachines.h

switches.o: switches.c switches.h

led.o: led.c led.h

buzzer.o: buzzer.c buzzer.h

p2_interrupt_handler.o: p2_interrupt_handler.c

wdInterruptHandler.o: stateMachines.h led.h
