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
//<ID> 2288
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
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(data, "enabled", flag_false);
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe_null");

    // step 2: Configure
    cJSON_AddNumberToObject(data, "threshold", 3.1415);
    cJSON_AddStringToObject(data, "name", "device-1");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddBoolToObject(meta, "ok", 0);

    // step 3: Operate and Validate
    cJSON *got_enabled = cJSON_GetObjectItem(data, "enabled");
    cJSON_bool enabled_is_false = cJSON_IsFalse(got_enabled);
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON_bool maybe_is_null = cJSON_IsNull(maybe);
    cJSON *n1 = cJSON_CreateNumber((double)enabled_is_false);
    cJSON *n2 = cJSON_CreateNumber((double)enabled_is_bool);
    cJSON *n3 = cJSON_CreateNumber((double)maybe_is_null);
    cJSON_AddItemToObject(root, "enabled_is_false", n1);
    cJSON_AddItemToObject(root, "enabled_is_bool", n2);
    cJSON_AddItemToObject(root, "maybe_is_null", n3);
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    cJSON *compare_node = cJSON_CreateNumber((double)equal);
    cJSON_AddItemToObject(root, "equal", compare_node);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}