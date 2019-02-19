SOURCES = main.c serial+.c petascii.c firmata.c
ASOURCES = 
CC65_TARGET = c64
NAME = firmata
C64_IP = 192.168.1.64
DISK = $(NAME).d64
VENDOR  = ../vendor
CBMFB   = $(VENDOR)/CBM-FileBrowser_v1.6/programs

CC      = cl65
CFLAGS  = -t $(CC65_TARGET) -O -g -I.
LDFLAGS = -t $(CC65_TARGET) -m $(NAME).map -Ln $(NAME).lbl -I.
C1541  = c1541

.PHONY: all clean assets disk
all: assets $(NAME)

ifneq ($(MAKECMDGOALS),clean)
-include $(SOURCES:.c=.d)
endif

%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(NAME): $(SOURCES:.c=.o) $(ASOURCES:.s=.o)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(ASOURCES:.s=.o) $(SOURCES:.c=.o) $(SOURCES:.c=.d) $(NAME) $(NAME).map $(NAME).lbl

run: disk
	x64 "$(DISK):$(NAME)"

runc: all
	codenet -n $(C64_IP) -w $(NAME)
	codenet -n $(C64_IP) -r

disk:   $(DISK)

$(DISK): all
	$(C1541) -format "$(NAME),AA" d64 $@
	$(C1541) -attach $@ -write $(CBMFB)/fb
	$(C1541) -attach $@ -write $(CBMFB)/fb64
	$(C1541) -attach $@ -write $(NAME)
	$(C1541) -attach $@ -write ./c64-up2400.ser
	$(C1541) -attach $@ -list
