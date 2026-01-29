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
//<ID> 940
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
    cJSON *first = cJSON_CreateString("alpha");
    cJSON *second = cJSON_CreateString("beta");
    cJSON_bool inserted_first = cJSON_InsertItemInArray(items, 0, first);
    cJSON_bool inserted_second = cJSON_InsertItemInArray(items, 1, second);
    (void)inserted_first;
    (void)inserted_second;
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("fuzzer_node"));
    cJSON_AddItemToObject(config, "enabled", cJSON_CreateTrue());

    // step 3: Operate and Validate
    cJSON *root_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool config_is_object = cJSON_IsObject(config);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON_AddNumberToObject(root, "config_is_object", (double)config_is_object);
    cJSON_AddBoolToObject(root, "root_equals_dup", equal);
    cJSON_InsertItemInArray(root_items, 1, cJSON_CreateNumber(99.0));

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}