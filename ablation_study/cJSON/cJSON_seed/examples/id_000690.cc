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
//<ID> 690
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
    cJSON *flag_original = cJSON_CreateTrue();
    cJSON *flag_dup = cJSON_Duplicate(flag_original, 0);
    (void)version;

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "flag_dup", flag_dup);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "meta");
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed, "flag_dup");
    cJSON *live_flag = cJSON_GetObjectItem(root, "flag_dup");
    cJSON_bool equal = cJSON_Compare(parsed_flag, live_flag, 1);
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(flag_original);
    // API sequence test completed successfully
    return 66;
}