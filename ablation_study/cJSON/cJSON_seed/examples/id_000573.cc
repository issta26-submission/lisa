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
//<ID> 573
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
    cJSON *topArray = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", topArray);
    const char *names[] = { "alice", "bob" };
    cJSON *stringArray = cJSON_CreateStringArray(names, 2);
    cJSON_AddItemToArray(topArray, stringArray);
    cJSON_AddFalseToObject(root, "flag");

    // step 2: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);

    // step 3: Validate
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    cJSON *parsed_inner = cJSON_GetArrayItem(parsed_list, 0);
    cJSON *first_name_item = cJSON_GetArrayItem(parsed_inner, 0);
    const char *first_name = cJSON_GetStringValue(first_name_item);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(parsed_flag);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;first=%s;flag_false=%d",
            version ? version : "null",
            first_name ? first_name : "null",
            (int)flag_is_false);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}