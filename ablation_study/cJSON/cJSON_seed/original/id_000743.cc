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
//<ID> 743
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *first = cJSON_CreateString("alpha");
    cJSON *second = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, first);
    cJSON_AddItemToArray(items, second);
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *enabled_flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled_flag);
    cJSON *null_field = cJSON_AddNullToObject(root, "missing");

    // step 3: Operate
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_dup);
    int original_size = cJSON_GetArraySize(items);
    int copy_size = cJSON_GetArraySize(items_dup);
    cJSON_AddNumberToObject(root, "original_size", (double)original_size);
    cJSON_AddNumberToObject(root, "copy_size", (double)copy_size);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}