#ifndef RUNNER_CONFIG_H
#define RUNNER_CONFIG_H

#include "ini.h"
#include "hashtable.h"

char* rootDir;
Hashtable configTable;

void config_init();
void config_terminate();
void config_load(const char* filename);
Ini* config_get(const char* name);

#endif