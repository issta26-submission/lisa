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
//<ID> 875
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("device-123");
    cJSON *value_item = cJSON_CreateNumber(123.45);
    cJSON *version_item = cJSON_CreateString(cJSON_Version());

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddItemToObject(root, "value", value_item);
    cJSON_AddItemToObject(root, "version", version_item);

    // step 3: Operate & Validate
    cJSON *fetched_value = cJSON_GetObjectItem(root, "value");
    cJSON_bool value_is_number = cJSON_IsNumber(fetched_value);
    cJSON *value_is_number_item = cJSON_CreateNumber((double)value_is_number);
    cJSON_AddItemToObject(root, "value_is_number", value_is_number_item);
    cJSON_bool value_is_invalid = cJSON_IsInvalid(fetched_value);
    cJSON *value_is_invalid_item = cJSON_CreateNumber((double)value_is_invalid);
    cJSON_AddItemToObject(root, "value_is_invalid", value_is_invalid_item);
    cJSON *fetched_name = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(fetched_name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}