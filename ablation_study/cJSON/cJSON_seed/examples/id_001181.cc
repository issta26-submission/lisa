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
//<ID> 1181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"flag\":true,\"data\":[1,2,3],\"note\":null}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToObject(root, "created", created_arr);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddNullToObject(root, "missing");

    // step 3: Operate & Validate
    cJSON *parsed_arr = cJSON_GetObjectItem(parsed, "data");
    cJSON *second_item = cJSON_GetArrayItem(parsed_arr, 1);
    cJSON *replacement = cJSON_CreateNumber(999.0);
    cJSON_ReplaceItemViaPointer(parsed_arr, second_item, replacement);
    cJSON *new_second = cJSON_GetArrayItem(parsed_arr, 1);
    double replaced_value = cJSON_GetNumberValue(new_second);
    cJSON_AddNumberToObject(root, "replaced_value", replaced_value);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}