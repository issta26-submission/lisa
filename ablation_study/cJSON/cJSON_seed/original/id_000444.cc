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
//<ID> 444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"example\",\"value\":\"hello\"}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);

    // step 2: Configure
    cJSON_AddNullToObject(root, "removed_item");
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    const char *value_text = cJSON_GetStringValue(value_item);
    cJSON *value_copy = cJSON_CreateString(value_text);
    cJSON_AddItemToObject(root, "value_copy", value_copy);

    // step 3: Operate
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "removed_item");

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}