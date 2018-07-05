CC      = gcc
PROF    =
OPTOMIZ = -O2
DEBUG   = -g3
WARN    = -Wall
#C_FLAGS = $(PROF) $(OPTOMIZ) $(WARN) $(DEBUG)
CFLAGS = -O2 -Wall -g3 -Dunix -Dlinux
LDFLAGS = -g3 -lcrypt
SYSTEM  =
TARGET = mwg

O_FILES = act_comm.o act_info.o act_move.o act_obj.o act_wiz.o comm.o\
	  champ_skill.o cyborg.o db.o farsight.o fight.o handler.o\
	  magic.o save.o special.o update.o bit.o engage.o mem.o olc.o\
	  olc_act.o olc_save.o string.o mob_commands.o mob_prog.o\
	  gr_magic.o id.o chat_new.o interp.o scan.o remort.o bomb.o\
	  chat_act.o ore_prog.o quest.o raceskill.o act_room.o track.o\
	  war.o skin.o mech.o casino.o const.o telnet.o


all: $(O_FILES)
#	@echo "."
	@echo "[1;35mBuilding the MUD... [1;31m [1;32m[0;37m"
	rm -f $(TARGET)
	$(CC) -o $(TARGET) $(O_FILES) $(LDFLAGS)
	@echo "Complete."

clean:
	rm -f *.o
	rm -f $(TARGET)
	make

.c.o: merc.h
#	@echo -n "."
	$(CC) -c $(CFLAGS) $(SYSTEM) $<

1234:
#	@echo "."
	@echo "Updating envy on 1234."
#	rm -f /home/storm/EOS/src/fubar
	cp -f fubar /home/storm/EOS/src
	@echo "Done."

2222:
#	@echo "."
	@echo "Updating envy on 2222."
#	rm -f /home/storm/EOSBUILD/src/fubar
	cp -f fubar /home/storm/EOSBUILD/src
	@echo "Done."

port:
#	make
	make 1234
	make 2222
