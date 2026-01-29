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
//<ID> 696
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
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddBoolToObject(meta, "flag", 0);

    // step 2: Configure
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_dup);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *parsed_meta_copy = cJSON_GetObjectItemCaseSensitive(parsed, "meta_copy");
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "meta_copy");
    cJSON *after_delete = cJSON_GetObjectItemCaseSensitive(parsed, "meta_copy");
    (void)version;
    (void)len;
    (void)parsed_meta;
    (void)parsed_meta_copy;
    (void)after_delete;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}