#include "configuration.h"
#include "internals.h"

//xpath statics
static const char * IG_OPCITEM_XPATH = "/opcconfig/subscription[@subid='%i']/item[@nodeidnum][@igid]";
static const char * IG_OPCSUBSCRIPTION_XPATH = "/opcconfig/subscription[@intervalms][@subid]";

static const char * IG_CONFIG_OPC_INTERVALATTRNAME = "intervalms";
static const char * IG_CONFIG_OPC_SUBID = "subid";

static const char * IG_CONFIG_OPC_NODEIDNAME = "nodeidnum";
static const char * IG_CONFIG_OPC_IGIDNAME = "igid";


typedef struct {
    IG_UInt32 subscription_id ;
    IG_UInt32 timeintervalms ;
} IG_OPC_Subscription;

typedef struct {
    IG_UInt32   to_sub_id   ;
    IG_UInt32   nodeid      ;//only numeric for now
    IG_Id       internal_id ;
} IG_OPC_Item;

// returns all subscriptions found in the xml file
IG_Status IG_Config_OPC_get_subscriptions(IG_Config * config, IG_ConfigResponse * response);

// returns all opc items
IG_Status IG_Config_OPC_get_items(IG_Config * config, IG_ConfigResponse * response, IG_UInt32 subid);