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
//<ID> 727
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"status\":\"ok\",\"count\":3,\"items\":[1,2,3]}";
    const char *parse_end1 = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end1, 1);
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    const char *status_str = cJSON_GetStringValue(status_item);
    cJSON *items = cJSON_GetObjectItem(root, "items");

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);
    int item_count = cJSON_GetArraySize(items);
    cJSON *count_marker = cJSON_CreateNumber((double)item_count);
    cJSON_AddItemToObject(root, "computed_count", count_marker);
    cJSON *status_copy = cJSON_CreateString(status_str);
    cJSON_AddItemToObject(root, "status_copy", status_copy);

    // step 3: Operate
    char scratch[256];
    memset(scratch, 0, sizeof(scratch));
    char *printed = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Validate & Cleanup
    const char *parse_end2 = NULL;
    cJSON *root2 = cJSON_ParseWithOpts(printed, &parse_end2, 1);
    cJSON *status2 = cJSON_GetObjectItem(root2, "status");
    char *reprint = cJSON_PrintBuffered(status2 ? status2 : root2, 64, 0);
    cJSON_free(printed);
    cJSON_free(reprint);
    cJSON_Delete(root2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}