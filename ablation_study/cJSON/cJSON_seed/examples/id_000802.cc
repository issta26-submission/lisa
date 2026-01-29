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
//<ID> 802
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *version_str = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_str);

    // step 2: Configure
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON_AddTrueToObject(root, "stable");

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_stable = cJSON_GetObjectItem(parsed, "stable");
    cJSON_bool stable_flag = cJSON_IsTrue(parsed_stable);
    cJSON *parsed_version = cJSON_GetObjectItem(parsed, "version");
    char *parsed_version_str = cJSON_GetStringValue(parsed_version);
    cJSON *parsed_dup = cJSON_Duplicate(parsed, 1);
    cJSON_bool equal = cJSON_Compare(parsed, parsed_dup, 1);
    (void)stable_flag;
    (void)parsed_version_str;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(parsed_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}