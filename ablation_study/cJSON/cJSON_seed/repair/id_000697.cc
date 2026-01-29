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
//<ID> 697
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "items", items);

    // step 2: Configure
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, flag);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToArray(items, child);
    cJSON_AddStringToObject(child, "name", "alpha");
    cJSON_AddNumberToObject(config, "version", 1.23);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON_bool child_is_obj = cJSON_IsObject(child);
    cJSON_AddBoolToObject(root, "child_is_object", child_is_obj);
    cJSON_AddNumberToObject(root, "items_count", (double)count);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}