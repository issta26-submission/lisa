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
//<ID> 742
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
    cJSON *users = cJSON_CreateArray();
    cJSON *u1 = cJSON_CreateString("alice");
    cJSON *u2 = cJSON_CreateString("bob");
    cJSON *u_null = cJSON_CreateNull();
    cJSON *meta = cJSON_CreateObject();
    (void)version;

    // step 2: Configure
    cJSON_AddItemToArray(users, u1);
    cJSON_AddItemToArray(users, u2);
    cJSON_AddItemToArray(users, u_null);
    cJSON_AddItemToObject(root, "users", users);
    cJSON_AddNumberToObject(meta, "count", 3);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "title", "example_title");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *meta_parsed = cJSON_GetObjectItem(parsed, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta_parsed, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *users_parsed = cJSON_GetObjectItem(parsed, "users");
    cJSON *detached_users = cJSON_DetachItemFromObjectCaseSensitive(parsed, "users");
    cJSON *first_user = cJSON_GetArrayItem(detached_users, 0);
    const char *first_name = cJSON_GetStringValue(first_user);
    (void)count_val;
    (void)first_name;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(detached_users);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}