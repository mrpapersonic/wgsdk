#ifndef __main_h
#define __main_h
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

typedef struct {
    int version;                   // version of the plugin structure
    char *description;             // name/title of the plugin 
    int(*init)();                 // function which will be executed on init event
    void(*conf)();              // function which will be executed on config event
    void(*quit)();                // function which will be executed on quit event
    HWND hwndParent;               // hwnd of the Winamp client main window (stored by Winamp when dll is loaded)
    HINSTANCE hDllInstance;        // hinstance of this plugin DLL. (stored by Winamp when dll is loaded) 
} winamp_general_purpose_plugin;

struct app_t {
    struct IDiscordCore* core;
    struct IDiscordUsers* users;
	struct IDiscordActivityManager* activities;
};

void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);
int init();
void conf();
void quit();
void update_rich_presence_details(void);
#endif