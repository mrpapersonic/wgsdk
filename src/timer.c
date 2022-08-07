#include <assert.h>
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "timer.h"
void timer_init(struct timer_t* timer, HWND winampClientWindow, TIMERPROC timer_proc) {
	timer->winampClientWindow = winampClientWindow;
	timer->timer_proc = timer_proc;
	timer->initialized = 1;
}

void timer_set(struct timer_t* timer, HWND winampClientWindow) {
	assert(timer->initialized);
	
	if (timer->is_timer_alive)
		return;
	
	timer->is_timer_alive = 1;
	SetTimer(winampClientWindow, 1, timer->interval, timer->timer_proc);
}

void timer_stop(struct timer_t* timer, HWND winampClientWindow) {
	assert(timer->initialized);
	
	if (!timer->is_timer_alive)
		return;
	
	
	timer->is_timer_alive = 0;
	KillTimer(winampClientWindow, 1);
}