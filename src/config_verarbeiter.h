#ifndef CONFIG_VERARBEITER_H
#define CONFIG_VERARBEITER_H


#include "configuration.h"
#include "verarbeiter.h"
#include "internals.h"

const static * IG_Char BLA = "blub";



IG_Status IG_Config_Verarbeiter_get_RuleSets(IG_Config* conf, IG_ConfigResponse * res);

IG_Status IG_Config_Verarbeiter_get_Rules(IG_Config* conf, IG_ConfigResponse * res);

#endif