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
//<ID> 921
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("device-A");
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *name_item = cJSON_CreateString("device-A");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *arr_num = cJSON_GetArrayItem(arr, 1);
    cJSON_SetNumberHelper(arr_num, 42.0);
    cJSON *replacement = cJSON_CreateString("device-B");
    cJSON_ReplaceItemInObject(root, "name", replacement);

    // step 3: Operate & Validate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    cJSON *orig_compare = cJSON_CreateString("device-A");
    cJSON_bool are_equal = cJSON_Compare(got_name, orig_compare, 1);
    cJSON_AddBoolToObject(meta, "name_changed", 1 - are_equal);
    cJSON_Delete(orig_compare);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}