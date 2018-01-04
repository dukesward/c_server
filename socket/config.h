#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

#ifndef _UTILS_H_
	#include "utils.h"
#endif

const char* _CONFIG_ = "config.ini";

void readConfig(struct F_State *s);
void readConfigBuf(FILE *f, struct F_Config *c);
void fillConfig(struct F_Config *c, char *key, char *value, int i);
void processConfig(char *c);
char *getConfig(struct F_Config *c, char *key);

#endif