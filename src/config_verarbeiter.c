#include<stdio.h>
#include<string.h>
#include "config_verarbeiter.h"


IG_Status get_outid_from_str(IG_Char * str, IG_UInt32  * outid);
IG_Status get_type_from_str(IG_Char * str, IG_RuleType  * type);
IG_Status get_interval_from_str(IG_Char * str, IG_Duration  * interval);


IG_Status IG_Config_Verarbeiter_get_RuleSets(IG_Config* conf, 
                                             IG_ConfigResponse * res) {

    IG_Input_RuleSet * sets;
    IG_Status rt;
    IG_UInt32 resAmount;
    
    xmlNodePtr node;
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;

    ctx = xmlXPathNewContext(conf->ptr);
    if(ctx == NULL) {
        printf("could not create XPath context\n");
        return IG_STATUS_BAD;
    }

    obj = xmlXPathEvalExpression(IG_VERARBEITER_RULESET_XPATH, ctx);
    if(obj == NULL) {
        printf("could not eval xpath\n");
        xmlXPathFreeContext(ctx);
        return IG_STATUS_BAD;
    }

    if(obj->nodesetval == NULL || obj->nodesetval->nodeTab == NULL) {
        xmlXPathFreeContext(ctx);
        xmlXPathFreeObject(obj);
        return IG_STATUS_BAD;
    }

    
    resAmount = obj->nodesetval->nodeNr;

    // allocate memory for array
    sets = (IG_Input_RuleSet * ) malloc(sizeof(IG_Input_RuleSet)*resAmount);

    for(IG_UInt32 i = 0; i<resAmount; i++) {
        node = obj->nodesetval->nodeTab[i];
        IG_UInt32 inid;
        IG_Datatype type;

        IG_Char * type_str = xmlGetProp(node,IG_VERARBEITER_RULESET_DATATYPE_VARNAME);
        IG_Char * inid_str = xmlGetProp(node,IG_VERARBEITER_RULESET_INPUTID_VARNAME);
        if(type_str == NULL || inid_str == NULL) {
            free(sets);
            xmlXPathFreeContext(ctx);
            xmlXPathFreeObject(obj);
            return IG_STATUS_BAD;
        }

        type = IG_Datatype_from_str(type_str);
        inid = atoi(inid_str);
        
        free(inid_str);
        free(type_str);

        if(type == IG_NULL || inid == 0) {
            xmlXPathFreeContext(ctx);
            xmlXPathFreeObject(obj);
            return IG_STATUS_BAD;
        }

        sets[i].datatype = type;
        sets[i].inputId = inid;
    }

    res->responseAmount = resAmount;
    res->data = sets;
    
    return IG_STATUS_GOOD;
} 

IG_Status IG_Config_Verarbeiter_get_Rules(IG_Config* conf, IG_ConfigResponse * res, IG_UInt32 inid) {
    
    IG_Rule * result = NULL;
    IG_Status rt;
    IG_UInt32 resAmount;

    //build xpath for specific input id
    IG_Char xpath[100];
    snprintf(xpath, 100, IG_VERARBEITER_RULES_XPATH_FORMAT, inid);

    xmlNodePtr node;
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;

    //execute buildpath
    ctx = xmlXPathNewContext(conf->ptr);
    if(ctx == NULL) {
        printf("could not create XPath context\n");
        return IG_STATUS_BAD;
    }

    obj = xmlXPathEvalExpression(xpath, ctx);
    if(obj == NULL) {
        printf("could not eval xpath\n");
        xmlXPathFreeContext(ctx);
        return IG_STATUS_BAD;
    }

    if(obj->nodesetval == NULL || obj->nodesetval->nodeTab == NULL) {
        xmlXPathFreeContext(ctx);
        xmlXPathFreeObject(obj);
        return IG_STATUS_BAD;
    }

    
    resAmount = obj->nodesetval->nodeNr;

    // allocate memory for array
    result = (IG_Rule * ) malloc(sizeof(IG_Rule)*resAmount);

    //for loop through results and get attributes
    for(IG_UInt32 i = 0; i<resAmount; i++) {
        node = obj->nodesetval->nodeTab[i];
        IG_UInt32 outid;
        IG_RuleType type;
        IG_Duration interval;
        //TODO: Getprops and parse

        IG_Char * outid_str = xmlGetProp(node,IG_VERARBEITER_RULE_OUTPUTID_VARNAME);
        IG_Char * type_str = xmlGetProp(node,IG_VERARBEITER_RULE_TYPE_VARNAME);
        IG_Char * interval_str = xmlGetProp(node,IG_VERARBEITER_RULE_INTERVAL_VARNAME);
        if(outid_str == NULL || type_str == NULL || interval_str == NULL) {
            free(result);
            xmlXPathFreeContext(ctx);
            xmlXPathFreeObject(obj);
            return IG_STATUS_BAD;
        }
        

        IG_Status rt1 = get_outid_from_str(outid_str, &outid);
        IG_Status rt2 = get_type_from_str(type_str, &type);
        IG_Status rt3 = get_interval_from_str(interval_str, &interval);

        free(outid_str);
        free(type_str);
        free(interval_str);
        if(rt1 != IG_STATUS_GOOD || rt2 != IG_STATUS_GOOD || rt3 != IG_STATUS_GOOD) {
            free(result); 
            xmlXPathFreeContext(ctx);
            xmlXPathFreeObject(obj);
            return IG_STATUS_BAD;
        }



        result[i].rule = type;
        result[i].interval = interval;
        result[i].outputId = outid;
    }

    res->responseAmount = resAmount;
    res->data = result;

    xmlXPathFreeContext(ctx);
    xmlXPathFreeObject(obj);
    return IG_STATUS_GOOD;
}

IG_Status get_outid_from_str(IG_Char * str, IG_UInt32  * outid) {

    *outid = atoi(str);
    if(outid == 0) {
        return IG_STATUS_BAD;
    }
    return IG_STATUS_GOOD;
}

IG_Status get_type_from_str(IG_Char * str, IG_RuleType  * type) {
    if(!strcmp(str,IG_VERARBEITER_TRANSMIT_TYPENAME)) {
        *type = IG_RULE_TRANSMIT;
    } else if(!strcmp(str,IG_VERARBEITER_AVERAGE_TYPENAME)) {
        *type = IG_RULE_AVG;
    } else if(!strcmp(str,IG_VERARBEITER_MAXIMUM_TYPENAME)) {
        *type = IG_RULE_MAX;
    } else if(!strcmp(str,IG_VERARBEITER_MINIMUM_TYPENAME)) {
        *type = IG_RULE_MIN;
    } else {
        printf("type not found or not implemented\n");
        return IG_STATUS_BAD;
    }
    return IG_STATUS_GOOD;

}
IG_Status get_interval_from_str(IG_Char * str, IG_Duration  * interval) {

    IG_UInt32 intervalms = atoi(str);
    if(intervalms == 0) {
        return IG_STATUS_BAD;
    }

    *interval = IG_Duration_from_ms(intervalms);
    return IG_STATUS_GOOD;
}
