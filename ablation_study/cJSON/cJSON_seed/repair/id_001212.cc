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
//<ID> 1212
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(obj1, "value", num1);
    cJSON *obj2 = cJSON_CreateObject();
    cJSON *num2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToObject(obj2, "value", num2);
    cJSON_AddItemToArray(arr, obj1);
    cJSON_AddItemToArray(arr, obj2);

    // step 2: Configure
    cJSON *dup_obj1 = cJSON_Duplicate(obj1, 1);
    cJSON_AddItemToArray(arr, dup_obj1);
    cJSON *container = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(container, "pi", pi);
    cJSON *detached_pi = cJSON_DetachItemFromObject(container, "pi");
    cJSON_AddItemToArray(arr, detached_pi);

    // step 3: Operate and Validate
    cJSON *a0 = cJSON_GetArrayItem(arr, 0);
    cJSON *a1 = cJSON_GetArrayItem(arr, 1);
    cJSON *a2 = cJSON_GetArrayItem(arr, 2);
    cJSON *a3 = cJSON_GetArrayItem(arr, 3);
    double v0 = cJSON_GetNumberValue(cJSON_GetObjectItem(a0, "value"));
    double v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(a1, "value"));
    double v2 = cJSON_GetNumberValue(cJSON_GetObjectItem(a2, "value"));
    double v3 = cJSON_GetNumberValue(a3);
    double sum = v0 + v1 + v2 + v3;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(container);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}