#ifndef __timer_h
#define __timer_h
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

struct timer_t {
	int initialized;
	int is_timer_alive;
	UINT interval;
	HWND winampClientWindow;
	TIMERPROC timer_proc;
};

void timer_init(struct timer_t* timer, HWND winampClientWindow, TIMERPROC timer_proc);
void timer_set(struct timer_t* timer, HWND winampClientWindow);
void timer_stop(struct timer_t* timer, HWND winampClientWindow);

#endif // __timer_h