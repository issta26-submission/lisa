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
//<ID> 571
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
    const char *names[] = {"alice", "bob", "carol"};
    cJSON *names_array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_array);
    cJSON *outer = cJSON_CreateArray();
    cJSON_AddItemToArray(outer, cJSON_CreateString("marker"));
    cJSON_AddItemToObject(root, "outer", outer);
    cJSON_AddFalseToObject(root, "active");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_active = cJSON_GetObjectItem(parsed, "active");
    cJSON_bool active_is_false = cJSON_IsFalse(parsed_active);
    cJSON *parsed_names = cJSON_GetObjectItem(parsed, "names");
    cJSON *first_name_item = cJSON_GetArrayItem(parsed_names, 0);
    char *first_name = cJSON_GetStringValue(first_name_item);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;active=%d;first=%s",
            version ? version : "null",
            active_is_false ? 1 : 0,
            first_name ? first_name : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}