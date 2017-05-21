#include "configuration.h"
#include "internals.h"

//xpath statics
static const char * IG_OPCITEM_XPATH = "/opcconfig/subscription[@subid='%i']/item[@nodeidnum][@igid]";
static const char * IG_OPCSUBSCRIPTION_XPATH = "/opcconfig/subscription[@intervalms][@subid]";
static const char * IG_OPCSERVER_XPATH = "/opcconfig/opcserver";

//---------OPCSERVER CONSTS
static const char * IG_OPCSERVER_HOST_VARNAME = "hostname";
static const char * IG_OPCSERVER_PROTOCOL_VARNAME = "proto";
static const char * IG_OPCSERVER_PORT_VARNAME = "port";

// format string for connection string <protocol>://<hostname>:<port>
static const char * IG_OPC_CONN_STRING_FORMAT = "%s://%s:%s";

//---------SUBSCRIPTION CONSTS
static const char * IG_CONFIG_OPC_INTERVALATTRNAME = "intervalms";
static const char * IG_CONFIG_OPC_SUBID = "subid";

//---------ITEM CONSTS
static const char * IG_CONFIG_OPC_NODEIDNAME = "nodeidnum";
static const char * IG_CONFIG_OPC_IGIDNAME = "igid";

//represents an opc ua subscription
typedef struct {
    IG_UInt32 subscription_id ;
    IG_UInt32 timeintervalms ;
} IG_OPC_Subscription;

//respresents an opc ua monitored item
typedef struct {
    IG_UInt32   to_sub_id   ;//which subscription it belongs to
    IG_UInt32   nodeid      ;//the numeric (for now!) OPCUA NodeId
    IG_Id       internal_id ;//which internal id it belongs to
} IG_OPC_Item;

// returns all subscriptions found in the xml file
IG_Status IG_Config_OPC_get_subscriptions(IG_Config * config, IG_ConfigResponse * response);

// returns all opc items
IG_Status IG_Config_OPC_get_items(IG_Config * config, IG_ConfigResponse * response, IG_UInt32 subid);

IG_Status IG_Config_OPC_get_conn_string(IG_Config * config, IG_ConfigResponse * response);