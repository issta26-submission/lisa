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
//<ID> 1119
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
    cJSON *num0 = cJSON_CreateNumber(10.0);
    cJSON *num1 = cJSON_CreateNumber(20.0);
    cJSON *label_ref = cJSON_CreateStringReference("example");

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Operate & Validate
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    double prev0 = cJSON_SetNumberHelper(it0, 15.5);
    double prev1 = cJSON_SetNumberHelper(it1, 25.5);
    cJSON_bool is_num0 = cJSON_IsNumber(it0);
    cJSON_bool is_num1 = cJSON_IsNumber(it1);
    double computed = prev0 + prev1 + (double)is_num0 + (double)is_num1;
    cJSON_AddNumberToObject(root, "computed_sum", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}