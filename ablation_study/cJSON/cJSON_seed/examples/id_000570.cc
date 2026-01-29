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
//<ID> 570
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
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *strArray = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToArray(arr, strArray);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *flag_item = cJSON_AddFalseToObject(root, "flag");

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");

    // step 3: Operate & Validate
    cJSON_bool flag_is_false = cJSON_IsFalse(parsed_flag);
    cJSON *first_item = cJSON_GetArrayItem(parsed_list, 0);
    const char *first_str = cJSON_GetStringValue(first_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;flag=%d;first=%s",
            version ? version : "null",
            (int)flag_is_false,
            first_str ? first_str : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}