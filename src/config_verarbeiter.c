

IG_Status IG_Config_Verarbeiter_get_RuleSets(IG_Config* conf, 
                                             IG_ConfigResponse * res) {
    
    
    return IG_STATUS_GOOD;
} 

IG_Status IG_Config_Verarbeiter_get_Rules(IG_Config* conf, IG_ConfigResponse * res) {
    
    IG_Rule * result = NULL;
    IG_Status rt;
    IG_ConfigResponse res;
    IG_UInt32 resAmount;

    //build xpath for specific input id

    //execute buildpath

    // allocate memory for array
    result = (IG_Rule * ) malloc(sizeof(IG_Rule)*resAmount);

    //for loop through results and get attributes
    for(IG_UInt32 i = 0; i<resAmount; i++) {
        IG_UInt32 outid;
        IG_RuleType type;
        IG_Duration interval;
        //TODO: Getprops and parse

        result[i].rule = type;
        result[i].interval = interval;
        result[i].outputId = outid;
    }

    res->responseAmount = resAmount;
    res->data = result;
    return IG_STATUS_GOOD;
}

