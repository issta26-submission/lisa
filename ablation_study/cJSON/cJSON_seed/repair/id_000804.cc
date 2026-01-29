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
//<ID> 804
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *str_ref0 = cJSON_CreateStringReference("hello");
    cJSON_AddItemToArray(root, str_ref0);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(root, false_item);

    // step 2: Configure
    const char *json_text = "{\"keep\":\"value\",\"remove_me\":\"temp\",\"nested\":{\"k\":1}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *keep_item = cJSON_GetObjectItem(parsed, "keep");
    const char *keep_value = cJSON_GetStringValue(keep_item);
    cJSON *imported_keep_ref = cJSON_CreateStringReference(keep_value);
    cJSON_AddItemToArray(root, imported_keep_ref);
    cJSON *another_ref = cJSON_CreateStringReference("imported");
    cJSON_AddItemToArray(root, another_ref);

    // step 3: Operate and Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "remove_me");
    cJSON_bool root_is_array = cJSON_IsArray(root);
    cJSON *first_item = cJSON_GetArrayItem(root, 0);
    cJSON_bool first_is_string = cJSON_IsString(first_item);
    (void)root_is_array;
    (void)first_is_string;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}