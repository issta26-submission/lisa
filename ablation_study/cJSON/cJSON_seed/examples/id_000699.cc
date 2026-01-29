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
//<ID> 699
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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(meta, "value", num);
    cJSON *num_dup = cJSON_Duplicate(num, 0);
    cJSON_AddItemToObject(root, "value_copy", num_dup);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_dup", meta_dup);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_active = cJSON_GetObjectItemCaseSensitive(parsed, "active");
    cJSON_bool is_active = cJSON_IsTrue(parsed_active);
    (void)is_active;
    cJSON_DeleteItemFromObjectCaseSensitive(root, "active");
    cJSON *after_delete = cJSON_GetObjectItemCaseSensitive(root, "active");
    (void)after_delete;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    (void)version;
    // API sequence test completed successfully
    return 66;
}