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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "node");
    cJSON_AddNumberToObject(child, "value", 123.0);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_bool added_ref = cJSON_AddItemReferenceToArray(arr, child_ref);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool are_equal = cJSON_Compare(child, first, 1);
    cJSON_bool first_is_object = cJSON_IsObject(first);
    cJSON *name_item = cJSON_GetObjectItem(first, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    int summary = (int)added_ref + (int)are_equal + (int)first_is_object + (name_str ? (int)name_str[0] : 0);
    cJSON_AddNumberToObject(root, "summary", (double)summary);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}