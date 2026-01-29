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
//<ID> 411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "[4,5]";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_SetNumberHelper(n1, 11.0);
    cJSON_SetNumberHelper(n2, 22.0);
    cJSON_SetNumberHelper(n3, 33.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);

    // step 2: Configure
    cJSON *p0 = cJSON_GetArrayItem(parsed, 0);
    cJSON *p1 = cJSON_GetArrayItem(parsed, 1);
    cJSON *p0_dup = cJSON_Duplicate(p0, 1);
    cJSON *p1_dup = cJSON_Duplicate(p1, 1);
    cJSON_AddItemToArray(arr, p0_dup);
    cJSON_AddItemToArray(arr, p1_dup);

    // step 3: Operate
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    double v3 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 3));
    double v4 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 4));
    double sum = v0 + v1 + v2 + v3 + v4;
    cJSON *sum_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(sum_item, sum);
    cJSON_AddItemToArray(arr, sum_item);

    // step 4: Validate and Cleanup
    int count = cJSON_GetArraySize(arr);
    (void)count;
    cJSON_Delete(parsed);
    cJSON_Delete(arr);
    // API sequence test completed successfully
    return 66;
}