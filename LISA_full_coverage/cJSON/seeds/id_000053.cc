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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"original\",\"value\":42,\"remove_me\":\"bye\"}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "name", new_name);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON *remove_item = cJSON_GetObjectItem(root, "remove_me");
    double value_num = cJSON_GetNumberValue(value_item);
    int summary = (int)replaced + (int)value_num + name_item->valueint + remove_item->valueint;
    (void)summary;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove_me");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}