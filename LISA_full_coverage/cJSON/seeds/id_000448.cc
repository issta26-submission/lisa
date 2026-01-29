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
//<ID> 448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"demo\",\"value\":\"v1\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", copy);
    cJSON_AddNullToObject(root, "nullable");
    char *snapshot_before = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "value");
    char *snapshot_after = cJSON_PrintBuffered(root, 128, 0);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_after);
    cJSON_Delete(root);
    (void)name_str;
    return 66; // API sequence test completed successfully
}