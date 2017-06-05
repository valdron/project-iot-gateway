#ifndef CONFIG_MAIN_H
#define CONFIG_MAIN_H

#include "configuration.h"

static const unsigned char * IG_CONFIG_MAIN_XPATH_FORMAT = "/igconfig/%s";

static const unsigned char * IG_CONFIG_MAIN_FILE_VARNAME = "filename";

IG_Status IG_Config_Main_get_filename(IG_Config * config, IG_ConfigResponse * response, unsigned char * configtag);

#endif

