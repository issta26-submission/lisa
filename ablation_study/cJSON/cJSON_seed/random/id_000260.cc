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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *float_arr2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *elem2 = (cJSON *)0;
    cJSON_bool equal = (cJSON_bool)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double total = 0.0;
    char *printed = (char *)0;

    // step 2: Initialize
    float values[3];
    values[0] = 1.1f;
    values[1] = 2.2f;
    values[2] = 3.3f;
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    float_arr = cJSON_CreateFloatArray(values, 3);
    float_arr2 = cJSON_CreateFloatArray(values, 3);

    // step 3: Configure
    cJSON_AddItemToArray(arr, float_arr);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToObject(root, "numbers_copy", float_arr2);

    // step 4: Operate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "numbers_copy");
    retrieved_arr = cJSON_GetObjectItem(root, "numbers");
    elem0 = cJSON_GetArrayItem(retrieved_arr, 0);
    elem1 = cJSON_GetArrayItem(retrieved_arr, 1);
    elem2 = cJSON_GetArrayItem(retrieved_arr, 2);
    v0 = cJSON_GetNumberValue(elem0);
    v1 = cJSON_GetNumberValue(elem1);
    v2 = cJSON_GetNumberValue(elem2);
    total = v0 + v1 + v2;
    cJSON_AddNumberToObject(root, "sum", total);
    equal = cJSON_Compare(detached, float_arr, (cJSON_bool)1);
    cJSON_AddBoolToObject(root, "detached_equals_embedded", equal);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}