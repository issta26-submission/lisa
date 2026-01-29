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
//<ID> 1382
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
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, false_item);

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *explicit_false = cJSON_AddBoolToObject(root, "explicit_false", 0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 3: Operate & Validate
    cJSON *items_ptr = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_array = cJSON_IsArray(items_ptr);
    int count = cJSON_GetArraySize(items_ptr);
    cJSON *first = cJSON_GetArrayItem(items_ptr, 0);
    double value = cJSON_GetNumberValue(first);
    double updated = value + (double)count + (double)is_array;
    cJSON_SetNumberHelper(first, updated);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}