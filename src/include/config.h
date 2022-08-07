#ifndef __config_h
#define __config_h
struct config_t {
	int display_title;
	int show_elapsed_time;
};

int cfg_load(struct config_t* config);
int cfg_save(struct config_t config);
#endif