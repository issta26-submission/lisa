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
//<ID> 2287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_AddItemToObject(flags, "enabled", enabled);
    cJSON *disabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(flags, "disabled", disabled);
    cJSON *maybe = cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("device-1");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate
    cJSON_bool got_null = cJSON_IsNull(maybe);
    cJSON_bool got_bool = cJSON_IsBool(enabled);
    cJSON_bool was_false = cJSON_IsFalse(disabled);
    cJSON *null_flag = cJSON_CreateNumber((double)got_null);
    cJSON_AddItemToObject(root, "maybe_is_null", null_flag);
    cJSON *bool_flag = cJSON_CreateNumber((double)got_bool);
    cJSON_AddItemToObject(root, "enabled_is_bool", bool_flag);
    cJSON *false_flag = cJSON_CreateNumber((double)was_false);
    cJSON_AddItemToObject(root, "disabled_is_false", false_flag);

    // step 4: Validate and Cleanup
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)prealloc_ok;
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}