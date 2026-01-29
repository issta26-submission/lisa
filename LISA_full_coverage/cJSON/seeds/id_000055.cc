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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_str[] = "{\"name\":\"initial\",\"value\":42,\"nested\":{\"keep\":true}}";
    size_t json_len = sizeof(json_str) - 1;
    cJSON *root = cJSON_ParseWithLength(json_str, json_len);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_bool replaced_name = cJSON_ReplaceItemInObject(root, "name", new_name);
    cJSON *new_value = cJSON_CreateNumber(100.0);
    cJSON_bool replaced_value = cJSON_ReplaceItemInObject(root, "value", new_value);

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(got_name);
    cJSON *got_value = cJSON_GetObjectItem(root, "value");
    double value_val = cJSON_GetNumberValue(got_value);
    int summary = 0;
    summary += (int)replaced_name;
    summary += (int)replaced_value;
    summary += (int)(name_val != NULL);
    summary += (int)value_val;
    (void)summary;
    (void)name_val;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "nested");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}