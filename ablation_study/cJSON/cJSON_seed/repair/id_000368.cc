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
//<ID> 368
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_AddObjectToObject(root, "container");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToArray(values, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(20.5));
    cJSON_AddItemToArray(values, cJSON_CreateNumber(-3.25));
    cJSON_AddItemToObject(container, "values", values);

    // step 2: Configure
    cJSON_AddNullToObject(container, "optional");
    cJSON *meta = cJSON_AddObjectToObject(container, "meta");
    cJSON_AddItemToObject(meta, "multiplier", cJSON_CreateNumber(2.0));
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItemCaseSensitive(container, "values");
    cJSON_bool is_arr = cJSON_IsArray(got_values);
    cJSON *first_item = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *mult_item = cJSON_GetObjectItemCaseSensitive(meta, "multiplier");
    double mult = cJSON_GetNumberValue(mult_item);
    double result_val = first_val * (double)is_arr * mult;
    cJSON *result = cJSON_CreateNumber(result_val);
    cJSON_AddItemToObject(root, "result", result);
    cJSON *opt_lookup = cJSON_GetObjectItemCaseSensitive(container, "optional");
    cJSON *optional_flag = cJSON_CreateBool(cJSON_IsNull(opt_lookup));
    cJSON_AddItemToObject(root, "optional_was_null", optional_flag);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}