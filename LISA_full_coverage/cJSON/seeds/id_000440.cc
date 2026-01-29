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
//<ID> 440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"username\":\"alice\",\"age\":30,\"features\":[\"a\",\"b\"]}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *username_item = cJSON_GetObjectItem(root, "username");
    char *username = cJSON_GetStringValue(username_item);

    // step 2: Configure
    cJSON *null_added = cJSON_AddNullToObject(root, "deprecated");
    (void)null_added;
    char *snapshot_before = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "age");
    char *snapshot_after = cJSON_PrintBuffered(root, 128, 0);
    (void)username;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_after);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}