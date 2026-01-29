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
//<ID> 1383
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
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *s0 = cJSON_CreateString("hello");
    cJSON *f0 = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, f0);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *flag = cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Operate & Validate
    cJSON_bool was_array = cJSON_IsArray(arr);
    double base = cJSON_GetNumberValue(n0);
    cJSON *updated = cJSON_CreateNumber(base + (double)was_array);
    cJSON_AddItemToObject(root, "updated", updated);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}