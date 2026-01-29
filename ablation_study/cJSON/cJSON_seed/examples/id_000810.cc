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
//<ID> 810
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names[] = { "alice", "bob", "carol" };
    cJSON *names_array = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_array);
    cJSON *active_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active_true);
    cJSON *flag_bool = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "flag", flag_bool);
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *owner_str = cJSON_CreateString("team-x");
    cJSON_AddItemToObject(meta, "owner", owner_str);

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
    char *second_name = cJSON_GetStringValue(second_item);
    cJSON *copied_name = cJSON_CreateString(second_name);
    cJSON_AddItemToObject(parsed, "second_name_copy", copied_name);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}