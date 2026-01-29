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
//<ID> 805
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    const char *version_str = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_str);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *id_ref = cJSON_CreateStringReference("device-123");
    cJSON_AddItemToObject(root, "id", id_ref);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *owner_ref = cJSON_CreateStringReference("team-a");
    cJSON_AddItemToObject(meta, "owner", owner_ref);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *version_item = cJSON_GetObjectItemCaseSensitive(root, "version");
    char *extracted_version = cJSON_GetStringValue(version_item);
    cJSON *enabled_item = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_true = cJSON_IsTrue(enabled_item);
    (void)extracted_version;
    (void)enabled_is_true;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}