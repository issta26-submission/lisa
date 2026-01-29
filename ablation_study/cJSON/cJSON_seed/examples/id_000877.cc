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
//<ID> 877
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *value_item = cJSON_CreateNumber(123.0);
    cJSON *name_item = cJSON_CreateString("device-123");

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "value", value_item);

    // step 3: Operate & Validate
    cJSON *got_name = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON *got_value = cJSON_GetObjectItem(root, "value");
    cJSON_bool value_is_number = cJSON_IsNumber(got_value);
    cJSON_bool value_is_invalid = cJSON_IsInvalid(got_value);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON *is_number_item = cJSON_CreateNumber((double)value_is_number);
    cJSON *is_invalid_item = cJSON_CreateNumber((double)value_is_invalid);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON_AddItemToObject(root, "value_is_number", is_number_item);
    cJSON_AddItemToObject(root, "value_is_invalid", is_invalid_item);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}