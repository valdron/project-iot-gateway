#include<stdio.h>
#include "configuration.h"
#include "config_verarbeiter.h"

#define USAGE "command [xmlfile]"

int main(int argc, char * argv[]) {

    if(argc != 2) {
        printf(USAGE);
        return -1;
    }
    IG_Config * conf = IG_Config_create(argv[1], IG_CONFIG_VERARBEITER);

    if(conf == NULL) {
        printf("wrong format of xml\n");
        return -1;
    }
    IG_ConfigResponse res;
    IG_Status rt = IG_Config_Verarbeiter_get_RuleSets(conf,&res);
    if(rt != IG_STATUS_GOOD) {
        return -1;
    }

    IG_UInt32 size = res.responseAmount;
    IG_Input_RuleSet * sets = (IG_Input_RuleSet *) res.data;

    for(IG_UInt32 i = 0; i < size; i++) {
        IG_ConfigResponse res_rules;
        rt = IG_Config_Verarbeiter_get_Rules(conf, &res_rules, sets[i].inputId);
        if(rt != IG_STATUS_GOOD) {
            return -1;
        }

        sets[i].ruleSize = res_rules.responseAmount;
        sets[i].rules = (IG_Rule *) res_rules.data;
    }

    return 0;
}