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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *detached_arr = (cJSON *)0;
    cJSON *extra_num = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    const float numbers[4] = {1.5f, 2.25f, 3.75f, 0.0f};
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double v3 = 0.0;
    double sum = 0.0;
    char *printed = (char *)0;
    cJSON_bool cmp_result = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(numbers, 3);
    cJSON_AddItemToObject(root, "floats", float_arr);

    // step 3: Configure
    detached_arr = cJSON_DetachItemFromObjectCaseSensitive(root, "floats");
    extra_num = cJSON_CreateNumber(4.25);
    cJSON_AddItemToArray(detached_arr, extra_num);

    // step 4: Operate
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(detached_arr, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(detached_arr, 1));
    v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(detached_arr, 2));
    v3 = cJSON_GetNumberValue(cJSON_GetArrayItem(detached_arr, 3));
    sum = v0 + v1 + v2 + v3;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 5: Validate
    cmp_result = cJSON_Compare(detached_arr, float_arr, 1);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_arr);

    // API sequence test completed successfully
    return 66;
}