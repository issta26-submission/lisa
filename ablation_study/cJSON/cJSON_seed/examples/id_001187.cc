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
//<ID> 1187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[10,20],\"flag\":false}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_arr = cJSON_CreateArray();
    cJSON *temp_str = cJSON_CreateString("old");
    cJSON_AddItemToObject(root, "temp", temp_str);
    cJSON_AddItemToObject(root, "arr", created_arr);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(parsed, "items");
    cJSON *second = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *copied_num = cJSON_CreateNumber(second_val);
    cJSON_AddItemToArray(created_arr, copied_num);
    cJSON_AddBoolToObject(root, "new_flag", 1);
    cJSON_AddNullToObject(root, "maybe_null");

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("new");
    cJSON *temp_ptr = cJSON_GetObjectItem(root, "temp");
    cJSON_ReplaceItemViaPointer(root, temp_ptr, replacement);
    cJSON *first_in_arr = cJSON_GetArrayItem(created_arr, 0);
    double first_val = cJSON_GetNumberValue(first_in_arr);
    char *out = cJSON_PrintUnformatted(root);
    (void)first_val;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}