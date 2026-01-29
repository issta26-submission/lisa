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
//<ID> 964
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
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, str_item);
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, num_item);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(items, null_item);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "child1");
    cJSON_AddNumberToObject(child, "val", 3.14);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    char *first_value = cJSON_GetStringValue(first);
    cJSON *third = cJSON_GetArrayItem(items, 2);
    char *third_value = cJSON_GetStringValue(third);
    cJSON *dup_string = cJSON_CreateString(first_value);
    cJSON_AddItemToObject(root, "dup_first", dup_string);
    char *snapshot = cJSON_PrintUnformatted(root);
    (void)third_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}