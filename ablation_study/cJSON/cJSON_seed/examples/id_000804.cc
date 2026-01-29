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
//<ID> 804
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *version = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version);

    // step 2: Configure
    cJSON *enabled_item = cJSON_AddTrueToObject(root, "enabled");
    cJSON_AddItemToObject(root, "version", version_ref);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_version = cJSON_GetObjectItem(parsed, "version");
    const char *extracted_version = cJSON_GetStringValue(parsed_version);
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool enabled_check = cJSON_IsTrue(parsed_enabled);
    (void)extracted_version;
    (void)enabled_check;
    (void)enabled_item;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}