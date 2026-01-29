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
//<ID> 555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *num_node = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "value", num_node);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *null_node = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_node);
    cJSON_AddStringToObject(root, "name", "example");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    char *name_str = cJSON_GetStringValue(parsed_name);
    cJSON *parsed_value = cJSON_GetObjectItem(parsed, "value");
    double value = cJSON_GetNumberValue(parsed_value);
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool enabled_flag = cJSON_IsTrue(parsed_enabled);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;name=%s;value=%.2f;enabled=%d",
            version ? version : "null",
            name_str ? name_str : "null",
            value,
            (int)enabled_flag);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}