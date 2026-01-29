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
//<ID> 82
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
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON *wrapper = cJSON_CreateObject();
    cJSON_AddItemToObject(wrapper, "dup", dup_arr);
    cJSON *n3 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(dup_arr, n3);

    // step 3: Operate and Validate
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    double v0 = cJSON_GetNumberValue(item0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    double v1 = cJSON_GetNumberValue(item1);
    cJSON_AddNumberToObject(root, "sum", v0 + v1);
    cJSON *dup_extra = cJSON_GetArrayItem(dup_arr, 2);
    double vdup = cJSON_GetNumberValue(dup_extra);
    cJSON_AddNumberToObject(root, "duplicate_extra", vdup);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(wrapper);
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}