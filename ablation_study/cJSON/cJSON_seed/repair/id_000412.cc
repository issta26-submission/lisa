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
//<ID> 412
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_json = "{\"values\":[10,20]}";
    cJSON *root = cJSON_Parse(initial_json);
    const char *other_json = "{\"a\":42}";
    cJSON *other = cJSON_Parse(other_json);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(0.0);
    double d1 = cJSON_SetNumberHelper(n1, 3.1415);
    (void)d1;
    cJSON *n2 = cJSON_CreateNumber(0.0);
    double d2 = cJSON_SetNumberHelper(n2, 2.718);
    (void)d2;
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate
    cJSON_AddItemToObject(root, "new_values", arr);
    cJSON *a_item = cJSON_GetObjectItem(other, "a");
    double a_val = cJSON_GetNumberValue(a_item);
    cJSON *n3 = cJSON_CreateNumber(a_val);
    cJSON_AddItemToArray(arr, n3);

    // step 4: Validate and Cleanup
    int sz0 = cJSON_GetArraySize(arr);
    (void)sz0;
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    double sum = v0 + v1 + v2;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_Delete(other);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}