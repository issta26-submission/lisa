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
//<ID> 331
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
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *extra = cJSON_CreateNumber(5.5);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON_AddNumberToObject(root, "added_via_api", 7.75);

    // step 3: Operate & Validate
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *val0 = cJSON_GetArrayItem(values, 0);
    cJSON *val1 = cJSON_GetArrayItem(values, 1);
    cJSON *val2 = cJSON_GetArrayItem(values, 2);
    double extra_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "extra"));
    double added_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "added_via_api"));
    double sum = cJSON_GetNumberValue(val0) + cJSON_GetNumberValue(val1) + cJSON_GetNumberValue(val2) + extra_val + added_val;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}