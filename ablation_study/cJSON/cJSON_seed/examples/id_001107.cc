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
//<ID> 1107
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
    cJSON *n0 = cJSON_CreateNumber(12.5);
    cJSON *n1 = cJSON_CreateNumber(7.5);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "active", flag);

    // step 2: Configure
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON *item1 = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
    int count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)count);

    // step 3: Operate & Validate
    cJSON_bool arr_check = cJSON_IsArray(arr);
    cJSON_AddNumberToObject(root, "is_array_flag", (double)arr_check);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}