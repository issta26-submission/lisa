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
//<ID> 1163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"enabled\":false,\"values\":[2,4]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.5);
    cJSON *n2 = cJSON_CreateNumber(-3.25);

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, n0);
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToObject(root, "numbers", created_arr);
    cJSON_AddNumberToObject(root, "constant", 3.1415);

    // step 3: Operate & Validate
    cJSON *enabled_item = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool is_false = cJSON_IsFalse(enabled_item);
    cJSON_AddNumberToObject(root, "enabled_is_false", (double)is_false);
    cJSON_bool has_values = cJSON_HasObjectItem(parsed, "values");
    cJSON_AddNumberToObject(root, "parsed_has_values", (double)has_values);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}