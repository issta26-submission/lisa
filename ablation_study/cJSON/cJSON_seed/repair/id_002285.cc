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
//<ID> 2285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "enabled", flag_false);
    cJSON *name = cJSON_CreateString("device-01");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *count = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(root, "count", count);
    cJSON *null_item = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(settings, "flag", true_item);
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(settings, "threshold", threshold);

    // step 3: Operate
    cJSON_bool optional_is_null = cJSON_IsNull(null_item);
    cJSON_bool enabled_is_false = cJSON_IsFalse(flag_false);
    cJSON_bool flag_is_bool = cJSON_IsBool(true_item);
    cJSON *num_optional_is_null = cJSON_CreateNumber((double)optional_is_null);
    cJSON_AddItemToObject(root, "optional_is_null", num_optional_is_null);
    cJSON *num_enabled_is_false = cJSON_CreateNumber((double)enabled_is_false);
    cJSON_AddItemToObject(root, "enabled_is_false", num_enabled_is_false);
    cJSON *num_flag_is_bool = cJSON_CreateNumber((double)flag_is_bool);
    cJSON_AddItemToObject(root, "flag_is_bool", num_flag_is_bool);
    cJSON *settings_copy = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "settings_copy", settings_copy);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}