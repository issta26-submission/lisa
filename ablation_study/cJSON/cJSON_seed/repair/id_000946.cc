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
//<ID> 946
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "name", "fuzzer_node");
    cJSON_AddNumberToObject(root, "version", 1.23);

    // step 2: Configure
    cJSON *first = cJSON_CreateString("alpha");
    cJSON *second = cJSON_CreateNumber(2.0);
    cJSON_InsertItemInArray(items, 0, first);
    cJSON_InsertItemInArray(items, 1, second);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(child, "inner", "value");

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool are_equal = cJSON_Compare(root, dup, 1);
    cJSON_AddNumberToObject(root, "equal", (double)are_equal);
    cJSON *meta_item = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_item);
    cJSON_AddBoolToObject(root, "meta_is_object", meta_is_obj);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON_AddStringToObject(child, "first_item", first_str ? first_str : "null");
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}