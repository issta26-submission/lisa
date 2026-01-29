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
//<ID> 932
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "name", "device_alpha");
    cJSON_AddNumberToObject(root, "id", 42.0);

    // step 2: Configure
    cJSON_AddItemToObject(meta, "status", cJSON_CreateString("ok"));
    cJSON_AddItemToObject(meta, "raw_meta", cJSON_CreateRaw("{\"enabled\":true}"));
    cJSON_AddItemToObject(root, "payload", cJSON_CreateObject());
    cJSON *payload = cJSON_GetObjectItemCaseSensitive(root, "payload");
    cJSON_AddItemToObject(payload, "value", cJSON_CreateNumber(3.14));

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON *replacement_raw = cJSON_CreateRaw("\"device_replaced\"");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(root, name_item, replacement_raw);
    (void)replaced;
    cJSON *queried_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *name_value = cJSON_GetStringValue(queried_name);
    (void)name_value;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}