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
//<ID> 746
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"items\":[1,2,3],\"meta\":{\"tag\":\"initial\"}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int initial_count = cJSON_GetArraySize(items);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);
    cJSON_AddNullToObject(root, "terminated");

    // step 3: Operate
    cJSON_AddItemToArray(items, cJSON_CreateNumber((double)initial_count));
    int final_count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "final_size", (double)final_count);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}