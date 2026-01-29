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
//<ID> 1103
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
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *t = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate & Validate
    cJSON_bool is_arr = cJSON_IsArray(arr);
    int count = cJSON_GetArraySize(arr);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    double sum = v0 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddNumberToObject(root, "count", (double)count);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    (void)is_arr;
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}