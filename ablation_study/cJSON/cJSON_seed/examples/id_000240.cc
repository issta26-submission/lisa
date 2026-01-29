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
//<ID> 240
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
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON *counter = cJSON_CreateNumber(0.0);

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "counter", counter);
    cJSON_SetNumberHelper(counter, 7.0);
    cJSON_SetNumberHelper(counter, (double)cJSON_GetArraySize(arr));

    // step 3: Operate & Validate
    double v1 = cJSON_GetNumberValue(num1);
    double v2 = cJSON_GetNumberValue(num2);
    cJSON *sum = cJSON_CreateNumber(v1 + v2);
    cJSON_AddItemToArray(arr, sum);
    cJSON_SetNumberHelper(counter, v1 + v2);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}