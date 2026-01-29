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
//<ID> 1184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"alpha\":true,\"items\":[3,9],\"meta\":null}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(100.0);
    cJSON *n2 = cJSON_CreateNumber(200.0);

    // step 2: Configure
    cJSON_AddItemToArray(created_arr, n1);
    cJSON_AddItemToArray(created_arr, n2);
    cJSON_AddItemToObject(root, "created", created_arr);
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON *child_old = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "replaceme", child_old);
    cJSON *child_new = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemViaPointer(root, child_old, child_new);

    // step 3: Operate & Validate
    cJSON *arr_item = cJSON_GetArrayItem(created_arr, 1);
    double arr_val = cJSON_GetNumberValue(arr_item);
    cJSON_AddNumberToObject(root, "copied", arr_val);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *parsed_second = cJSON_GetArrayItem(parsed_items, 1);
    double parsed_val = cJSON_GetNumberValue(parsed_second);
    cJSON_AddNumberToObject(root, "parsed_item1", parsed_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}