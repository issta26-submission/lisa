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
//<ID> 263
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
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *retrieved_extra = (cJSON *)0;
    cJSON *first_from_detached = (cJSON *)0;
    cJSON *first_from_extra = (cJSON *)0;
    cJSON *sum_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool cmp_result = (cJSON_bool)0;
    double v_det = 0.0;
    double v_extra = 0.0;
    double sum = 0.0;
    float values[3] = {1.1f, 2.2f, 3.3f};

    // step 2: Initialize
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(values, 3);

    // step 3: Configure
    cJSON_AddItemToObject(root, "values", float_arr);
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, "extra", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "values");
    cmp_result = cJSON_Compare((const cJSON *const)detached, (const cJSON *const)float_arr, (cJSON_bool)1);
    first_from_detached = cJSON_GetArrayItem(detached, 0);
    v_det = cJSON_GetNumberValue(first_from_detached);
    retrieved_extra = cJSON_GetObjectItem(root, "extra");
    first_from_extra = cJSON_GetArrayItem(retrieved_extra, 0);
    v_extra = cJSON_GetNumberValue(first_from_extra);
    sum = v_det + v_extra;
    sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}