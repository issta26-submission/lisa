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
//<ID> 721
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"status\":\"ok\",\"count\":2,\"items\":[10,20]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    const char *status_str = cJSON_GetStringValue(status_item);

    // step 2: Configure
    cJSON *active_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", active_false);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int initial_count = cJSON_GetArraySize(items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "status_copy", cJSON_CreateString(status_str));
    cJSON_AddItemToObject(meta, "initial_count", cJSON_CreateNumber((double)initial_count));
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON *active_check = cJSON_GetObjectItem(root, "active");
    cJSON *status_check = cJSON_GetObjectItem(root, "status");

    // step 4: Validate & Cleanup
    int final_count = cJSON_GetArraySize(items);
    cJSON_AddItemToObject(root, "final_count", cJSON_CreateNumber((double)final_count));
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}