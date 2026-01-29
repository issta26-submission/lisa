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
//<ID> 2282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *null_node = cJSON_AddNullToObject(root, "optional");
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(data, "enabled", flag);
    cJSON *name = cJSON_CreateString("device-7");
    cJSON_AddItemToObject(data, "name", name);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(meta, "count", count);
    cJSON *tag = cJSON_CreateString("v1");
    cJSON_AddItemToObject(meta, "tag", tag);

    // step 3: Operate and Validate
    cJSON_bool optional_is_null = cJSON_IsNull(null_node);
    cJSON *got_enabled = cJSON_GetObjectItem(data, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON_bool enabled_is_false = cJSON_IsFalse(got_enabled);
    cJSON *validation = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "validation", validation);
    cJSON_AddItemToObject(validation, "optional_is_null", cJSON_CreateBool(optional_is_null));
    cJSON_AddItemToObject(validation, "enabled_is_bool", cJSON_CreateBool(enabled_is_bool));
    cJSON_AddItemToObject(validation, "enabled_is_false", cJSON_CreateBool(enabled_is_false));
    cJSON *score = cJSON_CreateNumber((double)optional_is_null + (double)enabled_is_bool + (double)enabled_is_false);
    cJSON_AddItemToObject(validation, "score", score);

    // step 4: Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}