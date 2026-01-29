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
//<ID> 605
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
    const char *const names_list[] = { "alice", "bob", "carol" };
    cJSON *names = cJSON_CreateStringArray(names_list, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag_true);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", flag_false);

    // step 2: Configure
    int original_count = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(root, "count", (double)original_count);
    cJSON_AddStringToObject(root, "owner", "tester");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_names = cJSON_HasObjectItem(parsed, "names");
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *first_item = cJSON_GetArrayItem(parsed_names, 0);
    const char *first_name = cJSON_GetStringValue(first_item);
    cJSON *parsed_active = cJSON_GetObjectItem(parsed, "active");
    cJSON *parsed_disabled = cJSON_GetObjectItem(parsed, "disabled");
    cJSON_bool active_is_true = cJSON_IsTrue(parsed_active);
    cJSON_bool disabled_is_false = cJSON_IsFalse(parsed_disabled);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has=%d;count=%d;first=%s;active=%d;disabled=%d",
            version ? version : "null",
            (int)has_names,
            original_count,
            first_name ? first_name : "null",
            (int)active_is_true,
            (int)disabled_is_false);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}