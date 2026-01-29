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
//<ID> 817
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
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "enabled", flag_true);
    cJSON_AddItemToObject(root, "active", flag_false);

    // step 2: Configure
    cJSON_AddNullToObject(root, "missing");
    cJSON_AddItemToArray(names_array, cJSON_CreateString("delta"));
    cJSON *second_item = cJSON_GetArrayItem(names_array, 1);
    const char *second_value = cJSON_GetStringValue(second_item);
    cJSON *copied_second = cJSON_CreateString(second_value);
    cJSON_AddItemToObject(root, "copied_name", copied_second);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *parsed_second = cJSON_GetArrayItem(parsed_names, 1);
    const char *parsed_second_value = cJSON_GetStringValue(parsed_second);
    cJSON *reextracted = cJSON_CreateString(parsed_second_value);
    cJSON_AddItemToObject(parsed, "reextracted", reextracted);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}