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
//<ID> 812
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names_arr[] = {"alpha", "beta", "gamma"};
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);
    cJSON *flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddNullToObject(root, "maybe_null");

    // step 2: Configure
    const char *aliases_arr[] = {"a1", "b2"};
    cJSON *aliases = cJSON_CreateStringArray(aliases_arr, 2);
    cJSON_AddItemToObject(root, "aliases", aliases);
    cJSON *first_name_item = cJSON_GetArrayItem(names, 0);
    const char *first_name = cJSON_GetStringValue(first_name_item);
    cJSON *primary = cJSON_CreateString(first_name);
    cJSON_AddItemToObject(root, "primary", primary);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *second_item = cJSON_GetArrayItem(parsed_names, 1);
    const char *second_name = cJSON_GetStringValue(second_item);
    cJSON *second_copy = cJSON_CreateString(second_name);
    cJSON_AddItemToObject(parsed, "second_copy", second_copy);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}