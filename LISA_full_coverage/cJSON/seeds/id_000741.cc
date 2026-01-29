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
//<ID> 741
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"tester\",\"items\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int size_before = cJSON_GetArraySize(items);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled);

    // step 3: Operate
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_dup);
    cJSON *marker = cJSON_AddNullToObject(root, "marker");
    int size_dup = cJSON_GetArraySize(items_dup);

    // step 4: Validate & Cleanup
    cJSON *retrieved_copy = cJSON_GetObjectItem(root, "items_copy");
    int final_size = cJSON_GetArraySize(retrieved_copy);
    (void)size_before;
    (void)size_dup;
    (void)final_size;
    (void)marker;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}