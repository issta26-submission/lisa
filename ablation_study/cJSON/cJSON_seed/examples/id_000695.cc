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
//<ID> 695
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

    // step 2: Configure
    cJSON *enabled = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *root_dup = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_enabled = cJSON_GetObjectItemCaseSensitive(parsed, "enabled");
    cJSON_bool enabled_true = cJSON_IsTrue(parsed_enabled);
    cJSON *enabled_dup = cJSON_Duplicate(parsed_enabled, 0);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "enabled");
    (void)version;
    (void)enabled;
    (void)enabled_true;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(root_dup);
    cJSON_Delete(enabled_dup);
    // API sequence test completed successfully
    return 66;
}