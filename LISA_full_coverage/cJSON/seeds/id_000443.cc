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
//<ID> 443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"tester\",\"value\":42,\"remove_me\":\"bye\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON_AddStringToObject(root, "copied_name", name_str);
    cJSON *optional = cJSON_AddNullToObject(root, "optional");
    (void)optional;

    // step 3: Operate
    char *snapshot_before = cJSON_PrintBuffered(root, 256, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove_me");
    char *snapshot_after = cJSON_PrintBuffered(root, 128, 0);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_after);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}