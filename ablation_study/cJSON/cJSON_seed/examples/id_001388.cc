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
//<ID> 1388
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *false_item = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddRawToObject(root, "raw_meta", "{\"meta\":true}");

    // step 3: Operate & Validate
    cJSON_bool is_arr = cJSON_IsArray(arr);
    double current = cJSON_GetNumberValue(num);
    cJSON_SetNumberHelper(num, current + (double)is_arr);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}