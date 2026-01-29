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
//<ID> 814
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names[] = { "alpha", "beta", "gamma" };
    cJSON *names_array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_array);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active);
    cJSON *enabled = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_AddNullToObject(root, "maybe_null");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *second_item = cJSON_GetArrayItem(names_array, 1);
    const char *second_str = cJSON_GetStringValue(second_item);
    cJSON *copied_second = cJSON_CreateString(second_str);
    cJSON_AddItemToObject(meta, "second_name", copied_second);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *parsed_first = cJSON_GetArrayItem(parsed_names, 0);
    const char *first_val = cJSON_GetStringValue(parsed_first);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(parsed, "first_copy", first_copy);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}