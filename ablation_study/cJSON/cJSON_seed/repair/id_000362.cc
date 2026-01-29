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
//<ID> 362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);

    // step 2: Configure
    cJSON_AddNullToObject(config, "maybe");
    cJSON *inner = cJSON_AddObjectToObject(config, "inner");
    cJSON *sub = cJSON_CreateArray();
    cJSON_AddItemToObject(inner, "sub", sub);
    cJSON_AddItemToArray(sub, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(sub, cJSON_CreateString("beta"));

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON_bool is_arr = cJSON_IsArray(got_values);
    cJSON_AddItemToObject(root, "values_is_array", cJSON_CreateBool(is_arr));
    cJSON *got_inner = cJSON_GetObjectItemCaseSensitive(config, "inner");
    cJSON *got_sub = cJSON_GetObjectItemCaseSensitive(got_inner, "sub");
    cJSON *first = cJSON_GetArrayItem(got_sub, 0);
    const char *first_str = cJSON_GetStringValue(first);
    cJSON_AddItemToObject(root, "first_label", cJSON_CreateString(first_str ? first_str : ""));
    double sum = cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2) + (double)is_arr;
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(sum));

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}