#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirtools.h"
#include "config.h"
#include "main.h"
#include "resource.h"
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <windowsx.h>
#define MAX_LINE_LENGTH 128

extern struct config_t config; // from main

static unsigned int crc32b(unsigned char *message) {
   int i, j;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (message[i] != 0) {
      byte = message[i];            // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {    // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}

int cfg_load(struct config_t* config) {
	char line[MAX_LINE_LENGTH] = {0}, 
	     *path = dirtools_concat_paths(getenv("APPDATA"), "Winamp\\Plugins\\wgsdk\\config.txt");
	FILE* config_fp;
	config_fp = fopen(path, "r");
	if (config_fp == NULL) {
		free(path);
		return 1;
	}
	while (fgets(line, MAX_LINE_LENGTH, config_fp)) {
		switch (crc32b((unsigned char*)strtok(line, "="))) {
			case 0x2a666380: // display_title
				config->display_title = !!atoi(strtok(NULL, "="));
				break;
			case 0xbb631f7f: // show_elapsed_time
				config->show_elapsed_time = !!atoi(strtok(NULL, "="));
				break;
			default:
				break;
		}
	}
	free(path);
	return 0;
}

int cfg_save(struct config_t config) {
	char* path = dirtools_concat_paths(getenv("APPDATA"), "Winamp\\Plugins\\wgsdk");
	FILE* config_fp;
	assert(!dirtools_create_directory(path));
	free(path);
	path = dirtools_concat_paths(getenv("APPDATA"), "Winamp\\Plugins\\wgsdk\\config.txt");
	config_fp = fopen(path, "w");
	if (config_fp == NULL) {
		return 1;
	}
	fprintf(config_fp, "----- wgsdk config ----\n");
	fprintf(config_fp, "display_title=%d\n", config.display_title);
	fprintf(config_fp, "show_elapsed_time=%d\n", config.show_elapsed_time);
	fclose(config_fp);
	return 0;
}

/* --------------------------------- */

void cfg_dialog_to_struct(HWND hWnd) {
	HWND checkboxHwnd = GetDlgItem(hWnd, TITLE_CHECK);
	config.display_title = Button_GetCheck(checkboxHwnd);

	checkboxHwnd = GetDlgItem(hWnd, ELAPSED_TIME_CHECK);
	config.show_elapsed_time = Button_GetCheck(checkboxHwnd);
}

void cfg_on_confirm_settings_dialog(HWND hWnd)
{
    cfg_dialog_to_struct(hWnd);
    cfg_save(config);
	update_rich_presence_details();
}

void cfg_set_controls(HWND hWnd) {
	HWND checkboxHwnd = GetDlgItem(hWnd, TITLE_CHECK);
	Button_SetCheck(checkboxHwnd, config.display_title);

	checkboxHwnd = GetDlgItem(hWnd, ELAPSED_TIME_CHECK);
	Button_SetCheck(checkboxHwnd, config.show_elapsed_time);
}

BOOL CALLBACK cfg_win_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG: {
			if (GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)
			{
				// do nothing if this is a child window (tab page) callback, pass to the parent
				return FALSE;
			}

			cfg_set_controls(hWnd);
			return TRUE;
		}

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDOK: {
					cfg_on_confirm_settings_dialog(hWnd);
					EndDialog(hWnd, 0);
					return TRUE;
					break;
				}
				case IDCANCEL: {
					EndDialog(hWnd, 0);
					return TRUE;
					break;
				}
			}
	}

    return FALSE;
}
