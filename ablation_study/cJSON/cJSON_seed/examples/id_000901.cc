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
//<ID> 901
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = " { \"sensor\": { \"active\": true }, \"id\": null } ";
    cJSON_Minify(json);
    cJSON *parsed = cJSON_Parse(json);
    cJSON *sensor = cJSON_GetObjectItem(parsed, "sensor");
    cJSON *active_item = cJSON_GetObjectItem(sensor, "active");

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_item);

    // step 3: Operate & Validate
    cJSON_bool active_is_true = cJSON_IsTrue(active_item);
    cJSON_AddBoolToObject(root, "enabled", active_is_true);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}