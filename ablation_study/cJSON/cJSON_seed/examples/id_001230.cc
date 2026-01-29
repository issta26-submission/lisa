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
//<ID> 1230
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":2.5,\"nested\":null}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    double doubles_arr[3] = {4.0, 5.5, 6.75};
    cJSON *double_array = cJSON_CreateDoubleArray(doubles_arr, 3);
    cJSON *empty_array = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "imported", double_array);
    cJSON_AddItemToObject(root, "empty", empty_array);
    cJSON_AddItemToObject(root, "nil", null_item);
    cJSON *alpha = cJSON_GetObjectItem(parsed, "alpha");
    double alpha_val = cJSON_GetNumberValue(alpha);
    cJSON *first = cJSON_GetArrayItem(double_array, 0);
    double first_val = cJSON_GetNumberValue(first);
    double sum = alpha_val + first_val;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 3: Operate & Validate
    cJSON_bool is_null = cJSON_IsNull(null_item);
    cJSON *nil_flag = cJSON_CreateBool(is_null);
    cJSON_AddItemToObject(root, "nil_is_null", nil_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}