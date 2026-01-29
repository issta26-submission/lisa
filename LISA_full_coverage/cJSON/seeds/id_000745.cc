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
//<ID> 745
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"example\",\"items\":[10,20],\"meta\":{\"count\":2}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int original_size = cJSON_GetArraySize(items);
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", flag_false);
    cJSON_AddNullToObject(root, "deprecated");
    cJSON_AddItemToObject(root, "meta_copy", meta_dup);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int final_size = cJSON_GetArraySize(items);
    cJSON *final_count = cJSON_CreateNumber((double)final_size);
    cJSON_AddItemToObject(root, "final_count", final_count);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}