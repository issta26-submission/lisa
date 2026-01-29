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
//<ID> 820
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(config, "version", 2.5);
    cJSON *num_item = cJSON_CreateNumber(100.0);
    cJSON_AddItemToArray(values, num_item);

    // step 2: Configure
    const char *json_text = "{\"user\":{\"name\":\"alice\",\"id\":42},\"misc\":\"temporary\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *user = cJSON_GetObjectItem(parsed, "user");
    cJSON *name = cJSON_GetObjectItem(user, "name");

    // step 3: Operate and Validate
    cJSON *misc_detached = cJSON_DetachItemFromObject(parsed, "misc");
    cJSON *name_detached = cJSON_DetachItemViaPointer(user, name);
    cJSON_AddItemToObject(root, "imported_name", name_detached);
    cJSON_bool name_is_string = cJSON_IsString(name_detached);
    (void)name_is_string;
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(misc_detached);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}