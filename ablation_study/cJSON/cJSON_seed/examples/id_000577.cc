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
//<ID> 577
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    const char *strings[] = { "one", "two", "three" };
    cJSON *string_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToArray(arr, string_array);
    cJSON *extra_str = cJSON_CreateString("extra");
    cJSON_AddItemToArray(arr, extra_str);
    cJSON_AddFalseToObject(root, "flag");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(parsed_flag);
    cJSON *parsed_data = cJSON_GetObjectItem(parsed, "data");
    cJSON *first_item = cJSON_GetArrayItem(parsed_data, 0);
    cJSON *nested_first = cJSON_GetArrayItem(first_item, 0);
    char *first_str = (char*)cJSON_GetStringValue(nested_first);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;flag_is_false=%d;first=%s",
            version ? version : "null",
            (int)flag_is_false,
            first_str ? first_str : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}