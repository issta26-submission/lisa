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
//<ID> 960
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
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, str_item);
    cJSON *num_item = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, num_item);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(items, null_item);

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "child_one");
    cJSON_AddNumberToObject(child, "value", 100.0);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    char *first_value = cJSON_GetStringValue(first_item);
    cJSON *copied_name = cJSON_CreateString(first_value);
    cJSON_AddItemToObject(root, "copied_name", copied_name);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}