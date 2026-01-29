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
//<ID> 800
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *version_literal = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_literal);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON_AddTrueToObject(root, "active");
    cJSON *meta = cJSON_CreateObject();
    cJSON *name_ref = cJSON_CreateStringReference("device-xyz");
    cJSON_AddItemToObject(meta, "name", name_ref);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Operate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);

    // step 3: Validate
    cJSON *parsed_version = cJSON_GetObjectItem(parsed, "version");
    char *parsed_version_str = cJSON_GetStringValue(parsed_version);
    cJSON *parsed_active = cJSON_GetObjectItem(parsed, "active");
    cJSON_bool active_flag = cJSON_IsTrue(parsed_active);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_meta, "name");
    char *parsed_name_str = cJSON_GetStringValue(parsed_name);
    (void)parsed_version_str;
    (void)active_flag;
    (void)parsed_name_str;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}