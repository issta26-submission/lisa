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
//<ID> 576
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
    cJSON *inner_array = cJSON_CreateStringArray(words, 2);
    cJSON *outer_array = cJSON_CreateArray();
    cJSON_AddItemToArray(outer_array, inner_array);
    cJSON_AddItemToObject(root, "list", outer_array);
    cJSON_AddFalseToObject(root, "valid");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_valid = cJSON_GetObjectItem(parsed, "valid");
    cJSON_bool valid_is_false = cJSON_IsFalse(parsed_valid);
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    cJSON *first_item = cJSON_GetArrayItem(parsed_list, 0);
    char *first_str = cJSON_GetStringValue(first_item);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;valid_is_false=%d;first=%s",
            version ? version : "null",
            (int)valid_is_false,
            first_str ? first_str : "null");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}