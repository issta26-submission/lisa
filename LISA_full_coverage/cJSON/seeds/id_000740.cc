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
//<ID> 740
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"example\",\"items\":[1,2,3],\"meta\":{\"valid\":true}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int original_size = cJSON_GetArraySize(items);
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "items_copy", dup_items);
    cJSON_AddItemToObject(root, "enabled", flag_false);
    cJSON_AddNullToObject(root, "optional");

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    cJSON *items_copy = cJSON_GetObjectItem(root, "items_copy");
    int final_size = cJSON_GetArraySize(items_copy);
    cJSON_AddNumberToObject(root, "final_size", (double)final_size);
    (void)original_size; // keep original_size meaningfully part of flow (recorded earlier)
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}