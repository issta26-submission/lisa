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
//<ID> 517
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"fuzz\",\"enabled\":true,\"count\":2}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, (size_t)(sizeof(initial_json) - 1), &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *conf_flag = cJSON_AddBoolToObject(root, "configured", 1);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.5));
    (void)conf_flag;

    // step 3: Operate and Validate
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    cJSON_AddBoolToObject(root, "enabled_present", has_enabled);
    char *snapshot_str = cJSON_PrintUnformatted(root);
    cJSON *snapshot = cJSON_CreateString(snapshot_str);
    cJSON_AddItemToObject(root, "snapshot", snapshot);
    cJSON_free(snapshot_str);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}