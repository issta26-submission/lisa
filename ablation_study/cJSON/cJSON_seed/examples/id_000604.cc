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
//<ID> 604
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
    static const char *colors_arr[] = { "red", "green", "blue" };
    cJSON *colors = cJSON_CreateStringArray(colors_arr, 3);
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_true);
    cJSON_AddItemToObject(root, "disabled", flag_false);

    // step 2: Configure
    int original_count = cJSON_GetArraySize(colors);
    cJSON_AddNumberToObject(root, "count", (double)original_count);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_colors = cJSON_HasObjectItem(parsed, "colors");
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed, "enabled");
    cJSON *parsed_disabled = cJSON_GetObjectItem(parsed, "disabled");
    cJSON_bool is_enabled_true = cJSON_IsTrue(parsed_enabled);
    cJSON_bool is_disabled_false = cJSON_IsFalse(parsed_disabled);
    cJSON *parsed_colors = cJSON_GetObjectItem(parsed, "colors");
    int parsed_count = cJSON_GetArraySize(parsed_colors);
    cJSON_bool counts_match = (parsed_count == original_count);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has=%d;true=%d;false=%d;orig=%d;parsed=%d;match=%d",
            version ? version : "null",
            (int)has_colors,
            (int)is_enabled_true,
            (int)is_disabled_false,
            original_count,
            parsed_count,
            (int)counts_match);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}