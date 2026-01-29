#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateObject();
    cJSON *id = cJSON_CreateNumber(42.0);
    cJSON *greeting = cJSON_CreateString("hello world");

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON_AddItemToObject(meta, "id", id);
    cJSON_AddItemToObject(meta, "greeting", greeting);
    cJSON_AddStringToObject(payload, "message", "embedded");
    cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate & Validate
    cJSON *msg_item = cJSON_GetObjectItem(payload, "message");
    char *msg_value = cJSON_GetStringValue(msg_item);
    cJSON *echo = cJSON_CreateString(msg_value);
    cJSON_AddItemToObject(meta, "echo", echo);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}