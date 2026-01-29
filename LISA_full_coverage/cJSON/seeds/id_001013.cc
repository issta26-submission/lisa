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
//<ID> 1013
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"user\":{\"name\":\"Alice\",\"active\":false,\"meta\":null},\"count\":3}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);

    // step 2: Configure
    cJSON *user = cJSON_GetObjectItem(root, "user");
    cJSON *meta = cJSON_GetObjectItem(user, "meta");
    cJSON_bool meta_is_null = cJSON_IsNull(meta);
    cJSON *summary = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(summary, "greeting", greeting);
    cJSON_AddBoolToObject(summary, "meta_was_null", meta_is_null);
    cJSON_AddItemToObject(root, "summary", summary);
    cJSON_AddBoolToObject(root, "user_active_flag", cJSON_IsTrue(cJSON_GetObjectItem(user, "active")));

    // step 3: Operate
    char *compact = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(256);
    memset(print_buf, 0, 256);
    cJSON_PrintPreallocated(root, print_buf, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "count");
    cJSON_free(compact);
    cJSON_free(print_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}