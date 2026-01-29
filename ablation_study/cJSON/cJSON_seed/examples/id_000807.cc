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
//<ID> 807
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *id_ref = cJSON_CreateStringReference("device-123");
    cJSON_AddItemToObject(root, "id", id_ref);
    cJSON_AddTrueToObject(root, "enabled");

    // step 2: Configure
    const char *version_text = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_text);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON_AddStringToObject(meta, "owner", "team-x");

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_version = cJSON_GetObjectItemCaseSensitive(parsed, "version");
    const char *extracted_version = cJSON_GetStringValue(parsed_version);
    cJSON *version_copy = cJSON_CreateString(extracted_version);
    cJSON_AddItemToObject(parsed, "version_copy", version_copy);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}