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
//<ID> 969
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
    cJSON *first = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, first);
    cJSON *second = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, second);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(items, null_item);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "child_name", "ref_target");
    cJSON_AddNumberToObject(child, "child_val", 7.0);
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate and Validate
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    char *second_str = cJSON_GetStringValue(second_item);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    char *first_str = cJSON_GetStringValue(first_item);
    cJSON *copied_first = cJSON_CreateString(first_str);
    cJSON *copied_second = cJSON_CreateString(second_str);
    cJSON_AddItemToObject(root, "copied_first", copied_first);
    cJSON_AddItemToObject(root, "copied_second", copied_second);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}