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
//<ID> 574
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

    // step 2: Configure
    const char *words[] = { "alpha", "beta" };
    cJSON *string_array = cJSON_CreateStringArray(words, 2);
    cJSON *extra_array = cJSON_CreateArray();
    cJSON *gamma = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(extra_array, gamma);
    cJSON_AddItemToObject(root, "string_list", string_array);
    cJSON_AddItemToObject(root, "extra_list", extra_array);
    cJSON_AddFalseToObject(root, "flag");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_strings = cJSON_GetObjectItem(parsed, "string_list");
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(parsed_flag);
    cJSON *first_item = cJSON_GetArrayItem(parsed_strings, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;first=%s;flag=%d",
            version ? version : "null",
            first_value ? first_value : "null",
            (int)flag_is_false);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}