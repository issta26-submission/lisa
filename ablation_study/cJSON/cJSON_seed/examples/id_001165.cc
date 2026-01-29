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
//<ID> 1165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":false,\"values\":[3,7]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, n0);
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToObject(root, "new_values", created_arr);
    cJSON_AddNumberToObject(root, "constant", 42.0);

    // step 3: Operate & Validate
    cJSON_bool has_vals = cJSON_HasObjectItem(parsed, "values");
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    cJSON *val0 = cJSON_GetArrayItem(values, 0);
    cJSON *val1 = cJSON_GetArrayItem(values, 1);
    double sum_vals = cJSON_GetNumberValue(val0) + cJSON_GetNumberValue(val1);
    cJSON_AddNumberToObject(root, "sum_parsed", sum_vals);
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_false = cJSON_IsFalse(flag_item);
    cJSON_AddNumberToObject(root, "flag_is_false", (double)flag_false);
    cJSON_AddNumberToObject(root, "has_values", (double)has_vals);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}