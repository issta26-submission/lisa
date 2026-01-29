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
//<ID> 726
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"status\":\"ok\",\"count\":3}";
    const char *parse_end1 = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end1, 1);
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    cJSON *count_item = cJSON_GetObjectItem(root, "count");

    // step 2: Configure
    const char *status_str = cJSON_GetStringValue(status_item);
    cJSON *status_copy = cJSON_CreateString(status_str);
    cJSON_AddItemToObject(root, "status_copy", status_copy);
    cJSON *enabled_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled_false);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    const char *parse_end2 = NULL;
    cJSON *root2 = cJSON_ParseWithOpts(printed, &parse_end2, 1);
    cJSON *enabled_copy = cJSON_GetObjectItem(root2, "enabled");
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    (void)enabled_copy;
    (void)count_item;
    (void)status_item;
    cJSON_Delete(root2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}