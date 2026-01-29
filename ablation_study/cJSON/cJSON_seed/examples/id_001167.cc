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
//<ID> 1167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"device\",\"enabled\":false,\"values\":[1,2,3]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_array = cJSON_CreateArray();
    cJSON *n10 = cJSON_CreateNumber(10.0);
    cJSON *n20 = cJSON_CreateNumber(20.0);

    // step 2: Configure
    cJSON_AddItemToArray(created_array, n10);
    cJSON_AddItemToArray(created_array, n20);
    cJSON_AddItemToObject(root, "created", created_array);
    cJSON_AddNumberToObject(root, "threshold", 5.5);

    // step 3: Operate & Validate
    cJSON_bool has_enabled = cJSON_HasObjectItem(parsed, "enabled");
    cJSON *enabled_item = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool enabled_is_false = cJSON_IsFalse(enabled_item);
    double enabled_flag = 1.0 - (double)enabled_is_false; // 1.0 if not false, 0.0 if false
    cJSON_AddNumberToObject(root, "enabled_flag", enabled_flag);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    cJSON *first_val_item = cJSON_GetArrayItem(parsed_values, 0);
    double first_val = cJSON_GetNumberValue(first_val_item);
    cJSON *augmented = cJSON_CreateNumber(first_val + 100.0);
    cJSON_AddItemToArray(created_array, augmented);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}