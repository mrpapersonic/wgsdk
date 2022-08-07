#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <Winamp/wa_ipc.h>
#include "main.h"
#include "discord_game_sdk.h"
#include "timer.h"
#include "config.h"
#include "dirtools.h"
#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#define CLIENT_ID 969367220599803955
#define DISCORD_REQUIRE(x) \
	assert(x == DiscordResult_Ok)

#define GPPHDR_VER 0x10

winamp_general_purpose_plugin g_plugin = {
    GPPHDR_VER,  // version of the plugin, defined in "gen_myplugin.h"
    "Discord GameSDK", // name/title of the plugin, defined in "gen_myplugin.h"
    init,        // function name which will be executed on init event
    NULL,      // function name which will be executed on config event
    quit,        // function name which will be executed on quit event
    0,           // handle to Winamp main window, loaded by winamp when this dll is loaded
    0            // hinstance to this dll, loaded by winamp when this dll is loaded
};

WNDPROC g_lpWndProcOld = 0;

struct timer_t timer_callbacks = { .interval = 16 };
struct config_t config = {
	.display_title = 1,
	.show_elapsed_time = 1
};

struct DiscordActivity activity = {
	.application_id = CLIENT_ID,
	.name = "Winamp",
	.instance = 0
};

struct IDiscordActivityEvents activities_events;

struct app_t app;

void update_activity_callback(void* data, enum EDiscordResult result)
{
    DISCORD_REQUIRE(result);
}

void report_current_song_status(int playbackState)
{
	assert(playbackState != 0);
	activity.timestamps.start = 0;
	strcpy(activity.state, playbackState == 1 ? "(Playing)" : "(Paused)");

	if (playbackState == 1) {
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);
		ULARGE_INTEGER ul;
		ul.LowPart = ft.dwLowDateTime;
		ul.HighPart = ft.dwHighDateTime;
		long long dtn = ((ul.QuadPart - 116444736000000000ULL)/10000000);
		
		activity.timestamps.start = dtn - SendMessage(g_plugin.hwndParent, WM_WA_IPC, 0, IPC_GETOUTPUTTIME) / 1000;
	} else {
		activity.timestamps.start = 0;
	}
	
    char* detailsMessage = calloc(256, sizeof(char));
    //if (g_pluginSettings.DisplayTitleInStatus)
    //{
		wchar_t* title = (wchar_t*)SendMessageW(g_plugin.hwndParent, WM_WA_IPC, 0, IPC_GET_PLAYING_TITLE);
		assert(WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, title, -1, detailsMessage, 256, NULL, NULL));
    //}
    //else
    //{
    //    strcpy(activity.details, "");
    //}
	strcpy(activity.details, detailsMessage);
	free(detailsMessage);

	app.activities->update_activity(app.activities, &activity, &app, update_activity_callback);
}

void report_idle_status(void)
{
	activity.timestamps.start = 0;
	strcpy(activity.state, "(Idle)");

	app.activities->update_activity(app.activities, &activity, &app, update_activity_callback);
}

void update_rich_presence_details(void)
{
	LONG isPlayingResult = SendMessageW(g_plugin.hwndParent, WM_WA_IPC, 0, IPC_ISPLAYING);

	switch (isPlayingResult) {
		case 1:
			report_current_song_status(1);
			break;
		case 3:
			report_current_song_status(3);
			break;
		case 0:
			report_idle_status();
			break;
		default: break;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_WA_IPC && lParam == IPC_CB_MISC && wParam == IPC_CB_MISC_STATUS)
	{
		// Notification sent from Winamp on any change in playback.

		update_rich_presence_details();
    }

	return CallWindowProc(g_lpWndProcOld, hwnd, message, wParam, lParam);
}

int init() {
	char* path = dirtools_concat_paths(getenv("APPDATA"), "Winamp\\Plugins\\wgsdk");
	printf("%s", path);
	memset(&app, 0, sizeof(app));
	if (IsWindowUnicode(g_plugin.hwndParent)) {
		g_lpWndProcOld = (WNDPROC)SetWindowLongW(g_plugin.hwndParent, -4, (LONG)WndProc);
    } else {
		g_lpWndProcOld = (WNDPROC)SetWindowLongA(g_plugin.hwndParent, -4, (LONG)WndProc);
	}

	memset(&activity, 0, sizeof(activity));
    memset(&activities_events, 0, sizeof(activities_events));

	struct DiscordCreateParams params;
	DiscordCreateParamsSetDefault(&params);
	params.client_id = CLIENT_ID;
	params.flags = DiscordCreateFlags_Default;
	params.event_data = &app;
	params.activity_events = &activities_events;

	DISCORD_REQUIRE(DiscordCreate(DISCORD_VERSION, &params, &app.core));

	app.activities = app.core->get_activity_manager(app.core);

	timer_init(&timer_callbacks, g_plugin.hwndParent, TimerProc);
	timer_set(&timer_callbacks, g_plugin.hwndParent);

	cfg_load(&config);

	strcpy(activity.assets.large_image, "winamp-logo");

	update_rich_presence_details();

	return 0;
}

void quit() {
	assert(!cfg_save(config));
	app.activities->clear_activity(app.activities, &app, update_activity_callback);
}

void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD) {
	DISCORD_REQUIRE(app.core->run_callbacks(app.core));
}

__declspec(dllexport) winamp_general_purpose_plugin* winampGetGeneralPurposePlugin() {
	return &g_plugin;
}
