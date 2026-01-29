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
//<ID> 744
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[10,20,30]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int orig_size = cJSON_GetArraySize(items);
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag_false);
    cJSON_AddNullToObject(root, "marker");
    cJSON_AddItemToObject(root, "items_copy", dup_items);

    // step 3: Operate
    int dup_size = cJSON_GetArraySize(dup_items);
    cJSON_AddNumberToObject(root, "orig_size", (double)orig_size);
    cJSON_AddNumberToObject(root, "dup_size", (double)dup_size);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}