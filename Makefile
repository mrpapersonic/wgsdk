CFLAGS=-Wall -O2 -mdll -Isrc/include -I"discord_game_sdk/c"
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
DEPS=src/include/config.h src/include/dirtools.h src/include/main.h \
     src/include/timer.h src/include/resource.h
RC=src/include/dialog.rc
OBJ=src/config.o src/dirtools.o src/main.o src/timer.o

gen_DiscordGameSDK.dll: $(OBJ) src/include/dialog.o
	gcc -o $@ $(CFLAGS) $^ $(LDFLAGS)

src/include/dialog.o: src/include/dialog.rc
	windres -i $< -o $@

src/%.o: src/%.c $(DEPS)
	gcc -c $(CFLAGS) $< -o $@

clean:
	find . -type f -name '*.o' -not -path "./discord_game_sdk/*" -delete
	find . -type f -name '*.dll' -not -path "./discord_game_sdk/*" -delete
