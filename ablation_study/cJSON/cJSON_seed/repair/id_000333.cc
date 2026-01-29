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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[1,2],\"obj\":{\"a\":1},\"name\":\"device\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");

    // step 2: Configure
    cJSON *replacement_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(replacement_obj, "b", 42.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "obj", replacement_obj);
    cJSON *new_string_elem = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, new_string_elem);

    // step 3: Operate and Validate
    cJSON *third_elem = cJSON_GetArrayItem(arr, 2);
    const char *third_str = cJSON_GetStringValue(third_elem);
    cJSON *third_copy = cJSON_CreateString(third_str);
    cJSON_AddItemToObject(root, "third_str_copy", third_copy);
    char *out = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}