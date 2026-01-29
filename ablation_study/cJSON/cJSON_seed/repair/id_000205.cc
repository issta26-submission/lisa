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
//<ID> 205
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

    // step 2: Configure
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(items, false_item);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(items, null_item);

    // step 3: Operate and Validate
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_dup);
    cJSON *retrieved_copy = cJSON_GetObjectItem(root, "items_copy");
    int original_count = cJSON_GetArraySize(items);
    int copy_count = cJSON_GetArraySize(retrieved_copy);
    cJSON_AddNumberToObject(meta, "original_count", (double)original_count);
    cJSON_AddNumberToObject(meta, "copy_count", (double)copy_count);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}