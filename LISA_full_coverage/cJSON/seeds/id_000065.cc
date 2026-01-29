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
//<ID> 65
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
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "alpha");
    cJSON_AddNumberToObject(child, "value", 123.0);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemReferenceToArray(items, child_ref);

    // step 3: Operate and Validate
    cJSON *first_in_array = cJSON_GetArrayItem(items, 0);
    cJSON_bool is_obj = cJSON_IsObject(first_in_array);
    cJSON_bool equal = cJSON_Compare(child, first_in_array, 1);
    cJSON *value_item = cJSON_GetObjectItem(child, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    int summary = (int)is_obj + (int)equal + (int)value_num;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}