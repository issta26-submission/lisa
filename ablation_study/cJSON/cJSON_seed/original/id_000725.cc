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
//<ID> 725
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"config\":{\"enabled\":true,\"threshold\":3.14},\"name\":\"example\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *enabled = cJSON_GetObjectItem(config, "enabled");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_ReplaceItemInObject(config, "enabled", false_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON_AddItemToObject(root, "answer", cJSON_CreateNumber(42.0));

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}