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
//<ID> 1255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddNumberToObject(config, "threshold", 3.14);
    cJSON_AddStringToObject(config, "mode", "active");
    cJSON *first = cJSON_CreateString("alpha");
    cJSON *second = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, first);
    cJSON_AddItemToArray(items, second);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 3: Operate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON_bool is_invalid = cJSON_IsInvalid(got_config);
    (void)is_invalid;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    cJSON *detached = cJSON_DetachItemViaPointer(got_items, first_item);

    // step 4: Validate & Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}