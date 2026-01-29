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
//<ID> 803
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
    cJSON *active_item = cJSON_AddTrueToObject(root, "active");
    cJSON_AddItemToObject(root, "version", version_ref);

    // step 2: Configure
    cJSON *id_ref = cJSON_CreateStringReference("id-12345");
    cJSON_AddItemToObject(root, "id", id_ref);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t snapshot_len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(snapshot_len + 1);
    memset(buffer, 0, snapshot_len + 1);
    memcpy(buffer, snapshot, snapshot_len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_version_item = cJSON_GetObjectItemCaseSensitive(parsed, "version");
    const char *parsed_version_str = cJSON_GetStringValue(parsed_version_item);
    cJSON *copied_version = cJSON_CreateString(parsed_version_str);
    cJSON_AddItemToObject(parsed, "version_copy", copied_version);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}