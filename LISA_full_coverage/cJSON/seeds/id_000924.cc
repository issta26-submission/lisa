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
//<ID> 924
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"active\": false, \"count\": 3}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON_bool active_is_false = cJSON_IsFalse(active_item);
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddBoolToObject(cfg, "from_parsed_active_is_false", active_is_false);
    cJSON_AddItemToObject(root, "cfg", cfg);

    // step 3: Operate
    cJSON *dup_cfg = cJSON_Duplicate(cfg, 1);
    char *printed = cJSON_PrintBuffered(dup_cfg, 256, 1);
    void *prebuf = cJSON_malloc(128);
    memset(prebuf, 0, 128);
    cJSON_free(prebuf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_cfg);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}