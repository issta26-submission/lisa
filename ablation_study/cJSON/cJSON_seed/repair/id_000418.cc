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
//<ID> 418
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_Parse("{\"data\":[]}");
    cJSON *arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_Parse("10");
    cJSON *n1 = cJSON_Parse("20");
    cJSON *n2 = cJSON_Parse("5.5");
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON_SetNumberHelper(n1, 25.0);
    cJSON *extra = cJSON_Parse("100");
    cJSON_AddItemToArray(arr, extra);

    // step 3: Operate
    cJSON *i0 = cJSON_GetArrayItem(arr, 0);
    cJSON *i1 = cJSON_GetArrayItem(arr, 1);
    cJSON *i2 = cJSON_GetArrayItem(arr, 2);
    cJSON *i3 = cJSON_GetArrayItem(arr, 3);
    double v0 = cJSON_GetNumberValue(i0);
    double v1 = cJSON_GetNumberValue(i1);
    double v2 = cJSON_GetNumberValue(i2);
    double v3 = cJSON_GetNumberValue(i3);
    double sum = v0 + v1 + v2 + v3;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}