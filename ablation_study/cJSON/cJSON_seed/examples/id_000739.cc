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
//<ID> 739
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"users\":[\"alice\",\"bob\",\"carol\"],\"meta\":{\"count\":3}}";
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *root = cJSON_CreateObject();
    cJSON *new_array = cJSON_CreateArray();

    // step 2: Configure
    cJSON *users = cJSON_GetObjectItem(parsed, "users");
    cJSON *first_user = cJSON_GetArrayItem(users, 0);
    const char *first_name = cJSON_GetStringValue(first_user);
    cJSON *ref_from_parsed = cJSON_CreateStringReference(first_name);
    cJSON *ref_literal = cJSON_CreateStringReference("dave");
    cJSON_AddItemToArray(new_array, ref_from_parsed);
    cJSON_AddItemToArray(new_array, ref_literal);
    cJSON_AddItemToObject(root, "new_users", new_array);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *reparsed = cJSON_Parse(buffer);
    cJSON *found = cJSON_GetObjectItem(reparsed, "new_users");
    cJSON *first_reparsed = cJSON_GetArrayItem(found, 0);
    const char *validated_name = cJSON_GetStringValue(first_reparsed);
    (void)validated_name;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(reparsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}