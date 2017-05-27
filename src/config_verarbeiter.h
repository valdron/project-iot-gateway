#ifndef CONFIG_VERARBEITER_H
#define CONFIG_VERARBEITER_H


#include "configuration.h"
#include "verarbeiter.h"
#include "internals.h"

const static IG_Char * IG_VERARBEITER_RULES_XPATH_FORMAT = "/verarbeiterconfig/ruleset[@inid='%i']/rule";
const static IG_Char * IG_VERARBEITER_RULESET_XPATH = "/verarbeiterconfig/ruleset";
const static IG_Char * IG_VERARBEITER_RULESET_DATATYPE_VARNAME = "datatype";
const static IG_Char * IG_VERARBEITER_RULE_TYPE_VARNAME = "type";
const static IG_Char * IG_VERARBEITER_RULE_OUTPUTID_VARNAME = "outid";
const static IG_Char * IG_VERARBEITER_RULE_INTERVAL_VARNAME = "intervalms";

const static IG_Char * IG_VERARBEITER_TRANSMIT_TYPENAME = "transmit";
const static IG_Char * IG_VERARBEITER_AVERAGE_TYPENAME = "average";
const static IG_Char * IG_VERARBEITER_MAXIMUM_TYPENAME = "maximum";
const static IG_Char * IG_VERARBEITER_MINIMUM_TYPENAME = "minimum";




IG_Status IG_Config_Verarbeiter_get_RuleSets(IG_Config* conf, IG_ConfigResponse * res);

IG_Status IG_Config_Verarbeiter_get_Rules(IG_Config* conf, IG_ConfigResponse * res, IG_UInt32 inid);

#endif