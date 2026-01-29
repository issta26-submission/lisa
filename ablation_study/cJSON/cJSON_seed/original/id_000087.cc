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
//<ID> 87
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
    cJSON_AddStringToObject(child, "name", "original");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *flag = cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(items, num);
    cJSON *child_dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToArray(items, child_dup);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON *second = cJSON_GetArrayItem(items, 1);
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON *detached_child = cJSON_DetachItemViaPointer(root, child);
    char *printed = cJSON_Print(root);
    double first_val = cJSON_GetNumberValue(first);
    int count = cJSON_GetArraySize(items);
    int summary = (int)items_is_array + (int)first_val + count + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached_child);

    // API sequence test completed successfully
    return 66;
}