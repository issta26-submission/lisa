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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"original\",\"value\":42,\"removeMe\":true}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *new_name = cJSON_CreateString("replaced");

    // step 2: Configure
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "name", new_name);

    // step 3: Operate and Validate
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "removeMe");
    cJSON *removed_item = cJSON_GetObjectItem(root, "removeMe");
    int removed_was_gone = (removed_item == NULL);
    int summary = (int)replaced + (int)value_num + removed_was_gone;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}