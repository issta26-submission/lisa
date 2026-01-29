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
//<ID> 2284
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
    cJSON *enabled_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "enabled", enabled_false);
    cJSON_AddNullToObject(meta, "optional");
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(data, "count", cJSON_CreateNumber(3.0));
    cJSON_AddItemToObject(data, "name", cJSON_CreateString("device-A"));

    // step 2: Configure
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *opt_node = cJSON_GetObjectItem(meta_ref, "optional");
    cJSON *enabled_node = cJSON_GetObjectItem(meta_ref, "enabled");
    cJSON_bool opt_is_null = cJSON_IsNull(opt_node);
    cJSON_bool enabled_is_false = cJSON_IsFalse(enabled_node);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_node);

    // step 3: Operate
    cJSON_AddItemToObject(root, "optional_is_null", cJSON_CreateNumber((double)opt_is_null));
    cJSON_AddItemToObject(root, "enabled_is_false", cJSON_CreateNumber((double)enabled_is_false));
    cJSON_AddItemToObject(root, "enabled_is_bool", cJSON_CreateNumber((double)enabled_is_bool));
    char *unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    (void)printed;
    (void)unformatted;
    cJSON_free(buffer);
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}