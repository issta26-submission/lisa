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
//<ID> 1018
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"user\":{\"name\":\"alice\",\"active\":null,\"roles\":[\"admin\",\"user\"]}}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *user = cJSON_GetObjectItem(root, "user");
    cJSON *name = cJSON_GetObjectItem(user, "name");
    cJSON *active = cJSON_GetObjectItem(user, "active");
    cJSON *roles = cJSON_GetObjectItem(user, "roles");
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_copy = cJSON_CreateString(cJSON_GetStringValue(name));
    cJSON_AddItemToObject(meta, "username", name_copy);
    cJSON_bool active_was_null = cJSON_IsNull(active);
    cJSON_AddBoolToObject(meta, "active_was_null", active_was_null);
    cJSON_bool roles_are_array = cJSON_IsArray(roles);
    cJSON_AddBoolToObject(meta, "roles_are_array", roles_are_array);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    int roles_count = cJSON_GetArraySize(roles);
    cJSON_AddNumberToObject(root, "roles_count", roles_count);
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(user, "active");
    cJSON_DeleteItemFromObject(root, "meta");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}