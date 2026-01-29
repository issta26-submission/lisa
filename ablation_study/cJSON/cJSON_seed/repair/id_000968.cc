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
//<ID> 968
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
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(items, null_item);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "child_one");
    cJSON_AddNumberToObject(child, "id", 7.0);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(items, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON *copy = cJSON_CreateString(first_str);
    cJSON_AddItemToObject(root, "first_copy", copy);

    // step 3: Operate and Validate
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);
    cJSON *ref_name_item = cJSON_GetObjectItem(ref, "name");
    const char *ref_name = cJSON_GetStringValue(ref_name_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)first_str;
    (void)ref_name;
    // API sequence test completed successfully
    return 66;
}