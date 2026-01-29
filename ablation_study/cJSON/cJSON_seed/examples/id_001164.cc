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
//<ID> 1164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[4,5],\"flag\":false}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();

    // step 2: Configure
    cJSON *parsed_numbers = cJSON_GetObjectItem(parsed, "numbers");
    cJSON *p0 = cJSON_GetArrayItem(parsed_numbers, 0);
    cJSON *p1 = cJSON_GetArrayItem(parsed_numbers, 1);
    double v0 = cJSON_GetNumberValue(p0);
    double v1 = cJSON_GetNumberValue(p1);
    cJSON *n0 = cJSON_CreateNumber(v0 + 1.0);
    cJSON *n1 = cJSON_CreateNumber(v1 + 2.0);
    cJSON_AddItemToArray(created_arr, n0);
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToObject(root, "created", created_arr);

    // step 3: Operate & Validate
    cJSON_bool has_numbers = cJSON_HasObjectItem(parsed, "numbers");
    double presence_val = (double)has_numbers * (v0 + v1);
    cJSON_AddNumberToObject(root, "sum_if_present", presence_val);
    cJSON *flag_item = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool is_false = cJSON_IsFalse(flag_item);
    cJSON_AddNumberToObject(root, "flag_is_false", (double)is_false);
    cJSON *dup_parsed = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "parsed_dup", dup_parsed);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}