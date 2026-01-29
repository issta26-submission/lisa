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
//<ID> 902
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    char json_buf[] = " { \"sensor\": \"A\", \"value\": 42, \"active\": true } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "device", dup);

    // step 3: Operate & Validate
    cJSON *device = cJSON_GetObjectItem(root, "device");
    cJSON *value_item = cJSON_GetObjectItem(device, "value");
    double original_value = cJSON_GetNumberValue(value_item);
    double doubled_value = cJSON_SetNumberHelper(value_item, original_value * 2.0);
    cJSON *doubled_number = cJSON_CreateNumber(doubled_value);
    cJSON_AddItemToObject(root, "value_doubled", doubled_number);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}