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
//<ID> 1169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"status\":false,\"value\":42,\"items\":[5,7]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(100.0);
    cJSON *n2 = cJSON_CreateNumber(200.0);

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToObject(root, "created", created_arr);
    cJSON_bool has_value = cJSON_HasObjectItem(parsed, "value");
    cJSON *value_item = cJSON_GetObjectItem(parsed, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    cJSON_AddNumberToObject(root, "value_copy", value_num);

    // step 3: Operate & Validate
    cJSON *status_item = cJSON_GetObjectItem(parsed, "status");
    cJSON_bool status_is_false = cJSON_IsFalse(status_item);
    cJSON_AddNumberToObject(root, "status_is_false", (double)status_is_false);
    cJSON *vnum = cJSON_CreateNumber(value_num);
    cJSON_AddItemToArray(created_arr, vnum);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}