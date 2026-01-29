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
//<ID> 1168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":false,\"values\":[1,2.5]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *num_a = cJSON_CreateNumber(10.0);
    cJSON *num_b = cJSON_CreateNumber(20.0);

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, num_a);
    cJSON_AddItemToArray(created_arr, num_b);
    cJSON_AddItemToObject(root, "created_values", created_arr);
    cJSON_AddNumberToObject(root, "added_number_direct", 3.1415);
    cJSON_bool has_values = cJSON_HasObjectItem(parsed, "values");
    cJSON_AddNumberToObject(root, "parsed_has_values", (double)has_values);
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    cJSON_AddNumberToObject(root, "flag_is_false", (double)flag_is_false);

    // step 3: Operate & Validate
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    cJSON *pv0 = cJSON_GetArrayItem(parsed_values, 0);
    cJSON *pv1 = cJSON_GetArrayItem(parsed_values, 1);
    double v0 = cJSON_GetNumberValue(pv0);
    double v1 = cJSON_GetNumberValue(pv1);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "parsed_values_sum", sum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    return 66; // API sequence test completed successfully
}