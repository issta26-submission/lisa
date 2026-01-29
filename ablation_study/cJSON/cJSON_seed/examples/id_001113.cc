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
//<ID> 1113
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
    cJSON *num0 = cJSON_CreateNumber(1.5);
    cJSON *num1 = cJSON_CreateNumber(2.5);
    cJSON *sref = cJSON_CreateStringReference("example");

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "title", sref);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate & Validate
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    double set_ret = cJSON_SetNumberHelper(it0, 3.14159);
    cJSON_bool is_num = cJSON_IsNumber(it0);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}