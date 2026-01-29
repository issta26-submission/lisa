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
//<ID> 1068
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
    cJSON *version_item = cJSON_CreateString(version_str);
    cJSON *metadata = cJSON_AddObjectToObject(root, "metadata");

    // step 2: Configure & Operate
    cJSON_AddItemToObjectCS(root, "version", version_item);
    const char *retrieved_version = cJSON_GetStringValue(version_item);
    cJSON *version_copy = cJSON_CreateString(retrieved_version);
    cJSON_AddItemToObjectCS(metadata, "version_copy", version_copy);
    cJSON *origin_item = cJSON_CreateString("generated_at_runtime");
    cJSON_AddItemToObjectCS(metadata, "origin", origin_item);

    // step 3: Validate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    const char *copied_value = cJSON_GetStringValue(version_copy);
    cJSON *copied_snapshot = cJSON_CreateString(copied_value);
    cJSON_AddItemToObjectCS(root, "copied_snapshot", copied_snapshot);
    char *snapshot_print = cJSON_PrintUnformatted(copied_snapshot);
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(snapshot_print);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}