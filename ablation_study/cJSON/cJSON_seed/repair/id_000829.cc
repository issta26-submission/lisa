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
//<ID> 829
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_AddObjectToObject(root, "metrics");
    cJSON *n_pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(metrics, "pi", n_pi);
    cJSON_AddNumberToObject(metrics, "count", 42.0);

    // step 2: Configure
    const char *json_text = "{\"name\":\"deviceA\",\"value\":77,\"info\":{\"note\":\"ok\"}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    (void)name_is_str;
    cJSON *detached_name = cJSON_DetachItemViaPointer(parsed, name_item);
    cJSON_AddItemToObject(root, "device_name", detached_name);
    cJSON *detached_value = cJSON_DetachItemFromObject(parsed, "value");
    cJSON_AddItemToObject(metrics, "imported_value", detached_value);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}