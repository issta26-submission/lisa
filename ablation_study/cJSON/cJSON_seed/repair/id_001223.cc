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
//<ID> 1223
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
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", str);
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *label = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "label", label);
    cJSON *extra = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    int is_str_flag = (int)cJSON_IsString(str);
    cJSON *it0 = cJSON_GetArrayItem(arr, 0);
    cJSON *it1 = cJSON_GetArrayItem(arr, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double computed = v0 + v1 + (double)is_str_flag + cJSON_GetNumberValue(extra);
    cJSON *total = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "total", total);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}