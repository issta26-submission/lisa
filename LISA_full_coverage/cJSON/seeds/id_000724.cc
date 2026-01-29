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
//<ID> 724
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"status\":\"active\",\"value\":42.5}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    const char *status_str = cJSON_GetStringValue(status_item);
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    double value_num = cJSON_GetNumberValue(value_item);

    // step 2: Configure
    cJSON *status_copy = cJSON_CreateString(status_str);
    cJSON_AddItemToObject(root, "status_copy", status_copy);
    cJSON *explicit_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", explicit_false);

    // step 3: Operate
    cJSON *double_value = cJSON_CreateNumber(value_num * 2.0);
    cJSON_AddItemToObject(root, "double_value", double_value);
    char *printed = cJSON_PrintBuffered(root, 128, 1);
    cJSON_free((void *)printed);

    // step 4: Validate & Cleanup
    cJSON *enabled_check = cJSON_GetObjectItem(root, "enabled");
    (void)enabled_check;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}