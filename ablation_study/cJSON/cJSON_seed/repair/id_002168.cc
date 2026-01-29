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
//<ID> 2168
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
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *enabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *v1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(values, v1);
    cJSON *v2 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(values, v2);

    // step 2: Configure
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);
    cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);

    // step 3: Operate and Validate
    cJSON *enabled_check = cJSON_CreateBool(enabled_is_bool);
    cJSON_AddItemToObject(root, "enabled_check", enabled_check);
    char *name_val = cJSON_GetStringValue(name_item);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (name_val && name_val[0]) ? name_val[0] : '\0';
    buffer[1] = (json && json[0]) ? json[0] : '\0';
    buffer[2] = (json && json[1]) ? json[1] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "values_copy");
    cJSON_DeleteItemFromObject(root, "enabled_check");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}