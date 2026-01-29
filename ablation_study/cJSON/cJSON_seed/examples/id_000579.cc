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
//<ID> 579
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
    const char *names_arr[] = { "alice", "bob", "carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *list = cJSON_CreateArray();
    cJSON *entry1 = cJSON_CreateString("first");
    cJSON_AddItemToArray(list, entry1);
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON *enabled = cJSON_AddFalseToObject(root, "enabled");
    cJSON_bool is_false_original = cJSON_IsFalse(enabled);
    cJSON *entry2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(list, entry2);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool is_false_parsed = cJSON_IsFalse(parsed_enabled);
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *first_name_item = cJSON_GetArrayItem(parsed_names, 0);
    const char *first_name = cJSON_GetStringValue(first_name_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;orig_false=%d;parsed_false=%d;name=%s",
            version ? version : "null",
            (int)is_false_original,
            (int)is_false_parsed,
            first_name ? first_name : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}