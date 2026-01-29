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
//<ID> 490
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[1,2,3],\"meta\":{}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_array = cJSON_IsArray(items);
    int orig_size = cJSON_GetArraySize(items);

    // step 2: Configure
    cJSON_AddNullToObject(root, "deleted");
    cJSON_AddFalseToObject(root, "enabled");
    cJSON *new_num = cJSON_CreateNumber(10.0 + (double)items_is_array);
    cJSON_AddItemToArray(items, new_num);
    cJSON_AddNumberToObject(root, "size_before", (double)orig_size);

    // step 3: Operate
    int new_size = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "size_after", (double)new_size);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}