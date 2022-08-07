#ifndef __config_h
#define __config_h
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
struct config_t {
	int display_title;
	int show_elapsed_time;
};

int cfg_load(struct config_t* config);
int cfg_save(struct config_t config);
BOOL CALLBACK cfg_win_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif