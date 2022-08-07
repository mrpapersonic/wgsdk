CFLAGS=-Wall -mdll -Isrc/include -I"discord_game_sdk/c"
ifeq ($(PROCESSOR_ARCHITECTURE),x86)
	ifeq ($(PROCESSOR_ARCHITEW6432),x86)
		CFLAGS += -I"/c/Program Files/Winamp SDK" 
	else
		CFLAGS += -I"/c/Program Files (x86)/Winamp SDK"
	endif
else
	CFLAGS += -I"/c/Program Files (x86)/Winamp SDK"
endif
LDFLAGS=-L"discord_game_sdk/lib/x86" -ldiscord_game_sdk
DEPS=src/include/config.h src/include/dirtools.h src/include/main.h src/include/timer.h
OBJ=src/config.o src/dirtools.o src/main.o src/timer.o

src/%.o: src/%.c $(DEPS)
	gcc -c $(CFLAGS) $< -o $@

gen_DiscordGameSDK.dll: $(OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	rm -f src/*.o *.dll